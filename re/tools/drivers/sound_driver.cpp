// Compares the engine's SoundDevice with the original play / find / stop / update (see re/tools/gen_vectors_sound.py): every session replays the
// same operations and must give the same ids, the same record order and the same loads and stops.
// Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "common/scummsys.h"
#include "common/stream.h"
#include "tesoro/core/sounddevice.h"

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
static std::string unhex(const char *s) {
	std::string r;
	for (size_t i = 0; s[i] && s[i + 1] && s[i] != '\n'; i += 2) { char b[3] = {s[i], s[i + 1], 0}; r += (char)strtol(b, nullptr, 16); }
	return r;
}

struct Backend : VoiceBackend {
	std::vector<std::string> log; unsigned mask = 0;
	void start(uint slot, const byte *, uint32, uint32) override { log.push_back(fmt("load %u", slot)); }
	bool playing(uint slot) override { return (mask >> slot) & 1; }
	void stop(uint slot) override { log.push_back(fmt("stop %u", slot)); }
};

static std::string stateOf(const SoundDevice &d) {
	std::string s = fmt("%u %u", d.lastId(), d.activeCount());
	for (uint i = 0; i < 3; ++i) s += fmt(" %u", d.slotAt(i));
	for (uint i = 0; i < 3; ++i) s += fmt(" %u", d.idOfSlot(i));
	return s;
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char line[1 << 16];
	std::vector<std::string> wavs;
	std::string archive(0x1000 * 12, '\0');
	int sessions = 0, bad = 0, ops = 0, shown = 0;
	struct Op { std::string text, result, state; std::vector<std::string> log; };
	bool have = fgets(line, sizeof line, f);
	while (have && line[0] == 'W') { wavs.push_back(unhex(line + 2)); have = fgets(line, sizeof line, f); }
	for (size_t k = 0; k < wavs.size(); ++k) memcpy(&archive[0x1000 * (k + 1)], wavs[k].data(), wavs[k].size());
	while (have) {
		if (line[0] != 'S') { have = fgets(line, sizeof line, f); continue; }
		std::vector<Op> script;
		while ((have = fgets(line, sizeof line, f)) && line[0] != '#') {
			line[strcspn(line, "\n")] = 0;
			if (line[0] == 'o') { script.push_back(Op()); script.back().text = line + 2; }
			else if (line[0] == 'r') script.back().result = line + 2;
			else if (line[0] == 's') script.back().state = line + 2;
			else if (line[0] == '.') script.back().log.push_back(line + 2);
		}
		have = fgets(line, sizeof line, f);
		Backend backend; SoundDevice device(backend);
		VecStream dat(archive);
		++sessions; bool badSession = false;
		for (const Op &op : script) {
			backend.log.clear(); const char kind = op.text[0]; std::string got = "0";
			const char *a = op.text.c_str() + 2;
			if (kind == 'P') { int k; sscanf(a, "%d", &k); got = fmt("%u", device.play(dat, 0x1000 * (k + 1))); }
			else if (kind == 'F') { unsigned id; sscanf(a, "%u", &id); got = fmt("%d", device.active(id)); }
			else if (kind == 'T') { unsigned id; sscanf(a, "%u", &id); const bool was = device.active(id); device.stop(id); got = fmt("%d", was); }
			else if (kind == 'U') device.update();
			else if (kind == 'M') { unsigned m; sscanf(a, "%u", &m); backend.mask = m; }
			++ops;
			if (got != op.result || backend.log != op.log || stateOf(device) != op.state) {
				badSession = true;
				if (shown++ < 6) printf("op '%s': result got %s want %s\n state got [%s]\n state want [%s]\n", op.text.c_str(), got.c_str(), op.result.c_str(), stateOf(device).c_str(), op.state.c_str());
				break;
			}
		}
		if (badSession) ++bad;
	}
	printf("Sound device     %d/%d sessions match (%d operations)\n", sessions - bad, sessions, ops);
	return bad ? 1 : 0;
}
