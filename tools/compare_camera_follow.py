#!/usr/bin/env python3
"""Compare the production camera (follow, glide, clamp) with the original routines.

Uses a local ignored driver (local-media/jim-implementation/camera_driver.cpp) that runs the
production SceneCamera. The original camera update (0x401fc0), set-position (0x4021a0, which
starts a timed move or clamps at once, and calls the clamp 0x402120) and set-bounds (0x402200)
run in the Unicorn emulator on a camera laid out from the same inputs; the actor is an object
whose x (+0x4c) the test changes. Each scenario has random bounds, a start position, an
actor that walks, frame times that sometimes jump past a whole glide, and timed moves.
Compared after every step: the clamped camera position, and the group view for an update.

Run it with the Python of the local animation environment, which has Unicorn.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
from pathlib import Path

from compare_scene_native import NativeScene

ACTOR = 0x285800


def scenario(rng):
    left = rng.randint(-300, 300)
    right = left + rng.choice([640, 641, 700, 960, 1280, 2000, 2560])
    bottom = rng.randint(-100, 100)
    top = bottom + rng.choice([480, 500, 600, 900])
    follow = rng.random() < 0.85
    ops = [(0, rng.randint(left, right), rng.randint(bottom, top), 0, 0)]
    now = rng.randint(0, 5000)
    actor = ops[0][1]
    for _ in range(rng.randint(20, 60)):
        kind = rng.random()
        if kind < 0.1:
            ops.append((0, rng.randint(left - 200, right + 200), rng.randint(bottom - 200, top + 200),
                        now, rng.choice([0, 1, 300, 750, 2000])))
        else:
            now += rng.choice([0, 16, 17, 33, 100, 250, 400, 749, 750, 751, 800, 1500])
            if rng.random() < 0.6:
                actor += rng.randint(-30, 30)
            elif rng.random() < 0.3:
                actor = rng.randint(left - 100, right + 100)
            ops.append((1, actor, now))
    return (left, bottom, right, top), follow, ops


def original(native, bounds, follow, ops):
    left, bottom, right, top = bounds
    native.uc.mem_write(native.camera, bytes(64))
    native.uc.mem_write(ACTOR, bytes(0x80))
    # Engine bounds are left/top/right/bottom in the order of the original's +1c..+28.
    native.invoke(0x402200, native.camera, (left, top, right, bottom))
    if follow:
        native.invoke(0x402230, native.camera, (ACTOR,))
    rows = []
    for op in ops:
        if op[0] == 0:
            _, x, y, now, duration = op
            native.invoke(0x4021a0, native.camera, (x, y, duration), now=now)
            rows.append(list(struct.unpack('<2i', native.uc.mem_read(native.camera + 0x2c, 8))) + [None, None])
        else:
            _, actor, now = op
            native.uc.mem_write(ACTOR + 0x4c, struct.pack('<i', actor))
            native.invoke(0x401fc0, native.camera, (300,), now=now)
            rows.append(list(struct.unpack('<2i', native.uc.mem_read(native.camera + 0x2c, 8))) +
                        list(struct.unpack('<2i', native.uc.mem_read(native.camera + 0x34, 8))))
    return rows


def compare(game, driver, count, seed):
    rng = random.Random(seed)
    scenarios = [scenario(rng) for _ in range(count)]
    request = struct.pack('<I', len(scenarios))
    for (left, bottom, right, top), follow, ops in scenarios:
        request += struct.pack('<5I', left & 0xffffffff, top & 0xffffffff, right & 0xffffffff,
                               bottom & 0xffffffff, int(follow)) + struct.pack('<I', len(ops))
        for op in ops:
            if op[0] == 0:
                request += struct.pack('<IiiII', 0, op[1], op[2], op[3], op[4])
            else:
                request += struct.pack('<IiI', 1, op[1], op[2])
    output = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True,
                            timeout=600).stdout
    produced = struct.unpack('<%di' % (len(output) // 4), output)
    native = NativeScene((game / 'TESORO.EXE').read_bytes())
    at = steps = glides = follows = 0
    for bounds, follow, ops in scenarios:
        rows = original(native, bounds, follow, ops)
        previous = None
        for op, row in zip(ops, rows):
            got = list(produced[at:at + 4])
            at += 4
            if op[0] == 0:
                got[2:] = [None, None]
            if got != row:
                raise ValueError(f'bounds={bounds} follow={follow} op={op}: cpp={got} original={row}')
            steps += 1
            if op[0] == 1 and previous is not None and row[:2] != previous:
                follows += 1
            previous = row[:2]
        glides += sum(1 for op in ops if op[0] == 0 and op[4])
    if at != len(produced):
        raise ValueError('Wrong amount of C++ output')
    return dict(scenarios=count, steps=steps, timed_moves_started=glides, moving_updates=follows,
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--scenarios', type=int, default=2000)
    parser.add_argument('--seed', type=int, default=1)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver, args.scenarios, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest() for name in ('scene.cpp', 'scene.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'source_sha256'}))
