#!/usr/bin/env python3
"""Vectors for the pointer's frame step (Cursor_Update 0x4029f0 with PageInterfaceRight 0x4025b0 and the menu opening 0x4026d0 inlined or
called): runs the ORIGINAL routine in Unicorn on the game's globals: the interface level and its camera (0x4421ec), Jim and his level
(0x4420fc), the two menu levels and the board level (0x4421f0, 0x4421f4, 0x442168), the keyboard (0x442074, first state byte), the mouse
(0x44206c), the main pointer (0x442128) with its companion, the strip of carried things (0x4421fc) with its square and a list of pictures,
and the held item. Real code: Obj_SetPosition (0x401d30) and the list walks. Replaced by Python (all logged): the keyboard's update slot,
the display effect (0x4123e0), emptying the main list (0x40fe20), Jim entering a level (0x407b80), Jim's resume of facing (0x407aa0),
SelectAnimBank (0x401de0, on the pointer and on the texts object), the object search (0x408780: SALVAR and TEXTOS), the level's moved
notice (0x408440), the display's reset (0x411f20), the level's room cameras (0x408550) and compose (0x412e10, which fills the frame
with pseudo-random pixels so that the thumbnail sample can be compared).
Output: local-media/vectors/cursorupdate.txt (ignored). Consumed by re/tools/drivers/cursorupdate_driver.cpp.
Lines: B <hex bank file>
       K <case inputs> | <results>
"""
import os, random, struct, sys, zlib
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_ECX

rnd = random.Random(20261107)
M = 0xffffffff
HEAP = 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
FNS = 0x4000000; e.uc.mem_map(FNS, 0x10000, 7)
st = {"heap": HEAP, "now": 0, "seed": 0}
def mk(size):
    a = (st["heap"] + 15) // 16 * 16; st["heap"] = a + size; e.write(a, bytes(size)); return a
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
log = []

def bank_file(nseq):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, 0, b"\0\0\0"))
    for _ in range(nseq): out += struct.pack("<hhhhHhIIHH", 0, 0, 0, 0, rnd.randint(1, 300), -1, 0, 0, 0, 0)
    return bytes(out)

def hash_name(name):
    p = mk(16); e.write(p, name.encode() + b"\0"); e.call(0x415c40, [p]); return e.uc.reg_read(UC_X86_REG_EAX), e.uc.reg_read(UC_X86_REG_EDX)
KEY_SAL, KEY_TXT = hash_name("SALVAR"), hash_name("TEXTOS")

def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (slot.n + 1); slot.n += 1
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
slot.n = 0

# the world
KB, KBSTATE, KBVT = mk(0x40), mk(0x20), mk(0x100)
e.write(KB, struct.pack("<I", KBVT)); e.write(KB + 0x34, struct.pack("<I", KBSTATE)); e.write(0x442074, struct.pack("<I", KB))
slot(KBVT, 0, lambda emu: log.append("ack"), 0)
MS, BTN = mk(0x60), mk(0x10)
e.write(MS + 0x1c, struct.pack("<I", BTN)); e.write(0x44206c, struct.pack("<I", MS))
DISP = mk(0x1000); e.write(0x442108, struct.pack("<I", DISP))
FRAME = mk(0xa0000); e.write(DISP + 0x678, struct.pack("<I", FRAME))
MAINLIST = mk(0x40); e.write(0x442104, struct.pack("<I", MAINLIST))
def level(index):
    l = mk(0x200); cam = mk(0x80); e.write(l + 0x80, struct.pack("<I", cam)); e.write(l + 0x98, struct.pack("<i", index)); e.write(l + 0x70, struct.pack("<I", 0x7000 + index)); return l, cam
IFACE, ICAM = level(99); e.write(0x4421ec, struct.pack("<I", IFACE))
MAINL, MAINCAM = level(47); NEWL, NEWCAM = level(48); BOARDL, BOARDCAM = level(14)
e.write(0x4421f0, struct.pack("<I", MAINL)); e.write(0x4421f4, struct.pack("<I", NEWL)); e.write(0x442168, struct.pack("<I", BOARDL))
GAMEL, GAMECAM = level(5)
JIM = mk(0x200); e.write(0x4420fc, struct.pack("<I", JIM))
CUR, COMP = mk(0x200), mk(0x200); e.write(0x442128, struct.pack("<I", CUR))
CVT = mk(0x100); e.write(CUR, struct.pack("<I", CVT))
e.write(CUR + 0x74, struct.pack("<I", COMP)); e.write(CUR + 0x40, struct.pack("<I", IFACE))
THUMB, THUMB2 = mk(0x10a40), mk(0x10a40); e.write(CUR + 0x120, struct.pack("<I", THUMB)); e.write(CUR + 0x128, struct.pack("<I", THUMB2))
HELD = mk(0x100)
SAL, TXT = mk(0x100), mk(0x100); SVT = mk(0x100); e.write(SAL, struct.pack("<I", SVT))
STRIP, SQUARE, GHDR = mk(0x100), mk(0x100), mk(0x40)
e.write(STRIP + 0xa0, struct.pack("<I", GHDR)); e.write(STRIP + 0xa8, struct.pack("<I", SQUARE)); e.write(0x4421fc, struct.pack("<I", STRIP))
NMEM = 5; MEM = [mk(0x100) for _ in range(NMEM)]
LV = {IFACE: "iface", MAINL: "main", NEWL: "new", BOARDL: "board", GAMEL: "game"}

