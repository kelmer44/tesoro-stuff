// Compares the engine's Jim with the original constructor and methods on real animation players (see re/tools/gen_vectors_jim.py): every session builds
// Jim from the same nine animation files and replays the same operations; each must make the same calls, return the same value and leave Jim and
// all nine players in the same state.
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#include "tesoro/core/animbank.h"
#define private public
#include "tesoro/core/camera.h"
#undef private
#include "tesoro/core/display.h"
#include "tesoro/core/jimwalker.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

using namespace Tesoro;

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

static std::string fmt(const char *f, ...) {
	char buf[512]; va_list ap; va_start(ap, f); vsnprintf(buf, sizeof buf, f, ap); va_end(ap); return buf;
}
static std::string unhex(const char *s) {
	std::string r;
	for (size_t i = 0; s[i] && s[i + 1] && s[i] != '\n'; i += 2) { char b[3] = {s[i], s[i + 1], 0}; r += (char)strtol(b, nullptr, 16); }
	return r;
}

struct Target : WalkTarget {
	std::vector<std::string> *log = nullptr; int ret = 1;
	int arrived() override { log->push_back("arrived"); return ret; }
};

struct World : JimWorld {
	std::vector<std::string> log;
	std::map<const AnimImage *, std::string> imageName;
	SpeechTable::Line lines[6];
	int32 time = 0; int skip = 0, subs = 0, base = 0, scale = 0x100, fp = 0, ret = 1, has = 1, ohas = 1, ids = 0, rcalls = 0; unsigned voices = 0;

	int32 now() override { return time; }
	uint32 randomNumber(uint32 lo, uint32 hi) override { ++rcalls; log.push_back(fmt("random %u %u", lo, hi)); return lo + (rcalls * 7) % (hi - lo + 1); }
	uint32 playVoice(uint32 off) override { ++ids; log.push_back(fmt("play %u", off)); return ids; }
	bool voiceActive(uint32 id) override { log.push_back(fmt("find %u", id)); return (voices >> id) & 1; }
	void stopVoice(uint32) override { log.push_back("stop"); log.push_back("reap"); }
	bool skipPressed() override { return skip; }
	bool subtitlesOn() override { return subs; }
	void insertInMainList(Object *, int p) override { log.push_back(fmt("insert %d", p)); }
	void removeFromMainList(Object *) override { log.push_back("remove"); }
	bool hasWalkArea() override { return has; }
	int32 scaleAt(int32 x, int32 y) override { log.push_back(fmt("scale %d %d", x, y)); return scale; }
	bool findPath(int32 dx, int32 dy, WalkTarget *t) override { log.push_back(fmt("findpath %d %d %s", dx, dy, t ? "tgt" : "none")); return fp; }
	bool hasWalkAreaOf(Object *) override { return ohas; }
	int interactiveUpdate(Jim &) override { log.push_back("iupdate"); return base; }
	void drawSubtitle(Jim &, const SpeechTable::Line &line) override { log.push_back(fmt("subtitle %d", int(&line - lines))); }
};

struct Screen : Display {
	World *world = nullptr;
	void drawImage(int32, int32, uint16, const AnimImage &, uint32) override {}
	void drawImageScaled(int32 x, int32 y, uint16 depth, const AnimImage &image, int32 sx, int32 sy, int32 shear, uint32 extra) override {
		auto it = world->imageName.find(&image);
		world->log.push_back(fmt("scaled %d %d %u %s %d %d %d %u", x, y, depth, it == world->imageName.end() ? "?" : it->second.c_str(), sx, sy, shear, extra));
	}
};

