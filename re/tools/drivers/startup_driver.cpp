// Compares the engine's core primitives with vectors recorded from the original machine code
// (see re/tools/gen_vectors_startup.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
#include "common/scummsys.h"
#include "tesoro/core/clock.h"
#include "tesoro/core/namekey.h"
#include "tesoro/core/random.h"

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	char line[512];
	int total[4] = {0}, bad[4] = {0};
	std::unique_ptr<Tesoro::GameClock> clock;
	while (fgets(line, sizeof line, f)) {
		char tag[8];
		if (line[0] == '#') { clock.reset(); continue; }
		sscanf(line, "%7s", tag);
		if (!strcmp(tag, "R") || !strcmp(tag, "R7")) {
			unsigned tick; int lo, hi, want;
			sscanf(line + strlen(tag), " %u %d %d %d", &tick, &lo, &hi, &want);
			int got = !strcmp(tag, "R") ? Tesoro::randomRange(tick, lo, hi) : Tesoro::randomRangeAlt(tick, lo, hi);
			int k = !strcmp(tag, "R") ? 0 : 1; ++total[k];
			if (got != want && bad[k]++ < 5) printf("%s mismatch tick=%u [%d,%d]: got %d want %d\n", tag, tick, lo, hi, got, want);
		} else if (!strcmp(tag, "K") || !strcmp(tag, "KB")) {
			unsigned long long want; char name[300] = "";
			sscanf(line + strlen(tag), " %llx %299s", &want, name);
			std::string s = name;
			if (!strcmp(tag, "KB")) { s.clear(); for (size_t i = 0; i + 1 < strlen(name); i += 2) { char b[3] = {name[i], name[i + 1], 0}; s += (char)strtol(b, nullptr, 16); } }
			else if (s == "@") s.clear();
			++total[2];
			unsigned long long got = Tesoro::nameKey64(s.c_str());
			if (got != want && bad[2]++ < 5) printf("key mismatch '%s': got %016llx want %016llx\n", s.c_str(), got, want);
		} else if (!strcmp(tag, "C")) {
			char op[16], exp[32]; unsigned tick;
			sscanf(line + 1, " %15s %u %31s", op, &tick, exp);
			if (!strcmp(op, "new")) { clock.reset(new Tesoro::GameClock(tick)); continue; }
			if (!strcmp(op, "now")) {
				++total[3]; int want = atoi(exp); int got = clock->update(tick);
				if (got != want && bad[3]++ < 5) printf("clock now mismatch tick=%u: got %d want %d\n", tick, got, want);
			} else if (!strcmp(op, "freeze")) clock->freeze(tick);
			else if (!strcmp(op, "unfreeze")) clock->unfreeze();
			else if (!strcmp(op, "suspend")) clock->suspend();
			else if (!strcmp(op, "resume")) clock->resume(tick);
		}
	}
	const char *names[4] = {"RandomRange", "RandomRangeAlt", "nameKey64", "clock now"};
	int fails = 0;
	for (int i = 0; i < 4; ++i) { printf("%-13s %d/%d match\n", names[i], total[i] - bad[i], total[i]); fails += bad[i]; }
	return fails ? 1 : 0;
}
