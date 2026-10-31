// Compares the engine's InventoryItem (kind 13) with vectors recorded from the original code (see re/tools/gen_vectors_items.py): the same
// constructor, say, update, draw, pose, face, click, arrival and hover operations must leave the same fields and make the same calls to the rest of the game.
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
#include "tesoro/core/inventoryitem.h"
#include "tesoro/core/namekey.h"
#include "tesoro/core/level.h"
#include "tesoro/core/camera.h"
#include "tesoro/core/display.h"
#include "tesoro/core/room.h"
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
struct Probe : Object {
	std::vector<std::string> *log = nullptr; int index = 0;
	int verb(int v) override { if (v == 0 && log) log->push_back("verb0 " + std::to_string(index)); return 0; }
};

struct World;
struct HeldProbe : InventoryItem {
	World *world = nullptr;
	int useItem(int n) override;
};

struct World : ItemHost {
	std::vector<std::string> log;
	std::map<std::string, SpeechTable::Line *> lines;
	Object *self = nullptr;
	int32 time = 0;
	bool held = false;
	bool carryingHeld = false, carryingSelf = false;
	int jstep = 0, jlevel = 1, jbyte0 = 0, jflags = 6, camx = 0, hu = 0, counter = 0, salt = 0;
	Probe pool[8];
	HeldProbe heldProbe;
	std::map<std::pair<uint32, uint32>, int> special;

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
	void reset() { log.clear(); time = 0; held = false; carryingHeld = carryingSelf = false; jstep = 0; jlevel = 1; jbyte0 = 0; jflags = 6; camx = 0; hu = 0; counter = 0; special.clear(); }

	int32 now() override { return time; }
	const SpeechTable::Line *phrase(const char *name) override { log.push_back(std::string("phrase ") + name); return line(name); }
	bool jimHeld() override { return held; }
	bool isJim(const Object *) override { return false; }
	int jimFacing() override { return 0; }
	void jimFace(int f) override { log.push_back("face " + std::to_string(f)); }
	void jimStopTalking() override { log.push_back("stoptalk"); }
	void jimForgetLine() override {}
	void jimSayAnswer(const SpeechTable::Line &l) override { log.push_back("say " + nameOf(&l) + " 0 0"); }
	void jimWalkTo(int32, int32, WalkTarget *) override {}
	void jimUpdate() override { log.push_back("jimupdate"); }
	void setPointerActive(bool on) override { log.push_back(std::string("pointer ") + (on ? "1" : "0")); }
	void setHoverText(const char *text) override { log.push_back(std::string("hover ") + (text ? text : "null")); }
	void removeFromMainList(Object *) override { log.push_back("remove"); }
	void insertInMainList(Object *, int p) override { log.push_back("insert " + std::to_string(p)); }
	bool hasHeldItem() override { return carryingHeld || carryingSelf; }
	bool heldItemReply(uint64, const SpeechTable::Line *&) override { return false; }
	void dropHeldItem() override {}
	void releaseHeldItem() override {}

