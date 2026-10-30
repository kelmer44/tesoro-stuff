#!/usr/bin/env python3
"""Vectors for the walk area (object kind 8): runs the ORIGINAL ScaleAt (0x414740), FindPath (0x414f10) and NextWaypoint (0x414e80) in Unicorn on a fake walk
area whose current sequence is a random polyline (points and a scale per point). Jim is a fake object whose walk method (vtable slot 0x80) logs its
arguments and answers a chosen value; his position is the second input of FindPath. Not covered: vertical segments hit exactly (the original divides by zero).
Usage: gen_vectors_walkarea.py (from the repository root). Output: local-media/vectors/walkarea.txt (ignored), read by drivers/walkarea_driver.cpp.
Lines: S <id> <ax> <ay> <n> then n times <x> <y> <scale> | P <ret> (the answer of Jim's walk) | Q <x> <y> -> r <scale> | F <dx> <dy> <jx> <jy> <target 0|1> -> r <bool> and log lines | N -> r <eax> and log | #
Log lines: walk <x> <y> <area|target|none> <direct>"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

WORLD, FNS = 0x3100000, 0x7e000000
e = Emu(); e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
AREA, PLAYER, SEQS, SCALES, POINTS, JIM, VT, TGT = [WORLD + 0x1000 * i for i in range(8)]
log = []; world = {"ret": 1}
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
def name(p): return "area" if p == AREA else "target" if p == TGT else "none" if p == 0 else "?"
def walk(emu, x, y, t, d): log.append(f"walk {i32(x)} {i32(y)} {name(t)} {d & 0xff}"); return world["ret"]
e.write(VT + 0x80, struct.pack("<I", FNS + 0x10)); e.hook_function(FNS + 0x10, walk, 4, pops=True)
e.write(JIM, struct.pack("<I", VT)); e.write(0x4420fc, struct.pack("<I", JIM))

rnd = random.Random(20261105)
out = []
def op(text, addr, args, this=AREA):
    del log[:]
    ret = e.call(addr, args, conv="thiscall", this=this, max_insns=2_000_000)
    out.append(text); out.append("r " + str(ret & 0xffffffff)); out.extend(". " + l for l in log)
for sid in range(1, 1201):
    n = rnd.randint(1, 8); ax, ay = rnd.randint(-300, 300), rnd.randint(-300, 300)
    pts = []
    while len(pts) < n:
        x = rnd.randint(-200, 400)
        if pts and pts[-1][0] == x: continue
        pts.append((x, rnd.randint(-150, 250), rnd.randint(0x40, 0x240)))
    e.write(AREA, bytes(0x100)); e.write(AREA + 0x4c, struct.pack("<ii", ax, ay)); e.write(AREA + 0x38, struct.pack("<I", PLAYER))
    e.write(PLAYER, bytes(0x40)); e.write(PLAYER + 0x1c, struct.pack("<I", SEQS)); e.write(PLAYER + 0x30, struct.pack("<H", 1))
    e.write(SEQS, bytes(0x24 * 3))
    e.write(SEQS + 0x24 + 0x14, struct.pack("<I", SCALES)); e.write(SEQS + 0x24 + 0x1c, struct.pack("<H", n)); e.write(SEQS + 0x24 + 0x20, struct.pack("<I", POINTS))
    e.write(SCALES, b"".join(struct.pack("<H", s) for _, _, s in pts)); e.write(POINTS, b"".join(struct.pack("<hh", x, y) for x, y, _ in pts))
    out.append(f"S {sid} {ax} {ay} {n}"); out.extend(f"{x} {y} {s}" for x, y, s in pts)
    for _ in range(rnd.randint(4, 14)):
        r = rnd.random()
        if r < 0.15 and n > 0:
            # exactly on a corner and its segments (both segments meet there), and a point level with it
            k = rnd.randrange(n); x = pts[k][0] + ax; y = pts[k][1] + ay + rnd.choice([0, 0, 1, -1])
            op(f"Q {x} {y}", 0x414740, [x & 0xffffffff, y & 0xffffffff])
        elif r < 0.25 and n > 1:
            # ties: a destination or Jim exactly as far from two corners as from each other
            k = rnd.randrange(n - 1); d = rnd.randint(1, 40); e_ = rnd.randint(0, 40)
            pts[k + 1] = (pts[k][0] + 2 * d, pts[k][1], pts[k + 1][2])
            e.write(POINTS, b"".join(struct.pack("<hh", x, y) for x, y, _ in pts))
            dx, dy = pts[k][0] + d + ax, pts[k][1] + e_ + ay
            world["ret"] = 1; out.append("P 1")
            jx, jy = rnd.randint(-100, 640), rnd.randint(-100, 480)
            if rnd.random() < 0.5: jx, jy = dx + (pts[k][0] - (pts[k][0] + d)) + ax - ax, dy + e_ * 0   # Jim on the same level as the corner side
            e.write(JIM + 0x4c, struct.pack("<ii", jx, jy)); out.append(f"R {k} {pts[k + 1][0]} {pts[k + 1][1]} {pts[k + 1][2]}")
            op(f"F {dx} {dy} {jx} {jy} 0", 0x414f10, [dx & 0xffffffff, dy & 0xffffffff, 0])
        elif r < 0.5:
            if rnd.random() < 0.7 and n > 1:
                k = rnd.randrange(n - 1); x = rnd.randint(min(pts[k][0], pts[k + 1][0]), max(pts[k][0], pts[k + 1][0])) + ax
            else: x = rnd.randint(-250, 450) + ax
            y = rnd.randint(-200, 300) + ay
            op(f"Q {x} {y}", 0x414740, [x & 0xffffffff, y & 0xffffffff])
        elif r < 0.8:
            world["ret"] = rnd.choice([1, 1, 0, 2, 3]); out.append(f"P {world['ret']}")
            dx, dy = rnd.randint(-300, 700), rnd.randint(-200, 500); jx, jy = rnd.randint(-100, 640), rnd.randint(-100, 480)
            e.write(JIM + 0x4c, struct.pack("<ii", jx, jy)); t = rnd.randint(0, 1)
            op(f"F {dx} {dy} {jx} {jy} {t}", 0x414f10, [dx & 0xffffffff, dy & 0xffffffff, TGT if t else 0])
        else:
            world["ret"] = rnd.choice([1, 1, 0, 2, 3]); out.append(f"P {world['ret']}")
            op("N", 0x414e80, [], AREA)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/walkarea.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
