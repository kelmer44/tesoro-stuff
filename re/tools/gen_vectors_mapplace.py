#!/usr/bin/env python3
"""Vectors for the kind 19 object (map place): runs the ORIGINAL constructor (0x417720, MidBase 0x40ec90, animated base 0x401aa0,
sequence selection 0x401de0) and the click handler slot 14 (0x4178b0) in Unicorn with a real animation player (loader 0x401160).
Replaced by Python: pool allocator, clock, phrase lookup (0x413940, logged), object lookup in a level (0x408780, logged), script
table lookup and start (0x4060c0, 0x405fd0, logged), Jim entering a level (0x407b80), Jim's set-facing slot, the pointer's label
(0x4023f0) and the exit object's arrival slot (all logged).
Output: local-media/vectors/mapplace.txt (ignored). Consumed by re/tools/drivers/mapplace_driver.cpp.
Lines: B <hex bank file>
       K x y sortOffset roomOffset now data(hex) canoaState saliaFacing verb | state | results
state: x y prevX prevY sort flags dest hoverSet / bank first last current flags start total lx ly mx my seqx seqy / phrase log
results: return value then the log of the click (find level lo hi; start lo hi vars levelIsOwn; enter level x y; face n; text; arrive)
       #   end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_ECX

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
log = []
def cstr(p, n=12):
    b = e.read(p, n); return b.split(b"\0")[0].decode("latin1")
def find_phrase(emu, namep): n = cstr(namep); log.append(f"phrase {n}"); return 0x100000 + (sum(map(ord, n)) & 0xff) + 1
e.hook_function(0x413940, find_phrase, 1)
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442120, struct.pack("<I", 0x100380))

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]

def bank_file(nseq, images):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, rnd.choice([0, 1, 1, 2, 3]), b"\0\0\0"))
    for _ in range(nseq):
        out += struct.pack("<hhhhHhIIHH", rnd.randint(-9, 9), rnd.randint(-9, 9), rnd.randint(0, 40), rnd.randint(-10, 10), rnd.randint(1, 300), -1, 0, 0, 0, 0)
    return bytes(out)

# The world the click handler looks into: 47 levels, one SALIDA object each, one CANOA object, Jim, the pointer.
def hash_name(name):
    p = mk(16); e.write(p, name.encode() + b"\0"); e.call(0x415c40, [p]); return e.uc.reg_read(UC_X86_REG_EAX), e.uc.reg_read(UC_X86_REG_EDX)
KEY_SAL, KEY_CAN = hash_name("SALIDA"), hash_name("CANOA")
LV = {}; LVP = []
for i in range(47):
    a = mk(0x100); e.write(a + 0x70, struct.pack("<I", 0x5000 + 0x10 * i)); LV[a] = i; LVP.append(a); e.write(0x442130 + 4 * i, struct.pack("<I", a))
VT = mk(0x100); e.write(VT, e.read(0x428000, 4 * 33))
JVT = mk(0x100); e.write(JVT, e.read(0x428000, 4 * 33))
SAL = mk(0x100); e.write(SAL, struct.pack("<I", VT)); CAN = mk(0x100); CANB = mk(0x10)
e.write(CAN + 0x60, struct.pack("<I", CANB))
JIM = mk(0x100); e.write(JIM, struct.pack("<I", JVT)); e.write(0x4420fc, struct.pack("<I", JIM))
CUR = mk(0x100); e.write(0x442128, struct.pack("<I", CUR)); e.write(0x4420b8, struct.pack("<I", mk(0x100)))
FNS = 0x4000000; e.uc.mem_map(FNS, 0x10000, 7)
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
slot(VT, 0x70, lambda emu: log.append("arrive"), 0)
slot(JVT, 0x74, lambda emu, f: log.append(f"face {s32(f)}"), 1)
def find_obj(emu, lo, hi):
    ecx = e.uc.reg_read(UC_X86_REG_ECX); idx = LV[ecx]; log.append(f"find {idx} {lo} {hi}")
    if (lo, hi) == KEY_SAL: return SAL
    if (lo, hi) == KEY_CAN: return CAN
    return 0
e.hook_function(0x408780, find_obj, 2, pops=True)
def find_script(emu, lo, hi): log.append(f"findscript {lo} {hi}"); return 0x100500
e.hook_function(0x4060c0, find_script, 2, pops=True)
def start_script(emu, lo, hi, vars_, level):
    log.append(f"start {lo} {hi} {vars_} {int(level == st['level'])}"); return 3
e.hook_function(0x405fd0, start_script, 4, pops=True)
def enter(emu, level, rooms, x, y):
    log.append(f"enter {LV.get(level, -9)} {s32(x)} {s32(y)} {int(rooms == 0x5000 + 0x10 * LV.get(level, 0))}"); return 0
e.hook_function(0x407b80, enter, 4, pops=True)
e.hook_function(0x4023f0, lambda emu, text, col: log.append(f"text {text} {s32(col)}"), 2, pops=True)

WORLD_END = st["heap"]
out = []
for case in range(300):
    st["heap"] = WORLD_END; st["inserts"] = []
    nseq = rnd.choice([1, 2, 4, 6]); bdat = bytes(4) + bank_file(nseq, 0)
    PB = mk(0x40); st["data"] = bdat; st["pos"] = 0
    e.write(PB, bytes(0x40)); e.call(0x401160, [0, 0, 0], conv="thiscall", this=PB)
    out.append("B " + bdat.hex())
    OBJ, REC, ROOM, TBL, DATA, TP = mk(0x100), mk(0x40), mk(0x20), mk(0x20), mk(0x100), mk(0x40)
    e.write(TP, bytes(0x40)); e.call(0x401580, [PB], this=TP); e.write(TBL, struct.pack("<I", TP))
    dest = rnd.choice([-128, -3, 0, 6, 7, 7, 8, 9, 14, 15, 15, 15, 16, 20, 30, 46] + list(range(-5, 47)))
    d3 = rnd.choice([0, 0, ord("M"), ord("a"), ord("Z")])
    name = bytes(rnd.choice(b"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") for _ in range(rnd.randint(1, 11))) + b"\0"
    data = bytes([rnd.randint(0, nseq - 1), rnd.getrandbits(8), dest & 0xff]) + (bytes([d3]) + name[1:] if d3 == 0 else name) + bytes(rnd.getrandbits(8) for _ in range(0x60))
    ox, oy, so, ro = rnd.randint(-3000, 3000), rnd.randint(-3000, 3000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    e.write(DATA, data); e.write(DATA + 0x100 - 8, b"\0" * 8)
    e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, b"\1"); e.write(REC + 0xc, b"MAPPLACE\0"); e.write(REC + 0x1c, struct.pack("<I", 0x80))
    e.write(DATA + 0x80, b"\0" * 8)
    e.write(ROOM + 8, struct.pack("<h", ro))
    now = rnd.randint(0, 5000); st["now"] = now
    del log[:]
    e.call(0x417720, [REC, ROOM, TBL, DATA, 0], conv="thiscall", this=OBJ)
    plog = ",".join(l.replace(" ", "_") for l in log) or "-"; del log[:]
    LEVEL = mk(0x40); st["level"] = LEVEL
    e.write(OBJ + 0x3c, struct.pack("<I", ROOM)); e.write(OBJ + 0x40, struct.pack("<I", LEVEL))
    vt = e.u32(OBJ)
    cur = e.u32(OBJ + 0x38); p = e.read(cur, 0x40)
    cn = e.u32(OBJ + 0x30 * 0 + 0x38)
    seqrec = e.u32(cur + 0x1c) + u16(p, 0x30) * 0x24; sq = e.read(seqrec, 0x24)
    state = (f"{s32(e.u32(OBJ+0x4c))} {s32(e.u32(OBJ+0x50))} {s32(e.u32(OBJ+0x54))} {s32(e.u32(OBJ+0x58))} {s16(e.read(OBJ+0x5e,2),0)} {e.u32(OBJ+0x68)} "
             f"{struct.unpack('<b', e.read(OBJ+0xa0,1))[0]} {int(e.u32(OBJ+0x6c) != 0)} / "
             f"{e.read(OBJ+0x32,1)[0]} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)} {s16(sq,4)} {s16(sq,6)} / {plog}")
    canoa = rnd.choice([0, 0, 1, 7]); salf = rnd.randint(0, 9)
    e.write(CANB, bytes([canoa])); e.write(SAL + 0x90, struct.pack("<I", salf)); e.write(SAL + 0x94, struct.pack("<ii", rnd.randint(-999, 999), rnd.randint(-999, 999)))
    sx, sy = struct.unpack("<ii", e.read(SAL + 0x94, 8))
    verb = rnd.randint(0, 9)
    r = e.call(e.u32(vt + 4 * 14), [verb], conv="thiscall", this=OBJ) & M
    out.append(f"K {ox} {oy} {so} {ro} {now} {data.hex()} {canoa} {salf} {sx} {sy} {verb} | {state} | {r}:{','.join(l.replace(' ', '_') for l in log) or '-'}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/mapplace.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