	int pick(int idx, uint32 lo, uint32 hi) {
		auto it = special.find(std::make_pair(lo, hi));
		if (it != special.end() && it->second == 1) return 8;
		if (it != special.end() && it->second == 2) return 10;
		return (int)((lo * 3 + hi * 5 + (uint32)idx * 7 + (uint32)salt) & 7);
	}
	Object *findObjectByKey(int level, uint64 k) override {
		const uint32 lo = (uint32)k, hi = (uint32)(k >> 32);
		log.push_back("find " + std::to_string(level) + " " + std::to_string(lo) + " " + std::to_string(hi));
		const int v = pick(level, lo, hi);
		return v < 8 ? static_cast<Object *>(&pool[v]) : v == 8 ? static_cast<Object *>(&heldProbe) : self;
	}
	Object *findObject(int level, const char *name) override { return findObjectByKey(level, nameKey64(name)); }
	int startScript(InteractiveObject &who, const char *block, const char *label, byte *vars) override {
		const uint64 b = nameKey64(block), l = nameKey64(label);
		log.push_back("findscript " + std::to_string((uint32)b) + " " + std::to_string((uint32)(b >> 32)));
		std::string v = !vars ? "none" : (!who.stateBlob().empty() && vars == who.stateBlob().data()) ? "own" : "tmp" + std::to_string(*vars);
		for (int q = 0; q < 8; ++q) {
			if (vars && vars == pool[q].stateBlob().data()) v = "o" + std::to_string(q);
			if (vars && vars == pool[q].stateBlob().data() + 1) v = "o" + std::to_string(q) + "+1";
		}
		log.push_back("start " + std::to_string((uint32)l) + " " + std::to_string((uint32)(l >> 32)) + " " + v);
		return 3;
	}
	int startScriptInJimLevel(const char *block, const char *label, byte *vars) override { return startScript(*static_cast<InteractiveObject *>(self), block, label, vars); }
	Level *levelAt(int index) override { return reinterpret_cast<Level *>((uintptr_t)(0x1000 + index)); }
	Level *jimLevel() override { return levelAt(jlevel); }
	byte jimStep() override { return (byte)jstep; }
	void setJimStep(byte step) override { jbyte0 = 0; jstep = step; }
	uint32 randomAlt(uint32, uint32) override { return 0; }
	void jimShow(bool visible) override { jflags = (jflags & ~4) | (visible ? 4 : 0); }
	void jimEnterLevel(Level *, int32, int32) override {}

	InventoryItem *carriedItem() override { return carryingHeld ? static_cast<InventoryItem *>(&heldProbe) : carryingSelf ? static_cast<InventoryItem *>(self) : nullptr; }
	void setCarriedItem(InventoryItem *item) override { carryingHeld = item == &heldProbe; carryingSelf = item && item != &heldProbe; }
	void cursorFrame(int first, int last) override { log.push_back("cursor " + std::to_string(first) + " " + std::to_string(last)); }
	int32 interfaceCameraX() override { return camx; }
	void interfacePageLeft() override { log.push_back("pageleft"); }
	void itemCreated() override { ++counter; }
	void showFrame(Object &object, int first, int last) override {
		std::string n = &object == &heldProbe ? "held" : &object == self ? "obj" : "?";
		for (int q = 0; q < 8; ++q) if (&object == &pool[q]) n = std::to_string(q);
		log.push_back("frame " + n + " " + std::to_string(first) + " " + std::to_string(last));
	}
};

int HeldProbe::useItem(int n) { world->log.push_back("heldUse " + std::to_string(n)); return world->hu; }

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 5000) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

