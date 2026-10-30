// Compares the engine's speech tables with the original loader, lookup and four-table search (see re/tools/gen_vectors_speech.py).
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/speechtable.h"
#include "tesoro/core/namekey.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }

using namespace Tesoro;

static std::string unhex(const char *s) {
	std::string r;
	if (s[0] == '-') return r;
	for (size_t i = 0; s[i] && s[i + 1] && s[i] != '\n'; i += 2) { char b[3] = {s[i], s[i + 1], 0}; r += (char)strtol(b, nullptr, 16); }
	return r;
}

struct Wavs : WavLookup {
	uint32 find(const char *name) override {
		uint32 h = 0;
		for (const char *p = name; *p; ++p) h = h * 31 + (byte)*p;
		return h % 5 == 0 ? 0 : 1000 + h % 100000;
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1 << 21]; static char hex[1 << 21];
	Wavs wavs; SpeechTables tables; SpeechTable scratch;
	std::string text, name;
	struct Want { uint32 lo, hi, wav; std::string text; };
	std::vector<Want> want; int wantCount = 0, realIndex = 0;
	struct Probe { uint32 lo, hi; int idx; };
	std::vector<Probe> probes;
	int tablesTotal = 0, tablesBad = 0, lines = 0, linesBad = 0, finds = 0, findsBad = 0, phrases = 0, phrasesBad = 0;
	bool havePending = false;
	while (fgets(line, sizeof line, f)) {
		if (line[0] == 'F') {
			char nm[64]; unsigned size; sscanf(line + 2, "%63s %u %2097151s", nm, &size, hex);
			name = nm; text = unhex(hex); want.clear(); probes.clear(); havePending = true;
		} else if (line[0] == 'N') wantCount = atoi(line + 2);
		else if (line[0] == 'L') {
			unsigned lo, hi, wav; sscanf(line + 2, "%u %u %u %2097151s", &lo, &hi, &wav, hex);
			want.push_back({lo, hi, wav, unhex(hex)});
		} else if (line[0] == 'P') {
			unsigned lo, hi; int idx; sscanf(line + 2, "%u %u %d", &lo, &hi, &idx); probes.push_back({lo, hi, idx});
		} else if (line[0] == 'Q') {
			char nm[512]; int t, idx; sscanf(line + 2, "%511s %d %d", nm, &t, &idx);
			const std::string w = unhex(nm); ++phrases;
			const uint32 ref = tables.find(w.c_str());
			int gt = -1, gi = 0; if (ref) { gt = (ref - 1) >> 24; gi = (ref - 1) & 0xffffff; }
			if ((gt != t || (t >= 0 && gi != idx)) && phrasesBad++ < 5) printf("phrase '%s': got table %d line %d want %d %d\n", w.c_str(), gt, gi, t, idx);
		} else if (line[0] == '#' && havePending) {
			havePending = false;
			const bool fuzz = name.compare(0, 4, "FUZZ") == 0;
			SpeechTable &t = fuzz ? scratch : tables.table(realIndex);
			t.load((const byte *)text.data(), (uint32)text.size(), wavs);
			if (!fuzz) ++realIndex;
			++tablesTotal; bool ok = (int)t.count() == wantCount;
			for (uint32 i = 0; ok && i < t.count(); ++i) {
				++lines;
				const Want &w = want[i]; const uint64 k = (uint64(w.hi) << 32) | w.lo;
				if (t.line(i).key != k || t.line(i).wavOffset != w.wav || std::string(t.line(i).str()) != w.text) { ok = false; ++linesBad; printf("%s line %u differs\n", name.c_str(), i); }
			}
			if (!ok && tablesBad++ < 5) printf("%s: table differs (count got %u want %d)\n", name.c_str(), t.count(), wantCount);
			for (const Probe &p : probes) {
				++finds; const int got = t.find((uint64(p.hi) << 32) | p.lo);
				if (got != p.idx && findsBad++ < 5) printf("%s find %u/%u: got %d want %d\n", name.c_str(), p.lo, p.hi, got, p.idx);
			}
		}
	}
	printf("Speech tables   %d/%d files match (%d lines)\nSpeech lookup   %d/%d table searches match\nSpeech phrases  %d/%d four-table searches match\n",
	       tablesTotal - tablesBad, tablesTotal, lines, finds - findsBad, finds, phrases - phrasesBad, phrases);
	return (tablesBad || findsBad || phrasesBad) ? 1 : 0;
}
