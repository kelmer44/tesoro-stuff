#!/usr/bin/env python3
"""Compare the production renderer with original x86 span composition and font drawing.

Uses a local ignored driver. Original RLE buffers are independently relocated
into private emulator memory. No Windows process or DirectDraw is used.
"""
import argparse
from array import array
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

from compare_scene_native import NativeScene
from inspect_tesoro import read_index, read_banks, u16, u32
from trace_script import load_game


def resource(game, name):
    dat = (game / 'TESORO.DAT').read_bytes()
    entry = next(e for e in read_index((game / 'TESORO.TRE').read_bytes(), dat)
                 if e['name'].upper() == name)
    return dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]


class NativePixels(NativeScene):
    renderer = 0x4000000
    font_bank = 0x4002000
    font_frames = 0x4003000
    font_wrappers = 0x4005000
    text = 0x4006000
    rows = 0x4300000
    shadow_rows = 0x4400000
    output = 0x4500000

    def submit(self, uc, address, size, data):
        # Leave the native queue routine running; the scene-state harness intercepts it.
        pass

    def __init__(self, executable, scene, font):
        super().__init__(executable)
        self.uc.mem_map(0x4000000, 0x800000)
        self.next_image = 0x4010000
        self.buffers = {}
        for b, bank in enumerate(read_banks(scene)):
            for i, image in enumerate(bank['images']):
                self.buffers[b, i] = self.image(scene, image)
        font_meta = read_banks(font)[0]
        for i, image in enumerate(font_meta['images']):
            address = self.image(font, image)
            self.uc.mem_write(self.font_wrappers + i * 4, struct.pack('<I', address))
        raw = bytearray(64)
        struct.pack_into('<I', raw, 0x1c, self.font_frames)
        struct.pack_into('<HH', raw, 0x20, len(font_meta['images']), font_meta['frames'])
        self.uc.mem_write(self.font_bank, bytes(raw))
        at = 28
        for i in range(font_meta['frames']):
            frame = bytearray(36)
            frame[:24] = font[at:at + 24]
            image = struct.unpack_from('<h', frame, 10)[0]
            struct.pack_into('<I', frame, 24, self.font_wrappers + image * 4)
            self.uc.mem_write(self.font_frames + i * 36, bytes(frame))
            at += 24 + u16(font, at + 22)
        for offset, value in ((0x20, self.font_bank), (0x670, self.rows),
                              (0x674, self.shadow_rows), (0x678, self.output)):
            self.uc.mem_write(self.renderer + offset, struct.pack('<I', value))
        self.uc.mem_write(self.renderer + 0x66a, b'\1')

    def image(self, payload, image):
        base, size = image['base'], image['size']
        raw = bytearray(payload[base:base + size])
        address = self.next_image
        self.next_image += (size + 15) & ~15
        if self.next_image >= self.rows:
            raise ValueError('Image buffers exceeded their private arena')
        for y in range(image['height']):
            row = 16 + y * 8
            spans = u32(raw, row)
            count = u16(raw, row + 4)
            struct.pack_into('<I', raw, row, address + spans)
            if image['kind'] == 1:
                for s in range(count):
                    span = spans + s * 8
                    struct.pack_into('<I', raw, span, address + u32(raw, span))
        self.uc.mem_write(address, bytes(raw))
        return address

    def rgba(self):
        pixels = array('H', self.uc.mem_read(self.output, 640 * 480 * 2))
        if sys.byteorder != 'little':
            pixels.byteswap()
        result = array('I', (0xff000000 | ((p >> 11) * 255 // 31 << 16)
                            | (((p >> 5) & 63) * 255 // 63 << 8) | ((p & 31) * 255 // 31)
                            for p in pixels))
        if sys.byteorder != 'little':
            result.byteswap()
        return result.tobytes()

    def scene_frame(self, sprites):
        self.uc.mem_write(self.output, bytes(640 * 480 * 2))
        self.invoke(0x411f20, self.renderer)
        for bank, image, x, y, depth in sprites:
            self.invoke(0x411f80, self.renderer, (x, y, depth, self.buffers[bank, image], 0))
        self.invoke(0x412e10, self.renderer, instruction_limit=20000000)
        return self.rgba()

    def font_frame(self, text):
        self.uc.mem_write(self.output, struct.pack('<H', 0x39e7) * (640 * 480))
        self.uc.mem_write(self.renderer + 0x13a0, bytes(4))
        self.uc.mem_write(self.text, text + b'\0')
        self.invoke(0x411c80, self.renderer, (self.text, 0x001f, 0, 320, 240, 640, 0, 1, 0))
        count = u32(self.uc.mem_read(self.renderer + 0x13a0, 4), 0)
        for i in range(count):
            self.invoke(0x412c50, self.renderer, (self.renderer + 0x680 + i * 0x70,))
        return self.rgba()


def compare(game, driver):
    scene, font = resource(game, 'PUERTO.LVL'), resource(game, 'FUENTES.ANM')
    blocks, texts, _ = load_game(game)
    speech_ids = [line.strip().split(',')[0] for _, line in blocks['FANPRESENT'].lines
                  if ',' in line and not line.strip().startswith(('DO ', '//'))]
    samples = [texts[name].encode('cp850') for name in speech_ids]
    def packed(data):
        return struct.pack('<I', len(data)) + data
    data = packed(scene) + packed(font) + struct.pack('<I', len(samples)) + b''.join(map(packed, samples))
    process = subprocess.run([str(driver.resolve())], input=data, capture_output=True,
                             check=True, timeout=60)
    output, at = process.stdout, 0
    def take(size):
        nonlocal at
        raw = output[at:at + size]
        if len(raw) != size:
            raise ValueError('Truncated C++ output')
        at += size
        return raw
    def number():
        return struct.unpack('<I', take(4))[0]
    count = number()
    if count != 5 + len(samples):
        raise ValueError('Wrong number of C++ frames')
    original = NativePixels((game / 'TESORO.EXE').read_bytes(), scene, font)
    matches = []
    for case in range(count):
        number_sprites = number()
        if number_sprites == 0xffffffff:
            text = take(number())
            expected = original.font_frame(text)
            name = speech_ids[case - 5]
        else:
            sprites = [struct.unpack('<IIiiI', take(20)) for _ in range(number_sprites)]
            expected = original.scene_frame(sprites)
            name = ('black', 'harbour', 'ship', 'gull', 'recruiter')[case]
        actual = take(640 * 480 * 4)
        if actual != expected:
            pixel = next(i for i in range(640 * 480) if actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4])
            raise ValueError(f'{name}: pixel ({pixel % 640},{pixel // 640}) differs: '
                             f'cpp={actual[pixel*4:pixel*4+4].hex()} original={expected[pixel*4:pixel*4+4].hex()}')
        matches.append(dict(name=name, sha256=hashlib.sha256(actual).hexdigest()))
    if at != len(output):
        raise ValueError('Unexpected extra driver output')
    return dict(scene_frames=5, subtitle_frames=len(samples), pixels_compared=count * 640 * 480,
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                resource_sha256={name: hashlib.sha256(payload).hexdigest()
                                 for name, payload in (('PUERTO.LVL', scene), ('FUENTES.ANM', font))},
                matched=matches, driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest(),
                limitation='Same submitted sprites, original unscaled RLE composition and font routines; no actor logic, shadows, scaling, Windows or audible-output comparison.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest()
        for name in ('renderer.cpp', 'renderer.h', 'scene.cpp', 'scene.h', 'resources.cpp', 'animation.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k not in ('matched', 'source_sha256')}))
