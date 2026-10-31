// Compares the engine's path sprite (object kind 16) with vectors recorded from the original code
// (see re/tools/gen_vectors_pathsprite.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/pathsprite.h"
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

struct Tally {
	std::map<std::string, std::pair<int, int>> t;
	int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

// Hands back the recorded results of the random functions and checks the arguments.
struct Replay : Tesoro::PathSpriteHost {
	std::vector<std::string> expected; size_t next = 0; bool argsOk = true; int calls = 0;
	void load(const std::string &tokens) {
		expected.clear(); next = 0; argsOk = true; calls = 0;
		if (tokens == "-") return;
		std::istringstream in(tokens); std::string tok;
		while (in >> tok) expected.push_back(tok);
	}
	uint32 take(char tag, uint32 lo, uint32 hi) {
		++calls;
		if (next >= expected.size()) { argsOk = false; return 0; }
		char t; int elo, ehi; long long res;
		sscanf(expected[next++].c_str(), "%c:%d:%d:%lld", &t, &elo, &ehi, &res);
		if (t != tag || elo != (int32)lo || ehi != (int32)hi) argsOk = false;
		return (uint32)res;
	}
	uint32 randomNumber(uint32 lo, uint32 hi) override { return take('N', lo, hi); }
	uint32 randomAlt(uint32 lo, uint32 hi) override { return take('A', lo, hi); }
};

// The player's movement offset is not set by the constructors (the harness leaves it as garbage); the first tick sets it.
static std::string withoutMovement(const std::string &s) {
	std::istringstream in(s); std::string tok, out; int n = 0;
	while (in >> tok) { if (n == 10 || n == 11) tok = "x"; out += (n++ ? " " : "") + tok; }
	return out;
}

static std::string stateOf(Tesoro::PathSprite &p) {
	const Tesoro::AnimPlayer &pl = p.animation().current();
	char b[400];
	snprintf(b, sizeof b, "%d %d %d %d %d %d %d %d %d / %d %d %u %u %d %u %u %u", p._x, p._y, p._stamp, p._wait, p._travelling ? 1 : 0, (int32)p._path,
	         p._point, p._covered, p._scale, pl.movementX(), pl.movementY(), pl.current(), pl.flags(), pl.startTime(), pl.totalDuration(),
	         (uint16)pl.loopX(), (uint16)pl.loopY());
	return b;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 22];
	Tesoro::AnimPlayer anchorPlayer, spritePlayer;
	std::string adat, sdat; VecStream *as = nullptr, *ss = nullptr;
	Tesoro::Object anchor;
	Tesoro::PathSprite *sprite = nullptr;
	Replay host; int caseNo = 0; Tally tally;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "A" || op == "S") {
			std::string hex; in >> hex;
			if (op == "A") { adat = unhex(hex); delete as; as = new VecStream(adat); anchorPlayer = Tesoro::AnimPlayer(); tally.check("anchor loads", anchorPlayer.load(*as, 0, 0, false), caseNo + 1); }
			else { sdat = unhex(hex); delete ss; ss = new VecStream(sdat); spritePlayer = Tesoro::AnimPlayer(); tally.check("sprite loads", spritePlayer.load(*ss, 0, 0, false), caseNo + 1); }
		} else if (op == "K") {
			++caseNo;
			int ox, oy, so, ro, now0; std::string nameHex, dataHex; int ax, ay;
			in >> ox >> oy >> so >> ro >> now0 >> nameHex >> ax >> ay >> dataHex;
			std::string bar, state, rnd; std::getline(in, bar, '|'); std::getline(in, state, '|'); std::getline(in, rnd);
			while (!state.empty() && state[0] == ' ') state.erase(0, 1);
			while (!state.empty() && state.back() == ' ') state.pop_back();
			while (!rnd.empty() && rnd[0] == ' ') rnd.erase(0, 1);
			while (!rnd.empty() && (rnd.back() == '\n' || rnd.back() == ' ')) rnd.pop_back();
			std::string data = unhex(dataHex);
			Tesoro::ObjectRecord rec; rec.x = ox; rec.y = oy; rec.sortOffset = (int16)so; strcpy(rec.name, "SPR"); rec.banks.push_back(0); rec.kind = 16;
			rec.extra.resize(data.size()); memcpy(&rec.extra[0], data.data(), data.size());
			Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(spritePlayer);
			Common::Array<Tesoro::AnimPlayer> abanks; abanks.push_back(anchorPlayer);
			Tesoro::ObjectRecord arec; arec.banks.push_back(0);
			anchor = Tesoro::Object(); anchor.initAnimated(arec, 0, abanks, 0); anchor.setPosition(ax, ay);
			delete sprite; sprite = new Tesoro::PathSprite();
			host.load(rnd);
			sprite->init(rec, (int16)ro, banks, now0, host);
			sprite->_anchor = &anchor;
			tally.check("construct state", withoutMovement(stateOf(*sprite)) == withoutMovement(state), caseNo, "port " + stateOf(*sprite) + " original " + state);
			tally.check("construct random calls", host.argsOk && host.next == host.expected.size(), caseNo);
		} else if (op == "T") {
			int now; in >> now;
			std::string bar, state, rnd; std::getline(in, bar, '|'); std::getline(in, state, '|'); std::getline(in, rnd);
			while (!state.empty() && state[0] == ' ') state.erase(0, 1);
			while (!state.empty() && state.back() == ' ') state.pop_back();
			while (!rnd.empty() && rnd[0] == ' ') rnd.erase(0, 1);
			while (!rnd.empty() && (rnd.back() == '\n' || rnd.back() == ' ')) rnd.pop_back();
			host.load(rnd);
			sprite->updateAnimation(now);
			tally.check("tick state", stateOf(*sprite) == state, caseNo, "port " + stateOf(*sprite) + " original " + state);
			tally.check("tick random calls", host.argsOk && host.next == host.expected.size(), caseNo, "calls " + std::to_string(host.calls) + " vs " + rnd);
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-24s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
