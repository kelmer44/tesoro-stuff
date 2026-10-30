// Compares the engine's ScreenDisplay::compose with vectors recorded from the original code
// (see re/tools/gen_vectors_compose.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/animbank.h"
#define private public
#include "tesoro/core/screendisplay.h"
#undef private
#include <zlib.h>

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

class VecStream : public Common::SeekableReadStream {
public:
	VecStream(const std::string &d) : _d(d), _p(0), _eos(false) {}
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override {
		const uint32 avail = (uint32)(_d.size() - _p), got = n < avail ? n : avail;
		memcpy(out, _d.data() + _p, got); _p += got; if (got < n) _eos = true; return got;
	}
	int64 pos() const override { return (int64)_p; }
	int64 size() const override { return (int64)_d.size(); }
	bool seek(int64 off, int whence = SEEK_SET) override {
		int64 np = whence == SEEK_SET ? off : whence == SEEK_CUR ? (int64)_p + off : (int64)_d.size() + off;
		if (np < 0 || np > (int64)_d.size()) return false;
		_p = (size_t)np; _eos = false; return true;
	}
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
private:
	std::string _d; size_t _p; bool _eos;
};

static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer player; VecStream *stream = nullptr;
	Tesoro::ScreenDisplay *screen = new Tesoro::ScreenDisplay();
	int caseNo = 0, frames = 0, badFrames = 0, rows = 0, badRows = 0, shown = 0;
	bool loaded = false;
	std::vector<uLong> want;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op == "F") {
			want.clear(); unsigned long c; while (in >> c) want.push_back(c);
			++frames; bool ok = want.size() == 480;
			for (uint y = 0; ok && y < 480; ++y) {
				++rows;
				if (crc32(0, (const Bytef *)(screen->frame() + 640 * y), 1280) != want[y]) { ok = false; ++badRows; if (shown++ < 5) printf("mismatch case %d row %u\n", caseNo, y); }
			}
			if (!ok) ++badFrames;
		} else if (op == "C") {
			++caseNo; std::string hex; unsigned entry; in >> hex >> entry;
			delete stream; stream = new VecStream(unhex(hex)); player = Tesoro::AnimPlayer();
			loaded = player.load(*stream, entry, 0, false);
		} else if (!loaded) {
			continue;
		} else if (op == "Z") {
			screen->startFrame();
		} else if (op == "S") {
			unsigned k; int x, y; unsigned depth; in >> k >> x >> y >> depth;
			screen->drawImage(x, y, (uint16)depth, player.bank()->image(k), 0);
		} else if (op == "K") {
			for (uint i = 0; i < screen->_frame.size(); ++i) screen->_frame[i] = 0xabab;
			screen->compose();
		}
	}
	printf("%-28s %d/%d match\n", "composed frames", frames - badFrames, frames);
	printf("%-28s %d/%d match\n", "frame rows", rows - badRows, rows);
	return (badFrames || badRows) ? 1 : 0;
}
