#!/usr/bin/env python3
"""Read-only Tesoro format audit and sample image decoder (Python stdlib only).

Based on direct inspection and the existing adventure-explorer Tesoro reader.
This is an investigation tool, not a game engine. Output remains local.
"""
import argparse
from collections import Counter
import hashlib
import io
import json
from pathlib import Path
import re
import struct
import wave
import zlib


def u16(data, at):
    return struct.unpack_from('<H', data, at)[0]


def u32(data, at):
    return struct.unpack_from('<I', data, at)[0]


def read_index(tre, dat):
    if len(tre) % 28:
        raise ValueError('TRE is not a whole number of 28-byte records')
    entries = []
    for at in range(0, len(tre), 28):
        offset, size, flags = struct.unpack_from('<IIB', tre, at)
        if not flags & 4:
            continue
        name = tre[at + 9:at + 28].split(b'\0')[0].decode('ascii')
        if not name or Path(name).name != name:
            raise ValueError('Invalid resource name')
        if offset + 4 + size > len(dat) or u32(dat, offset) != size:
            raise ValueError(f'Invalid resource bounds/size prefix: {name}')
        entries.append(dict(name=name, offset=offset, size=size, flags=flags))
    if len({e['name'].lower() for e in entries}) != len(entries):
        raise ValueError('Duplicate resource names')
    return entries


def read_banks(data):
    count, at = 1, 0
    if data[:4] == b'LVL\1':
        count, at = u32(data, 8), 16
    if count > 10000:
        raise ValueError('Implausible bank count')
    banks = []
    for _ in range(count):
        if data[at:at + 4] not in (b'ANM\0', b'ANM\1'):
            raise ValueError(f'Invalid bank at {at}')
        frames, image_count = u16(data, at + 4), u16(data, at + 8)
        if max(frames, image_count) > 10000:
            raise ValueError('Implausible bank dimensions')
        name = data[at + 12:at + 24].split(b'\0')[0].decode('cp850')
        font = data[at + 3] == 0
        at += 28
        for _ in range(frames):
            if at + 24 > len(data):
                raise ValueError('Truncated frame record')
            at += 24 + u16(data, at + 22)
        images = []
        for _ in range(image_count):
            size, base = u32(data, at), at + 4
            width, height, kind = struct.unpack_from('<HHH', data, base + 8)
            if (not 0 < width <= 4096 or not 0 < height <= 4096
                    or size < 16 + height * 8 or base + size > len(data)
                    or kind not in (1, 2, 3)):
                raise ValueError('Invalid image block')
            images.append(dict(base=base, size=size, width=width, height=height, kind=kind))
            at = base + size
        banks.append(dict(name=name, frames=frames, font=font, images=images, end=at))
    return banks


