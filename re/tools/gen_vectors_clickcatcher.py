#!/usr/bin/env python3
"""Vectors for the kind 15 object (click catcher): runs the ORIGINAL constructor (0x415770 with the MidBase base 0x40ecb0
and the animated base 0x401aa0), OnAttach (0x4157b0), and slots 14 (0x4157c0), 15 (0x40f040), 16 and 17 (0x417b30) through
the object's own vtable in Unicorn, with a real animation player (loader 0x401160). Replaced by Python: pool allocator,
game clock, Room_InsertSorted (0x4104a0, logged).
Output: local-media/vectors/clickcatcher.txt (ignored). Consumed by re/tools/drivers/clickcatcher_driver.cpp.
Lines: B <hex bank file>
       K x y sortOffset roomOffset now data(hex) | state | results
state: x y prevX prevY sort flags approachX approachY facing mode pending answered / bank first last current flags start total lx ly
results: slot14 slot15 slot16 slot17 insertions(room matches this+0x3c)
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
    e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, b"\1"); e.write(REC + 0xc, b"CATCH\0"); e.write(REC + 0x1c, struct.pack("<I", 0x80))
    e.write(DATA + 0x80, b"\0" * 8)
    e.write(ROOM + 8, struct.pack("<h", ro))
    now = rnd.randint(0, 5000); st["now"] = now
    e.call(0x415770, [REC, ROOM, TBL, DATA], conv="thiscall", this=OBJ)
    e.write(OBJ + 0x3c, struct.pack("<I", ROOM))
    vt = e.u32(OBJ)
    verb = rnd.randint(0, 9)
    r14 = e.call(e.u32(vt + 14 * 4), [verb], conv="thiscall", this=OBJ) if False else None
    def slot(n, args=()): return e.call(e.u32(vt + 4 * n), list(args), conv="thiscall" if args else "cdecl", this=OBJ) & M
    cur = e.u32(OBJ + 0x38); p = e.read(cur, 0x40)
    state = (f"{s32(e.u32(OBJ+0x4c))} {s32(e.u32(OBJ+0x50))} {s32(e.u32(OBJ+0x54))} {s32(e.u32(OBJ+0x58))} {s16(e.read(OBJ+0x5e,2),0)} {e.u32(OBJ+0x68)} "
             f"{s32(e.u32(OBJ+0x94))} {s32(e.u32(OBJ+0x98))} {s32(e.u32(OBJ+0x90))} {s32(e.u32(OBJ+0x9c))} {e.read(OBJ+0x8c,1)[0]} {e.read(OBJ+0x8d,1)[0]} / "
             f"{e.read(OBJ+0x32,1)[0]} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)}")
    res = []
    res.append(slot(14, [verb])); res.append(slot(15, [verb])); res.append(slot(16)); res.append(slot(17))
    slot(0)
    res.append(f"{len(st['inserts'])}:{int(all(i == OBJ for i in st['inserts']))}")
    out.append(f"K {ox} {oy} {so} {ro} {now} {data.hex()} | {state} | {' '.join(map(str, res))} {verb}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/clickcatcher.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
