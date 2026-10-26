#define FORBIDDEN_SYMBOL_ALLOW_ALL
// Drives the production scenes, objects and script executor with a fake game around them: a click
// arrives at once, speech lasts 200 ms, walks end at once. Reads commands from stdin.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "common/system.h"
#include "common/textconsole.h"
#include "tesoro/scriptexec.h"
#include "tesoro/interface.h"
#include "tesoro/jim.h"
#include "tesoro/objects.h"
#include "tesoro/renderer.h"

OSystem *g_system = nullptr;
using namespace Tesoro;

static const char *const kNames[] = {
	"DESPACHO.LVL", "PUERTO.LVL", "TABERNA.LVL", "CUBIERTA.LVL", "CAMAROTE.LVL", "ALMACEN.LVL",
	"COCINA.LVL", "PLAYA.LVL", "MAPA.LVL", "POBLADO.LVL", "PANTANO.LVL", "CAMPAMENTO.LVL",
	"VOLCAN.LVL", "ARMARIO.LVL", "TABLERO.LVL", "TIERRA.LVL", "RISCO.LVL", "CUBIERTA2.LVL",
	"CAMAROTE2.LVL", "ALMACEN2.LVL", "COCINA2.LVL", "LUCHA.LVL", "TRONO.LVL", "ENTRADA.LVL",
	"1.LVL", "2.LVL", "3.LVL", "4.LVL", "5.LVL", "6.LVL", "7.LVL", "8.LVL", "9.LVL", "10.LVL",
	"11.LVL", "12.LVL", "13.LVL", "14.LVL", "15.LVL", "16.LVL", "17.LVL", "18.LVL", "19.LVL",
	"20.LVL", "RISCOFINAL.LVL", "FINAL.LVL", "CREDITOS.LVL"
};

static std::string dir;
static Bytes readFile(const std::string &path) {
	FILE *f = fopen(path.c_str(), "rb");
	if (!f) { fprintf(stderr, "cannot open %s\n", path.c_str()); exit(2); }
	Bytes b; fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
	b.resize(n); if (fread(b.data(), 1, n, f) != (size_t)n) exit(2); fclose(f); return b;
}
static void formatter(char *dst, const char *src, size_t len) { printf("ERROR: %s\n", src); fflush(stdout); strncpy(dst, src, len); exit(1); }

struct Game : ScriptHost, PointerHost {
	uint32 clock = 0;
	ScriptLibrary lib;
	std::map<std::string, Common::SharedPtr<Scene>> scenes;
	Common::SharedPtr<Scene> interface;
	std::vector<Common::SharedPtr<Script>> scripts;
	Scene *jimScene = nullptr;
	uint32 speechEnd = 0;
	uint8 noVars[256] = {};
	Common::SharedPtr<JimObject> jim;

