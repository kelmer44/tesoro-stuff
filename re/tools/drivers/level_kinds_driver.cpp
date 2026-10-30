// Tallies the object kinds of every level in the shipped archive, using the engine's Level loader.
// Usage: level_kinds_driver <TESORO dir>   (read-only; see re/tools/count_level_kinds.py for the index walk)
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

struct Tally : Tesoro::Level {
	std::map<unsigned, int> kinds;
protected:
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &r, Tesoro::Room &room) override {
		kinds[r.kind]++;
		return Tesoro::Level::createObject(r, room);
	}
};

int main(int argc, char **argv) {
	if (argc < 2) return 1;
	std::string dir = argv[1];
	FILE *tre = fopen((dir + "/TESORO.TRE").c_str(), "rb"), *dat = fopen((dir + "/TESORO.DAT").c_str(), "rb");
	if (!tre || !dat) return 2;
	FileStream ds(dat);
	std::map<std::string, unsigned> all; std::map<unsigned, unsigned> alltypes;
	unsigned char rec[28];
	while (fread(rec, 28, 1, tre) == 1) {
		if (!(rec[8] & 4)) continue;
		std::string nm((char *)rec + 9, strnlen((char *)rec + 9, 19));
		for (auto &c : nm) c = (char)toupper((unsigned char)c);
		if (nm.size() < 4 || nm.substr(nm.size() - 4) != ".LVL") continue;
		uint32 off = rec[0] | rec[1] << 8 | rec[2] << 16 | (uint32)rec[3] << 24;
		Tally lv;
		if (!lv.load(ds, off + 4, 0, false, 0)) { printf("%s: load failed\n", nm.c_str()); continue; }
		printf("%-14s rooms %u banks %u objects %u |", nm.c_str(), lv.roomCount(), lv.bankCount(), (unsigned)lv.objects().count());
		for (auto &k : lv.kinds) { printf(" k%u:%d", k.first, k.second); all[std::to_string(k.first)] += k.second; }
		{
			std::map<unsigned, int> types; unsigned images = 0;
			for (uint b = 0; b < lv.bankCount(); ++b) {
				const Tesoro::AnimBank *bk = lv.banks()[b].bank();
				for (uint i = 0; i < bk->imageCount(); ++i) { ++images; if (bk->image(i).isLoaded()) types[bk->image(i).type()]++; }
			}
			printf(" | images %u:", images);
			for (auto &t : types) printf(" t%u:%d", t.first, t.second);
			for (auto &t : types) alltypes[t.first] += t.second;
		}
		printf("\n");
	}
	printf("TOTAL:");
	for (auto &k : all) printf(" k%s:%u", k.first.c_str(), k.second);
	printf("\nIMAGE TYPES:"); for (auto &t : alltypes) printf(" t%u:%u", t.first, t.second);
	printf("\n");
	return 0;
}
