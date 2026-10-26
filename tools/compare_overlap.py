#!/usr/bin/env python3
"""Compare the production overlap geometry with the original routines.

Uses a local ignored driver (local-media/jim-implementation/overlap_driver.cpp) that links the
production code (engines/tesoro/overlap.cpp). The original routines run in the Unicorn
emulator on the same inputs: the swept box test (0x409280), point in polygon (0x409560),
polygon against polygon (0x409790), the clip (0x409c10) and the pair test (0x408c50), the last
one with a small stand-in for the scene's queue from which the queued pair is read back.
Polygons are every type 2 and type 4 frame of the game's scenes (as the engine reads them),
translated and clipped at random, plus random outlines. Cases where the original traps
(a zero divisor, or a loop that does not end) are skipped and counted.

Run it with the Python of the local animation environment, which has Unicorn.
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

HDR_A, PTS_A = 0x900000, 0x900400
HDR_B, PTS_B = 0x901000, 0x901400
HDR_O, PTS_O = 0x902000, 0x902400
OBJ_A, OBJ_B = 0x910000, 0x911000
BANK_A, BANK_B = 0x912000, 0x912100
FRAMES_A, FRAMES_B = 0x913000, 0x914000
SCENE, QUEUE, POOL, NODES = 0x920000, 0x921000, 0x922000, 0x923000
POINTS_A2, POINTS_B2 = 0x930400, 0x931400  # points of the bodies, with room around them


def s32(v):
    return v - (1 << 32) if v & 0x80000000 else v


def s16(v):
    v &= 0xffff
    return v - 0x10000 if v & 0x8000 else v


def split_of(points):
    """The loader's rule: the last point of the first run that does not go back in x."""
    split = 0
    for last in range(len(points) - 1):
        if split < len(points) - 1 and not points[split + 1][0] < points[split][0]:
            split += 1
        else:
            break
    return split if points else 0


def real_polygons(game):
    """Every point list of type 2 and type 4 frames, as the engine reads them."""
    found = []
    for name, payload, banks, objects in scenes(game):
        start = 16
        for bank in banks:
            at = start + 28
            for _ in range(bank['frames']):
                ftype, extra = u16(payload, at + 20), u16(payload, at + 22)
                if ftype in (2, 4) and extra >= 2:
                    entry, first = (4, 6) if ftype == 2 else (6, 8)
                    count = u16(payload, at + 24)
                    points = []
                    for p in range(count):
                        present = first + (p + 1) * entry <= extra
                        x = struct.unpack_from('<h', payload, at + 24 + first + p * entry)[0] if present else 0
                        y = struct.unpack_from('<h', payload, at + 26 + first + p * entry)[0] if present else 0
                        points.append((x, y))
                    if 0 < len(points) <= 90:
                        found.append(points)
                at += 24 + extra
            start = bank['end']
    return found


def random_polygon(rng):
    """A closed outline: a run left to right, then back, ending where it began."""
    n = rng.randint(2, 7)
    xs = sorted(rng.sample(range(-60, 60), n))
    top = [(x, rng.randint(-50, 50)) for x in xs]
    back = [(x, rng.randint(-50, 50)) for x in reversed(xs[1:-1])]
    return top + back + [top[0]]


def translate(points, dx, dy):
    return [(s16(x + dx), s16(y + dy)) for x, y in points]


