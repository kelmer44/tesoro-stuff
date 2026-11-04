// Scans every outline in the shipped levels' animation banks: sizes, chain order and convexity. Read-only.
// Usage: outlinescan_driver <TESORO dir>. Not a comparison with the original; it shows which outline shapes exist in the data.
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
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

int main(int argc, char **argv) {
	std::string dir = argv[1];
	FILE *tre = fopen((dir + "/TESORO.TRE").c_str(), "rb"), *dat = fopen((dir + "/TESORO.DAT").c_str(), "rb");
	if (!tre || !dat) return 2;
	FileStream ds(dat);
	unsigned char rec[28];
	int sequences = 0, shaped = 0, single = 0, multi = 0, nonConvex = 0, chainOrder = 0, noSplit = 0, degenerate = 0, type2 = 0, type4 = 0;
	std::map<int, int> sizes;
	while (fread(rec, 28, 1, tre) == 1) {
		if (!(rec[8] & 4)) continue;
		std::string nm((char *)rec + 9, strnlen((char *)rec + 9, 19));
		for (auto &c : nm) c = (char)toupper((unsigned char)c);
		if (nm.size() < 4 || nm.substr(nm.size() - 4) != ".LVL") continue;
		uint32 off = rec[0] | rec[1] << 8 | rec[2] << 16 | (uint32)rec[3] << 24;
		Tesoro::Level lv;
		if (!lv.load(ds, off + 4, 0, false, 0)) continue;
		for (uint b = 0; b < lv.banks().size(); ++b) {
			Tesoro::AnimBank *bank = lv.banks()[b].bank();
			if (!bank) continue;
			for (uint32 s = 0; s < bank->sequenceCount(); ++s) {
				const Tesoro::AnimSequence &q = bank->sequence(s);
				++sequences;
				const int n = (int)q.entries.size();
				if (!n || (q.type != 2 && q.type != 4)) continue;
				++shaped; (q.type == 2 ? type2 : type4)++; sizes[n > 12 ? 13 : n]++;
				if (n == 1) { ++single; continue; }
				++multi;
				if (argc > 2) { printf("shape %s %u %u %u:", nm.c_str(), b, s, q.firstDescent); for (int i = 0; i < n; ++i) printf(" %d:%d", (int16)(q.entries[i] & 0xffff), (int16)(q.entries[i] >> 16)); printf("\n"); }
				std::vector<int> x(n), y(n);
				for (int i = 0; i < n; ++i) { x[i] = (int16)(q.entries[i] & 0xffff); y[i] = (int16)(q.entries[i] >> 16); }
				// chain order: first chain left to right, second chain back
				bool ok = q.firstDescent > 0 && q.firstDescent <= n;
				if (!ok) ++noSplit;
				for (int i = 1; ok && i < (int)q.firstDescent; ++i) if (x[i] < x[i - 1]) ok = false;
				for (int i = q.firstDescent + 1; ok && i < n; ++i) if (x[i] > x[i - 1]) ok = false;
				if (!ok) ++chainOrder;
				// convexity of the closed polygon
				int pos = 0, neg = 0;
				for (int i = 0; i < n; ++i) {
					const int j = (i + 1) % n, k = (i + 2) % n;
					const long long cr = (long long)(x[j] - x[i]) * (y[k] - y[j]) - (long long)(y[j] - y[i]) * (x[k] - x[j]);
					if (cr > 0) ++pos; else if (cr < 0) ++neg;
				}
				if (!pos && !neg) ++degenerate;
				else if (pos && neg) { ++nonConvex; if (nonConvex <= 8) printf("non-convex: %s bank %u seq %u, %d points\n", nm.c_str(), b, s, n); }
			}
		}
	}
	printf("%d sequences, %d with outlines (type 2: %d, type 4: %d): %d single point, %d with 2+ points\n", sequences, shaped, type2, type4, single, multi);
	printf("of the multi-point ones: non-convex %d, degenerate (collinear) %d, chains not in left/right order or no split %d (no split %d)\n", nonConvex, degenerate, chainOrder, noSplit);
	printf("sizes:"); for (auto &e : sizes) printf(" %d%s:%d", e.first, e.first == 13 ? "+" : "", e.second); printf("\n");
	return 0;
}
