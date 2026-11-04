// Compares the engine's board game piece display with vectors recorded from the original code
// (see re/tools/gen_vectors_pieces.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/boardpieces.h"
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

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer bank; VecStream *bs = nullptr; std::string bdat;
	Tally tally; int caseNo = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "B") {
			std::string hex; in >> hex; bdat = unhex(hex); delete bs; bs = new VecStream(bdat); bank = Tesoro::AnimPlayer();
			tally.check("bank loads", bank.load(*bs, 0, 0, false), caseNo + 1);
		} else if (op == "K") {
			++caseNo;
			int now, state, blink, stamp, stale; std::string boardHex, namesCsv;
			int win[8];
			in >> now >> state >> blink >> stamp >> boardHex;
			for (int i = 0; i < 8; ++i) in >> win[i];
			in >> stale >> namesCsv;
			std::string bar, results; std::getline(in, bar, '|'); std::getline(in, results);
			std::vector<std::string> names; { std::istringstream n(namesCsv); std::string t; while (namesCsv != "-" && std::getline(n, t, ',')) names.push_back(t); }
			std::vector<Tesoro::Object *> objects; Tesoro::ObjectList list;
			for (size_t i = 0; i < names.size(); ++i) {
				Tesoro::ObjectRecord rec; strcpy(rec.name, names[i].c_str()); rec.banks.push_back(0); rec.banks.push_back(1);
				Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(bank); banks.push_back(bank);
				Tesoro::Object *o = new Tesoro::Object(); o->initAnimated(rec, 0, banks, 0);
				objects.push_back(o); list.pushBack(o);
			}
			if (stale >= 0) list._value = objects[stale];
			Tesoro::ConnectFour game; std::string b = unhex(boardHex); memcpy(game.raw(), b.data(), 57);
			for (int i = 0; i < 8; ++i) game._line[i] = win[i];
			Tesoro::BoardPieces pieces; pieces.state = (byte)state; pieces.blink = (byte)blink; pieces.blinkStamp = stamp;
			pieces.update(game, list, now);
			std::string got = std::to_string((int)pieces.blink) + " " + std::to_string(pieces.blinkStamp);
			for (size_t i = 0; i < objects.size(); ++i) {
				Tesoro::Object *o = objects[i];
				const bool chosen = o->animation().current().startTime() == now;
				got += " " + std::to_string((o->flags() & Tesoro::Object::kVisible) ? 1 : 0) + ":" + (chosen ? std::to_string(o->animation().bankIndex()) : std::string("-"));
			}
			if (objects.empty()) got += " -";
			tally.check("blink, visibility and picture of each piece", norm(got) == norm(results), caseNo, std::string("\n port     ") + got + "\n original " + norm(results));
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-40s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
