// Compares the engine's pointer frame step (Cursor::update with the menu key, the interface pages and the menu opening) with vectors
// recorded from the original code (see re/tools/gen_vectors_cursorupdate.py). Build/run: re/tools/run_startup_check.sh
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
#include "common/stream.h"
#define private public
#define protected public
#include "tesoro/core/camera.h"
#include "tesoro/core/display.h"
#include "tesoro/core/cursor.h"
#include "tesoro/core/level.h"
#include "tesoro/core/room.h"
#include "tesoro/core/inventorystrip.h"
#include "tesoro/core/itemgroup.h"
#include "tesoro/core/mouse.h"
#undef private
#undef protected

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
static int g_warnings = 0;
void warning(const char *s, ...) { (void)s; ++g_warnings; }

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

struct NullDisplay : Tesoro::Display {
	void drawImage(int32, int32, uint16, const Tesoro::AnimImage &, uint32) override {}
};

static std::string unhex(const std::string &s) {
	std::string r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}
static std::string hexOf(const std::string &s) {
	std::string r; char b[4];
	for (unsigned char c : s) { snprintf(b, sizeof b, "%02x", c); r += b; }
	return r;
}
static std::string norm(const std::string &s) {
	std::istringstream in(s); std::string tok, out;
	while (in >> tok) out += (out.empty() ? "" : " ") + tok;
	return out;
}

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};


static std::string hex(const byte *p, int n) { std::string s; char b[3]; for (int i = 0; i < n; ++i) { snprintf(b, sizeof b, "%02x", p[i]); s += b; } return s; }

static uint32 crc32of(const byte *p, size_t n) {
	uint32 c = 0xffffffffu;
	for (size_t i = 0; i < n; ++i) { c ^= p[i]; for (int k = 0; k < 8; ++k) c = (c >> 1) ^ (0xedb88320u & (0u - (c & 1))); }
	return ~c;
}

struct Spy : Tesoro::Object {
	std::vector<std::string> *log = nullptr;
	void setActive(bool on) override { log->push_back(on ? "save_1" : "save_0"); Tesoro::Object::setActive(on); }
};

