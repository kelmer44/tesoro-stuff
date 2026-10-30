#!/usr/bin/env python3
"""Vectors for Level: the ORIGINAL Level_Load 0x4080e0 (with AnimBank_Load, Room_Ctor, the object factory 0x409e50 and the
animated object constructor), Level_AttachObjects 0x408540, Level_ObjectMoved 0x408440, Level_FindObjectByKey 0x408780,
Level_CameraMoveTo 0x408520 and Level_UpdateRoomCameras 0x408550 in Unicorn on synthetic level files. CRT calls, the archive
seek (0x4032e0), the pool allocators and the game clock (0x413a10) are replaced by Python. Objects of kind 0 are built by the
real code; other kinds are numbers the factory has no class for (it returns null).
Output: local-media/vectors/level.txt (ignored). Consumed by re/tools/drivers/level_driver.cpp.
Lines: C <hex data> <entry> <mode> <is555> <now> / = <state> after the load and after every op below;
       F <obj> <flags>   set flags       A   attach all       M <obj>   object moved       K <lo> <hi>   find by key (= <obj or -1>)
       G <x> <y> <dur> <now>   camera move       U <now>   update room cameras (= state with every object's bounds)
       #  end of case
State: camera dwords, moved objects, per room (index divisor sort extent | objects), counts, banks, objects.
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261104)
M = 0xffffffff
LV, ARCH, FP, DISP, HEAP = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000
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
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b3f0, fseek, 3)
e.hook_function(0x41b600, fsetpos, 2)
e.hook_function(0x4032e0, seek_entry, 1, pops=True); e.hook_function(0x40dc10, alloc, 3, pops=True)
e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
e.hook_function(0x413a10, lambda emu: st["now"])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP)); e.write(0x442120, struct.pack("<I", 0x100400))

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def rb(n): return bytes(rnd.getrandbits(8) for _ in range(n))

def make_bank():
    """Four skipped bytes, then a small animation file: sequences of the simple types, images with no spans."""
    nseq = rnd.choice([1, 2, 3]); nimg = rnd.choice([0, 1, 2]) if nseq else 0
    out = bytearray(rb(4))
    out += struct.pack("<HHHHIIIB3s", nseq, rnd.getrandbits(16), nimg, rnd.getrandbits(16),
                       rnd.getrandbits(32), rnd.getrandbits(32), rnd.getrandbits(32), rnd.choice([0, 1, 1, 2]), b"\0\0\0")
    for _ in range(nseq):
        typ = rnd.choice([0, 1, 2, 4]); c = rnd.randint(0, 3)
        if typ == 2: extra = struct.pack("<H", c) + rb(4) + b"".join(struct.pack("<I", rnd.getrandbits(32)) for _ in range(c))
        elif typ == 4: extra = struct.pack("<H", c) + rb(6) + b"".join(struct.pack("<hhH", rnd.randint(-100, 300), rnd.randint(-50, 50), rnd.getrandbits(16)) for _ in range(c))
        else: extra = rb(rnd.randint(0, 8))
        out += struct.pack("<IHHHhIIHH", rnd.getrandbits(32), rnd.getrandbits(16), rnd.getrandbits(16), rnd.randint(1, 400),
                           rnd.randint(-1, nimg - 1) if nimg else -1, rnd.getrandbits(32), rnd.getrandbits(32), typ, len(extra)) + extra
    for _ in range(nimg):
        blob = struct.pack("<IIHHHH", 0x10, 0, rnd.getrandbits(16), 0, 1, rnd.getrandbits(16))
        out += struct.pack("<I", len(blob)) + blob
    return bytes(out)

def make_level(entry):
    nbank = rnd.choice([0, 1, 2, 3, 4]); nroom = rnd.choice([0, 1, 2, 3])
    banks = [make_bank() for _ in range(nbank)]; names = []
    rooms = []; total = 0
    for r in range(nroom):
        nobj = rnd.randint(0, 5) if total < 8 else 0; total += nobj
        rec = bytearray(rb(0x28)); struct.pack_into("<HHI", rec, 0x20, nobj, rnd.randint(0, 60), rnd.choice([rnd.randint(1, 900), (rnd.randint(-900, -1) & 0xffffffff), rnd.getrandbits(32) | 1]))
        objs = bytearray()
        for _ in range(nobj):
            kind = rnd.choice([0, 0, 0, 0, 0x12, 0x1a, 0x30, 0x100, 0xffff] + ([1, 1] if nbank else []))
            nb = rnd.randint(0, 3) if nbank else 0
            name = "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") for _ in range(rnd.randint(1, 11)))
            extra = rb(rnd.randint(0, 20))
            if kind == 1:      # camera frame: needs a bank; data = name of another object (or none), then three more values
                nb = max(nb, 1)
                pick = rnd.random()
                ref = b"" if pick < 0.2 else (rnd.choice(names).encode() if names and pick < 0.8 else rb(rnd.randint(1, 12)).replace(b"\0", b"A"))
                extra = ref.ljust(12, b"\0") + rb(5) + rb(rnd.randint(0, 6))
            names.append(name)
            orec = struct.pack("<iihBB12sII", rnd.randint(-700, 700), rnd.randint(-500, 500), rnd.randint(-40, 40), rnd.getrandbits(8), nb,
                               name.encode(), kind, len(extra))
            objs += orec + extra + bytes(rnd.randint(0, nbank - 1) for _ in range(nb))
        rooms.append(bytes(rec) + bytes(objs))
    body = struct.pack("<III", nroom, nbank, total) + b"".join(banks) + b"".join(rooms)
    return rb(entry + 4) + body + rb(rnd.randint(0, 9))

def walk(head):
    """Values of a list whose header is at `head` (first node pointer at +4, value at node+4)."""
    node = e.u32(head + 4); v = []
    while node and len(v) < 200:
        v.append(e.u32(node + 4)); node = e.u32(node)
    return v

def objects():
    return walk(e.u32(e.u32(LV + 0x74)))

def state(with_bounds=False):
    cam = e.u32(LV + 0x80); objs = objects(); oid = {a: i for i, a in enumerate(objs)}
    s = "cam " + " ".join(str(s32(e.u32(cam + o))) for o in range(4, 0x40, 4))
    s += " | mv " + (" ".join(str(oid[a]) for a in walk(e.u32(LV + 0x7c))) or "-")
    rooms = e.u32(LV + 0x70); nr = e.u32(LV + 0x84)
    for r in range(nr):
        R = e.read(rooms + 0x10 * r, 0x10)
        s += f" | room {u32(R,0)} {s32(u32(R,4))} {s16(R,8)} {s16(R,0xa)} : " + (" ".join(str(oid[a]) for a in walk(u32(R, 0xc))) or "-")
    s += f" | n {nr} {e.u32(LV + 0x88)} {len(objs)} {e.u32(LV + 0x8c)} {e.read(LV + 0x94, 1)[0]} {int(e.u32(LV + 0x90) != 0)}"
    banks = e.u32(LV + 0x6c)
    for b in range(e.u32(LV + 0x88)):
        p = e.read(e.u32(banks + 4 * b), 0x40); s += f" | bank {u16(p,0x22)} {u16(p,0x20)} {u32(p,0x18)}"
    for a in objs:
        o = e.read(a, 0x84); n = o[0x33]; arr = u32(o, 0x34)
        s += (f" | obj {u32(o,0x44)} {u32(o,0x48)} {o[0x5c]} {s32(u32(o,0x4c))} {s32(u32(o,0x50))} {s16(o,0x5e)} {n} {u32(o,0x68)} "
              f"{(u32(o,0x3c) - rooms) // 0x10} {s32(u32(o,0x54))} {s32(u32(o,0x58))}")
        for k in range(n):
            p = e.read(e.u32(arr + 4 * k), 0x40); s += f" {u16(p,0x22)}.{u16(p,0x20)}.{u32(p,0x18)}"
        if o[0x5c] == 1:
            s += f" k1 {o[0x6c:0x78].hex()} {oid.get(u32(o,0x78), -1)} {o[0x7d]} {u16(o,0x7e)} {u16(o,0x80)}"
        if with_bounds:
            s += " b " + " ".join(str(s32(u32(o, x))) for x in (0x10, 0x14, 0x18, 0x1c, 0x20, 0x24, 0x28, 0x2c)) + f" {u16(o,0x30)}"
    return s

out = []
for case in range(500):
    mode = rnd.choice([0, 0, 2]); is555 = rnd.random() < 0.5
    entry = rnd.randint(0, 40); now = rnd.choice([0, 1000, 123456])
    st["data"] = make_level(entry); st["pos"] = 0; st["heap"] = HEAP; st["now"] = now
    e.write(DISP + 0x67c, bytes([1 if is555 else 0])); e.write(LV, bytes([0xcd]) * 0x20); e.write(LV + 0x20, bytes(0x100))
    e.call(0x4080e0, [entry, mode], conv="thiscall", this=LV)
    out.append(f"C {st['data'].hex()} {entry} {mode} {int(is555)} {now}"); out.append("= " + state())
    objs = objects(); n = len(objs)
    if n:
        # some objects stand in their room's list; attach puts them there, ordered by x
        for i in range(n):
            if rnd.random() < 0.7:
                f = (e.u32(objs[i] + 0x68) & 0x10) | rnd.choice([0x2, 0x6, 0x6, 0x7, 0x4, 0x8 | 0x2, 0x6 | 0x8]) | 0x100
                if e.read(objs[i] + 0x33, 1)[0] == 0: f &= ~4   # without a player UpdateBounds reads a null pointer (the game never draws such an object)
                e.write(objs[i] + 0x68, struct.pack("<I", f)); out.append(f"F {i} {f}")
        e.call(0x408540, [], this=LV); out.append("A"); out.append("= " + state())
    t = now
    for _ in range(rnd.randint(0, 8)):
        op = rnd.choice("MMKGUU") if n else rnd.choice("GU")
        if op == "M":
            i = rnd.randrange(n)
            if rnd.random() < 0.3:
                f = (e.u32(objs[i] + 0x68) & ~8 & M) | rnd.choice([0, 8]); e.write(objs[i] + 0x68, struct.pack("<I", f)); out.append(f"F {i} {f}")
            e.call(0x408440, [objs[i]], conv="thiscall", this=LV); out.append(f"M {i}"); out.append("= " + state())
        elif op == "K":
            if rnd.random() < 0.7:
                o = e.read(objs[rnd.randrange(n)], 0x70); lo, hi = u32(o, 0x44), u32(o, 0x48)
                if rnd.random() < 0.3: hi ^= 1 << rnd.randrange(32)   # right low word, wrong high word
                elif rnd.random() < 0.15: lo ^= 1 << rnd.randrange(32)
            else: lo, hi = rnd.getrandbits(32), rnd.getrandbits(32)
            r = e.call(0x408780, [lo, hi], conv="thiscall", this=LV)
            out.append(f"K {lo} {hi}"); out.append(f"= {objs.index(r) if r in objs else -1}")
        elif op == "G":
            x, y, d = rnd.randint(-900, 900), rnd.randint(-700, 700), rnd.choice([0, 0, 100, 1000, 5000])
            t += rnd.choice([0, 1, 50, 700]); st["now"] = t
            e.call(0x408520, [x, y, d], conv="thiscall", this=LV); out.append(f"G {x} {y} {d} {t}"); out.append("= " + state())
        else:
            t += rnd.choice([0, 1, 50, 700, 4000]); st["now"] = t
            e.call(0x408550, [], this=LV)
            out.append(f"U {t}"); out.append("= " + state(True))
    if mode == 2 and rnd.random() < 0.5:
        pass
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/level.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
