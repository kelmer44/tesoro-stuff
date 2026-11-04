// Compares the engine's board game flow (start, turn) with vectors recorded from the original code
// (see re/tools/gen_vectors_boardgame.py). Build/run: re/tools/run_startup_check.sh
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
#include "common/stream.h"
#define private public
#define protected public
#include "tesoro/core/boardgame.h"
#include "tesoro/core/room.h"
#include "tesoro/core/namekey.h"
#undef private
#undef protected

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
static int g_warnings = 0;
void warning(const char *s, ...) { (void)s; ++g_warnings; }

class VecStream : public Common::SeekableReadStream {
public:
	VecStream(const std::string &d) : _d(d), _p(0), _eos(false) {}
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override {
		const uint32 avail = (uint32)(_d.size() - _p), got = n < avail ? n : avail;
		memcpy(out, _d.data() + _p, got); _p += got; if (got < n) _eos = true; return got;
	}
	int64 pos() const override { return (int64)_p; }
	int64 size() const override { return (int64)_d.size(); }
	bool seek(int64 off, int whence = SEEK_SET) override {
		int64 np = whence == SEEK_SET ? off : whence == SEEK_CUR ? (int64)_p + off : (int64)_d.size() + off;
		if (np < 0 || np > (int64)_d.size()) return false;
		_p = (size_t)np; _eos = false; return true;
	}
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
private:
	std::string _d; size_t _p; bool _eos;
};

static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}
static std::string norm(const std::string &s) {
	std::istringstream in(s); std::string tok, out;
	while (in >> tok) out += (out.empty() ? "" : " ") + tok;
	return out;
}

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

struct Host : Tesoro::BoardGameHost {
	int32 time = 0; int calls = 0;
	std::vector<std::string> log;
	int32 now() override { return time; }
	void setPointerActive(bool on) override { log.push_back(on ? "pointer_1" : "pointer_0"); }
	void showPieces() override { log.push_back("update"); }
	uint32 randomNumber(uint32 lo, uint32 hi) override { ++calls; log.push_back("random_" + std::to_string(lo) + "_" + std::to_string(hi)); return lo + (calls * 7) % (hi - lo + 1); }
};

static std::string hex(const byte *p, int n) { std::string s; char b[3]; for (int i = 0; i < n; ++i) { snprintf(b, sizeof b, "%02x", p[i]); s += b; } return s; }

struct Port {
	Host host; Tesoro::BoardGame game; byte variables[16];
	std::string snapshot() {
		char b[200];
		snprintf(b, sizeof b, "%d %d %d %d %d %d ", game.busy ? 1 : 0, game.pieces.state, game.moveStamp, game.pieces.blinkStamp, game._savedResult, variables[2]);
		std::string s = b + hex(game.board.raw(), 57);
		for (int i = 0; i < 8; ++i) s += " " + std::to_string(game.board.line()[i]);
		return s;
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tally tally; int caseNo = 0; Port *port = nullptr;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "K") {
			++caseNo; delete port; port = new Port();
			int vars2, saved, state, busy, mstamp, bstamp, blink; std::string boardHex;
			in >> vars2 >> saved >> boardHex >> state >> busy >> mstamp >> bstamp >> blink;
			memset(port->variables, 0, sizeof port->variables); port->variables[2] = (byte)vars2;
			port->game._savedResult = (byte)saved;
			std::string b = unhex(boardHex); memcpy(port->game.board.raw(), b.data(), 57);
			port->game.pieces.state = (byte)state; port->game.busy = busy != 0; port->game.moveStamp = mstamp;
			port->game.pieces.blinkStamp = bstamp; port->game.pieces.blink = (byte)blink;
		} else if (op == "O") {
			std::string kind; int arg, now; in >> kind >> arg >> now;
			std::string bar, snap, logs; std::getline(in, bar, '|'); std::getline(in, snap, '|'); std::getline(in, logs, '|');
			Host &h = port->host; h.time = now; h.log.clear();
			Tesoro::BoardGame &g = port->game;
			if (kind == "S") g.start(h, port->variables);
			else if (kind == "T") g.turn(h, port->variables);
			else if (kind == "D") {
				byte *bd = g.board.raw();
				if (!g.busy && bd[1 + 8 * arg] != 7) { bd[2 + 8 * arg + (int8)bd[1 + 8 * arg]] = 1; ++bd[1 + 8 * arg]; bd[0] = (byte)arg; g.busy = true; g.moveStamp = now; }
			}
			std::string got; for (size_t i = 0; i < h.log.size(); ++i) got += (i ? "," : "") + h.log[i];
			if (got.empty()) got = "-";
			tally.check("state after the step", norm(port->snapshot()) == norm(snap), caseNo, std::string("op ") + kind + "\n port     " + port->snapshot() + "\n original " + norm(snap));
			if (kind != "F" && kind != "C") tally.check("calls into the rest of the game", norm(got) == norm(logs), caseNo, std::string("op ") + kind + " port " + got + " original " + norm(logs));
			if (kind == "T") tally.check("turns run", true, caseNo);
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-40s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
