#!/usr/bin/env python3
"""Vectors for the input state machine: runs the ORIGINAL InputDevice state update (0x403700, GetTickCount replaced) in Unicorn on a
fake device (release delay at +0xc, repeat delay at +0x10, stamp array at +0x14, state array at +0x1c).
Output: local-media/vectors/input.txt (ignored). Consumed by re/tools/drivers/input_driver.cpp.
Lines: I <n> <releaseDelay> <repeatDelay> <state:stamp> ...   initial device
       U <tick> <raw0> .. <rawN-1> | <state:stamp> ...        one update (raw 0 or 4) and the resulting states/stamps
       #                                                       end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
rnd = random.Random(20261103)
DEV, STAMPS, STATES, RAW = 0x100c00, 0x100d00, 0x100e00, 0x100f00
e = Emu(); tick = [0]
e.stub_import(0x44426c, lambda emu: tick[0] & 0xffffffff)
out = []
for case in range(400):
    n = rnd.randint(1, 4); rel, rep = rnd.choice([300, 0, 50, 1000, rnd.getrandbits(31)]), rnd.choice([500, 0, 100, rnd.getrandbits(31)])
    init = [(rnd.choice([0, 0, 1, 2, 4, 8, 0x10, 0x20, 3, 0x40]), rnd.choice([0, rnd.getrandbits(32), rnd.randint(0, 2000)])) for _ in range(n)]
    e.write(DEV, bytes(0x40)); e.write(DEV + 0xc, struct.pack("<II", rel, rep)); e.write(DEV + 0x14, struct.pack("<I", STAMPS)); e.write(DEV + 0x1c, struct.pack("<I", STATES))
    for i, (s, t) in enumerate(init): e.write(STATES + i, bytes([s])); e.write(STAMPS + 4 * i, struct.pack("<I", t))
    out.append(f"I {n} {rel} {rep} " + " ".join(f"{s}:{t}" for s, t in init))
    tick[0] = rnd.choice([0, 1000, 123456, 0xfffffff0])
    for _ in range(rnd.randint(2, 40)):
        tick[0] = (tick[0] + rnd.choice([0, 1, 16, 100, 299, 300, 301, 499, 500, 501, 2000])) & 0xffffffff
        raw = [rnd.choice([0, 4, 4]) if rnd.random() < 0.8 else rnd.choice([0, 4]) for _ in range(n)]
        e.write(RAW, bytes(raw)); e.call(0x403700, [RAW, n], conv="thiscall", this=DEV)
        res = " ".join(f"{e.read(STATES + i, 1)[0]}:{e.u32(STAMPS + 4 * i)}" for i in range(n))
        out.append(f"U {tick[0]} " + " ".join(map(str, raw)) + " | " + res)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/input.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
