#!/usr/bin/env python3
"""Vectors for the text queue: runs the ORIGINAL Display_ResetRenderQueue (0x411f20), Display_DrawText (0x411c80, the layout) and Display_Compose
(0x412e10, which draws the queued texts with 0x412c50; a solid 640x480 background is queued first) in Unicorn on synthetic font banks (95..190 glyph sequences loaded by the original bank
loader 0x401160, glyphs of literal and solid images, two fonts) and random texts, colours, positions and widths. After each case the queued
records (text, x, y, colour, font) and the CRC-32 of every frame row (frame pre-filled with a pattern, no other image queued) are dumped.
Cases are dropped when they overrun the original's buffers: a line over 100 characters or more than 29 lines in a frame (its 30 record slots end at its counter). Texts have no control characters and none beyond the font.
Usage: gen_vectors_text.py (from the repository root). Output: local-media/vectors/text.txt (ignored), read by drivers/text_driver.cpp.
Lines: C <hex file> <entry>  (font 0, then font 1) | Z | B <hex background file, entry 0> | A <hex text> <colour> <font> <cx> <y> <width> <tag> <centred> <lineHeight> <555> | K |
       then N <count> | R <hex text> <x> <y> <colour> <font> | T <layer tag> | F <crc of row 0> ... <crc of row 479>   | #"""
