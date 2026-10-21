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

#include "tesoro/script.h"
#include "common/memstream.h"
#include "common/textconsole.h"
namespace Tesoro {
static Common::String clean(Common::String s) {
	size_t p = s.find("//");
	if (p != Common::String::npos)
		s = Common::String(s.c_str(), p);
	s.trim();
	return s;
}
Common::Array<Common::String> tokenize(Common::String line) {
	Common::Array<Common::String> words;
	Common::String word;
	bool quote = false;
	for (uint i = 0; i < line.size(); ++i) {
		char c = line[i];
		if (c == '\'') {
			quote = !quote;
			continue;
		}
		if (!quote && (c == ' ' || c == '\t' || c == ',')) {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
		} else
			word += c;
	}
	if (!word.empty())
		words.push_back(word);
	return words;
}
void Catalog::load(Archive &archive) {
	Common::Array<Common::String> names = archive.names();
	for (uint n = 0; n < names.size(); ++n) {
		bool dlg = names[n].hasSuffix(".DLG"), txt = names[n].hasSuffix(".TXT");
		if (!dlg && !txt)
			continue;
		Common::ScopedPtr<Common::SeekableReadStream> in(archive.stream(names[n]));
		Common::Array<Common::String> lines;
		while (!in->eos())
			lines.push_back(in->readLine());
		if (txt) {
			for (uint i = 0; i < lines.size(); ++i) {
				Common::String s = lines[i];
				size_t comma = s.find(','), first = s.find('"'), last = s.findLastOf('"');
				if (comma == Common::String::npos || first == Common::String::npos || last <= first)
					continue;
				Common::String id(s.c_str(), comma);
				id.trim();
				id.toUppercase();
				texts[id] = Common::String(s.c_str() + first + 1, last - first - 1);
			}
			continue;
		}
		uint i = 0;
		while (i < lines.size()) {
			Common::String head = clean(lines[i++]);
			if (head.empty())
				continue;
			if (head[0] != '.' || head == ".END" || i >= lines.size())
				error("Tesoro: bad DLG header");
			Common::Array<Common::String> h = tokenize(head);
			Block b;
			b.name = h[0].c_str() + 1;
			b.name.toUppercase();
			b.file = names[n];
			b.nativeType = h.size() > 1 ? atoi(h[1].c_str()) : 0;
			uint count = atoi(lines[i++].c_str());
			if (count > 100 || i + count > lines.size())
				error("Tesoro: bad DLG actor count");
			for (uint k = 0; k < count; ++k)
				b.actors.push_back(clean(lines[i++]));
			uint ord = 0;
			while (i < lines.size() && clean(lines[i]) != ".END") {
				Common::String s = clean(lines[i]);
				uint pc = b.lines.size();
				if (s.hasPrefix("_")) {
					Common::Array<Common::String> t = tokenize(s);
					b.labels[t[0].c_str() + 1] = pc;
				}
				if (s.hasPrefix("DO "))
					b.nativeOrdinals[pc] = ord++;
				b.lines.push_back(s);
				b.sourceLines.push_back(i + 1);
				++i;
			}
			if (i == lines.size())
				error("Tesoro: unterminated DLG block");
			++i;
			if (blocks.contains(b.name))
				error("Tesoro: duplicate DLG block");
			blocks[b.name] = b;
		}
	}
}
void ScriptVM::jump(const Common::String &label) {
	if (!_block->labels.contains(label)) {
		failure = "Missing script label: " + label;
		stop();
		return;
	}
	_pc = _block->labels[label];
}
void ScriptVM::start(Common::String name, const Common::String &label, byte *vars) {
	stop();
	failure.clear();
	name.toUppercase();
	if (!_catalog.blocks.contains(name)) {
		failure = "Missing script: " + name;
		return;
	}
	_block = &_catalog.blocks[name];
	_vars = vars;
	jump(label);
}
void ScriptVM::update() {
	if (!_block)
		return;
	if (_waiting) {
		if (!_host.complete())
			return;
		_waiting = false;
	}
	for (uint budget = 0; budget < 1000 && _block; ++budget) {
		if (_pc >= _block->lines.size()) {
			stop();
			return;
		}
		uint current = _pc++;
		Common::String s = _block->lines[current];
		if (s.empty() || s[0] == '_')
			continue;
		Common::Array<Common::String> t = tokenize(s);
		if (s == "return") {
			stop();
			return;
		}
		if (t[0] == "jmp" && t.size() == 2) {
			jump(t[1]);
			continue;
		}
		if (t.size() == 6 && t[0] == "if" && t[1][0] == 'v' && t[2] == "==" && t[4] == "jmp") {
			uint v = atoi(t[1].c_str() + 1);
			if (v >= 32)
				goto unsupported;
			if (_vars[v] == byte(atoi(t[3].c_str())))
				jump(t[5]);
			continue;
		}
		if (t.size() == 3 && t[0][0] == 'v' && t[1] == "=") {
			uint v = atoi(t[0].c_str() + 1);
			if (v >= 32)
				goto unsupported;
			_vars[v] = atoi(t[2].c_str());
			continue;
		}
		if (t[0] == "run" && t.size() == 3) {
			uint next = _pc;
			while (next < _block->lines.size() && (_block->lines[next].empty() || _block->lines[next][0] == '_'))
				++next;
			if (next >= _block->lines.size() || _block->lines[next] != "return")
				goto unsupported;
			byte *context = _vars;
			start(t[1], t[2], context);
			continue;
		}
		{
			Request r(Request::kMessage);
			if (t[0] == "sleep" && t.size() == 2) {
				r.kind = Request::kSleep;
				r.number = atoi(t[1].c_str());
			} else if (t[0] == "DO" && _block->nativeOrdinals.contains(current)) {
				r.kind = Request::kNative;
				r.nativeType = _block->nativeType;
				r.number = _block->nativeOrdinals[current];
			} else if (t[0] == "sendmessage" && t.size() >= 4) {
				r.target = t[1];
				uint from = 2;
				if (r.target[0] >= '0' && r.target[0] <= '9') {
					uint slot = atoi(r.target.c_str());
					if (!slot || slot > _block->actors.size())
						goto unsupported;
					r.target = _block->actors[slot - 1];
				}
				if (t[2] == "in") {
					if (t.size() < 6)
						goto unsupported;
					r.scope = t[3];
					from = 4;
				}
				for (uint k = from; k < t.size(); ++k)
					r.words.push_back(t[k]);
			} else if ((t.size() == 2 || t.size() == 3) && s.contains(',')) {
				r.kind = Request::kSpeech;
				r.textId = t[0];
				r.textId.toUppercase();
				if (!_catalog.texts.contains(r.textId))
					goto unsupported;
				r.text = _catalog.texts[r.textId];
				uint slot = atoi(t[1].c_str());
				if (slot > _block->actors.size())
					goto unsupported;
				if (slot)
					r.target = _block->actors[slot - 1];
			} else
				goto unsupported;
			if (_host.request(r)) {
				_waiting = true;
				return;
			}
			continue;
		}
	unsupported:
		failure = Common::String::format("%s:%u (%s): unsupported: %s", _block->file.c_str(), _block->sourceLines[current], _block->name.c_str(), s.c_str());
		warning("%s", failure.c_str());
		stop();
		return;
	}
	if (_block) {
		failure = "Script instruction limit exceeded";
		stop();
	}
}
} // namespace Tesoro
