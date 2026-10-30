// Compares the engine's WalkArea with the original ScaleAt / FindPath / NextWaypoint (see re/tools/gen_vectors_walkarea.py).
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/animbank.h"
#include "tesoro/core/walkarea.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

using namespace Tesoro;

struct Jim : Walker {
	std::vector<std::string> log;
	WalkArea *area = nullptr; WalkTarget *other = nullptr;
	int ret = 1;
	int walkTo(int32 x, int32 y, WalkTarget *t, bool direct) override {
		char b[128]; snprintf(b, sizeof b, "walk %d %d %s %d", x, y, t == area ? "area" : t == other ? "target" : t == nullptr ? "none" : "?", direct);
		log.push_back(b); return ret;
	}
};
struct Dummy : WalkTarget { int arrived() override { return 0; } };

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1024];
	int sessions = 0, bad = 0, ops = 0, shown = 0;
	bool have = fgets(line, sizeof line, f);
	while (have) {
		if (line[0] != 'S') { have = fgets(line, sizeof line, f); continue; }
		int sid, ax, ay, n; sscanf(line + 2, "%d %d %d %d", &sid, &ax, &ay, &n);
		AnimSequence seq;
		for (int i = 0; i < n; ++i) {
			fgets(line, sizeof line, f); int x, y, s; sscanf(line, "%d %d %d", &x, &y, &s);
			seq.entries.push_back((uint32)(uint16)x | ((uint32)(uint16)y << 16)); seq.scales.push_back((uint16)s);
		}
		Jim jim; Dummy target; jim.other = &target;
		WalkArea area(jim); jim.area = &area; area.setShape(&seq); area.setPosition(ax, ay);
		++sessions; bool badSession = false;
		int jx = 0, jy = 0;
		while ((have = fgets(line, sizeof line, f)) && line[0] != '#') {
			if (line[0] == 'P') { jim.ret = atoi(line + 2); continue; }
			if (line[0] == 'R') { int k, x, y, s; sscanf(line + 2, "%d %d %d %d", &k, &x, &y, &s); seq.entries[k + 1] = (uint32)(uint16)x | ((uint32)(uint16)y << 16); seq.scales[k + 1] = (uint16)s; continue; }
			if (line[0] != 'Q' && line[0] != 'F' && line[0] != 'N') continue;
			const char kind = line[0]; int a = 0, b = 0, c = 0, d = 0, t = 0;
			if (kind == 'Q') sscanf(line + 2, "%d %d", &a, &b); else if (kind == 'F') sscanf(line + 2, "%d %d %d %d %d", &a, &b, &c, &d, &t);
			std::string got; jim.log.clear();
			if (kind == 'Q') got = std::to_string((uint32)area.scaleAt(a, b));
			else if (kind == 'F') got = std::to_string(area.findPath(a, b, c, d, t ? &target : nullptr) ? 1 : 0), jx = c, jy = d;
			else got = std::to_string((uint32)area.arrived());
			// expected: the result line then the log lines
			fgets(line, sizeof line, f); std::string want = line + 2; want.erase(want.find_last_not_of("\n") + 1);
			std::vector<std::string> wl;
			while (true) { long p = ftell(f); if (!fgets(line, sizeof line, f)) break; if (line[0] != '.') { fseek(f, p, SEEK_SET); break; } std::string s = line + 2; s.erase(s.find_last_not_of("\n") + 1); wl.push_back(s); }
			++ops;
			if (kind == 'F') { unsigned w = (unsigned)atol(want.c_str()) & 0xff; want = std::to_string(w); }
			if (got != want || jim.log != wl) {
				badSession = true;
				if (shown++ < 6) printf("session %d op %c: result got %s want %s, %zu/%zu calls\n", sid, kind, got.c_str(), want.c_str(), jim.log.size(), wl.size());
			}
		}
		(void)jx; (void)jy;
		if (badSession) ++bad;
		have = fgets(line, sizeof line, f);
	}
	printf("Walk area        %d/%d sessions match (%d operations)\n", sessions - bad, sessions, ops);
	return bad ? 1 : 0;
}
