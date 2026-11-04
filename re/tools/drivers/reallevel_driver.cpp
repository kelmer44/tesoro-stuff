// (reallevel) Compares the engine's Level with vectors recorded from the original level code
// (see re/tools/gen_vectors_level.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#define private public
#include "tesoro/core/cameraframe.h"
#include "tesoro/core/display.h"
#include "tesoro/core/level.h"
#undef private

namespace Tesoro {
struct NullDisplay : Display { void drawImage(int32, int32, uint16, const AnimImage &, uint32) override {} };
}

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

// Minimal in-memory stream (all inline, so the driver needs no ScummVM library code).
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



struct FileStream : public Common::SeekableReadStream {
	FileStream(FILE *f) : _f(f), _eos(false) { fseek(f, 0, SEEK_END); _size = ftell(f); fseek(f, 0, SEEK_SET); }
	bool eos() const override { return _eos; }
	uint32 read(void *out, uint32 n) override { size_t g = fread(out, 1, n, _f); if (g < n) _eos = true; return (uint32)g; }
	int64 pos() const override { return ftell(_f); }
	int64 size() const override { return _size; }
	bool seek(int64 off, int whence = SEEK_SET) override { _eos = false; return fseek(_f, (long)off, whence) == 0; }
	char *readLine(char *, size_t, bool = true) override { return nullptr; }
	Common::String readLine(bool = true) override { return Common::String(); }
private: FILE *_f; bool _eos; long _size;
};

/** Every kind but the camera frame is built as the plain animated object, as the original's run was told to. */
struct CoercedLevel : Tesoro::Level {
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &record, Tesoro::Room &room) override {
		Tesoro::ObjectRecord r = record;
		if (r.kind != 1) r.kind = 0;
		return Tesoro::Level::createObject(r, room);
	}
};

static std::string stateOf(Tesoro::Level &lv, bool withBounds) {
	std::ostringstream o;
	const Tesoro::Camera &c = lv.camera();
	o << "cam " << c._startX << " " << c._startY << " " << c._startTime << " " << c._duration << " " << c._goalX << " " << c._goalY << " "
	  << c._boundLeft << " " << c._boundTop << " " << c._boundRight << " " << c._boundBottom << " " << c._x << " " << c._y << " "
	  << c._scaledX << " " << c._scaledY << " " << c._divisor;
	auto id = [&](Tesoro::Object *obj) {
		for (uint i = 0; i < lv._objects.count(); ++i)
			if (lv._objects.valueAt(i) == obj) return (int)i;
		return -1;
	};
	o << " | mv";
	if (lv._movedObjects.count() == 0) o << " -";
	for (uint i = 0; i < lv._movedObjects.count(); ++i) o << " " << id(lv._movedObjects.valueAt(i));
	for (uint r = 0; r < lv.roomCount(); ++r) {
		const Tesoro::Room &room = lv.room(r);
		o << " | room " << room.index() << " " << room.cameraDivisor() << " " << room.sortOffset() << " " << room._extent << " :";
		if (room.objects().count() == 0) o << " -";
		for (uint i = 0; i < room.objects().count(); ++i) o << " " << id(room.objects().valueAt(i));
	}
	o << " | n " << lv.roomCount() << " " << lv.bankCount() << " " << lv._objects.count() << " " << lv.loadTime() << " " << (int)lv.deferredReloaded() << " " << (int)(lv.specialObject() != nullptr);
	for (uint b = 0; b < lv.bankCount(); ++b) {
		const Tesoro::AnimPlayer &p = lv.banks()[b];
		o << " | bank " << p.bank()->sequenceCount() << " " << p.bank()->imageCount() << " " << p.flags();
	}
	for (uint i = 0; i < lv._objects.count(); ++i) {
		Tesoro::Object *obj = lv._objects.valueAt(i);
		o << " | obj " << (obj->nameKey() & 0xffffffffu) << " " << (obj->nameKey() >> 32) << " " << (int)obj->kindId() << " " << obj->x() << " " << obj->y() << " "
		  << obj->sortValue() << " " << obj->animation().bankCount() << " " << obj->flags() << " " << obj->room()->index() << " " << obj->previousX() << " " << obj->previousY();
		for (uint k = 0; k < obj->animation().bankCount(); ++k) {
			Tesoro::AnimPlayer &p = obj->animation().player(k);
			o << " " << p.bank()->sequenceCount() << "." << p.bank()->imageCount() << "." << p.flags();
		}
		if (obj->kindId() == 1) {
			const Tesoro::CameraFrame *cf = static_cast<const Tesoro::CameraFrame *>(obj);
			char hex[32]; for (int b = 0; b < 12; ++b) snprintf(hex + 2 * b, 3, "%02x", (byte)cf->targetName()[b]);
			o << " k1 " << hex << " " << id(cf->target()) << " " << (int)cf->flag() << " " << cf->value() << " " << cf->count();
		}
		if (withBounds) {
			const Tesoro::ObjectRect &h = obj->hitRect(), &m = obj->movedArea();
			o << " b " << h.x1 << " " << h.y1 << " " << h.x2 << " " << h.y2 << " " << m.x1 << " " << m.y1 << " " << m.x2 << " " << m.y2 << " " << obj->shownSequence();
		}
	}
	return o.str();
}


int main(int argc, char **argv) {
	// usage: reallevel_driver <vectors> <TESORO folder>
	FILE *f = fopen(argv[1], "r");
	FILE *dat = fopen((std::string(argv[2]) + "/TESORO.DAT").c_str(), "rb");
	if (!f || !dat) { fprintf(stderr, "no vectors or data\n"); return 2; }
	FileStream ds(dat);
	static char buf[1 << 24];
	CoercedLevel *lv = nullptr; bool loaded = false;
	int ok = 0, bad = 0, levels = 0; std::string name; bool attached = false;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "R") {
			unsigned entry; in >> name >> entry; ++levels; attached = false;
			delete lv; lv = new CoercedLevel();
			loaded = lv->load(ds, entry, 2, false, 0);
		} else if (op == "A") { lv->attachObjects(); attached = true; }
		else if (op == "=") {
			std::string want(buf + 2); while (!want.empty() && want.back() == '\n') want.pop_back();
			std::string got = loaded ? stateOf(*lv, false) : "load failed";
			// the kind number each side stores is an artefact of building every kind as the plain object: leave it out
			static const std::regex kindField(" \\| obj ([0-9]+) ([0-9]+) [0-9]+ ");
			want = std::regex_replace(want, kindField, " | obj $1 $2 K ");
			got = std::regex_replace(got, kindField, " | obj $1 $2 K ");
			if (want == got) ++ok;
			else {
				++bad;
				size_t k = 0; while (k < want.size() && k < got.size() && want[k] == got[k]) ++k;
				printf("mismatch in %s (%s) at char %zu\n want ...%s\n got  ...%s\n", name.c_str(), attached ? "after attach" : "after load", k, want.substr(k > 80 ? k - 80 : 0, 200).c_str(), got.substr(k > 80 ? k - 80 : 0, 200).c_str());
			}
		}
	}
	printf("real levels: %d levels, %d states match, %d differ\n", levels, ok, bad);
	delete lv; return bad != 0;
}
