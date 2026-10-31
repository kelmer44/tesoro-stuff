// Checks Jim::enterLevel against the order and effects read from the original (contract 00407b80).
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "common/stream.h"
#include "tesoro/core/jimwalker.h"
#include "tesoro/core/level.h"

using namespace Tesoro;

void warning(const char *s, ...) { (void)s; }
void error(const char *s, ...) { (void)s; ::abort(); }

static int failures = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

struct EmptyStream : Common::SeekableReadStream {
	bool eos() const override { return true; }
	uint32 read(void *, uint32) override { return 0; }
	int64 pos() const override { return 0; }
	int64 size() const override { return 0; }
	bool seek(int64, int) override { return true; }
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
};

struct World : JimWorld {
	std::vector<std::string> log;
	EmptyStream stream;
	int32 now() override { return 100; }
	uint32 randomNumber(uint32 lo, uint32) override { return lo; }
	uint32 playVoice(uint32) override { return 0; }
	bool voiceActive(uint32) override { return false; }
	void stopVoice(uint32) override {}
	bool skipPressed() override { return false; }
	bool subtitlesOn() override { return false; }
	void insertInMainList(Object *o, int p) override { log.push_back("insert " + std::to_string(p) + (o == lvl ? " level" : " other")); }
	void removeFromMainList(Object *o) override { log.push_back(std::string("remove") + (o == lvl ? " level" : " other")); }
	Common::SeekableReadStream &archiveStream() override { return stream; }
	bool is555() override { return false; }
	int useItemOnInterface() override { log.push_back("interface"); return 9; }
	bool hasWalkArea() override { return false; }
	int32 scaleAt(int32, int32) override { return 0x100; }
	bool findPath(int32, int32, WalkTarget *) override { return false; }
	bool hasWalkAreaOf(Object *) override { return false; }
	int interactiveUpdate(Jim &) override { return 1; }
	void drawSubtitle(Jim &, const SpeechTable::Line &) override {}
	Object *lvl = nullptr;
};

int main() {
	World w;
	Jim jim(w);
	Level a, b;
	Room ra, rb;
	ra.init(0, 1, 0, 0);
	rb.init(0, 1, 0, 0);
	a.setIndex(3);
	b.setIndex(8);
	jim.setPosition(40, 50);

	w.lvl = &a;
	jim.enterLevel(&a, &ra, 10, 20);
	CHECK(w.log.size() == 1 && w.log[0] == "insert 20 level");
	CHECK(jim.level() == &a && jim.room() == &ra);
	CHECK(jim.x() == 10 && jim.y() == 20 && jim.previousX() == 10 && jim.previousY() == 20);
	CHECK(jim.flags() & Object::kHitter);
	CHECK(ra.objects().startIteration() == false && ra.objects().value() == &jim);
	CHECK(a.objects().startIteration() == false && a.objects().value() == &jim);

	w.log.clear();
	w.lvl = &a;
	jim.enterLevel(&b, &rb, 5, 6);
	CHECK(w.log.size() == 2 && w.log[0] == "remove level" && w.log[1] == "insert 20 other");
	CHECK(a.objects().startIteration() == true);
	CHECK(ra.objects().startIteration() == true);
	CHECK(rb.objects().startIteration() == false);
	CHECK(jim.level() == &b && jim.room() == &rb);
	CHECK(!(jim.flags() & Object::kHitter));
	CHECK(jim.x() == 5 && jim.y() == 6);

	jim.useItem(37);
	jim.useItem(3);
	CHECK(w.log.back() == "interface" && w.log.size() == 3);
	printf(failures ? "enterLevel: %d failures\n" : "enterLevel: all checks passed\n", failures);
	return failures != 0;
}
