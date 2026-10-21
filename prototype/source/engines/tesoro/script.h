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

#ifndef TESORO_SCRIPT_H
#define TESORO_SCRIPT_H
#include "tesoro/resources.h"
namespace Tesoro {
struct Block {
	Common::String name, file;
	uint nativeType;
	Common::Array<Common::String> actors, lines;
	Common::Array<uint> sourceLines;
	Common::HashMap<Common::String, uint> labels;
	Common::HashMap<uint, uint> nativeOrdinals;
};
class Catalog {
public:
	Common::HashMap<Common::String, Block> blocks;
	Common::HashMap<Common::String, Common::String> texts;
	void load(Archive &archive);
};
// A request is either completed immediately or held by the host. The VM never
// reads the next instruction while a speech/movement/animation request is held.
struct Request {
	enum Kind { kSpeech,
				kSleep,
				kMessage,
				kNative } kind;
	Common::String target, scope, textId, text;
	Common::Array<Common::String> words;
	uint number, nativeType;
	Request(Kind k) : kind(k), number(0), nativeType(0) {}
};
class ScriptHost {
public:
	virtual ~ScriptHost() {}
	virtual bool request(const Request &r) = 0; // true: asynchronous completion
	virtual bool complete() = 0;
};
class ScriptVM {
	Catalog &_catalog;
	ScriptHost &_host;
	const Block *_block;
	byte *_vars;
	uint _pc;
	bool _waiting;
	void jump(const Common::String &label);

public:
	ScriptVM(Catalog &c, ScriptHost &h) : _catalog(c), _host(h), _block(nullptr), _vars(nullptr), _pc(0), _waiting(false) {}
	void start(Common::String name, const Common::String &label, byte *vars);
	void update();
	void stop() {
		_block = nullptr;
		_waiting = false;
	}
	bool busy() const { return _block != nullptr; }
	uint pc() const { return _pc; }
	Common::String failure;
};
Common::Array<Common::String> tokenize(Common::String line);
} // namespace Tesoro
#endif
