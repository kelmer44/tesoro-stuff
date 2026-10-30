#!/usr/bin/env python3
"""Vectors for the compose step: image 0 is a 640x480 background, queued first at the farthest depth so every row is covered.
Runs the ORIGINAL Display_ResetRenderQueue (0x411f20), Display_AddSprite (0x411f80) and Display_Compose (0x412e10) in Unicorn; the
surface is pre-locked (flag and pointer set) so no DirectDraw call is made. After each compose the CRC-32 of every frame row is dumped.
Output: local-media/vectors/compose.txt (ignored). Consumed by re/tools/drivers/compose_driver.cpp.
Lines: C <hex dat> <entry>      bank file (image 0 = background, others small literal images)
       Z                         reset (0x411f20)
       S <image> <x> <y> <depth> add a sprite
       K then F <crc of row 0> ... <crc of row 479>   compose and dump
       #                         end of case
"""
import os, random, struct, sys, zlib
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261102)
PL, ARCH, FP, DISP, HEAP, ROWS, FRAME = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000, 0x3000000, 0x3200000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(ROWS, 0x150000, 3); e.uc.mem_map(FRAME, 0xa0000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def make_blob")])
e.write(DISP + 0x670, struct.pack("<I", ROWS)); e.write(DISP + 0x674, struct.pack("<I", ROWS + 240000 * 4))

def make_image():
    solid = rnd.random() < 0.4
    H = rnd.randint(1, 8); rows = []; end = 0
    for _ in range(H):
        items = []; x = 0
        for _ in range(rnd.choice([0, 1, 1, 2, 3, 4])):
            gap, ln = rnd.randint(0, 6), rnd.randint(1, 12); items.append((gap, ln)); x += gap + ln
        rows.append(items); end = max(end, x)
    W = end + rnd.choice([0, 0, 3])
    nitems = sum(len(r) for r in rows); pix = 2 * rnd.randint(20, 50)
    head = 0x10 + 8 * H; itoff = head; pixoff = head + 8 * nitems; size = pixoff + pix
    b = bytearray(size)
    struct.pack_into("<IIHHHH", b, 0, pixoff, pix, W, H, 3 if solid else 1, 0)
    o = itoff
    for r, items in enumerate(rows):
        struct.pack_into("<IHH", b, 0x10 + 8 * r, o, len(items), 0)
        for gap, ln in items:
            struct.pack_into("<IHH", b, o, rnd.getrandbits(32) if solid else pixoff + 2 * rnd.randint(0, pix // 2 - 14), gap, ln); o += 8
    return bytes(b)

def make_background():
    H, W = 480, 640; head = 0x10 + 8 * H; pixoff = head + 8 * H; pix = 4 * 1280; size = pixoff + pix
    b = bytearray(size); struct.pack_into("<IIHHHH", b, 0, pixoff, pix, W, H, 1, 0)
    for r in range(H):
        struct.pack_into("<IHH", b, 0x10 + 8 * r, head + 8 * r, 1, 0)
        struct.pack_into("<IHH", b, head + 8 * r, pixoff + 1280 * (r % 4), 0, W)
    for i in range(pix): b[pixoff + i] = rnd.getrandbits(8)
    return bytes(b)

def make_bank(nimg):
    out = bytearray(struct.pack("<HHHHIIIB3s", 1, 0, nimg, 0, 0, 0, 0, 1, b"\0\0\0"))
    out += struct.pack("<IHHHhIIHH", 0, 0, 0, 10, -1, 0, 0, 0, 0)
    for k in range(nimg):
        blob = make_background() if k == 0 else make_image(); out += struct.pack("<I", len(blob)) + blob
    return bytes(out)

def crcs():
    e.write(FRAME, bytes([0xab]) * 614400)
    e.write(DISP + 0x66a, b"\x01"); e.write(DISP + 0x678, struct.pack("<I", FRAME))
    e.call(0x412e10, [], this=DISP)
    f = e.read(FRAME, 614400)
    return "F " + " ".join(str(zlib.crc32(f[1280 * y:1280 * (y + 1)])) for y in range(480))

out = []
for case in range(300):
    nimg = rnd.randint(2, 3); dat = make_bank(nimg); entry = rnd.randint(0, 20)
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0; st["heap"] = HEAP
    e.write(DISP + 0x67c, b"\0"); e.write(PL, bytes(0x40))
    e.call(0x401160, [entry, 0, 0], conv="thiscall", this=PL)
    imgp = e.u32(PL); bases = [e.u32(imgp + 4 * k) for k in range(nimg)]
    sizes = []
    hdr = struct.calcsize("<HHHHIIIB3s") + struct.calcsize("<IHHHhIIHH"); p = hdr
    for k in range(nimg): ln = struct.unpack_from("<I", dat, p)[0]; sizes.append(ln); p += 4 + ln
    out.append(f"C {st['data'].hex()} {entry}")
    e.call(0x411f20, [], this=DISP); out.append("Z")
    e.call(0x411f80, [0, 0, 65535, bases[0], 0], conv="thiscall", this=DISP); out.append("S 0 0 0 65535")
    for batch in range(rnd.randint(1, 3)):
        for _ in range(rnd.randint(1, 14)):
            k = rnd.randrange(1, nimg)
            x = rnd.choice([rnd.randint(-40, 80), rnd.randint(560, 680), rnd.randint(-20, 640)]); y = rnd.choice([rnd.randint(-8, 10), rnd.randint(470, 490), rnd.randint(0, 480)])
            if rnd.random() < 0.5: x, y = rnd.randint(0, 30) + 300, rnd.randint(100, 110)       # cluster: many overlaps
            depth = rnd.choice([0, 1, 2, 3, 3, 5, 65535, rnd.getrandbits(16)])
            e.call(0x411f80, [x & 0xffffffff, y & 0xffffffff, depth, bases[k], 0], conv="thiscall", this=DISP)
            out.append(f"S {k} {x} {y} {depth}")
        out.append("K"); out.append(crcs())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/compose.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