def save_slot(emu, v):
    log.append(f"save {v & 0xff}"); e.write(SAL + 0x68, struct.pack("<I", 2 if (v & 0xff) else 0))
slot(SVT, 0x20, save_slot, 1)
def sel(emu, bank, first, last, loop, total):
    who = ecx(); name = "cur" if who == CUR else "txt" if who == TXT else "?"
    log.append(f"sel_{name} {bank & 0xff} {first & 0xffff} {s32(last)} {loop & 0xff} {total & 0xff}"); return 0
e.hook_function(0x401de0, sel, 5, pops=True)
def find_obj(emu, lo, hi):
    if (lo, hi) == KEY_SAL: return SAL
    if (lo, hi) == KEY_TXT: return TXT
    return 0
e.hook_function(0x408780, find_obj, 2, pops=True)
e.hook_function(0x40f680, lambda emu, lo, hi: 0, 2, pops=True)
e.hook_function(0x408440, lambda emu, o: 0, 1, pops=True)
e.hook_function(0x4123e0, lambda emu, a, b: log.append(f"fade {a} {b}"), 2, pops=True)
e.hook_function(0x40fe20, lambda emu: log.append("clearlist"), 0)
def enter(emu, level_, rooms, x, y): log.append(f"enter {LV.get(level_, '?')} {int(rooms == 0x7000 + (47 if level_ == MAINL else 48))} {s32(x)} {s32(y)}"); return 0
e.hook_function(0x407b80, enter, 4, pops=True)
e.hook_function(0x407aa0, lambda emu: log.append("resume"), 0)
e.hook_function(0x411f20, lambda emu: log.append("reset"), 0)
e.hook_function(0x408550, lambda emu: log.append("draw"), 0)
def lcg_frame(seed):
    v = seed & M; out = bytearray(614400)
    for i in range(0, 614400, 2):
        v = (v * 1103515245 + 12345) & M; struct.pack_into("<H", out, i, (v >> 8) & 0xffff)
    return bytes(out)
def compose(emu):
    cam = u32(e.read(ICAM + 0x30, 4), 0); cury = u32(e.read(CUR + 0x50, 4), 0)
    log.append(f"compose {s32(cam)} {s32(cury)}"); e.write(FRAME, lcg_frame(st["seed"])); return 0
e.hook_function(0x412e10, compose, 0)
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
WORLD_END = st["heap"]

def put_list(items):
    nodes = [mk(8) for _ in items]
    for i, n in enumerate(nodes): e.write(n, struct.pack("<II", nodes[i + 1] if i + 1 < len(nodes) else 0, items[i]))
    e.write(GHDR + 4, struct.pack("<I", nodes[0] if nodes else 0)); e.write(GHDR + 0xc, struct.pack("<I", GHDR + 4)); e.write(GHDR + 0x14, b"\1")

