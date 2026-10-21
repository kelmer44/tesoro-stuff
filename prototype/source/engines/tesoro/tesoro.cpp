/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "tesoro/tesoro.h"
#include "audio/audiostream.h"
#include "audio/decoders/wave.h"
#include "common/config-manager.h"
#include "common/events.h"
#include "common/str-enc.h"
#include "common/system.h"
#include "common/textconsole.h"
#include "engines/util.h"
#include "graphics/font.h"
#include "graphics/fontman.h"
#include "image/bmp.h"
namespace Tesoro {
TesoroEngine::TesoroEngine(OSystem *syst, const ADGameDescription *) : Engine(syst), _vm(_catalog, *this), _scene(nullptr), _wait(kNone), _deadline(0), _lastTick(0), _destX(0), _destY(0), _direction(0), _verb(0), _animationWait(nullptr), _captainSpeechSlot(0), _captainSpeechFrame(0), _captainTalking(false), _captainSpeechRunning(false), _captainSpeechLoop(false) {}
TesoroEngine::~TesoroEngine() {
	_mixer->stopHandle(_speech);
	_screen.free();
}
void TesoroEngine::initialize() {
	_archive.open();
	_catalog.load(_archive);
	_office = readScene(_archive.read("DESPACHO.LVL"));
	_harbour = readScene(_archive.read("PUERTO.LVL"));
	const char *names[] = {"JIMQUIETO", "JIMABAJO", "JIMARRIBA", "JIMDERECHA", "JIMIZQUIERDA", "JIMHABAB", "JIMHABAR", "JIMHABDER", "JIMHABIZ"};
	for (uint i = 0; i < ARRAYSIZE(names); ++i)
		_jimBanks[names[i]] = readAnimation(_archive.read(Common::String(names[i]) + ".ANM"));
	reset();
}
void TesoroEngine::reset() {
	_vm.stop();
	finishSpeech();
	_inventory.clear();
	_wait = kNone;
	Scene *rooms[] = {&_office, &_harbour};
	for (uint r = 0; r < 2; ++r)
		for (uint i = 0; i < rooms[r]->objects.size(); ++i) {
			Object &o = rooms[r]->objects[i];
			memset(o.vars, 0, sizeof(o.vars));
			o.active = o.shown = true;
			o.running = o.loop = false;
			o.slot = o.frame = 0;
			// Static/animated scenery constructor 0x40e5c0: first byte is
			// initial frame; second byte selects hidden/hold/once/loop.
			if (o.kind == 3 && o.parameters.size() >= 2) {
				o.frame = o.parameters[0];
				o.shown = o.parameters[1] != 0;
				o.running = o.parameters[1] >= 2;
				o.loop = o.parameters[1] == 3;
				o.tick = _system->getMillis();
			}
		}
	_office.find("CAPITAN")->active = false;
	_office.find("JIMNOTA")->shown = false;
	// FANPRESENT explicitly disables this harbour overlay before normal play.
	_harbour.find("FONDONEGRO")->active = false;
	_scene = &_office;
	_jim = Object();
	_jim.name = "JIM";
	_jim.priority = 40;
	_jim.x = 550;
	_jim.y = 100;
	_jimBank = "JIMQUIETO";
	_direction = 0;
	_status = "Coge el boli de la mesa";
	_verb = 0;
	_lastTick = _system->getMillis();
}
void TesoroEngine::face(const Common::String &d) {
	// Direction indices follow the original idle frame order. Walking paths and
	// actor depth are provisional until the native floor/path code is recovered.
	if (d == "ARRIBA")
		_direction = 0;
	else if (d == "ABAJO")
		_direction = 1;
	else if (d == "IZQUIERDA")
		_direction = 2;
	else if (d == "DERECHA")
		_direction = 3;
	else
		error("Tesoro: unsupported direction");
	_jimBank = "JIMQUIETO";
	_jim.frame = _direction;
	_jim.running = false;
}
void TesoroEngine::move(int x, int y) {
	_destX = x;
	_destY = y;
	if (ABS(x - _jim.x) >= ABS(y - _jim.y))
		face(x >= _jim.x ? "DERECHA" : "IZQUIERDA");
	else
		face(y >= _jim.y ? "ARRIBA" : "ABAJO");
	const char *walk[] = {"JIMARRIBA", "JIMABAJO", "JIMIZQUIERDA", "JIMDERECHA"};
	_jimBank = walk[_direction];
	_jim.frame = 0;
	_jim.tick = _system->getMillis();
	_jim.running = _jim.loop = true;
	_wait = kMove;
}
void TesoroEngine::finishSpeech() {
	_mixer->stopHandle(_speech);
	_subtitle.clear();
	_speaker.clear();
	if (_captainTalking && _scene) {
		Object *o = _scene->find("CAPITAN");
		if (o) {
			o->slot = _captainSpeechSlot;
			o->frame = _captainSpeechFrame;
			o->running = _captainSpeechRunning;
			o->loop = _captainSpeechLoop;
			o->tick = _system->getMillis();
		}
	}
	_captainTalking = false;
	if (_jimBank.hasPrefix("JIMHAB")) {
		_jimBank = "JIMQUIETO";
		_jim.frame = _direction;
		_jim.running = false;
	}
}
bool TesoroEngine::request(const Request &r) {
	uint32 now = _system->getMillis();
	if (r.kind == Request::kSpeech) {
		finishSpeech();
		_subtitle = r.text;
		_speaker = r.target;
		_speechId = r.textId;
		Audio::AudioStream *audio = Audio::makeWAVStream(_archive.stream(r.textId + ".WAV"), DisposeAfterUse::YES);
		if (!audio)
			error("Tesoro: invalid speech WAV %s", r.textId.c_str());
		_mixer->playStream(Audio::Mixer::kSpeechSoundType, &_speech, audio);
		if (r.target == "JIM") {
			const char *talk[] = {"JIMHABAR", "JIMHABAB", "JIMHABIZ", "JIMHABDER"};
			_jimBank = talk[_direction];
			_jim.frame = 0;
			_jim.tick = now;
			_jim.running = _jim.loop = true;
		} else if (r.target == "CAPITAN" && _scene == &_office) {
			Object *o = _scene->find("CAPITAN");
			_captainSpeechSlot = o->slot;
			_captainSpeechFrame = o->frame;
			_captainSpeechRunning = o->running;
			_captainSpeechLoop = o->loop;
			_captainTalking = true;
			o->slot = 3;
			o->frame = 0;
			o->tick = now;
			o->running = o->loop = true;
		}
		_wait = kSpeech;
		return true;
	}
	if (r.kind == Request::kSleep) {
		_deadline = now + r.number;
		_wait = kSleep;
		return true;
	}
	if (r.kind == Request::kNative) {
		if (r.nativeType == 0 || r.nativeType == 11 || r.nativeType == 13)
			return false;
		if (r.nativeType == 21 && r.number == 0) {
			door(_office);
			return false;
		}
		error("Tesoro: unimplemented native callback type=%u ordinal=%u", r.nativeType, r.number);
	}
	const Common::Array<Common::String> &w = r.words;
	if (r.scope == "INVENTARIO" && w.size() == 2 && w[0] == "activate" && (w[1] == "on" || w[1] == "off")) {
		_inventory[r.target] = w[1] == "on";
		return false;
	}
	if (!r.scope.empty())
		error("Tesoro: unsupported message scope %s", r.scope.c_str());
	if (r.target == "JIM") {
		if (w.size() == 2 && w[0] == "mirar") {
			face(w[1]);
			return false;
		}
		if (w.size() == 3 && w[0] == "ira") {
			move(atoi(w[1].c_str()), atoi(w[2].c_str()));
			return true;
		}
	}
	Object *o = _scene->find(r.target);
	if (!o)
		error("Tesoro: missing message target %s", r.target.c_str());
	if (w.size() == 2 && (w[0] == "activate" || w[0] == "show") && (w[1] == "on" || w[1] == "off")) {
		if (w[0] == "activate")
			o->active = w[1] == "on";
		else
			o->shown = w[1] == "on";
		return false;
	}
	if (w.size() == 4 && w[0] == "play") {
		uint slot = atoi(w[1].c_str()), frame = atoi(w[2].c_str());
		if (slot >= o->refs.size() || frame >= _scene->banks[o->refs[slot]].frames.size())
			error("Tesoro: animation reference outside bank");
		if (w[3] != "play" && w[3] != "stop" && w[3] != "loop" && w[3] != "wait")
			error("Tesoro: unknown play mode");
		o->slot = slot;
		o->frame = frame;
		o->tick = now;
		o->running = w[3] != "stop";
		o->loop = w[3] == "loop";
		if (w[3] == "wait") {
			_animationWait = o;
			_wait = kAnimation;
			return true;
		}
		return false;
	}
	error("Tesoro: unsupported object message to %s", r.target.c_str());
}
bool TesoroEngine::complete() {
	switch (_wait) {
	case kSpeech:
		if (_mixer->isSoundHandleActive(_speech))
			return false;
		finishSpeech();
		break;
	case kSleep:
		if (int32(_system->getMillis() - _deadline) < 0)
			return false;
		break;
	case kMove:
		if (_jim.x != _destX || _jim.y != _destY)
			return false;
		break;
	case kAnimation:
		if (_animationWait && _animationWait->running)
			return false;
		break;
	default:
		break;
	}
	_wait = kNone;
	return true;
}
void TesoroEngine::door(Scene &source) {
	Object *from = source.find("PUERTA1");
	if (!from || from->parameters.size() < 117)
		error("Tesoro: incomplete door parameters");
	if (from->vars[0]) {
		_status = "La puerta esta cerrada";
		return;
	}
	uint dest = from->parameters[103];
	Common::String name((const char *)from->parameters.data() + 104, 12);
	name = Common::String(name.c_str());
	Scene *to = dest == 0 ? &_office : dest == 1 ? &_harbour
												 : nullptr;
	if (!to)
		error("Tesoro: prototype cannot enter room %u", dest);
	Object *entry = to->find(name);
	if (!entry)
		error("Tesoro: destination door not found");
	_scene = to;
	_jim.x = entry->x;
	_jim.y = entry->y;
	_jim.running = false;
	_jim.loop = false;
	face(from->parameters[116] == 1 ? "DERECHA" : "IZQUIERDA");
	_status = to == &_harbour ? "Puerto: pulsa en la puerta izquierda para volver" : "Despacho";
}
void TesoroEngine::animate(Object &o, const Common::Array<Bank> &banks, uint32 now) {
	if (!o.running || o.refs.empty() || o.slot >= o.refs.size())
		return;
	const Bank &b = banks[o.refs[o.slot]];
	for (uint safety = 0; safety < 1000 && o.running; ++safety) {
		uint duration = MAX<uint>(1, b.frames[o.frame].duration);
		if (now - o.tick < duration)
			break;
		o.tick += duration;
		if (o.frame + 1 < b.frames.size())
			++o.frame;
		else if (o.loop)
			o.frame = 0;
		else
			o.running = false;
	}
}
void TesoroEngine::updateWorld(uint32 now) {
	uint elapsed = MIN<uint>(now - _lastTick, 100);
	_lastTick = now;
	if (_wait == kMove) {
		int dx = _destX - _jim.x, dy = _destY - _jim.y;
		int distance = MAX(ABS(dx), ABS(dy));
		int step = MAX<int>(1, elapsed * (_scene == &_office ? 110 : 40) / 1000);
		if (distance <= step) {
			_jim.x = _destX;
			_jim.y = _destY;
			_jim.running = false;
			_jimBank = "JIMQUIETO";
			_jim.frame = _direction;
		} else {
			_jim.x += dx * step / distance;
			_jim.y += dy * step / distance;
		}
	}
	for (uint i = 0; i < _scene->objects.size(); ++i)
		animate(_scene->objects[i], _scene->banks, now);
	if (_jim.running) {
		const Bank &b = _jimBanks[_jimBank];
		uint duration = MAX<uint>(1, b.frames[_jim.frame].duration);
		if (now - _jim.tick >= duration) {
			_jim.tick = now;
			_jim.frame = (_jim.frame + 1) % b.frames.size();
		}
	}
	if (_vm.busy()) {
		_vm.update();
		if (!_vm.failure.empty())
			_status = _vm.failure;
	} else if (_wait != kNone)
		complete();
}
void TesoroEngine::drawImage(const Image &im, int x, int y, int scale) {
	int width = MAX<int>(1, im.w * scale / 100), height = MAX<int>(1, im.h * scale / 100);
	for (int sy = MAX(0, -y); sy < height && sy + y < _screen.h; ++sy)
		for (int sx = MAX(0, -x); sx < width && sx + x < _screen.w; ++sx) {
			uint32 c = im.pixels[(sy * im.h / height) * im.w + sx * im.w / width];
			if (!c)
				continue;
			uint32 *dst = (uint32 *)_screen.getBasePtr(sx + x, sy + y);
			if (im.kind == 2) {
				uint k = c & 255, d = *dst;
				*dst = 0xff000000 | ((((d >> 16) & 255) * k / 255) << 16) | ((((d >> 8) & 255) * k / 255) << 8) | ((d & 255) * k / 255);
			} else
				*dst = c;
		}
}
void TesoroEngine::drawBank(const Bank &b, uint frame, int x, int y, int scale) {
	if (frame >= b.frames.size())
		return;
	x -= cameraX();
	const Frame &f = b.frames[frame];
	if (f.image < 0)
		return;
	const Image &im = *b.images[f.image];
	// Some talk frames contain only a head patch. Keep the first full body below
	// these patches; ordinary full-frame animation always redraws from the scene.
	if (frame && b.frames[0].image >= 0) {
		const Frame &base = b.frames[0];
		const Image &body = *b.images[base.image];
		if (im.w < body.w && im.h < body.h)
			drawImage(body, x + base.x * scale / 100, 480 - y - base.y * scale / 100, scale);
	}
	drawImage(im, x + f.x * scale / 100, 480 - y - f.y * scale / 100, scale);
}
int TesoroEngine::cameraX() const {
	// Both backgrounds are slightly wider than the viewport. Clamp scrolling to
	// their measured width; the original general camera controller is not ported.
	return CLIP<int>(_jim.x - 320, 0, _scene == &_office ? 42 : 45);
}
void TesoroEngine::drawObject(const Object &o) {
	if (o.active && o.shown && !o.refs.empty() && o.slot < o.refs.size())
		drawBank(_scene->banks[o.refs[o.slot]], o.frame, o.x, o.y);
}
void TesoroEngine::draw() {
	_screen.fillRect(Common::Rect(640, 480), 0xff000000);
	// Descending layer priorities reproduce the office mask/actor layering. Jim's
	// dynamic floor-dependent priority still needs the original floor algorithm.
	Common::Array<const Object *> order;
	for (uint i = 0; i < _scene->objects.size(); ++i)
		order.push_back(&_scene->objects[i]);
	order.push_back(&_jim);
	for (uint i = 1; i < order.size(); ++i)
		for (uint j = i; j > 0 && order[j]->priority > order[j - 1]->priority; --j)
			SWAP(order[j], order[j - 1]);
	for (uint i = 0; i < order.size(); ++i)
		if (order[i] == &_jim)
			// Harbour scale is a temporary estimate. Recover floor interpolation
			// before using this renderer for general gameplay.
			drawBank(_jimBanks[_jimBank], _jim.frame, _jim.x, _jim.y, _scene == &_office ? 100 : 25);
		else
			drawObject(*order[i]);
	const Graphics::Font *font = FontMan.getFontByUsage(Graphics::FontManager::kGUIFont);
	if (!_subtitle.empty()) {
		Common::Array<Common::U32String> lines;
		font->wordWrapText(_subtitle.decode(Common::kDos850), 600, lines);
		int y = 440 - lines.size() * (font->getFontHeight() + 2);
		_screen.fillRect(Common::Rect(10, y - 5, 630, 445), 0xff101018);
		for (uint i = 0; i < lines.size(); ++i) {
			font->drawString(&_screen, lines[i], 20, y, 600, _speaker == "CAPITAN" ? 0xffffdd80 : 0xffffffff, Graphics::kTextAlignCenter);
			y += font->getFontHeight() + 2;
		}
	}
	_screen.fillRect(Common::Rect(0, 450, 640, 480), 0xff202838);
	const char *verbs[] = {"Coger", "Mirar", "Andar"};
	for (uint i = 0; i < 3; ++i)
		font->drawString(&_screen, verbs[i], i * 80 + 5, 459, 75, _verb == (int)i ? 0xffffff40 : 0xffcccccc, Graphics::kTextAlignCenter);
	Common::String inventory = "";
	if (_inventory.contains("IBOLI") && _inventory["IBOLI"])
		inventory += " Boli";
	if (_inventory.contains("ICUERDA") && _inventory["ICUERDA"])
		inventory += " Cuerda";
	font->drawString(&_screen, inventory, 245, 459, 140, 0xff88ddff);
	font->drawString(&_screen, _vm.busy() ? "Espacio: saltar voz" : _status, 390, 459, 245, 0xffcccccc, Graphics::kTextAlignRight);
	_system->copyRectToScreen(_screen.getPixels(), _screen.pitch, 0, 0, 640, 480);
	_system->updateScreen();
}
void TesoroEngine::click(int x, int y) {
	if (_vm.busy() || _wait != kNone)
		return;
	if (y >= 450) {
		if (x < 240)
			_verb = x / 80;
		return;
	}
	if (_scene == &_office) {
		Object *pen = _office.find("BOLI");
		const Bank &bank = _scene->banks[pen->refs[pen->slot]];
		const Frame &frame = bank.frames[pen->frame];
		const Image &im = *bank.images[frame.image];
		int left = pen->x + frame.x - cameraX(), top = 480 - pen->y - frame.y;
		if (pen->active && Common::Rect(left - 4, top - 4, left + im.w + 4, top + im.h + 4).contains(x, y) && _verb != 2) {
			_vm.start("FBOLI", _verb == 1 ? "mirar" : "coger", _office.find("CAPITAN")->vars);
			return;
		}
		if (x > 580 && y > 170 && y < 405) {
			door(_office);
			return;
		}
	} else if (x < 85 && y > 140 && y < 425) {
		door(_harbour);
		return;
	}
	if (_verb == 2)
		move(CLIP(x + cameraX(), 60, 620), CLIP(480 - y, 70, 145));
}
void TesoroEngine::snapshot(const char *configKey) {
	if (!ConfMan.hasKey(configKey))
		return;
	Common::DumpFile out;
	if (!out.open(ConfMan.getPath(configKey)) || !::Image::writeBMP(out, _screen))
		warning("Tesoro: cannot write %s", configKey);
}
// The same VM is exercised with a controlled host. Each asynchronous request is
// deliberately held for an update to catch accidental instruction advancement.
class TestHost : public ScriptHost {
public:
	bool released;
	uint speech, sleep, messages, native, requests;
	TestHost() : released(false), speech(0), sleep(0), messages(0), native(0), requests(0) {}
	bool request(const Request &r) override {
		++requests;
		released = false;
		if (r.kind == Request::kSpeech)
			++speech;
		if (r.kind == Request::kSleep)
			++sleep;
		if (r.kind == Request::kMessage)
			++messages;
		if (r.kind == Request::kNative)
			++native;
		return true;
	}
	bool complete() override { return released; }
};
bool TesoroEngine::selfTest() {
	if (_archive.count() != 2416 || _catalog.blocks.size() != 145)
		return false;
	Common::Array<Common::String> names = _archive.names();
	uint scenes = 0, objects = 0, images = 0, banks = 0;
	for (uint i = 0; i < names.size(); ++i) {
		Common::Array<Bank> bs;
		if (names[i].hasSuffix(".LVL")) {
			Scene sc = readScene(_archive.read(names[i]));
			++scenes;
			objects += sc.objects.size();
			bs = sc.banks;
		} else if (names[i].hasSuffix(".ANM"))
			bs.push_back(readAnimation(_archive.read(names[i])));
		for (uint b = 0; b < bs.size(); ++b) {
			++banks;
			images += bs[b].images.size();
		}
	}
	if (scenes != 50 || objects != 747 || images != 2012 || banks != 371)
		return false;
	for (uint variant = 0; variant < 2; ++variant) {
		byte vars[32] = {};
		vars[0] = variant ? 2 : 0;
		TestHost host;
		ScriptVM vm(_catalog, host);
		vm.start("FBOLI", "coger", vars);
		for (uint safety = 0; vm.busy() && safety < 1000; ++safety) {
			vm.update();
			uint pc = vm.pc(), count = host.requests;
			vm.update();
			if (vm.busy() && (pc != vm.pc() || count != host.requests))
				return false;
			host.released = true;
		}
		if (vm.busy() || !vm.failure.empty() || vars[0] != (variant ? 3 : 1) || host.speech != (variant ? 2U : 25U) || host.sleep != (variant ? 1U : 4U) || host.native != 1 || host.messages != (variant ? 11U : 14U)) {
			warning("Tesoro test variant %u: v0=%u speech=%u sleep=%u message=%u native=%u failure=%s", variant, vars[0], host.speech, host.sleep, host.messages, host.native, vm.failure.c_str());
			return false;
		}
	}
	{
		byte vars[32] = {};
		TestHost host;
		ScriptVM vm(_catalog, host);
		vm.start("CONV1", "main", vars);
		for (uint safety = 0; vm.busy() && safety < 1000; ++safety) {
			vm.update();
			host.released = true;
		}
		if (vm.busy() || vm.failure.empty())
			return false; // An unsupported conversation must not be silently skipped.
	}
	// Regression: after talking, restore the captain's LAST arrival frame.
	// Restoring frame zero would make him disappear into the other room again.
	Object *captain = _office.find("CAPITAN");
	captain->slot = 1;
	captain->frame = _office.banks[captain->refs[1]].frames.size() - 1;
	uint arrivedFrame = captain->frame;
	Request speech(Request::kSpeech);
	speech.target = "CAPITAN";
	speech.textId = "OBJ001T23";
	speech.text = _catalog.texts[speech.textId];
	request(speech);
	finishSpeech();
	if (captain->slot != 1 || captain->frame != arrivedFrame || captain->running)
		return false;
	Request play(Request::kMessage);
	play.target = "CAPITAN";
	play.words = tokenize("play 2 0 wait");
	if (!request(play) || complete())
		return false;
	uint32 end = captain->tick;
	const Bank &exit = _office.banks[captain->refs[2]];
	for (uint i = 0; i < exit.frames.size(); ++i)
		end += MAX<uint>(1, exit.frames[i].duration);
	animate(*captain, _office.banks, end);
	if (!complete())
		return false;
	// Verify native destination parameters and actual host state separately.
	Request inv(Request::kMessage);
	inv.target = "IBOLI";
	inv.scope = "INVENTARIO";
	inv.words.push_back("activate");
	inv.words.push_back("on");
	request(inv);
	if (!_inventory["IBOLI"])
		return false;
	door(_office);
	if (_scene != &_harbour || _jim.x != 20 || _jim.y != 90)
		return false;
	door(_harbour);
	if (_scene != &_office || _jim.x != 561 || _jim.y != 95)
		return false;
	warning("TESORO_SELFTEST PASS: 2416 resources, 2012 images, 50 scenes, 747 objects; both pen branches block correctly; native doors round-trip");
	reset();
	return true;
}
Common::Error TesoroEngine::run() {
	initialize();
	if (ConfMan.hasKey("tesoro_selftest") && ConfMan.getBool("tesoro_selftest")) {
		bool ok = selfTest();
		quitGame();
		if (!ok)
			return Common::Error(Common::kUnknownError, "Tesoro self-test failed");
		return Common::kNoError;
	}
	Graphics::PixelFormat format(4, 8, 8, 8, 8, 16, 8, 0, 24);
	initGraphics(640, 480, &format);
	_screen.create(640, 480, format);
	byte cursor[] = {0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0};
	byte cursorColors[] = {0, 0, 0, 255, 255, 255};
	_system->setCursorPalette(cursorColors, 0, 2);
	_system->setMouseCursor(cursor, 5, 5, 2, 2, 0);
	_system->showMouse(true);
	bool autoplay = ConfMan.hasKey("tesoro_autoplay") && ConfMan.getBool("tesoro_autoplay");
	if (autoplay)
		_vm.start("FBOLI", "coger", _office.find("CAPITAN")->vars);
	uint frames = 0, captured = 0;
	while (!shouldQuit()) {
		Common::Event ev;
		while (_eventMan->pollEvent(ev)) {
			if (ev.type == Common::EVENT_LBUTTONDOWN)
				click(ev.mouse.x, ev.mouse.y);
			if (ev.type == Common::EVENT_KEYDOWN) {
				if (ev.kbd.keycode == Common::KEYCODE_ESCAPE)
					quitGame();
				if (ev.kbd.keycode == Common::KEYCODE_r)
					reset();
				if (ev.kbd.keycode == Common::KEYCODE_SPACE && _wait == kSpeech)
					finishSpeech();
			}
		}
		updateWorld(_system->getMillis());
		draw();
		++frames;
		if (ConfMan.hasKey("tesoro_snapshot") && frames == 2) {
			snapshot("tesoro_snapshot");
			if (!autoplay)
				quitGame();
		}
		if (_speechId == "OBJ001T09" && !(captured & 1)) {
			snapshot("tesoro_snapshot_note");
			captured |= 1;
		}
		if (_speechId == "OBJ001T23" && !(captured & 2)) {
			snapshot("tesoro_snapshot_captain");
			captured |= 2;
		}
		if (autoplay && !_vm.busy()) {
			snapshot("tesoro_snapshot_harbour");
			bool passed = _vm.failure.empty() && _scene == &_harbour && _office.find("CAPITAN")->vars[0] == 1 && _inventory["IBOLI"] && !_office.find("BOLI")->active && !_office.find("JIMNOTA")->shown;
			warning("TESORO_AUTOPLAY %s room=%s v0=%u inventory=%u", passed ? "PASS" : "FAIL", _scene == &_harbour ? "HARBOUR" : "OFFICE", _office.find("CAPITAN")->vars[0], _inventory["IBOLI"]);
			quitGame();
			if (!passed)
				return Common::Error(Common::kUnknownError, "Tesoro autoplay failed");
		}
		_system->delayMillis(16);
	}
	return Common::kNoError;
}
} // namespace Tesoro