	Game() {
		const char *txt[] = {"OBJETOS.TXT", "ANIMS.TXT", "CONVERS.TXT", "ICONS.TXT"};
		const char *dlg[] = {"OBJETOS.DLG", "ANIMS.DLG", "CONVERS.DLG", "ICONS.DLG"};
		for (auto n : txt) lib.addTextTable(readFile(dir + "/" + n));
		for (auto n : dlg) lib.addScripts(n, readFile(dir + "/" + n));
		lib.link();
		interface.reset(new Scene(readScene(readFile(dir + "/INTERFACE.LVL")), 0));
		Common::Array<Common::SharedPtr<Animation>> banks;
		for (uint i = 0; i < JimObject::bankCount(); ++i)
			banks.push_back(Common::SharedPtr<Animation>(new Animation(readAnimation(readFile(dir + "/" + JimObject::bankName(i))))));
		jim.reset(new JimObject(banks));
		for (auto &o : interface->objects()) if (o->kind() == 13) static_cast<ItemObject *>(o.get())->setGame(*this);
		jim->setActive(true);
		jim->show(true);
	}
	// ScriptHost
	uint32 gameTime() const override { return clock; }
	Common::RandomSource &randomSource() override { static char s[sizeof(Common::RandomSource)] = {}; return *reinterpret_cast<Common::RandomSource *>(s); }
	Scene *scene(ScriptOp::Scope scope, int level) override {
		if (scope == ScriptOp::kInventory) return interface.get();
		if (scope == ScriptOp::kLevel) return levelScene(level);
		return jimScene;
	}
	void setInputEnabled(bool on) override { printf("  input %d\n", on); }
	void startScript(const ScriptBlock &b, uint64 label, uint8 *vars, Scene &s) override {
		printf("  run %s (%s)\n", b.name.c_str(), "label");
		Common::SharedPtr<Script> sc(new Script(*this, b, label, vars, s)); scripts.push_back(sc); sc->start();
	}
	void startSpeech(SceneObject *sp, const TextEntry &t, int) override { printf("  say %s%s\n", t.name.c_str(), sp ? " (character)" : ""); speechEnd = clock + 200; }
	bool isSpeaking(SceneObject *) const override { return clock < speechEnd; }
	int polls = 0, pollsThisFrame = 0, maxPolls = 0, menus = 0;
	int pickDialogueChoice(const Common::Array<const TextEntry *> &c) override {
		++pollsThisFrame;
		// The player takes a frame to decide; then picks the first choice.
		if ((polls++ & 1) == 0) return -1;
		// First choice at the first menu of a conversation, then always the last one (to leave).
		int pick = menus++ == 0 ? 0 : int(c.size()) - 1;
		printf("  menu with %u choices: picks %s\n", c.size(), c[pick]->name.c_str());
		return pick;
	}
	SceneObject *player() override { return jim.get(); }
	void dialogueChoiceDone() override { printf("  menu done\n"); }
	void startMovie(uint n) override { printf("  movie %u\n", n); }
	bool isMovieFinished() const override { return true; }
	void playSound(const Common::String &n) override { printf("  sound %s\n", n.c_str()); }
	void playMusic(uint t) override { printf("  music %u\n", t); }
	void doNative(uint type, uint ord, Script &) override {
		printf("  native %u %u\n", type, ord);
		if (type == 21 && ord == 0) static_cast<DoorObject *>(levelScene(0)->find("PUERTA1"))->useFromScript(*this);
	}
	void walk(SceneObject &o, int x, int y, Script &w) override { printf("  walk %s to %d %d\n", o.name().c_str(), x, y); w.actionDone(); }
	void look(SceneObject &o, ScriptOp::Direction d) override { printf("  look %s %d\n", o.name().c_str(), d); }
	// PointerHost
	void setPointerText(const Common::String &) override {}
	const TextEntry *findText(const Common::String &n) const override { return lib.findText(encodeName(n)); }
	void walkJim(int32 x, int32 y, ActionWaiter *w) override { printf("  jim walks to %d %d\n", x, y); if (w) w->actionDone(); }
	void turnJim(int d) override { printf("  jim turns %d\n", d); }
	void jimSays(const TextEntry &t) override { printf("  jim says %s\n", t.name.c_str()); speechEnd = clock + 200; }
	uint32 now() const override { return clock; }
	void setVerb(int v) override { printf("  verb %d\n", v); }
	SceneObject *held = nullptr; bool invOpen = false;
	SceneObject *heldItem() const override { return held; }
	void holdItem(SceneObject *item, int frame) override {
		if (!item) { if (held) held->show(true); if (held) printf("  puts back %s\n", held->name().c_str()); held = nullptr; printf("  cursor 0\n"); return; }
		held = item; printf("  holds %s (cursor frame %d)\n", item->name().c_str(), frame);
	}
	bool inventoryOpen() const override { return invOpen; }
	void closeInventory() override { if (invOpen) { invOpen = false; printf("  inventory closes\n"); } }
	Scene *levelScene(int level) override {
		auto &s = scenes[kNames[level]];
		if (!s) s.reset(new Scene(readScene(readFile(dir + "/" + kNames[level])), clock));
		return s.get();
	}
	Scene *interfaceScene() override { return interface.get(); }
	void startObjectScript(const char *block, const char *label, uint8 *vars, Scene &s) override {
		const ScriptBlock *b = lib.find(block);
		if (!b) { printf("ERROR: no script %s\n", block); exit(1); }
		printf("  object starts %s %s\n", block, label);
		startScript(*b, encodeName(label), vars ? vars : noVars, s);
	}
	void putJim(Scene &s, int32 x, int32 y) {
		jim->leave();
		s.addObject(jim, 0);
		jim->place(s, x, y, clock);
		jimScene = &s;
	}
	JimState state; uint randomBelow(uint n) override { return 0; } bool nativeBusy() const override { return false; } JimState &jimState() override { return state; }
	void transferJim(Scene &s, int32 x, int32 y, int d, bool fade, SceneObject *exitDoor) override {
		putJim(s, x, y);
		if (d) jim->look(ScriptOp::Direction(d), clock);
		if (fade) printf("  (fades)\n");
		if (exitDoor) exitDoor->arrivedThrough(*this);
		for (auto &kv : scenes) if (kv.second.get() == &s) printf("  jim goes to %s at %d %d facing %d\n", kv.first.c_str(), x, y, d);
	}
	int jimLevel() const override {
		for (uint i = 0; i < sizeof(kNames) / sizeof(*kNames); ++i) { auto it = scenes.find(kNames[i]); if (it != scenes.end() && it->second.get() == jimScene) return i; }
		return -1;
	}
	void run() {
		for (int guard = 0; guard < 200000; ++guard) {
			bool all = true;
			for (auto &s : scripts) all = all && s->finished();
			if (scripts.empty() && clock >= speechEnd) break;
			clock += 10;
			pollsThisFrame = 0;
			for (int i = int(scripts.size()) - 1; i >= 0; --i) {
				Common::SharedPtr<Script> sc = scripts[i];
				sc->update();
				if (!sc->finished()) break;
			}
			if (pollsThisFrame > maxPolls) maxPolls = pollsThisFrame;
			for (uint i = 0; i < scripts.size();) { if (scripts[i]->finished()) scripts.erase(scripts.begin() + i); else ++i; }
		}
		printf("  most menus polled in one frame: %d, scripts left %u\n", maxPolls, (unsigned)scripts.size());
		scripts.clear();
	}
};

