#!/usr/bin/env python3
"""Checks the reading of the island map's story update (vtable 0042bc08 slot 13, 004179b0) against the original machine code in Unicorn:
object lookups (00408780) and name hashing (00415c40) are logging stubs, objects have a state record (+0x60), flags (+0x68) and a
vtable whose slot 8 (activate) logs. Measurement tool only. Run: re/.venv/bin/python re/tools/check_map_events.py
"""
import random, struct, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EDX, UC_X86_REG_ECX

A = 0x03000000
e = Emu(); e.uc.mem_map(A, 0x100000, 7)
LEVELS = {0x442154: 9, 0x44214c: 7, 0x442160: 12, 0x442158: 10}
log, names = [], {}
def nhash(em, p):
    n = em.read(p, 16).split(b"\0")[0].decode(); k = (len(n) << 8 | sum(n.encode()), 0x5000 + len(n)); names[k] = n
    em.uc.reg_write(UC_X86_REG_EDX, k[1]); return k[0]
e.hook_function(0x415c40, nhash, 1)
objs = {}
def find(em, lo, hi):
    lvl = em.uc.reg_read(UC_X86_REG_ECX)
    key = (lvl, names[(lo, hi)]); log.append(("find", LEVELS[[g for g, v in glob.items() if v == lvl][0]], key[1])); return objs[key]
e.hook_function(0x408780, find, 2, pops=True)
glob = {}
def slot8(em, active):
    who = [k for k, v in objs.items() if v == em.uc.reg_read(UC_X86_REG_ECX)][0]
    log.append(("activate", LEVELS[[g for g, v in glob.items() if v == who[0]][0]], who[1], active)); return 0
tmp = A + 0x70000; e.stub_import(tmp, slot8, 1); stub = e.u32(tmp)
VT = A + 0x71000; e.write(VT + 0x20, struct.pack("<I", stub))
for i, (g, lvl) in enumerate(LEVELS.items()):
    glob[g] = A + 0x100 * (i + 1)                     # the level object (only its address is used)
    e.write(g, struct.pack("<I", glob[g]))
cells = {}
for i, (lvl, name) in enumerate([(g, n) for g in glob.values() for n in ("ORZOGUEI", "CANOA", "JUSTINO", "FERNAN")]):
    o = A + 0x1000 + 0x100 * i; blob = A + 0x20000 + 0x40 * i; objs[(lvl, name)] = o
    e.write(o, struct.pack("<I", VT)); cells[(lvl, name)] = (o, blob); e.write(o + 0x60, struct.pack("<I", blob))
def obj(level_index, name): return cells[(glob[[g for g, v in LEVELS.items() if v == level_index][0]], name)]
rnd = random.Random(9); fails = 0
for case in range(800):
    for k, (o, blob) in cells.items():
        e.write(blob, bytes(rnd.choice([0, 0, 1, 2, 3, 4, 5, 6, 200]) for _ in range(16))); e.write(o + 0x68, struct.pack("<I", rnd.getrandbits(32)))
    if rnd.random() < .5:                             # make the ORZOGUEI tasks done more often
        e.write(obj(9, "ORZOGUEI")[1] + 0xb, bytes([1, 1, 1, 1]))
    oz_o, oz = obj(9, "ORZOGUEI"); ca_o, _ = obj(7, "CANOA"); fe_o, fe = obj(10, "FERNAN"); ju_o, ju = obj(12, "JUSTINO")
    b = lambda p, i: e.read(p + i, 1)[0]
    before = {n: e.read(c[1], 16) for n, c in cells.items()}; cflags = e.u32(ca_o + 0x68); fflags = e.u32(fe_o + 0x68)
    log.clear(); r = e.call(0x4179b0, [], this=0) & 0xffffffff
    want = [("find", 9, "ORZOGUEI")]; oz_after = bytearray(e.read(oz, 16)); wb = bytearray(before[(glob[0x442154], "ORZOGUEI")])
    ozb = bytearray(before[(glob[0x442154], "ORZOGUEI")])
    if ozb[0xb] and ozb[0xc] and ozb[0xd] and ozb[0xe]: ozb[1] = 5
    want.append(("find", 7, "CANOA"))
    cond = (not (cflags >> 1) & 1) and ((ozb[1] - 256 if ozb[1] > 127 else ozb[1]) > 4)
    if cond: want += [("find", 7, "CANOA"), ("activate", 7, "CANOA", 1), ("find", 12, "JUSTINO"), ("activate", 12, "JUSTINO", 0)]
    want.append(("find", 10, "FERNAN"))
    if before[(glob[0x442158], "FERNAN")][0] == 0:
        want.append(("find", 12, "JUSTINO"))
        if before[(glob[0x442160], "JUSTINO")][0] == 2: want += [("find", 10, "FERNAN"), ("activate", 10, "FERNAN", 1)]
    ok = log == want and r == 1 and bytes(e.read(oz, 16)) == bytes(ozb) and all(bytes(e.read(c[1], 16)) == before[n] for n, c in cells.items() if n != (glob[0x442154], "ORZOGUEI"))
    if not ok:
        fails += 1
        if fails < 4: print("MISMATCH\n want", want, "\n got ", log, r)
print("FAILURES:", fails); sys.exit(1 if fails else 0)
