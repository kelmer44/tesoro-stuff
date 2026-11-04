// Compares the engine's board column button (object kind 21) with vectors recorded from the original code
// (see re/tools/gen_vectors_boardcol.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/boardcolumn.h"
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

struct Host : Tesoro::BoardColumnHost {
	int32 time = 0;
	std::string log;
	bool busy = false; int busyByte = 0; int32 stamp = 0;
	Tesoro::ConnectFour game;
	std::map<std::string, Tesoro::SpeechTable::Line *> lines;
	void add(const std::string &t) { log += (log.empty() ? "" : ",") + t; }
	bool boardBusy() override { return busyByte != 0; }
	void setBoardBusy() override { busyByte = 1; }
	Tesoro::ConnectFour &board() override { return game; }
	void stampMoveTime() override { stamp = time; add("stamp"); }
	void showPieces() override { add("update"); }
	int32 now() override { return time; }
	const Tesoro::SpeechTable::Line *phrase(const char *name) override {
		{ std::string h; static const char *dg = "0123456789abcdef"; for (const char *c = name; *c; ++c) { h += dg[(*c >> 4) & 15]; h += dg[*c & 15]; } add("phrase_" + h); }
		Tesoro::SpeechTable::Line *&l = lines[name];
		if (!l) l = new Tesoro::SpeechTable::Line();
		return l;
	}
	bool jimHeld() override { return false; }
	bool isJim(const Tesoro::Object *) override { return false; }
	int jimFacing() override { return 1; }
	void jimFace(int) override {}
	void jimStopTalking() override {}
	void jimForgetLine() override {}
	void jimSayAnswer(const Tesoro::SpeechTable::Line &) override {}
	void jimWalkTo(int32, int32, Tesoro::WalkTarget *) override {}
	void jimUpdate() override {}
	void setPointerActive(bool) override {}
	void setHoverText(const char *) override {}
	void removeFromMainList(Tesoro::Object *) override {}
	void insertInMainList(Tesoro::Object *, int) override {}
	bool hasHeldItem() override { return false; }
	bool heldItemReply(uint64, const Tesoro::SpeechTable::Line *&) override { return false; }
	void dropHeldItem() override {}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer bank; VecStream *bs = nullptr; std::string bdat;
	Host host; Tally tally; int caseNo = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "B") {
			std::string hex; in >> hex; bdat = unhex(hex); delete bs; bs = new VecStream(bdat); bank = Tesoro::AnimPlayer();
			tally.check("bank loads", bank.load(*bs, 0, 0, false), caseNo + 1);
		} else if (op == "K") {
			++caseNo;
			int ox, oy, so, ro, now, busyIn, verb; std::string dataHex, boardHex;
			in >> ox >> oy >> so >> ro >> now >> dataHex >> busyIn >> boardHex >> verb;
			std::string bar, state, results; std::getline(in, bar, '|'); std::getline(in, state, '|'); std::getline(in, results);
			std::string data = unhex(dataHex);
			Tesoro::ObjectRecord rec; rec.x = ox; rec.y = oy; rec.sortOffset = (int16)so; strcpy(rec.name, "OBJCOL"); rec.banks.push_back(0); rec.kind = 21;
			rec.extra.resize(0x80); memset(&rec.extra[0], 0, 0x80); memcpy(&rec.extra[0], data.data(), data.size() < 0x80 ? data.size() : 0x80);
			Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(bank);
			host.time = now; host.log.clear();
			Tesoro::BoardColumn bc; bc.init(rec, (int16)ro, banks, now, host);
			Tesoro::Room room; bc._room = &room;
			const Tesoro::AnimPlayer &p = bc.animation().current();
			char sb[400];
			snprintf(sb, sizeof sb, "%d %d %d %d %d %u %d %d / %u %u %u %u %u %d %u %u %u /", bc._x, bc._y, bc._previousX, bc._previousY, bc._sortValue, bc._flags,
			         bc.column(), bc._hoverName ? 1 : 0,
			         bc.animation().bankIndex(), p.first(), p.last(), p.current(), p.flags(), p.startTime(), p.totalDuration(), (uint16)p.loopX(), (uint16)p.loopY());
			std::string port = std::string(sb) + " " + (host.log.empty() ? "-" : host.log);
			tally.check("constructed state and phrase lookup", norm(port) == norm(state), caseNo, std::string("\n port     ") + norm(port) + "\n original " + norm(state));
			if (norm(results) != "skip") {
				host.log.clear(); host.busyByte = busyIn; host.stamp = (int32)0xdeadbeef;
				std::string b = unhex(boardHex); memcpy(host.game.raw(), b.data(), 57);
				const int r = bc.setVerbAndClick(verb);
				std::string hex; static const char *dig = "0123456789abcdef";
				for (int i = 0; i < 57; ++i) { hex += dig[host.game.raw()[i] >> 4]; hex += dig[host.game.raw()[i] & 15]; }
				char rb[60]; snprintf(rb, sizeof rb, "%d:", r);
				std::string got = std::string(rb) + (host.log.empty() ? "-" : host.log) + ":" + std::to_string(host.busyByte) + ":" + std::to_string((uint32)host.stamp) + ":" + hex;
				tally.check("click: busy, full column, drop, order of calls", norm(got) == norm(results), caseNo, std::string("\n port     ") + got + "\n original " + norm(results));
			}
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-40s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
