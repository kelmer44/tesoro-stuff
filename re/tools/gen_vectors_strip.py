#!/usr/bin/env python3
"""Vectors for the inventory strip (kind 11): runs the ORIGINAL strip constructor (0x415250 with the MidBase and animated
base constructors), OnAttach (0x415290), IsInteractive (0x415350), ShowHoverText (0x415490), ClearHoverText (0x415640)
and SetVerbAndClick (0x415440) in Unicorn, with a real item group built by the original kind 2 constructor and attach
(0x414930, 0x414a90), real animation players (loader 0x401160, SelectAnimBank 0x401de0, SetRange 0x401740) on fake
picture, square and pointer objects. Replaced by Python: pool allocator, game clock, the object search (0x40f680),
Room_InsertSorted, and Cursor_SetText (0x4023f0, logged).
Output: local-media/vectors/strip.txt (ignored). Consumed by re/tools/drivers/strip_driver.cpp.
Lines: B <hex bank file>
       C n kinds(,) xs(,) ys(,) pointerX | data(hex of the group's record data)     the pictures and the group
       o <op> <arg> <now> | result texts | state     op: A attach, I IsInteractive, S ShowHoverText, X ClearHoverText, K click,
                                                      M pointer x := arg, J Jim's level := arg (0 free, 1 board, 2 menu, 3 new game menu),
                                                      H held := arg (picture index or -1), W now only
       #   end of case
state: slot hovered(index or -1) | strip anim | square anim | pointer anim held | picture x y anim ...  (anim: bank first last current flags start total lx ly)
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261031)
M = 0xffffffff
FP, ARCH, HEAP = 0x100200, 0x100100, 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3); e.set_fs()
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0, "keys": {}, "texts": 0}

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
def find(emu, lo, hi): return st["keys"].get((lo, hi), 0)
def settext(emu, text, colour): st["texts"] += 1; return 0
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b3f0, fseek, 3)
e.hook_function(0x41b600, fsetpos, 2)
e.hook_function(0x4032e0, seek_entry, 1, pops=True); e.hook_function(0x40dc10, alloc, 3, pops=True)
e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
e.hook_function(0x413a10, lambda emu: st["now"])
e.hook_function(0x40f680, find, 2, pops=True); e.hook_function(0x4104a0, lambda emu, obj: 0, 1, pops=True)
e.hook_function(0x4023f0, settext, 2, pops=True)
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442120, struct.pack("<I", 0x100380))

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]
def s32(v): return v - (1 << 32) if v & 0x80000000 else v

def bank_file(nseq):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, rnd.choice([0, 1, 1, 2, 3]), b"\0\0\0"))
    x = y = 0
    for _ in range(nseq):
        x += rnd.randint(0, 40); y += rnd.randint(-10, 10)
        out += struct.pack("<hhhhHhIIHH", 0, 0, x, y, rnd.randint(1, 300), -1, 0, 0, 0, 0)
    return bytes(out)

def key_of(name):
    buf = mk(32); e.write(buf, name + b"\0" * 16); e.call(0x415c40, [buf]); k = e.edx_eax(); return (k & M, k >> 32)

def fake_object(nbanks, PB, kind, x, y):
    o = mk(0x100); arr = mk(4 * nbanks); players = []
    for k in range(nbanks):
        p = mk(0x40); e.write(p, bytes([0xcd]) * 0x40); e.call(0x401580, [PB], this=p); players.append(p); e.write(arr + 4 * k, struct.pack("<I", p))
    e.write(o + 0x34, struct.pack("<I", arr)); e.write(o + 0x38, struct.pack("<I", players[0])); e.write(o + 0x32, b"\0")
    e.write(o + 0x4c, struct.pack("<ii", x, y)); e.write(o + 0x5c, bytes([kind])); e.write(o + 0x68, struct.pack("<I", 2 | 4))
    return o, players

def anim(o):
    cur = e.u32(o + 0x38); p = e.read(cur, 0x40)
    return f"{e.read(o + 0x32, 1)[0]} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)}"

def sel_all(o, nbanks, now):
    st["now"] = now
    for b in range(nbanks): e.call(0x401de0, [b, 0, 0, 0, 0], conv="thiscall", this=o)
    e.write(o + 0x32, b"\0"); e.write(o + 0x38, e.read(e.u32(o + 0x34), 4))

_call = e.call
def _dbg(addr, args=(), conv="cdecl", this=None, max_insns=2_000_000):
    try: return _call(addr, args, conv, this, max_insns)
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP
        sys.exit(f"fault calling {addr:#x} at {e.uc.reg_read(UC_X86_REG_EIP):#x}")
e.call = _dbg
out = []; cases = int(sys.argv[1]) if len(sys.argv) > 1 else 60
for case in range(cases):
    st["heap"] = HEAP; st["texts"] = 0
    bdat = bank_file(rnd.choice([4, 5, 6])); load_data = bytes(4) + bdat
    PB = mk(0x40); st["data"] = load_data; st["pos"] = 0
    e.write(PB, bytes([0xcd]) * 0x40); e.call(0x401160, [0, 0, 0], conv="thiscall", this=PB)
    out.append("B " + load_data.hex())
    n = rnd.choice([1, 2, 3, 5, 8, 12]); kinds = [rnd.choice([3, 3, 3, 4, 13]) for _ in range(n)]
    x0 = rnd.randint(-100, 100); step = rnd.randint(30, 70)
    xs = [x0 + step * i for i in range(n)]; ybase = rnd.choice([0x1e5, 0x1e0, 0x1df, 0x190, 0x230, 0x100])
    ys = [ybase + rnd.randint(-3, 3) if rnd.random() < .3 else ybase for _ in range(n)]
    px = rnd.choice(xs) + rnd.randint(-60, 60)
    pics = []; names = []
    for i in range(n):
        nm = f"PIC{i}".encode(); names.append(nm)
        o, pl = fake_object(3, PB, kinds[i], xs[i], ys[i]); pics.append(o); st["keys"][key_of(nm)] = o
    cursor, _ = fake_object(1, PB, 10, px, 0)
    square, _ = fake_object(2, PB, 5, 0, 0)
    tobj, _ = fake_object(1, PB, 3, rnd.randint(-50, 50), rnd.randint(-50, 50)); st["keys"][key_of(b"TEXTOICONOS")] = tobj
    st["keys"][key_of(b"CURSOR")] = cursor; st["keys"][key_of(b"CUADRADO")] = square
    nc, nsq = rnd.randint(0, 50), rnd.randint(0, 50); npics = [rnd.randint(0, 50) for _ in pics]
    sel_all(cursor, 1, nc); sel_all(square, 2, nsq)
    for o, tm in zip(pics, npics): sel_all(o, 3, tm)
    # group
    data = bytes([rnd.getrandbits(8), rnd.getrandbits(8)]) + struct.pack("<H", n) + bytes(12) + b"".join(nm + b"\0" * (12 - len(nm)) for nm in names)
    DATA, REC, ROOM, LVG, LV, TBL, LISTH = mk(0x1000), mk(0x40), mk(0x20), mk(0x100), mk(0x100), mk(0x40), mk(0x100)
    grp = mk(0x100)
    e.write(DATA, data + bytes(0x20)); e.write(REC + 0xc, b"GRP\0"); e.write(REC + 0x1c, struct.pack("<I", 0x20))
    e.write(LVG + 0x74, struct.pack("<I", LISTH)); e.write(LISTH, struct.pack("<I", LISTH + 0x40))
    e.call(0x414930, [REC, ROOM, TBL, DATA], conv="thiscall", this=grp)
    e.write(grp + 0x40, struct.pack("<I", LVG)); e.write(grp + 0x3c, struct.pack("<I", ROOM)); e.call(0x414a90, [], this=grp)
    st["keys"][key_of(b"ICONOS")] = grp
    # the strip
    SDATA, SREC, SROOM, STBL, SP = mk(0x200), mk(0x40), mk(0x20), mk(0x20), mk(0x40)
    strip = mk(0x100)
    e.write(SP, bytes([0xcd]) * 0x40); e.call(0x401580, [PB], this=SP); e.write(STBL, struct.pack("<I", SP))
    sfirst = rnd.randint(0, 3)
    e.write(SDATA, bytes([sfirst]) + bytes(0x1ff)); e.write(SREC, bytes(0x20)); e.write(SREC + 0xb, b"\1"); e.write(SREC + 0xc, b"STRIP\0"); e.write(SREC + 0x1c, struct.pack("<I", 0x100))
    e.write(SDATA + 0x100, b"\0"); e.write(SROOM + 8, struct.pack("<h", 0))
    now = rnd.randint(0, 1000); st["now"] = now
    out.append(f"C {n} {','.join(map(str, kinds))} {','.join(map(str, xs))} {','.join(map(str, ys))} {px} {sfirst} {now} {nc} {nsq} {','.join(map(str, npics))} | {data.hex()}")
    e.call(0x415250, [SREC, SROOM, STBL, SDATA], conv="thiscall", this=strip)
    sel_all(strip, 1, now)
    e.write(strip + 0x40, struct.pack("<I", LV)); e.write(strip + 0x3c, struct.pack("<I", SROOM))
    JIM, A14, AM1, AM2, LOTHER = mk(0x100), mk(0x10), mk(0x10), mk(0x10), mk(0x10)
    e.write(0x4420fc, struct.pack("<I", JIM)); e.write(0x442168, struct.pack("<I", A14)); e.write(0x4421f0, struct.pack("<I", AM1)); e.write(0x4421f4, struct.pack("<I", AM2))
    e.write(0x4421ec, struct.pack("<I", LV)); e.write(LV + 0x74, struct.pack("<I", LISTH)); e.write(0x442128, struct.pack("<I", cursor)); e.write(0x442200, struct.pack("<I", 0))
    jim_levels = [LOTHER, A14, AM1, AM2]; e.write(JIM + 0x40, struct.pack("<I", LOTHER)); jl = 0
    key_strip = {(0, 0): 0}
    def state():
        hov = e.u32(0x442200); hidx = pics.index(hov) if hov in pics else -1
        held = e.u32(cursor + 0x12c); hheld = pics.index(held) if held in pics else -1
        s = f"{s32(e.u32(strip + 0xb8))} {hidx} | {anim(strip)} | {anim(square)} | {anim(cursor)} {hheld} | "
        s += " ; ".join(f"{s32(e.u32(o + 0x4c))} {s32(e.u32(o + 0x50))} {anim(o)} {e.u32(o + 0x68)}" for o in pics)
        return s
    def log(op, arg, res):
        out.append(f"o {op} {arg} {now} | {res & M} {st['texts']} | {state()}")
    st["texts"] = 0
    e.call(0x415290, [], this=strip); log("A", 0, 0)
    for _ in range(rnd.randint(8, 40)):
        op = rnd.choice("IISSSXKKMMMHJW"); now += rnd.choice([0, 1, 7, 40, 300]); st["now"] = now; st["texts"] = 0; arg = 0; res = 0
        if op == "I": res = e.call(0x415350, [], this=strip)
        elif op == "S": res = e.call(0x415490, [], this=strip)
        elif op == "X": res = e.call(0x415640, [], this=strip)
        elif op == "K": res = e.call(0x415440, [], this=strip)
        elif op == "M": arg = rnd.choice(xs) + rnd.choice([-36, -35, -34, 34, 35, 36, 0, rnd.randint(-70, 70), rnd.randint(-70, 70)]); e.write(cursor + 0x4c, struct.pack("<i", arg))
        elif op == "J": arg = rnd.choice([0, 0, 0, 1, 2, 3]); e.write(JIM + 0x40, struct.pack("<I", jim_levels[arg]))
        elif op == "H":
            arg = rnd.choice([-1] + list(range(n))); e.write(cursor + 0x12c, struct.pack("<I", 0 if arg < 0 else pics[arg]))
        if op in ("I", "S", "X", "K") and e.u32(0x442200) == 0xcdcdcdcd: sys.exit("bad hover global")
        log(op, arg, res)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/strip.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
