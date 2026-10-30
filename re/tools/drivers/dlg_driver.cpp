// Compares the engine's script compiler with the original loader's output for every block of the shipped .DLG files
// (see re/tools/gen_vectors_dlg.py). Only bytes the original compiler wrote are compared (mask line M).
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/scriptblock.h"
#include "tesoro/core/namekey.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }

using namespace Tesoro;

static std::string unhex(const char *s) {
	std::string r;
	if (s[0] == '-') return r;
	for (size_t i = 0; s[i] && s[i + 1] && s[i] != '\n'; i += 2) { char b[3] = {s[i], s[i + 1], 0}; r += (char)strtol(b, nullptr, 16); }
	return r;
}

// Stand-ins for the lookups that need live levels, the same formulas as the generator.
struct Lookup : ScriptLookup {
	uint32 phraseId(const char *name) override { const uint64 k = nameKey64(name); return (uint32(k) ^ (uint32(k >> 32) * 3) ^ 0x1234567) | 1; }
	uint32 levelRef(int i) override { return 0x71000000 + 0x100 * i; }
	uint32 inventoryRef() override { return 0x72000000; }
};

struct Want {
	uint32 k0, k1, type;
	std::string chars, labels, entries, mask, resolved;
};

static bool same(const std::string &got, const std::string &want, const std::string &mask) {
	if (got.size() != want.size()) return false;
	for (size_t i = 0; i < got.size(); ++i)
		if (mask.empty() ? got[i] != want[i] : ((got[i] ^ want[i]) & mask[i])) return false;
	return true;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1 << 22];
	Lookup lookup;
	int blocks = 0, blockBad = 0, files = 0, fileBad = 0, resolved = 0, resolvedBad = 0, bytes = 0;
	std::string text; std::string name;
	std::vector<Want> want;
	bool original_error = false; int errorCases = 0, errorBad = 0;
	while (fgets(line, sizeof line, f)) {
		if (line[0] == 'F') {
			char nm[64]; unsigned size; static char hex[1 << 22];
			sscanf(line + 2, "%63s %u %4194303s", nm, &size, hex);
			name = nm; text = unhex(hex); want.clear(); original_error = false;
		} else if (line[0] == 'B') {
			Want w; unsigned a, b, c; sscanf(line + 2, "%u %u %u", &a, &b, &c); w.k0 = a; w.k1 = b; w.type = c; want.push_back(w);
		} else if (line[0] == 'C') want.back().chars = unhex(line + 2);
		else if (line[0] == 'L') want.back().labels = unhex(line + 2);
		else if (line[0] == 'E') want.back().entries = unhex(strchr(line + 2, ' ') + 1);
		else if (line[0] == 'M') want.back().mask = unhex(strchr(line + 2, ' ') + 1);
		else if (line[0] == 'R') {
			const unsigned i = strtoul(line + 2, nullptr, 10);
			want[i].resolved = unhex(strchr(line + 2, ' ') + 1);
		} else if (line[0] == 'Z') {
			original_error = true;
		} else if (line[0] == '#') {
			ScriptLibrary lib;
			const bool ok = lib.load((const byte *)text.data(), (uint32)text.size(), lookup);
			if (original_error) {
				++errorCases;
				if (ok && errorBad++ < 8) printf("%s: the original reported a bad line, the port accepted the text\n", name.c_str());
				continue;
			}
			++files;
			bool fileOk = ok && lib.blockCount() == want.size();
			if (!fileOk && fileBad++ < 5) printf("%s: blocks got %u want %zu (ok=%d)\n", name.c_str(), lib.blockCount(), want.size(), ok);
			std::vector<std::string> gotEntries;
			for (uint32 i = 0; i < lib.blockCount() && i < want.size(); ++i) {
				const ScriptBlock &b = lib.block(i); const Want &w = want[i];
				std::string chars, labels, entries;
				for (uint32 k = 0; k < b.characterCount(); ++k) { uint64 v = b.character(k); chars.append((const char *)&v, 8); }
				for (uint32 k = 0; k < b.labelCount(); ++k) {
					uint32 t[3] = { uint32(b.label(k).key), uint32(b.label(k).key >> 32), b.label(k).entry };
					labels.append((const char *)t, 12);
				}
				for (uint32 k = 0; k < b.entryCount(); ++k) entries.append((const char *)b.entry(k).data, 24);
				const uint64 wk = (uint64(w.k1) << 32) | w.k0;
				bool good = b.key() == wk && b.type() == w.type && chars == w.chars && labels == w.labels && same(entries, w.entries, w.mask);
				++blocks; bytes += (int)entries.size();
				if (!good && blockBad++ < 8) {
					printf("%s block %u differs: key %d type %d (%u/%u) chars %d labels %d entries %d (got %zu bytes want %zu)\n", name.c_str(), i,
					       b.key() == wk, b.type() == w.type, b.type(), w.type, chars == w.chars, labels == w.labels, same(entries, w.entries, w.mask), entries.size(), w.entries.size());
					if (entries.size() == w.entries.size())
						for (size_t k = 0; k < entries.size(); ++k) if ((entries[k] ^ w.entries[k]) & (w.mask.empty() ? 0xff : w.mask[k])) { printf("  entry %zu byte %zu got %02x want %02x\n   got ", k / 24, k % 24, (byte)entries[k], (byte)w.entries[k]); const size_t e0 = k / 24 * 24; for (int q = 0; q < 24; ++q) printf("%02x ", (byte)entries[e0 + q]); printf("\n  want "); for (int q = 0; q < 24; ++q) printf("%02x ", (byte)w.entries[e0 + q]); printf("\n  mask "); for (int q = 0; q < 24; ++q) printf("%02x ", w.mask.empty() ? 255 : (byte)w.mask[e0 + q]); printf("\n"); break; }
				}
			}
			lib.resolve();
			for (uint32 i = 0; i < lib.blockCount() && i < want.size(); ++i) {
				std::string entries;
				for (uint32 k = 0; k < lib.block(i).entryCount(); ++k) entries.append((const char *)lib.block(i).entry(k).data, 24);
				++resolved;
				if (!same(entries, want[i].resolved, want[i].mask) && resolvedBad++ < 5) printf("%s block %u differs after resolve\n", name.c_str(), i);
			}
		}
	}
	printf("Script compiler  %d/%d blocks match (%d bytes of commands), %d/%d files\nScript resolver  %d/%d blocks match\nScript errors    %d/%d bad texts rejected\n",
	       blocks - blockBad, blocks, bytes, files - fileBad, files, resolved - resolvedBad, resolved, errorCases - errorBad, errorCases);
	return (blockBad || fileBad || resolvedBad || errorBad) ? 1 : 0;
}
