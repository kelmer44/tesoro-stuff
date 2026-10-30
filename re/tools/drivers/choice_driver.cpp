// Compares the engine's ChoiceMenu (with TextQueue underneath) with the original Dialogue_ChoiceMenu and text layout (see re/tools/gen_vectors_choice.py):
// every run must return the same choice, make the same pointer and label calls and leave the same text lines in the same places.
// Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/scriptrunner.h"

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
	char buf[256]; va_list ap; va_start(ap, f); vsnprintf(buf, sizeof buf, f, ap); va_end(ap); return buf;
}
static std::string unhex(const std::string &s) {
	std::string r;
	if (s == "-") return r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

struct Host : ChoiceMenuHost {
	std::vector<std::string> log; int mx = 0, my = 0; bool button = false;
	void clearLabel() override { log.push_back("label null -1"); }
	int32 mouseX() override { return mx; }
	int32 mouseY() override { return my; }
	bool leftButton() override { return button; }
	void showCursorAt(int32 x, int32 y) override { log.push_back(fmt("cursor %d %d %d", x, y, 0x777000)); }
	void removeJimFromMainList() override { log.push_back("remove jim"); }
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer font; VecStream *stream = nullptr;
	Tesoro::ScreenDisplay *screen = new Tesoro::ScreenDisplay();
	Host host;
	std::vector<SpeechTable::Line> lines;
	std::vector<std::string> wantLog; std::string wantResult;
	struct Want { std::string text; int x, y; unsigned colour, font; };
	std::vector<Want> wantLines; unsigned wantTag = 0;
	int runs = 0, bad = 0, shown = 0, caseNo = 0;
	bool pending = false; int got = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op == "C") {
			++caseNo; std::string hex; unsigned entry; in >> hex >> entry;
			delete stream; stream = new VecStream(unhex(hex)); font = Tesoro::AnimPlayer();
			if (!font.load(*stream, entry, 0, false)) { fprintf(stderr, "font does not load\n"); return 3; }
			screen->setFont(0, font.bank());
		} else if (op == "P") {
			unsigned n; in >> n; lines.assign(n, SpeechTable::Line());
			for (unsigned i = 0; i < n; ++i) {
				std::string hex; in >> hex; const std::string t = unhex(hex);
				lines[i].text.clear(); for (char c : t) lines[i].text.push_back(c); lines[i].text.push_back(0);
			}
		} else if (op == "Z") {
			screen->startFrame();
		} else if (op == "A") {
			std::string hex; unsigned colour, fnt, tag; int cx, y, width, centred, lh, f555; in >> hex >> colour >> fnt >> cx >> y >> width >> tag >> centred >> lh >> f555;
			const std::string text = unhex(hex);
			screen->drawText((const byte *)text.c_str(), colour, fnt, cx, y, width, (uint16)tag, centred != 0, lh);
		} else if (op == "M") {
			in >> host.mx >> host.my; int b; in >> b; host.button = b != 0;
			host.log.clear(); wantLines.clear(); wantLog.clear();
			std::vector<const SpeechTable::Line *> ptrs; for (auto &l : lines) ptrs.push_back(&l);
			got = ChoiceMenu::run(host, *screen, ptrs.data(), ptrs.size());
			pending = true;
		} else if (op == "r") {
			in >> wantResult;
		} else if (op == ".") {
			wantLog.push_back(line.substr(2));
		} else if (op == "R") {
			Want w; std::string hex; in >> hex >> w.x >> w.y >> w.colour >> w.font; w.text = unhex(hex); wantLines.push_back(w);
		} else if (op == "T" && pending) {
			in >> wantTag; pending = false; ++runs;
			const TextQueue &q = screen->_texts;
			bool ok = std::to_string(got) == wantResult && host.log == wantLog && q.lineCount() == wantLines.size() && screen->_layerDepth == wantTag;
			for (uint32 i = 0; ok && i < wantLines.size(); ++i) {
				const TextQueue::Line &l = q.line(i);
				ok = l.x == wantLines[i].x && l.y == wantLines[i].y && l.colour == wantLines[i].colour && l.font == wantLines[i].font &&
				     l.chars.size() == wantLines[i].text.size() && (wantLines[i].text.empty() || memcmp(&l.chars[0], wantLines[i].text.data(), wantLines[i].text.size()) == 0);
			}
			if (!ok) {
				++bad;
				if (shown++ < 6) {
					printf("case %d run at mouse (%d, %d) button %d: result got %d want %s, %zu/%zu lines, %zu/%zu calls\n", caseNo, host.mx, host.my, host.button, got, wantResult.c_str(), (size_t)q.lineCount(), wantLines.size(), host.log.size(), wantLog.size());
					for (uint32 i = 0; i < wantLines.size() && i < q.lineCount(); ++i)
						if (q.line(i).y != wantLines[i].y || q.line(i).colour != wantLines[i].colour) { printf("   line %u got y %d colour %u want y %d colour %u\n", i, q.line(i).y, q.line(i).colour, wantLines[i].y, wantLines[i].colour); break; }
				}
			}
		}
	}
	printf("Choice menu      %d/%d runs match\n", runs - bad, runs);
	return bad ? 1 : 0;
}
