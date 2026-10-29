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
#include "tesoro/core/animbank.h"

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
			tally.check("sequence", s.field0 == f0 && s.x == x && s.y == y && s.duration == dur && s.type == type && s.field0c == f0c &&
				s.field10 == f10 && s.imageIndex == idx && s.entries.size() == cnt && s.firstDescent == fd && pe == unhex(ent) && ps == unhex(scl), caseNo);
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
