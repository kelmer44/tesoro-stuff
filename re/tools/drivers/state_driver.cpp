// Compares the engine's Object save/load state with vectors recorded from the original
// (see re/tools/gen_vectors_state.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include "common/stream.h"
#include "common/scummsys.h"
#include "tesoro/core/object.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

// Minimal in-memory streams (the real ones live in a file that drags in half of ScummVM).
class MemReader : public Common::ReadStream {
public:
	MemReader(const byte *d, size_t n) : _d(d), _n(n), _p(0) {}
	bool eos() const override { return _p >= _n; }
	uint32 read(void *dst, uint32 len) override { size_t k = len < _n - _p ? len : _n - _p; if (k) memcpy(dst, _d + _p, k); _p += k; return (uint32)k; }
private:
	const byte *_d; size_t _n, _p;
};
class MemWriter : public Common::WriteStream {
public:
	uint32 write(const void *src, uint32 len) override { const byte *b = (const byte *)src; data.insert(data.end(), b, b + len); return len; }
	int64 pos() const override { return data.size(); }
	std::vector<unsigned char> data;
};

static std::vector<unsigned char> fromHex(const std::string &h) {
	std::vector<unsigned char> v;
	if (h == "-") return v;
	for (size_t i = 0; i + 1 < h.size(); i += 2) v.push_back((unsigned char)strtoul(h.substr(i, 2).c_str(), nullptr, 16));
	return v;
}
static std::string toHex(const unsigned char *d, size_t n) {
	if (!n) return "-";
	std::string s; char b[3];
	for (size_t i = 0; i < n; ++i) { snprintf(b, sizeof b, "%02x", d[i]); s += b; }
	return s;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	Tesoro::Object obj;
	int caseNo = 0, nBanks = 0, cap = 0, ok = 0, bad = 0, wOk = 0, wBad = 0;
	std::string lastOp;
	std::vector<Tesoro::AnimPlayer> prime;
	struct Pl { unsigned flags, first, last, current; };
	std::vector<Pl> pls; int startBank = 0; bool anim = false;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") { int a; in >> nBanks >> cap >> a >> startBank; anim = a; pls.clear(); ++caseNo; lastOp = "ctor"; }
		else if (op == "Pl") { int i; Pl p; in >> i >> p.flags >> p.first >> p.last >> p.current; pls.push_back(p); }
		else if (op == "Bl") {
			std::string h; in >> h;
			Tesoro::ObjectRecord rec; for (int i = 0; i < nBanks; ++i) rec.banks.push_back((byte)i);
			Common::Array<Tesoro::AnimPlayer> banks; banks.resize(nBanks);
			obj = Tesoro::Object(); obj.initAnimated(rec, 0, banks, (uint16)cap);
			if (!anim) obj.setFlags(obj.flags() & ~(uint32)Tesoro::Object::kHasAnimation);
			// Prime every player through its own load path, then restore the full flag words.
			for (int i = 0; i < nBanks; ++i) {
				byte raw[6] = { (byte)i, (byte)pls[i].first, (byte)(pls[i].first >> 8), (byte)pls[i].last, (byte)(pls[i].last >> 8), (byte)pls[i].current };
				byte full[7]; memcpy(full, raw, 6); full[6] = (byte)pls[i].flags;
				MemReader s(full, 7); obj.animation().loadState(s);
				obj.animation().player(i).setFlags(pls[i].flags);
			}
			byte sel[7] = { (byte)startBank, (byte)pls[startBank].first, (byte)(pls[startBank].first >> 8), (byte)pls[startBank].last, (byte)(pls[startBank].last >> 8), (byte)pls[startBank].current, (byte)pls[startBank].flags };
			MemReader s(sel, 7); obj.animation().loadState(s);
			obj.animation().player(startBank).setFlags(pls[startBank].flags);
			std::vector<unsigned char> b = fromHex(h);
			for (size_t i = 0; i < b.size(); ++i) obj.stateBlob()[i] = b[i];
		}
		else if (op == "sv") { MemWriter w; obj.saveState(w); std::string got = toHex(w.data.data(), w.data.size()); std::string h; fgets(buf, sizeof buf, f); std::istringstream in2(buf); std::string w2; in2 >> w2 >> h; if (got == h) ++wOk; else { ++wBad; ++bad; if (bad < 12) printf("mismatch case %d save: want %s got %s\n", caseNo, h.c_str(), got.c_str()); } lastOp = "sv"; }
		else if (op == "ld") { std::string h; in >> h; std::vector<unsigned char> d = fromHex(h); MemReader s(d.data(), d.size()); obj.loadState(s); lastOp = "ld " + h; }
		else if (op == "S") {
			std::string want, got, tok; std::vector<std::string> t; while (in >> tok) t.push_back(tok);
			for (auto &x : t) want += x + " ";
			got = std::to_string(obj.animation().bankIndex()) + " ";
			for (int i = 0; i < nBanks; ++i) { auto &p = obj.animation().player(i); got += std::to_string(p.flags()) + " " + std::to_string(p.first()) + " " + std::to_string(p.last()) + " " + std::to_string(p.current()) + " "; }
			got += "| " + std::to_string(obj.stateSize()) + " " + toHex(obj.stateBlob().data(), cap);
			got += " ";
			if (got == want) ++ok; else { ++bad; if (bad < 12) printf("mismatch case %d after %s:\n want %s\n got  %s\n", caseNo, lastOp.c_str(), want.c_str(), got.c_str()); }
		}
	}
	printf("object state %d/%d match, saved bytes %d/%d\n", ok, ok + bad - wBad, wOk, wOk + wBad);
	return bad ? 1 : 0;
}
