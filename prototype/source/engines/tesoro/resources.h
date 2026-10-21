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

#ifndef TESORO_RESOURCES_H
#define TESORO_RESOURCES_H
#include "common/array.h"
#include "common/file.h"
#include "common/hashmap.h"
#include "common/ptr.h"
#include "common/str.h"
namespace Tesoro {
struct Entry {
	uint32 offset, size;
};
typedef Common::Array<byte> Bytes;
class Archive {
	Common::File _dat;
	Common::HashMap<Common::String, Entry> _entries;

public:
	void open();
	Bytes read(Common::String name);
	Common::SeekableReadStream *stream(Common::String name);
	Common::Array<Common::String> names() const;
	uint count() const { return _entries.size(); }
};
struct Image {
	uint16 w, h, kind;
	Common::Array<uint32> pixels;
};
struct Frame {
	int16 x, y, dx, dy, image;
	uint16 duration, type;
};
struct Bank {
	Common::String name;
	Common::Array<Frame> frames;
	Common::Array<Common::SharedPtr<Image>> images;
};
struct Object {
	Common::String name;
	int32 x, y;
	uint16 priority;
	uint32 kind;
	Bytes parameters, refs;
	byte vars[32];
	bool active, shown, running, loop;
	uint slot, frame;
	uint32 tick;
	Object() : x(0), y(0), priority(0), kind(0), active(true), shown(true), running(false), loop(false), slot(0), frame(0), tick(0) { memset(vars, 0, sizeof(vars)); }
};
struct Scene {
	Common::Array<Bank> banks;
	Common::Array<Object> objects;
	Object *find(Common::String name);
};
Scene readScene(const Bytes &data);
Bank readAnimation(const Bytes &data);
} // namespace Tesoro
#endif
