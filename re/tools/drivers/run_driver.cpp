// Compares the engine's script runner with the original executor (see re/tools/gen_vectors_run.py): every session replays the same
// operations (world settings, start, tick, resume) and must make the same calls to the rest of the game, in the same order, return the same
// values and leave the same variables behind.
// Usage: run_driver <vectors> <dir with ANIMS/CONVERS/ICONS/OBJETOS.DLG>. Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "common/scummsys.h"
#include "common/endian.h"
#include "tesoro/core/namekey.h"
#include "tesoro/core/scriptblock.h"
#include "tesoro/core/scriptrunner.h"

void NORETURN_PRE error(const char *s, ...) { va_list ap; va_start(ap, s); fprintf(stderr, "engine error: "); vfprintf(stderr, s, ap); fprintf(stderr, "\n"); va_end(ap); exit(3); }

using namespace Tesoro;

struct Lookup : ScriptLookup {
	uint32 phraseId(const char *name) override { const uint64 k = nameKey64(name); return (uint32(k) ^ (uint32(k >> 32) * 3) ^ 0x1234567) | 1; }
	uint32 levelRef(int i) override { return 0x71000000 + 0x100 * i; }
	uint32 inventoryRef() override { return 0x72000000; }
};

static std::string fmt(const char *f, ...) {
	char buf[512]; va_list ap; va_start(ap, f); vsnprintf(buf, sizeof buf, f, ap); va_end(ap); return buf;
}

// Objects are numbered by first use: Jim is 1, object n is 100 + n.
static Object *handle(int n) { return reinterpret_cast<Object *>(uintptr_t(100 + n)); }
static Object *const kJim = reinterpret_cast<Object *>(uintptr_t(1));
static int indexOf(Object *o) { return o == kJim ? 0 : int(uintptr_t(o)) - 100; }
static std::string nameOf(Object *o) { return !o ? "null" : o == kJim ? "jim" : fmt("o%d", indexOf(o)); }

struct World : ScriptHost {
	std::vector<std::string> log;
	std::map<std::tuple<uint32, uint32, uint32>, int> objects;
	std::vector<int> busy;
	uint32 time = 0;
	uint64 speak = 0, anim = 0;
	int menuChoice = -1;

	void reset() { log.clear(); objects.clear(); busy.clear(); time = 0; speak = anim = 0; menuChoice = -1; }
	bool bit(uint64 mask, Object *o) { return (mask >> indexOf(o)) & 1; }

