// Compares the engine's Keyboard and Mouse with vectors recorded from the original constructors and reads (see re/tools/gen_vectors_devices.py).
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
#include "tesoro/core/keyboard.h"
#include "tesoro/core/mouse.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *, ...) {}

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[4096];
	int checks = 0, bad = 0, shown = 0;
	auto check = [&](bool ok, const char *what, const std::string &detail) { ++checks; if (!ok && ++bad && shown++ < 8) printf("mismatch %s %s\n", what, detail.c_str()); };
	{	// the host keys the engine watches, in the order of the original's scan codes (Escape, Return, Space, S, L)
		const Common::KeyCode host[5] = {Common::KEYCODE_ESCAPE, Common::KEYCODE_RETURN, Common::KEYCODE_SPACE, Common::KEYCODE_s, Common::KEYCODE_l};
		for (int k = 0; k < 5; ++k) { Tesoro::Keyboard::Key key; check(Tesoro::Keyboard::fromHost(host[k], key) && (int)key == k, "host key order", std::to_string(k)); }
		Tesoro::Keyboard::Key other; check(!Tesoro::Keyboard::fromHost(Common::KEYCODE_a, other), "other keys ignored", "a");
	}
	Tesoro::Keyboard *kb = new Tesoro::Keyboard();
	Tesoro::Mouse *mouse = new Tesoro::Mouse();
	static const int scan[5] = {1, 28, 57, 31, 38};
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "KC") {
			int rel, rep, cap, codes[5]; in >> rel >> rep >> cap; for (int &c : codes) in >> c;
			bool same = true; for (int i = 0; i < 5; ++i) same = same && codes[i] == scan[i];
			check(same, "keyboard key order", buf);
		} else if (op == "MC") {
			int n, rel, rep, minx, miny, maxx, maxy, x, y; in >> n >> rel >> rep >> minx >> miny >> maxx >> maxy >> x >> y;
			check(mouse->x() == x && mouse->y() == y, "mouse centre", buf);
			check(minx == 0 && miny == 0 && maxx == Tesoro::Mouse::kMaxX && maxy == Tesoro::Mouse::kMaxY, "mouse range", buf);
		} else if (op == "KS") { delete kb; kb = new Tesoro::Keyboard(); }
		else if (op == "MS") { delete mouse; mouse = new Tesoro::Mouse(); }
		else if (op == "KU") {
			unsigned tick; int sc[5], st[5], ret; std::string bar; in >> tick; for (int &c : sc) in >> c; in >> bar; for (int &c : st) in >> c; in >> ret;
			for (int k = 0; k < 5; ++k) kb->setDown((Tesoro::Keyboard::Key)k, (sc[k] & 0x80) != 0);
			kb->update(tick);
			std::string got, want;
			for (int k = 0; k < 5; ++k) { got += std::to_string(kb->state((Tesoro::Keyboard::Key)k)) + " "; want += std::to_string(st[k]) + " "; }
			check(got == want, "keyboard states", got + "/ " + want);
			check(ret == 1, "keyboard read returns 1", buf);
		} else if (op == "MU") {
			unsigned tick; int dx, dy, dz, b[4], x, y, st[3], ret; std::string bar; in >> tick >> dx >> dy >> dz; for (int &c : b) in >> c; in >> bar >> x >> y; for (int &c : st) in >> c; in >> ret;
			mouse->moveTo(mouse->x() + dx, mouse->y() + dy);
			bool down[3]; for (int k = 0; k < 3; ++k) down[k] = (b[k] & 0x80) != 0;
			mouse->update(down, tick);
			check(mouse->x() == x && mouse->y() == y, "mouse position", std::to_string(mouse->x()) + "," + std::to_string(mouse->y()) + " want " + std::to_string(x) + "," + std::to_string(y));
			std::string got, want;
			for (int k = 0; k < 3; ++k) { got += std::to_string(mouse->buttonState((Tesoro::Mouse::Button)k)) + " "; want += std::to_string(st[k]) + " "; }
			check(got == want, "mouse button states", got + "/ " + want);
		}
	}
	printf("keyboard and mouse           %d checks, %d mismatches\n", checks, bad);
	return bad != 0;
}
