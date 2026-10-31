// Compares the engine's ConnectFour with vectors recorded from the original code (see re/tools/gen_vectors_connect.py): the same line scoring, best-move search
// and machine turn must give the same results, board, line cells, score and random-number calls. Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/connectfour.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *, ...) {}

using namespace Tesoro;

struct Host : ConnectFourHost {
	std::vector<std::string> log; int calls = 0;
	uint32 randomNumber(uint32 lo, uint32 hi) override { ++calls; log.push_back("random " + std::to_string(lo) + " " + std::to_string(hi)); return lo + (calls * 7) % (hi - lo + 1); }
};

static std::string hex(const byte *p, int n) { std::string s; char b[3]; for (int i = 0; i < n; ++i) { snprintf(b, sizeof b, "%02x", p[i]); s += b; } return s; }
static std::string unhex(const std::string &h) { std::string r; for (size_t i = 0; i + 1 < h.size(); i += 2) r += (char)strtol(h.substr(i, 2).c_str(), nullptr, 16); return r; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) return 2;
	static char buf[1 << 16];
	std::vector<std::string> lines;
	while (fgets(buf, sizeof buf, f)) { buf[strcspn(buf, "\n")] = 0; lines.push_back(buf); }
	int cases = 0, ops = 0, bad = 0, calls = 0;
	ConnectFour board; Host host;
	for (size_t i = 0; i < lines.size(); ++i) {
		if (lines[i].compare(0, 2, "B ") != 0) continue;
		++cases;
		board.clear();
		const std::string raw = unhex(lines[i].substr(2));
		memcpy(board.raw(), raw.data(), 57);
		int32 outScore = 0;
		bool badCase = false;
		for (i += 2; i < lines.size() && lines[i] != "#"; ) {
			const std::string op = lines[i].substr(2);
			const std::string result = lines[i + 1].substr(2), want = lines[i + 2].substr(2), wantLine = lines[i + 3].substr(2), wantScore = lines[i + 4].substr(2);
			i += 5;
			std::vector<std::string> wantLog;
			while (i < lines.size() && lines[i].compare(0, 2, ". ") == 0) wantLog.push_back(lines[i++].substr(2));
			host.log.clear(); host.calls = 0;
			int got = 0;
			int a = 0, b = 0;
			if (op[0] == 'S') { sscanf(op.c_str() + 2, "%d %d", &a, &b); got = board.score(a, b); }
			else if (op[0] == 'F') { sscanf(op.c_str() + 2, "%d", &a); got = board.bestMove(host, outScore, a); }
			else { sscanf(op.c_str() + 2, "%d", &a); got = (int)board.choose(host, a); }
			std::string line; for (int q = 0; q < 14; ++q) { int32 v = board.line()[q]; line += hex((const byte *)&v, 4); }
			++ops; calls += (int)wantLog.size();
			const std::string sc = op[0] == 'F' ? std::to_string(outScore) : wantScore;
			const bool ok = std::to_string(got) == result && hex(board.raw(), 57) == want && line == wantLine && sc == wantScore && host.log == wantLog;
			if (!ok) { badCase = true; if (bad < 5) printf("case %d op '%s': got %d want %s\n", cases, op.c_str(), got, result.c_str()); break; }
		}
		while (i < lines.size() && lines[i] != "#") ++i;
		if (badCase) ++bad;
	}
	printf("Connect four     %d/%d sessions match (%d operations, %d random-number calls)\n", cases - bad, cases, ops, calls);
	return bad ? 1 : 0;
}
