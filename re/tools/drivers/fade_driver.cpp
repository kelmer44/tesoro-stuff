// Compares the engine's ScreenDisplay::fadeOut with vectors recorded from the original code (see re/tools/gen_vectors_fade.py).
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <zlib.h>
#include "common/scummsys.h"
#define private public
#include "tesoro/core/screendisplay.h"
#undef private

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

struct Recorder : Tesoro::FadePresenter {
	std::vector<unsigned long> crcs; int begins = 0, ends = 0, lastDelay = -1;
	void beginStep() override { ++begins; }
	void show(const uint16 *frame) override { crcs.push_back(crc32(0, (const Bytef *)frame, 640 * 480 * 2)); }
	void endStep(int delay) override { ++ends; lastDelay = delay; }
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) return 2;
	static char buf[1 << 16];
	Tesoro::ScreenDisplay *screen = new Tesoro::ScreenDisplay();
	unsigned seed = 0; int steps = 0, is555 = 0, delay = 0, total = 0, bad = 0, frames = 0;
	Recorder rec;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "Z") {
			in >> seed >> steps >> is555 >> delay;
			unsigned x = seed; byte *p = (byte *)&screen->_frame[0];
			for (int i = 0; i < 640 * 480 * 2; ++i) { x = x * 1103515245u + 12345u; p[i] = (byte)((x >> 16) & 0xff); }
			screen->setPixelFormat555(is555 != 0);
			rec = Recorder(); screen->fadeOut(steps, delay, rec);
		} else if (op == "F") {
			std::vector<unsigned long> want; unsigned long c; while (in >> c) want.push_back(c);
			++total; frames += (int)want.size();
			if (want != rec.crcs || rec.begins != (int)want.size() || rec.ends != (int)want.size() || rec.lastDelay != delay) { ++bad; if (bad < 5) printf("mismatch: steps %d is555 %d, %zu frames want, %zu got\n", steps, is555, want.size(), rec.crcs.size()); }
		}
	}
	printf("%-28s %d/%d match (%d frames)\n", "fade out", total - bad, total, frames);
	return bad ? 1 : 0;
}
