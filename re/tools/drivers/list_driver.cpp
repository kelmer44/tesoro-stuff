// Compares the engine's CursorList with vectors recorded from the original list code
// (see re/tools/gen_vectors_list.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/cursorlist.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	Tesoro::CursorList<uint32> list;
	bool objectList = false;
	std::map<std::string, std::pair<int, int>> tally; int shown = 0, caseNo = 0;
	auto check = [&](const char *what, bool ok, const std::string &detail) {
		auto &c = tally[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	};
	std::string lastOp;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") { std::string k; unsigned cap; in >> k >> cap; list = Tesoro::CursorList<uint32>(); objectList = k == "O"; ++caseNo; lastOp = "ctor"; }
		else if (op == "S") {
			unsigned n; in >> n; bool ok = list.count() == n; std::string tok, got, want;
			for (unsigned i = 0; i < n; ++i) {
				in >> tok; want += tok + " ";
				if (i < list.count()) got += std::to_string(list.valueAt(i)) + ":" + std::to_string(objectList ? list.priorityAt(i) : 0) + " ";
			}
			ok = ok && got == want;
			in >> tok;	// "-" for an empty list was consumed as the entry list
			std::string bar; unsigned long long dv; long long dp;
			if (n == 0) { in >> bar; } else { bar = tok; }
			in >> dv >> dp;
			ok = ok && list.value() == dv && list.priority() == dp;
			check(objectList ? "object list state" : "list state", ok, lastOp + " want [" + want + "] got [" + got + "] data " + std::to_string(dv) + " " + std::to_string(dp));
		} else {
			unsigned v = 0; int p = 0, r = 0;
			lastOp = op;
			if (op == "pb") { in >> v; list.pushBack(v); }
			else if (op == "rm") { in >> v >> r; check("list remove result", list.remove(v) == (r != 0), "rm " + std::to_string(v)); }
			else if (op == "cl") list.clear();
			else if (op == "rw") list.rewind();
			else if (op == "ia") { in >> v; list.insertAfterCursor(v); }
			else if (op == "io") { in >> v >> p; list.insertByPriority(v, p); }
			else if (op == "if") { in >> v >> p; list.insertByPriorityFront(v, p); }
			else if (op == "ro") { in >> v >> r; check("object remove result", list.removeAndRewind(v) == (r != 0), "ro " + std::to_string(v)); }
			else if (op == "ef") { in >> r; check("start iteration result", list.startIteration() == (r != 0), "ef"); }
		}
	}
	int bad = 0;
	for (auto &kv : tally) { printf("%-28s %d/%d match\n", kv.first.c_str(), kv.second.first - kv.second.second, kv.second.first); bad += kv.second.second; }
	return bad ? 1 : 0;
}
