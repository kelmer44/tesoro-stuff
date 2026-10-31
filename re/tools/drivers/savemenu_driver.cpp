// Compares the engine's save and load screen objects (kinds 22 and 23) with vectors recorded from the original code
// (see re/tools/gen_vectors_savemenu.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/camera.h"
#include "tesoro/core/display.h"
#include "tesoro/core/menubutton.h"
#include "tesoro/core/slotlabel.h"
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

struct NullDisplay : Tesoro::Display {
	void drawImage(int32, int32, uint16, const Tesoro::AnimImage &, uint32) override {}
};

static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}
static std::string hexOf(const std::string &s) {
	std::string r; char b[4];
	for (unsigned char c : s) { snprintf(b, sizeof b, "%02x", c); r += b; }
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

struct Host : Tesoro::MenuButtonHost, Tesoro::SlotLabelHost {
	int32 time = 0; bool subs = false; int32 camX = 0; int slot = 0; std::string date; std::vector<std::string> events;
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
	void setHoverText(const char *text) override { events.push_back(std::string("T:") + (text ? text : "-")); }
	void removeFromMainList(Tesoro::Object *) override {}
	void insertInMainList(Tesoro::Object *, int) override {}
	bool hasHeldItem() override { return false; }
	bool heldItemReply(uint64, const Tesoro::SpeechTable::Line *&) override { return false; }
	void dropHeldItem() override {}
	void previousSlot() override { events.push_back("prev"); }
	void nextSlot() override { events.push_back("next"); }
	void loadSlot() override { events.push_back("load"); }
	void saveSlot() override { events.push_back("save"); }
	void fadeOut(int effect, int speed) override { events.push_back("F:" + std::to_string(effect) + ":" + std::to_string(speed)); }
	void clearMainList() override { events.push_back("C"); }
	void scrollInterfaceOut() override { events.push_back("S"); }
	bool toggleSubtitles() override { subs = !subs; return subs; }
	int32 interfaceCameraX() override { return camX; }
	int selectedSlot() override { return slot; }
	const char *slotTime() override { return date.c_str(); }
	void drawMenuText(const char *text, uint32 colour, int32 x, int32 y, int32 width) override {
		events.push_back("W:" + hexOf(text) + ":" + std::to_string(colour) + ":" + std::to_string(x) + ":" + std::to_string(y) + ":" + std::to_string(width));
	}
};

static std::string animOf(Tesoro::Object &o) {
	const Tesoro::AnimPlayer &p = o.animation().current();
	char b[200];
	snprintf(b, sizeof b, "%u %u %u %u %u %d %u %u %u", o.animation().bankIndex(), p.first(), p.last(), p.current(), p.flags(), p.startTime(), p.totalDuration(), (uint16)p.loopX(), (uint16)p.loopY());
	return b;
}
static std::string join(const std::vector<std::string> &v) {
	std::string r;
	for (size_t i = 0; i < v.size(); ++i) r += (i ? ";" : "") + v[i];
	return r.empty() ? "-" : r;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer bank; VecStream *bs = nullptr; std::string bdat;
	Host host; Tally tally; int caseNo = 0;
	Tesoro::MenuButton *button = nullptr; Tesoro::SlotLabel *label = nullptr;
	NullDisplay display;
	Tesoro::Camera camera; camera.init();
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "B") {
			std::string hex; in >> hex; bdat = unhex(hex); delete bs; bs = new VecStream(bdat); bank = Tesoro::AnimPlayer();
			tally.check("bank loads", bank.load(*bs, 0, 0, false), caseNo + 1);
		} else if (op == "K22" || op == "K23") {
			++caseNo; delete button; delete label; button = nullptr; label = nullptr;
			int ox, oy, so, ro, now, jim = 0; std::string dataHex;
			in >> ox >> oy >> so >> ro >> now >> dataHex; if (op == "K22") in >> jim;
			std::string bar, state; std::getline(in, bar, '|'); std::getline(in, state);
			std::string data = unhex(dataHex);
			Tesoro::ObjectRecord rec; rec.x = ox; rec.y = oy; rec.sortOffset = (int16)so; strcpy(rec.name, "BTN"); rec.banks.push_back(0);
			rec.extra.resize(0x80); memset(&rec.extra[0], 0, 0x80); memcpy(&rec.extra[0], data.data(), data.size() < 0x80 ? data.size() : 0x80);
			Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(bank);
			host.time = now; host.subs = jim != 0; host.events.clear();
			char sb[300];
			if (op == "K22") {
				button = new Tesoro::MenuButton(); button->init(rec, (int16)ro, banks, now, host);
				snprintf(sb, sizeof sb, "%d %u %u / %s", button->_sortValue, button->_flags, button->_action, animOf(*button).c_str());
			} else {
				label = new Tesoro::SlotLabel(); label->init(rec, (int16)ro, banks, now, host);
				snprintf(sb, sizeof sb, "%d %u %d / %s", label->_sortValue, label->_flags, label->_caption ? 1 : 0, animOf(*label).c_str());
			}
			tally.check("constructed state", norm(sb) == norm(state), caseNo, std::string("\n port     ") + norm(sb) + "\n original " + norm(state));
		} else if (op == "o") {
			std::string name; int verb, now; in >> name >> verb >> now;
			std::string bar, res, events, state; std::getline(in, bar, '|'); std::getline(in, res, '|'); std::getline(in, events, '|'); std::getline(in, state);
			host.time = now; host.events.clear(); int result = 0;
			if (name == "I") result = button->acceptsPointer() ? 1 : 0;
			else if (name == "X") result = button->clearHoverText();
			else if (name == "H") result = button->showHoverText();
			else if (name == "K") result = button->setVerbAndClick(verb);
			else if (name == "R") result = button->rightClick(verb);
			char sb[300]; snprintf(sb, sizeof sb, "%s / %d", animOf(*button).c_str(), host.subs ? 1 : 0);
			tally.check("result", atoi(res.c_str()) == result, caseNo, std::string("op ") + name + " port " + std::to_string(result) + " original " + norm(res));
			tally.check("events", norm(events) == norm(join(host.events)), caseNo, std::string("op ") + name + " port " + join(host.events) + " original " + norm(events));
			tally.check("state after", norm(sb) == norm(state), caseNo, std::string("op ") + name + "\n port     " + norm(sb) + "\n original " + norm(state));
		} else if (op == "q") {
			int r[5]; for (int &x : r) in >> x;
			int mine[5] = { label->setVerbAndClick(1), label->rightClick(1), label->acceptsPointer() ? 1 : 0, label->showHoverText(), label->clearHoverText() };
			bool ok = true; for (int i = 0; i < 5; ++i) ok = ok && r[i] == mine[i];
			tally.check("caption slots 14 to 18", ok, caseNo);
		} else if (op == "d") {
			int camx, slot; std::string tm; in >> camx >> slot >> tm;
			std::string bar, events; std::getline(in, bar, '|'); std::getline(in, events);
			host.camX = camx; host.slot = slot; host.date = unhex(tm); host.events.clear();
			label->draw(camera, display);
			// The original asks the base to draw too (a "B" event); the port's base draw has no visible effect here.
			std::string mine = join(host.events), want = norm(events);
			if (want.size() >= 1 && (want == "B" || want.compare(want.size() - 2, 2, ";B") == 0)) want = want == "B" ? "-" : want.substr(0, want.size() - 2);
			tally.check("caption drawing", norm(mine) == want, caseNo, "port " + mine + " original " + norm(events));
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-24s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
