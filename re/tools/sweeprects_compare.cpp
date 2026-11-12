// Compares the engine's sweepRects with the original's version on random rectangles (no output = no difference).
// The original's code is the old collision.cpp with sweepRects renamed sweepRectsOriginal (kept in git history of
// codex/array-lists before the simplification). Build (from scummvm-tesoro-nohosts):
//   clang++ -std=c++17 -O2 -I STUBS -I DIR -I engines -o sweep_compare sweeprects_compare.cpp orig.cpp engines/tesoro/core/collision.cpp
// STUBS needs common/scummsys.h with int32, int64, uint32 typedefs; orig.cpp declares sweepRectsOriginal.
#include "tesoro/core/collision.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
using namespace Tesoro;
namespace Tesoro { int32 sweepRectsOriginal(int32,int32,int32,int32,int32,int32,int32,int32,int32,int32); }

struct Frac { int64_t n, d; };            // d > 0
static Frac mk(int64_t n, int64_t d) { if (d < 0) { n = -n; d = -d; } return {n, d}; }
static bool lt(Frac a, Frac b) { return a.n * b.d < b.n * a.d; }

// The simple version: overlap windows per axis, closed intervals, as exact fractions.
static int32_t simple(int32_t ax1, int32_t ay1, int32_t ax2, int32_t ay2, int32_t bx1, int32_t by1, int32_t bx2, int32_t by2, int32_t dx, int32_t dy) {
	if (ax1 <= bx2 && bx1 <= ax2 && ay2 <= by1 && by2 <= ay1) return 0;
	bool has[2]; Frac en[2], ex[2];
	int32_t a1[2] = {ax1, ay2}, a2[2] = {ax2, ay1}, b1[2] = {bx1, by2}, b2[2] = {bx2, by1}, d[2] = {dx, dy};
	for (int i = 0; i < 2; ++i) {
		if (d[i] == 0) {
			if (a1[i] <= b2[i] && b1[i] <= a2[i]) { has[i] = false; continue; }   // always overlapping: no constraint
			return -1;
		}
		has[i] = true;
		if (d[i] > 0) { en[i] = mk(b1[i] - a2[i], d[i]); ex[i] = mk(b2[i] - a1[i], d[i]); }
		else          { en[i] = mk(b2[i] - a1[i], d[i]); ex[i] = mk(b1[i] - a2[i], d[i]); }
	}
	Frac e = {-1000000, 1}, x = {1000000, 1};
	for (int i = 0; i < 2; ++i) if (has[i]) { if (lt(e, en[i])) e = en[i]; if (lt(ex[i], x)) x = ex[i]; }
	if (lt(x, e)) return -1;                    // windows never meet
	if (lt(x, Frac{0, 1})) return -1;           // met only in the past
	if (lt(Frac{1, 1}, e)) return -1;           // meet only after the move
	if (e.n < 0) return 0;
	return (int32_t)((e.n * 65536) / e.d);
}

int main(int argc, char **argv) {
	srand(1);
	long long nz=0,nm=0,np=0,total = 0, diff = 0, shown = 0, hitDiff = 0, valDiff = 0;
	for (int round = 0; round < 4; ++round) {
		const int span = round == 0 ? 12 : round == 1 ? 60 : round == 2 ? 640 : 3000;
		const int maxd = round == 0 ? 8 : round == 1 ? 40 : round == 2 ? 200 : 1000;
		const int maxs = round == 0 ? 6 : round == 1 ? 20 : round == 2 ? 120 : 400;
		for (long long it = 0; it < 4000000; ++it) {
			auto R = [&](int lo, int hi) { return lo + rand() % (hi - lo + 1); };
			int32_t ax1 = R(0, span), ay2 = R(0, span), bx1 = R(0, span), by2 = R(0, span);
			int32_t ax2 = ax1 + R(0, maxs), ay1 = ay2 + R(0, maxs), bx2 = bx1 + R(0, maxs), by1 = by2 + R(0, maxs);
			int32_t dx = R(-maxd, maxd), dy = R(-maxd, maxd);
			int32_t o;
			o = sweepRectsOriginal(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2, dx, dy); int32_t s = sweepRects(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2, dx, dy);
			++total; if (o==0) ++nz; else if (o<0) ++nm; else ++np;
			if (o != s) {
				++diff; if ((o == -1) != (s == -1)) ++hitDiff; else ++valDiff;
				if (shown < 8) { ++shown; printf("A(%d,%d,%d,%d) B(%d,%d,%d,%d) d(%d,%d): original %d simple %d\n", ax1, ay1, ax2, ay2, bx1, by1, bx2, by2, dx, dy, o, s); }
			}
		}
	}
	printf("overlap-at-start %lld, never %lld, hit later %lld\n", nz, nm, np);
	printf("total %lld, different %lld (hit/miss differs %lld, same hit but different time %lld)\n", total, diff, hitDiff, valDiff);
}
