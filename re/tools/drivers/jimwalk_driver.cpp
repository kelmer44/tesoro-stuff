// Compares the engine's JimWalker with the original walk order / update / collision callback / set facing (see re/tools/gen_vectors_jimwalk.py):
// every session replays the same operations and must make the same calls, in the same order, and leave the same state.
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/jimwalker.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }

void warning(const char *s, ...) { (void)s; }

using namespace Tesoro;

static std::string fmt(const char *f, ...) {
	char buf[256]; va_list ap; va_start(ap, f); vsnprintf(buf, sizeof buf, f, ap); va_end(ap); return buf;
}

struct Target : WalkTarget {
	std::vector<std::string> *log = nullptr; int ret = 1;
	int arrived() override { log->push_back("arrived"); return ret; }
};

struct World : JimWalkerHost {
	std::vector<std::string> log;
	int32 px = 0, py = 0, prevX = 0, prevY = 0;
	int mx = 0, my = 0, scale = 0x100, su = 1, fp = 0, has = 1, ohas = 1;
	int cur = 0; unsigned mask = 0;
	Target target;

	int32 positionX() override { return px; }
	int32 positionY() override { return py; }
	void placeAt(int32 x, int32 y) override { px = x; py = y; }
	void headTo(int32 x, int32 y) override { log.push_back(fmt("move %d %d", x, y)); prevX = x; prevY = y; }
	void stopAtCurrentPosition() override { prevX = px; prevY = py; }
	bool hasWalkArea() override { return has; }
	int32 scaleAt(int32 x, int32 y) override { log.push_back(fmt("scale %d %d", x, y)); return scale; }
	bool findPath(int32 dx, int32 dy, WalkTarget *t) override { log.push_back(fmt("findpath %d %d %s", dx, dy, t == &target ? "tgt" : t ? "?" : "none")); return fp; }
	void insertInMainList(int p) override { log.push_back(fmt("insert %d", p)); }
	void removeFromMainList() override { log.push_back("remove"); }
	bool startWalkAnimation(int facing) override {
		const int bank = 2 + facing;
		if (cur != bank || !((mask >> cur) & 1)) {
			cur = bank; log.push_back("range 1 -1 1 0"); mask |= 1u << cur; return true;
		}
		return false;
	}
	void advanceAnimation(int16 &moveX, int16 &moveY) override { log.push_back("advance"); moveX = mx; moveY = my; }
	void tickAnimation() override { log.push_back("advance"); }
	void showFacing(int f) override { log.push_back(fmt("face %d", f)); }
	int speakerUpdate() override { log.push_back("speakerupdate"); return su; }
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[4096];
	int sessions = 0, bad = 0, ops = 0, calls = 0, shown = 0;
	struct Op { std::string text, result, state; std::vector<std::string> log; };
	while (fgets(line, sizeof line, f)) {
		if (line[0] != 'S') continue;
		std::vector<Op> script;
		while (fgets(line, sizeof line, f) && line[0] != '#') {
			line[strcspn(line, "\n")] = 0;
			if (line[0] == 'o') { script.push_back(Op()); script.back().text = line + 2; }
			else if (line[0] == 'r') script.back().result = line + 2;
			else if (line[0] == 's') script.back().state = line + 2;
			else if (line[0] == '.') script.back().log.push_back(line + 2);
		}
		World world; world.target.log = &world.log;
		JimWalker walker(world);
		++sessions; bool badSession = false;
		for (const Op &op : script) {
			world.log.clear();
			const char kind = op.text[0]; std::string got = "-";
			const char *a = op.text.c_str() + 2;
			if (kind == 'B') { int cur; unsigned mask; sscanf(a, "%d %u", &cur, &mask); world.cur = cur; world.mask = mask; }
			else if (kind == 'J') { int x, y, fc; sscanf(a, "%d %d %d", &x, &y, &fc); world.px = world.prevX = x; world.py = world.prevY = y; walker.setFacingValue(fc); }
			else if (kind == 'E') { sscanf(a, "%d %d %d %d %d %d %d %d", &world.mx, &world.my, &world.scale, &world.su, &world.fp, &world.target.ret, &world.has, &world.ohas); }
			else if (kind == 'W') { int x, y, t, d; sscanf(a, "%d %d %d %d", &x, &y, &t, &d); got = fmt("%u", (uint32)walker.walkTo(x, y, t ? &world.target : nullptr, d != 0)); }
			else if (kind == 'U') got = fmt("%u", (uint32)walker.update());
			else if (kind == 'X') got = fmt("%u", (uint32)walker.touched(world.ohas != 0));
			else if (kind == 'F') { int fc; sscanf(a, "%d", &fc); walker.setFacing(fc); }
			++ops; calls += (int)world.log.size();
			const std::string st = fmt("%d %d %d %d %d %d %s %d %d %d %d %d %d %d %d %d %u", walker.walking(), walker.fixedX(), walker.fixedY(), walker.usedMoveX(), walker.usedMoveY(), walker.direct(),
			                           walker.target() == &world.target ? "tgt" : walker.target() ? "?" : "none", walker.scale(), walker.destX(), walker.destY(), walker.facing(), world.px, world.py,
			                           world.prevX, world.prevY, world.cur, world.mask & 0x1ff);
			if (got != op.result && !(kind == 'E' || kind == 'B' || kind == 'J' || kind == 'F') || world.log != op.log || st != op.state) {
				badSession = true;
				if (shown++ < 6) {
					printf("session op '%s': result got %s want %s\n state got [%s]\n state want [%s]\n", op.text.c_str(), got.c_str(), op.result.c_str(), st.c_str(), op.state.c_str());
					for (size_t i = 0; i < std::max(world.log.size(), op.log.size()); ++i) {
						const std::string g = i < world.log.size() ? world.log[i] : "(none)", w = i < op.log.size() ? op.log[i] : "(none)";
						if (g != w) { printf("   call %zu got '%s' want '%s'\n", i, g.c_str(), w.c_str()); break; }
					}
				}
				break;
			}
		}
		if (badSession) ++bad;
	}
	printf("Jim walking      %d/%d sessions match (%d operations, %d calls to the rest of the game)\n", sessions - bad, sessions, ops, calls);
	return bad ? 1 : 0;
}
