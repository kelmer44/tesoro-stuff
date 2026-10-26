#!/usr/bin/env python3
"""Compare the production renderer's shadow images with the original x86 routines.

The only shadow image in the game is INTERFACE.LVL's CUADRADO (640x100, every span level 0), so
variants of it with other levels are made too. Each case draws the harbour's backdrop, sometimes
a sprite of Jim at a depth below, equal to or above the shadows, and one or two shadows. The
original queue (0x411f80), row reset (0x411f20) and composition (0x412e10, with the shadow merge
0x412640 and the lookup table TABLA1.TBL at renderer+0x66c) are executed in the Unicorn emulator.
Uses the ignored driver local-media/jim-implementation/shadow_driver.cpp, which links the
production renderer. Run with the Python of the local animation environment.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
from pathlib import Path

from compare_opening_pixels import NativePixels as BasePixels, resource
from inspect_tesoro import read_index, read_banks, u16

BANKS = ['JIMHABIZ', 'JIMHABDER', 'JIMQUIETO', 'JIMARRIBA', 'JIMABAJO', 'JIMIZQUIERDA',
         'JIMDERECHA', 'JIMHABAB', 'JIMHABAR']
LEVELS = 4
TABLE = 0x4ea0000
KEEP_GOING = False


class NativePixels(BasePixels):
    rows = 0x4c00000
    # The shadow lists follow the 480 row lists of 2,000 bytes each, where 0x411f20 resets them.
    shadow_rows = 0x4c00000 + 480 * 2000
    output = 0x4e00000
    arena_size = 0xf00000


def packed(data):
    return struct.pack('<I', len(data)) + data


def shadow_variants(game):
    """CUADRADO's bank with every span set to each level."""
    pl = resource(game, 'INTERFACE.LVL')
    banks = read_banks(pl)
    index = next(i for i, b in enumerate(banks) if b['name'] == 'CUADRADO')
    start = 16 if index == 0 else banks[index - 1]['end']
    bank = banks[index]
    image = bank['images'][0]
    variants = []
    for level in range(LEVELS):
        data = bytearray(pl[start:bank['end']])
        base = image['base'] - start
        for y in range(image['height']):
            offset, count = struct.unpack_from('<IH', data, base + 16 + y * 8)
            for j in range(count):
                struct.pack_into('<I', data, base + offset + j * 8, level)
        variants.append(bytes(data))
    return variants


def make_cases(count, seed, images):
    rng = random.Random(seed)
    cases = []
    for _ in range(count):
        bank = rng.randrange(len(BANKS))
        shadows = []
        for _ in range(2):
            shadows.append((rng.randrange(LEVELS), rng.randint(-120, 120), rng.randint(-60, 450),
                            rng.choice((20, 35, 35, 50))))
        cases.append(dict(has_front=int(rng.random() < 0.7), bank=bank, image=rng.randrange(images[bank]),
                          x=rng.randint(-150, 600), y=rng.randint(-150, 450),
                          depth=rng.choice((10, 34, 35, 36, 50, 60)),
                          count=rng.choice((1, 1, 2)), swap=rng.randrange(2), shadows=shadows))
    return cases


def compare(game, driver, count, seed):
    dat = (game / 'TESORO.DAT').read_bytes()
    index = {e['name'].upper(): e for e in read_index((game / 'TESORO.TRE').read_bytes(), dat)}
    anm = []
    for name in BANKS:
        entry = index[name + '.ANM']
        anm.append(dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']])
    images = [u16(a, 8) for a in anm]
    shadow_base = len(anm)
    anm += shadow_variants(game)
    puerto = resource(game, 'PUERTO.LVL')
    anm.append(puerto[16:read_banks(puerto)[0]['end']])
    table_entry = index['TABLA1.TBL']
    table = dat[table_entry['offset'] + 4:table_entry['offset'] + 4 + table_entry['size']]
    cases = make_cases(count, seed, images)

    request = struct.pack('<I', len(anm)) + b''.join(map(packed, anm)) + table
    request += struct.pack('<iI', shadow_base, len(cases))
    for c in cases:
        request += struct.pack('<8i', c['has_front'], c['bank'], c['image'], c['x'], c['y'], c['depth'],
                               c['count'], c['swap'])
        for level, x, y, depth in c['shadows']:
            request += struct.pack('<5i', level, x, y, depth, 0)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True, timeout=600)
    output = process.stdout
    if len(output) != len(cases) * 640 * 480 * 4:
        raise ValueError('Wrong amount of C++ output')

    lvl = b'LVL\1' + struct.pack('<III', 0, len(anm), 0) + b''.join(anm)
    original = NativePixels((game / 'TESORO.EXE').read_bytes(), lvl, resource(game, 'FUENTES.ANM'))
    original.uc.mem_write(TABLE, table)
    original.uc.mem_write(original.renderer + 0x66c, struct.pack('<I', TABLE))
    original.uc.mem_write(original.renderer + 0x67c, b'\0')  # RGB565 display
    back = len(anm) - 1
    hashes, shadowed = [], 0
    for number, c in enumerate(cases):
        original.uc.mem_write(original.output, bytes(640 * 480 * 2))
        original.invoke(0x411f20, original.renderer)
        original.invoke(0x411f80, original.renderer, (0, 0, 200, original.buffers[back, 0], 0))
        if c['has_front']:
            original.invoke(0x411f80, original.renderer,
                            (c['x'], c['y'], c['depth'], original.buffers[c['bank'], c['image']], 0))
        order = list(range(c['count']))
        if c['swap']:
            order.reverse()
        for k in order:
            level, x, y, depth = c['shadows'][k]
            original.invoke(0x411f80, original.renderer,
                            (x, y, depth, original.buffers[shadow_base + level, 0], 0))
        original.invoke(0x412e10, original.renderer, instruction_limit=40000000)
        expected = original.rgba()
        actual = output[number * 640 * 480 * 4:(number + 1) * 640 * 480 * 4]
        if actual != expected:
            differing = sum(actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4] for i in range(640 * 480))
            pixel = next(i for i in range(640 * 480) if actual[i * 4:i * 4 + 4] != expected[i * 4:i * 4 + 4])
            message = (f'case {number} {c}: {differing} pixels differ, first ({pixel % 640},{pixel // 640}) '
                       f'cpp={actual[pixel*4:pixel*4+4].hex()} original={expected[pixel*4:pixel*4+4].hex()}')
            if not KEEP_GOING:
                raise ValueError(message)
            print(message)
            continue
        hashes.append(hashlib.sha256(actual).hexdigest())
    return dict(cases=len(cases), pixels_compared=len(cases) * 640 * 480,
                with_front_sprite=sum(c['has_front'] for c in cases),
                with_two_shadows=sum(c['count'] == 2 for c in cases),
                levels=LEVELS, table_sha256=hashlib.sha256(table).hexdigest(),
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest(), frames=hashes)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cases', type=int, default=40)
    parser.add_argument('--seed', type=int, default=1)
    parser.add_argument('--keep-going', action='store_true')
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    KEEP_GOING = args.keep_going
    report = compare(args.game, args.driver, args.cases, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest()
        for name in ('renderer.cpp', 'renderer.h', 'resources.cpp', 'resources.h', 'scene.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k not in ('frames', 'source_sha256')}))
