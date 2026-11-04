// Compares the engine's map place (object kind 19) with vectors recorded from the original code
// (see re/tools/gen_vectors_mapplace.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/mapplace.h"
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

struct Fake : Tesoro::Object {
	int32 ax = 0, ay = 0; int fac = 0; std::string *log = nullptr;
	int32 approachX() const override { return ax; }
	int32 approachY() const override { return ay; }
	int facing() const override { return fac; }
	int exitArrival() override { *log += (log->empty() ? "" : ",") + std::string("arrive"); return 0; }
};

struct Host : Tesoro::MapPlaceHost {
	int32 time = 0;
	std::string log;
	Tesoro::InteractiveObject *owner = nullptr;
	Fake salida, canoa;
	char levels[47];
	std::map<std::string, Tesoro::SpeechTable::Line *> lines;
	void add(const std::string &t) { log += (log.empty() ? "" : ",") + t; }
	Tesoro::Object *findObject(int level, const char *name) override {
		const uint64 k = Tesoro::nameKey64(name);
		char t[100]; snprintf(t, sizeof t, "find_%d_%u_%u", level, (unsigned)k, (unsigned)(k >> 32)); add(t);
		if (!strcmp(name, "SALIDA")) return &salida;
		if (!strcmp(name, "CANOA") && level == 7) return &canoa;
		return nullptr;
	}
	int startScript(Tesoro::InteractiveObject &who, const char *block, const char *label, byte *vars) override {
		const uint64 b = Tesoro::nameKey64(block), l = Tesoro::nameKey64(label);
		char t[200]; snprintf(t, sizeof t, "findscript_%u_%u,start_%u_%u_%d_%d", (unsigned)b, (unsigned)(b >> 32), (unsigned)l, (unsigned)(l >> 32), vars ? 1 : 0, &who == owner ? 1 : 0);
		add(t); return 3;
	}
	Tesoro::Level *levelAt(int index) override { return reinterpret_cast<Tesoro::Level *>(&levels[index]); }
	void jimEnterLevel(Tesoro::Level *level, int32 x, int32 y) override {
		char t[100]; snprintf(t, sizeof t, "enter_%d_%d_%d_1", (int)(reinterpret_cast<char *>(level) - levels), x, y); add(t);
	}
	int32 now() override { return time; }
	const Tesoro::SpeechTable::Line *phrase(const char *name) override {
		add(std::string("phrase_") + name);
		Tesoro::SpeechTable::Line *&l = lines[name];
		if (!l) l = new Tesoro::SpeechTable::Line();
		return l;
	}
	bool jimHeld() override { return false; }
	bool isJim(const Tesoro::Object *) override { return false; }
	int jimFacing() override { return 1; }
	void jimFace(int f) override { add("face_" + std::to_string(f)); }
	void jimStopTalking() override {}
	void jimForgetLine() override {}
	void jimSayAnswer(const Tesoro::SpeechTable::Line &) override {}
	void jimWalkTo(int32, int32, Tesoro::WalkTarget *) override {}
	void jimUpdate() override {}
	void setPointerActive(bool) override {}
	void setHoverText(const char *text) override { add(std::string("text_") + (text ? text : "0") + "_-1"); }
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
			int ox, oy, so, ro, now, canoaState, salFacing, sx, sy, verb; std::string dataHex;
			in >> ox >> oy >> so >> ro >> now >> dataHex >> canoaState >> salFacing >> sx >> sy >> verb;
			std::string bar, state, results; std::getline(in, bar, '|'); std::getline(in, state, '|'); std::getline(in, results);
			std::string data = unhex(dataHex);
			Tesoro::ObjectRecord rec; rec.x = ox; rec.y = oy; rec.sortOffset = (int16)so; strcpy(rec.name, "MAPPLACE"); rec.banks.push_back(0); rec.kind = 19;
			rec.extra.resize(0x80); memset(&rec.extra[0], 0, 0x80); memcpy(&rec.extra[0], data.data(), data.size() < 0x80 ? data.size() : 0x80);
			Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(bank);
			host.time = now; host.log.clear();
			Tesoro::MapPlace mp; mp.init(rec, (int16)ro, banks, now, host);
			host.owner = &mp;
			Tesoro::Room room; mp._room = &room;
			const Tesoro::AnimPlayer &p = mp.animation().current();
			const Tesoro::AnimSequence &sq = p.bank()->sequence(p.current());
			char sb[400];
			snprintf(sb, sizeof sb, "%d %d %d %d %d %u %d %d / %u %u %u %u %u %d %u %u %u %d %d /", mp._x, mp._y, mp._previousX, mp._previousY, mp._sortValue, mp._flags,
			         mp.destination(), mp._hoverName ? 1 : 0,
			         mp.animation().bankIndex(), p.first(), p.last(), p.current(), p.flags(), p.startTime(), p.totalDuration(), (uint16)p.loopX(), (uint16)p.loopY(), (int)sq.x, (int)sq.y);
			std::string port = std::string(sb) + " " + (host.log.empty() ? "-" : host.log);
			tally.check("constructed state and phrase lookup", norm(port) == norm(state), caseNo, std::string("\n port     ") + norm(port) + "\n original " + norm(state));
			host.log.clear();
			host.salida.ax = sx; host.salida.ay = sy; host.salida.fac = salFacing; host.salida.log = &host.log;
			host.canoa.stateBlob().clear(); host.canoa.stateBlob().push_back((byte)canoaState);
			const int r = mp.setVerbAndClick(verb);
			char rb[40]; snprintf(rb, sizeof rb, "%d:", r);
			std::string got = std::string(rb) + (host.log.empty() ? "-" : host.log);
			tally.check("click: lookups, travel, script, result", norm(got) == norm(results), caseNo, std::string("\n port     ") + got + "\n original " + norm(results));
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-40s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
