// Loads a level of the shipped game with the engine classes, runs one frame and writes it as a PPM picture.
// Usage: show_driver <TESORO dir> <LEVEL.LVL> <out.ppm> [555]   (read-only on the game files). Build/run: re/tools/run_startup_check.sh
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
#include "tesoro/core/archiveindex.h"
#include "tesoro/core/level.h"

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

static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

static std::string slurp(const std::string &path) {
	FILE *f = fopen(path.c_str(), "rb"); std::string r; if (!f) return r;
	char b[65536]; size_t n; while ((n = fread(b, 1, sizeof b, f)) > 0) r.append(b, n);
	fclose(f); return r;
}

int main(int argc, char **argv) {
	if (argc < 4) return 1;
	const std::string dir = argv[1]; const bool is555 = argc > 4;
	std::string tre = slurp(dir + "/TESORO.TRE");
	Tesoro::ArchiveIndex index; index.load((const byte *)tre.data(), (uint32)tre.size());
	std::string name = argv[2]; for (auto &c : name) c = (char)toupper((unsigned char)c);
	const uint32 offset = index.find(name.c_str());
	if (!offset) { fprintf(stderr, "no such entry\n"); return 2; }
	FILE *dat = fopen((dir + "/TESORO.DAT").c_str(), "rb");
	FileStream stream(dat);
	Tesoro::Level level;
	if (!level.load(stream, offset, 0, is555, 0)) { fprintf(stderr, "load failed\n"); return 3; }
	printf("%s: %u rooms, %u banks, %u objects\n", name.c_str(), level.roomCount(), level.bankCount(), (unsigned)level.objects().count());
	level.attachObjects();
	Tesoro::ScreenDisplay screen;
	screen.startFrame();
	level.updateRoomCameras(0, screen);
	screen.compose();
	FILE *out = fopen(argv[3], "wb");
	fprintf(out, "P6\n640 480\n255\n");
	for (uint i = 0; i < 640 * 480; ++i) {
		const uint16 p = screen.frame()[i]; byte rgb[3];
		if (is555) { rgb[0] = (p >> 10 & 31) * 255 / 31; rgb[1] = (p >> 5 & 31) * 255 / 31; rgb[2] = (p & 31) * 255 / 31; }
		else { rgb[0] = (p >> 11 & 31) * 255 / 31; rgb[1] = (p >> 5 & 63) * 255 / 63; rgb[2] = (p & 31) * 255 / 31; }
		fwrite(rgb, 1, 3, out);
	}
	fclose(out);
	return 0;
}