static std::vector<std::string> split(const std::string &s) { std::istringstream in(s); std::vector<std::string> v; std::string t; while (in >> t) v.push_back(t); return v; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	AnimPlayer player; VecStream *stream = nullptr; std::string dat;
	InventoryItem *obj = nullptr; World world; Tally tally;
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
				// Each bank of the level is the same file with its own tag at the start of the header.
				Common::Array<AnimPlayer> table;
				{
					std::vector<std::string> tags; { std::istringstream l(head[12]); std::string t2; while (std::getline(l, t2, ',')) tags.push_back(unhex(t2)); }
					const size_t entry = (size_t)num(head[13]);
					for (size_t q = 0; q < tags.size(); ++q) {
						std::string d = dat; for (size_t z = 0; z < 3; ++z) d[entry + 4 + 8 + z] = tags[q][z]; d[entry + 4 + 8 + 3] = 0;
						VecStream s2(d); AnimPlayer p; p.load(s2, (uint)entry, 0, false); table.push_back(p);
					}
				}
				ObjectRecord r; r.x = (int32)num(head[2]); r.y = (int32)num(head[3]); r.sortOffset = (int16)num(head[4]);
				strncpy(r.name, head[6].c_str(), 12);
				{ std::istringstream l(head[10]); std::string t2; while (std::getline(l, t2, ',')) r.banks.push_back((byte)atoi(t2.c_str())); }
				const std::string data = unhex(head[11]); for (size_t q = 0; q < data.size(); ++q) r.extra.push_back((byte)data[q]);
				world.reset();
				for (int q = 0; q < 8; ++q) {
					world.pool[q] = Probe(); world.pool[q].initPlain(); world.pool[q].stateBlob().resize(16); world.pool[q].setFlags(0);
					world.pool[q].log = &world.log; world.pool[q].index = q;
					Common::Array<AnimPlayer> one; one.push_back(AnimPlayer()); const byte idx4[4] = { 0, 0, 0, 0 }; world.pool[q].animation().init(one, idx4, 4);
				}
				world.heldProbe.world = &world; world.heldProbe._flags = 0; world.heldProbe._items = &world; world.heldProbe._host = &world;
				world.salt = (int)num(head[19]);
				obj = new InventoryItem(); world.self = obj;
				obj->init(r, (int16)num(head[5]), table, (int32)num(head[9]), world);
				if (head[17] != "-") { const std::string own = unhex(head[17]); for (size_t q = 0; q < own.size() && q < obj->stateBlob().size(); ++q) obj->stateBlob()[q] = (byte)own[q]; }
			} else if (!obj) {
				continue;
			} else if (ot[0] == "E") {
				world.time = (int32)num(ot[1]); world.held = num(ot[2]);
				world.carryingHeld = num(ot[3]); world.carryingSelf = false;
				world.heldProbe._itemNumber = (uint16)num(ot[4]); world.hu = (int)num(ot[5]); world.heldProbe._flags = (uint32)num(ot[6]);
				world.heldProbe._comboKeys.clear(); world.heldProbe._comboLines.clear();
				if (ot[7] != "-") { std::istringstream l(ot[7]); std::string t2; while (std::getline(l, t2, ',')) {
					const size_t c1 = t2.find(':'), c2 = t2.rfind(':');
					world.heldProbe._comboKeys.push_back(((uint64)strtoull(t2.substr(c1 + 1, c2 - c1 - 1).c_str(), nullptr, 10) << 32) | strtoull(t2.c_str(), nullptr, 10));
					world.heldProbe._comboLines.push_back(world.line("RP" + t2.substr(c2 + 1)));
				} }
				world.jstep = (int)num(ot[8]); world.jbyte0 = 0x55; world.jlevel = (int)num(ot[9]); world.camx = (int)num(ot[10]);
				world.special.clear();
				if (ot[11] != "-") { std::istringstream l(ot[11]); std::string t2; while (std::getline(l, t2, ',')) {
					const size_t c1 = t2.find(':'), c2 = t2.rfind(':');
					world.special[std::make_pair((uint32)strtoull(t2.c_str(), nullptr, 10), (uint32)strtoull(t2.substr(c1 + 1, c2 - c1 - 1).c_str(), nullptr, 10))] = atoi(t2.c_str() + c2 + 1);
				} }
				{ std::istringstream l(ot[12]); std::string t2; int q = 0; while (q < 8 && std::getline(l, t2, ',')) {
					size_t c = t2.find(':'), c2 = t2.rfind(':'); world.pool[q].setFlags((uint32)strtoul(t2.c_str(), nullptr, 10));
					const std::string bl = unhex(t2.substr(c + 1, c2 - c - 1)); for (size_t z = 0; z < 16; ++z) world.pool[q].stateBlob()[z] = (byte)bl[z];
					world.pool[q].animation().switchTo((uint)atoi(t2.c_str() + c2 + 1)); ++q; } }
			} else if (ot[0] == "V") got = std::to_string(obj->setVerbAndClick((int)num(ot[1])));
			else if (ot[0] == "I") got = std::to_string(obj->useItem((int)num(ot[1])));
			else if (ot[0] == "U") got = std::to_string(obj->update());
			else if (ot[0] == "P") got = std::to_string(obj->verb((int)num(ot[1])));
			else if (ot[0] == "S") obj->setActive(num(ot[1]));
			std::vector<std::string> st = split(blk.state);
			bool ok = st.size() >= 51; std::string why;
			auto name = [&](const SpeechTable::Line *l) { return world.nameOf(l); };
			if (ok) {
				auto bad = [&](bool c, const char *w) { if (!c && why.empty()) why = w; return c; };
				ok = bad((long long)obj->flags() == num(st[0]), "flags") && bad(obj->_pending == num(st[1]), "pending") && bad(obj->_verb == num(st[2]), "verb") &&
					bad(obj->_approachX == num(st[3]), "home x") && bad(obj->_approachY == num(st[4]), "home y") && bad(name(obj->_hoverName) == st[5], "hover");
				for (int v = 0; ok && v < 7; ++v) ok = bad(name(obj->_verbLine[v]) == st[6 + v], "verbline");
				ok = ok && bad((long long)obj->combinationCount() == num(st[13]), "combo count") && bad(obj->itemNumber() == num(st[14]), "item number");
				std::string combos;
				for (uint q = 0; q < obj->combinationCount(); ++q) combos += (q ? "," : "") + std::to_string((uint32)obj->combinationKey(q)) + ":" + std::to_string((uint32)(obj->combinationKey(q) >> 32)) + ":" + name(obj->combinationLine(q));
				ok = ok && bad((combos.empty() ? std::string("-") : combos) == st[15], "combos") && bad(obj->sortValue() == num(st[16]), "sort") && bad(obj->kindId() == num(st[17]), "kind byte");
				AnimPlayer &p = obj->animation().current();
				auto eq = [&](long long g, size_t i, long long wild) { return num(st[18 + i]) == wild || g == num(st[18 + i]); };
				const bool pl = eq(p.flags(), 0, U32) && eq(p.startTime(), 1, -842150451) && eq(p.totalDuration(), 2, 3452816845LL) && eq(p.current(), 3, U16) &&
					eq(p.first(), 4, U16) && eq(p.last(), 5, U16) && eq((uint16)p.loopX(), 6, U16) && eq((uint16)p.loopY(), 7, U16) && eq((uint16)p.movementX(), 8, U16) && eq((uint16)p.movementY(), 9, U16);
				ok = ok && bad(pl, "player");
				ok = ok && bad(obj->animation().stashedBank() == (uint32)num(st[28]), "stashed bank") && bad(obj->animation().stashedFlags() == (uint32)num(st[29]), "stashed flags");
				const std::string carried = world.carryingHeld ? "held" : world.carryingSelf ? "obj" : "none";
				ok = ok && bad(carried == st[30], "carried") && bad(world.heldProbe.flags() == (uint32)num(st[31]), "held flags") && bad(world.counter == num(st[32]), "item counter");
				char jb[8]; snprintf(jb, sizeof jb, "%02x%02x", world.jbyte0, world.jstep & 0xff);
				ok = ok && bad(std::string(jb) == st[33], "jim step");
				auto hexOf = [](const Common::Array<byte> &a) { std::string h; char b2[4]; for (uint z = 0; z < a.size(); ++z) { snprintf(b2, sizeof b2, "%02x", a[z]); h += b2; } return h.empty() ? std::string("-") : h; };
				ok = ok && bad(hexOf(obj->stateBlob()) == st[34], "own state");
				for (int q = 0; ok && q < 8; ++q) ok = bad(world.pool[q].flags() == (uint32)num(st[35 + 2 * q]) && hexOf(world.pool[q].stateBlob()) == st[36 + 2 * q], "other object");
			}
			const char *label = isK ? "constructor" : ot[0] == "E" ? "world" : ot[0] == "V" || ot[0] == "I" || ot[0] == "P" ? "click" : ot[0] == "U" ? "update" : "pick up";
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
