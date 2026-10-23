#!/usr/bin/env python3
"""Compare Jim's production walk with the original start-walk and update routines.

Uses a local ignored driver (local-media/jim-implementation/walk_driver.cpp) that runs the
production JimObject on the real harbour scene and Jim's real animation banks. The original
routines for starting a walk (0x407600) and for one step of it (0x407790), with the animation
advance (0x401860), the animation start (0x401740) and the floor scale (0x414740), run in
the Unicorn emulator on a Jim object, banks and floor laid out from the same files. Object
list and scene bookkeeping are stubbed. Each step compares the committed position, scale,
animation frame and whether he has arrived, over many walks and frame times.

Run it with the Python of the local animation environment, which has Unicorn.
"""
import argparse
import hashlib
import json
import random
import struct
import subprocess
from pathlib import Path

from compare_floor_scale import floors
from compare_scene_native import NativeScene
from inspect_tesoro import read_index, read_banks, u16
from unicorn import UC_HOOK_CODE

BANKS = ['JIMHABIZ', 'JIMHABDER', 'JIMQUIETO', 'JIMARRIBA', 'JIMABAJO', 'JIMIZQUIERDA',
         'JIMDERECHA', 'JIMHABAB', 'JIMHABAR']
WALK = {1: 'JIMARRIBA', 2: 'JIMABAJO', 3: 'JIMIZQUIERDA', 4: 'JIMDERECHA'}
JIM_VTABLE = 0x428b70


def signed32(value):
    return value - (1 << 32) if value & 0x80000000 else value


