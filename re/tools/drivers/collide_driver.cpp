// Compares the engine's Level::collisionPass with vectors recorded from the original code
// (see re/tools/gen_vectors_collide.py). Build/run: re/tools/run_startup_check.sh
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
#define protected public
#include "tesoro/core/level.h"
#undef private
#undef protected

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

struct Script {
	std::vector<Tesoro::Object *> objects;
	std::vector<std::string> events;
	std::map<std::pair<int, int>, bool> hitter;
	int indexOf(const Tesoro::Object *o) const { for (size_t i = 0; i < objects.size(); ++i) if (objects[i] == o) return (int)i; return -1; }
} script;

// An object whose bounds step and collision answers are scripted, so the pass can be watched from outside.
class TestObject : public Tesoro::Object {
public:
	void updateBounds(int32) override { script.events.push_back("B" + std::to_string(script.indexOf(this))); }
	bool isHitter(const Tesoro::Object *other) const override {
		auto it = script.hitter.find(std::make_pair(script.indexOf(this), script.indexOf(other)));
		return it != script.hitter.end() && it->second;
	}
	int onTouched(Tesoro::Object *other, int extra) override {
		script.events.push_back("T" + std::to_string(script.indexOf(this)) + ":" + std::to_string(script.indexOf(other)) + ":" + std::to_string(extra));
		return 0;
	}
};

class TestLevel : public Tesoro::Level {
protected:
	Tesoro::Object *createObject(const Tesoro::ObjectRecord &record, Tesoro::Room &room) override {
		TestObject *o = new TestObject();
		o->initAnimated(record, room.sortOffset(), _banks, 0);
		return o;
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 22];
	TestLevel *lv = nullptr; VecStream *stream = nullptr;
	int caseNo = 0, checks = 0, passes = 0, badEvents = 0, badState = 0, touches = 0;
	std::string wantEvents;
	auto state = [&]() {
		std::ostringstream o;
		for (size_t i = 0; i < script.objects.size(); ++i) {
			Tesoro::Object *ob = script.objects[i]; if (i) o << " ";
			o << ob->x() << "," << ob->y() << "," << ob->flags();
		}
		o << " | mv " << lv->movedObjects().count() << " t " << lv->loadTime();
		return o.str();
	};
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		std::istringstream in(line); std::string op; in >> op;
		if (op == "C") {
			++caseNo; std::string hex; unsigned entry; int mode, f555, now; in >> hex >> entry >> mode >> f555 >> now;
			delete lv; delete stream; stream = new VecStream(unhex(hex)); lv = new TestLevel();
			lv->load(*stream, entry, mode, f555 != 0, now);
			script.objects.clear(); script.hitter.clear();
			for (uint i = 0; i < lv->_objects.count(); ++i) script.objects.push_back(lv->_objects.valueAt(i));
		} else if (!lv) continue;
		else if (op == "F") { unsigned i, fl; in >> i >> fl; script.objects[i]->setFlags(fl); }
		else if (op == "G") {
			unsigned i; int v[13]; in >> i; for (int k = 0; k < 13; ++k) in >> v[k];
			Tesoro::Object *o = script.objects[i];
			o->_hitRect.x1 = v[0]; o->_hitRect.y1 = v[1]; o->_hitRect.x2 = v[2]; o->_hitRect.y2 = v[3];
			o->_movedArea.x1 = v[4]; o->_movedArea.y1 = v[5]; o->_movedArea.x2 = v[6]; o->_movedArea.y2 = v[7];
			o->setPosition(v[8], v[9]); o->setPreviousPosition(v[10], v[11]); o->_sortValue = (int16)v[12];
		}
		else if (op == "H") { std::string bits; in >> bits; size_t n = script.objects.size(); script.hitter.clear(); for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < n; ++j) script.hitter[std::make_pair((int)i, (int)j)] = bits[i * n + j] == '1'; }
		else if (op == "P") { int i; in >> i; lv->setPlayer(i >= 0 ? script.objects[i] : nullptr); }
		else if (op == "A") lv->attachObjects();
		else if (op == "M") { unsigned i; in >> i; lv->objectMoved(script.objects[i]); }
		else if (op == "N") { unsigned i; int x, y, sc; in >> i >> x >> y >> sc; script.objects[i]->moveTo(x, y, sc != 0); }
		else if (op == "R") {
			int now; in >> now; script.events.clear(); lv->collisionPass(now); ++passes;
		} else if (op == "E") {
			std::string got; for (size_t i = 0; i < script.events.size(); ++i) { if (i) got += " "; got += script.events[i]; touches += script.events[i][0] == 'T'; }
			if (got.empty()) got = "-";
			std::string want = line.substr(2);
			if (got != want) { ++badEvents; if (badEvents < 6) printf("mismatch events case %d\n want %s\n got  %s\n", caseNo, want.c_str(), got.c_str()); }
		} else if (op == "=") {
			std::string got = state(), want = line.substr(2);
			++checks;
			if (got != want) { ++badState; if (badState < 6) printf("mismatch state case %d\n want %s\n got  %s\n", caseNo, want.c_str(), got.c_str()); }
		}
	}
	printf("%-28s %d/%d match (%d touches)\n", "collision pass events", passes - badEvents, passes, touches);
	printf("%-28s %d/%d match\n", "collision pass state", checks - badState, checks);
	return (badEvents || badState) ? 1 : 0;
}
