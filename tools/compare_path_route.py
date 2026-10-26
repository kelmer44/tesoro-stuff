#!/usr/bin/env python3
"""Compare the production path object's routing with the original route and leg-end routines.

The local ignored driver local-media/jim-implementation/route_driver.cpp loads every room, finds the path
that its floor names and, for random positions of Jim and target points, records the walks that the path
starts: the first leg, and the leg after each arrival. The original route routine (0x414f10) and leg end
(0x414e80) run in the Unicorn emulator on a path object laid out from the same nodes, with Jim's
start-walk call captured, and must make the same walks. Run it with the Python of the local animation
environment.
"""
import argparse, hashlib, json, random, struct, subprocess
from pathlib import Path
from compare_floor_scale import floors
from compare_jim_walk import load_banks, BANKS
from compare_scene_native import NativeScene
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP


class NativePath(NativeScene):
    PATH, BANK, FRAMES, NODES, JIM, JIM_VTABLE, STUB, WAITER = 0x600000, 0x600100, 0x600200, 0x600400, 0x600800, 0x600900, 0x600a00, 0x600b00

    def __init__(self, exe):
        super().__init__(exe)
        self.calls = []
        self.uc.hook_add(UC_HOOK_CODE, self.start_walk, begin=self.STUB, end=self.STUB)

    def start_walk(self, uc, address, size, data):
        sp = uc.reg_read(UC_X86_REG_ESP)
        _, x, y, waiter, flag = struct.unpack('<IiiII', bytes(uc.mem_read(sp, 20)))
        self.calls.append((x, y, waiter, flag))
        self.ret(1, 16)

    def lay_out(self, x, y, nodes):
        w = self.uc.mem_write
        w(self.PATH, bytes(0xc0))
        w(self.PATH + 0x38, struct.pack('<I', self.BANK))
        w(self.PATH + 0x4c, struct.pack('<ii', x, y))
        w(self.BANK, bytes(0x40))
        w(self.BANK + 0x1c, struct.pack('<I', self.FRAMES))
        w(self.BANK + 0x30, struct.pack('<H', 0))
        w(self.FRAMES, bytes(0x24))
        w(self.FRAMES + 0x1c, struct.pack('<H', len(nodes)))
        w(self.FRAMES + 0x20, struct.pack('<I', self.NODES))
        w(self.NODES, b''.join(struct.pack('<hh', *n) for n in nodes))
        w(self.JIM, bytes(0x200))
        w(self.JIM, struct.pack('<I', self.JIM_VTABLE))
        w(self.JIM_VTABLE + 0x80, struct.pack('<I', self.STUB))
        w(0x4420fc, struct.pack('<I', self.JIM))

    def walks(self, jim, target):
        self.uc.mem_write(self.JIM + 0x4c, struct.pack('<ii', *jim))
        self.calls = []
        routed = self.invoke(0x414f10, self.PATH, (target[0], target[1], self.WAITER)) & 0xff
        legs = []
        if routed:
            for _ in range(200):
                legs.append(self.calls[-1][:2])
                if legs[-1] == tuple(target) and len(legs) >= 2:
                    break
                self.uc.mem_write(self.JIM + 0x4c, struct.pack('<ii', *legs[-1]))
                self.invoke(0x414e80, self.PATH)
        return routed, legs


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--driver', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--queries', type=int, default=200)
    p.add_argument('--seed', type=int, default=1)
    a = p.parse_args()
    rng = random.Random(a.seed)
    raw, _ = load_banks(a.game)
    items = list(floors(a.game))
    req = b''.join(struct.pack('<I', len(raw[n])) + raw[n] for n in BANKS) + struct.pack('<I', len(items))
    queries = []
    for name, scene, floor in items:
        xs = [floor['x'] + q[0] for q in floor['points']] if floor else [0]
        ys = [floor['y'] + q[1] for q in floor['points']] if floor else [0]
        qs = [(rng.randint(min(xs) - 60, max(xs) + 60), rng.randint(min(ys) - 60, max(ys) + 60),
               rng.randint(min(xs) - 60, max(xs) + 60), rng.randint(min(ys) - 60, max(ys) + 60)) for _ in range(a.queries)]
        queries.append(qs)
        req += struct.pack('<I', len(scene)) + scene + struct.pack('<I', len(qs))
        for q in qs:
            req += struct.pack('<iiii', *q)
    out = subprocess.run([str(a.driver)], input=req, capture_output=True, timeout=1800)
    if out.returncode:
        raise SystemExit(out.stderr.decode()[-2000:])
    vals = struct.unpack('<%di' % (len(out.stdout) // 4), out.stdout)
    native = NativePath((a.game / 'TESORO.EXE').read_bytes())
    at, report, compared, routed_count, legs_total = 0, [], 0, 0, 0
    for (name, scene, floor), qs in zip(items, queries):
        has, px, py, count = vals[at:at + 4]; at += 4
        nodes = [tuple(vals[at + 2 * i:at + 2 * i + 2]) for i in range(count)]; at += 2 * count
        if has:
            native.lay_out(px, py, nodes)
        mismatches = 0
        for q in qs:
            if not has:
                at += 2
                continue
            routed, nlegs = vals[at:at + 2]; at += 2
            legs = [tuple(vals[at + 2 * i:at + 4 + 2 * i][:2]) for i in range(nlegs)]; at += 2 * nlegs
            expected = native.walks(q[:2], q[2:])
            if (bool(routed), legs) != (bool(expected[0]), expected[1]):
                raise ValueError(f'{name} {q}: cpp={(routed, legs)} original={expected}')
            compared += 1
            routed_count += bool(routed)
            legs_total += len(legs)
        report.append(dict(scene=name, path=bool(has), nodes=count))
    result = dict(queries_compared=compared, routed=routed_count, legs=legs_total, scenes_with_path=sum(r['path'] for r in report),
                  scenes=report, executable_sha256=hashlib.sha256((a.game / 'TESORO.EXE').read_bytes()).hexdigest(),
                  driver_sha256=hashlib.sha256(a.driver.read_bytes()).hexdigest(), tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True, exist_ok=True)
    a.output.write_text(json.dumps(result, indent=1) + '\n')
    print(json.dumps({k: v for k, v in result.items() if k != 'scenes'}))

main()