import os, random, struct, sys, zlib
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261103)
PL, ARCH, FP, DISP, HEAP, ROWS, FRAME = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000, 0x3000000, 0x3200000
PL2 = 0x100c00
e = Emu(); e.uc.mem_map(DISP, 0x40000, 3); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(ROWS, 0x150000, 3); e.uc.mem_map(FRAME, 0xa0000, 3)
e.uc.mem_map(0x3400000, 0x10000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def make_blob")])
e.write(DISP + 0x670, struct.pack("<I", ROWS)); e.write(DISP + 0x674, struct.pack("<I", ROWS + 240000 * 4))

def make_glyph():
    """An image of W x H pixels: literal (pixels) or solid (colour 0 or not)."""
    solid = rnd.random() < 0.3
    W, H = rnd.randint(2, 14), rnd.randint(2, 12); rows = []
    for _ in range(H):
        items = []; x = 0
        while x < W and rnd.random() < 0.8:
            gap = rnd.randint(0, 2); ln = rnd.randint(1, 5)
            if x + gap + ln > W: break
            items.append((gap, ln)); x += gap + ln
        rows.append(items)
    nitems = sum(len(r) for r in rows); pix = 2 * max(1, sum(l for r in rows for _, l in r))
    head = 0x10 + 8 * H; pixoff = head + 8 * nitems; size = pixoff + pix
    b = bytearray(size)
    for i in range(pixoff, size): b[i] = rnd.getrandbits(8)
    struct.pack_into("<IIHHHH", b, 0, pixoff, pix, W, H, 3 if solid else 1, 0)
    o = head; p = 0
    for r, items in enumerate(rows):
        struct.pack_into("<IHH", b, 0x10 + 8 * r, o, len(items), 0)
        for gap, ln in items:
            struct.pack_into("<IHH", b, o, rnd.choice([0, 0, 1, rnd.getrandbits(32)]) if solid else pixoff + p, gap, ln)
            p += 2 * ln; o += 8
    return bytes(b), W, H

def make_font(nglyph):
    """nglyph sequences (glyph c is sequence c - 0x21; sequence 94 is the space), each with its own image; 95..190 of them."""
    out = bytearray(struct.pack("<HHHHIIIB3s", nglyph, 0, nglyph, 0, 0, 0, 0, 1, b"\0\0\0"))
    glyphs = [make_glyph() for _ in range(nglyph)]
    base_y = rnd.randint(8, 14)
    for k, (blob, W, H) in enumerate(glyphs):
        adv = rnd.randint(6, 13)
        img_y = base_y - rnd.randint(0, 4) if k != 94 else base_y
        hitH = rnd.randint(8, 16)
        out += struct.pack("<hhhhHhhhhhHH", 0, img_y, 0, 0, 10, k, 0, 0, adv, hitH, 0, 0)
    for blob, W, H in glyphs: out += struct.pack("<I", len(blob)) + blob
    return bytes(out)

def make_background():
    """A 640x480 solid image (0xabcd) so that every row is covered, as the game's own background does."""
    H, W = 480, 640; head = 0x10 + 8 * H; itoff = head; size = head + 8 * H
    b = bytearray(size); struct.pack_into("<IIHHHH", b, 0, size, 0, W, H, 3, 0)
    for r in range(H):
        struct.pack_into("<IHH", b, 0x10 + 8 * r, itoff + 8 * r, 1, 0)
        struct.pack_into("<IHH", b, itoff + 8 * r, 0xabcd, 0, W)
    out = bytearray(struct.pack("<HHHHIIIB3s", 1, 0, 1, 0, 0, 0, 0, 1, b"\0\0\0"))
    out += struct.pack("<IHHHhIIHH", 0, 0, 0, 10, 0, 0, 0, 0, 0) + struct.pack("<I", len(b)) + bytes(b)
    return bytes(out)

def load_font(pl, dat, entry):
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0; st["pos"] = 0
    e.write(pl, bytes(0x40)); e.call(0x401160, [entry, 0, 0], conv="thiscall", this=pl)
    return "C " + st["data"].hex() + f" {entry}"

def cstr(a):
    b = bytearray()
    while True:
        c = e.read(a, 1)
        if c == b"\0": return bytes(b)
        b += c; a += 1

case = 0
def crcs():
    e.write(FRAME, bytes([0xab, 0xcd]) * 307200)
    e.write(DISP + 0x66a, b"\x01"); e.write(DISP + 0x678, struct.pack("<I", FRAME))
    try: e.call(0x412e10, [], this=DISP)
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP
        print("compose fault at eip %x" % e.uc.reg_read(UC_X86_REG_EIP), file=sys.stderr); raise
    f = e.read(FRAME, 614400)
    if os.environ.get("DUMPCASE") == str(case): open("local-media/text_orig.bin", "wb").write(f)
    return "F " + " ".join(str(zlib.crc32(f[1280 * y:1280 * (y + 1)])) for y in range(480))

WORDS = ["a", "un", "el", "tesoro", "Alcachofa", "isla", "capitan", "pirata", "!hola!", "?", "ab,cd", "0123456789", "supercalifragilistico", "x" * 17]
def make_text(nglyph):
    words = []
    for _ in range(rnd.randint(1, 9)):
        w = rnd.choice(WORDS)
        if rnd.random() < 0.15: w = bytes(rnd.choice(range(0x21, min(0x21 + nglyph, 0x100))) for _ in range(rnd.randint(1, 6))).decode("latin1")
        words.append(w)
    t = (" " * rnd.choice([0, 0, 0, 1, 2])) + (" " * rnd.choice([1, 1, 1, 2])).join(words) + (" " * rnd.choice([0, 0, 1]))
    return t.encode("latin1")

out = []
dropped = 0
for case in range(400):
    start = len(out); overflow = False; st["heap"] = HEAP
    nglyph = rnd.choice([95, 95, 130, 190])
    f0, f1 = make_font(nglyph), make_font(nglyph)
    e.write(DISP + 0x67c, b"\0")           # the loader converts the pixels when the flag is set: the fonts are 5-6-5
    out.append(load_font(PL, f0, rnd.randint(0, 20))); out.append(load_font(PL2, f1, rnd.randint(0, 20)))
    e.write(DISP + 0x20, struct.pack("<II", PL, PL2)); e.write(DISP + 0x67c, b"\0")
    # the font's own metrics the generator needs to keep texts short enough
    e.call(0x411f20, [], this=DISP); out.append("Z")
    st["data"] = bytes(4) + make_background(); st["pos"] = 0
    out.append("B " + st["data"].hex()); e.write(0x100c40, bytes(0x40)); e.call(0x401160, [0, 0, 0], conv="thiscall", this=0x100c40)
    e.call(0x411f80, [0, 0, 65535, e.u32(e.u32(0x100c40)), 0], conv="thiscall", this=DISP)
    for _ in range(rnd.randint(1, 4)):
        text = make_text(nglyph); colour = rnd.choice([0xffff, 0xffe0, 0x1234, rnd.getrandbits(16)]); font = rnd.randint(0, 1)
        cx = rnd.choice([320, rnd.randint(0, 640), rnd.randint(100, 540)]); y = rnd.choice([rnd.randint(-30, 480), rnd.randint(40, 200), 240, 470])
        width = rnd.choice([640, rnd.randint(120, 700), rnd.randint(150, 400), 130]); tag = rnd.choice([0, 0, 5, rnd.getrandbits(16)])
        centred = rnd.random() < 0.6; lh = rnd.choice([0, 0, rnd.randint(6, 30)]); f555 = int(rnd.random() < 0.3)
        e.write(DISP + 0x67c, bytes([f555]))
        n0 = e.u32(DISP + 0x13a0)
        e.write(0x3400000, text + b"\0")
        try: e.call(0x411c80, [0x3400000, colour, font, cx & 0xffffffff, y & 0xffffffff, width, tag, int(centred), lh], conv="thiscall", this=DISP)
        except Exception:
            from unicorn.x86_const import UC_X86_REG_EIP
            print("layout fault eip %x case %d text %r font %d width %d nglyph %d" % (e.uc.reg_read(UC_X86_REG_EIP), case, text, font, width, nglyph), file=sys.stderr); raise
        too_long = any(len(cstr(DISP + 0x680 + 0x70 * i)) >= 100 for i in range(n0, e.u32(DISP + 0x13a0)))
        out.append(f"A {text.hex()} {colour} {font} {cx} {y} {width} {tag} {int(centred)} {lh} {f555}")
        if too_long or e.u32(DISP + 0x13a0) > 29: overflow = True; break        # the original's 30 record slots end at its counter
    if not overflow and rnd.random() < 0.5:
        # Boundary cases: a line whose left edge is exactly the margin, or whose right edge is exactly at the margin, or a y equal to the line height.
        pl = rnd.choice([PL, PL2]); font = 0 if pl == PL else 1
        sp = struct.unpack("<h", e.read(e.u32(pl + 0x1c) + 94 * 0x24 + 0x10, 2))[0] + 2; widest = (320 - sp) * 2
        word = rnd.choice(WORDS[:8]).encode("latin1"); colour = rnd.getrandbits(16)
        e.write(DISP + 0x67c, b"\0")
        def add(cx, y, centred, lh):
            e.write(0x3400000, word + b"\0"); n = e.u32(DISP + 0x13a0)
            e.call(0x411c80, [0x3400000, colour, font, cx & 0xffffffff, y & 0xffffffff, widest, 0, int(centred), lh], conv="thiscall", this=DISP)
            out.append(f"A {word.hex()} {colour} {font} {cx} {y} {widest} 0 {int(centred)} {lh} 0"); return n
        n = add(320, 100, True, 0); w2 = 320 - struct.unpack("<i", e.read(DISP + 0x680 + 0x70 * n + 0x64, 4))[0]
        add(sp + w2, 150, True, 0); add(640 - sp - w2, 200, True, 0)
        lh = rnd.randint(8, 30); add(320, lh, False, lh); add(320, lh, True, lh * 2)
        if e.u32(DISP + 0x13a0) > 29: overflow = True
    if overflow: del out[start:]; dropped += 1; continue
    n = e.u32(DISP + 0x13a0)
    out.append("K"); out.append(f"N {n}")
    for i in range(n):
        r = DISP + 0x680 + 0x70 * i
        x, yy = struct.unpack("<ii", e.read(r + 0x64, 8)); col, fnt = struct.unpack("<HH", e.read(r + 0x6c, 4))
        out.append(f"R {cstr(r).hex() or '-'} {x} {yy} {col} {fnt}")
    out.append("T " + str(struct.unpack("<H", e.read(DISP + 0x668, 2))[0]))
    out.append(crcs()); out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/text.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines,", dropped, "cases dropped", file=sys.stderr)