	uint32 now() override { return time; }
	Object *jim() override { return kJim; }
	Level *levelFromRef(uint32 ref) override { return reinterpret_cast<Level *>(uintptr_t(ref)); }
	Object *findObject(Level *level, uint64 key) override {
		const uint32 lv = uint32(uintptr_t(level));
		auto k = std::make_tuple(lv, uint32(key), uint32(key >> 32));
		if (!objects.count(k)) {
			const int i = objects.size() + 1;
			objects[k] = i;
			busy.resize(i + 1);
			busy[i] = (i * 7 + 3) % 5 == 0;
		}
		const int i = objects[k];
		log.push_back(fmt("find %x %u %u o%d", lv, std::get<1>(k), std::get<2>(k), i));
		return handle(i);
	}
	void insertScript(uint32 b, int priority) override { log.push_back(fmt("insert script%u %d", b, priority)); }
	void removeScript(uint32 b) override { log.push_back(fmt("remove script%u", b)); }
	void removeObject(Object *o) override { log.push_back("remove " + nameOf(o)); }
	void setCursorActive(bool on) override { log.push_back(fmt("cursor %d", on)); }
	void say(Object *actor, uint32 phrase, uint32 extra) override {
		const uint32 voice = actor == kJim ? 0x77 : 0x100 + indexOf(actor);
		log.push_back(fmt("say %s %u %u %u", nameOf(actor).c_str(), phrase, voice, extra));
	}
	void sayJim(uint32 phrase) override { log.push_back(fmt("sayjim jim %u", phrase)); }
	bool isSpeaking(Object *actor) override { return bit(speak, actor); }
	void releaseIfBusy(Object *c) override {
		const int i = indexOf(c);
		if (busy[i]) {
			busy[i] = 0;
			log.push_back("release " + nameOf(c));
			log.push_back("release2 " + nameOf(c));
		}
	}
	void animate(Object *o, byte first, int16 last, int32 third, bool loop) override {
		log.push_back(fmt("animate %s %u %u %u %d 0", nameOf(o).c_str(), first, uint16(last), uint32(third), loop));
	}
	void walk(Object *o, int16 x, int16 y, uint32 notify) override { log.push_back(fmt("walk %s %d %d script%u 0", nameOf(o).c_str(), x, y, notify)); }
	void look(Object *o, int8 d) override { log.push_back(fmt("look %s %d", nameOf(o).c_str(), d)); }
	void activate(Object *o, bool on) override { log.push_back(fmt("activate %s %d", nameOf(o).c_str(), on)); }
	void show(Object *o, bool on) override { log.push_back(fmt("show %s %d", nameOf(o).c_str(), on)); }
	bool animationBusy(Object *o) override { log.push_back("update " + nameOf(o)); return bit(anim, o); }
	void resumeJim() override { log.push_back("resumejim 4660"); }
	void beginMenu() override { log.push_back("beginmenu"); }
	int showMenu(const uint32 *phrases, uint32 count) override {
		std::string l = fmt("menu jim %u", count);
		for (uint32 i = 0; i < count; ++i) l += fmt(" %u", phrases[i]);
		log.push_back(l);
		return menuChoice >= 0 && uint32(menuChoice) < count ? menuChoice : -1;
	}
	void endMenu() override { log.push_back("endmenu"); }
	void runDo(ScriptRun &run, uint32 ordinal) override { log.push_back(fmt("do script%u %u", run.blockIndex, ordinal)); }
	void playVideo(uint32 n) override { log.push_back(fmt("video %u", n)); }
	void playSound(const char *name) override { log.push_back(fmt("sound %s.WAV", name)); }
	void playTrack(byte t) override { log.push_back(fmt("track %u 1", t)); }
	void throneScene(ScriptRun &run, int variant) override { log.push_back(fmt("throne script%u %d", run.blockIndex, variant)); }
};

static std::string unhex(const char *s) {
	std::string r;
	for (size_t i = 0; s[i] && s[i + 1] && s[i] != '\n'; i += 2) { char b[3] = {s[i], s[i + 1], 0}; r += (char)strtol(b, nullptr, 16); }
	return r;
}

static std::string slurp(const std::string &path) {
	FILE *f = fopen(path.c_str(), "rb");
	if (!f) { fprintf(stderr, "cannot read %s\n", path.c_str()); exit(2); }
	std::string s; char buf[65536]; size_t n;
	while ((n = fread(buf, 1, sizeof buf, f)) > 0) s.append(buf, n);
	fclose(f);
	return s;
}

struct Op {
	std::string text, result;
	std::vector<std::string> log;
};

