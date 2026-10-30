// Compares the engine's SpriteObject (object kind 3) with vectors recorded from the original code
// (see re/tools/gen_vectors_sprite.py). Build/run: re/tools/run_startup_check.sh
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
#include "common/array.h"
#include "common/stream.h"
#define private public
#define protected public
#include "tesoro/core/camera.h"
#include "tesoro/core/display.h"
#include "tesoro/core/spriteobject.h"
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

struct Recorder : Tesoro::Display {
	int calls = 0; int32 x = 0, y = 0; uint16 depth = 0; const Tesoro::AnimImage *image = nullptr; uint32 extra = 0;
	void drawImage(int32 px, int32 py, uint16 d, const Tesoro::AnimImage &img, uint32 e) override { ++calls; x = px; y = py; depth = d; image = &img; extra = e; }
};

static std::string unhex(const std::string &s) {
	std::string r;
	if (s == "-") return r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer player; VecStream *stream = nullptr; std::string dat;
	Tesoro::SpriteObject *obj = nullptr; Tesoro::Camera cam; Recorder rec;
	uint nseq = 0; int caseNo = 0; bool loaded = false;
	Tally tally;
	const long long U32 = 0xcdcdcdcdLL, U16 = 0xcdcd;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") {
			++caseNo; std::string hex; unsigned entry; int mode, f555; in >> hex >> entry >> mode >> f555;
			dat = unhex(hex); delete stream; stream = new VecStream(dat); player = Tesoro::AnimPlayer();
			loaded = player.load(*stream, entry, mode, f555 != 0); nseq = loaded ? player.bank()->sequenceCount() : 0;
			tally.check("load succeeds", loaded, caseNo);
			delete obj; obj = nullptr;
			continue;
		}
		if (!loaded || op == "#") continue;
		std::vector<std::string> before, after; std::string tok; bool seenBar = false;
		while (in >> tok) { if (tok == "|") seenBar = true; else (seenBar ? after : before).push_back(tok); }
		auto num = [](const std::string &s) { return atoll(s.c_str()); };
		// state: flags vtableOk, per player 8 fields, then x y of each sequence
		auto checkState = [&](size_t at, const char *what) {
			bool ok = (long long)obj->flags() == num(after[at]) && num(after[at + 1]) == 1;
			Tesoro::AnimSet &set = obj->animation();
			std::string det;
			for (uint k = 0; ok && k < set.bankCount(); ++k) {
				const size_t b = at + 2 + 8 * k; Tesoro::AnimPlayer &p = set.player(k);
				auto eq = [&](long long got, size_t i, long long wild) { return num(after[b + i]) == wild || got == num(after[b + i]); };
				ok = eq(p.flags(), 0, U32) && eq(p.startTime(), 1, -842150451) && eq(p.totalDuration(), 2, 3452816845LL) && eq(p.current(), 3, U16) &&
					eq(p.first(), 4, U16) && eq(p.last(), 5, U16) && eq((uint16)p.loopX(), 6, U16) && eq((uint16)p.loopY(), 7, U16);
			}
			const size_t s = at + 2 + 8 * set.bankCount();
			for (uint i = 0; ok && i < nseq; ++i)
				ok = player.bank()->sequence(i).x == num(after[s + 2 * i]) && player.bank()->sequence(i).y == num(after[s + 2 * i + 1]);
			tally.check(what, ok, caseNo, ok ? "" : "flags " + std::to_string(obj->flags()) + " want " + after[at]);
		};
		if (op == "eK") {
			// K x y sort roomSort name nb N t0 idxs data
			Common::Array<Tesoro::AnimPlayer> table; for (long long k = 0; k < num(before[0]); ++k) table.push_back(player);
			Tesoro::ObjectRecord r; r.x = (int32)num(before[1]); r.y = (int32)num(before[2]); r.sortOffset = (int16)num(before[3]);
			strncpy(r.name, before[5].c_str(), 12);
			{ std::istringstream l(before[9]); std::string t2; while (std::getline(l, t2, ',')) r.banks.push_back((byte)atoi(t2.c_str())); }
			const std::string data = unhex(before[10]); for (size_t i = 0; i < data.size(); ++i) r.extra.push_back((byte)data[i]);
			obj = new Tesoro::SpriteObject(); obj->init(r, (int16)num(before[4]), table, (int32)num(before[8]));
			checkState(0, "constructor");
		} else if (!obj) {
			continue;
		} else if (op == "eT") {
			obj->updateAnimation((int32)num(before[0])); checkState(0, "tick");
		} else if (op == "eM") {
			Tesoro::AnimPlayer &p = obj->animation().current(); p._movementX = (int16)num(before[0]); p._movementY = (int16)num(before[1]);
		} else if (op == "eX") {
			cam._scaledX = (int32)num(before[0]); cam._scaledY = (int32)num(before[1]);
			obj->setPosition((int32)num(before[2]), (int32)num(before[3])); obj->_sortValue = (int16)num(before[4]);
			rec = Recorder(); obj->draw(cam, rec);
			bool ok = rec.calls == num(after[0]);
			if (ok && rec.calls) {
				const Tesoro::AnimBank *b = obj->animation().current().bank();
				ok = rec.x == num(after[1]) && rec.y == num(after[2]) && rec.depth == (uint16)num(after[3]) && rec.extra == (uint32)num(after[5]) &&
					rec.image == &b->image((uint)num(after[4]));
			}
			tally.check("draw", ok, caseNo, ok ? "" : "want " + after[0] + (after.size() > 5 ? " " + after[1] + "," + after[2] + " d " + after[3] + " img " + after[4] : "") + " got " + std::to_string(rec.calls) + " " + std::to_string(rec.x) + "," + std::to_string(rec.y) + " d " + std::to_string(rec.depth));
		}
	}
	int bad = 0;
	for (auto &kv : tally.t) { printf("%-28s %d/%d match\n", kv.first.c_str(), kv.second.first - kv.second.second, kv.second.first); bad += kv.second.second; }
	return bad ? 1 : 0;
}
