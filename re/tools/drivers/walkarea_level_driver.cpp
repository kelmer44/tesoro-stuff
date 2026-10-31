// Loads every level of the shipped archive and checks that its floor (kind 7) is found through its kind 1 object and that the floor finds its route (kind 8).
// Usage: walkarea_level_driver <TESORO dir>   (read-only; see re/tools/count_level_kinds.py for the index walk)
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#include "tesoro/core/level.h"
#include "tesoro/core/walkfloor.h"
#include "tesoro/core/pathobject.h"
#include "tesoro/core/cameraframe.h"
#include "tesoro/core/namekey.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

class FileStream : public Common::SeekableReadStream {
public:
	FileStream(FILE *f) : _f(f), _eos(false) { fseek(f, 0, SEEK_END); _size = ftell(f); fseek(f, 0, SEEK_SET); }
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override { size_t g = fread(out, 1, n, _f); if (g < n) _eos = true; return (uint32)g; }
	int64 pos() const override { return ftell(_f); }
	int64 size() const override { return _size; }
	bool seek(int64 off, int whence = SEEK_SET) override { _eos = false; return fseek(_f, (long)off, whence) == 0; }
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
private:
	FILE *_f; bool _eos; long _size;
};

struct Peek : Tesoro::Level {
	std::string floorName;
protected:
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &r, Tesoro::Room &room) override {
		if (r.kind == 8) floorName = r.name;
		return Tesoro::Level::createObject(r, room);
	}
};

int main(int argc, char **argv) {
	if (argc < 2) return 1;
	std::string dir = argv[1];
	FILE *tre = fopen((dir + "/TESORO.TRE").c_str(), "rb"), *dat = fopen((dir + "/TESORO.DAT").c_str(), "rb");
	if (!tre || !dat) return 2;
	FileStream ds(dat);
	unsigned char rec[28];
	int levels = 0, floors = 0, withPath = 0, kind8 = 0, noFloor = 0, noPath = 0;
	while (fread(rec, 28, 1, tre) == 1) {
		if (!(rec[8] & 4)) continue;
		std::string nm((char *)rec + 9, strnlen((char *)rec + 9, 19));
		for (auto &c : nm) c = (char)toupper((unsigned char)c);
		if (nm.size() < 4 || nm.substr(nm.size() - 4) != ".LVL") continue;
		uint32 off = rec[0] | rec[1] << 8 | rec[2] << 16 | (uint32)rec[3] << 24;
		Peek lv;
		if (!lv.load(ds, off + 4, 0, false, 0)) continue;
		lv.attachObjects();
		++levels;
		int n8 = 0;
		lv.objects().rewind();
		while (lv.objects().step()) if (lv.objects().value()->kindId() == 8) ++n8;
		kind8 += n8;
		Tesoro::WalkFloor *f = lv.floor();
		if (!f) { ++noFloor; if (n8) printf("%-14s kind 8 present but no floor\n", nm.c_str()); continue; }
		++floors;
		if (f->path()) ++withPath; else { ++noPath; printf("%-14s floor without a route\n", nm.c_str()); }
		if (getenv("DUMP")) printf("%-14s floor at (%d,%d), scale at its origin %d\n", nm.c_str(), (int)f->x(), (int)f->y(), (int)f->scaleAt(f->x(), f->y()));
	}
	printf("levels %d, levels with a floor %d (route found %d, none %d), levels without floor %d, kind 8 objects %d\n", levels, floors, withPath, noPath, noFloor, kind8);
	return 0;
}