int main(int argc, char **argv) {
	if (argc < 3) { fprintf(stderr, "usage: run_driver vectors dlgdir\n"); return 2; }
	Lookup lookup;
	ScriptLibrary lib;
	std::vector<std::string> texts;
	for (const char *nm : {"ANIMS.DLG", "CONVERS.DLG", "ICONS.DLG", "OBJETOS.DLG"}) {
		texts.push_back(slurp(std::string(argv[2]) + "/" + nm));
		lib.load((const byte *)texts.back().data(), (uint32)texts.back().size(), lookup);
	}
	lib.resolve();

	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1 << 16];
	World world;
	int sessions = 0, sessionBad = 0, ops = 0, opBad = 0, calls = 0, blocks = 0, blockBad = 0;
	while (fgets(line, sizeof line, f)) {
		if (line[0] == 'B') {
			// Targets of jumps, ifs and runs and the commands' kinds after resolving (other bytes are left over from the compiler).
			unsigned i, n; int used = 0;
			sscanf(line + 2, "%u %u %n", &i, &n, &used);
			const std::string want = unhex(line + 2 + used);
			++blocks;
			bool same = i < lib.blockCount() && lib.block(i).entryCount() == n;
			for (unsigned k = 0; same && k < n; ++k) {
				const ScriptEntry &e = lib.block(i).entry(k);
				const byte *w = (const byte *)want.data() + 24 * k;
				same = e.op() == READ_LE_UINT32(w) && ((e.op() != kScriptJump && e.op() != kScriptIfEquals && e.op() != kScriptRun) || e.word(4) == READ_LE_UINT32(w + 4));
			}
			if (!same && blockBad++ < 5) printf("block %u differs after resolving all four files\n", i);
			continue;
		}
		if (line[0] != 'S') continue;
		unsigned sid, block, lo, hi; char hex[512] = "";
		sscanf(line + 2, "%u %u %u %u %511s", &sid, &block, &lo, &hi, hex);
		std::vector<Op> script; std::string finalVars;
		while (fgets(line, sizeof line, f) && line[0] != '#') {
			line[strcspn(line, "\n")] = 0;
			if (line[0] == 'o') { script.push_back(Op()); script.back().text = line + 2; }
			else if (line[0] == 'r') script.back().result = line + 2;
			else if (line[0] == '.') script.back().log.push_back(line + 2);
			else if (line[0] == 'x') finalVars = line + 2;
		}
		world.reset();
		ScriptRunner runner(world, lib);
		byte vars[256] = {};
		const std::string v = unhex(hex);
		memcpy(vars, v.data(), v.size());
		bool bad = false;
		++sessions;
		for (const Op &op : script) {
			world.log.clear();
			char kind = op.text[0];
			unsigned a = 0, b = 0, c = 0, d = 0; unsigned long long w1 = 0, w2 = 0;
			std::string got = "-";
			if (kind == 'E') {
				unsigned long long speak, anim; unsigned step, nowv; int choice;
				sscanf(op.text.c_str() + 2, "%u %u %llu %llu %d", &step, &nowv, &speak, &anim, &choice);
				world.time = nowv; world.speak = speak; world.anim = anim; world.menuChoice = choice;
			} else if (kind == 'K') {
				sscanf(op.text.c_str() + 2, "%u %llu %llu %u", &a, &w1, &w2, &d);
				got = fmt("%d", runner.start(a, (w2 << 32) | w1, vars, reinterpret_cast<Level *>(uintptr_t(d))));
			} else if (kind == 'T') {
				sscanf(op.text.c_str() + 2, "%u", &a);
				got = fmt("%d", runner.tick(a));
			} else if (kind == 'R') {
				sscanf(op.text.c_str() + 2, "%u", &a);
				runner.resume(a);
			}
			++ops; calls += (int)op.log.size();
			if (got != op.result || world.log != op.log) {
				bad = true;
				if (opBad++ < 6) {
					printf("session %u op '%s': result got %s want %s\n", sid, op.text.c_str(), got.c_str(), op.result.c_str());
					for (size_t i = 0; i < std::max(world.log.size(), op.log.size()); ++i) {
						const std::string g = i < world.log.size() ? world.log[i] : "(none)", w = i < op.log.size() ? op.log[i] : "(none)";
						if (g != w) { printf("   call %zu got '%s' want '%s'\n", i, g.c_str(), w.c_str()); break; }
					}
				}
				break;
			}
		}
		if (!bad) {
			std::string gotVars;
			for (int i = 0; i < 64; ++i) gotVars += fmt("%02x", vars[i]);
			if (gotVars != finalVars) { bad = true; if (opBad++ < 6) printf("session %u: variables differ\n", sid); }
		}
		if (bad) ++sessionBad;
	}
	printf("Script blocks    %d/%d resolved across the four files\n", blocks - blockBad, blocks);
	printf("Script runner    %d/%d sessions match (%d operations, %d calls to the rest of the game)\n", sessions - sessionBad, sessions, ops, calls);
	return (sessionBad || blockBad) ? 1 : 0;
}
