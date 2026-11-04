// Compares the engine's ScreenDisplay::compose with vectors recorded from the original code
// (see re/tools/gen_vectors_compose.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/animbank.h"
#define private public
#include "tesoro/core/screendisplay.h"
#undef private
#include <zlib.h>

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

static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

#include <csignal>
static char gLast[256]; static int gCase;
static void onCrash(int) { fprintf(stderr, "crash in case %d after: %s\n", gCase, gLast); _exit(4); }

int main(int argc, char **argv) {
	signal(SIGSEGV, onCrash); signal(SIGBUS, onCrash);
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer player; VecStream *stream = nullptr; std::map<int, std::string> maskWant; int maskRows = 0, badMask = 0;
	Tesoro::ScreenDisplay *screen = new Tesoro::ScreenDisplay();
	int caseNo = 0, frames = 0, badFrames = 0, rows = 0, badRows = 0, shown = 0;
	bool loaded = false;
	std::vector<uLong> want;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op != "C") strncpy(gLast, line.c_str(), 255); gCase = caseNo;
		if (op == "T") {
			std::string hex; in >> hex; std::string t = unhex(hex); screen->setShadeTable((const byte *)t.data());
		} else if (op == "M") {
			int y; in >> y; std::string rest; std::getline(in, rest); size_t p = rest.find_first_not_of(' '); maskWant[y] = p == std::string::npos ? "" : rest.substr(p);
		} else if (op == "F") {
			want.clear(); unsigned long c; while (in >> c) want.push_back(c);
			++frames; bool ok = want.size() == 480;
			for (uint y = 0; ok && y < 480; ++y) {
				++rows;
				if (crc32(0, (const Bytef *)(screen->frame() + 640 * y), 1280) != want[y]) { ok = false; ++badRows; if (shown++ < 5) printf("mismatch case %d row %u\n", caseNo, y); }
			}
			if (!ok) ++badFrames;
		} else if (op == "C") {
			++caseNo; std::string hex; unsigned entry; in >> hex >> entry;
			delete stream; stream = new VecStream(unhex(hex)); player = Tesoro::AnimPlayer();
			loaded = player.load(*stream, entry, 0, false);
		} else if (!loaded) {
			continue;
		} else if (op == "Z") {
			screen->startFrame();
		} else if (op == "S") {
			unsigned k; int x, y; unsigned depth; unsigned extra = 0; in >> k >> x >> y >> depth >> extra;
			screen->drawImage(x, y, (uint16)depth, player.bank()->image(k), extra);
		} else if (op == "Q") {
			unsigned k; int x, y; unsigned depth; int sx, sy, shear; in >> k >> x >> y >> depth >> sx >> sy >> shear;
			screen->drawImageScaled(x, y, (uint16)depth, player.bank()->image(k), sx, sy, shear, 0);
		} else if (op == "L") {
			unsigned layer; in >> layer; screen->setLayerDepth((uint16)layer);
		} else if (op == "K" || op == "N") {
			for (int y = 0; y < 480; ++y) {
				std::string got; const Tesoro::RenderQueue::Row &row = screen->_queue.maskRow(y);
				for (int n = row.head(); n; n = row.span(n).next) {
					const Tesoro::RenderQueue::Span &sp = row.span(n); char b[80];
					snprintf(b, sizeof b, "%s%d:%d:%u:%u:%u", got.empty() ? "" : " ", (int)sp.x0, (int)sp.x1, (unsigned)sp.depth, (unsigned)sp.pixels, (unsigned)sp.colour);
					got += b;
				}
				++maskRows; if (got != maskWant[y]) { ++badMask; if (shown++ < 5) printf("mask mismatch case %d row %d\n port     %s\n original %s\n", caseNo, y, got.c_str(), maskWant[y].c_str()); }
			}
			maskWant.clear();
			if (op == "N") continue;
			for (uint i = 0; i < screen->_frame.size(); ++i) screen->_frame[i] = 0xabab;
			screen->compose();
			if (getenv("DUMPFRAME")) { FILE *df = fopen(getenv("DUMPFRAME"), "wb"); fwrite(screen->frame(), 2, 640 * 480, df); fclose(df); }
		}
	}
	printf("%-28s %d/%d match\n", "mask rows before compose", maskRows - badMask, maskRows);
	printf("%-28s %d/%d match\n", "composed frames", frames - badFrames, frames);
	printf("%-28s %d/%d match\n", "frame rows", rows - badRows, rows);
	return (badFrames || badRows || badMask) ? 1 : 0;
}
