// Compares the engine's Camera with vectors recorded from the original camera code
// (see re/tools/gen_vectors_camera.py). Build/run: re/tools/run_startup_check.sh
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include "common/scummsys.h"
#include "tesoro/core/camera.h"
#include "tesoro/core/object.h"

void NORETURN_PRE error(const char *s, ...) { fprintf(stderr, "engine error: %s\n", s); exit(3); }
void warning(const char *s, ...) { (void)s; }

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "r");
	if (!f) { fprintf(stderr, "no vectors\n"); return 2; }
	static char buf[1 << 16];
	Tesoro::Camera cam;
	Tesoro::Object target;
	int32 now = 0;
	int caseNo = 0, ok = 0, bad = 0, clampOk = 0, clampBad = 0; std::string lastOp;
	while (fgets(buf, sizeof buf, f)) {
		std::istringstream in(buf); std::string op; in >> op;
		if (op == "C") { ++caseNo; target = Tesoro::Object(); target.setPosition(0, 0); now = 0; }
		else if (op == "ct") { cam.init(); }
		else if (op == "sb") { int l, t, r, b; in >> l >> t >> r >> b; cam.setBounds(l, t, r, b); }
		else if (op == "st") { int h; in >> h; cam.setTarget(h ? &target : nullptr); }
		else if (op == "tx") { int x; in >> x; target.setPosition(x, 0); }
		else if (op == "tm") { in >> now; }
		else if (op == "sp") { int x, y, r; in >> x >> y >> r; bool got = cam.setPositionClamped(x, y); if (got == (r != 0)) ++clampOk; else { ++clampBad; ++bad; if (bad < 12) printf("mismatch case %d: sp %d %d want %d\n", caseNo, x, y, r); } }
		else if (op == "mv") { int x, y, d; in >> x >> y >> d; cam.moveTo(x, y, d, now); }
		else if (op == "ut") { int d; in >> d; cam.updateToward(d, now); }
		else if (op == "S") {
			int x, y, sx, sy; in >> x >> y >> sx >> sy;
			bool same = cam.x() == x && cam.y() == y && cam.scaledX() == sx && cam.scaledY() == sy;
			if (same) ++ok; else { ++bad; if (bad < 12) printf("mismatch case %d after %s: want %d %d %d %d got %d %d %d %d\n", caseNo, lastOp.c_str(), x, y, sx, sy, cam.x(), cam.y(), cam.scaledX(), cam.scaledY()); }
		}
		if (op != "S") lastOp = buf, lastOp.pop_back();
	}
	printf("camera state %d/%d match, clamp results %d/%d\n", ok, ok + bad - clampBad, clampOk, clampOk + clampBad);
	return bad ? 1 : 0;
}
