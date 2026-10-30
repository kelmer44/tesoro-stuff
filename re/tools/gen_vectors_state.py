#!/usr/bin/env python3
"""Vectors for object save-game state: ORIGINAL Obj_SaveState 0x401ee0 and Obj_LoadState 0x401e20 in Unicorn (fread 0x41b4c0 and
fwrite 0x41b620 replaced by a byte buffer) on a fake object (flags +0x68, bank byte +0x32, player array +0x34, current player +0x38,
blob +0x60 / size +0x64), vs engine Object::saveState / loadState.
Output: local-media/vectors/state.txt (ignored), consumed by re/tools/drivers/state_driver.cpp.
Lines: C <banks> <capacity> <hasAnim> <bank> / Pl <i> <flags> <first> <last> <current> / Bl <hex> / sv / W <hex written> /
       ld <hex stream> / S <bank> <players: flags first last current>... | <size> <blob hex of capacity bytes>   #
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261102)
OBJ, ARR, PLS, BLOB = 0x100100, 0x100300, 0x100400, 0x100800
e = Emu()
st = {"in": b"", "pos": 0, "out": b""}
def fread(emu, buf, size, count, fp):
    n = size * count; chunk = st["in"][st["pos"]:st["pos"] + n]; emu.write(buf, chunk); st["pos"] += len(chunk); return len(chunk) // size if size else 0
def fwrite(emu, buf, size, count, fp):
    st["out"] += emu.read(buf, size * count); return count
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b620, fwrite, 4)
def u8(a): return e.read(a, 1)[0]
def u16(a): return struct.unpack("<H", e.read(a, 2))[0]
def u32(a): return struct.unpack("<I", e.read(a, 4))[0]
def hx(b): return bytes(b).hex() or "-"
def stream(bank, cap, short):
    p = struct.pack("<BHHBB", bank, rnd.getrandbits(16), rnd.getrandbits(16), rnd.getrandbits(8), rnd.getrandbits(8))
    return p
out = []
for case in range(500):
    n = rnd.randint(1, 3); cap = rnd.choice([0, 0, 6, 40]); anim = rnd.random() < 0.8; bank = rnd.randrange(n)
    e.write(OBJ, bytes(0x80)); e.write(PLS, bytes(0x200)); e.write(BLOB, bytes(0x100))
    e.write(OBJ + 0x32, bytes([bank])); e.write(OBJ + 0x33, bytes([n])); e.write(OBJ + 0x34, struct.pack("<I", ARR))
    e.write(OBJ + 0x38, struct.pack("<I", PLS + bank * 0x40)); e.write(OBJ + 0x68, struct.pack("<I", 0x10 if anim else 0))
    e.write(OBJ + 0x60, struct.pack("<I", BLOB if cap else 0)); e.write(OBJ + 0x64, struct.pack("<H", cap))
    out.append(f"C {n} {cap} {int(anim)} {bank}")
    for i in range(n):
        e.write(ARR + 4 * i, struct.pack("<I", PLS + i * 0x40))
        fl, f, l, c = rnd.getrandbits(32), rnd.getrandbits(16), rnd.getrandbits(16), rnd.randrange(256)
        e.write(PLS + i * 0x40 + 0x18, struct.pack("<I", fl)); e.write(PLS + i * 0x40 + 0x24, struct.pack("<HH", f, l)); e.write(PLS + i * 0x40 + 0x30, struct.pack("<H", c))
        out.append(f"Pl {i} {fl} {f} {l} {c}")
    blob = bytes(rnd.getrandbits(8) for _ in range(cap)); e.write(BLOB, blob); out.append(f"Bl {hx(blob)}")
    def state():
        b = u8(OBJ + 0x32); parts = []
        for i in range(n):
            p = PLS + i * 0x40; parts.append(f"{u32(p + 0x18)} {u16(p + 0x24)} {u16(p + 0x26)} {u16(p + 0x30)}")
        return f"S {b} {' '.join(parts)} | {u16(OBJ + 0x64)} {hx(e.read(BLOB, cap))}"
    out.append(state())
    for _ in range(rnd.randint(2, 8)):
        if rnd.random() < 0.4:
            st["out"] = b""; e.call(0x401ee0, [FP := 0], conv="thiscall", this=OBJ); out += ["sv", f"W {hx(st['out'])}"]
        else:
            size = rnd.choice([0, cap, rnd.randint(0, cap)]) if cap else 0
            data = (stream(rnd.randrange(n), cap, False) if anim else b"") + (struct.pack("<H", size) + bytes(rnd.getrandbits(8) for _ in range(size)) if cap else b"")
            if rnd.random() < 0.15: data = data[:rnd.randint(0, len(data))]
            st["in"], st["pos"] = data, 0; e.call(0x401e20, [0], conv="thiscall", this=OBJ); out.append(f"ld {hx(data)}")
        out.append(state())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/state.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
