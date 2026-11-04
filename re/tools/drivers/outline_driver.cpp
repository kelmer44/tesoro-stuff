// Compares the engine's Outline (contains, clipColumns, overlaps) with vectors recorded from the original code
// (see re/tools/gen_vectors_outline.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/outline.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

// Points sit between zero padding, as in the vectors, so reads just outside the chains see the same zeros.
struct Poly {
	std::vector<uint32> store;
	Tesoro::Outline o;
	void set(std::istringstream &in, int n, int m) {
		store.assign(n + 128, 0);
		for (int i = 0; i < n; ++i) { int x, y; char c; in >> x >> c >> y; store[64 + i] = (uint32)(uint16)x | ((uint32)(uint16)y << 16); }
		o.count = (uint16)n; o.split = (uint16)m; o.points = &store[64];
	}
};

int main(int argc, char **argv) {
	std::ifstream f(argv[1]);
	std::string line; int total = 0, bad = 0;
	while (std::getline(f, line)) {
		std::istringstream in(line); std::string kind, bar, eq; in >> kind;
		int n, m; in >> n >> m; Poly a; a.set(in, n, m); in >> bar; ++total;
		bool ok = true;
		if (kind == "C") {
			int px, py, r; in >> px >> py >> eq >> r;
			ok = a.o.contains(px, py) == (r != 0);
		} else if (kind == "K") {
			int bx1, bx2, cx1, cx2, rn, rm; in >> bx1 >> bx2 >> cx1 >> cx2 >> eq >> rn >> rm;
			std::vector<uint32> buf(n + 200, 0); Tesoro::Outline out; out.count = 0xdead; out.split = 0xbeef;
			a.o.clipColumns(bx1, bx2, cx1, cx2, out, &buf[0]);
			ok = out.count == rn && out.split == rm;
			for (int i = 0; i < rn && ok; ++i) { int x, y; char c; in >> x >> c >> y; ok = buf[i] == ((uint32)(uint16)x | ((uint32)(uint16)y << 16)); }
		} else {
			int n2, m2, r; in >> n2 >> m2; Poly b; b.set(in, n2, m2); in >> eq >> r;
			ok = a.o.overlaps(b.o) == (r != 0);
		}
		if (!ok && ++bad <= 5) printf("MISMATCH %s\n", line.c_str());
	}
	printf("outline: %d cases, %d mismatches\n", total, bad);
	return bad ? 1 : 0;
}
