// Compares the engine's Cursor::isHitter with vectors recorded from the original (see re/tools/gen_vectors_hitter.py).
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "common/scummsys.h"
#include "tesoro/core/cursor.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) return 2;
	unsigned pf, of, want; int total = 0, bad = 0;
	while (fscanf(f, "%u %u = %u", &pf, &of, &want) == 3) {
		Tesoro::Cursor cursor; Tesoro::Object other;
		cursor.init(&Tesoro::Cursor::sharedPrevious()); cursor.setFlags(pf); other.initPlain(); other.setFlags(of);
		++total; if ((cursor.isHitter(&other) ? 1u : 0u) != want) ++bad;
	}
	printf("%-28s %d/%d match\n", "pointer isHitter", total - bad, total);
	return bad ? 1 : 0;
}
