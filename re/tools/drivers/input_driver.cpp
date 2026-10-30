// Compares the engine's InputStates with vectors recorded from the original code (see re/tools/gen_vectors_input.py).
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/inputstate.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	Tesoro::InputStates st;
	int updates = 0, bad = 0, caseNo = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "I") {
			++caseNo; unsigned n, rel, rep; in >> n >> rel >> rep; st.init(n, rel, rep);
			for (unsigned i = 0; i < n; ++i) { std::string tok; in >> tok; unsigned s, t; sscanf(tok.c_str(), "%u:%u", &s, &t); st.set(i, (byte)s, t); }
		} else if (op == "U") {
			unsigned long tick; in >> tick; bool down[8] = {}; std::string tok;
			for (uint i = 0; i < st.count(); ++i) { in >> tok; down[i] = tok == "4"; }
			in >> tok; st.update(down, (uint32)tick); ++updates;
			bool ok = true;
			for (uint i = 0; i < st.count(); ++i) { in >> tok; unsigned s, t; sscanf(tok.c_str(), "%u:%u", &s, &t); ok = ok && st.state(i) == s && st.stamp(i) == t; }
			if (!ok) { ++bad; if (bad < 8) printf("mismatch case %d\n", caseNo); }
		}
	}
	printf("%-28s %d/%d match\n", "input state update", updates - bad, updates);
	return bad ? 1 : 0;
}
