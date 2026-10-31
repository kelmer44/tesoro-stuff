// Compares the engine's inventory strip (object kind 11) with vectors recorded from the original code
// (see re/tools/gen_vectors_strip.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/inventorystrip.h"
#include "tesoro/core/itemgroup.h"
#include "tesoro/core/level.h"
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
static std::vector<int> ints(const std::string &s) {
	std::vector<int> v; std::string cur;
	for (char c : s) { if (c == ',') { v.push_back(atoi(cur.c_str())); cur.clear(); } else cur += c; }
	v.push_back(atoi(cur.c_str())); return v;
}

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

struct Host : Tesoro::StripHost {
	int32 time = 0, texts = 0; bool available = true;
	Tesoro::Object *held = nullptr, *cursor = nullptr;
	int32 now() override { return time; }
	const Tesoro::SpeechTable::Line *phrase(const char *) override { return nullptr; }
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
	void setHoverText(const char *) override { ++texts; }
	void removeFromMainList(Tesoro::Object *) override {}
	void insertInMainList(Tesoro::Object *, int) override {}
	bool hasHeldItem() override { return held != nullptr; }
	bool heldItemReply(uint64, const Tesoro::SpeechTable::Line *&) override { return false; }
	void dropHeldItem() override { if (held) held->setVisible(true); held = nullptr; cursorFrame(0, 0); }
	bool stripAvailable() override { return available; }
	void cursorFrame(int first, int last) override { cursor->animation().selectBank(0, first, last, false, false, time); }
	void stripReady(Tesoro::Object *) override {}
};

static std::string animOf(Tesoro::Object &o) {
	const Tesoro::AnimPlayer &p = o.animation().current();
	char b[200];
	snprintf(b, sizeof b, "%u %u %u %u %u %d %u %u %u", o.animation().bankIndex(), p.first(), p.last(), p.current(), p.flags(), p.startTime(), p.totalDuration(), (uint16)p.loopX(), (uint16)p.loopY());
	return b;
}
static std::string norm(const std::string &s) {
	std::istringstream in(s); std::string tok, out;
	while (in >> tok) out += (out.empty() ? "" : " ") + tok;
	return out;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 22];
	Tesoro::AnimPlayer bank; VecStream *bs = nullptr; std::string bdat;
	Host host; Tally tally; int caseNo = 0;
	std::vector<Tesoro::Object *> pics; Tesoro::Object cursor, square; Tesoro::ItemGroup group; Tesoro::InventoryStrip *strip = nullptr;
	Tesoro::Level *level = nullptr;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "B") {
			std::string hex; in >> hex; bdat = unhex(hex); delete bs; bs = new VecStream(bdat); bank = Tesoro::AnimPlayer();
			tally.check("bank loads", bank.load(*bs, 0, 0, false), caseNo + 1);
		} else if (op == "C") {
			++caseNo;
			for (Tesoro::Object *o : pics) delete o;
			pics.clear(); delete strip; delete level;
			int n, px, sfirst, nowStrip, nc, nsq; std::string ks, xs, ys, npics, bar, dataHex;
			in >> n >> ks >> xs >> ys >> px >> sfirst >> nowStrip >> nc >> nsq >> npics >> bar >> dataHex;
			std::vector<int> kinds = ints(ks), X = ints(xs), Y = ints(ys), NP = ints(npics);
			Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(bank);
			level = new Tesoro::Level();
			auto make = [&](Tesoro::Object &o, const char *name, int nb, int kind, int x, int y, int now) {
				Tesoro::ObjectRecord r; strncpy(r.name, name, 12); r.name[12] = 0;
				for (int i = 0; i < nb; ++i) r.banks.push_back(0);
				o.initAnimated(r, 0, banks, 0);
				for (int b = 0; b < nb; ++b) o.animation().selectBank(b, 0, 0, false, false, now);
				o.animation().switchTo(0);
				o.setKindId((byte)kind); o.setPosition(x, y); o.setFlags(6);
			};
			cursor = Tesoro::Object(); square = Tesoro::Object(); group = Tesoro::ItemGroup();
			for (int i = 0; i < n; ++i) {
				Tesoro::Object *o = new Tesoro::Object(); char nm[16]; snprintf(nm, sizeof nm, "PIC%d", i);
				make(*o, nm, 3, kinds[i], X[i], Y[i], NP[i]); pics.push_back(o); level->objects().pushBack(o);
			}
			make(cursor, "CURSOR", 1, 10, px, 0, nc); make(square, "CUADRADO", 2, 5, 0, 0, nsq);
			level->objects().pushBack(&cursor); level->objects().pushBack(&square);
			std::string data = unhex(dataHex);
			Tesoro::ObjectRecord grec; strcpy(grec.name, "ICONOS"); grec.extra.resize(data.size()); memcpy(&grec.extra[0], data.data(), data.size());
			Common::Array<Tesoro::AnimPlayer> none;
			group.init(grec, 0, none); group._level = level; group.onAttach(); level->objects().pushBack(&group);
			host.cursor = &cursor; host.held = nullptr; host.available = true; host.texts = 0; host.time = nowStrip;
			Tesoro::ObjectRecord srec; strcpy(srec.name, "STRIP"); srec.banks.push_back(0);
			srec.extra.resize(0x200); memset(&srec.extra[0], 0, 0x200); srec.extra[0] = (byte)sfirst;
			strip = new Tesoro::InventoryStrip(); strip->init(srec, 0, banks, nowStrip, host);
			strip->animation().selectBank(0, 0, 0, false, false, nowStrip); strip->animation().switchTo(0);
			strip->_level = level;
		} else if (op == "o") {
			std::string name; int arg, now; in >> name >> arg >> now;
			std::string bar, res, state; std::getline(in, bar, '|'); std::getline(in, res, '|'); std::getline(in, state);
			host.time = now; host.texts = 0; int result = 0;
			if (name == "A") strip->onAttach();
			else if (name == "I") result = strip->acceptsPointer() ? 1 : 0;
			else if (name == "S") result = strip->showHoverText();
			else if (name == "X") result = strip->clearHoverText();
			else if (name == "K") result = strip->setVerbAndClick(0);
			else if (name == "M") cursor.setPosition(arg, cursor.y());
			else if (name == "J") host.available = arg == 0;
			else if (name == "H") host.held = arg < 0 ? nullptr : pics[arg];
			char rb[64]; snprintf(rb, sizeof rb, "%d %d", result, host.texts);
			std::string mine; int hov = -1, heldIdx = -1;
			for (size_t i = 0; i < pics.size(); ++i) { if (strip->_hovered == pics[i]) hov = (int)i; if (host.held == pics[i]) heldIdx = (int)i; }
			char sb[64]; snprintf(sb, sizeof sb, "%d %d | ", strip->_slot, hov); mine = sb;
			mine += animOf(*strip) + " | " + animOf(square) + " | " + animOf(cursor) + " " + std::to_string(heldIdx) + " | ";
			for (size_t i = 0; i < pics.size(); ++i) {
				char pb[100]; snprintf(pb, sizeof pb, "%d %d ", pics[i]->x(), pics[i]->y()); mine += (i ? " ; " : "") + std::string(pb) + animOf(*pics[i]) + " " + std::to_string(pics[i]->flags());
			}
			tally.check("result and text clears", norm(res) == norm(rb), caseNo, std::string("op ") + name + " port " + rb + " original " + norm(res));
			tally.check("state", norm(state) == norm(mine), caseNo, std::string("op ") + name + "\n port     " + norm(mine) + "\n original " + norm(state));
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-24s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
