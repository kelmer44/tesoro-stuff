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

#include "tesoro/resources.h"
#include "common/algorithm.h"
#include "common/endian.h"
#include "common/textconsole.h"
namespace Tesoro {
static Common::String key(Common::String s) {
	s.toUppercase();
	return s;
}
void Archive::open() {
	Common::File tre;
	if (!tre.open("TESORO.TRE") || !_dat.open("TESORO.DAT"))
		error("Tesoro: missing TRE/DAT");
	if (tre.size() % 28)
		error("Tesoro: invalid TRE size");
	while (tre.pos() < tre.size()) {
		Entry e;
		e.offset = tre.readUint32LE();
		e.size = tre.readUint32LE();
		byte flags = tre.readByte();
		char name[20] = {};
		tre.read(name, 19);
		if (!(flags & 4))
			continue;
		if (!name[0] || uint64(e.offset) + 4 + e.size > uint64(_dat.size()))
			error("Tesoro: bad resource bounds");
		_dat.seek(e.offset);
		if (_dat.readUint32LE() != e.size)
			error("Tesoro: bad resource prefix");
		Common::String n = key(name);
		if (_entries.contains(n))
			error("Tesoro: duplicate resource %s", name);
		_entries[n] = e;
	}
}
Common::SeekableReadStream *Archive::stream(Common::String name) {
	name = key(name);
	if (!_entries.contains(name))
		error("Tesoro: missing resource %s", name.c_str());
	const Entry &e = _entries[name];
	_dat.seek(e.offset + 4);
	return _dat.readStream(e.size);
}
Bytes Archive::read(Common::String name) {
	Common::ScopedPtr<Common::SeekableReadStream> in(stream(name));
	Bytes b;
	b.resize(in->size());
	if (in->read(b.data(), b.size()) != b.size())
		error("Tesoro: truncated resource");
	return b;
}
Common::Array<Common::String> Archive::names() const {
	Common::Array<Common::String> out;
	for (auto i = _entries.begin(); i != _entries.end(); ++i)
		out.push_back(i->_key);
	return out;
}
// Every read is bounded against its own resource, not the surrounding archive.
class Reader {
	const Bytes &_b;

public:
	uint32 at;
	Reader(const Bytes &b) : _b(b), at(0) {}
	void check(uint32 p, uint32 n) const {
		if (uint64(p) + n > _b.size())
			error("Tesoro: resource overrun at %u", p);
	}
	uint16 u16(uint32 p) const {
		check(p, 2);
		return READ_LE_UINT16(_b.data() + p);
	}
	uint32 u32(uint32 p) const {
		check(p, 4);
		return READ_LE_UINT32(_b.data() + p);
	}
	Common::String str(uint32 p, uint32 n) const {
		check(p, n);
		uint32 end = p;
		while (end < p + n && _b[end])
			++end;
		return Common::String((const char *)_b.data() + p, end - p);
	}
	Bytes bytes(uint32 p, uint32 n) const {
		check(p, n);
		Bytes out;
		out.resize(n);
		if (n)
			memcpy(out.data(), _b.data() + p, n);
		return out;
	}
	Bank bank();
};
Bank Reader::bank() {
	check(at, 28);
	if ((u32(at) & 0xffffff) != 0x4d4e41 || (u32(at) >> 24) > 1)
		error("Tesoro: bad ANM header");
	uint n = u16(at + 4), ni = u16(at + 8);
	if (n > 10000 || ni > 10000)
		error("Tesoro: implausible ANM count");
	Bank bank;
	bank.name = str(at + 12, 12);
	at += 28;
	for (uint i = 0; i < n; ++i) {
		check(at, 24);
		Frame f;
		f.x = u16(at);
		f.y = u16(at + 2);
		f.dx = u16(at + 4);
		f.dy = u16(at + 6);
		f.duration = u16(at + 8);
		f.image = u16(at + 10);
		f.type = u16(at + 20);
		uint extra = u16(at + 22);
		check(at + 24, extra);
		at += 24 + extra;
		bank.frames.push_back(f);
	}
	for (uint i = 0; i < ni; ++i) {
		uint32 size = u32(at), base = at + 4;
		check(base, size);
		if (size < 16)
			error("Tesoro: short image header");
		Common::SharedPtr<Image> im(new Image);
		im->w = u16(base + 8);
		im->h = u16(base + 10);
		im->kind = u16(base + 12);
		uint32 pixelStart = u32(base);
		if (!im->w || !im->h || im->w > 4096 || im->h > 4096 || im->kind < 1 || im->kind > 3 || pixelStart < 16 + im->h * 8U || pixelStart > size)
			error("Tesoro: bad image dimensions/table");
		im->pixels.resize(im->w * im->h);
		Common::fill(im->pixels.begin(), im->pixels.end(), 0);
		for (uint y = 0; y < im->h; ++y) {
			uint32 offset = u32(base + 16 + y * 8);
			uint count = u16(base + 20 + y * 8), x = 0;
			if (offset < 16 + im->h * 8U || uint64(offset) + count * 8 > pixelStart)
				error("Tesoro: bad span table");
			for (uint s = 0; s < count; ++s) {
				uint32 span = base + offset + s * 8, value = u32(span);
				uint skip = u16(span + 4), len = u16(span + 6);
				x += skip;
				if (x + len > im->w || (im->kind == 1 && (value < pixelStart || uint64(value) + len * 2 > size)))
					error("Tesoro: bad image span");
				for (uint k = 0; k < len; ++k) {
					uint32 c;
					if (im->kind == 2)
						c = 0x80000000 | ((value & 31) * 255 / 31); // Shadow marker: renderer multiplies destination.
					else {
						uint16 rgb = im->kind == 1 ? u16(base + value + k * 2) : value;
						c = 0xff000000 | (((rgb >> 11) * 255 / 31) << 16) | ((((rgb >> 5) & 63) * 255 / 63) << 8) | ((rgb & 31) * 255 / 31);
					}
					im->pixels[y * im->w + x + k] = c;
				}
				x += len;
			}
		}
		bank.images.push_back(im);
		at = base + size;
	}
	for (uint i = 0; i < bank.frames.size(); ++i)
		if (bank.frames[i].image < -1 || bank.frames[i].image >= (int)ni)
			error("Tesoro: bad frame image reference");
	return bank;
}
Object *Scene::find(Common::String name) {
	for (uint i = 0; i < objects.size(); ++i)
		if (objects[i].name.equalsIgnoreCase(name))
			return &objects[i];
	return nullptr;
}
Scene readScene(const Bytes &data) {
	Reader r(data);
	if (r.u32(0) != 0x014c564c)
		error("Tesoro: bad LVL header");
	uint groups = r.u32(4), count = r.u32(8);
	r.at = 16;
	if (groups > 10000 || count > 10000)
		error("Tesoro: implausible LVL count");
	Scene sc;
	for (uint i = 0; i < count; ++i)
		sc.banks.push_back(r.bank());
	for (uint g = 0; g < groups; ++g) {
		r.check(r.at, 40);
		uint n = r.u16(r.at + 32);
		r.at += 40;
		for (uint j = 0; j < n; ++j) {
			r.check(r.at, 32);
			Object o;
			o.x = r.u32(r.at);
			o.y = r.u32(r.at + 4);
			o.priority = r.u16(r.at + 8);
			o.name = r.str(r.at + 12, 12);
			o.kind = r.u32(r.at + 24);
			uint extra = r.bytes(r.at + 11, 1)[0];
			uint32 len = r.u32(r.at + 28);
			r.at += 32;
			o.parameters = r.bytes(r.at, len);
			r.at += len;
			o.refs = r.bytes(r.at, extra);
			r.at += extra;
			for (uint k = 0; k < o.refs.size(); ++k)
				if (o.refs[k] >= sc.banks.size())
					error("Tesoro: bad object bank reference");
			sc.objects.push_back(o);
		}
	}
	if (r.at != data.size())
		error("Tesoro: LVL trailing bytes");
	return sc;
}
Bank readAnimation(const Bytes &data) {
	Reader r(data);
	Bank b = r.bank();
	if (r.at != data.size())
		error("Tesoro: ANM trailing bytes");
	return b;
}
} // namespace Tesoro
