#!/usr/bin/env python3
"""Vectors for the kind 16 sprite (path sprite): runs the ORIGINAL constructor (0x40e7e0 with the kind 3 and animated
base constructors), OnAttach (0x40ebd0) and animation tick (0x40e890, with AnimPlayer_Advance 0x401860, SelectAnimBank
0x401de0 and the CRT sqrt/ftol) in Unicorn on synthetic animation files. The animation loader (0x401160) is the original
too. Replaced by Python: CRT file calls, archive seek, pool allocator, game clock (0x413a10), the two random functions
(0x413b00, 0x413ac0: results come from the vector and are recorded), the object list search (0x40f680: returns the
anchor), Room_InsertSorted (0x4104a0).
Output: local-media/vectors/pathsprite.txt (ignored). Consumed by re/tools/drivers/pathsprite_driver.cpp.
Lines: A <hex file> / S <hex file>    anchor's and sprite's animation file (loaded at entry 0, mode 0)
       K x y sortOffset roomOffset now name12(hex) ax ay data(hex) | state | randoms     construct + attach
       T now | state | randoms                                                        one tick
       #   end of case
state: x y stamp wait travelling path point covered scale / moveX moveY current flags start total loopX loopY
randoms: tokens N:lo:hi:result (0x413b00) or A:lo:hi:result (0x413ac0), "-" if none
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261031)
M = 0xffffffff
PA, PS, AO, APL, OBJ, TBL, TP, REC, ROOM, LV, DATA, IDX, KEYBUF = (0x100800, 0x100840, 0x100c00, 0x101000, 0x101200, 0x101400,
    0x101500, 0x101600, 0x101680, 0x101a00, 0x101700, 0x101780, 0x1017c0)
FP, ARCH, DISP, HEAP = 0x100200, 0x100100, 0x1200000, 0x2000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0, "vals": {"N": 0, "A": 0}, "log": [], "key": (0, 0)}

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
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def rand_fn(tag):
    def f(emu, lo, hi):
        lo, hi = s32(lo), s32(hi)
        res = st["vals"][tag]
        st["log"].append(f"{tag}:{lo}:{hi}:{res}"); return res & M
    return f
def find(emu, lo, hi): return AO if (lo, hi) == st["key"] else 0
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b3f0, fseek, 3)
e.hook_function(0x41b600, fsetpos, 2)
e.hook_function(0x4032e0, seek_entry, 1, pops=True); e.hook_function(0x40dc10, alloc, 3, pops=True)
e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
e.hook_function(0x413a10, lambda emu: st["now"])
e.hook_function(0x413b00, rand_fn("N"), 2, pops=True); e.hook_function(0x413ac0, rand_fn("A"), 2, pops=True)
e.hook_function(0x40f680, find, 2, pops=True); e.hook_function(0x4104a0, lambda emu, obj: 0, 1, pops=True)
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP)); e.write(0x442120, struct.pack("<I", 0x100380))
e.write(DISP + 0x67c, b"\0")

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]

def seq_record(x, y, dur, typ, extra):
    return struct.pack("<hhhhHhIIHH", rnd.randint(-5, 5), rnd.randint(-5, 5), x, y, dur, -1, 0, 0, typ, len(extra)) + extra

def anchor_file(nseq):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, 0, b"\0\0\0"))
    for _ in range(nseq):
        c = rnd.choice([2, 2, 3, 4, 5, 6]); x = rnd.randint(-200, 0); pts = []
        for _ in range(c):
            x += rnd.randint(-60, 160); pts.append((x, rnd.randint(-120, 120), rnd.choice([0x100, rnd.randint(0x20, 0x300)])))
        extra = struct.pack("<H", c) + bytes(6) + b"".join(struct.pack("<hhH", *p) for p in pts)
        out += seq_record(rnd.randint(-5, 5), rnd.randint(-5, 5), rnd.randint(1, 300), 4, extra)
    return bytes(out)

def sprite_file(nseq):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, rnd.choice([0, 1, 1, 2, 3, 3]), b"\0\0\0"))
    x = y = 0
    for _ in range(nseq):
        x += rnd.randint(0, 120); y += rnd.randint(-40, 40)
        out += seq_record(x, y, rnd.randint(1, 400), rnd.choice([0, 4]), b"" if rnd.random() < .5 else struct.pack("<H", 0) + bytes(6))
    return bytes(out)

def load(fp_addr, data):
    data = bytes(4) + data   # the loader skips a 4 byte size in front of the entry
    st["data"] = data; st["pos"] = 0
    e.write(fp_addr, bytes([0xcd]) * 0x40); e.call(0x401160, [0, 0, 0], conv="thiscall", this=fp_addr)
    return data

def state():
    cur = e.u32(OBJ + 0x38); p = e.read(cur, 0x40)
    return (f"{s32(e.u32(OBJ + 0x4c))} {s32(e.u32(OBJ + 0x50))} {s32(e.u32(OBJ + 0x74))} {s32(e.u32(OBJ + 0x78))} {e.read(OBJ + 0x8e, 1)[0]} "
            f"{s32(e.u32(OBJ + 0x90))} {s32(e.u32(OBJ + 0x94))} {s32(e.u32(OBJ + 0x98))} {s32(e.u32(OBJ + 0x9c))} / "
            f"{s16(p,0x28)} {s16(p,0x2a)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)}")

out = []; started = 0; finished = 0; cases = int(sys.argv[1]) if len(sys.argv) > 1 else 100
for case in range(cases):
    na = rnd.choice([1, 1, 2, 3]); ns = rnd.choice([2, 3, 4, 5])
    adat, sdat = anchor_file(na), sprite_file(ns); st["heap"] = HEAP
    adat = load(PA, adat); sdat = load(PS, sdat)
    out.append("A " + adat.hex()); out.append("S " + sdat.hex())
    ax, ay = rnd.randint(-500, 500), rnd.randint(-300, 300)
    e.write(AO, bytes(0x100)); e.write(APL, bytes([0xcd]) * 0x40); e.call(0x401580, [PA], this=APL)
    e.write(AO + 0x38, struct.pack("<I", APL)); e.write(AO + 0x4c, struct.pack("<ii", ax, ay))
    e.write(TP, bytes([0xcd]) * 0x40); e.call(0x401580, [PS], this=TP); e.write(TBL, struct.pack("<I", TP))
    name = "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(rnd.randint(1, 11)))
    e.write(KEYBUF, name.encode() + b"\0" * 13); e.call(0x415c40, [KEYBUF]); k = e.edx_eax(); st["key"] = (k & M, k >> 32)
    namebytes = name.encode() + b"\0" * (12 - len(name))
    lo_w, hi_w = rnd.randint(1, 150), rnd.randint(1, 400)
    lo_p = rnd.randint(0, na - 1); hi_p = rnd.randint(lo_p, na - 1)
    first = rnd.randint(0, ns - 1); mode = rnd.choice([0, 1, 2, 3])
    data = bytes([first, mode]) + namebytes + bytes([lo_p, hi_p]) + struct.pack("<HHH", rnd.getrandbits(16), lo_w, hi_w)
    ox, oy, so, ro = rnd.randint(-2000, 2000), rnd.randint(-2000, 2000), rnd.randint(-100, 100), rnd.randint(-100, 100)
    e.write(DATA, data + bytes(0x10)); e.write(IDX, b"\0")
    e.write(REC, bytes(0x20)); e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, b"\1")
    e.write(REC + 0xc, b"SPR\0"); e.write(REC + 0x1c, struct.pack("<I", IDX - DATA))
    e.write(ROOM, bytes(0x10)); e.write(ROOM + 8, struct.pack("<h", ro))
    e.write(LV, bytes(0x100)); e.write(OBJ, bytes(0x100))
    st["now"] = now0 = rnd.randint(0, 300)
    st["vals"] = {"N": rnd.randint(lo_w, max(lo_w, hi_w)), "A": rnd.randint(lo_p, hi_p)}; st["log"] = []
    e.call(0x40e7e0, [REC, ROOM, TBL, DATA], conv="thiscall", this=OBJ)
    e.write(OBJ + 0x40, struct.pack("<I", LV)); e.write(OBJ + 0x3c, struct.pack("<I", ROOM)); e.write(LV + 0x74, struct.pack("<I", 0x101800))
    e.call(0x40ebd0, [], this=OBJ)
    out.append(f"K {ox} {oy} {so} {ro} {now0} {namebytes.hex()} {ax} {ay} {data.hex()} | {state()} | {' '.join(st['log']) or '-'}")
    now = st["now"]
    for t in range(rnd.choice([600, 1000, 1500])):
        now += rnd.choice([1, 1, 1, 2, 2, 3, 5, 10, 40])
        st["now"] = now; st["log"] = []
        before = e.read(OBJ + 0x8e, 1)[0]
        st["vals"] = {"N": rnd.randint(lo_w, max(lo_w, hi_w)) if rnd.random() < .9 else rnd.randint(-3, 800), "A": rnd.randint(lo_p, hi_p)}
        try:
            e.call(0x40e890, [], this=OBJ)
        except Exception:
            from unicorn.x86_const import UC_X86_REG_EIP
            sys.exit(f"fault in case {case} tick {t} at {e.uc.reg_read(UC_X86_REG_EIP):#x}, state {state()}")
        after = e.read(OBJ + 0x8e, 1)[0]
        started += before == 0 and after == 1; finished += before == 1 and after == 0
        out.append(f"T {now} | {state()} | {' '.join(st['log']) or '-'}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/pathsprite.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines;", started, "trips started,", finished, "finished")
