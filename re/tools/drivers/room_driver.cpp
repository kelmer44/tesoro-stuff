// Compares the engine's Room with vectors recorded from the original room code
// (see re/tools/gen_vectors_room.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include "common/scummsys.h"
#include "tesoro/core/object.h"
#include "tesoro/core/room.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

static const int kObjs = 12;

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	static Tesoro::Object objs[kObjs];
	Tesoro::Room room;
	int caseNo = 0, ok = 0, bad = 0;
	std::string lastOp;
	auto idOf = [&](Tesoro::Object *o) { return o ? (int)(o - objs) : -1; };
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") {
			int cap, idx, so, ex; unsigned f4; in >> cap >> idx >> f4 >> so >> ex;
			room.init(idx, f4, (int16)so, (int16)ex); ++caseNo; lastOp = "ctor";
		} else if (op == "in") {
			int i, x; in >> i >> x; objs[i].setPosition(x, 0); room.insertSorted(&objs[i]); lastOp = "in";
		} else if (op == "rm") {
			int i; in >> i; room.removeObject(&objs[i]); lastOp = "rm";
		} else if (op == "S") {
			unsigned n; in >> n; std::string tok, want, got;
			for (unsigned i = 0; i < n; ++i) { in >> tok; want += tok + " "; }
			if (n == 0) in >> tok;
			std::string bar; int last; in >> bar >> last;
			const auto &l = room.objects();
			for (unsigned i = 0; i < l.count(); ++i)
				got += std::to_string(idOf(l.valueAt(i))) + ":" + std::to_string(l.valueAt(i)->x()) + " ";
			bool same = got == want && n == l.count() && idOf(l.value()) == last;
			if (same) ++ok; else { ++bad; if (bad < 12) printf("mismatch case %d after %s: want [%s] last %d got [%s] last %d\n", caseNo, lastOp.c_str(), want.c_str(), last, got.c_str(), idOf(l.value())); }
		}
	}
	printf("room state %d/%d match\n", ok, ok + bad);
	return bad ? 1 : 0;
}
