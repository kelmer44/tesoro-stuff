#!/usr/bin/env python3
"""Vectors for the swept rectangle test: runs the ORIGINAL 0x409280 (cdecl, ten ints) in Unicorn on random rectangles and movements.
Output: local-media/vectors/sweep.txt (ignored). Consumed by re/tools/drivers/sweep_driver.cpp. Line: <10 args> = <result>
Cases whose result would divide by zero (the original faults) are left out."""
import os, random, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn import UcError
rnd = random.Random(20261104)
e = Emu(); out = []
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def rect(span):
    x1 = rnd.randint(-span, span); y1 = rnd.randint(-span, span); w = rnd.randint(0, span); h = rnd.randint(0, span)
    return x1, y1, x1 + w, y1 + h        # the game keeps x1 <= x2 and y1 <= y2 ... in either order for y, so also try swapped
n = 0
while n < 120000:
    span = rnd.choice([1, 2, 2, 3, 3, 4, 10, 50, 300])
    a = rect(span); b = rect(span)
    if rnd.random() < 0.5: a = (a[0], a[3], a[2], a[1])        # y up: the second y is the lower one
    if rnd.random() < 0.5: b = (b[0], b[3], b[2], b[1])
    if rnd.random() < 0.6:       # b close to where a is heading, so that the movement often ends in a touch
        ox, oy = rnd.randint(-2 * span, 2 * span), rnd.randint(-2 * span, 2 * span)
        b = (a[0] + ox, a[1] + oy, a[0] + ox + rnd.randint(0, span), a[1] + oy + rnd.randint(0, span))
        if rnd.random() < 0.5: b = (b[0], b[3], b[2], b[1])
    dx, dy = rnd.randint(-span, span), rnd.randint(-span, span)
    if rnd.random() < 0.15: dx = 0
    if rnd.random() < 0.15: dy = 0
    args = [*a, *b, dx, dy]
    try: r = s32(e.call(0x409280, [v & 0xffffffff for v in args], conv="cdecl"))
    except UcError: continue
    out.append(" ".join(map(str, args)) + f" = {r}"); n += 1
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/sweep.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
