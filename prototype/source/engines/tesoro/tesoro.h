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

#ifndef TESORO_ENGINE_H
#define TESORO_ENGINE_H
#include "audio/mixer.h"
#include "engines/engine.h"
#include "graphics/surface.h"
#include "tesoro/script.h"
struct ADGameDescription;
namespace Tesoro {
class TesoroEngine : public Engine, public ScriptHost {
	Archive _archive;
	Catalog _catalog;
	ScriptVM _vm;
	Scene _office, _harbour;
	Scene *_scene;
	Common::HashMap<Common::String, Bank> _jimBanks;
	Graphics::Surface _screen;
	Object _jim;
	Common::String _jimBank, _subtitle, _speaker, _status, _speechId;
	Common::HashMap<Common::String, bool> _inventory;
	Audio::SoundHandle _speech;
	enum Wait { kNone,
				kSpeech,
				kSleep,
				kMove,
				kAnimation } _wait;
	uint32 _deadline, _lastTick;
	int _destX, _destY, _direction, _verb;
	Object *_animationWait;
	uint _captainSpeechSlot, _captainSpeechFrame;
	bool _captainTalking, _captainSpeechRunning, _captainSpeechLoop;
	void initialize();
	void reset();
	void animate(Object &o, const Common::Array<Bank> &banks, uint32 now);
	void updateWorld(uint32 now);
	void draw();
	int cameraX() const;
	void snapshot(const char *configKey);
	void drawImage(const Image &im, int x, int y, int scale = 100);
	void drawBank(const Bank &b, uint frame, int x, int y, int scale = 100);
	void drawObject(const Object &o);
	void face(const Common::String &direction);
	void move(int x, int y);
	void finishSpeech();
	void door(Scene &source);
	void click(int x, int y);
	bool selfTest();

public:
	TesoroEngine(OSystem *syst, const ADGameDescription *desc);
	~TesoroEngine() override;
	Common::Error run() override;
	bool request(const Request &r) override;
	bool complete() override;
};
} // namespace Tesoro
#endif