def decode_image(data, image):
    base, size = image['base'], image['size']
    width, height, kind = image['width'], image['height'], image['kind']
    pixel_start = u32(data, base)
    if not 16 + height * 8 <= pixel_start <= size:
        raise ValueError('Invalid pixel offset')
    pixels = bytearray(width * height * 3)
    for y in range(height):
        row = base + 16 + y * 8
        offset, count = u32(data, row), u16(data, row + 4)
        if offset < 16 + height * 8 or offset + count * 8 > pixel_start:
            raise ValueError('Invalid span table')
        x = 0
        for j in range(count):
            value, skip, length = struct.unpack_from('<IHH', data, base + offset + j * 8)
            x += skip
            if x + length > width:
                raise ValueError('Span outside image')
            if kind == 1 and not pixel_start <= value <= size - length * 2:
                raise ValueError('Pixel data outside image')
            for k in range(length):
                if kind == 2:
                    gray = value % 32 * 255 // 31
                    rgb = (gray, gray, gray)  # Diagnostic preview, not in-game blending.
                else:
                    color = u16(data, base + value + k * 2) if kind == 1 else value % 65536
                    rgb = (color >> 11, (color >> 5) & 63, color & 31)
                    rgb = (rgb[0] * 255 // 31, rgb[1] * 255 // 63, rgb[2] * 255 // 31)
                dst = (y * width + x + k) * 3
                pixels[dst:dst + 3] = bytes(rgb)
            x += length
    return pixels


def read_scene_objects(data, banks=None):
    """Structural LVL records; parameter meanings are intentionally left raw."""
    if data[:4] != b'LVL\1':
        raise ValueError('Expected LVL resource')
    banks = read_banks(data) if banks is None else banks
    at = banks[-1]['end'] if banks else 16
    objects = []
    for group in range(u32(data, 4)):
        if at + 40 > len(data):
            raise ValueError('Truncated object group')
        count = u16(data, at + 32)
        group_header = data[at:at + 40].hex()
        at += 40
        for _ in range(count):
            if at + 32 > len(data):
                raise ValueError('Truncated object record')
            x, y = struct.unpack_from('<ii', data, at)
            count_extra = data[at + 11]
            kind, parameter_size = u32(data, at + 24), u32(data, at + 28)
            end_parameters = at + 32 + parameter_size
            end = end_parameters + count_extra
            if end > len(data):
                raise ValueError('Truncated object parameters/references')
            objects.append(dict(group=group, group_header=group_header,
                offset=at, name=data[at + 12:at + 24].split(b'\0')[0].decode('cp850'),
                x=x, y=y, field_08=u16(data, at + 8), field_10=data[at + 10],
                kind=kind, parameters_hex=data[at + 32:end_parameters].hex(),
                extra_bytes=list(data[end_parameters:end])))
            at = end
    if at != len(data):
        raise ValueError(f'Unconsumed scene bytes: {len(data) - at}')
    return objects


def save_png(path, image, pixels):
    def chunk(tag, content):
        return (struct.pack('>I', len(content)) + tag + content
                + struct.pack('>I', zlib.crc32(tag + content)))
    width, height = image['width'], image['height']
    rows = b''.join(b'\0' + pixels[y * width * 3:(y + 1) * width * 3] for y in range(height))
    path.write_bytes(b'\x89PNG\r\n\x1a\n'
                     + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
                     + chunk(b'IDAT', zlib.compress(rows)) + chunk(b'IEND', b''))


def audit(game, output):
    tre, dat = (game / 'TESORO.TRE').read_bytes(), (game / 'TESORO.DAT').read_bytes()
    entries = read_index(tre, dat)
    output.mkdir(parents=True, exist_ok=True)
    report = dict(input_directory=str(game.resolve()), file_count=len(entries),
                  index_records=len(tre) // 28,
                  extensions=dict(Counter(Path(e['name']).suffix.lower() for e in entries)),
                  hashes={}, images=0, banks=0, sequence_previews=0, scene_objects=0,
                  audio_formats=Counter(), scripts=[], media_refs={'avi': set(), 'cd': set()},
                  previews=[], resources=entries)
    for name in ('TESORO.EXE', 'TESORO.TRE', 'TESORO.DAT'):
        path = game / name
        report['hashes'][name] = dict(size=path.stat().st_size,
                                     sha256=hashlib.sha256(path.read_bytes()).hexdigest())
    for entry in entries:
        at, size, name = entry['offset'], entry['size'], entry['name']
        payload = dat[at + 4:at + 4 + size]
        ext = Path(name).suffix.lower()
        if ext in ('.anm', '.lvl'):
            banks = read_banks(payload)
            report['banks'] += len(banks)
            report['images'] += sum(len(b['images']) for b in banks)
            report['sequence_previews'] += sum(len(b['images']) > 1 and not b['font'] for b in banks)
            if ext == '.lvl':
                scene_objects = read_scene_objects(payload, banks)
                report['scene_objects'] += len(scene_objects)
                if name.lower() == 'despacho.lvl':
                    (output / 'office-objects.json').write_text(json.dumps(scene_objects, indent=2) + '\n')
            if name.lower() in ('principal.lvl', 'puerto.lvl', 'taberna.lvl', 'jimquieto.anm'):
                image = banks[0]['images'][0]
                target = output / (Path(name).stem.lower() + '.png')
                save_png(target, image, decode_image(payload, image))
                report['previews'].append(target.name)
        elif ext == '.wav':
            with wave.open(io.BytesIO(payload)) as audio:
                key = f'{audio.getnchannels()}ch/{audio.getframerate()}Hz/{audio.getsampwidth()*8}bit/{audio.getcomptype()}'
                report['audio_formats'][key] += 1
                if len(audio.readframes(audio.getnframes())) != audio.getnframes() * audio.getnchannels() * audio.getsampwidth():
                    raise ValueError(f'Truncated audio: {name}')
        elif ext == '.dlg':
            text = payload.decode('cp850')  # Readable Spanish; verify original glyph mapping separately.
            lines = text.splitlines()
            report['scripts'].append(dict(name=name, lines=len(lines),
                blocks=sum(l.startswith('.') and not l.startswith('.END') for l in lines),
                native_calls=sum(l.startswith('DO ') for l in lines)))
            for kind, number in re.findall(r'(?m)^play (avi|cd) (\d+)', text):
                report['media_refs'][kind].add(int(number))
    report['audio_formats'] = dict(report['audio_formats'])
    report['media_refs'] = {k: sorted(v) for k, v in report['media_refs'].items()}
    report['unindexed_bytes'] = len(dat) - sum(e['size'] + 4 for e in entries)
    (output / 'inventory.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k not in ('resources', 'hashes')}, indent=2))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('game', type=Path)
    parser.add_argument('--output', type=Path, default=Path('analysis'))
    args = parser.parse_args()
    audit(args.game, args.output)
