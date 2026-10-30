#!/usr/bin/env python3
"""Vectors for the render queue: runs the ORIGINAL Display_ResetRenderQueue (0x411f20) and Display_AddSprite (0x411f80), which
queues the rows of literal-type images (0x4105a0), in Unicorn on synthetic images read by the original loader (0x401160).
After every batch of sprites the span lists of all rows are dumped.
Output: local-media/vectors/render.txt (ignored). Consumed by re/tools/drivers/render_driver.cpp.
Lines: C <hex dat> <entry>      bank file with literal images (loaded in mode 0, 5-6-5)
       Z                         reset (0x411f20)
       S <image> <x> <y> <depth> add a sprite
       D then R <y> <bump> <free chain length> : x0,x1,depth,src;...   (src: 0, 1:colour or image:offset)   rows that are not empty
       #                         end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261101)
PL, ARCH, FP, DISP, HEAP, ROWS = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000, 0x3000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(ROWS, 0x150000, 3)
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

def make_bank(nimg):
    out = bytearray(struct.pack("<HHHHIIIB3s", 1, 0, nimg, 0, 0, 0, 0, 1, b"\0\0\0"))
    out += struct.pack("<IHHHhIIHH", 0, 0, 0, 10, -1, 0, 0, 0, 0)
    for _ in range(nimg):
        blob = make_image(); out += struct.pack("<I", len(blob)) + blob
    return bytes(out)

def dump(imgs, sizes):
    out = []
    for y in range(480):
        row = ROWS + 2000 * y; bump = u16(e.read(row + 0xc, 2), 0); free = e.u32(row + 0x10); head = e.u32(row)
        if head == 0 and bump == 2 and free == 0: continue
        n = 0; f = free
        while f and n < 200: n += 1; f = e.u32(f)
        spans = []; node = head
        while node and len(spans) < 200:
            x0, x1, d = s16(e.read(node + 8, 2), 0), s16(e.read(node + 10, 2), 0), u16(e.read(node + 12, 2), 0)
            p = e.u32(node + 4); desc = str(p) if p != 1 else f"1:{u16(e.read(node + 14, 2), 0)}"
            if p >= 2:
                for k, (base, size) in enumerate(zip(imgs, sizes)):
                    if base <= p <= base + size: desc = f"{k}:{p - base}"; break
                else: desc = f"?{p}"
            spans.append(f"{x0},{x1},{d},{desc}"); node = e.u32(node)
        out.append(f"R {y} {bump} {n} : " + ";".join(spans))
    return out

out = []
for case in range(300):
    nimg = rnd.randint(1, 3); dat = make_bank(nimg); entry = rnd.randint(0, 20)
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0; st["heap"] = HEAP
    e.write(DISP + 0x67c, b"\0"); e.write(PL, bytes(0x40))
    e.call(0x401160, [entry, 0, 0], conv="thiscall", this=PL)
    imgp = e.u32(PL); bases = [e.u32(imgp + 4 * k) for k in range(nimg)]
    sizes = []
    hdr = struct.calcsize("<HHHHIIIB3s") + struct.calcsize("<IHHHhIIHH"); p = hdr
    for k in range(nimg): ln = struct.unpack_from("<I", dat, p)[0]; sizes.append(ln); p += 4 + ln
    out.append(f"C {st['data'].hex()} {entry}")
    e.call(0x411f20, [], this=DISP); out.append("Z")
    for batch in range(rnd.randint(1, 3)):
        for _ in range(rnd.randint(1, 14)):
            k = rnd.randrange(nimg)
            x = rnd.choice([rnd.randint(-40, 80), rnd.randint(560, 680), rnd.randint(-20, 640)]); y = rnd.choice([rnd.randint(-8, 10), rnd.randint(470, 490), rnd.randint(0, 480)])
            if rnd.random() < 0.5: x, y = rnd.randint(0, 30) + 300, rnd.randint(100, 110)       # cluster: many overlaps
            depth = rnd.choice([0, 1, 2, 3, 3, 5, 65535, rnd.getrandbits(16)])
            e.call(0x411f80, [x & 0xffffffff, y & 0xffffffff, depth, bases[k], 0], conv="thiscall", this=DISP)
            out.append(f"S {k} {x} {y} {depth}")
        out.append("D"); out += dump(bases, sizes)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/render.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