class Emulated:
    def __init__(self, exe):
        self.native = NativeScene(exe)
        self.uc = self.native.uc
        self.traps = 0
        self.undefined = 0
        self.fill = 0
        self.scene = 0x920000

    def poly(self, header, base, points):
        self.uc.mem_write(base - 0x400, bytes([self.fill]) * 0x2400)
        raw = b''.join(struct.pack('<hh', *p) for p in points)
        self.uc.mem_write(base, raw)
        self.uc.mem_write(header, struct.pack('<HHI', len(points), split_of(points), base))

    def call(self, target, args, this=0):
        # The routines build the clipped outline in a local array and may read past it, so the stack
        # below the call is filled with the same pattern as the memory around the points.
        self.uc.mem_write(self.native.stack_top - 0x4000, bytes([self.fill]) * 0x3f00)
        try:
            return self.native.invoke(target, this, args)
        except Exception:  # a divide error, or a loop that never ends
            self.traps += 1
            return None

    def robust(self, run):
        """Runs an original routine with different bytes around the points. The result counts only
        if it does not depend on them: the original reads past the end of a point list on some inputs."""
        results = []
        # The scene's address is varied too: the clip buffer sits next to the saved pointer to it.
        for fill, scene in ((0x00, 0x920000), (0x40, 0x944048), (0xc0, 0x9bc0c0)):
            self.fill, self.scene = fill, scene
            results.append(run())
        self.fill, self.scene = 0, 0x920000
        if any(r is None for r in results):
            return None
        if any(r != results[0] for r in results):
            self.undefined += 1
            return None
        return results[0]

    def box(self, a, b, dx, dy):
        r = self.call(0x409280, [*a, *b, dx, dy])
        return None if r is None else s32(r)

    def inside(self, points, x, y):
        def run():
            self.poly(HDR_A, PTS_A, points)
            r = self.call(0x409560, [HDR_A, x, y])
            return None if r is None else r & 0xff
        return self.robust(run)

    def cross(self, a, b):
        def run():
            self.poly(HDR_A, PTS_A, a)
            self.poly(HDR_B, PTS_B, b)
            r = self.call(0x409790, [HDR_A, HDR_B])
            return None if r is None else r & 0xff
        return self.robust(run)

    def clip(self, points, left, right, clip_left, clip_right):
        return self.robust(lambda: self._clip(points, left, right, clip_left, clip_right))

    def _clip(self, points, left, right, clip_left, clip_right):
        self.poly(HDR_A, PTS_A, points)
        self.uc.mem_write(PTS_O - 0x400, bytes(0x2400))
        self.uc.mem_write(HDR_O, struct.pack('<HHI', 0, 0, PTS_O))
        r = self.call(0x409c10, [HDR_A, left, 0, right, 0, HDR_O, clip_left, 0, clip_right])
        if r is None:
            return None
        count, split = struct.unpack('<HH', self.uc.mem_read(HDR_O, 4))
        pts = struct.unpack('<%dh' % (count * 2), self.uc.mem_read(PTS_O, count * 4))
        return [count, split] + list(pts)

    def body(self, obj, bank, frames, pts_base, header_points, body):
        self.uc.mem_write(obj, bytes(0x100))
        box = body['box']
        self.uc.mem_write(obj + 0x10, struct.pack('<4i', *box))
        self.uc.mem_write(obj + 0x20, struct.pack('<4i', body['from'][0], body['from'][1], body['to'][0], body['to'][1]))
        self.uc.mem_write(obj + 0x38, struct.pack('<I', bank))
        self.uc.mem_write(obj + 0x4c, struct.pack('<ii', *body['pos']))
        self.uc.mem_write(obj + 0x5e, struct.pack('<H', body['depth']))
        self.uc.mem_write(bank, bytes(0x40))
        self.uc.mem_write(bank + 0x1c, struct.pack('<I', frames))
        self.uc.mem_write(frames, bytes(0x100))
        self.poly(frames + 0x1c, pts_base, body['poly'])

    def pair(self, a, b):
        return self.robust(lambda: self._pair(a, b))

    def _pair(self, a, b):
        self.body(OBJ_A, BANK_A, FRAMES_A, POINTS_A2, None, a)
        self.body(OBJ_B, BANK_B, FRAMES_B, POINTS_B2, None, b)
        jim = OBJ_A if a['jim'] else OBJ_B if b['jim'] else 0
        self.uc.mem_write(0x4420fc, struct.pack('<I', jim))
        scene, queue, pool, nodes = self.scene, self.scene + 0x1000, self.scene + 0x2000, self.scene + 0x3000
        self.uc.mem_write(scene, bytes(0x100))
        self.uc.mem_write(scene + 0x78, struct.pack('<I', queue))
        self.uc.mem_write(queue, bytes(0x100))
        self.uc.mem_write(queue, struct.pack('<I', pool))
        self.uc.mem_write(pool, bytes(0x100))
        self.uc.mem_write(pool + 0x14, struct.pack('<I', nodes))
        self.uc.mem_write(nodes, bytes(0x100))
        r = self.call(0x408c50, [OBJ_A, OBJ_B], this=scene)
        if r is None:
            return None
        node = struct.unpack('<I', self.uc.mem_read(queue + 4, 4))[0]
        if node == 0:
            return [0, 0, 0]
        pa, pb, t, prio = struct.unpack('<IIiI', self.uc.mem_read(node + 4, 16))
        assert pa == OBJ_A and pb == OBJ_B
        return [1, t, prio & 0xffff]


