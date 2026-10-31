#!/usr/bin/env python3
"""Vectors for the save and load screen objects (kinds 22 and 23): runs the ORIGINAL constructors (0x40df70, 0x40dd40 with
the MidBase base 0x40ecb0), kind 22's IsInteractive (0x40dfc0), ClearHoverText (0x40e560), SetVerbAndClick (0x40e000, only
the actions that do not touch files) and right click (0x40f040), and kind 23's draw (0x40ddc0) in Unicorn, with a real
animation player (loader 0x401160, SetRange 0x401740). Replaced by Python and logged: Cursor_SetText (0x4023f0),
Display_FadeOut (0x4123e0), ObjectList_Clear (0x40fe20), Interface_ScrollOut (0x4029a0), Display_DrawText (0x411c80),
MidBase draw (0x40efa0), sprintf (0x41c140, only "%i") and ctime (0x41c1b0).
Output: local-media/vectors/savemenu.txt (ignored). Consumed by re/tools/drivers/savemenu_driver.cpp.
Lines: B <hex bank file>
       K22 x y sortOffset roomOffset now data(hex) jimByte | state     construct a button
       o <op> <verb> <now> | result | events | state                  op: I, X, K, R
       K23 x y sortOffset roomOffset now data(hex) | state            construct a caption
       d <cameraX> <slot> <ctime hex> | events                        draw it
       #   end of case
state: sort flags action / bank first last current flags start total lx ly / jim subtitle flag (button) or just the first two groups
events: T:text (pointer text, - for none), F:steps:delay, C (list cleared, right list), S (scrolled out), W:hex:colour:x:y:width (text drawn), B (base draw)
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

rnd = random.Random(20261031)
M = 0xffffffff
FP, ARCH, HEAP = 0x100200, 0x100100, 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0, "ev": [], "ctime": 0}

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
def cstr(addr):
    out = b""
    while len(out) < 400:
        c = e.read(addr, 1)
        if c == b"\0": break
        out += c; addr += 1
    return out
def settext(emu, text, colour): st["ev"].append("T:" + (cstr(text).decode("latin1") if text else "-")); return 0
def fade(emu, steps, delay): st["ev"].append(f"F:{steps}:{delay}"); return 0
def clear(emu): st["ev"].append("C" if emu.uc.reg_read(UC_X86_REG_ECX) == e.u32(0x442104) else "C?"); return 0
def scroll(emu): st["ev"].append("S"); return 0
def drawtext(emu, text, colour, bank, cx, y, width, tag, centred, lh):
    assert (bank, tag, centred, lh) == (0, 0, 0, 0), (bank, tag, centred, lh)
    st["ev"].append(f"W:{cstr(text).hex()}:{colour}:{s32(cx)}:{s32(y)}:{s32(width)}"); return 0
def sprintf(emu, buf, fmt, val):
    assert cstr(fmt) == b"%i"; emu.write(buf, str(s32(val)).encode() + b"\0"); return 0
def ctime(emu, tptr): return st["ctime"]
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b3f0, fseek, 3)
e.hook_function(0x41b600, fsetpos, 2)
e.hook_function(0x4032e0, seek_entry, 1, pops=True); e.hook_function(0x40dc10, alloc, 3, pops=True)
e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
e.hook_function(0x413a10, lambda emu: st["now"])
e.hook_function(0x4023f0, settext, 2, pops=True); e.hook_function(0x4123e0, fade, 2, pops=True)
e.hook_function(0x40fe20, clear); e.hook_function(0x4029a0, scroll)
e.hook_function(0x411c80, drawtext, 9, pops=True)
e.hook_function(0x40efa0, lambda emu, cam: st["ev"].append("B") or 0, 1, pops=True)
e.hook_function(0x41c140, sprintf, 3); e.hook_function(0x41c1b0, ctime, 1)
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442120, struct.pack("<I", 0x100380))

def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def s16(b, o): return struct.unpack_from("<h", b, o)[0]

def bank_file(nseq):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, rnd.choice([0, 1, 1, 2, 3]), b"\0\0\0"))
    for _ in range(nseq):
        out += struct.pack("<hhhhHhIIHH", 0, 0, rnd.randint(0, 40), rnd.randint(-10, 10), rnd.randint(1, 300), -1, 0, 0, 0, 0)
    return bytes(out)

def animstate(o):
    cur = e.u32(o + 0x38); p = e.read(cur, 0x40)
    return f"{e.read(o + 0x32, 1)[0]} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x30)} {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x2c)} {u16(p,0x2e)}"

out = []
for case in range(300):
    st["heap"] = HEAP; st["ev"] = []
    nseq = rnd.choice([2, 3, 4, 6]); bdat = bytes(4) + bank_file(nseq)
    PB = mk(0x40); st["data"] = bdat; st["pos"] = 0
    e.write(PB, bytes(0x40)); e.call(0x401160, [0, 0, 0], conv="thiscall", this=PB)
    out.append("B " + bdat.hex())
    OBJ, REC, ROOM, TBL, DATA, TP = mk(0x100), mk(0x40), mk(0x20), mk(0x20), mk(0x100), mk(0x40)
    e.write(TP, bytes(0x40)); e.call(0x401580, [PB], this=TP); e.write(TBL, struct.pack("<I", TP))
    kind22 = case % 2 == 0
    action = rnd.choice([0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 200]) if kind22 else 0
    d = bytearray(rnd.getrandbits(8) for _ in range(0x60))
    if kind22: d[1] = action
    else: d[1] = rnd.choice([0, 1, 1, 7]); d[0] = rnd.randint(0, nseq - 1)
    data = bytes(d)
    ox, oy, so, ro = rnd.randint(-3000, 3000), rnd.randint(-3000, 3000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    e.write(DATA, data); e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, b"\1")
    e.write(REC + 0xc, b"BTN\0"); e.write(REC + 0x1c, struct.pack("<I", 0x80)); e.write(DATA + 0x80, b"\0" * 8); e.write(ROOM + 8, struct.pack("<h", ro))
    JIM, MAINL, DISP, CUR, LV, CAM, TB = mk(0x200), mk(0x10), mk(0x20), mk(0x200), mk(0x100), mk(0x40), mk(0x40)
    e.write(0x4420fc, struct.pack("<I", JIM)); e.write(0x442104, struct.pack("<I", MAINL)); e.write(0x442108, struct.pack("<I", DISP))
    e.write(0x442128, struct.pack("<I", CUR)); e.write(0x4421ec, struct.pack("<I", LV)); e.write(LV + 0x80, struct.pack("<I", CAM))
    jim = rnd.getrandbits(8) if rnd.random() < .5 else rnd.choice([0, 1]); e.write(JIM + 0x112, bytes([jim]))
    now = rnd.randint(0, 5000); st["now"] = now
    if kind22:
        e.call(0x40df70, [REC, ROOM, TBL, DATA], conv="thiscall", this=OBJ)
        e.write(OBJ + 0x3c, struct.pack("<I", ROOM))
        out.append(f"K22 {ox} {oy} {so} {ro} {now} {data.hex()} {jim} | {s16(e.read(OBJ+0x5e,2),0)} {e.u32(OBJ+0x68)} {e.read(OBJ+0xa0,1)[0]} / {animstate(OBJ)}")
        vt = e.u32(OBJ)
        for _ in range(rnd.randint(4, 14)):
            op = rnd.choice("IXKRIXH"); verb = rnd.randint(0, 6)
            if op in "KR" and action <= 3: op = rnd.choice("IXH")
            now += rnd.choice([0, 1, 40, 300]); st["now"] = now; st["ev"] = []
            if op == "I": res = e.call(e.u32(vt + 16 * 4), [], this=OBJ)
            elif op == "X": res = e.call(e.u32(vt + 18 * 4), [], this=OBJ)
            elif op == "H": res = e.call(e.u32(vt + 17 * 4), [], this=OBJ)
            elif op == "K": res = e.call(e.u32(vt + 14 * 4), [], this=OBJ)
            else: res = e.call(e.u32(vt + 15 * 4), [verb], conv="thiscall", this=OBJ)
            jb = e.read(JIM + 0x112, 1)[0]
            out.append(f"o {op} {verb} {now} | {s32(res & M)} | {';'.join(st['ev']) or '-'} | {animstate(OBJ)} / {int(jb != 0)}")
    else:
        e.call(0x40dd40, [REC, ROOM, TBL, DATA], conv="thiscall", this=OBJ)
        e.write(OBJ + 0x3c, struct.pack("<I", ROOM))
        out.append(f"K23 {ox} {oy} {so} {ro} {now} {data.hex()} | {s16(e.read(OBJ+0x5e,2),0)} {e.u32(OBJ+0x68)} {int(e.read(OBJ+0xa0,1)[0] != 0)} / {animstate(OBJ)}")
        vt = e.u32(OBJ); verb = rnd.randint(0, 6)
        out.append("q " + " ".join(str(s32(e.call(e.u32(vt + 4 * n), [verb] if n in (14, 15) else [], conv="thiscall" if n in (14, 15) else "cdecl", this=OBJ))) for n in (14, 15, 16, 17, 18)))
        for _ in range(rnd.randint(3, 8)):
            camx = rnd.choice([-640, -1, 0, 1, 641, rnd.randint(-700, 700)]); slot = rnd.choice([0, 0, 1, 2, 9, 40, 65535])
            tm = f"{rnd.choice(['Mon','Tue','Sat','Sun'])} {rnd.choice(['Jan','Oct','Dec'])} {rnd.randint(1,31):2d} {rnd.randint(0,23):02d}:{rnd.randint(0,59):02d}:{rnd.randint(0,59):02d} {rnd.randint(1999,2030)}\n".encode()
            if rnd.random() < .1: tm = tm[:rnd.randint(11, 20)]
            e.write(TB, tm + b"\0"); st["ctime"] = TB
            e.write(CAM + 0x2c, struct.pack("<i", camx)); e.write(CUR + 0x11c, struct.pack("<H", slot)); e.write(CUR + 0x124, struct.pack("<I", 12345))
            st["ev"] = []
            e.call(e.u32(vt + 6 * 4), [CAM], conv="thiscall", this=OBJ)
            out.append(f"d {camx} {slot} {tm.hex()} | {';'.join(st['ev']) or '-'}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/savemenu.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
