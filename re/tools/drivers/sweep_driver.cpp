// Compares the engine's sweepRects with vectors recorded from the original code (see re/tools/gen_vectors_sweep.py).
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common/scummsys.h"
#include "tesoro/core/collision.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	int a[10], want, total = 0, bad = 0, byResult[3] = {0, 0, 0};
	while (fscanf(f, "%d %d %d %d %d %d %d %d %d %d = %d", a, a + 1, a + 2, a + 3, a + 4, a + 5, a + 6, a + 7, a + 8, a + 9, &want) == 11) {
		const int got = Tesoro::sweepRects(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
		++total; ++byResult[want == 0 ? 0 : want < 0 ? 1 : 2];
		if (got != want) { ++bad; if (bad < 8) printf("mismatch: want %d got %d\n", want, got); }
	}
	printf("%-28s %d/%d match (overlap %d, miss %d, hit %d)\n", "sweepRects", total - bad, total, byResult[0], byResult[1], byResult[2]);
	return bad ? 1 : 0;
}
