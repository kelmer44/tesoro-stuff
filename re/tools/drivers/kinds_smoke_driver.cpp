// Builds kinds 2, 15 and 16 of every shipped level with stub hosts and runs the path sprites for a while.
// Usage: kinds_smoke_driver <TESORO dir>   (read-only). Not a comparison with the original: it only shows that the
// shipped data builds, attaches and moves without warnings or runaway values.
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#include "tesoro/core/clickcatcher.h"
#include "tesoro/core/itemgroup.h"
#include "tesoro/core/level.h"
#include "tesoro/core/pathsprite.h"
#include "tesoro/core/shadowsprite.h"

static int g_warnings = 0;
void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { va_list a; va_start(a, s); fprintf(stderr, "warning: "); vfprintf(stderr, s, a); fprintf(stderr, "\n"); va_end(a); ++g_warnings; }

class FileStream : public Common::SeekableReadStream {
public:
	FileStream(FILE *f) : _f(f), _eos(false) { fseek(f, 0, SEEK_END); _size = ftell(f); fseek(f, 0, SEEK_SET); }
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override { size_t g = fread(out, 1, n, _f); if (g < n) _eos = true; return (uint32)g; }
	int64 pos() const override { return ftell(_f); }
	int64 size() const override { return _size; }
	bool seek(int64 off, int whence = SEEK_SET) override { _eos = false; return fseek(_f, (long)off, whence) == 0; }
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
private:
	FILE *_f; bool _eos; long _size;
};

struct RandomHost : Tesoro::PathSpriteHost {
	uint32 tick = 12345;
	uint32 next() { tick = tick * 1103515245u + 12345u; return tick >> 8; }
	uint32 randomNumber(uint32 lo, uint32 hi) override { return lo + next() % (hi - lo + 1); }
	uint32 randomAlt(uint32 lo, uint32 hi) override { return (uint32)((int32)lo + (int32)(next() % (uint32)((int32)hi - (int32)lo + 1))); }
};

struct Stub : Tesoro::InteractiveHost {
	int32 now() override { return 0; }
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
	void setHoverText(const char *) override {}
	void removeFromMainList(Tesoro::Object *) override {}
	void insertInMainList(Tesoro::Object *, int) override {}
	bool hasHeldItem() override { return false; }
	bool heldItemReply(uint64, const Tesoro::SpeechTable::Line *&) override { return false; }
	void dropHeldItem() override {}
};

static RandomHost g_random;
static Stub g_stub;

struct Probe : Tesoro::Level {
protected:
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &r, Tesoro::Room &room) override {
		if (r.kind == 2) { auto *o = new Tesoro::ItemGroup(); o->init(r, room.sortOffset(), banks()); return o; }
		if (r.kind == 4) { auto *o = new Tesoro::ShadowSprite(); o->init(r, room.sortOffset(), banks(), 0); return o; }
		if (r.kind == 15) { auto *o = new Tesoro::ClickCatcher(); o->init(r, room.sortOffset(), banks(), 0, g_stub); return o; }
		if (r.kind == 16) { auto *o = new Tesoro::PathSprite(); o->init(r, room.sortOffset(), banks(), 0, g_random); return o; }
		return Tesoro::Level::createObject(r, room);
	}
};

int main(int argc, char **argv) {
	if (argc < 2) return 1;
	std::string dir = argv[1];
	FILE *tre = fopen((dir + "/TESORO.TRE").c_str(), "rb"), *dat = fopen((dir + "/TESORO.DAT").c_str(), "rb");
	if (!tre || !dat) return 2;
	FileStream ds(dat);
	unsigned char rec[28];
	int built = 0, bad = 0;
	while (fread(rec, 28, 1, tre) == 1) {
		if (!(rec[8] & 4)) continue;
		std::string nm((char *)rec + 9, strnlen((char *)rec + 9, 19));
		for (auto &c : nm) c = (char)toupper((unsigned char)c);
		if (nm.size() < 4 || nm.substr(nm.size() - 4) != ".LVL") continue;
		uint32 off = rec[0] | rec[1] << 8 | rec[2] << 16 | (uint32)rec[3] << 24;
		Probe lv;
		if (!lv.load(ds, off + 4, 0, false, 0)) continue;
		lv.attachObjects();
		for (uint32 n = 0; n < lv.objects().count(); ++n) {
			Tesoro::Object *obj = lv.objects().valueAt(n);
			if (obj->kindId() == 2) {
				auto *g = static_cast<Tesoro::ItemGroup *>(obj);
				int nulls = 0;
				for (uint i = 0; i < g->members().size(); ++i) nulls += !g->members()[i];
				printf("%-12s kind 2 group: %u members, %d not found\n", nm.c_str(), (unsigned)g->members().size(), nulls);
				bad += nulls; ++built;
			} else if (obj->kindId() == 15) {
				printf("%-12s kind 15: sort %d flags %x\n", nm.c_str(), obj->sortValue(), obj->flags());
				++built;
			} else if (obj->kindId() == 16) {
				auto *p = static_cast<Tesoro::PathSprite *>(obj);
				int starts = 0, was = 0; int32 maxOff = 0;
				for (int32 t = 0; t < 600000; t += 40) {
					p->updateAnimation(t);
					const Tesoro::AnimPlayer *pl = p->animation().bankCount() ? &p->animation().current() : nullptr;
					if (pl) { int32 m = abs(pl->movementX()) + abs(pl->movementY()); if (m > maxOff) maxOff = m; }
					if (p->travelling() && !was) ++starts;
					was = p->travelling();
				}
				printf("%-12s kind 16: trips started in 10 min of clock: %d, largest offset %d\n", nm.c_str(), starts, maxOff);
				++built;
			}
		}
	}
	printf("built %d, problems %d, warnings %d\n", built, bad, g_warnings);
	return bad != 0;
}
