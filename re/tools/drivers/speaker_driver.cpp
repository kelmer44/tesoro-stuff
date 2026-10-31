// Compares the engine's Speaker with the original say / update / clear / talk / draw methods (see re/tools/gen_vectors_speaker.py):
// every session replays the same operations and must make the same calls, in the same order, and leave the same state.
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "tesoro/core/speaker.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }

using namespace Tesoro;

static std::string fmt(const char *f, ...) {
	char buf[256]; va_list ap; va_start(ap, f); vsnprintf(buf, sizeof buf, f, ap); va_end(ap); return buf;
}

struct World : SpeakerHost {
	Speaker *self = nullptr;
	int objectUpdate() override { return self->update(); }
	std::vector<std::string> log;
	SpeechTable::Line lines[5];
	bool held = false, pendingByte = false;
	int32 time = 0; int skip = 0, subs = 0, base = 0, count = 4, ids = 0, rcalls = 0; unsigned long long active = 0;

	int32 now() override { return time; }
	uint32 randomNumber(uint32 lo, uint32 hi) override {
		++rcalls; log.push_back(fmt("random %u %u", lo, hi)); return lo + (rcalls * 7) % (hi - lo + 1);
	}
	uint32 playVoice(uint32 off) override { ++ids; log.push_back(fmt("play %u", off)); return ids; }
	bool voiceActive(uint32 id) override { log.push_back(fmt("find %u", id)); return (active >> id) & 1; }
	void stopVoice(uint32) override { log.push_back("stop"); log.push_back("reap"); }
	bool skipPressed() override { return skip; }
	bool subtitlesOn() override { return subs; }
	void insertInMainList(int p) override { log.push_back(fmt("insert %d", p)); }
	void removeFromMainList() override { log.push_back("remove"); }
	void pauseAnimation() override { log.push_back("pause"); }
	void stashAnimation() override { log.push_back("stash"); }
	void setPose(uint32 p) override { log.push_back(fmt("pose %u", p)); }
	void queueFrame() override { log.push_back("queue"); }
	void setFrameRange(uint16 first, int32 last) override { log.push_back(fmt("range %u %d 0 0", first, last)); }
	uint32 frameCount() override { return count; }
	bool animationHeld() override { return held; }
	void setAnimationHeld(bool h) override { held = h; }
	bool pending() override { return pendingByte; }
	void setPending(bool p) override { pendingByte = p; }
	int interactiveUpdate() override { log.push_back("iupdate"); return base; }
	void interactiveDraw() override { log.push_back("idraw"); }
	void drawSubtitle(const SpeechTable::Line &line) override { log.push_back(fmt("subtitle %d", int(&line - lines))); }
};

static std::string stateOf(const Speaker &s, const World &w) {
	return fmt("%d %d %d %d %u %d %d", s.speaking(), w.held, w.pendingByte, s.line() ? int(s.line() - w.lines) : -1, s.voiceId(), s.colour(), s.talkTime());
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[4096];
	World world; Speaker *speaker = nullptr;
	int sessions = 0, bad = 0, ops = 0, calls = 0, shown = 0;
	struct Op { std::string text, result, state; std::vector<std::string> log; };
	while (fgets(line, sizeof line, f)) {
		if (line[0] != 'S') continue;
		unsigned sid, count, wav[5];
		sscanf(line + 2, "%u %u %u %u %u %u %u", &sid, &count, &wav[0], &wav[1], &wav[2], &wav[3], &wav[4]);
		std::vector<Op> script;
		while (fgets(line, sizeof line, f) && line[0] != '#') {
			line[strcspn(line, "\n")] = 0;
			if (line[0] == 'o') { script.push_back(Op()); script.back().text = line + 2; }
			else if (line[0] == 'r') script.back().result = line + 2;
			else if (line[0] == 's') script.back().state = line + 2;
			else if (line[0] == '.') script.back().log.push_back(line + 2);
		}
		delete speaker; world = World(); world.count = count;
		for (int i = 0; i < 5; ++i) world.lines[i].wavOffset = wav[i];
		speaker = new Speaker(world); world.self = speaker;
		++sessions; bool badSession = false;
		for (const Op &op : script) {
			world.log.clear();
			const char kind = op.text[0]; std::string got = "-";
			if (kind == 'E') {
				unsigned long long active; unsigned nowv; int skip, subs, base;
				sscanf(op.text.c_str() + 2, "%u %d %llu %d %d", &nowv, &skip, &active, &subs, &base);
				world.time = (int32)nowv; world.skip = skip; world.active = active; world.subs = subs; world.base = base;
			} else if (kind == 'Y') {
				int ln, colour; unsigned pose; sscanf(op.text.c_str() + 2, "%d %d %u", &ln, &colour, &pose);
				speaker->say(world.lines[ln], colour, pose);
			} else if (kind == 'U') got = fmt("%d", speaker->update());
			else if (kind == 'D') { if (op.text[2] == '0') speaker->drawCharacter(); else speaker->drawJim(); }
			else if (kind == 'P') world.pendingByte = op.text[2] == '1';
			else if (kind == 'H') world.held = op.text[2] == '1';
			else if (kind == 'C') speaker->clearSpeech();
			++ops; calls += (int)op.log.size();
			const std::string st = kind == 'E' ? "-" : stateOf(*speaker, world);
			const bool resultOk = kind != 'U' || got == op.result;
			if (!resultOk || world.log != op.log || st != op.state) {
				badSession = true;
				if (shown++ < 6) {
					printf("session %u op '%s': result got %s want %s; state got [%s] want [%s]\n", sid, op.text.c_str(), got.c_str(), op.result.c_str(), st.c_str(), op.state.c_str());
					for (size_t i = 0; i < std::max(world.log.size(), op.log.size()); ++i) {
						const std::string g = i < world.log.size() ? world.log[i] : "(none)", w = i < op.log.size() ? op.log[i] : "(none)";
						if (g != w) { printf("   call %zu got '%s' want '%s'\n", i, g.c_str(), w.c_str()); break; }
					}
				}
				break;
			}
		}
		if (badSession) ++bad;
	}
	printf("Speaker          %d/%d sessions match (%d operations, %d calls to the rest of the game)\n", sessions - bad, sessions, ops, calls);
	return bad ? 1 : 0;
}
