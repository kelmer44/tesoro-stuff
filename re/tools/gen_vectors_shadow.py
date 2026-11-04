#!/usr/bin/env python3
"""Vectors for the shadow images (type 2): like the compose vectors, plus shadow sprites queued through the original
Display_AddSprite (0x411f80 -> 0x4112d0) with a random depth window or none, the shade table (a random 32 KB table at
display +0x66c) and the original merge of the mask queue (0x412640) and flush inside Display_Compose (0x412e10). Scaled literal
images are queued too, so that shadows also fall on spans that are already drawn. After each compose the CRC-32 of every frame
row is dumped.
Output: local-media/vectors/shadow.txt (ignored). Consumed by re/tools/drivers/shadow_driver.cpp.
Lines: T <hex 32768 bytes>      the shade table (once)
       C <hex dat> <entry>      bank file (image 0 = background, others small images of types 1, 2 and 3)
       Z                         reset (0x411f20)
       S <image> <x> <y> <depth> <extra> add a sprite
       Q <image> <x> <y> <depth> <scaleX> <scaleY> <shear> <extra>   scaled sprite (any type) (0x4120c0)      L <layer depth>   (display +0x668)
       K then F <crc of row 0> ... <crc of row 479>   compose and dump
       #                         end of case
"""
import os, random, struct, sys, zlib
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261106)
PL, ARCH, FP, DISP, HEAP, ROWS, FRAME = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000, 0x3000000, 0x3200000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(ROWS, 0x150000, 3); e.uc.mem_map(FRAME, 0xa0000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def make_blob")])
TBL = HEAP + 0xf00000; TABLE = bytes(rnd.getrandbits(8) for _ in range(0x8000)); e.write(TBL, TABLE); e.write(DISP + 0x66c, struct.pack("<I", TBL))
e.write(DISP + 0x670, struct.pack("<I", ROWS)); e.write(DISP + 0x674, struct.pack("<I", ROWS + 240000 * 4))

def make_image():
    kind = rnd.choice([1, 1, 3, 2, 2]); solid = kind == 3
    H = rnd.randint(1, 8); rows = []; end = 0
    for _ in range(H):
        items = []; x = 0
        for _ in range(rnd.choice([0, 1, 1, 2, 3, 4] if kind != 2 else [1, 1, 2, 3])):
            gap, ln = rnd.randint(0, 6), rnd.randint(1, 12) if rnd.random() < 0.7 else rnd.randint(20, 70); items.append((gap, ln)); x += gap + ln
        rows.append(items); end = max(end, x)
    W = end + rnd.choice([0, 0, 3])
    nitems = sum(len(r) for r in rows); pix = 2 * rnd.randint(100, 150)
    head = 0x10 + 8 * H; itoff = head; pixoff = head + 8 * nitems; size = pixoff + pix
    b = bytearray(size)
    for i in range(pixoff, size): b[i] = rnd.getrandbits(8)       # the pixels must differ, or a wrong source offset shows nothing
    struct.pack_into("<IIHHHH", b, 0, pixoff, pix, W, H, kind, 0)
    o = itoff
    for r, items in enumerate(rows):
        struct.pack_into("<IHH", b, 0x10 + 8 * r, o, len(items), 0)
        for gap, ln in items:
            struct.pack_into("<IHH", b, o, rnd.randint(0, 31) if kind == 2 else rnd.getrandbits(32) if solid else pixoff + 2 * rnd.randint(0, pix // 2 - 74), gap, ln); o += 8
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

out = ["T " + TABLE.hex()]
for case in range(300):
    nimg = rnd.randint(3, 5); dat = make_bank(nimg); entry = rnd.randint(0, 20)
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0; st["heap"] = HEAP
    e.write(DISP + 0x67c, b"\0"); e.write(PL, bytes(0x40))
    e.call(0x401160, [entry, 0, 0], conv="thiscall", this=PL)
    imgp = e.u32(PL); bases = [e.u32(imgp + 4 * k) for k in range(nimg)]
    sizes = []
    hdr = struct.calcsize("<HHHHIIIB3s") + struct.calcsize("<IHHHhIIHH"); p = hdr
    for k in range(nimg): ln = struct.unpack_from("<I", dat, p)[0]; sizes.append(ln); p += 4 + ln
    out.append(f"C {st['data'].hex()} {entry}")
    qcount = 0
    for batch in range(rnd.randint(1, 3)):
        e.call(0x411f20, [], this=DISP); out.append("Z")
        e.call(0x411f80, [0, 0, 65535, bases[0], 0], conv="thiscall", this=DISP); out.append("S 0 0 0 65535 0")
        layer = rnd.choice([0, 0, 0, 1, 2, 4]); e.write(DISP + 0x668, struct.pack("<H", layer)); out.append(f"L {layer}")
        wanted = rnd.choice([0, 1, 1, 1, 2, 3, 5]); shadows = 0
        for _ in range(rnd.randint(1, 14)):
            k = rnd.randrange(1, nimg)
            x = rnd.choice([rnd.randint(-40, 80), rnd.randint(560, 680), rnd.randint(-20, 640)]); y = rnd.choice([rnd.randint(-8, 10), rnd.randint(470, 490), rnd.randint(0, 480)])
            if rnd.random() < 0.5: x, y = rnd.randint(0, 30) + 300, rnd.randint(100, 110)       # cluster: many overlaps
            depth = rnd.choice([0, 1, 2, 3, 3, 5, 65535, rnd.getrandbits(16)])
            ktype = u16(e.read(bases[k] + 12, 2), 0)
            if ktype == 2 and shadows >= wanted: continue
            lo = rnd.choice([0, 1, 2, 3, 5, 100]); hi = rnd.choice([lo, lo + 2, 5, 65535, 3000])
            extra = rnd.choice([0, 0, 0, (hi << 16) | lo]) if ktype == 2 else 0
            shadows += 1 if ktype == 2 else 0
            e.call(0x411f80, [x & 0xffffffff, y & 0xffffffff, depth, bases[k], extra], conv="thiscall", this=DISP)
            out.append(f"S {k} {x} {y} {depth} {extra}")
            for _rep in range(rnd.choice([1, 1, 2, 3])):
              if rnd.random() < 0.6 and qcount < 40:
                # a scaled sprite too: it is drawn straight into the frame when composed
                k2 = rnd.randrange(1, nimg); h2 = u16(e.read(bases[k2] + 10, 2), 0)
                sx = rnd.choice([0x10000] * 4 + [0x8000, 0xc000, 0x18000, 0x20000, 0x14000, rnd.randint(0x4000, 0x30000)])
                sy = rnd.choice([0x10000] * 3 + [0x8000, 0xc000, 0x18000, 0x20000, -0x10000, -0x20000, rnd.randint(0x4000, 0x30000)])
                shear = 0
                k2type = u16(e.read(bases[k2] + 12, 2), 0)
                if h2 >= 2 and abs(((h2 - 1) * sy)) >> 16 >= 1 and not (k2type == 2 and shadows >= wanted):
                    x2 = rnd.choice([rnd.randint(-40, 80), rnd.randint(560, 680), rnd.randint(0, 600)]); y2 = rnd.choice([rnd.randint(-8, 10), rnd.randint(440, 490), rnd.randint(0, 470)])
                    cluster = rnd.random() < 0.6
                    if cluster: x2, y2 = rnd.randint(0, 30) + 300, rnd.randint(100, 110)
                    d2 = rnd.choice([0, 1, 2]) if cluster else rnd.choice([0, 0, 1, 2, 3, 5, 65535])
                    lo2 = rnd.choice([0, 1, 2, 3, 5, 100]); hi2 = rnd.choice([lo2, lo2 + 2, 5, 65535, 3000])
                    extra2 = rnd.choice([0, 0, 0, (hi2 << 16) | lo2]) if k2type == 2 else 0
                    shadows += 1 if k2type == 2 else 0
                    e.call(0x4120c0, [x2 & 0xffffffff, y2 & 0xffffffff, d2, bases[k2], sx & 0xffffffff, sy & 0xffffffff, shear & 0xffffffff, extra2], conv="thiscall", this=DISP)
                    out.append(f"Q {k2} {x2} {y2} {d2} {sx} {sy} {shear} {extra2}"); qcount += 1
        # the mask queue as it stands before the compose
        mbase = u32(e.read(DISP + 0x674, 4), 0)
        for yy in range(480):
            rowb = mbase + yy * 800; n = u32(e.read(rowb, 4), 0); spans = []
            while n:
                nd = e.read(n, 16); spans.append(f"{s16(nd, 8)}:{s16(nd, 10)}:{u16(nd, 12)}:{u32(nd, 4)}:{u16(nd, 14)}"); n = u32(nd, 0)
            if spans: out.append(f"M {yy} " + " ".join(spans))
        if shadows <= 1:
            out.append("K"); out.append(crcs())
        else:
            out.append("N")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/shadow.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
