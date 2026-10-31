// Compares the engine's InteractiveObject with vectors recorded from the original code (see re/tools/gen_vectors_interactive.py): the same
// constructor, click, update, arrival and hover operations must leave the same fields and make the same calls to the rest of the game.
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#define private public
#define protected public
#include "tesoro/core/interactive.h"
#undef private
#undef protected

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

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
	if (s == "-") return r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

using namespace Tesoro;

// The world the object asks: what the vectors set, and one log line per call.
struct World : InteractiveHost {
	std::vector<std::string> log;
	std::map<std::string, SpeechTable::Line *> lines;
	Object *self = nullptr;
	int32 time = 0;
	bool held = false, talking = false, jimIsObj = false, item = false, dropped = false;
	int facing = 0, heldBit = 0;
	std::vector<std::pair<bool, int>> replies;

	std::string nameOf(const SpeechTable::Line *l) { return !l ? "null" : std::string(l->str() + 2); }
	SpeechTable::Line *line(const std::string &name) {
		SpeechTable::Line *&l = lines[name];
		if (!l) {
			l = new SpeechTable::Line();
			l->key = 0; l->wavOffset = 0;
			const std::string t = "T:" + name;
			for (char c : t) l->text.push_back(c);
			l->text.push_back('\0');
		}
		return l;
	}
	void reset() { log.clear(); time = 0; held = talking = jimIsObj = item = dropped = false; facing = heldBit = 0; replies.clear(); }

	int32 now() override { return time; }
	const SpeechTable::Line *phrase(const char *name) override { log.push_back(std::string("phrase ") + name); return line(name); }
	bool jimHeld() override { return held && !jimIsObj; }
	bool isJimObject(const Object *o) { return jimIsObj && o == self; }
	bool isJim(const Object *o) override { return isJimObject(o); }
	int jimFacing() override { return facing; }
	void jimFace(int f) override { log.push_back("face " + std::to_string(f)); }
	void jimStopTalking() override { log.push_back("stoptalk"); }
	void jimForgetLine() override {}
	// The colour and flag are Jim's own and are checked with Jim; here they are zero in both worlds.
	void jimSayAnswer(const SpeechTable::Line &l) override { log.push_back("say " + nameOf(&l) + " 0 0"); }
	void jimWalkTo(int32 x, int32 y, WalkTarget *target) override {
		log.push_back("walk " + std::to_string(x) + " " + std::to_string(y) + (target == static_cast<WalkTarget *>(static_cast<InteractiveObject *>(self)) ? " obj" : " ?") + " 0");
	}
	void jimUpdate() override { log.push_back("jimupdate"); }
	void setPointerActive(bool on) override { log.push_back(std::string("pointer ") + (on ? "1" : "0")); }
	void setHoverText(const char *text) override { log.push_back(std::string("hover ") + (text ? text : "null")); }
	void removeFromMainList(Object *) override { log.push_back("remove"); }
	void insertInMainList(Object *, int p) override { log.push_back("insert " + std::to_string(p)); }
	bool hasHeldItem() override { return item && !dropped; }
	bool heldItemReply(uint64, const SpeechTable::Line *&l) override {
		for (auto &r : replies) if (r.first) { l = line("RP" + std::to_string(r.second)); return true; }
		return false;
	}
	void dropHeldItem() override { log.push_back("drop"); heldBit = 4; dropped = true; }
};

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

