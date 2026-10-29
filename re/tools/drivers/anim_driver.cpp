// Compares the engine's animation bank port with vectors recorded from the original code
// (see re/tools/gen_vectors_anim.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/object.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

// Minimal in-memory stream (all inline, so the driver needs no ScummVM library code).
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
static std::string toStr(const byte *p, size_t n) { return std::string((const char *)p, n); }

struct Tally {
	std::map<std::string, std::pair<int, int>> t;	// checks, failures
	int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer player;
	Tesoro::AnimSet set;
	bool setHas = false;
	Tesoro::Object object;
	bool haveObject = false;
	std::string dat; VecStream *stream = nullptr;
	bool is555 = false, loaded = false;
	int caseNo = 0, imgIdx = 0, seqIdx = 0;
	Tally tally;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		Tesoro::AnimBank *bank = loaded ? player.bank() : nullptr;
		if (op == "C") {
			++caseNo; std::string hex; unsigned entry; int mode, f555; in >> hex >> entry >> mode >> f555;
			dat = unhex(hex); is555 = f555 != 0; delete stream;
			stream = new VecStream(dat);
			player = Tesoro::AnimPlayer();
			loaded = player.load(*stream, entry, mode, is555);
			tally.check("load succeeds", loaded, caseNo);
			imgIdx = seqIdx = 0;
		} else if (!loaded) {
			continue;
		} else if (op == "H") {
			uint32 seqs, imgs, h[3], flags; in >> seqs >> imgs >> h[0] >> h[1] >> h[2] >> flags;
			tally.check("header", bank->sequenceCount() == seqs && bank->imageCount() == imgs && bank->headerDword(0) == h[0] &&
				bank->headerDword(1) == h[1] && bank->headerDword(2) == h[2], caseNo);
			tally.check("flags after load", player.flags() == flags, caseNo);
			seqIdx = imgIdx = 0;
		} else if (op == "S") {
			unsigned f0, dur, type, f0c, f10, cnt, fd; int x, y, idx; std::string ent, scl;
			in >> f0 >> x >> y >> dur >> type >> f0c >> f10 >> idx >> cnt >> fd >> ent >> scl;
			const Tesoro::AnimSequence &s = bank->sequence(seqIdx++);
			std::string pe, ps;
			for (uint32 i = 0; i < s.entries.size(); ++i) { byte b[4]; WRITE_LE_UINT32(b, s.entries[i]); pe += toStr(b, 4); }
			for (uint32 i = 0; i < s.scales.size(); ++i) { byte b[2]; WRITE_LE_UINT16(b, s.scales[i]); ps += toStr(b, 2); }
			tally.check("sequence", (uint16)s.imageX == (f0 & 0xffff) && (uint16)s.imageY == (f0 >> 16) && s.x == x && s.y == y && s.duration == dur && s.type == type &&
				(uint16)s.hitX == (f0c & 0xffff) && (uint16)s.hitY == (f0c >> 16) && (uint16)s.hitW == (f10 & 0xffff) && (uint16)s.hitH == (f10 >> 16) && s.imageIndex == idx && s.entries.size() == cnt && s.firstDescent == fd && pe == unhex(ent) && ps == unhex(scl), caseNo);
		} else if (op == "P") {
			std::string list; in >> list; bool ok = true; uint32 k = 0;
			if (list != "-") {
				std::istringstream l(list); std::string tok;
				while (std::getline(l, tok, ',')) ok = ok && k < bank->imageCount() && bank->imagePosition(k++) == (uint32)strtoul(tok.c_str(), nullptr, 10);
			}
			tally.check("image positions", ok && k == bank->imageCount(), caseNo);
		} else if (op == "I" || op == "D" || op == "J") {
			std::string hex; in >> hex; const Tesoro::AnimImage &img = bank->image(imgIdx++);
			std::string want = unhex(hex);
			bool ok = hex == "-" ? !img.isLoaded() : (img.isLoaded() && toStr(img.data(), img.size()) == want);
			tally.check(op == "I" ? "image after load" : op == "D" ? "image after deferred load" : "image after brightness", ok, caseNo);
		} else if (op == "X") {
			tally.check("deferred load succeeds", bank->loadDeferredImages(*stream, is555), caseNo); imgIdx = 0;
		} else if (op == "Y") {
			unsigned factor; in >> factor; bank->scaleBrightness((uint16)factor, is555); imgIdx = 0;
		} else if (op == "R") {
			int first, last, loop, sum, now; std::string bar; int start; unsigned total, flags, fst, lst, c1, c2, rs;
			in >> first >> last >> loop >> sum >> now >> bar >> start >> total >> flags >> fst >> lst >> c1 >> c2 >> rs;
			player.setRange((uint16)first, last, loop != 0, sum != 0, now);
			tally.check("setRange", player.startTime() == start && player.totalDuration() == total && player.flags() == flags &&
				player.first() == fst && player.last() == lst && (uint16)player.loopX() == c1 && (uint16)player.loopY() == c2 && player.current() == rs, caseNo);
		} else if (op.size() > 1 && op[0] == 'o') {
			const std::string name = op.substr(1);
			std::vector<long long> before, after; std::string tok; bool seenBar = false;
			while (in >> tok) { if (tok == "|") seenBar = true; else (seenBar ? after : before).push_back(atoll(tok.c_str())); }
			if (name == "O") {
				const unsigned n = (unsigned)before[0];
				Common::Array<Tesoro::AnimPlayer> table; table.push_back(player);
				std::vector<byte> idx(n, 0); set.init(table, idx.data(), n); setHas = before[1] != 0;
				continue;
			}
			if (name == "Sel") set.selectBank((byte)before[0], (uint16)before[1], (int)before[2], before[3] != 0, before[4] != 0, (int32)before[5]);
			else if (name == "T") { if (setHas) set.tick((int32)before[0]); }
			else if (name == "U") set.tickClearMovement((int32)before[0]);
			else if (name == "St") set.stash();
			else if (name == "Ap") set.restoreStashed();
			else if (name == "F") set.setStashed(true);
			else if (name == "Z") set.current().pause((int32)before[0]);
			else if (name == "W") set.current().resume((int32)before[0]);
			// state: bank stashedBank stashedFlags stashedFlag ok | flags start total current loopX loopY first last paused [movementX movementY]
			// fields the original leaves uninitialised until first written read back as 0xcdcdcdcd
			Tesoro::AnimPlayer &p = set.current();
			const long long U = 0xcdcdcdcdLL;
			bool ok = (long long)set.bankIndex() == after[0] && after[4] == 1 && (after[1] == U || (long long)set.stashedBank() == after[1]) &&
				(after[2] == U || (long long)set.stashedFlags() == after[2]) && set.isStashed() == (after[3] != 0) && (long long)p.flags() == after[5] &&
				p.startTime() == after[6] && (long long)p.totalDuration() == after[7] && p.current() == after[8] && (uint16)p.loopX() == after[9] &&
				(uint16)p.loopY() == after[10] && p.first() == after[11] && p.last() == after[12] && (after[13] == U || (long long)(uint32)p.pausedAt() == after[13]);
			if (ok && (name == "T" || name == "U")) ok = (after[14] == -12851 || p.movementX() == after[14]) && (after[15] == -12851 || p.movementY() == after[15]);
			tally.check(("object " + name).c_str(), ok, caseNo);
		} else if (op.size() > 1 && op[0] == 'e') {
			const std::string name = op.substr(1);
			std::vector<std::string> before, after; std::string tok; bool seenBar = false;
			while (in >> tok) { if (tok == "|") seenBar = true; else (seenBar ? after : before).push_back(tok); }
			auto num = [](const std::string &s) { return atoll(s.c_str()); };
			const long long U = 0xcdcdcdcdLL;
			// object state: vtable flags sort nameLo nameHi banks index curOk x y px py stateSize blob, then 8 fields per player
			auto checkObject = [&](const std::vector<std::string> &a, size_t at, const char *what) {
				bool ok = (long long)object.flags() == num(a[at + 1]) && object.sortValue() == num(a[at + 2]) &&
					(object.nameKey() & 0xffffffffu) == (uint64)num(a[at + 3]) && (object.nameKey() >> 32) == (uint64)num(a[at + 4]) &&
					object.animation().bankCount() == num(a[at + 5]) && object.animation().bankIndex() == num(a[at + 6]) && (object.animation().bankCount() == 0 || num(a[at + 7]) == 1) &&
					object.x() == num(a[at + 8]) && object.y() == num(a[at + 9]) && object.previousX() == num(a[at + 10]) && object.previousY() == num(a[at + 11]) &&
					object.stateSize() == num(a[at + 12]);
				for (uint k = 0; ok && k < object.animation().bankCount(); ++k) {
					const size_t b = at + 14 + 8 * k; Tesoro::AnimPlayer &p = object.animation().player(k);
					ok = (long long)p.flags() == num(a[b]) && p.startTime() == num(a[b + 1]) && (long long)p.totalDuration() == num(a[b + 2]) && p.current() == num(a[b + 3]) &&
						p.first() == num(a[b + 4]) && p.last() == num(a[b + 5]) && (uint16)p.loopX() == num(a[b + 6]) && (uint16)p.loopY() == num(a[b + 7]);
				}
				{ std::string det; if (!ok) { for (size_t i = at; i < a.size() && i < at + 24; ++i) det += a[i] + " "; det += " | got fl " + std::to_string(object.flags()) + " nb " + std::to_string(object.animation().bankCount()) + " key " + std::to_string(object.nameKey()) + " sort " + std::to_string(object.sortValue()); } tally.check(what, ok, caseNo, det); }
			};
			if (name == "K") {
				// K x y sort roomSort name banks size indices | state
				Common::Array<Tesoro::AnimPlayer> table; for (long long k = 0; k < num(before[0]); ++k) table.push_back(player);
				Tesoro::ObjectRecord rec; rec.x = (int32)num(before[1]); rec.y = (int32)num(before[2]); rec.sortOffset = (int16)num(before[3]);
				strncpy(rec.name, before[5].c_str(), 12);
				if (before[8] != "-") { std::istringstream l(before[8]); std::string t2; while (std::getline(l, t2, ',')) rec.banks.push_back((byte)atoi(t2.c_str())); }
				object = Tesoro::Object(); object.initAnimated(rec, (int16)num(before[4]), table, (uint16)num(before[7])); haveObject = true;
				checkObject(after, 0, "object constructor");
			} else if (name == "B") {
				Tesoro::Object plain; plain.initPlain();
				tally.check("plain object", (long long)plain.flags() == num(after[0]) && plain.sortValue() == num(after[1]), caseNo);
			} else if (haveObject) {
				if (name == "P") { object.setPosition((int32)num(before[0]), (int32)num(before[1])); object.setPreviousPosition((int32)num(before[2]), (int32)num(before[3])); }
				else if (name == "F") object.setFlags((uint32)num(before[0]));
				else if (name == "Sa" || name == "Sv") {
					const int b = (int)num(before[0]);
					if (name == "Sa") object.setActive((b & 1) != 0); else object.setVisible((b & 1) != 0);
					tally.check(name == "Sa" ? "object setActive" : "object setVisible", (long long)object.flags() == num(after[0]), caseNo);
				} else if (name == "U") {
					object.updateBounds((int32)num(before[0]));
					const Tesoro::ObjectRect &bd = object.hitRect(), &mv = object.movedArea();
					bool ok = bd.x1 == num(after[0]) && bd.y1 == num(after[1]) && bd.x2 == num(after[2]) && bd.y2 == num(after[3]) &&
						(num(after[4]) - object.x() == -12851 || mv.x1 == num(after[4])) && (num(after[5]) - object.y() == -12851 || mv.y1 == num(after[5])) && mv.x2 == num(after[6]) && mv.y2 == num(after[7]) && object.shownSequence() == num(after[8]);
					tally.check("object updateBounds", ok, caseNo, ok ? "" : "want " + after[0] + " " + after[1] + " " + after[2] + " " + after[3] + " | " + after[4] + " " + after[5] + " " + after[6] + " " + after[7] + " seq " + after[8] + " got " + std::to_string(bd.x1) + " " + std::to_string(bd.y1) + " " + std::to_string(bd.x2) + " " + std::to_string(bd.y2) + " | " + std::to_string(mv.x1) + " " + std::to_string(mv.y1) + " " + std::to_string(mv.x2) + " " + std::to_string(mv.y2));
					checkObject(after, 9, "object state after update");
				}
			}
		} else if (op == "A") {
			int now, mx, my, start; unsigned cur, flags, total, lx, ly; std::string bar;
			in >> now >> bar >> mx >> my >> cur >> flags >> start >> total >> lx >> ly;
			player.advance(now);
			tally.check("advance", player.movementX() == mx && player.movementY() == my && player.current() == cur && player.flags() == flags &&
				player.startTime() == start && player.totalDuration() == total && (uint16)player.loopX() == lx && (uint16)player.loopY() == ly, caseNo,
				"want " + std::to_string(mx) + "," + std::to_string(my) + " cur " + std::to_string(cur) + " fl " + std::to_string(flags) + " st " + std::to_string(start) + " got " + std::to_string(player.movementX()) + "," + std::to_string(player.movementY()) + " " + std::to_string(player.current()) + " " + std::to_string(player.flags()) + " " + std::to_string(player.startTime()));
		} else if (op == "Z" || op == "W") {
			int now, start; unsigned flags, paused; std::string bar;
			in >> now >> bar >> flags >> start >> paused;
			if (op == "Z") player.pause(now); else player.resume(now);
			tally.check(op == "Z" ? "pause" : "resume", player.flags() == flags && player.startTime() == start && (uint32)player.pausedAt() == paused, caseNo,
				op + " want flags " + std::to_string(flags) + " start " + std::to_string(start) + " paused " + std::to_string(paused) + " got " + std::to_string(player.flags()) + " " + std::to_string(player.startTime()) + " " + std::to_string((uint32)player.pausedAt()));
		}
	}
	int bad = 0;
	for (auto &kv : tally.t) { printf("%-28s %d/%d match\n", kv.first.c_str(), kv.second.first - kv.second.second, kv.second.first); bad += kv.second.second; }
	delete stream;
	return bad ? 1 : 0;
}