static std::string stateOf(Jim &j, World &w) {
	std::string s = fmt("%d %d %d %d %d %d %d", j.x(), j.y(), j.previousX(), j.previousY(), j.walker().facing(), j.walker().walking(), j.walker().scale());
	s += fmt(" %d %d %d %d %d %s %d %d", j.walker().fixedX(), j.walker().fixedY(), j.walker().usedMoveX(), j.walker().usedMoveY(), j.walker().direct(),
	         j.walker().target() ? "tgt" : "none", j.walker().destX(), j.walker().destY());
	const Speaker &sp = j.speaker();
	s += fmt(" %d %d %d %u %d %d %d %d %d %u", sp.speaking(), j.animation().isStashed(), sp.line() ? int(sp.line() - w.lines) : -1, sp.voiceId(), sp.colour(), sp.talkTime(),
	         j.anchorX(), j.anchorY(), (int)j.animation().bankIndex(), 0);
	s = s.substr(0, s.rfind(' ')) ;
	s += fmt(" %d %d %d %d", j.hitRect().x1, j.hitRect().y1, j.hitRect().x2, j.hitRect().y2);
	for (uint i = 0; i < 9; ++i) {
		AnimPlayer &p = j.animation().player(i);
		s += fmt(" %u %u %u %u %d %d %d %d %d %d %u", p.flags(), p.current(), p.first(), p.last(), p.startTime(), p.pausedAt(), p.movementX(), p.movementY(), p.loopX(), p.loopY(), p.totalDuration());
	}
	return s;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1 << 20];
	int sessions = 0, bad = 0, ops = 0, calls = 0, shown = 0;
	struct Op { std::string text, result, state; std::vector<std::string> log; };
	while (fgets(line, sizeof line, f)) {
		if (line[0] != 'S') continue;
		unsigned sid; int used = 0; sscanf(line + 2, "%u %n", &sid, &used);
		const std::string data = unhex(line + 2 + used);
		std::vector<Op> script;
		while (fgets(line, sizeof line, f) && line[0] != '#') {
			line[strcspn(line, "\n")] = 0;
			if (line[0] == 'o') { script.push_back(Op()); script.back().text = line + 2; }
			else if (line[0] == 'r') script.back().result = line + 2;
			else if (line[0] == 's') script.back().state = line + 2;
			else if (line[0] == '.') script.back().log.push_back(line + 2);
		}
		World world; Screen screen; screen.world = &world; Target target; target.log = &world.log;
		for (int i = 0; i < 6; ++i) world.lines[i].wavOffset = (unsigned[]){0, 1000, 2000, 0, 3000, 4000}[i];
		VecStream stream(data);
		Common::Array<AnimPlayer> banks;
		for (int k = 0; k < 9; ++k) {
			AnimPlayer p;
			if (!p.load(stream, 0x4000 * (k + 1), AnimBank::kLoadDeferred, false)) { fprintf(stderr, "bank %d does not load\n", k); return 3; }
			banks.push_back(p);
		}
		for (int k = 0; k < 9; ++k) {
			banks[k].bank()->loadDeferredImages(stream, false);
			for (uint i = 0; i < banks[k].bank()->imageCount(); ++i) world.imageName[&banks[k].bank()->image(i)] = fmt("%d.%u", k, i);
		}
		Jim jim(world); jim.init(banks, &world.lines[0]);
		Camera cam; cam._scaledX = cam._scaledY = 0; Object other;
		++sessions; bool badSession = false;
		for (const Op &op : script) {
			world.log.clear();
			const char kind = op.text[0]; std::string got = "-";
			const char *a = op.text.c_str() + 2;
			std::string want = op.text;
			if (kind == 'C') {
				// the constructor's result: fields as the original leaves them (checked as text)
				std::string s = fmt("%u %d %u %u %d %u %u %u %d %d %d %d", jim.flags(), jim.sortValue(), (uint32)jim.nameKey(), (uint32)(jim.nameKey() >> 32), jim.state(), jim.answerColour(), jim.textWidth(),
				                    jim.scriptColour(), jim.walker().scale(), jim.walker().facing(), jim.x(), jim.y());
				s += fmt(" 2 2 %d %d %d %d", 0, 1, 7, 8);
				std::string pts;
				for (int k = 0; k < 9; ++k) {
					pts += fmt(" %d:", k);
					const AnimBank &b = *jim.animation().player(k).bank();
					for (uint i = 0; i < b.sequenceCount(); ++i) pts += fmt("%s%u", i ? "," : "", (unsigned)b.sequence(i).entries.size());
				}
				if (op.text.substr(2) != s + pts) { badSession = true; if (shown++ < 4) printf("session %u constructor differs:\n got  %s\n want %s\n", sid, (s + pts).c_str(), op.text.c_str() + 2); }
			}
			else if (kind == 'I') { int x, y; sscanf(a, "%d %d", &x, &y); jim.setPosition(x, y); jim.setPreviousPosition(x, y); }
			else if (kind == 'E') {
				unsigned voices; int nowv;
				sscanf(a, "%d %d %u %d %d %d %d %d %d %d %d %d", &nowv, &world.skip, &voices, &world.subs, &world.base, &world.scale, &world.fp, &world.ret, &world.has, &world.ohas, &cam._scaledX, &cam._scaledY);
				world.time = nowv; world.voices = voices; target.ret = world.ret;
			}
			else if (kind == 'Y') { int k; sscanf(a, "%d", &k); jim.sayScriptLine(world.lines[k]); }
			else if (kind == 'A') { int k; sscanf(a, "%d", &k); jim.sayAnswer(world.lines[k]); }
			else if (kind == 'U') got = fmt("%u", (uint32)jim.update());
			else if (kind == 'W') { int x, y, t, d; sscanf(a, "%d %d %d %d", &x, &y, &t, &d); got = fmt("%u", (uint32)jim.walkTo(x, y, t ? &target : nullptr, d != 0)); }
			else if (kind == 'X') got = fmt("%u", (uint32)jim.onTouched(&other, 0));
			else if (kind == 'F') { int fc; sscanf(a, "%d", &fc); jim.setFacing(fc); }
			else if (kind == 'D') jim.draw(cam, screen);
			else if (kind == 'B') jim.updateBounds(world.time);
			++ops; calls += (int)world.log.size();
			const std::string st = stateOf(jim, world);
			const bool resultCompared = kind == 'U' || kind == 'W' || kind == 'X';
			if ((resultCompared && got != op.result) || world.log != op.log || st != op.state) {
				badSession = true;
				if (shown++ < 4) {
					printf("session %u op '%s': result got %s want %s\n", sid, op.text.c_str(), got.c_str(), op.result.c_str());
					for (size_t i = 0; i < std::max(world.log.size(), op.log.size()); ++i) {
						const std::string g = i < world.log.size() ? world.log[i] : "(none)", w = i < op.log.size() ? op.log[i] : "(none)";
						if (g != w) { printf("   call %zu got '%s' want '%s'\n", i, g.c_str(), w.c_str()); break; }
					}
					size_t k = 0; while (k < st.size() && k < op.state.size() && st[k] == op.state[k]) ++k;
					if (st != op.state) printf("   state differs at %zu: got '...%s' want '...%s'\n", k, st.substr(k > 20 ? k - 20 : 0, 60).c_str(), op.state.substr(k > 20 ? k - 20 : 0, 60).c_str());
				}
				break;
			}
		}
		if (badSession) ++bad;
	}
	printf("Jim object       %d/%d sessions match (%d operations, %d calls to the rest of the game)\n", sessions - bad, sessions, ops, calls);
	return bad ? 1 : 0;
}
