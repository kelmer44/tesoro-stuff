// Saves the object records of every shipped level after changing flags and animation state, loads them into a fresh
// copy of the same level and compares. Usage: roundtrip_driver <TESORO dir>. Not a comparison with the original.
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

class BufWriter : public Common::WriteStream {
public:
	std::string data;
	uint32 write(const void *p, uint32 n) override { data.append((const char *)p, n); return n; }
	int64 pos() const override { return (int64)data.size(); }
};
class BufReader : public Common::ReadStream {
public:
	BufReader(const std::string &d) : _d(d), _p(0), _eos(false) {}
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override {
		size_t got = _d.size() - _p < n ? _d.size() - _p : n; memcpy(out, _d.data() + _p, got); _p += got; if (got < n) _eos = true; return (uint32)got;
	}
	size_t pos() const { return _p; }
private:
	const std::string &_d; size_t _p; bool _eos;
};

static std::string dump(Tesoro::Level &lv) {
	BufWriter out;
	lv.saveRecords(out, nullptr);
	return out.data;
}

int main(int argc, char **argv) {
	std::string dir = argv[1];
	FILE *tre = fopen((dir + "/TESORO.TRE").c_str(), "rb"), *dat = fopen((dir + "/TESORO.DAT").c_str(), "rb");
	if (!tre || !dat) return 2;
	FileStream ds(dat);
	unsigned char rec[28]; int levels = 0, objects = 0, bad = 0; unsigned seed = 7;
	while (fread(rec, 28, 1, tre) == 1) {
		if (!(rec[8] & 4)) continue;
		std::string nm((char *)rec + 9, strnlen((char *)rec + 9, 19));
		for (auto &c : nm) c = (char)toupper((unsigned char)c);
		if (nm.size() < 4 || nm.substr(nm.size() - 4) != ".LVL") continue;
		uint32 off = rec[0] | rec[1] << 8 | rec[2] << 16 | (uint32)rec[3] << 24;
		Tesoro::Level a, b;
		if (!a.load(ds, off + 4, 0, false, 0) || !b.load(ds, off + 4, 0, false, 0)) continue;
		// Change the first level's objects: flags and a different animation range.
		for (uint32 i = 0; i < a.objects().count(); ++i) {
			Tesoro::Object *o = a.objects().valueAt(i);
			seed = seed * 1103515245u + 12345u;
			o->setFlags((o->flags() & ~0x6u) | ((seed >> 16) & 6));
			if ((o->flags() & Tesoro::Object::kHasAnimation) && o->animation().bankCount())
				o->animation().selectBank(0, 0, 0, (seed >> 20) & 1, false, (int32)(seed >> 8) & 0xfff);
			++objects;
		}
		const std::string saved = dump(a);
		BufReader in(saved);
		bool ok = b.loadRecords(in) && in.pos() == saved.size();
		const bool same = ok && dump(b) == saved;
		printf("%-14s %3u objects, %zu bytes: %s\n", nm.c_str(), (unsigned)a.objects().count(), saved.size(), same ? "round trip identical" : "DIFFERENT");
		if (!same) {
			int dup = 0;
			for (uint32 i = 0; i < a.objects().count(); ++i)
				for (uint32 j = i + 1; j < a.objects().count(); ++j)
					dup += a.objects().valueAt(i)->nameKey() == a.objects().valueAt(j)->nameKey();
			printf("  objects sharing a name key: %d\n", dup);
		}
		bad += !same; ++levels;
	}
	printf("%d levels, %d objects, %d problems\n", levels, objects, bad);
	return bad != 0;
}
