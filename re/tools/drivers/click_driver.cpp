// Compares the engine's MenuItem click with vectors recorded from the original code (see re/tools/gen_vectors_click.py).
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include "common/scummsys.h"
#define private public
#include "tesoro/core/menuitem.h"
#undef private

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

struct Host : Tesoro::MenuHost {
	std::vector<std::string> events;
	void fadeOut(int steps, int delay) override { events.push_back("fade " + std::to_string(steps) + " " + std::to_string(delay)); }
	void enterLevel(const char *level) override { events.push_back(std::string("enter ") + level); }
	void startScript(const char *block, const char *label) override { events.push_back(std::string("script ") + block + " " + label); }
	void notPorted(const char *what) override { events.push_back(strstr(what, "thumbnail") ? "unported thumbnail" : strstr(what, "start") ? "unported start" : std::string("unported ") + what); }
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) return 2;
	static char buf[4096]; int total = 0, bad = 0;
	while (fgets(buf, sizeof buf, f)) {
		std::string line(buf); while (!line.empty() && line.back() == '\n') line.pop_back();
		const size_t bar = line.find(" | "); std::istringstream head(line.substr(0, bar)); unsigned long action; int slot; std::string eq; unsigned want; head >> action >> slot >> eq >> want;
		// the recorded events: drop the script start ("run") and the coordinates / level kind the engine's host does not take
		std::vector<std::string> expected; std::string rest = line.substr(bar + 3), ev;
		std::istringstream es(rest);
		while (std::getline(es, ev, ';')) {
			while (!ev.empty() && ev[0] == ' ') ev.erase(0, 1); while (!ev.empty() && ev.back() == ' ') ev.pop_back();
			if (ev == "-" || ev == "run" || ev.empty()) continue;
			if (ev.compare(0, 5, "enter") == 0) { std::istringstream t(ev); std::string a, b; t >> a >> b; ev = a + " " + b; }
			if (ev.compare(0, 6, "script") == 0) { std::istringstream t(ev); std::string a, b, c; t >> a >> b >> c; ev = a + " " + b + " " + c; }
			expected.push_back(ev);
		}
		Host host; Tesoro::MenuItem item; item._action = (uint32)action; item.setHost(&host);
		const int got = slot == 14 ? item.setVerbAndClick(0) : item.rightClick(0);
		++total;
		if (got != (int)want || host.events != expected) { ++bad; if (bad < 5) printf("mismatch action %lu slot %d: want %u got %d\n", action, slot, want, got); }
	}
	printf("%-28s %d/%d match\n", "menu item click", total - bad, total);
	return bad ? 1 : 0;
}