static std::vector<std::string> split(const std::string &s) { std::istringstream in(s); std::vector<std::string> v; std::string t; while (in >> t) v.push_back(t); return v; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	AnimPlayer player; VecStream *stream = nullptr; std::string dat;
	InteractiveObject *obj = nullptr; World world; Tally tally;
	int caseNo = 0; bool loaded = false;
	const long long U32 = 0xcdcdcdcdLL, U16 = 0xcdcd;
	struct Block { std::string op, result, state; std::vector<std::string> log; };
	std::vector<std::string> lines;
	while (fgets(buf, sizeof buf, f)) { buf[strcspn(buf, "\n")] = 0; lines.push_back(buf); }
	auto num = [](const std::string &s) { return atoll(s.c_str()); };
	for (size_t li = 0; li < lines.size(); ++li) {
		const std::string &ln = lines[li];
		if (ln.compare(0, 2, "C ") == 0) {
			++caseNo; std::vector<std::string> t = split(ln);
			dat = unhex(t[1]); delete stream; stream = new VecStream(dat); player = AnimPlayer();
			loaded = player.load(*stream, (uint)num(t[2]), 0, false);
			tally.check("load succeeds", loaded, caseNo);
			obj = nullptr; world.reset();
			continue;
		}
		if (!loaded) continue;
		std::vector<std::string> head = split(ln);
		if (head.empty()) continue;
		bool isK = head[0] == "eK";
		if (head[0] == "eK" || ln.compare(0, 2, "o ") == 0) {
			Block b; if (!isK) b.op = ln.substr(2);
			size_t j = li + (isK ? 1 : 0);
			if (isK) { b.op = lines[j].substr(2); }
			// o line is at j; read r, s and the log lines after it
			Block blk; blk.op = lines[j].substr(2);
			size_t k = j + 1;
			if (k < lines.size() && lines[k].compare(0, 2, "r ") == 0) blk.result = lines[k++].substr(2);
			if (k < lines.size() && lines[k].compare(0, 2, "s ") == 0) blk.state = lines[k++].substr(2);
			while (k < lines.size() && lines[k].compare(0, 2, ". ") == 0) blk.log.push_back(lines[k++].substr(2));
			li = k - 1;
			std::string got = "-";
			world.log.clear();
			std::vector<std::string> ot = split(blk.op);
			if (isK) {
				Common::Array<AnimPlayer> table; for (long long q = 0; q < num(head[1]); ++q) table.push_back(player);
				ObjectRecord r; r.x = (int32)num(head[2]); r.y = (int32)num(head[3]); r.sortOffset = (int16)num(head[4]);
				strncpy(r.name, head[6].c_str(), 12);
				{ std::istringstream l(head[10]); std::string t2; while (std::getline(l, t2, ',')) r.banks.push_back((byte)atoi(t2.c_str())); }
				const std::string data = unhex(head[11]); for (size_t q = 0; q < data.size(); ++q) r.extra.push_back((byte)data[q]);
				obj = new InteractiveObject(); world.self = obj;
				obj->init(r, (int16)num(head[5]), table, (int32)num(head[9]), world);
			} else if (!obj) {
				continue;
			} else if (ot[0] == "E") {
				world.time = (int32)num(ot[1]); world.held = num(ot[2]); world.talking = num(ot[3]); world.facing = (int)num(ot[4]);
				world.jimIsObj = num(ot[5]); world.item = num(ot[6]); world.dropped = false; world.heldBit = 0; world.replies.clear();
				if (ot[7] != "-") { std::istringstream l(ot[7]); std::string t2; while (std::getline(l, t2, ',')) world.replies.push_back({t2[0] == '1', atoi(t2.c_str() + 2)}); }
			} else if (ot[0] == "V") got = std::to_string(obj->setVerbAndClick((int)num(ot[1])));
			else if (ot[0] == "R") got = std::to_string(obj->rightClick((int)num(ot[1])));
			else if (ot[0] == "I") got = std::to_string(obj->useItem((int)num(ot[1])));
			else if (ot[0] == "U") got = std::to_string(obj->update());
			else if (ot[0] == "A") obj->arrived();
			else if (ot[0] == "H") got = std::to_string(obj->showHoverText());
			else if (ot[0] == "C") got = std::to_string(obj->clearHoverText());
			// state: flags pending answered facing ax ay verb hover v0..v6 shown heldBit held current(8 fields)
			std::vector<std::string> st = split(blk.state);
			bool ok = st.size() >= 25; std::string why;
			auto name = [&](const SpeechTable::Line *l) { return world.nameOf(l); };
			if (ok) {
				auto bad = [&](bool c, const char *w) { if (!c && why.empty()) why = w; return c; };
				ok = bad((long long)obj->flags() == num(st[0]), "flags") && bad(obj->_pending == num(st[1]), "pending") && bad(obj->_answered == num(st[2]), "answered") && bad(obj->_facing == num(st[3]), "facing") &&
					bad(obj->_approachX == num(st[4]), "ax") && bad(obj->_approachY == num(st[5]), "ay") && bad(obj->_verb == num(st[6]), "verb") && bad(name(obj->_hoverName) == st[7], "hover");
				for (int v = 0; ok && v < 7; ++v) ok = bad(name(obj->_verbLine[v]) == st[8 + v], "verbline");
				ok = ok && bad(obj->shownSequence() == num(st[15]), "shown") && bad(world.item ? world.heldBit == num(st[16]) : num(st[16]) == -1, "heldbit") &&
					bad(std::string(world.hasHeldItem() ? "held" : "none") == st[17], "heldpresence");
				AnimPlayer &p = obj->animation().current();
				auto eq = [&](long long g, size_t i, long long wild) { return num(st[18 + i]) == wild || g == num(st[18 + i]); };
				if (ok && !(eq(p.flags(), 0, U32) && eq(p.startTime(), 1, -842150451) && eq(p.totalDuration(), 2, 3452816845LL) && eq(p.current(), 3, U16) && eq(p.first(), 4, U16) && eq(p.last(), 5, U16) && eq((uint16)p.loopX(), 6, U16) && eq((uint16)p.loopY(), 7, U16))) why = "player";
				ok = ok && eq(p.flags(), 0, U32) && eq(p.startTime(), 1, -842150451) && eq(p.totalDuration(), 2, 3452816845LL) && eq(p.current(), 3, U16) &&
					eq(p.first(), 4, U16) && eq(p.last(), 5, U16) && eq((uint16)p.loopX(), 6, U16) && eq((uint16)p.loopY(), 7, U16);
			}
			const char *label = isK ? "constructor" : ot[0] == "E" ? "world" : ot[0] == "V" || ot[0] == "R" || ot[0] == "I" ? "click" : ot[0] == "U" ? "update" : ot[0] == "A" ? "arrival" : "hover";
			tally.check((std::string("state after ") + label).c_str(), ok, caseNo, ok ? "" : "op " + blk.op + " field " + why);
			if (blk.result != "-" && !isK && ot[0] != "E") tally.check("results", got == blk.result, caseNo, "op " + blk.op + " got " + got + " want " + blk.result);
			const bool same = world.log == blk.log;
			tally.check("calls", same, caseNo, same ? "" : "op " + blk.op);
			if (!same && tally.shown <= 12) {
				for (size_t q = 0; q < std::max(world.log.size(), blk.log.size()); ++q) {
					const std::string g = q < world.log.size() ? world.log[q] : "(none)", w = q < blk.log.size() ? blk.log[q] : "(none)";
					if (g != w) { printf("   call %zu got '%s' want '%s'\n", q, g.c_str(), w.c_str()); break; }
				}
			}
		}
	}
	int bad = 0;
	for (auto &kv : tally.t) { printf("%-34s %d/%d match\n", kv.first.c_str(), kv.second.first - kv.second.second, kv.second.first); bad += kv.second.second; }
	return bad ? 1 : 0;
}
