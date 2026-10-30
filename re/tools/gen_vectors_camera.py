#!/usr/bin/env python3
"""Vectors for the camera: ORIGINAL Camera_Ctor 0x402110, SetBounds 0x402200, SetValue0 (target) 0x402230,
SetPositionClamped 0x402120, MoveTo 0x4021a0 and UpdateToward 0x401fc0 in Unicorn (game clock 0x413a10 replaced by a
settable time), vs engine Camera. The target is a fake object with x at +0x4c.
Output: local-media/vectors/camera.txt (ignored), consumed by re/tools/drivers/camera_driver.cpp.
Lines: C / ct / sb l t r b / st <0|1> / tx x / tm t / sp x y <ret> / mv x y d / ut div / each followed by
       S x y scaledX scaledY (positions after the op)   #
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261101)
CAM, TGT = 0x100100, 0x100400
e = Emu()
clock = {"t": 0}
e.hook_function(0x413a10, lambda emu, *a: clock["t"], 0)
u32 = e.u32
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def state(): return f"S {s32(u32(CAM + 0x2c))} {s32(u32(CAM + 0x30))} {s32(u32(CAM + 0x34))} {s32(u32(CAM + 0x38))}"
def w32(a, v): e.write(a, (v & 0xffffffff).to_bytes(4, "little"))
def pos(wide):
    if wide and rnd.random() < 0.15: return rnd.randint(-2**30, 2**30)
    return rnd.choice([rnd.randint(-700, 700), rnd.randint(-100, 100), rnd.randint(-2000, 2000)])

out = []
for case in range(600):
    e.write(CAM, bytes(0x40)); e.write(TGT, bytes(0x100)); clock["t"] = rnd.choice([0, 1000, 5000, 2**31 - 500])
    e.call(0x402110, [], this=CAM); out += ["C", f"tm {s32(clock['t'])}", "ct", state()]
    b = rnd.choice([(-320, 239, 319, -240), (-320, 239, 959, -240), (-640, 500, 1300, -500), tuple(sorted([pos(0) for _ in range(2)]))[0:1] + (pos(0), pos(0), pos(0))])
    if len(b) != 4: b = (-320, 239, 319, -240)
    e.call(0x402200, list(b), conv="thiscall", this=CAM); out += ["sb %d %d %d %d" % b, state()]
    have = rnd.random() < 0.8
    if have: e.call(0x402230, [TGT], conv="thiscall", this=CAM)
    out += [f"st {int(have)}", state()]
    for _ in range(rnd.randint(3, 40)):
        op = rnd.choice(["tm", "tm", "tx", "sp", "mv", "ut", "ut", "ut", "ut", "tx_edge", "tm_edge", "sb_edge", "combo"])
        if op == "tm":
            clock["t"] = (clock["t"] + rnd.choice([0, 16, 50, 300, 800, 2000, 2**31 - 1])) & 0xffffffff
            clock["t"] = s32(clock["t"]) & 0xffffffff; out.append(f"tm {s32(clock['t'])}")
        elif op == "combo":
            # glide to an out-of-bounds goal, let it finish, then make the target scroll the camera
            x, y, d = pos(0), rnd.choice([-3000, -900, 900, 3000]), rnd.choice([100, 750])
            e.call(0x4021a0, [x, y, d], conv="thiscall", this=CAM); out.append(f"mv {x} {y} {d}"); out.append(state())
            clock["t"] = (s32(u32(CAM + 0xc)) + d + 1) & 0xffffffff; out += [f"tm {s32(clock['t'])}"]; out.append(state())
            e.call(0x401fc0, [300], conv="thiscall", this=CAM); out += ["ut 300", state()]
            if rnd.random() < 0.7:   # widen the bounds, so a goal that was left outside them would now count
                b = (-5000, 5000, 5000, -5000); e.call(0x402200, list(b), conv="thiscall", this=CAM); out += ["sb %d %d %d %d" % b, state()]
            g = s32(u32(CAM + 0x14)); xx = g + rnd.choice([-90, 90]); w32(TGT + 0x4c, xx); out += [f"tx {xx}", state()]
            e.call(0x401fc0, [300], conv="thiscall", this=CAM); out += ["ut 300", state()]
            clock["t"] = (clock["t"] + 400) & 0xffffffff; out += [f"tm {s32(clock['t'])}", state()]
            e.call(0x401fc0, [300], conv="thiscall", this=CAM); out.append("ut 300")
        elif op == "tx_edge":
            g = s32(u32(CAM + 0x14)); x = g + rnd.choice([-77, -76, -75, -74, 74, 75, 76, 77]); w32(TGT + 0x4c, x); out.append(f"tx {x}")
        elif op == "tm_edge":
            t = (s32(u32(CAM + 0xc)) + s32(u32(CAM + 0x10)) + rnd.choice([-1, 0, 1])) & 0xffffffff
            clock["t"] = t; out.append(f"tm {s32(t)}")
        elif op == "sb_edge":
            g = s32(u32(CAM + 0x14)); b = (-320, 239, g + 0x13f + rnd.choice([-1, 0, 1, 2]), -240)
            e.call(0x402200, list(b), conv="thiscall", this=CAM); out.append("sb %d %d %d %d" % b)
        elif op == "tx":
            x = pos(1); w32(TGT + 0x4c, x); out.append(f"tx {x}")
        elif op == "sp":
            x, y = pos(1), pos(1); r = e.call(0x402120, [x, y], conv="thiscall", this=CAM) & 0xff; out.append(f"sp {x} {y} {r}")
        elif op == "mv":
            x, y = pos(1), pos(1); d = rnd.choice([0, 0, 100, 750, 1500, rnd.randint(1, 5000)])
            e.call(0x4021a0, [x, y, d], conv="thiscall", this=CAM); out.append(f"mv {x} {y} {d}")
        else:
            dv = rnd.choice([300, 300, 150, 600, 100, 1, -7, rnd.randint(1, 900)])
            e.call(0x401fc0, [dv], conv="thiscall", this=CAM); out.append(f"ut {dv}")
        out.append(state())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/camera.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
