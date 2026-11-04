#!/usr/bin/env python3
"""Vectors for the keyboard and mouse classes: runs the ORIGINAL constructors (0x403820, 0x403980) and per-frame reads (0x4038f0, 0x403aa0) in Unicorn
with a fake DirectInput (the harness of check_devices.py), and records the values the constructors leave and what every read does to the
pointer position and the button and key states. Output: local-media/vectors/devices.txt (ignored), read by drivers/devices_driver.cpp.
Lines: KC <release> <repeat> <keys> <scan codes..>        keyboard after construction
       MC <buttons> <release> <repeat> <minx> <miny> <maxx> <maxy> <x> <y>     mouse after construction
       KU <tick> <5 scan bytes> | <5 states>               one keyboard read
       MU <tick> <dx> <dy> <dz> <4 button bytes> | <x> <y> <3 states> <ret>      one mouse read
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
src = open(__file__.rsplit("/", 1)[0] + "/check_devices.py").read()
exec(src[:src.index("# --- constructors")])           # World, rd, s32, constants
rng = random.Random(20261106)
out = []
w = World(); e = w.e
kb = w.alloc(0x40); e.call(0x403820, [HW, HI, 6], conv="stdcall", this=kb)
out.append("KC " + " ".join(str(rd(w, kb + o)) for o in (0xc, 0x10, 0x18)) + " " + " ".join(str(b) for b in rd(w, kb + 0x20, 5)))
keys = list(rd(w, kb + 0x20, 5))
ms = w.alloc(0x40); e.call(0x403980, [HW, HI, 5], conv="stdcall", this=ms)
out.append("MC " + " ".join(str(s32(rd(w, ms + o))) for o in (0x18, 0xc, 0x10, 0x20, 0x24, 0x28, 0x2c, 0x38, 0x3c)))
for sess in range(60):
    w = World(); e = w.e
    k = w.alloc(0x40); e.call(0x403820, [HW, HI, 6], conv="stdcall", this=k)
    out.append("KS")
    scan = bytearray(256)
    for i in range(rng.randint(5, 40)):
        if rng.random() < 0.5:         # keys stay as they were: held keys cross the release and repeat delays
            scan = bytearray(256)
            for j in keys: scan[j] = rng.choice([0, 0x80, 0xff, 0x7f, 0x20, 0xa0])
        w.state = bytes(scan); w.tick += rng.choice([1, 50, 250, 400, 5000, 15000, 30000])
        ret = w.e.call(0x4038f0, [], this=k) & 0xff
        st = w.e.read(rd(w, k + 0x1c), 5)
        out.append(f"KU {w.tick} " + " ".join(str(scan[j]) for j in keys) + " | " + " ".join(str(b) for b in st) + f" {ret}")
    w = World(); e = w.e
    m = w.alloc(0x40); e.call(0x403980, [HW, HI, 5], conv="stdcall", this=m)
    out.append("MS")
    for i in range(rng.randint(5, 40)):
        dx, dy, dz = rng.choice([0, 1, -1, 50, -400, 900, -900, 3000]), rng.choice([0, 7, -7, 300, -600, 700, 5000]), rng.randint(-3, 3)
        b = [rng.choice([0, 0x80, 0xff, 0x7f]) for _ in range(4)]
        w.state = struct.pack("<iii", dx, dy, dz) + bytes(b); w.tick += rng.choice([1, 100, 400, 700])
        ret = w.e.call(0x403aa0, [], this=m) & 0xff
        st = w.e.read(rd(w, m + 0x1c), 3)
        out.append(f"MU {w.tick} {dx} {dy} {dz} " + " ".join(str(x) for x in b) + f" | {s32(rd(w, m + 0x38))} {s32(rd(w, m + 0x3c))} " + " ".join(str(x) for x in st) + f" {ret}")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/devices.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
