// Compares the engine's archive index with vectors recorded from the original code
// (see re/tools/gen_vectors_archive.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/archiveindex.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }

static std::string unhex(const char *s) {
	std::string r;
	if (!strcmp(s, "-")) return r;
	for (size_t i = 0; s[i] && s[i + 1]; i += 2) { char b[3] = {s[i], s[i + 1], 0}; r += (char)strtol(b, nullptr, 16); }
	return r;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1 << 16];
	Tesoro::ArchiveIndex idx;
	std::vector<std::string> wantNames; std::vector<unsigned> wantOffs;
	int cases = 0, caseBad = 0, finds = 0, findBad = 0;
	bool loaded = false;
	auto finishLoad = [&]() {
		bool ok = idx.count() == wantNames.size();
		for (uint32 i = 0; ok && i < idx.count(); ++i)
			ok = wantNames[i] == idx.entry(i).name && wantOffs[i] == idx.entry(i).offset;
		++cases; if (!ok && caseBad++ < 5) printf("index mismatch in case %d (count got %u want %zu)\n", cases, idx.count(), wantNames.size());
	};
	while (fgets(line, sizeof line, f)) {
		char a[1 << 15], b[64];
		if (line[0] == 'T') {
			if (sscanf(line + 2, "%32767s", a) < 1) a[0] = 0;
			std::string img = unhex(a);
			idx.load((const byte *)img.data(), (uint32)img.size());
			wantNames.clear(); wantOffs.clear(); loaded = false;
		} else if (line[0] == 'E') {
			unsigned off; sscanf(line + 2, "%32767s %u", a, &off);
			wantNames.push_back(unhex(a)); wantOffs.push_back(off);
		} else if (line[0] == 'F') {
			if (!loaded) { finishLoad(); loaded = true; }
			unsigned want; sscanf(line + 2, "%32767s %u", a, &want);
			std::string nm = unhex(a); ++finds;
			unsigned got = idx.find(nm.c_str());
			if (got != want && findBad++ < 5) printf("find mismatch '%s': got %u want %u\n", nm.c_str(), got, want);
		} else if (line[0] == '#') {
			if (!loaded) finishLoad();
			loaded = false;
		}
	}
	printf("ArchiveIndex  %d/%d index cases match\nArchive find  %d/%d lookups match\n", cases - caseBad, cases, finds - findBad, finds);
	return (caseBad || findBad) ? 1 : 0;
}
