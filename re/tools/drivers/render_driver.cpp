// Compares the engine's RenderQueue with vectors recorded from the original code
// (see re/tools/gen_vectors_render.py). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/renderqueue.h"

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

static std::string describe(const Tesoro::RenderQueue &queue, const Tesoro::AnimBank &bank, uint y) {
	const Tesoro::RenderQueue::Row &row = queue.row(y);
	int free = 0; for (int n = row.freeHead(); n && free < 200; n = row.span(n).next) ++free;
	if (row.head() == 0 && row.bumpCount() == 2 && free == 0) return "";
	std::ostringstream o; o << "R " << y << " " << row.bumpCount() << " " << free << " : ";
	bool first = true; int count = 0;
	for (int n = row.head(); n && count < 200; n = row.span(n).next, ++count) {
		const Tesoro::RenderQueue::Span &sp = row.span(n);
		std::ostringstream src;
		if (sp.pixels < 2) src << sp.pixels;
		else {
			bool found = false;
			for (uint k = 0; k < bank.imageCount() && !found; ++k) {
				const uintptr_t base = (uintptr_t)bank.image(k).data();
				if (base && sp.pixels >= base && sp.pixels <= base + bank.image(k).size()) { src << k << ":" << (sp.pixels - base); found = true; }
			}
			if (!found) src << "?" << sp.pixels;
		}
		if (!first) o << ";";
		first = false;
		o << sp.x0 << "," << sp.x1 << "," << sp.depth << "," << src.str();
	}
	return o.str();
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer player; VecStream *stream = nullptr;
	Tesoro::RenderQueue *queue = new Tesoro::RenderQueue();
	int caseNo = 0, dumps = 0, badDumps = 0, rows = 0, badRows = 0, shown = 0;
	bool loaded = false, collecting = false;
	std::vector<std::string> expected;
	auto compare = [&]() {
		if (!collecting) return;
		collecting = false; ++dumps; bool ok = true;
		std::vector<std::string> got;
		for (uint y = 0; y < 480; ++y) { std::string d = describe(*queue, *player.bank(), y); if (!d.empty()) got.push_back(d); }
		rows += (int)expected.size();
		if (got.size() != expected.size()) ok = false;
		for (size_t i = 0; i < expected.size() && i < got.size(); ++i)
			if (got[i] != expected[i]) { ok = false; ++badRows; if (shown++ < 5) printf("mismatch case %d\n want %s\n got  %s\n", caseNo, expected[i].c_str(), got[i].c_str()); }
		if (!ok) { ++badDumps; if (got.size() != expected.size() && shown++ < 5) printf("mismatch case %d: %zu rows, want %zu\n", caseNo, got.size(), expected.size()); }
	};
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op == "R") { expected.push_back(line); continue; }
		compare();
		if (op == "C") {
			++caseNo; std::string hex; unsigned entry; in >> hex >> entry;
			delete stream; stream = new VecStream(unhex(hex)); player = Tesoro::AnimPlayer();
			loaded = player.load(*stream, entry, 0, false);
		} else if (!loaded) {
			continue;
		} else if (op == "Z") {
			queue->reset();
		} else if (op == "S") {
			unsigned k; int x, y; unsigned depth; in >> k >> x >> y >> depth;
			queue->addSprite(x, y, (uint16)depth, player.bank()->image(k), 0);
		} else if (op == "D") {
			expected.clear(); collecting = true;
		}
	}
	compare();
	printf("%-28s %d/%d match\n", "span dumps", dumps - badDumps, dumps);
	printf("%-28s %d/%d match\n", "rows", rows - badRows, rows);
	return (badDumps || badRows) ? 1 : 0;
}
