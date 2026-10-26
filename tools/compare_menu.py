#!/usr/bin/env python3
"""Compare the production dialogue menu layout with the original routines.

The menu code (0x407d00) lays each choice out with the text routine 0x411c80 (width 640, centre
320, line height 18, no vertical centring), starts the next one 30 below the last line of the
one before, moves the block down so that it ends at 465, and draws the choice under the pointer
again in yellow. This tool executes the text routine (0x411c80) and the glyph drawing (0x412c50)
in the Unicorn emulator and applies the menu's arithmetic to the entries it produces, then compares the frame with
the production renderer's layoutMenu/drawMenu. It does not execute 0x407d00 itself (it reads
the mouse and the pointer), so the arithmetic is read from the decompilation. Uses the ignored driver
local-media/jim-implementation/menu_driver.cpp. Run with the Python of the local animation environment.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
from pathlib import Path

from compare_opening_pixels import NativePixels, resource
from compare_text_layout import speech_texts


class MenuPixels(NativePixels):
    def menu_frame(self, texts, hovered):
        self.uc.mem_write(self.output, struct.pack('<H', 0x39e7) * (640 * 480))
        self.uc.mem_write(self.renderer + 0x13a0, bytes(4))

        def count():
            return struct.unpack('<I', self.uc.mem_read(self.renderer + 0x13a0, 4))[0]

        def entry_y(i):
            return struct.unpack('<i', self.uc.mem_read(self.renderer + 0x680 + i * 0x70 + 0x68, 4))[0]

        starts, y = [], 50
        for text in texts:
            starts.append(y)
            self.uc.mem_write(self.text, text + b'\0')
            self.invoke(0x411c80, self.renderer, (self.text, 0xffff, 0, 320, y, 640, 5, 0, 18))
            y = entry_y(count() - 1) + 30
        starts.append(480)
        shift = 0x1d1 - y
        for i in range(count()):
            address = self.renderer + 0x680 + i * 0x70 + 0x68
            self.uc.mem_write(address, struct.pack('<i', entry_y(i) + shift))
        if hovered >= 0:
            self.uc.mem_write(self.text, texts[hovered] + b'\0')
            self.invoke(0x411c80, self.renderer, (self.text, 0xffe0, 0, 320, starts[hovered] + shift, 640, 5, 0, 18))
        for i in range(count()):
            self.invoke(0x412c50, self.renderer, (self.renderer + 0x680 + i * 0x70,))
        return self.rgba()


def compare(game, driver, count, seed):
    rng = random.Random(seed)
    texts = [t for t in speech_texts(game) if len(t) > 3]
    font = resource(game, 'FUENTES.ANM')
    cases = []
    for _ in range(count):
        n = rng.choice((1, 2, 3, 3, 4, 5))
        choices = [rng.choice(texts) for _ in range(n)]
        cases.append((choices, rng.choice([-1] + list(range(n)))))
    request = struct.pack('<I', len(font)) + font + struct.pack('<I', len(cases))
    for choices, hovered in cases:
        request += struct.pack('<I', len(choices))
        for text in choices:
            request += struct.pack('<I', len(text)) + text
        request += struct.pack('<i', hovered)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True, timeout=1800)
    output = process.stdout
    if len(output) != len(cases) * 640 * 480 * 4:
        raise ValueError('Wrong amount of C++ output')
    original = MenuPixels((game / 'TESORO.EXE').read_bytes(), font, font)
    lines_max = 0
    for number, (choices, hovered) in enumerate(cases):
        expected = original.menu_frame(choices, hovered)
        actual = output[number * 640 * 480 * 4:(number + 1) * 640 * 480 * 4]
        if actual != expected:
            pixel = next(i for i in range(640 * 480) if actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4])
            differing = sum(actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4] for i in range(640 * 480))
            raise ValueError(f'case {number} {cases[number]}: {differing} pixels differ, first ({pixel % 640},{pixel // 640})')
    return dict(cases=len(cases), pixels_compared=len(cases) * 640 * 480,
                with_hovered_choice=sum(1 for c in cases if c[1] >= 0),
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                font_sha256=hashlib.sha256(font).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cases', type=int, default=100)
    parser.add_argument('--seed', type=int, default=1)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver, args.cases, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest()
        for name in ('renderer.cpp', 'renderer.h', 'resources.cpp', 'resources.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'source_sha256'}))
