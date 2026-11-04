#!/usr/bin/env python3
"""Checks the reading of the level's picture-reload step on the ship's deck (vtable 0042bbd0 slot 13, 00416800) against the original
machine code in Unicorn, with its callees replaced by logging stubs (base cue 00408840, CdAudio_PlayTrack 00403cb0, name hash 00415c40,
script lookup 004060c0, Script_Start 00405fd0, FindObject 00408780). Measurement tool only.
Run: re/.venv/bin/python re/tools/check_deck_events.py
"""
import random, struct, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EDX, UC_X86_REG_ECX

A = 0x03000000
e = Emu(); e.uc.mem_map(A, 0x100000, 7)
JIM, LVL, FRAME, OTHER_FRAME, DECK, FISH, BLOB, CD, SCRIPTS, BLOCK, VARS = [A + 0x1000 * i for i in range(1, 12)]
log, names = [], {}
def s(em, p): return em.read(p, 32).split(b"\0")[0].decode()
def nhash(em, p):
    n = s(em, p); k = (len(n) << 8 | sum(n.encode()), 0x5000 + len(n)); names[k] = n
    em.uc.reg_write(UC_X86_REG_EDX, k[1]); return k[0]
e.hook_function(0x415c40, nhash, 1)
e.hook_function(0x408840, lambda em: log.append(("base", em.uc.reg_read(UC_X86_REG_ECX))), 0)
e.hook_function(0x403cb0, lambda em, t, f: log.append(("track", t & 0xff, f)), 2, pops=True)
def find_block(em, lo, hi): log.append(("find", names[(lo, hi)])); return BLOCK
e.hook_function(0x4060c0, find_block, 2, pops=True)
def start(em, lo, hi, vars_, level):
    log.append(("start", names[(lo, hi)], em.read(vars_, 1)[0], level)); return 3
e.hook_function(0x405fd0, start, 4, pops=True)
def find_obj(em, lo, hi): log.append(("object", names[(lo, hi)])); return FISH
e.hook_function(0x408780, find_obj, 2, pops=True)
e.write(0x4420fc, struct.pack("<I", JIM)); e.write(0x442080, struct.pack("<I", CD - 0x1c)); e.write(0x4420b8, struct.pack("<I", SCRIPTS))
e.write(0x44213c, struct.pack("<I", DECK))
e.write(JIM + 0x40, struct.pack("<I", LVL))

rnd = random.Random(5); fails = 0
for case in range(600):
    step = rnd.choice([0, 0, 1, 2, 3, 4, 8, 127, 128, 200, 255]); phase = rnd.choice([0, 1, 2]); own = rnd.choice([0, 7, 8, 9])
    fish = rnd.choice([0, 1, 2, 3]); entered = rnd.choice([0, 1])
    e.write(JIM + 0x110, bytes([phase, step])); e.write(LVL + 0x90, struct.pack("<I", FRAME)); e.write(FRAME + 0x7d, bytes([own]))
    e.write(FISH + 0x60, struct.pack("<I", BLOB)); e.write(BLOB, bytes([fish]))
    log.clear(); r = e.call(0x416800, [], this=LVL) & 0xffffffff
    # model of the contract
    want = [("base", LVL)]; wown, wstep, wphase, wfish = own, step, phase, fish; wret = 1
    if step != 0 and own == 8: wown = 9; want.append(("track", 9, 1))
    sgn = step - 256 if step > 127 else step
    if sgn > 2:
        want += [("find", "FANMOTIN"), ("start", "main", step, JIM and struct.unpack("<I", e.read(JIM + 0x40, 4))[0])]
        wstep = (step + 1) & 0xff; wphase = 1; wret = 3
    elif step == 0:
        want.append(("object", "PESCADOR"))
        if fish == 2: wfish = 3; want.append(("object", "PESCADOR"))   # looked up a second time to set it
    # the lookup of the label and the block happen in this order: hash main, hash block, find, start
    got_calls = [x for x in log]
    ok = got_calls == want and r == wret and e.read(FRAME + 0x7d, 1)[0] == wown and list(e.read(JIM + 0x110, 2)) == [wphase, wstep] and e.read(BLOB, 1)[0] == wfish
    if not ok:
        fails += 1
        if fails < 5: print("MISMATCH", dict(step=step, own=own, fish=fish), "\n want", want, wret, wown, wstep, wphase, wfish, "\n got ", got_calls, r, e.read(FRAME + 0x7d, 1)[0], list(e.read(JIM + 0x110, 2)), e.read(BLOB, 1)[0])
print("FAILURES:", fails); sys.exit(1 if fails else 0)