def rd(a, n=4): return e.read(a, n)
def i32(a): return s32(u32(rd(a), 0))
out = ["B " + (bytes(4) + bank_file(4)).hex()]
for case in range(500):
    del log[:]
    # inputs
    place = rnd.choice(["game", "game", "main", "new", "board"])
    jl, jcam = {"game": (GAMEL, GAMECAM), "main": (MAINL, MAINCAM), "new": (NEWL, NEWCAM), "board": (BOARDL, BOARDCAM)}[place]
    gidx = rnd.choice([0, 3, 5, 14, 22, 23, 24, 40]) if place == "game" else 0
    if place == "game": e.write(GAMEL + 0x98, struct.pack("<i", gidx))
    e.write(JIM + 0x40, struct.pack("<I", jl))
    cx = rnd.choice([0, 0, 100, 0x27f, 0x280, 0x280, 0x281, 0x505, 0x300, -1, -0x285, -100, 0x1000, rnd.randint(-0x400, 0x600)])
    cy = rnd.randint(-100, 700)
    jcx, jcy = rnd.randint(-300, 300), rnd.randint(-300, 600)
    e.write(ICAM + 0x2c, struct.pack("<ii", cx, cy)); e.write(jcam + 0x2c, struct.pack("<ii", jcx, jcy))
    key = rnd.choice([0, 0, 8, 8, 4, 0x10]); right = rnd.choice([0, 0, 0, 8, 0x10, 0x18, 4])
    e.write(KBSTATE, bytes([key])); e.write(BTN, bytes([rnd.choice([0, 8]), right]))
    mx, my = rnd.randint(0, 639), rnd.randint(0, 479); e.write(MS + 0x38, struct.pack("<ii", mx, my))
    subs = rnd.choice([0, 1]); e.write(JIM + 0x112, bytes([subs]))
    held = rnd.choice([0, 1]); e.write(CUR + 0x12c, struct.pack("<I", HELD if held else 0)); hflags = rnd.getrandbits(8) & ~4 | rnd.choice([0, 4]); e.write(HELD + 0x68, struct.pack("<I", hflags))
    stripx, sqx = rnd.randint(-500, 1500), rnd.randint(-500, 1500); memx = [rnd.randint(-500, 1500) for _ in range(NMEM)]
    e.write(STRIP + 0x4c, struct.pack("<i", stripx)); e.write(SQUARE + 0x4c, struct.pack("<i", sqx))
    for m, x in zip(MEM, memx): e.write(m + 0x4c, struct.pack("<i", x))
    nm = rnd.randint(0, NMEM); put_list(MEM[:nm])
    curx, cury = rnd.randint(-500, 1500), rnd.randint(-500, 700); e.write(CUR + 0x4c, struct.pack("<iiii", curx, cury, 0, 0))
    e.write(COMP + 0x4c, struct.pack("<iiii", 1, 2, 3, 4)); e.write(COMP + 0x3c, struct.pack("<I", 0)); e.write(COMP + 0x40, struct.pack("<I", 0))
    e.write(CUR + 0x78, bytes([rnd.choice([0, 65])])); label = rd(CUR + 0x78, 1)[0]
    slotw = rnd.randint(0, 9); e.write(CUR + 0x11c, struct.pack("<H", slotw))
    e.write(SAL + 0x68, struct.pack("<I", 0)); e.write(THUMB, bytes([7]) * 0x10a40); e.write(THUMB2, bytes([9]) * 0x10a40)
    st["seed"] = rnd.getrandbits(32); now = rnd.randint(1000, 9000); st["now"] = now
    inputs = f"{place} {gidx} {cx} {cy} {jcx} {jcy} {key} {right} {mx} {my} {subs} {held} {hflags} {stripx} {sqx} {nm} {' '.join(map(str, memx))} {curx} {cury} {label} {slotw} {st['seed']}"
    ret = e.call(0x4029f0, [], this=CUR) & M
    # fold the four calls of the scene capture into one entry
    folded = []; i = 0
    while i < len(log):
        if log[i] == "reset" and i + 3 < len(log) and log[i + 1] == "draw" and log[i + 2].startswith("compose") and log[i + 3] == "reset":
            folded.append("capture" + log[i + 2][len("compose"):]); i += 4
        else: folded.append(log[i]); i += 1
    sel_cur = [l for l in folded if l.startswith("sel_cur")]; sel_txt = [l for l in folded if l.startswith("sel_txt")]
    calls = [l for l in folded if not l.startswith("sel_")]
    thumb_crc = zlib.crc32(rd(THUMB2, 0x10a40)); thumb2_zero = int(rd(THUMB, 0x10a40) == bytes(0x10a40))
    res = (f"{ret} | {','.join(l.replace(' ', '_') for l in calls) or '-'} | {','.join(l.replace(' ', '_') for l in sel_cur) or '-'} | {','.join(l.replace(' ', '_') for l in sel_txt) or '-'} | "
           f"{i32(CUR+0x4c)} {i32(CUR+0x50)} {i32(CUR+0x54)} {i32(CUR+0x58)} | {LV.get(u32(rd(COMP+0x40),0),'?')} {int(u32(rd(COMP+0x3c),0) == u32(rd(jl+0x70),0))} {i32(COMP+0x4c)} {i32(COMP+0x50)} {i32(COMP+0x54)} {i32(COMP+0x58)} | "
           f"{i32(ICAM+0x2c)} {i32(ICAM+0x30)} | {i32(STRIP+0x4c)} {i32(SQUARE+0x4c)} {' '.join(str(i32(m + 0x4c)) for m in MEM)} | {u32(rd(HELD+0x68),0)} {u32(rd(CUR+0x12c),0) != 0:d} | {rd(CUR+0x78,1)[0]} {struct.unpack('<H', rd(CUR+0x11c,2))[0]} | {u32(rd(SAL+0x68),0)} {thumb_crc} {thumb2_zero}")
    out.append(f"K {inputs} | {res}")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/cursorupdate.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
