// Compares the engine's scene routines (core/scenes.cpp) with vectors recorded from the original routines
// (see re/tools/gen_vectors_scenes.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/scenes.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

using namespace Tesoro;

struct Host : SceneHost {
	std::vector<std::string> log;
	int blockLevel = 0, jimLevel = 0; int32 jx = 0, jy = 0; int phase = 0, step = 0; int held = 0;
	std::map<std::string, int> sorts;
	static std::string L(int level) { return "L" + std::to_string(level); }
	static std::string obj(int level, const char *name) { return L(level) + "." + name; }
	void push(const std::string &s) { log.push_back(s); }
	void find(int level, const char *name) { push("find:" + L(level) + ":" + name); }
	// The objects stand where the vectors put them.
	static int32 objX(int level, const char *name) { return 300 + 10 * level + (int32)strlen(name); }
	static int32 objY(int level, const char *name) { return -(50 + 7 * level + (int32)strlen(name)); }
	void enter(int level, int32 x, int32 y) {
		push("enter:" + L(level) + ":" + std::to_string(x) + ":" + std::to_string(y));
		blockLevel = level; jimLevel = level; jx = x; jy = y;
	}
	void enterLevel(int level, int32 x, int32 y) override { enter(level, x, y); }
	void enterLevelAtJim(int level) override { enter(level, jx, jy); }
	void enterLevelAtFloor(int level) override { enter(level, 1000 + level, 2000 + level); }
	void enterLevelAtObject(int level, const char *name) override { find(level, name); enter(level, objX(level, name), objY(level, name)); }
	void rebindCharacters() override { find(blockLevel, "AAA"); find(blockLevel, "BBB"); }
	void cameraToJim(bool block) override { push("camera:" + L(block ? blockLevel : jimLevel) + ":" + std::to_string(jx) + ":" + std::to_string(jy)); }
	void setJimVisible(bool v) override { push(std::string("visible:jim:") + (v ? "1" : "0")); }
	void setJimActive(bool v) override { push(std::string("active:jim:") + (v ? "1" : "0")); }
	void setJimPosition(int32 x, int32 y) override { jx = x; jy = y; }
	void setJimPhase(byte v) override { phase = v; }
	void setJimStep(byte v) override { step = v; }
	void setObjectActive(int level, const char *name, bool v) override { find(level, name); push("active:" + obj(level, name) + (v ? ":1" : ":0")); }
	void setObjectVisible(int level, const char *name, bool v) override { find(level, name); push("visible:" + obj(level, name) + (v ? ":1" : ":0")); }
	void selectBank(int level, const char *name, int bank, int first, int last, bool loop, bool sum) override {
		find(level, name);
		push("bank:" + obj(level, name) + ":" + std::to_string(bank) + ":" + std::to_string(first) + ":" + std::to_string(last) + ":" + (loop ? "1" : "0") + ":" + (sum ? "1" : "0"));
	}
	void setSortValue(int level, const char *name, int16 v) override { find(level, name); sorts[obj(level, name)] = v; }
	void runDoorAction(int level, const char *name) override { find(level, name); push("door:" + obj(level, name)); }
	void fadeOut(int effect, int speed) override { push("fade:" + std::to_string(effect) + ":" + std::to_string(speed)); }
	void wait(int ms) override { push("wait:" + std::to_string(ms)); }
	void resetRenderQueue() override { push("reset"); }
	void clearMainList() override { push("clearmain"); }
	void releaseHeldItem() override { held = 0; push("bank:pointer:0:0:0:0:0"); }
};

static std::string join(const std::vector<std::string> &v) {
	std::string s; for (size_t i = 0; i < v.size(); ++i) s += (i ? "," : "") + v[i]; return s.empty() ? "-" : s;
}
static std::string trim(const std::string &s) { size_t a = s.find_first_not_of(" \r\n"), b = s.find_last_not_of(" \r\n"); return a == std::string::npos ? "" : s.substr(a, b - a + 1); }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	std::map<std::string, std::pair<int, int>> tally; int shown = 0, caseNo = 0, unhandled = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); size_t p1 = line.find('|'), p2 = line.find('|', p1 + 1);
		std::istringstream in(line.substr(2, p1 - 2));
		uint32 type, ordinal; int jx, jy, jl, bl, ph, st, held;
		in >> type >> ordinal >> jx >> jy >> jl >> bl >> ph >> st >> held;
		std::string events = trim(line.substr(p1 + 1, p2 - p1 - 1)), final = trim(line.substr(p2 + 1));
		++caseNo;
		Host h; h.jx = jx; h.jy = jy; h.jimLevel = jl; h.blockLevel = bl; h.phase = ph; h.step = st; h.held = held;
		if (!SceneRoutines::run(h, type, ordinal)) { ++unhandled; continue; }
		std::string sorts; for (auto &kv : h.sorts) sorts += (sorts.empty() ? "" : ";") + kv.first + "=" + std::to_string(kv.second);
		char fin[400];
		snprintf(fin, sizeof fin, "block=L%d jimlevel=L%d pos=%d,%d bytes=%d,%d held=%d sorts=%s", h.blockLevel, h.jimLevel, h.jx, h.jy, h.phase, h.step, h.held, sorts.empty() ? "-" : sorts.c_str());
		const std::string got = join(h.log);
		auto check = [&](const char *what, bool ok, const std::string &a, const std::string &b) {
			auto &c = tally[what]; ++c.first;
			if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s case %d (type %u line %u)\n  port     %s\n  original %s\n", what, caseNo, type, ordinal, a.c_str(), b.c_str()); }
		};
		check("calls into the rest of the game", got == events, got, events);
		check("state afterwards", std::string(fin) == final, fin, final);
	}
	int bad = 0;
	for (auto &e : tally) { printf("%-40s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("cases with a type the port does not handle: %d\n", unhandled);
	return bad != 0 || unhandled != 0;
}