class NativeJim(NativeScene):
    J, SCENE, ROOM, FLOOR, FLOOR_BANK, FLOOR_FRAMES = 0x600000, 0x600400, 0x600500, 0x600600, 0x600700, 0x600800
    ARRAY, WAITER, WAITER_VTABLE, STUB, IDLE = 0x600900, 0x600a00, 0x600b00, 0x600c00, 0x6f0000
    POINTS, SCALES = 0x601000, 0x601400

    def __init__(self, exe):
        super().__init__(exe)
        self.moved = self.arrived = False
        for address, arguments, value in ((0x40fb20, 4, 1), (0x40fa10, 8, 0)):
            self.uc.hook_add(UC_HOOK_CODE, lambda uc, a, s, d, n=arguments, v=value: self.ret(v, n),
                             begin=address, end=address)
        self.uc.hook_add(UC_HOOK_CODE, self.scene_moved, begin=0x408440, end=0x408440)
        self.uc.hook_add(UC_HOOK_CODE, self.resumed, begin=self.STUB, end=self.STUB)

    def scene_moved(self, uc, address, size, data):
        self.moved = True
        self.ret(0, 4)

    def resumed(self, uc, address, size, data):
        self.arrived = True
        self.ret(3, 0)  # a script that yields returns 3, which ends Jim's update

    def lay_out(self, banks, floor):
        w = self.uc.mem_write
        w(self.J, bytes(0x200))
        w(self.J, struct.pack('<I', JIM_VTABLE))
        w(self.SCENE, bytes(0x100))
        w(self.SCENE + 0x90, struct.pack('<I', self.ROOM))
        w(self.ROOM + 0x78, struct.pack('<I', self.FLOOR))
        # The floor: an object with a bank whose current frame holds the outline.
        w(self.FLOOR, bytes(0x100))
        w(self.FLOOR + 0x38, struct.pack('<I', self.FLOOR_BANK))
        w(self.FLOOR + 0x4c, struct.pack('<ii', floor['x'], floor['y']))
        w(self.FLOOR_BANK, bytes(0x100))
        w(self.FLOOR_BANK + 0x1c, struct.pack('<I', self.FLOOR_FRAMES))
        w(self.FLOOR_BANK + 0x30, struct.pack('<H', floor['frame']))
        frame = self.FLOOR_FRAMES + floor['frame'] * 0x24
        w(frame + 0x14, struct.pack('<I', self.SCALES))
        w(frame + 0x1c, struct.pack('<H', len(floor['points'])))
        w(frame + 0x20, struct.pack('<I', self.POINTS))
        w(self.POINTS, b''.join(struct.pack('<hh', p[0], p[1]) for p in floor['points']))
        w(self.SCALES, b''.join(struct.pack('<H', p[2]) for p in floor['points']))
        # The walking banks, as the animation loader leaves them.
        w(self.ARRAY, bytes(0x40))
        for direction, name in WALK.items():
            frames, interpolate = banks[name]
            address = 0x610000 + direction * 0x10000
            w(address, bytes(64))
            w(address + 0x18, struct.pack('<I', 8 if interpolate else 0))
            w(address + 0x1c, struct.pack('<I', address + 0x1000))
            w(address + 0x22, struct.pack('<H', len(frames)))
            for i, (header, image) in enumerate(frames):
                raw = bytearray(36)
                raw[:24] = header
                struct.pack_into('<I', raw, 24, 0 if image < 0 else 1)
                w(address + 0x1000 + i * 36, bytes(raw))
            w(self.ARRAY + direction * 4, struct.pack('<I', address))
        w(self.IDLE, bytes(64))
        w(self.IDLE + 0x22, struct.pack('<H', 1))
        w(self.IDLE + 0x1c, struct.pack('<I', self.IDLE + 0x1000))
        w(self.IDLE + 0x1000, bytes(36))
        w(self.WAITER, struct.pack('<I', self.WAITER_VTABLE))
        w(self.WAITER_VTABLE + 0x34, struct.pack('<I', self.STUB))
        w(self.J + 0x38, struct.pack('<I', self.IDLE))
        w(self.J + 0x40, struct.pack('<I', self.SCENE))
        w(self.J + 0xd8, struct.pack('<I', self.ARRAY))
        w(0x4420fc, struct.pack('<I', self.J))  # the global that holds Jim

    def scale(self, x, y):
        return signed32(self.invoke(0x414740, self.FLOOR, (x, y)))

    def place(self, x, y, scale):
        self.uc.mem_write(self.J + 0x4c, struct.pack('<iiii', x, y, x, y))  # committed and pending
        self.uc.mem_write(self.J + 0xfc, struct.pack('<i', scale))
        self.moved = self.arrived = False

    def start(self, x, y, now):
        self.invoke(0x407600, self.J, (x, y, self.WAITER, 0), now=now)

    def step(self, now):
        self.moved = False
        self.invoke(0x407790, self.J, now=now, instruction_limit=2000000)
        read = lambda at, fmt: struct.unpack(fmt, bytes(self.uc.mem_read(self.J + at, struct.calcsize(fmt))))
        if self.moved and not self.arrived:  # the scene commits positions at the end of a frame
            self.uc.mem_write(self.J + 0x4c, bytes(self.uc.mem_read(self.J + 0x54, 8)))
        x, y = read(0x4c, '<ii')
        (scale,) = read(0xfc, '<i')
        (bank,) = read(0x38, '<I')
        (frame,) = struct.unpack('<H', bytes(self.uc.mem_read(bank + 0x30, 2)))
        return x, y, scale, 0 if self.arrived else 1, frame


def load_banks(game):
    dat = (game / 'TESORO.DAT').read_bytes()
    index = {e['name'].upper(): e for e in read_index((game / 'TESORO.TRE').read_bytes(), dat)}
    raw, native = {}, {}
    for name in BANKS:
        entry = index[name + '.ANM']
        data = dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]
        raw[name] = data
        frames, at = [], 28
        for _ in range(u16(data, 4)):
            header = bytearray(data[at:at + 24])
            image = struct.unpack_from('<h', header, 10)[0]
            frames.append([header, image])
            at += 24 + u16(data, at + 22)
        # The loader makes movement relative to the first frame and clears the first.
        ox, oy = struct.unpack_from('<hh', frames[0][0], 4)
        for header, _ in frames:
            dx, dy = struct.unpack_from('<hh', header, 4)
            struct.pack_into('<hh', header, 4, (dx - ox + 32768) % 65536 - 32768, (dy - oy + 32768) % 65536 - 32768)
        native[name] = (frames, data[24] == 1)
    return raw, native