static SceneObject *find(Game &g, int level, const std::string &name) {
	SceneObject *o = level < 0 ? g.interface->find(name.c_str()) : g.levelScene(level)->find(name.c_str());
	if (!o) { printf("ERROR: no object %s in %d\n", name.c_str(), level); exit(1); }
	return o;
}

int main(int argc, char **argv) {
	Common::setErrorOutputFormatter(formatter);
	dir = argv[1];
	Game g;
	std::string line;
	while (std::getline(std::cin, line)) {
		std::istringstream in(line);
		std::string cmd; in >> cmd;
		if (cmd.empty() || cmd[0] == '#') continue;
		printf("> %s\n", line.c_str());
		int level, a = 0, b = 0; std::string name;
		if (cmd == "enter") { in >> level; g.putJim(*g.levelScene(level), 550, 100); printf("  jim is in %s\n", kNames[level]); }
		else if (cmd == "click" || cmd == "rclick") {
			in >> level >> name >> a;
			SceneObject *o = find(g, level, name);
			g.menus = 0;
			g.speechEnd = 0;
			int r = cmd == "click" ? o->onLeftClick(g, a) : o->onRightClick(g, a);
			printf("  result %d\n", r);
			g.run();
		} else if (cmd == "state") {
			in >> level >> name;
			SceneObject *o = find(g, level, name);
			printf("  %s: active=%d shown=%d bank=%u vars=", name.c_str(), !!(o->flags() & SceneObject::kActive), !!(o->flags() & SceneObject::kShown), o->currentBank());
			if (o->variables()) printf("%d %d %d %d", o->variables()[0], o->variables()[1], o->variables()[2], o->variables()[3]); else printf("none");
			printf("\n");
		} else if (cmd == "open") { g.invOpen = !g.invOpen; printf("  inventory %s\n", g.invOpen ? "open" : "closed"); }
		else if (cmd == "page") {
			// Where the interface draws things with the camera on the inventory page.
			SceneBounds b; b.left = 0; b.bottom = 0; b.right = 1280; b.top = 480; g.interface->camera().setBounds(b);
			g.interface->camera().setPosition(960, 240);
			Common::Array<SceneSprite> sprites;
			g.interface->appendSprites(g.clock, g.randomSource(), sprites);
			printf("  %u sprites on the page\n", sprites.size());
			for (uint i = 0; i < sprites.size(); ++i) printf("   sprite at %d %d depth %u kind %u\n", sprites[i].x, sprites[i].y, sprites[i].depth, (unsigned)sprites[i].image->kind);
			g.interface->camera().setPosition(320, 240);
		} else if (cmd == "drop") g.holdItem(nullptr, 0);
		else if (cmd == "probe") {
			// What the pointer overlaps on the inventory page at a world point.
			in >> a >> b;
			static Common::SharedPtr<PointerProbe> probe;
			if (!probe) { probe.reset(new PointerProbe()); g.interface->addObject(probe, 0); }
			probe->clearCandidates();
			probe->placeAt(a, b);
			g.interface->objectMoved(probe.get());
			g.interface->resolveOverlaps(g.clock);
			printf("  candidates:");
			for (uint i = 0; i < probe->candidates().size(); ++i) printf(" %s(depth %u)", probe->candidates()[i]->name().c_str(), probe->candidates()[i]->depth());
			printf("\n");
		} else if (cmd == "render") {
			// The interface on the inventory page, drawn over the office as the engine does.
			std::string file; in >> file;
			SceneBounds b; b.left = 0; b.bottom = 0; b.right = 1280; b.top = 480; g.interface->camera().setBounds(b);
			g.interface->camera().setPosition(960, 240);
			Common::Array<SceneSprite> sprites;
			g.levelScene(0)->appendSprites(g.clock, g.randomSource(), sprites);
			Common::Array<SceneSprite> ui;
			g.interface->appendSprites(g.clock, g.randomSource(), ui);
			for (uint i = 0; i < ui.size(); ++i) {}
			SceneRenderer renderer; Common::Array<uint8> tbl;
			Bytes table = readFile(dir + "/TABLA1.TBL"); renderer.setShadowTable(table);
			Graphics::Surface surf; surf.create(640, 480, Graphics::PixelFormat(4, 8, 8, 8, 8, 16, 8, 0, 24));
			renderer.draw(surf, sprites);
			renderer.draw(surf, ui, false);
			FILE *o = fopen(file.c_str(), "wb"); fprintf(o, "P6 640 480 255\n");
			for (int y = 0; y < 480; ++y) for (int x = 0; x < 640; ++x) { uint32 p = *(uint32 *)surf.getBasePtr(x, y); fputc((p >> 16) & 255, o); fputc((p >> 8) & 255, o); fputc(p & 255, o); }
			fclose(o);
			g.interface->camera().setPosition(320, 240);
		} else if (cmd == "run") {
			// Starts a script block: run LEVEL BLOCK LABEL
			std::string block, label; in >> level >> block >> label;
			g.speechEnd = 0; g.putJim(*g.levelScene(level), 0, 0);
			g.startObjectScript(block.c_str(), label.c_str(), nullptr, *g.levelScene(level));
			g.run();
		} else if (cmd == "phase") { in >> a >> b; g.state.phase = a; g.state.busy = b; printf("  phase %d busy %d\n", a, b); }
		else if (cmd == "play") { in >> level >> name >> a; find(g, level, name)->play(a, g.clock, 0, -1); }
		else if (cmd == "setvar") { in >> level >> name >> a >> b; find(g, level, name)->variables()[a] = b; }
		else if (cmd == "setactive") { in >> level >> name >> a; find(g, level, name)->setActive(a); }
		else printf("  unknown command\n");
		fflush(stdout);
	}
	return 0;
}
