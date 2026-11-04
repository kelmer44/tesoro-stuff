#!/usr/bin/env python3
"""Vectors for the scene routines (the native routines behind the DO lines of the numbered script blocks): runs the ORIGINAL routines
(FANPRESENT 0x4197a0 ... FCAPITAN 0x416b50, 18 of them, see ROUTINES) in Unicorn for every line, from random starting states, with the callees replaced
by recorders (object lookup by name, set visible / active, select animation bank, Jim entering a level, camera move, fade, wait, render
queue reset, clearing the main list, the door's slot 20). Name keys are faked (a recorded string per key). The board game's two routines
and the throne room's are not here (the first are checked by gen_vectors_boardgame.py).
Output: local-media/vectors/scenes.txt (ignored), read by drivers/scenes_driver.cpp. One line per run:
  V <type> <ordinal> <jimX> <jimY> <jimLevel> <blockLevel> <phase> <step> <pointerHeld> | <events> | <final state>
Objects are placed by formula (x = 300+10*level+len(name), y = -(50+7*level+len(name))), the floor of level n at (1000+n, 2000+n).
Usage: gen_vectors_scenes.py (from the repository root).
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EDX, UC_X86_REG_ECX

rnd = random.Random(20261105)
A = 0x03000000
e = Emu(); e.uc.mem_map(A, 0x100000, 7)
ecx = lambda: e.uc.reg_read(UC_X86_REG_ECX)
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
JIM, DISPLAY, POINTER, BLOCK, VT = A + 0x200, A + 0x300, A + 0x400, A + 0x500, A + 0x71000
LEVEL_BASE = 0x442130
levels = {}   # level object address -> index
for slot in range(47):
    lv = A + 0x2000 + 0x200 * slot; levels[lv] = slot
    e.write(LEVEL_BASE + 4 * slot, struct.pack("<I", lv)); e.write(lv + 0x70, struct.pack("<I", 0x70000 + slot))
    fr, floor = lv + 0x100, lv + 0x150
    e.write(lv + 0x90, struct.pack("<I", fr)); e.write(fr + 0x78, struct.pack("<I", floor)); e.write(floor + 0x4c, struct.pack("<ii", 1000 + slot, 2000 + slot))
e.write(0x4420fc, struct.pack("<I", JIM)); e.write(0x442108, struct.pack("<I", DISPLAY)); e.write(0x442128, struct.pack("<I", POINTER))
e.write(0x442104, struct.pack("<I", A + 0x900))
names, log, objects = {}, [], {}
def lname(p): return f"L{levels[p]}" if p in levels else "L?"
def tname(p):
    if p == JIM: return "jim"
    if p == POINTER: return "pointer"
    for (l, n), o in objects.items():
        if o == p: return f"L{l}.{n}"
    return f"@{p:x}"
def key_of(n): return (len(n) << 8 | sum(n.encode()), 0x5000 + len(n))
def nhash(em, p):
    n = em.read(p, 16).split(b"\0")[0].decode(); k = key_of(n); names[k] = n
    em.uc.reg_write(UC_X86_REG_EDX, k[1]); return k[0]
e.hook_function(0x415c40, nhash, 1)
def obj_for(level, name):
    key = (levels[level], name)
    if key not in objects:
        o = A + 0x8000 + 0x100 * len(objects); objects[key] = o
        e.write(o, struct.pack("<I", VT)); e.write(o + 0x5e, struct.pack("<h", 0x1234))
        L = levels[level]; e.write(o + 0x4c, struct.pack("<ii", 300 + 10 * L + len(name), -(50 + 7 * L + len(name))))
    return objects[key]
def find(em, lo, hi):
    lvl = ecx(); n = names[(lo, hi)]; log.append(f"find:L{levels[lvl]}:{n}"); return obj_for(lvl, n)
e.hook_function(0x408780, find, 2, pops=True)
e.hook_function(0x401d80, lambda em, v: log.append(f"visible:{tname(ecx())}:{v & 0xff}") or 0, 1, pops=True)
e.hook_function(0x401de0, lambda em, a, b, c, d, f: log.append(f"bank:{tname(ecx())}:{a & 0xff}:{b & 0xffff}:{s32(c)}:{d & 0xff}:{f & 0xff}") or 0, 5, pops=True)
def enter(em, lv, rooms, x, y):
    log.append(f"enter:{lname(lv)}:{s32(x)}:{s32(y)}")
    e.write(JIM + 0x40, struct.pack("<I", lv)); e.write(JIM + 0x4c, struct.pack("<ii", s32(x), s32(y)))   # what Jim_EnterLevel leaves in Jim
e.hook_function(0x407b80, enter, 4, pops=True)
e.hook_function(0x408520, lambda em, x, y, d: log.append(f"camera:{lname(ecx())}:{s32(x)}:{s32(y)}") or 0, 3, pops=True)
e.hook_function(0x4123e0, lambda em, a, b: log.append(f"fade:{a}:{b}") or 0, 2, pops=True)
e.hook_function(0x4139e0, lambda em, ms: log.append(f"wait:{ms}") or 0, 1, pops=True)
e.hook_function(0x411f20, lambda em: log.append("reset") or 0, 0)
e.hook_function(0x40fe20, lambda em: log.append("clearmain") or 0, 0)
def slot(i, label, nargs):
    tmp = A + 0x70000 + 4 * i
    e.stub_import(tmp, lambda em, *a: log.append(f"{label}:{tname(ecx())}" + "".join(f":{x & 0xff}" for x in a)) or 0, nargs)
    e.write(VT + 4 * i, e.read(tmp, 4))
slot(8, "active", 1); slot(20, "door", 0)
for o in (JIM, POINTER): e.write(o, struct.pack("<I", VT))

# (name, address, block type, number of lines the routine handles)
ROUTINES = [("FANPRESENT", 0x4197a0, 1, 6), ("FANCAMAROTE", 0x416300, 2, 10), ("FANEMBARKE", 0x4198d0, 3, 3), ("FANMOTIN", 0x4169a0, 4, 3),
            ("FANMLGISLA", 0x4199a0, 5, 1), ("FANPLAYA", 0x419a10, 6, 1), ("FEXPLOSION", 0x419ac0, 7, 1), ("FSALPANTA", 0x419b30, 9, 2),
            ("CONV31", 0x419d90, 10, 4), ("CONV35", 0x419b90, 12, 2), ("FIXCUERDA", 0x419cf0, 15, 1), ("FBARCO", 0x419d40, 16, 1),
            ("FANMMENU", 0x417c80, 17, 4), ("FANLUCHDICK", 0x416b80, 18, 2), ("FFINAL", 0x416c80, 19, 5), ("FCAPITAN", 0x416b50, 21, 1)]
chp, keyp = A + 0x600, A + 0x700
LV = list(levels)
out = []
for name, addr, typ, nlines in ROUTINES:
    for ordinal in list(range(nlines)) + [nlines, nlines + 3, 99]:
        for rep in range(4):
            objects.clear(); log.clear()
            jx, jy = rnd.randint(-600, 600), rnd.randint(-300, 300)
            jl, bl = rnd.randrange(47), rnd.randrange(47)
            ph, st, held = rnd.choice([0, 1, 2, 0x55]), rnd.choice([0, 1, 2, 0x66]), rnd.choice([0, 0x99])
            e.write(JIM + 0x4c, struct.pack("<ii", jx, jy)); e.write(JIM + 0x40, struct.pack("<I", LV[jl]))
            e.write(JIM + 0x110, bytes([ph, st])); e.write(POINTER + 0x12c, struct.pack("<I", held))
            e.write(BLOCK + 0x40, struct.pack("<I", LV[bl])); e.write(BLOCK + 0xb0, struct.pack("<I", chp)); e.write(BLOCK + 0xb4, struct.pack("<I", keyp)); e.write(BLOCK + 0xb8, struct.pack("<I", 2))
            for i, n in enumerate(("AAA", "BBB")): k = key_of(n); names[k] = n; e.write(keyp + 8 * i, struct.pack("<II", *k))
            e.write(chp, bytes(8))
            try: e.call(addr, [ordinal], this=BLOCK)
            except Exception as ex: print("FAULT", name, ordinal, ex, log, file=sys.stderr); raise
            sorts = ";".join(f"L{l}.{n}={struct.unpack('<h', e.read(o + 0x5e, 2))[0]}" for (l, n), o in objects.items() if struct.unpack('<h', e.read(o + 0x5e, 2))[0] != 0x1234)
            jp = struct.unpack("<ii", e.read(JIM + 0x4c, 8)); b = e.read(JIM + 0x110, 2)
            final = f"block=L{levels[e.u32(BLOCK + 0x40)]} jimlevel=L{levels[e.u32(JIM + 0x40)]} pos={jp[0]},{jp[1]} bytes={b[0]},{b[1]} held={e.u32(POINTER + 0x12c)} sorts={sorts or '-'}"
            out.append(f"V {typ} {ordinal} {jx} {jy} {jl} {bl} {ph} {st} {held} | {','.join(log) or '-'} | {final}")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/scenes.txt", "w").write("\n".join(out) + "\n")
print(len(out), "cases")
