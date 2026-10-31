// Compares the engine's item group (object kind 2) with vectors recorded from the original code
// (see re/tools/gen_vectors_itemgroup.py). Build/run: re/tools/run_startup_check.sh
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
#define private public
#define protected public
#include "tesoro/core/itemgroup.h"
#include "tesoro/core/level.h"
#undef private
#undef protected

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
static int g_warnings = 0;
void warning(const char *s, ...) { (void)s; ++g_warnings; }

static std::string unhex(const std::string &s) {
	std::string r;
	if (s == "-") return r;
	for (size_t i = 0; i + 1 < s.size(); i += 2) r += (char)strtol(s.substr(i, 2).c_str(), nullptr, 16);
	return r;
}
static std::vector<std::string> split(const std::string &s, char c) {
	std::vector<std::string> v; std::string cur;
	if (s == "-") return v;
	for (char ch : s) { if (ch == c) { v.push_back(cur); cur.clear(); } else cur += ch; }
	v.push_back(cur); return v;
}

struct Tally {
	std::map<std::string, std::pair<int, int>> t; int shown = 0;
	void check(const char *what, bool ok, int caseNo, const std::string &detail = "") {
		auto &c = t[what]; ++c.first;
		if (!ok) { ++c.second; if (shown++ < 12) printf("mismatch %s in case %d %s\n", what, caseNo, detail.c_str()); }
	}
};

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 20];
	int caseNo = 0; Tally tally;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op != "K") continue;
		++caseNo;
		unsigned option, count; std::string names, dataHex, bar1, keys, bar2, members;
		in >> option >> count >> names >> dataHex >> bar1 >> keys >> bar2 >> members;

		Tesoro::Level level;
		Common::Array<Tesoro::AnimPlayer> noBanks;
		std::vector<Tesoro::Object *> objects;
		for (const std::string &n : split(names, ',')) {
			Tesoro::ObjectRecord r; std::string name = unhex(n);
			strncpy(r.name, name.c_str(), 12); r.name[12] = 0;
			Tesoro::Object *o = new Tesoro::Object(); o->initAnimated(r, 0, noBanks, 0);
			level.objects().pushBack(o); objects.push_back(o);
		}
		std::string data = unhex(dataHex);
		Tesoro::ObjectRecord rec; strcpy(rec.name, "GRP"); rec.kind = 2;
		rec.extra.resize(data.size()); memcpy(&rec.extra[0], data.data(), data.size());
		Tesoro::ItemGroup group; group.init(rec, 0, noBanks);
		group._level = &level; group.onAttach();

		tally.check("option byte", group._option == option, caseNo);
		tally.check("member count", group._keys.size() == count, caseNo);
		std::vector<std::string> ek = split(keys, ',');
		bool keysOk = ek.size() == group._keys.size();
		for (size_t i = 0; keysOk && i < ek.size(); ++i) {
			unsigned long lo, hi; sscanf(ek[i].c_str(), "%lu:%lu", &lo, &hi);
			keysOk = (uint32)group._keys[i] == lo && (uint32)(group._keys[i] >> 32) == hi;
		}
		tally.check("name keys", keysOk, caseNo, keys);
		std::vector<std::string> em = split(members, ',');
		bool memOk = em.size() == group.members().size();
		for (size_t i = 0; memOk && i < em.size(); ++i) {
			unsigned want = (unsigned)atoi(em[i].c_str()), got = 0;
			for (size_t k = 0; k < objects.size(); ++k) if (group.members()[i] == objects[k]) got = (unsigned)k + 1;
			memOk = want == got;
		}
		tally.check("members", memOk, caseNo, members);
		for (Tesoro::Object *o : objects) { (void)o; }
	}
	int bad = 0;
	for (auto &e : tally.t) { printf("%-24s %d checks, %d mismatches\n", e.first.c_str(), e.second.first, e.second.second); bad += e.second.second; }
	printf("warnings from the port: %d\n", g_warnings);
	return bad != 0;
}
