// Compares the engine's text queue (layout and drawing, through ScreenDisplay) with vectors recorded from the original code
// (see re/tools/gen_vectors_text.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
	if (s == "-") return r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer fonts[2], background;
	VecStream *streams[3] = {nullptr, nullptr, nullptr};
	Tesoro::ScreenDisplay *screen = new Tesoro::ScreenDisplay();
	int caseNo = 0, nfont = 0, cases = 0, badCases = 0, lines = 0, badLines = 0, rows = 0, badRows = 0, shown = 0, layers = 0, badLayers = 0;
	bool queued = false;
	struct Want { std::string text; int x, y; unsigned colour, font; };
	std::vector<Want> want; unsigned wantTag = 0; std::vector<Tesoro::TextQueue::Line> got; bool caseBad = false;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op == "C") {
			if (nfont == 0) ++caseNo;
			std::string hex; unsigned entry; in >> hex >> entry;
			delete streams[nfont]; streams[nfont] = new VecStream(unhex(hex)); fonts[nfont] = Tesoro::AnimPlayer();
			if (!fonts[nfont].load(*streams[nfont], entry, 0, false)) { fprintf(stderr, "font does not load (case %d)\n", caseNo); return 3; }
			screen->setFont(nfont, fonts[nfont].bank());
			nfont = (nfont + 1) % 2;
		} else if (op == "Z") {
			screen->startFrame(); caseBad = false;
		} else if (op == "B") {
			std::string hex; in >> hex;
			delete streams[2]; streams[2] = new VecStream(unhex(hex)); background = Tesoro::AnimPlayer();
			if (!background.load(*streams[2], 0, 0, false)) { fprintf(stderr, "background does not load\n"); return 3; }
			screen->drawImage(0, 0, 65535, background.bank()->image(0), 0);
		} else if (op == "A") {
			std::string hex; unsigned colour, font, tag; int cx, y, width, centred, lh, f555; in >> hex >> colour >> font >> cx >> y >> width >> tag >> centred >> lh >> f555;
			const std::string text = unhex(hex);
			screen->setPixelFormat555(f555 != 0);
			screen->drawText((const byte *)text.c_str(), colour, font, cx, y, width, (uint16)tag, centred != 0, lh);
		} else if (op == "K") {
			want.clear(); queued = true;
		} else if (op == "N") {
			unsigned n; in >> n;
			want.clear();
		} else if (op == "R") {
			Want w; std::string hex; in >> hex >> w.x >> w.y >> w.colour >> w.font; w.text = unhex(hex); want.push_back(w);
		} else if (op == "T") {
			in >> wantTag;
			// The layout: the same lines, in the same places, in the same colours.
			const Tesoro::TextQueue &q = screen->_texts;
			bool ok = q.lineCount() == want.size();
			for (uint32 i = 0; i < want.size(); ++i) {
				++lines;
				bool same = i < q.lineCount() && q.line(i).x == want[i].x && q.line(i).y == want[i].y && q.line(i).colour == want[i].colour && q.line(i).font == want[i].font &&
				            q.line(i).chars.size() == want[i].text.size() && (want[i].text.empty() || memcmp(&q.line(i).chars[0], want[i].text.data(), want[i].text.size()) == 0);
				if (!same) { ok = false; ++badLines; if (shown++ < 6) printf("layout case %d line %u differs: got x %d y %d len %u want x %d y %d len %zu\n", caseNo, i, i < q.lineCount() ? q.line(i).x : -1, i < q.lineCount() ? q.line(i).y : -1, i < q.lineCount() ? (unsigned)q.line(i).chars.size() : 0, want[i].x, want[i].y, want[i].text.size()); }
			}
			if (q.lineCount() != want.size() && shown++ < 6) printf("layout case %d: %u lines, want %zu\n", caseNo, q.lineCount(), want.size());
			++layers; if (screen->_layerDepth != wantTag) { ++badLayers; ok = false; if (shown++ < 6) printf("layer depth case %d got %u want %u\n", caseNo, screen->_layerDepth, wantTag); }
			caseBad = !ok;
		} else if (op == "F") {
			for (uint i = 0; i < screen->_frame.size(); ++i) screen->_frame[i] = 0xabab;
			screen->compose();
			if (getenv("DUMPCASE") && atoi(getenv("DUMPCASE")) == caseNo) { FILE *df = fopen("local-media/text_port.bin", "wb"); fwrite(screen->frame(), 2, 640 * 480, df); fclose(df); }
			std::vector<unsigned long> crcs; unsigned long c; while (in >> c) crcs.push_back(c);
			bool ok = crcs.size() == 480;
			for (uint y = 0; ok && y < 480; ++y) {
				++rows;
				if (crc32(0, (const Bytef *)(screen->frame() + 640 * y), 1280) != crcs[y]) { ok = false; ++badRows; caseBad = true; if (shown++ < 6) printf("pixels case %d row %u differ\n", caseNo, y); }
			}
			++cases; if (caseBad) ++badCases;
		}
	}
	(void)queued; (void)got;
	printf("%-28s %d/%d match\n", "text layout (lines)", lines - badLines, lines);
	printf("%-28s %d/%d match\n", "text layer depth", layers - badLayers, layers);
	printf("%-28s %d/%d match\n", "text pixels (frame rows)", rows - badRows, rows);
	return (badCases) ? 1 : 0;
}
