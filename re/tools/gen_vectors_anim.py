#!/usr/bin/env python3
"""Vectors for the animation bank: runs the ORIGINAL AnimBank_Load (0x401160) with AnimBank_LoadImage (0x4015a0),
AnimBank_LoadDeferredImages (0x401510), AnimBank_ScaleBrightness (0x401010) and AnimPlayer_SetRange (0x401740)
in Unicorn on synthetic animation files. CRT calls, the archive seek (0x4032e0), the pool allocator (0x40dc10) and
the game clock (0x413a10) are replaced by Python functions.
Output: local-media/vectors/anim.txt (ignored). Consumed by re/tools/drivers/anim_driver.cpp.
Lines: C <hex dat> <entryOffset> <mode> <is555>   load; then
       H seqs images h0 h1 h2 flags / S <sequence> / P <image positions> / I <image blob, pointers rebased | ->
       X  load deferred images, then D <blob>...    Y <factor>  scale brightness, then J <blob>...
       R first last loop sum now | startTime totalDuration flags first last loopX loopY current
       A now | movementX movementY current flags startTime total loopX loopY    advance (0x401860)
       Z now | flags startTime pausedAt    pause (0x401a50)      W now | ...   resume (0x401a70)
       #  end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261029)
M = 0xffffffff
PL, ARCH, FP, DISP, HEAP = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}

def fread(emu, buf, size, count, fp):
    n = size * count; chunk = st["data"][st["pos"]:st["pos"] + n]
    emu.write(buf, chunk); st["pos"] += len(chunk)
    return len(chunk) // size if size else 0
def seek_entry(emu, offset): st["pos"] = offset; return FP
def fgetpos(emu, fp, out): emu.write(out, struct.pack("<Q", st["pos"])); return 0
def fsetpos(emu, fp, pos): st["pos"] = struct.unpack("<Q", emu.read(pos, 8))[0]; return 0
def fseek(emu, fp, off, whence):
    assert whence == 1; st["pos"] += struct.unpack("<i", struct.pack("<I", off))[0]; return 0
def alloc(emu, size, align, arena):
    a = (st["heap"] + align - 1) // align * align; st["heap"] = a + size; return a
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b3f0, fseek, 3)
e.hook_function(0x41b600, fsetpos, 2)
e.hook_function(0x4032e0, seek_entry, 1, pops=True); e.hook_function(0x40dc10, alloc, 3, pops=True)
e.hook_function(0x413a10, lambda emu: st["now"])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def hx(b): return b.hex() or "-"

def make_blob(is555):
    typ = rnd.choice([1, 1, 1, 2, 0, 3, 7]); nsp = rnd.randint(0, 3); counts = [rnd.randint(0, 4) for _ in range(nsp)]
    pix = 2 * rnd.randint(0, 24); trail = 2 * rnd.randint(0, 3)
    pixbytes = pix if rnd.random() < 0.7 else rnd.randint(0, pix)
    head = 0x10 + 8 * nsp; items = sum(8 * c for c in counts)
    pixel_first = (not is555) and rnd.random() < 0.5
    if pixel_first: pixoff, itoff = head, head + pix + trail
    else: pixoff, itoff = head + items, head
    size = head + items + pix + trail
    b = bytearray(rnd.getrandbits(8) for _ in range(size))
    struct.pack_into("<IIHHHH", b, 0, pixoff, pixbytes, rnd.getrandbits(16), nsp, typ, rnd.getrandbits(16))
    o = itoff
    for s, c in enumerate(counts):
        struct.pack_into("<IHH", b, 0x10 + 8 * s, o, c, rnd.getrandbits(16))
        for k in range(c):
            struct.pack_into("<II", b, o + 8 * k, rnd.randint(0, size) if typ == 1 else rnd.getrandbits(32), rnd.getrandbits(32))
        o += 8 * c
    return bytes(b)

def make_file(mode, is555, safe=False):
    nseq = rnd.choice([0, 1, 2, 3, 4, 6, 9]); nimg = rnd.choice([0, 1, 2, 3, 5]) if nseq else 0
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, rnd.getrandbits(16), nimg, rnd.getrandbits(16),
                                rnd.getrandbits(32), rnd.getrandbits(32), rnd.getrandbits(32), rnd.choice([0, 1, 1, 2]), b"\0\0\0"))
    for _ in range(nseq):
        typ = rnd.choice([0, 1, 2, 4, 4, 2]); c = rnd.randint(0, 5)
        if typ == 2: extra = struct.pack("<H", c) + bytes(rnd.getrandbits(8) for _ in range(4)) + b"".join(struct.pack("<I", rnd.getrandbits(32) if rnd.random() < .4 else rnd.randint(0, 50)) for _ in range(c))
        elif typ == 4:
            xs = sorted(rnd.randint(-100, 300) for _ in range(c))
            if c > 1 and rnd.random() < .4: rnd.shuffle(xs)
            extra = struct.pack("<H", c) + bytes(rnd.getrandbits(8) for _ in range(6)) + b"".join(struct.pack("<hhH", x, rnd.randint(-50, 50), rnd.getrandbits(16)) for x in xs)
        else: extra = bytes(rnd.getrandbits(8) for _ in range(rnd.randint(0, 12)))
        out += struct.pack("<IHHHhIIHH", rnd.getrandbits(32), rnd.getrandbits(16), rnd.getrandbits(16), rnd.randint(1, 400) if safe else rnd.getrandbits(16),
                           rnd.randint(-1, nimg - 1) if nimg else -1, rnd.getrandbits(32), rnd.getrandbits(32), typ, len(extra)) + extra
    sizes = []
    for _ in range(nimg):
        blob = make_blob(is555); sizes.append(len(blob)); out += struct.pack("<I", len(blob)) + blob
    return bytes(out), sizes

def rebase(blob, B):
    b = bytearray(blob); struct.pack_into("<I", b, 0, (u32(b, 0) - B) & M)
    typ = u16(b, 0xc)
    for s in range(u16(b, 0xa)):
        ip, cnt = u32(b, 0x10 + 8 * s), u16(b, 0x10 + 8 * s + 4); io = (ip - B) & M
        struct.pack_into("<I", b, 0x10 + 8 * s, io)
        if typ == 1:
            for k in range(cnt): struct.pack_into("<I", b, io + 8 * k, (u32(b, io + 8 * k) - B) & M)
    return bytes(b)

def dump_images(prefix, sizes):
    out = []; imgs = e.u32(PL)
    for k, size in enumerate(sizes):
        B = e.u32(imgs + 4 * k)
        out.append(f"{prefix} " + ("-" if B == 0 else hx(rebase(e.read(B, size), B))))
    return out

def dump_bank(sizes, mode):
    nseq, nimg = u16(e.read(PL + 0x22, 2), 0), u16(e.read(PL + 0x20, 2), 0)
    h = e.read(PL + 0x32, 12); out = [f"H {nseq} {nimg} {u32(h,0)} {u32(h,4)} {u32(h,8)} {e.u32(PL + 0x18)}"]
    seqs, imgs, pos = e.u32(PL + 0x1c), e.u32(PL), e.u32(PL + 4)
    for i in range(nseq):
        r = e.read(seqs + 0x24 * i, 0x24); typ = u16(r, 0xa); cnt = u16(r, 0x1c); slot = u32(r, 0x18)
        idx = (slot - imgs) // 4 if slot else -1
        ent = scl = b""; fd = 0
        if typ in (2, 4) and cnt: ent = e.read(u32(r, 0x20), 4 * cnt); fd = u16(r, 0x1e)
        if typ == 4 and cnt: scl = e.read(u32(r, 0x14), 2 * cnt)
        out.append(f"S {u32(r,0)} {s16(r,4)} {s16(r,6)} {u16(r,8)} {typ} {u32(r,0xc)} {u32(r,0x10)} {idx} {cnt if typ in (2,4) else 0} {fd} {hx(ent)} {hx(scl)}")
    out.append("P " + (",".join(str(e.u32(pos + 4 * k)) for k in range(nimg)) or "-"))
    out += dump_images("I", sizes) if mode != 2 else ["I -"] * nimg
    return out

out = []
for case in range(700):
    mode = rnd.choice([0, 0, 2, 2, 2]); is555 = rnd.random() < 0.5
    safe = rnd.random() < 0.5
    dat, sizes = make_file(mode, is555, safe); entry = rnd.randint(0, 40)
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat + bytes(rnd.getrandbits(8) for _ in range(rnd.randint(0, 9)))
    st["pos"] = 0; st["heap"] = HEAP
    e.write(DISP + 0x67c, bytes([1 if is555 else 0])); e.write(PL, bytes([0xcd]) * 0x40)
    e.call(0x401160, [entry, 0, mode], conv="thiscall", this=PL)
    out.append(f"C {st['data'].hex()} {entry} {mode} {int(is555)}")
    out += dump_bank(sizes, mode)
    nseq = u16(e.read(PL + 0x22, 2), 0)
    if mode == 2:
        e.call(0x401510, [], this=PL); out.append("X"); out += dump_images("D", sizes)
    if rnd.random() < 0.6:
        f = rnd.choice([0, 1, 100, 200, 255, 256, 257, 300, 512, 1000, 4095, 65535, rnd.getrandbits(16)])
        e.call(0x401010, [f], conv="thiscall", this=PL); out.append(f"Y {f}"); out += dump_images("J", sizes)
    for _ in range(rnd.randint(0, 5)):
        sm, lp = rnd.choice([0, 1]), rnd.choice([0, 1])
        if nseq == 0: first, last = rnd.randint(0, 3), rnd.choice([-1, 0, 2])
        elif sm: first, last = rnd.randint(0, nseq - 1), rnd.choice([-1, rnd.randint(0, nseq - 1)])
        else: first, last = rnd.randint(0, nseq + 1), rnd.choice([-1, rnd.randint(0, nseq + 3)])
        now = rnd.choice([rnd.getrandbits(31), rnd.randint(-5, 1000), -rnd.getrandbits(31)]); st["now"] = now & M
        e.call(0x401740, [first, last & M, lp, sm], conv="thiscall", this=PL)
        p = e.read(PL, 0x40)
        out.append(f"R {first} {last} {lp} {sm} {now} | {s32(u32(p,8))} {u32(p,0xc)} {u32(p,0x18)} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x2c)} {u16(p,0x2e)} {u16(p,0x30)}")
    if safe and nseq > 0:
        # play head: SetRange, then advance / pause / resume at increasing game times
        first = rnd.randint(0, nseq - 1); last = rnd.choice([-1, rnd.randint(first, nseq - 1)]); lp = rnd.choice([0, 1]); sm = rnd.choice([0, 1])
        t = rnd.choice([0, 1000, 123456]); st["now"] = t
        e.call(0x401740, [first, last & M, lp, sm], conv="thiscall", this=PL)
        p = e.read(PL, 0x40)
        out.append(f"R {first} {last} {lp} {sm} {t} | {s32(u32(p,8))} {u32(p,0xc)} {u32(p,0x18)} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x2c)} {u16(p,0x2e)} {u16(p,0x30)}")
        seen_pause = False   # the pause time is uninitialised until the first pause
        for _ in range(rnd.randint(1, 9)):
            op = rnd.choice("AAAAZW" if seen_pause else "AAAAZ"); seen_pause |= op == "Z"; t += rnd.choice([0, 1, 5, 17, 100, 333, 1500, 5000, 20000]); st["now"] = t
            if op == "A":
                e.call(0x401860, [], this=PL); p = e.read(PL, 0x40)
                out.append(f"A {t} | {s16(p,0x28)} {s16(p,0x2a)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)}")
            else:
                e.call(0x401a50 if op == "Z" else 0x401a70, [], this=PL); p = e.read(PL, 0x40)
                out.append(f"{op} {t} | {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0x10)}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/anim.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
