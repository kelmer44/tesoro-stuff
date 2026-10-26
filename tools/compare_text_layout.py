#!/usr/bin/env python3
"""Compare the production text layout with the original routines at any position.

Uses a local ignored driver (local-media/jim-implementation/text_driver.cpp) that links the
production renderer. The original layout routine (0x411c80) and glyph drawing (0x412c50) run in
the Unicorn emulator over the real font. The original keeps at most 30 lines per frame, so very narrow widths are not tried.
Each case lays out one speech text from the game's
own text tables with a random colour, centre, height, maximum width and line height, on a
grey frame. Run it with the Python of the local animation environment, which has Unicorn.
"""
import argparse
import hashlib
import json
import random
import re
import struct
import subprocess
from pathlib import Path

from compare_opening_pixels import NativePixels, resource
from inspect_tesoro import read_index

TABLES = ['OBJETOS.TXT', 'ANIMS.TXT', 'CONVERS.TXT', 'ICONS.TXT']


class AtPositionPixels(NativePixels):
    def font_frame_at(self, text, colour, cx, y, width, line_height):
        self.uc.mem_write(self.output, struct.pack('<H', 0x39e7) * (640 * 480))
        self.uc.mem_write(self.renderer + 0x13a0, bytes(4))
        self.uc.mem_write(self.text, text + b'\0')
        self.invoke(0x411c80, self.renderer, (self.text, colour, 0, cx, y, width, 0, 1, line_height))
        count = struct.unpack('<I', self.uc.mem_read(self.renderer + 0x13a0, 4))[0]
        for i in range(count):
            self.invoke(0x412c50, self.renderer, (self.renderer + 0x680 + i * 0x70,))
        return self.rgba()


def speech_texts(game):
    dat = (game / 'TESORO.DAT').read_bytes()
    index = {e['name'].upper(): e for e in read_index((game / 'TESORO.TRE').read_bytes(), dat)}
    texts = []
    for table in TABLES:
        entry = index[table]
        raw = dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]
        for line in raw.split(b'\n'):
            m = re.match(rb'\s*[^,]+,\s*"(.*)"', line.rstrip(b'\r'))
            if m and m.group(1).strip():
                texts.append(m.group(1))
    return texts


def compare(game, driver, count, seed):
    rng = random.Random(seed)
    texts = speech_texts(game)
    font = resource(game, 'FUENTES.ANM')
    cases = []
    for _ in range(count):
        kind = rng.random()
        if kind < 0.15:
            cx, y, width = 320, 240, 640        # the narration layout
        else:
            cx, y = rng.randint(-50, 700), rng.randint(-80, 560)
            width = rng.choice((300, 300, 300, 150, 200, 450, 600, 640, 700, 120))
        cases.append((rng.choice(texts), rng.choice((0x1f, 0xffc0, 0xffff, 0xf800, 0x07e0)),
                      cx, y, width, rng.choice((0, 0, 0, 20, 31, 45))))
    request = struct.pack('<I', len(font)) + font + struct.pack('<I', len(cases))
    for text, colour, cx, y, width, lh in cases:
        request += struct.pack('<I', len(text)) + text + struct.pack('<5i', colour, cx, y, width, lh)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True, timeout=1800)
    output = process.stdout
    if len(output) != len(cases) * 640 * 480 * 4:
        raise ValueError('Wrong amount of C++ output')
    # The original needs a scene-shaped payload only for its image arena; the font is passed as the scene.
    original = AtPositionPixels((game / 'TESORO.EXE').read_bytes(), font, font)
    lines_total = 0
    for number, (text, colour, cx, y, width, lh) in enumerate(cases):
        expected = original.font_frame_at(text, colour, cx, y, width, lh)
        actual = output[number * 640 * 480 * 4:(number + 1) * 640 * 480 * 4]
        if actual != expected:
            pixel = next(i for i in range(640 * 480) if actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4])
            differing = sum(actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4] for i in range(640 * 480))
            raise ValueError(f'case {number} {cases[number]}: {differing} pixels differ, first ({pixel % 640},'
                             f'{pixel // 640})')
    return dict(cases=len(cases), pixels_compared=len(cases) * 640 * 480,
                narration_layout_cases=sum(1 for c in cases if c[2:5] == (320, 240, 640)),
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                font_sha256=hashlib.sha256(font).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cases', type=int, default=200)
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