struct Host : Tesoro::CursorHost {
	std::vector<std::string> log;
	int keyState = 0, level = 0, page = 0, nowMs = 0;
	Place where = kGame;
	Tesoro::Level *iface = nullptr;
	Tesoro::Cursor *cursor = nullptr;
	Tesoro::Object *salvar = nullptr, *textos = nullptr;
	uint32 seed = 0;
	std::vector<uint16> thumb; std::vector<byte> slotPicture; int slot = 0;
	bool menuKeyPressed() override { return (keyState & 8) != 0; }
	void acknowledgeKeys() override { log.push_back("ack"); }
	Place place() override { return where; }
	int levelIndex() override { return level; }
	int32 now() override { return nowMs; }
	void fadeOut(int a, int b) override { log.push_back("fade_" + std::to_string(a) + "_" + std::to_string(b)); }
	void clearMainList() override { log.push_back("clearlist"); }
	void enterMainMenu() override { log.push_back("enter_main_1_0_0"); }
	void jimResumeFacing() override { log.push_back("resume"); }
	Tesoro::Object *interfaceObject(const char *name) override { return !strcmp(name, "SALVAR") ? salvar : !strcmp(name, "TEXTOS") ? textos : nullptr; }
	int menuPage() override { return page; }
	void captureThumbnail() override {
		log.push_back("capture_" + std::to_string(iface->camera()._y) + "_" + std::to_string(cursor->_y));
		std::vector<uint16> frame(640 * 480); uint32 v = seed;
		for (size_t i = 0; i < frame.size(); ++i) { v = v * 1103515245u + 12345u; frame[i] = (uint16)((v >> 8) & 0xffff); }
		thumb.assign(Tesoro::Cursor::kThumbWidth * Tesoro::Cursor::kThumbHeight, 0x0909);
		Tesoro::Cursor::sampleScene(&frame[0], &thumb[0]);
	}
	void menuOpened() override { slot = 0; std::fill(slotPicture.begin(), slotPicture.end(), 0); }
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	Tesoro::AnimPlayer bank; VecStream *bs = nullptr; std::string bdat;
	Tally tally; int caseNo = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "B") {
			std::string hexs; in >> hexs; bdat = unhex(hexs); delete bs; bs = new VecStream(bdat); bank = Tesoro::AnimPlayer();
			tally.check("bank loads", bank.load(*bs, 0, 0, false), 0);
		} else if (op == "K") {
			++caseNo;
			std::string place; int gidx, cx, cy, jcx, jcy, key, right, mx, my, subs, held, hflags, stripx, sqx, nm, curx, cury, label, slotw; uint32 seed;
			in >> place >> gidx >> cx >> cy >> jcx >> jcy >> key >> right >> mx >> my >> subs >> held >> hflags >> stripx >> sqx >> nm;
			int memx[5]; for (int i = 0; i < 5; ++i) in >> memx[i];
			in >> curx >> cury >> label >> slotw >> seed;
			std::string bar, results; std::getline(in, bar, '|'); std::getline(in, results);
			const int32 now = 4000 + caseNo;

			Common::Array<Tesoro::AnimPlayer> banks; banks.push_back(bank);
			Tesoro::ObjectRecord rec; strcpy(rec.name, "OBJ"); rec.banks.push_back(0);
			Tesoro::Level iface, player;
			iface._rooms.push_back(Tesoro::Room()); player._rooms.push_back(Tesoro::Room());
			iface.camera()._x = cx; iface.camera()._y = cy; player.camera()._x = jcx; player.camera()._y = jcy;
			player._index = gidx;
			Tesoro::Mouse mouse; mouse.moveTo(mx, my);
			mouse._buttons.set(0, 0, 0); mouse._buttons.set(1, (byte)right, 0);
			Tesoro::Cursor cursor; cursor.initAnimated(rec, 0, banks, 0);
			cursor._level = &iface; cursor._x = curx; cursor._y = cury; cursor._previousX = 0; cursor._previousY = 0;
			cursor._companion = new Tesoro::Cursor();
			cursor._companion->_x = 1; cursor._companion->_y = 2; cursor._companion->_previousX = 3; cursor._companion->_previousY = 4;
			cursor.setSurroundings(&mouse, &player);
			if (label) cursor.label().set("A", 0);
			Tesoro::Object heldItem; heldItem.setFlags((uint32)hflags);
			cursor.setHeldItem(held ? &heldItem : nullptr);
			Spy salvar; Tesoro::Object textos; textos.initAnimated(rec, 0, banks, 0);
			salvar.setFlags(0);
			Tesoro::Object square; square._x = sqx;
			Tesoro::Object members[5]; for (int i = 0; i < 5; ++i) members[i]._x = memx[i];
			Tesoro::ItemGroup group; for (int i = 0; i < nm; ++i) group._members.push_back(&members[i]);
			Tesoro::InventoryStrip strip; strip._x = stripx; strip._square = &square; strip._group = &group;
			cursor.setStrip(&strip);

			Host host; host.keyState = key; host.nowMs = now; host.seed = seed; host.page = subs;
			host.where = place == "main" ? Tesoro::CursorHost::kMainMenu : place == "new" ? Tesoro::CursorHost::kNewGameMenu : place == "board" ? Tesoro::CursorHost::kBoard : Tesoro::CursorHost::kGame;
			host.level = gidx; host.iface = &iface; host.cursor = &cursor; host.salvar = &salvar; host.textos = &textos;
			host.slotPicture.assign(0x10a40, 7); host.slot = slotw; host.thumb.assign(Tesoro::Cursor::kThumbWidth * Tesoro::Cursor::kThumbHeight, 0x0909);
			salvar.log = &host.log;
			cursor.setHost(&host);

			const int ret = cursor.update();

			std::string callsOut, selCur, selTxt;
			for (size_t i = 0; i < host.log.size(); ++i) callsOut += (i ? "," : "") + host.log[i];
			if (callsOut.empty()) callsOut = "-";
			auto selState = [&](Tesoro::Object &o, const char *name) -> std::string {
				const Tesoro::AnimPlayer &p = o.animation().current();
				if (p.startTime() != now) return "-";
				char b[80]; snprintf(b, sizeof b, "sel_%s_%d_%u_%d_0_0", name, o.animation().bankIndex(), (unsigned)p.first(), (int)(int16)p.last()); return b;
			};
			selCur = selState(cursor, "cur"); selTxt = selState(textos, "txt");
			char rb[40];
			snprintf(rb, sizeof rb, "%d", ret);
			std::string got = std::string(rb) + " | " + callsOut + " | " + selCur + " | " + selTxt + " | ";
			bool zero = true; for (byte b : host.slotPicture) zero = zero && b == 0;
			char tail[700];
			snprintf(tail, sizeof tail, "%d %d %d %d | %s %d %d %d %d %d | %d %d | %d %d %d %d %d %d %d | %u %d | %d %d | %u %u %d",
			         cursor._x, cursor._y, cursor._previousX, cursor._previousY,
			         cursor._companion->_level == &player ? place.c_str() : "?", cursor._companion->_room == &player.room(0) ? 1 : 0, cursor._companion->_x, cursor._companion->_y, cursor._companion->_previousX, cursor._companion->_previousY,
			         iface.camera()._x, iface.camera()._y, strip._x, square._x, members[0]._x, members[1]._x, members[2]._x, members[3]._x, members[4]._x,
			         heldItem.flags(), cursor.heldItem() ? 1 : 0, (int)(byte)cursor.label().text()[0], host.slot,
			         salvar.flags(), host.thumb.empty() ? 0u : crc32of((const byte *)&host.thumb[0], host.thumb.size() * 2), zero ? 1 : 0);
			got += tail;
			tally.check("pointer step", norm(got) == norm(results), caseNo, std::string("\n port     ") + norm(got) + "\n original " + norm(results));
		}
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-40s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
