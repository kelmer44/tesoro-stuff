#!/usr/bin/env python3
"""Vectors for the outline routines: runs the ORIGINAL 00409560 (contains point), 00409c10 (clip to columns) and 00409790 (overlap test)
in Unicorn on random convex outlines (hull split into a left-to-right and a right-to-left chain, as the animation files store them)
and on raw monotone chains. Cases where the original faults (division by zero on a vertical edge) or runs away are dropped.
Output local-media/vectors/outline.txt (ignored), consumed by drivers/outline_driver.cpp.
  C n m pts.. | px py = r           contains          K n m pts.. | bx1 bx2 cx1 cx2 = n' m' pts'..     clip          O n m pts.. | n m pts.. = r   overlap
Points are written as x:y.
"""
import random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn import UcError
from unicorn import UC_HOOK_MEM_READ
from unicorn.x86_const import UC_X86_REG_EIP

ARENA = 0x03000000
rnd = random.Random(20261105)
e = Emu(); e.uc.mem_map(ARENA, 0x100000, 7)
allowed, stray = [], [False]      # a read outside the points (and 16 entries around them) drops the case: the port could not see the same bytes
def on_read(uc, access, addr, size, value, _):
    if not any(lo <= addr < hi for lo, hi in allowed): stray[0] = True
e.uc.hook_add(UC_HOOK_MEM_READ, on_read, begin=ARENA, end=ARENA + 0xfffff)
def s16(v): return v - 65536 if v & 0x8000 else v
def put(base, n, m, pts):
    ptr = base + 0x100                       # zero padding on both sides of the points
    allowed.append((base, base + 8)); allowed.append((ptr - 64, ptr + 4 * n + 64))
    e.write(base - 0x200, bytes(0x800))
    e.write(base, struct.pack("<HHI", n, m, ptr))
    e.write(ptr, b"".join(struct.pack("<hh", x, y) for x, y in pts))
    return base

def hull(pts):
    pts = sorted(set(pts))
    if len(pts) < 3: return None
    def cross(o, a, b): return (a[0]-o[0])*(b[1]-o[1]) - (a[1]-o[1])*(b[0]-o[0])
    lo = []
    for p in pts:
        while len(lo) >= 2 and cross(lo[-2], lo[-1], p) <= 0: lo.pop()
        lo.append(p)
    up = []
    for p in reversed(pts):
        while len(up) >= 2 and cross(up[-2], up[-1], p) <= 0: up.pop()
        up.append(p)
    return lo, up                              # lo: left to right, up: right to left (starts at the rightmost point)

def shape(span=300):
    while True:
        k = rnd.randint(3, 9); r = rnd.choice([6, 40, span])
        h = hull([(rnd.randint(-r, r), rnd.randint(-r, r)) for _ in range(k)])
        if h:
            lo, up = h
            pts = lo + up[1:-1] if rnd.random() < .5 else lo + up[1:]     # with or without the closing point
            if rnd.random() < .3:                                          # raw perturbation of a chain
                pts = [(x, y + rnd.randint(-3, 3)) if rnd.random() < .3 else (x, y) for x, y in pts]
            return pts, len(lo) - 1
def closed():
    pts, m = shape(); return pts + [pts[0]], m

def run(fn, args):
    stray[0] = False
    try: r = e.call(fn, args, max_insns=200000) & 0xff
    except UcError: return None
    return r if e.uc.reg_read(UC_X86_REG_EIP) == 0x7ffff000 and not stray[0] else None      # dropped when it ran away

out = []
for case in range(9000):
    kind = case % 3
    allowed.clear()
    if kind == 0:
        pts, m = closed(); put(ARENA + 0x1000, len(pts), m, pts)
        xs = [p[0] for p in pts]; ys = [p[1] for p in pts]
        if rnd.random() < .5: px, py = rnd.choice(pts)                    # on a vertex
        elif rnd.random() < .5:
            a, b = rnd.sample(pts, 2); t = rnd.random(); px, py = round(a[0]+(b[0]-a[0])*t), round(a[1]+(b[1]-a[1])*t)
        else: px, py = rnd.randint(min(xs)-5, max(xs)+5), rnd.randint(min(ys)-5, max(ys)+5)
        r = run(0x409560, [ARENA + 0x1000, px, py])
        if r is not None: out.append(f"C {len(pts)} {m} " + " ".join(f"{x}:{y}" for x, y in pts) + f" | {px} {py} = {r}")
    elif kind == 1:
        pts, m = shape(); put(ARENA + 0x1000, len(pts), m, pts)
        xs = [p[0] for p in pts]; bx1, bx2 = min(xs), max(xs)
        if rnd.random() < .3: bx1 -= rnd.randint(0, 20); bx2 += rnd.randint(0, 20)
        cx1, cx2 = sorted(rnd.randint(bx1 - 30, bx2 + 30) for _ in range(2))
        out_base = ARENA + 0x8000; allowed.append((out_base, out_base + 8)); e.write(out_base - 0x200, bytes(0x1000)); e.write(out_base, struct.pack("<HHI", 0xdead, 0xbeef, out_base + 0x100))
        r = run(0x409c10, [ARENA + 0x1000, bx1, 0, bx2, 0, out_base, cx1, 0, cx2])
        if r is not None:
            n, mm, ptr = struct.unpack("<HHI", e.read(out_base, 8)); res = [struct.unpack("<hh", e.read(ptr + 4 * i, 4)) for i in range(n)]
            out.append(f"K {len(pts)} {m} " + " ".join(f"{x}:{y}" for x, y in pts) + f" | {bx1} {bx2} {cx1} {cx2} = {n} {mm} " + " ".join(f"{x}:{y}" for x, y in res))
    else:
        a, am = shape(); b, bm = shape(); dx, dy = rnd.randint(-80, 80), rnd.randint(-80, 80)
        if rnd.random() < .6: b = [(x + dx, y + dy) for x, y in b]
        put(ARENA + 0x1000, len(a), am, a); put(ARENA + 0x4000, len(b), bm, b)
        r = run(0x409790, [ARENA + 0x1000, ARENA + 0x4000])
        if r is not None: out.append(f"O {len(a)} {am} " + " ".join(f"{x}:{y}" for x, y in a) + f" | {len(b)} {bm} " + " ".join(f"{x}:{y}" for x, y in b) + f" = {r}")
import os; os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/outline.txt", "w").write("\n".join(out) + "\n")
from collections import Counter
print(Counter(l[0] for l in out), "overlap true:", sum(1 for l in out if l[0] == "O" and l.endswith("= 1")), "contains true:", sum(1 for l in out if l[0] == "C" and l.endswith("= 1")))
