#!/usr/bin/env python3
"""Vectors for the archive index: runs the ORIGINAL Archive_ReadIndex (0x403380) and Archive_FindEntry (0x403500)
in Unicorn on synthetic .TRE images (CRT calls hooked) and records the resulting index and lookups.
Output: local-media/vectors/archive.txt (ignored). Consumed by re/tools/drivers/archive_driver.cpp.
Lines: T <hex tre image>   E <hex name> <offset>  (index entry i, in order)   F <hex name> <offset|0>   # end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261029)
ARCH, FILEP, IDX, TRE = 0x100100, 0x100200, 0x1000000, 0x1100000
e = Emu(); e.uc.mem_map(IDX, 0x20000, 3); e.uc.mem_map(TRE, 0x100000, 3)
state = {"data": b"", "pos": 0}

def fread(emu, buf, size, count, fp):          # fread(buf, size, count, FILE*): sets EOF flag (0x10 at +0xc) when it runs dry
    n = size * count; chunk = state["data"][state["pos"]:state["pos"] + n]
    emu.write(buf, chunk); state["pos"] += len(chunk)
    if len(chunk) < n: emu.write(fp + 0xc, b"\x10")
    return len(chunk) // size
def strupr(emu, s):
    b = bytearray(); a = s
    while emu.read(a, 1) != b"\0": b.append(emu.read(a, 1)[0]); a += 1
    emu.write(s, bytes(c - 32 if 0x61 <= c <= 0x7a else c for c in b)); return s
def strncpy(emu, dst, src, n):
    s = bytearray(); a = src
    while len(s) < n and emu.read(a, 1) != b"\0": s.append(emu.read(a, 1)[0]); a += 1
    emu.write(dst, bytes(s) + bytes(n - len(s))); return dst
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b940, strupr, 1); e.hook_function(0x41b840, strncpy, 3)
e.write(0x442068, struct.pack("<I", IDX))       # global index buffer pointer

alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-"
def name():
    n = rnd.choice([1, 2, 3, 5, 8, 12, 17, 18, 18]); s = "".join(rnd.choice(alpha) for _ in range(n))
    if rnd.random() < 0.05: s = s[:-1] + chr(rnd.choice([0xe9, 0xf1, 0xff]))
    return s.encode("latin1")
out = []
for case in range(400):
    n = rnd.choice([0, 1, 2, 3, 10, 40, 150]); recs = []; names = []
    for _ in range(n):
        nm = name() if not names or rnd.random() > 0.08 else rnd.choice(names)   # a few duplicate names
        flags = rnd.choice([4, 4, 4, 0, 1, 5, 6, 2, 0x14 & ~0x10]) if rnd.random() < 0.8 else 4
        recs.append(struct.pack("<IIB", rnd.getrandbits(31), rnd.getrandbits(20), flags) + nm.ljust(19, b"\0")); names.append(nm)
    img = b"".join(recs)
    if rnd.random() < 0.1: img += bytes(rnd.getrandbits(8) for _ in range(rnd.randint(1, 27)))   # trailing partial record
    state["data"], state["pos"] = img, 0
    e.write(FILEP, bytes(0x20)); e.write(ARCH, struct.pack("<II", 0, FILEP))
    e.call(0x403380, [], this=ARCH)
    cnt = e.u32(ARCH)
    out.append("T " + img.hex() + " ")
    kept = []
    for i in range(cnt):
        raw = e.read(IDX + 0x18 * i, 0x13); nm = raw.split(b"\0")[0]; kept.append(nm)
        out.append(f"E {nm.hex() or '-'} {e.u32(IDX + 0x18 * i + 0x14)}")
    probes = list(kept) + [nm.upper() for nm in names] + [nm for nm in names] + [name() for _ in range(5)] + [b"", b"ZZZZZZZZZZZZZZZZZZZZ"]
    for nm in probes:
        e.write(0x100400, nm + b"\0")
        out.append(f"F {nm.hex() or '-'} {e.call(0x403500, [0x100400, 0], conv='thiscall', this=ARCH)}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/archive.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
