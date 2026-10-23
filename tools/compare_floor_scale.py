#!/usr/bin/env python3
"""Compare the production floor scale with the original routine over every room's floor.

Uses a local ignored driver (local-media/jim-implementation/floor_driver.cpp) that builds
each real scene with the production code and asks its floor for the scale at many points.
The original routine (0x414740) runs in the Unicorn emulator on a floor object, bank and
frame that are laid out from the same scene file. Run it with the Python of the local
animation environment, which has Unicorn.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
from pathlib import Path

from compare_scene_native import NativeScene, scenes
from inspect_tesoro import u16
from verify_script_compiler import encode_name

BASE = 0x700000


def signed32(value):
    return value - (1 << 32) if value & 0x80000000 else value


def floors(game):
    """Yield (scene name, payload, floor description or None) for every scene."""
    for name, payload, banks, objects in scenes(game):
        floor = None
        rooms = [o for o in objects if o['kind'] == 1]
        if rooms:
            parameters = bytes.fromhex(rooms[-1]['parameters_hex'])
            floor_name = parameters[:12].split(b'\0')[0].decode('cp850')
            if floor_name:
                found = [o for o in objects if encode_name(o['name']) == encode_name(floor_name)]
                if found and found[0]['kind'] == 7:
                    record = found[0]
                    params = bytes.fromhex(record['parameters_hex'])
                    bank = record['extra_bytes'][0]
                    # Locate the frame's extra data in the scene file.
                    at = 16
                    for b in range(bank):
                        at = banks[b]['end']
                    at += 28
                    for _ in range(params[0]):
                        at += 24 + u16(payload, at + 22)
                    frame_type, extra_size = u16(payload, at + 20), u16(payload, at + 22)
                    extra = payload[at + 24:at + 24 + extra_size]
                    if frame_type == 4:
                        count = u16(extra, 0)
                        points = [struct.unpack_from('<hhH', extra, 8 + 6 * i) for i in range(count)]
                        floor = dict(x=record['x'], y=record['y'], points=points, frame=params[0])
        yield name, payload, floor


def original_scale(native, floor, x, y):
    frames = BASE + 0x400
    points, scales = BASE + 0x2000, BASE + 0x3000
    native.uc.mem_write(BASE, bytes(0x4000))
    native.uc.mem_write(BASE + 0x38, struct.pack('<I', BASE + 0x100))
    native.uc.mem_write(BASE + 0x4c, struct.pack('<ii', floor['x'], floor['y']))
    native.uc.mem_write(BASE + 0x100 + 0x1c, struct.pack('<I', frames))
    native.uc.mem_write(BASE + 0x100 + 0x30, struct.pack('<H', floor['frame']))
    frame = frames + floor['frame'] * 0x24
    native.uc.mem_write(frame + 0x14, struct.pack('<I', scales))
    native.uc.mem_write(frame + 0x1c, struct.pack('<H', len(floor['points'])))
    native.uc.mem_write(frame + 0x20, struct.pack('<I', points))
    native.uc.mem_write(points, b''.join(struct.pack('<hh', p[0], p[1]) for p in floor['points']))
    native.uc.mem_write(scales, b''.join(struct.pack('<H', p[2]) for p in floor['points']))
    return signed32(native.invoke(0x414740, BASE, (x, y)))


def compare(game, driver, per_scene, seed):
    rng = random.Random(seed)
    items = list(floors(game))
    request = struct.pack('<I', len(items))
    probes = []
    for name, payload, floor in items:
        if floor:
            xs = [p[0] for p in floor['points']]
            ys = [p[1] for p in floor['points']]
            candidates = [(floor['x'] + px, floor['y'] + py) for px, py, _ in floor['points']]
            candidates += [(floor['x'] + rng.randint(min(xs) - 60, max(xs) + 60),
                            floor['y'] + rng.randint(min(ys) - 60, max(ys) + 60)) for _ in range(per_scene)]
        else:
            candidates = [(rng.randint(-100, 700), rng.randint(-100, 500)) for _ in range(4)]
        probes.append(candidates)
        request += struct.pack('<I', len(payload)) + payload + struct.pack('<I', len(candidates))
        request += b''.join(struct.pack('<ii', x, y) for x, y in candidates)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True, timeout=600)
    output = process.stdout
    results = struct.unpack('<%di' % (len(output) // 4), output)
    if len(results) != sum(len(p) for p in probes):
        raise ValueError('Wrong amount of C++ output')

    native = NativeScene((game / 'TESORO.EXE').read_bytes())
    at, compared, skipped, with_floor, no_floor = 0, 0, [], 0, 0
    for (name, payload, floor), candidates in zip(items, probes):
        if not floor:
            no_floor += 1
            at += len(candidates)
            continue
        with_floor += 1
        for x, y in candidates:
            expected = None
            try:
                expected = original_scale(native, floor, x, y)
            except Exception as error:  # the original divides by zero on a vertical edge
                skipped.append(dict(scene=name, point=[x, y], error=str(error)))
            if expected is not None and expected != results[at]:
                raise ValueError(f'{name} ({x},{y}): cpp={results[at]} original={expected}')
            compared += expected is not None
            at += 1
    return dict(scenes=len(items), scenes_with_floor=with_floor, scenes_without_floor=no_floor,
                points_compared=compared, points_skipped=len(skipped), skipped=skipped[:10],
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--points', type=int, default=200)
    parser.add_argument('--seed', type=int, default=1)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver, args.points, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest()
        for name in ('scene.cpp', 'scene.h', 'resources.cpp', 'resources.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k not in ('skipped', 'source_sha256')}))
