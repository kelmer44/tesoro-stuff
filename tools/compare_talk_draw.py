#!/usr/bin/env python3
"""Compare the sprites of a talking character with the original draw methods.

Uses a local ignored driver (local-media/jim-implementation/talk_driver.cpp) that builds the real
office scene with the production code and asks the captain for the sprites that it submits, for
many camera positions, while it stands in its first bank and after it has talked (the bank that
shows it talking, on the closed-mouth frame). The original character draw method (0x4102e0) runs
in the Unicorn emulator on a captain constructed by the original constructors (0x40fec0 and the
interactive base 0x40ed10) from the same scene file, with the sprite queue (0x411f80) captured.
Positions, depths and images, in submission order, must match.

Run it with the Python of the local animation environment, which has Unicorn.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
from pathlib import Path

from unicorn import UC_HOOK_CODE
from compare_scene_native import NativeScene, scenes
from inspect_tesoro import u16

CAPTAIN_VTABLE = 0x42b280


class NativeCharacter(NativeScene):
    def __init__(self, exe):
        super().__init__(exe)
        self.captured = []
        # Text lookups need the game's text tables, which are not loaded here.
        self.uc.hook_add(UC_HOOK_CODE, lambda uc, a, s, d: self.ret(0, 0), begin=0x413940, end=0x413940)

    def submit(self, uc, address, size, data):
        import struct as st
        sp = uc.reg_read(__import__('unicorn.x86_const', fromlist=['UC_X86_REG_ESP']).UC_X86_REG_ESP)
        x, y, depth, image, unused = st.unpack('<iiIII', uc.mem_read(sp + 4, 20))
        self.captured.append((x, y, depth & 0xffff, tuple(self.images[image])))
        self.ret(arguments=20)

    def build_captain(self, record, depth_base):
        self.captured = []
        self.heap = 0x290000
        self.uc.mem_write(self.object, bytes(0x200))
        raw = struct.pack('<iihBB12sII', record['x'], record['y'], (record['field_08'] + 32768) % 65536 - 32768,
                          record['field_10'], len(record['extra_bytes']),
                          record['name'].encode('cp850'), record['kind'],
                          # The factory takes the four leading bytes (colour and width) off the size of a character's
                          # parameters before it builds the character (0x409fa2).
                          len(bytes.fromhex(record['parameters_hex'])) - 4)
        self.uc.mem_write(self.record, raw)
        self.uc.mem_write(self.parameters, bytes.fromhex(record['parameters_hex']) + bytes(record['extra_bytes']))
        self.uc.mem_write(self.group, bytes(16))
        self.uc.mem_write(self.group + 8, struct.pack('<H', depth_base))
        self.invoke(0x40fec0, self.object, (self.record, self.group, self.bank_table, self.parameters, 4))
        self.uc.mem_write(self.object, struct.pack('<I', CAPTAIN_VTABLE))
        self.uc.mem_write(self.object + 0x40, struct.pack('<I', self.scene))
        self.uc.mem_write(self.object + 0x3c, struct.pack('<I', self.group))

    def talk_state(self):
        """What the original leaves after a character has talked: its first talking bank, on frame 1."""
        read = lambda at: struct.unpack('<I', bytes(self.uc.mem_read(self.object + at, 4)))[0]
        array, count = read(0xb0), read(0xc0)
        if not count:
            raise ValueError('The captain has no talking bank')
        bank = struct.unpack('<I', bytes(self.uc.mem_read(read(0x34) + array * 4, 4)))[0]
        self.uc.mem_write(self.object + 0x32, bytes([array & 255]))
        self.uc.mem_write(self.object + 0x38, struct.pack('<I', bank))
        self.invoke(0x401740, bank, (1, 1, 0, 0))
        self.uc.mem_write(self.object + 0xc, b'\1')

    def draw(self, view):
        self.captured = []
        self.uc.mem_write(self.camera, bytes(0x80))
        self.uc.mem_write(self.camera + 0x34, struct.pack('<ii', view[0], view[1]))
        self.uc.mem_write(self.scene + 0x80, struct.pack('<I', self.camera))
        self.invoke(0x4102e0, self.object, (self.camera,), instruction_limit=2000000)
        return list(self.captured)


def compare(game, driver, count, seed):
    rng = random.Random(seed)
    office = next(item for item in scenes(game) if item[0].lower() == 'despacho.lvl')
    _, payload, banks, objects = office
    captain = next(o for o in objects if o['name'] == 'CAPITAN')
    depth_base = 0
    # The group's depth base is the sum of the spans of the groups before it (the office has one).
    cases = [(rng.randint(-400, 900), rng.randint(-300, 600), rng.randint(0, 1)) for _ in range(count)]
    request = struct.pack('<I', len(payload)) + payload + struct.pack('<I', len(cases))
    for cx, cy, mode in cases:
        request += struct.pack('<iiI', cx, cy, mode)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True, timeout=300)
    lines = process.stdout.decode().splitlines()

    native = NativeCharacter((game / 'TESORO.EXE').read_bytes())
    native.load_scene(banks)
    at, compared, with_back = 0, 0, 0
    for number, (cx, cy, mode) in enumerate(cases):
        head = lines[at].split()
        assert head[0] == 'C' and int(head[2]) == number
        view = (int(head[3]), int(head[4]))
        n = int(head[5])
        actual = [tuple(map(int, lines[at + 1 + i].split()[1:])) for i in range(n)]
        at += 1 + n
        native.build_captain(captain, depth_base)
        if mode == 1:
            native.talk_state()
        expected = native.draw(view)
        expected = [(x, y, depth, bank, image) for x, y, depth, (bank, image) in expected]
        if expected != actual:
            raise ValueError(f'case {number} (camera {cx},{cy} mode {mode}): cpp={actual} original={expected}')
        compared += len(expected)
        with_back += mode == 1
    return dict(cases=len(cases), sprites_compared=compared, talking_cases=with_back,
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
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
        for name in ('scene.cpp', 'scene.h', 'resources.cpp', 'animation.cpp')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'source_sha256'}))
