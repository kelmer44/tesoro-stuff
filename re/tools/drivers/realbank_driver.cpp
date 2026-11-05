// Compares every animation bank of the real levels, as the engine loads it, with the banks the original's loader built (see re/tools/gen_vectors_realbanks.py).
// Usage: realbank_driver <vectors> <TESORO folder>. Build/run: re/tools/run_startup_check.sh
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
struct CoercedLevel : Tesoro::Level {
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &record, Tesoro::Room &room) override {
		Tesoro::ObjectRecord r = record; if (r.kind != 1) r.kind = 0; return Tesoro::Level::createObject(r, room);
	}
};
static std::string unhex(const std::string &s) {
	std::string r; if (s == "-") return r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}
static std::string toStr(const byte *p, size_t n) { return std::string((const char *)p, n); }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	FILE *dat = fopen((std::string(argv[2]) + "/TESORO.DAT").c_str(), "rb");
	if (!f || !dat) { fprintf(stderr, "no vectors or data\n"); return 2; }
	FileStream ds(dat);
	static char buf[1 << 20];
	CoercedLevel *lv = nullptr; bool loaded = false; int bank = -1, seq = 0;
	std::string name; int levels = 0, banks = 0, seqs = 0, bad = 0, shown = 0;
	std::map<std::string, int> badByLevel;
	int known = 0;
	auto fail = [&](const std::string &what) {
		// four single-point outlines of the two kitchen levels claim a point their data does not hold: the original reads what lies after it in memory
		if ((name == "COCINA.LVL" || name == "COCINA2.LVL") && bank == 1 && (seq == 1 || seq == 2) && what.compare(0, 6, "points") == 0) { ++known; return; }
		++bad; ++badByLevel[name]; if (shown++ < 12) printf("mismatch in %s bank %d sequence %d: %s\n", name.c_str(), bank, seq, what.c_str());
	};
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "R") {
			unsigned entry; in >> name >> entry; ++levels; delete lv; lv = new CoercedLevel();
			loaded = lv->load(ds, entry, 2, false, 0); if (!loaded) fail("load failed");
		} else if (!loaded) continue;
		else if (op == "B") { in >> bank; ++banks; seq = 0; }
		else if (op == "H") {
			uint32 nseq, nimg, h[3], flags; in >> nseq >> nimg >> h[0] >> h[1] >> h[2] >> flags;
			const Tesoro::AnimPlayer &p = lv->banks()[bank]; const Tesoro::AnimBank *b = p.bank();
			if (!(b->sequenceCount() == nseq && b->imageCount() == nimg && b->headerDword(0) == h[0] && b->headerDword(1) == h[1] && b->headerDword(2) == h[2] && p.flags() == flags)) fail("header");
		} else if (op == "S") {
			unsigned f0, dur, type, f0c, f10, cnt, fd; int x, y, idx; std::string ent, scl;
			in >> f0 >> x >> y >> dur >> type >> f0c >> f10 >> idx >> cnt >> fd >> ent >> scl;
			const Tesoro::AnimSequence &s = lv->banks()[bank].bank()->sequence(seq);
			std::string pe, ps;
			for (uint32 i = 0; i < s.entries.size(); ++i) { byte b4[4]; WRITE_LE_UINT32(b4, s.entries[i]); pe += toStr(b4, 4); }
			for (uint32 i = 0; i < s.scales.size(); ++i) { byte b2[2]; WRITE_LE_UINT16(b2, s.scales[i]); ps += toStr(b2, 2); }
			++seqs;
			const bool fields = (uint16)s.imageX == (f0 & 0xffff) && (uint16)s.imageY == (f0 >> 16) && s.x == x && s.y == y && s.duration == dur && s.type == type &&
				(uint16)s.hitX == (f0c & 0xffff) && (uint16)s.hitY == (f0c >> 16) && (uint16)s.hitW == (f10 & 0xffff) && (uint16)s.hitH == (f10 >> 16) && s.imageIndex == idx && s.entries.size() == cnt && s.firstDescent == fd;
			if (!fields) fail("fields");
			else if (pe != unhex(ent) || ps != unhex(scl)) fail("points or scales (type " + std::to_string(type) + ", " + std::to_string(cnt) + " points)");
			++seq;
		} else if (op == "P") {
			std::string list; in >> list; bool ok = true; uint32 k = 0;
			if (list != "-") { std::istringstream l(list); std::string tok; while (std::getline(l, tok, ',')) ok = ok && k < lv->banks()[bank].bank()->imageCount() && lv->banks()[bank].bank()->imagePosition(k++) == (uint32)strtoul(tok.c_str(), nullptr, 10); }
			if (!(ok && k == lv->banks()[bank].bank()->imageCount())) fail("image positions");
		}
	}
	printf("real banks                   %d levels, %d banks, %d sequences: %d mismatches (and %d known: points the data does not hold)\n", levels, banks, seqs, bad, known);
	for (auto &kv : badByLevel) printf("   %s: %d\n", kv.first.c_str(), kv.second);
	return bad != 0;
}
