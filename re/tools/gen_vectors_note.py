#!/usr/bin/env python3
"""Vectors for the kind 24 object (note object): runs the ORIGINAL constructor (0x4175b0, on the clickable base 0x40ed10) and
slots 21 and 22 (0x4175f0) through the object's own vtable in Unicorn, with a real animation player (loader 0x401160).
Replaced by Python: pool allocator, game clock, phrase lookup (no phrases), the script table lookup (0x4060c0) and the
script start (0x405fd0), both logged; the start hook also writes through the variables pointer.
Output: local-media/vectors/note.txt (ignored). Consumed by re/tools/drivers/note_driver.cpp.
Lines: B <hex bank file>
       K x y sortOffset roomOffset now data(hex) | state | results
state: x y prevX prevY sort flags approachX approachY facing mode pending answered data / bank first last current flags start total lx ly
results: for slot 21 then 22: return value, log (find lo hi; start lo hi valueSeen levelIsOwn), data after
       #   end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261031)
M = 0xffffffff
FP, ARCH, HEAP = 0x100200, 0x100100, 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0, "inserts": []}

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
def mk(size):
    a = (st["heap"] + 15) // 16 * 16; st["heap"] = a + size; e.write(a, bytes(size)); return a
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b3f0, fseek, 3)
e.hook_function(0x41b600, fsetpos, 2)
e.hook_function(0x4032e0, seek_entry, 1, pops=True); e.hook_function(0x40dc10, alloc, 3, pops=True)
e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
e.hook_function(0x413a10, lambda emu: st["now"])
e.hook_function(0x4104a0, lambda emu, obj: st["inserts"].append(obj) or 0, 1, pops=True)
e.hook_function(0x413940, lambda emu, name: 0, 1)
log = []
def find_script(emu, lo, hi): log.append(f"find {lo} {hi}"); return 0x100500
e.hook_function(0x4060c0, find_script, 2, pops=True)
def start_script(emu, lo, hi, vars_, level):
    v = e.u32(vars_); log.append(f"start {lo} {hi} {v} {int(level == st['level'])}"); e.write(vars_, struct.pack("<I", (v + 0x11) & M)); return 3
e.hook_function(0x405fd0, start_script, 4, pops=True)
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442120, struct.pack("<I", 0x100380))

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]

def bank_file(nseq, images):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, rnd.choice([0, 1, 1, 2, 3]), b"\0\0\0"))
    for _ in range(nseq):
        out += struct.pack("<hhhhHhIIHH", 0, 0, rnd.randint(0, 40), rnd.randint(-10, 10), rnd.randint(1, 300), -1, 0, 0, 0, 0)
    return bytes(out)

out = []
for case in range(200):
    st["heap"] = HEAP; st["inserts"] = []
    nseq = rnd.choice([1, 2, 4, 6]); bdat = bytes(4) + bank_file(nseq, 0)
    PB = mk(0x40); st["data"] = bdat; st["pos"] = 0
    e.write(PB, bytes(0x40)); e.call(0x401160, [0, 0, 0], conv="thiscall", this=PB)
    out.append("B " + bdat.hex())
    OBJ, REC, ROOM, TBL, DATA, TP = mk(0x100), mk(0x40), mk(0x20), mk(0x20), mk(0x100), mk(0x40)
    e.write(TP, bytes(0x40)); e.call(0x401580, [PB], this=TP); e.write(TBL, struct.pack("<I", TP))
    data = bytes([rnd.randint(0, nseq + 1)]) + bytes(rnd.getrandbits(8) for _ in range(0x60))
    ox, oy, so, ro = rnd.randint(-3000, 3000), rnd.randint(-3000, 3000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    e.write(DATA, data); e.write(DATA + 0x100 - 8, b"\0" * 8)
    e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, b"\1"); e.write(REC + 0xc, b"NOTE\0"); e.write(REC + 0x1c, struct.pack("<I", 0x80))
    e.write(DATA + 0x80, b"\0" * 8)
    e.write(ROOM + 8, struct.pack("<h", ro))
    now = rnd.randint(0, 5000); st["now"] = now
    e.call(0x4175b0, [REC, ROOM, TBL, DATA], conv="thiscall", this=OBJ)
    LEVEL = mk(0x40); st["level"] = LEVEL
    e.write(OBJ + 0x3c, struct.pack("<I", ROOM)); e.write(OBJ + 0x40, struct.pack("<I", LEVEL))
    vt = e.u32(OBJ)
    def slot(n): return e.call(e.u32(vt + 4 * n), [], conv="cdecl", this=OBJ) & M
    cur = e.u32(OBJ + 0x38); p = e.read(cur, 0x40)
    state = (f"{s32(e.u32(OBJ+0x4c))} {s32(e.u32(OBJ+0x50))} {s32(e.u32(OBJ+0x54))} {s32(e.u32(OBJ+0x58))} {s16(e.read(OBJ+0x5e,2),0)} {e.u32(OBJ+0x68)} "
             f"{s32(e.u32(OBJ+0x94))} {s32(e.u32(OBJ+0x98))} {s32(e.u32(OBJ+0x90))} {s32(e.u32(OBJ+0x9c))} {e.read(OBJ+0x8c,1)[0]} {e.read(OBJ+0x8d,1)[0]} {e.u32(OBJ+0xa0)} / "
             f"{e.read(OBJ+0x32,1)[0]} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)}")
    res = []
    for n in (21, 22, 21):
        del log[:]
        r = slot(n)
        res.append(f"{r}:{','.join(l.replace(' ', '_') for l in log)}:{e.u32(OBJ+0xa0)}")
    out.append(f"K {ox} {oy} {so} {ro} {now} {data.hex()} | {state} | {' '.join(res)}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/note.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