def make_body(rng, poly, near):
    xs = [p[0] for p in poly] or [0]
    ys = [p[1] for p in poly] or [0]
    slack = rng.choice([0, 0, 2, 10])
    box = [min(xs) - slack, max(ys) + slack, max(xs) + slack, min(ys) - slack]
    if rng.random() < 0.1:
        box = [rng.randint(-80, 0), rng.randint(0, 80), rng.randint(0, 80), rng.randint(-80, 0)]
    fx, fy = near[0] + rng.randint(-120, 120), near[1] + rng.randint(-120, 120)
    move = rng.choice([(0, 0), (0, 0), (rng.randint(-60, 60), rng.randint(-60, 60)),
                       (rng.randint(-8, 8), 0), (0, rng.randint(-8, 8)), (rng.randint(-200, 200), rng.randint(-200, 200))])
    return dict(box=box, **{'from': (fx, fy), 'to': (fx + move[0], fy + move[1])},
                pos=(fx + move[0], fy + move[1]), depth=rng.randint(0, 300), jim=False, poly=poly)


def compare(game, driver, cases, seed):
    rng = random.Random(seed)
    real = real_polygons(game)
    emu = Emulated((game / 'TESORO.EXE').read_bytes())

    def pick():
        r = rng.random()
        if r < 0.55:
            return rng.choice(real)
        if r < 0.9:
            return random_polygon(rng)
        return rng.choice([[], [(rng.randint(-20, 20), rng.randint(-20, 20))]])

    request, expected, kinds, inputs = [], [], [], []

    def add(kind, ints, result):
        kinds.append(kind)
        inputs.append(ints)
        request.extend(ints)
        expected.append(result)

    def poly_ints(points):
        out = [len(points), -1]
        for x, y in points:
            out += [x, y]
        return out

    for _ in range(cases):
        # Boxes.
        a = [rng.randint(-100, 100) for _ in range(4)]
        a[2] = a[0] + rng.randint(0, 80)
        a[1], a[3] = rng.randint(0, 100), rng.randint(-100, 0)
        b = [rng.randint(-100, 100) for _ in range(4)]
        b[2] = b[0] + rng.randint(0, 80)
        b[1], b[3] = rng.randint(0, 100), rng.randint(-100, 0)
        dx, dy = rng.choice([(0, 0), (rng.randint(-150, 150), rng.randint(-150, 150)),
                             (rng.randint(-150, 150), 0), (0, rng.randint(-150, 150))])
        add('box', [0, *a, *b, dx, dy], emu.box(a, b, dx, dy))
        # A point against an outline: on vertices, on edges, level with a vertex, anywhere.
        poly = pick()
        if poly:
            vx, vy = rng.choice(poly)
            x, y = rng.choice([(vx, vy), (vx + rng.randint(-3, 3), vy), (rng.randint(-100, 100), vy),
                               (rng.randint(-100, 100), rng.randint(-100, 100))])
            add('inside', [1, *poly_ints(poly), x, y], emu.inside(poly, x, y))
        # Two outlines, one moved against the other.
        p, q = pick(), pick()
        if p and q:
            q = translate(q, rng.randint(-60, 60), rng.randint(-60, 60))
            add('cross', [2, *poly_ints(p), *poly_ints(q)], emu.cross(p, q))
        # A clip.
        poly = pick()
        if poly:
            xs = [pt[0] for pt in poly]
            left, right = min(xs), max(xs)
            cl, cr = rng.randint(left - 20, right + 20), rng.randint(left - 20, right + 20)
            if cl > cr:
                cl, cr = cr, cl
            add('clip', [3, *poly_ints(poly), left, right, cl, cr], emu.clip(poly, left, right, cl, cr))
        # A pair.
        pa, pb = pick(), pick()
        ba, bb = make_body(rng, pa, (0, 0)), make_body(rng, pb, (0, 0))
        bb['from'] = (rng.randint(-40, 40), rng.randint(-40, 40))
        bb['to'] = (bb['from'][0] + rng.choice([0, 0, rng.randint(-10, 10)]), bb['from'][1] + rng.choice([0, 0, rng.randint(-10, 10)]))
        bb['pos'] = bb['to']
        if rng.random() < 0.7:
            # Send a through b: its centre passes near b's centre part of the way along its move.
            move = (rng.randint(-120, 120), rng.randint(-120, 120))
            if move == (0, 0):
                move = (7, 3)
            aim = (bb['from'][0] + (bb['box'][0] + bb['box'][2]) // 2 + rng.randint(-15, 15),
                   bb['from'][1] + (bb['box'][1] + bb['box'][3]) // 2 + rng.randint(-15, 15))
            u = rng.choice([0.2, 0.5, 0.8, 1.0])
            centre = ((ba['box'][0] + ba['box'][2]) // 2, (ba['box'][1] + ba['box'][3]) // 2)
            start = (int(aim[0] - move[0] * u) - centre[0], int(aim[1] - move[1] * u) - centre[1])
            ba['from'], ba['to'] = start, (start[0] + move[0], start[1] + move[1])
            ba['pos'] = ba['to']
        ba['jim'] = rng.random() < 0.1
        ints = [4]
        for body in (ba, bb):
            ints += [*body['box'], *body['from'], *body['to'], *body['pos'], body['depth'], int(body['jim']), *poly_ints(body['poly'])]
        add('pair', ints, emu.pair(ba, bb))

    data = struct.pack('<i', len(kinds)) + struct.pack('<%di' % len(request), *request)
    Path('/private/tmp/overlap_request.bin').write_bytes(data)
    Path('/private/tmp/overlap_kinds.json').write_text(json.dumps(kinds))
    output = subprocess.run([str(driver.resolve())], input=data, capture_output=True, check=True, timeout=120).stdout
    produced = struct.unpack('<%di' % (len(output) // 4), output)

    at, compared, skipped, per_kind, positives = 0, 0, 0, {}, {}
    for kind, want, given in zip(kinds, expected, inputs):
        width = {'box': 1, 'inside': 1, 'cross': 1, 'pair': 3}.get(kind)
        if kind == 'clip':
            width = 2 + produced[at] * 2
        got = list(produced[at:at + width])
        at += width
        if want is None:
            skipped += 1
            continue
        want = want if isinstance(want, list) else [want]
        if kind == 'clip' and got != want:
            raise ValueError(f'clip: input={given} cpp={got[:40]} original={want[:40]}')
        if kind != 'clip' and got != want:
            raise ValueError(f'{kind}: input={given} cpp={got} original={want}')
        compared += 1
        if kind in ('inside', 'cross') and want == [1]:
            positives[kind] = positives.get(kind, 0) + 1
        if kind == 'pair' and want[0] == 1:
            positives['pair_queued'] = positives.get('pair_queued', 0) + 1
            positives['pair_touching'] = positives.get('pair_touching', 0) + (want[1] == 0)
        per_kind[kind] = per_kind.get(kind, 0) + 1
    return dict(cases_per_kind=cases, compared=compared, per_kind=per_kind, positive_results=positives, skipped_original_traps=skipped, dependent_on_memory_past_the_points=emu.undefined,
                real_polygons=len(real),
                executable_sha256=hashlib.sha256((game / 'TESORO.EXE').read_bytes()).hexdigest(),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cases', type=int, default=500)
    parser.add_argument('--seed', type=int, default=1)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver, args.cases, args.seed)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines/tesoro' / name).read_bytes()).hexdigest() for name in ('overlap.cpp', 'overlap.h')}
    report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'source_sha256'}))
