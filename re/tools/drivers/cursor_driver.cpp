// Compares the engine's Cursor with vectors recorded from the original cursor code
// (see re/tools/gen_vectors_cursor.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/cursor.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

static const int kObjs = 6;
static std::vector<std::string> g_trace;

class Candidate : public Tesoro::Object {
public:
	int id = 0, accept = 0, r14 = 0, r15 = 0, r17 = 0, r18 = 0;
	int setVerbAndClick(int verb) override { g_trace.push_back("14:" + std::to_string(id) + ":" + std::to_string(verb)); return r14; }
	int rightClick(int verb) override { g_trace.push_back("15:" + std::to_string(id) + ":" + std::to_string(verb)); return r15; }
	bool acceptsPointer() override { g_trace.push_back("16:" + std::to_string(id)); return accept != 0; }
	int showHoverText() override { g_trace.push_back("17:" + std::to_string(id)); return r17; }
	int clearHoverText() override { g_trace.push_back("18:" + std::to_string(id)); return r18; }
};
static Candidate *g_objs[kObjs];

static int idOf(Tesoro::Object *o) { for (int i = 0; i < kObjs; ++i) if (g_objs[i] == o) return i; return -1; }
static std::string idsOf(const Tesoro::ObjectList &l) { std::string s; for (uint i = 0; i < l.count(); ++i) s += std::to_string(idOf(l.valueAt(i))) + " "; return s; }
static std::vector<int> parseIds(std::istringstream &in) { std::vector<int> v; std::string t; while (in >> t) if (t != "-") v.push_back(atoi(t.c_str())); return v; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	for (int i = 0; i < kObjs; ++i) g_objs[i] = new Candidate();
	Tesoro::ObjectList previous;
	Tesoro::Cursor cursor;
	int caseNo = 0, ok = 0, bad = 0, ctorOk = 0, ctorBad = 0, buttons = 0, verb = 0, got = 0; std::string gotTrace;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") {
			unsigned flags, sort, label; in >> flags >> sort >> label; ++caseNo;
			previous = Tesoro::ObjectList(); cursor.init(&previous);
			bool same = cursor.flags() == flags && (unsigned)(uint16)cursor.sortValue() == sort && cursor.label().text()[0] == (char)label;
			if (same) ++ctorOk; else { ++ctorBad; ++bad; if (bad < 12) printf("mismatch case %d ctor flags %u want %u\n", caseNo, cursor.flags(), flags); }
		} else if (op == "O") {
			int i, key; Candidate *o = nullptr; in >> i >> key; o = g_objs[i]; o->id = i;
			in >> o->accept >> o->r14 >> o->r15 >> o->r17;
			Tesoro::ObjectRecord rec; snprintf(rec.name, sizeof rec.name, "K%d", key);
			Common::Array<Tesoro::AnimPlayer> banks;
			*static_cast<Tesoro::Object *>(o) = Tesoro::Object(); o->initAnimated(rec, 0, banks, 0);
			o->id = i; in.clear();
		} else if (op == "F") { in >> buttons >> verb; }
		else if (op == "H") { std::vector<int> v = parseIds(in); cursor.hovered().clear(); for (int i : v) cursor.hovered().pushBack(g_objs[i]); }
		else if (op == "K") { std::vector<int> v = parseIds(in); previous.clear(); for (int i : v) previous.pushBack(g_objs[i]); }
		else if (op == "T") { std::vector<int> v = parseIds(in); cursor.clearTouched(); for (int i : v) cursor.onTouched(g_objs[i], 0); }
		else if (op == "L") { cursor.label().set("ABC", 0); }
		else if (op == "SA") { int fl; in >> fl; g_trace.clear(); cursor.setActive(fl != 0); gotTrace.clear(); for (auto &t : g_trace) gotTrace += t + " "; }
		else if (op == "Q") {
			unsigned flags, label; std::string tok, wantTrace, wantH; std::string *dst = &wantTrace; in >> flags >> label;
			while (in >> tok) { if (tok == "|") continue; if (tok == "t") continue; if (tok == "H") { dst = &wantH; continue; } if (tok != "-") *dst += tok + " "; }
			bool same = cursor.flags() == flags && (unsigned)(unsigned char)cursor.label().text()[0] == label && wantTrace == gotTrace && wantH == idsOf(cursor.hovered());
			if (same) ++ok; else { ++bad; if (bad < 12) printf("mismatch case %d setActive: flags %u/%u label %u trace want [%s] got [%s]\n", caseNo, cursor.flags(), flags, label, wantTrace.c_str(), gotTrace.c_str()); }
		}
		else if (op == "P") {
			g_trace.clear(); got = cursor.processCandidates((byte)buttons, verb);
			gotTrace.clear(); for (auto &t : g_trace) gotTrace += t + " ";
		} else if (op == "R") {
			int want; std::string bar, tok; in >> want >> bar >> tok;
			std::string wantTrace, wantH, wantT, wantK; std::string *dst = &wantTrace;
			while (in >> tok) {
				if (tok == "|") continue;
				if (tok == "H") { dst = &wantH; continue; } if (tok == "T") { dst = &wantT; continue; } if (tok == "K") { dst = &wantK; continue; }
				if (tok != "-") *dst += tok + " ";
			}
			bool same = want == got && wantTrace == gotTrace && wantH == idsOf(cursor.hovered()) && wantT == idsOf(cursor.touched()) && wantK == idsOf(previous);
			if (same) ++ok; else { ++bad; if (bad < 12) printf("mismatch case %d: ret %d/%d\n want t[%s] H[%s] T[%s] K[%s]\n got  t[%s] H[%s] T[%s] K[%s]\n", caseNo, want, got, wantTrace.c_str(), wantH.c_str(), wantT.c_str(), wantK.c_str(), gotTrace.c_str(), idsOf(cursor.hovered()).c_str(), idsOf(cursor.touched()).c_str(), idsOf(previous).c_str()); }
		}
	}
	printf("cursor constructor %d/%d, candidate processing %d/%d match\n", ctorOk, ctorOk + ctorBad, ok, ok + bad - ctorBad);
	return bad ? 1 : 0;
}
