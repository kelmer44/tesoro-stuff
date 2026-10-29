// Compares the engine's object-list passes with vectors recorded from the original PassA/PassB
// (see re/tools/gen_vectors_pass.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/cursorlist.h"
#include "tesoro/core/object.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

static const int kObjs = 7;
using ObjectList = Tesoro::CursorList<Tesoro::Object *>;
static ObjectList g_list;
static std::vector<int> g_trace;

struct Op { bool insert; int target; int priority; };
class ScriptObject;
static ScriptObject *g_objs[kObjs];

class ScriptObject : public Tesoro::Object {
public:
	int id = 0;
	std::vector<Op> ops[2];
	int ret[2] = {0, 0};
	int run(int slot);
	int update() override { return run(0); }
	int update2() override { return run(1); }
};

int ScriptObject::run(int slot) {
	g_trace.push_back(id);
	for (const Op &op : ops[slot]) {
		if (op.insert) g_list.insertByPriority(g_objs[op.target], op.priority);
		else g_list.removeAndRewind(g_objs[op.target]);
	}
	return ret[slot];
}

static int idOf(Tesoro::Object *o) { for (int i = 0; i < kObjs; ++i) if (g_objs[i] == o) return i; return -1; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	for (int i = 0; i < kObjs; ++i) { g_objs[i] = new ScriptObject(); g_objs[i]->id = i; }
	int caseNo = 0, ok = 0, bad = 0; char pass = 'a';
	auto report = [&](bool same, const std::string &msg) { if (same) ++ok; else { ++bad; if (bad < 12) printf("mismatch case %d: %s\n", caseNo, msg.c_str()); } };
	int wantRet = 0; std::string wantTrace, gotTrace;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") { g_list = ObjectList(); ++caseNo; for (auto *o : g_objs) { o->ops[0].clear(); o->ops[1].clear(); } }
		else if (op == "O") { int i; unsigned fl; in >> i >> fl; g_objs[i]->setFlags(fl); }
		else if (op == "L") { int i, p; in >> i >> p; g_list.insertByPriority(g_objs[i], p); }
		else if (op == "A") {
			int i, slot, r, n; in >> i >> slot >> r >> n; ScriptObject *o = g_objs[i]; int s = slot - 4;
			o->ret[s] = r; o->ops[s].clear();
			for (int k = 0; k < n; ++k) { std::string kind; Op p; in >> kind >> p.target >> p.priority; p.insert = kind == "ins"; o->ops[s].push_back(p); }
		} else if (op == "P") {
			in >> pass; g_trace.clear();
			wantRet = -1;
			int r = pass == 'a' ? Tesoro::runUpdatePass(g_list) : Tesoro::runUpdate2Pass(g_list);
			gotTrace.clear(); for (int t : g_trace) gotTrace += std::to_string(t) + " ";
			wantRet = r;
		} else if (op == "R") {
			int r; std::string bar, t; in >> r >> bar >> t;
			std::string want; std::string tok; while (in >> tok) want += (tok == "-" ? "" : tok + " ");
			report(r == wantRet && want == gotTrace, "pass " + std::string(1, pass) + " want ret " + std::to_string(r) + " trace [" + want + "] got ret " + std::to_string(wantRet) + " [" + gotTrace + "]");
		} else if (op == "S") {
			unsigned n; in >> n; std::string tok, want, got;
			for (unsigned i = 0; i < n; ++i) { in >> tok; want += tok + " "; }
			for (unsigned i = 0; i < g_list.count(); ++i) got += std::to_string(idOf(g_list.valueAt(i))) + ":" + std::to_string(g_list.priorityAt(i)) + " ";
			report(want == got, "list want [" + want + "] got [" + got + "]");
		} else if (op == "F") {
			std::string want, got, tok; while (in >> tok) want += tok + " ";
			for (auto *o : g_objs) got += std::to_string(o->flags()) + " ";
			report(want == got, "flags want [" + want + "] got [" + got + "]");
		}
	}
	printf("object list passes %d/%d match\n", ok, ok + bad);
	return bad ? 1 : 0;
}
