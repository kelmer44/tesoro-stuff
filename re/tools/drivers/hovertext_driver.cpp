// Compares the engine's HoverText with vectors recorded from the original Cursor_SetText
// (see re/tools/gen_vectors_hovertext.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include "common/scummsys.h"
#include "tesoro/core/hovertext.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

static std::string hex(const unsigned char *d, size_t n) { std::string s; char b[3]; for (size_t i = 0; i < n; ++i) { snprintf(b, sizeof b, "%02x", d[i]); s += b; } return s; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	Tesoro::HoverText label;
	int caseNo = 0, ok = 0, bad = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") { label = Tesoro::HoverText(); ++caseNo; }
		else if (op == "set") {
			long long colour; std::string h; in >> colour >> h;
			if (h == "-null") label.set(nullptr, (int)colour);
			else {
				std::string text; if (h != "-") for (size_t i = 0; i + 1 < h.size(); i += 2) text += (char)strtoul(h.substr(i, 2).c_str(), nullptr, 16);
				label.set(text.c_str(), (int)colour);
			}
		} else if (op == "S") {
			unsigned colour; std::string h; in >> colour >> h;
			bool same = label.colour() == colour && hex((const unsigned char *)label.buffer(), Tesoro::HoverText::kSize) == h;
			if (same) ++ok; else { ++bad; if (bad < 8) printf("mismatch case %d: colour want %u got %u\n", caseNo, colour, label.colour()); }
		}
	}
	printf("hover text %d/%d match\n", ok, ok + bad);
	return bad ? 1 : 0;
}