def compare(game, driver, count, seed):
    rng = random.Random(seed)
    raw, native_banks = load_banks(game)
    puerto = next(item for item in floors(game) if item[0].lower() == 'puerto.lvl')
    scene, floor = puerto[1], puerto[2]
    native = NativeJim((game / 'TESORO.EXE').read_bytes())
    native.lay_out(native_banks, floor)

    xs = [floor['x'] + p[0] for p in floor['points']]
    ys = [floor['y'] + p[1] for p in floor['points']]
    scenarios = []
    while len(scenarios) < count:
        sx, sy = rng.randint(min(xs), max(xs)), rng.randint(min(ys), max(ys))
        tx, ty = sx + rng.randint(-260, 260), sy + rng.randint(-120, 120)
        if rng.random() < 0.25:
            tx, ty = sx + rng.randint(-60, 60), sy + rng.randint(-250, 250)
        if (tx, ty) == (sx, sy):
            continue
        try:
            native.scale(sx, sy)
        except Exception:
            continue  # the original divides by zero on a vertical edge
        gaps = [rng.choice((16, 16, 17, 16, 33, 50, 8, 100)) for _ in range(500)]
        scenarios.append((sx, sy, tx, ty, rng.randint(0, 100000), gaps))

    request = b''.join(struct.pack('<I', len(raw[n])) + raw[n] for n in BANKS)
    request += struct.pack('<I', len(scene)) + scene + struct.pack('<I', len(scenarios))
    for sx, sy, tx, ty, start, gaps in scenarios:
        request += struct.pack('<iiiiII', sx, sy, tx, ty, start, len(gaps)) + struct.pack('<%dI' % len(gaps), *gaps)
    process = subprocess.run([str(driver.resolve())], input=request, capture_output=True, check=True, timeout=1200)
    results = struct.unpack('<%di' % (len(process.stdout) // 4), process.stdout)

    at, steps, arrivals, directions, errors, stuck = 0, 0, 0, {}, [], []
    for number, (sx, sy, tx, ty, start, gaps) in enumerate(scenarios):
        try:
            native.lay_out(native_banks, floor)  # every walk starts from a fresh Jim, as in the driver
            scale = native.scale(sx, sy)
            native.place(sx, sy, scale)
            native.start(tx, ty, start)
            direction = struct.unpack('<I', bytes(native.uc.mem_read(native.J + 0x10c, 4)))[0]
            directions[direction] = directions.get(direction, 0) + 1
            now = start
            for index, gap in enumerate(gaps):
                now += gap
                expected = native.step(now)
                actual = tuple(results[(at + index) * 5:(at + index) * 5 + 5])
                if expected != actual:
                    raise ValueError(f'walk {number} {(sx, sy, tx, ty)} step {index}: cpp={actual} original={expected}')
                steps += 1
                if expected[3] == 0:
                    arrivals += 1
                    break
            else:
                # Both stay in step but never arrive, for instance where the floor gives him no size.
                stuck.append(dict(walk=number, start=[sx, sy], target=[tx, ty], final=list(expected)))
        except ValueError:
            raise
        except Exception as error:
            errors.append(dict(walk=number, error=str(error)))
        at += len(gaps)
    return dict(walks=len(scenarios), steps_compared=steps, arrivals=arrivals, never_arrive=stuck[:10], never_arrive_count=len(stuck), directions=directions,
                native_errors=errors[:10], native_error_count=len(errors),
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--walks', type=int, default=100)
    parser.add_argument('--seed', type=int, default=1)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver, args.walks, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest()
        for name in ('jim.cpp', 'jim.h', 'scene.cpp', 'animation.cpp', 'resources.cpp')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'source_sha256'}))
