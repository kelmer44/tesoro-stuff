#!/usr/bin/env python3
"""Compare the production renderer's scaled drawing with the original x86 routines.

Uses a local ignored driver (local-media/jim-implementation/scaled_driver.cpp) that links
the production renderer. the
unscaled queue (0x411f80), the row reset (0x411f20) and the row composition (0x412e10)
are executed in the Unicorn emulator over Jim's real animation images. Each case draws
one scaled sprite, and sometimes an unscaled one in front of it, onto a black frame.

Run it with the Python of the local animation environment, which has Unicorn.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
import sys
from pathlib import Path

from compare_opening_pixels import NativePixels as BasePixels, resource
from inspect_tesoro import read_index, u16
from inspect_tesoro import read_banks

BANKS = ['JIMHABIZ', 'JIMHABDER', 'JIMQUIETO', 'JIMARRIBA', 'JIMABAJO', 'JIMIZQUIERDA',
         'JIMDERECHA', 'JIMHABAB', 'JIMHABAR']
SCALES = [20, 35, 50, 68, 73, 85, 100, 128, 200, 256, 300]


class NativePixels(BasePixels):
    # Jim's images need a larger private arena than the harbour's.
    rows = 0x4c00000
    shadow_rows = 0x4d00000
    output = 0x4e00000
    arena_size = 0xf00000


def packed(data):
    return struct.pack('<I', len(data)) + data


def make_cases(counts, seed):
    rng = random.Random(seed)
    cases = []
    for n in range(counts):
        bank = rng.randrange(len(BANKS))
        image = rng.randrange(images[bank])
        scale = rng.choice(SCALES)
        x = rng.randint(-250, 650)
        y = rng.randint(-300, 520)
        front = rng.random() < 0.5
        fbank = rng.randrange(len(BANKS))
        cases.append((bank, image, x, y, scale, 35, int(front), fbank,
                      rng.randrange(images[fbank]), x + rng.randint(-40, 40),
                      y + rng.randint(-60, 60), rng.choice((10, 34, 35, 36, 60))))
    return cases


def compare(game, driver, count, seed):
    global images
    dat = (game / 'TESORO.DAT').read_bytes()
    index = {e['name'].upper(): e for e in read_index((game / 'TESORO.TRE').read_bytes(), dat)}
    anm = []
    for name in BANKS:
        entry = index[name + '.ANM']
        anm.append(dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']])
    images = [u16(a, 8) for a in anm]
    cases = make_cases(count, seed)
    # The harbour's backdrop (the first bank of PUERTO.LVL) goes behind every case.
    puerto = resource(game, 'PUERTO.LVL')
    first = read_banks(puerto)[0]
    anm.append(puerto[16:first['end']])
    request = struct.pack('<I', len(anm)) + b''.join(map(packed, anm)) + struct.pack('<I', len(cases))
    for case in cases:
        request += struct.pack('<12i', *case)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True,
                             check=True, timeout=600)
    output = process.stdout
    if len(output) != len(cases) * 640 * 480 * 4:
        raise ValueError('Wrong amount of C++ output')

    # The original needs the images in one scene-like payload: a header and the banks.
    lvl = b'LVL\1' + struct.pack('<III', 0, len(anm), 0) + b''.join(anm)
    back = len(anm) - 1
    original = NativePixels((game / 'TESORO.EXE').read_bytes(), lvl, resource(game, 'FUENTES.ANM'))
    matches = []
    for number, (bank, image, x, y, scale, depth, front, fbank, fimage, fx, fy, fdepth) in enumerate(cases):
        original.uc.mem_write(original.output, bytes(640 * 480 * 2))
        original.invoke(0x411f20, original.renderer)
        original.invoke(0x411f80, original.renderer, (0, 0, 200, original.buffers[back, 0], 0))
        if front:
            original.invoke(0x411f80, original.renderer,
                            (fx, fy, fdepth, original.buffers[fbank, fimage], 0))
        # Scaled sprites are queued and drawn by the composition, from the farthest to the nearest.
        original.invoke(0x4120c0, original.renderer,
                        (x, y, depth, original.buffers[bank, image], scale << 8, scale << 8, 0, 0))
        original.invoke(0x412e10, original.renderer, instruction_limit=20000000)
        expected = original.rgba()
        actual = output[number * 640 * 480 * 4:(number + 1) * 640 * 480 * 4]
        if actual != expected:
            pixel = next(i for i in range(640 * 480) if actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4])
            differing = sum(actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4] for i in range(640 * 480))
            raise ValueError(f'case {number} {cases[number]}: {differing} pixels differ, first ({pixel % 640},'
                             f'{pixel // 640}) cpp={actual[pixel*4:pixel*4+4].hex()} '
                             f'original={expected[pixel*4:pixel*4+4].hex()}')
        matches.append(hashlib.sha256(actual).hexdigest())
    return dict(cases=len(cases), pixels_compared=len(cases) * 640 * 480,
                with_unscaled_front=sum(c[6] for c in cases),
                scales=sorted({c[4] for c in cases}),
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                resource_sha256={n: hashlib.sha256(a).hexdigest() for n, a in zip(BANKS + ['PUERTO-FONDO'], anm)},
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest(), frames=matches)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cases', type=int, default=40)
    parser.add_argument('--seed', type=int, default=1)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver, args.cases, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest()
        for name in ('renderer.cpp', 'renderer.h', 'resources.cpp', 'resources.h', 'scene.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k not in ('frames', 'source_sha256', 'resource_sha256')}))
