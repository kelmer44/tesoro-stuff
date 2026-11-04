// Compares the engine's drawing of the real levels (Level::updateRoomCameras and ScreenDisplay::compose) with the frames the original code drew
// (see re/tools/gen_vectors_realscenes.py). Usage: realscene_driver <vectors> <TESORO folder>. Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#include "tesoro/core/level.h"
#define private public
#include "tesoro/core/screendisplay.h"
#undef private
#include <zlib.h>

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *, ...) {}

struct FileStream : public Common::SeekableReadStream {
	FileStream(FILE *f) : _f(f), _eos(false) { fseek(f, 0, SEEK_END); _size = ftell(f); fseek(f, 0, SEEK_SET); }
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override { size_t g = fread(out, 1, n, _f); if (g < n) _eos = true; return (uint32)g; }
	int64 pos() const override { return ftell(_f); }
	int64 size() const override { return _size; }
	bool seek(int64 off, int whence = SEEK_SET) override { _eos = false; return fseek(_f, (long)off, whence) == 0; }
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
private: FILE *_f; bool _eos; long _size;
};

/** Kinds other than 0, 1 and 3 are built as the plain object, as the original's run was told to. */
struct CoercedLevel : Tesoro::Level {
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &record, Tesoro::Room &room) override {
		Tesoro::ObjectRecord r = record;
		if (r.kind != 0 && r.kind != 1 && r.kind != 3) r.kind = 0;
		return Tesoro::Level::createObject(r, room);
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	FILE *dat = fopen((std::string(argv[2]) + "/TESORO.DAT").c_str(), "rb");
	if (!f || !dat) { fprintf(stderr, "no vectors or data\n"); return 2; }
	FileStream ds(dat);
	static char buf[1 << 20];
	Tesoro::ScreenDisplay *screen = new Tesoro::ScreenDisplay();
	CoercedLevel *lv = nullptr; bool loaded = false;
	std::string name; int frame = 0, bad = 0, levels = 0, rows = 0, badRows = 0, t = 0, shown = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op == "R") {
			unsigned entry; in >> name >> entry; ++levels;
			delete lv; lv = new CoercedLevel();
			loaded = lv->load(ds, entry, 0, false, 0);
			if (loaded) lv->attachObjects();
		} else if (op == "T") {
			in >> t;
			for (uint i = 0; i < screen->_frame.size(); ++i) screen->_frame[i] = 0xabab;
			screen->startFrame();
			if (loaded) lv->updateRoomCameras(t, *screen);
			screen->compose();
		} else if (op == "F") {
			std::vector<unsigned long> want; unsigned long c; while (in >> c) want.push_back(c);
			++frame; bool ok = loaded && want.size() == 480; int badHere = 0; int firstBad = -1;
			for (uint y = 0; ok && y < 480; ++y) {
				++rows;
				if (crc32(0, (const Bytef *)(screen->frame() + 640 * y), 1280) != want[y]) { ++badRows; ++badHere; if (firstBad < 0) firstBad = (int)y; }
			}
			if (badHere || !ok) { ++bad; if (shown++ < 12) printf("mismatch in %s at time %d: %d rows differ (first row %d)\n", name.c_str(), t, badHere, firstBad); }
		}
	}
	printf("real scenes                  %d levels, %d/%d frames match, %d/%d rows match\n", levels, frame - bad, frame, rows - badRows, rows);
	return bad != 0;
}
