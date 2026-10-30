// Compares the engine's Level with vectors recorded from the original level code
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
#include "common/scummsys.h"
#include "common/array.h"
#include "common/stream.h"
#define private public
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


static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}

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
		if (withBounds) {
			const Tesoro::ObjectRect &h = obj->hitRect(), &m = obj->movedArea();
			o << " b " << h.x1 << " " << h.y1 << " " << h.x2 << " " << h.y2 << " " << m.x1 << " " << m.y1 << " " << m.x2 << " " << m.y2 << " " << obj->shownSequence();
		}
	}
	return o.str();
}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 22];
	Tesoro::Level *lv = nullptr;
	VecStream *stream = nullptr;
	bool is555 = false, loaded = false, withBounds = false;
	int caseNo = 0, ok = 0, bad = 0;
	std::string lastOp;
	std::map<std::string, std::pair<int, int>> byOp;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") {
			++caseNo; if (getenv("TRACE")) fprintf(stderr, "case %d\n", caseNo); std::string hex; unsigned entry; int mode, f555, now; in >> hex >> entry >> mode >> f555 >> now;
			delete lv; delete stream;
			stream = new VecStream(unhex(hex)); is555 = f555 != 0;
			lv = new Tesoro::Level();
			loaded = lv->load(*stream, entry, mode, is555, now);
			lastOp = "load"; withBounds = false;
		} else if (op == "F") { unsigned i, fl; in >> i >> fl; lv->_objects.valueAt(i)->setFlags(fl); lastOp = "flags"; }
		else if (op == "A") { lv->attachObjects(); lastOp = "attach"; }
		else if (op == "M") { unsigned i; in >> i; lv->objectMoved(lv->_objects.valueAt(i)); lastOp = "moved"; }
		else if (op == "K") {
			unsigned lo, hi; in >> lo >> hi; Tesoro::Object *found = lv->findObject((uint64)lo | ((uint64)hi << 32));
			int idx = -1; for (uint i = 0; i < lv->_objects.count(); ++i) if (lv->_objects.valueAt(i) == found) idx = (int)i;
			lastOp = "find"; buf[0] = 0; fgets(buf, sizeof buf, f);
			std::string want(buf); while (!want.empty() && (want.back() == '\n')) want.pop_back();
			const bool same = want == "= " + std::to_string(idx);
			auto &c = byOp[lastOp]; ++c.first; if (!same) { ++c.second; ++bad; if (bad < 10) printf("mismatch find case %d: want %s got %d\n", caseNo, want.c_str(), idx); } else ++ok;
		}
		else if (op == "G") { int x, y, d, now; in >> x >> y >> d >> now; lv->cameraMoveTo(x, y, d, now); lastOp = "cameraMoveTo"; }
		else if (op == "U") { int now; in >> now; Tesoro::NullDisplay nd; lv->updateRoomCameras(now, nd); lastOp = "updateRoomCameras"; withBounds = true; }
		else if (op == "=") {
			std::string want(buf + 2); while (!want.empty() && (want.back() == '\n')) want.pop_back();
			const std::string got = loaded ? stateOf(*lv, withBounds) : "load failed";
			withBounds = false;
			const bool same = want == got;
			auto &c = byOp[lastOp]; ++c.first;
			if (!same) {
				++c.second; ++bad;
				if (bad < 6) {
					size_t k = 0; while (k < want.size() && k < got.size() && want[k] == got[k]) ++k;
					printf("mismatch after %s in case %d at char %zu\n want ...%s\n got  ...%s\n", lastOp.c_str(), caseNo, k, want.substr(k > 60 ? k - 60 : 0, 160).c_str(), got.substr(k > 60 ? k - 60 : 0, 160).c_str());
				}
			} else ++ok;
		}
	}
	for (auto &kv : byOp) printf("%-20s %d/%d match\n", kv.first.c_str(), kv.second.first - kv.second.second, kv.second.first);
	delete lv; delete stream;
	return bad ? 1 : 0;
}
