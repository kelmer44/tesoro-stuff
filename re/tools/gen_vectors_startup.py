#!/usr/bin/env python3
"""Generate test vectors for the engine's core primitives by running the ORIGINAL machine code in Unicorn.
Output: local-media/vectors/startup.txt (ignored). Consumed by re/tools/drivers/startup_driver.cpp.
Lines:  R <tick> <min> <max> <expected>   RandomRange        R7 ...  RandomRangeAlt
        K <hex key> <name>                NameHash64 (name has no spaces, '@' for empty)
        C <op> <tick> <expected|->        clock op sequence: new/now/freeze/unfreeze/suspend/resume, '#' separates sequences
"""
import random, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

def s32(v): return v - (1 << 32) if v & 0x80000000 else v
rnd = random.Random(20261029)
out = []
e = Emu(); tick = [0]
e.stub_import(0x44426c, lambda emu: tick[0] & 0xffffffff)

for fn, tag in ((0x413b00, "R"), (0x413ac0, "R7")):
    for _ in range(4000):
        tick[0] = rnd.getrandbits(32); lo = rnd.randint(-20, 100); hi = lo + rnd.randint(-3, 400)
        out.append(f"{tag} {tick[0]} {lo} {hi} {s32(e.call(fn, [lo, hi]))}")

names = ["A", "JIM", "MAPA", "PRINCIPAL", "ABCDEFGHIJKL", "ABCDEFGHIJKLMNOP", "jim", "0123", "ZZ9", "frase_01", "x", "ABCDEF", "ABCDEFG", "abcdef", "012345", "56789", "_", "~~~"]
alphabet = list(range(0x30, 0x5b)) + [0x5f, 0x61, 0x7a, 0x2d, 0x2e]
names += ["".join(chr(rnd.choice(alphabet)) for _ in range(rnd.randint(1, 14))) for _ in range(500)]
for nm in names:
    e.write(0x100000, nm.encode("latin1") + b"\0"); e.call(0x415c40, [0x100000])
    out.append(f"K {e.edx_eax():016x} {nm}")
# bytes >= 0x80 (signed compare) and empty name
for raw in (b"\xe9ABC", b"A\xf1O", b"\x80\xff"):
    e.write(0x100000, raw + b"\0"); e.call(0x415c40, [0x100000])
    out.append(f"KB {e.edx_eax():016x} {raw.hex()}")
e.write(0x100000, b"\0"); e.call(0x415c40, [0x100000]); out.append(f"K {e.edx_eax():016x} @")

CLK = 0x100100
for seq in range(300):
    t = rnd.getrandbits(32) if seq % 5 == 0 else rnd.randint(0, 1000000)
    tick[0] = t; e.write(CLK, bytes(16)); e.call(0x4139c0, [], this=CLK)
    out.append(f"C new {t} -")
    for _ in range(rnd.randint(5, 60)):
        r = rnd.random()
        t = (t + (rnd.choice([0, 1, 3, 16, 33, 100, 499, 500, 501, 2000, 100000]) if r < 0.9 else -rnd.randint(1, 50))) & 0xffffffff
        tick[0] = t
        op = rnd.choice(["now", "now", "now", "freeze", "unfreeze", "suspend", "resume"])
        addr = {"now": 0x413a10, "freeze": 0x413a50, "unfreeze": 0x413ab0, "suspend": 0x413a80, "resume": 0x413a90}[op]
        v = s32(e.call(addr, [], this=CLK)) if op == "now" else (e.call(addr, [], this=CLK) and None)
        out.append(f"C {op} {t} {v if op == 'now' else '-'}")
    out.append("#")
import os
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/startup.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
