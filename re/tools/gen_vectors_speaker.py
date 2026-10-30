#!/usr/bin/env python3
"""Vectors for the speaking part of a character: runs the ORIGINAL say (0x410330), kind 5 update (0x4101f0), clear speech (0x410400),
talk animation (0x410060), kind 5 draw (0x4102e0) and Jim's draw (0x407ac0) in Unicorn on a fake kind 5 object. Everything the methods call is a Python hook that
appends a line to a log (voices, sound lookups, the main list, the pose, the picture queue, the animation range, the interactive base update and
draw, the subtitle); the world (clock, skip input, which voices play, the subtitles option, the base update's answer) is set per step and recorded
so that the port can be driven by the same answers. Not covered: what the hooked calls do inside.
Usage: gen_vectors_speaker.py (from the repository root). Output: local-media/vectors/speaker.txt (ignored), read by drivers/speaker_driver.cpp.
Lines: S <sessionId> <frameCount> <wav0> ... <wav4>   then per operation  o <op...> | r <result> | s <state> | . <log line>...   #
Operations: E <now> <skip> <activeMask> <subtitles> <baseUpdate> | Y <line> <colour> <pose> | U | D <0 character|1 Jim> | P <0|1> | H <0|1> | C
State: speaking held pending line voice colour talkTime"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu, RET_MAGIC
from unicorn.x86_const import UC_X86_REG_ECX

WORLD, FNS = 0x3100000, 0x7e000000
e = Emu(); e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
OBJ, VT, PLAYER, SEQS, SLOT, IMG, LEVEL, CAM, JIM, KBD, STATE, MOUSE, BTNS, SND, LIST, LINES, REC, ARG = [WORLD + 0x1000 * i for i in range(18)]
log = []; world = {"now": 0, "skip": 0, "active": 0, "subs": 0, "base": 0, "ids": 0, "count": 4, "rcalls": 0}
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
e.write(VT + 0x10, struct.pack("<I", 0x4101f0)); e.write(VT + 0x70, struct.pack("<I", 0x410400)); e.write(VT + 0x7c, struct.pack("<I", 0x410060))
slot(VT, 0x78, lambda emu, v: log.append(f"pose {v}"), 1)
e.hook_function(0x401a50, lambda emu: log.append("pause"), 0)
e.hook_function(0x401da0, lambda emu: log.append("stash"), 0)
def play(emu, off, a, b, c): world["ids"] += 1; log.append(f"play {off}"); return world["ids"]
e.hook_function(0x404110, play, 4, pops=True)
def find(emu, i): log.append(f"find {i}"); return REC if (world["active"] >> i) & 1 else 0
e.hook_function(0x4040d0, find, 1, pops=True)
e.hook_function(0x4043b0, lambda emu: log.append("stop"), 0)
e.hook_function(0x404210, lambda emu: log.append("reap"), 0)
e.hook_function(0x413a10, lambda emu: world["now"], 0)
e.hook_function(0x40f900, lambda emu, o, p: log.append(f"insert {p}"), 2, pops=True)
e.hook_function(0x40fb20, lambda emu, o: log.append("remove"), 1, pops=True)
e.hook_function(0x40ee70, lambda emu: (log.append("iupdate"), world["base"])[1], 0)
e.hook_function(0x40efa0, lambda emu, cam: log.append("idraw"), 1, pops=True)
e.hook_function(0x4120c0, lambda emu, a, b, c, d, f, g, h, k: log.append("idraw"), 8, pops=True)
e.hook_function(0x40ffa0, lambda emu, ln: log.append(f"subtitle {(ln - LINES) // 16}"), 1, pops=True)
e.hook_function(0x411f80, lambda emu, x, y, d, img, extra: log.append("queue"), 5, pops=True)
def rnd_hook(emu, lo, hi):
    world["rcalls"] += 1; v = lo + (world["rcalls"] * 7) % (hi - lo + 1); log.append(f"random {lo} {hi}"); return v
e.hook_function(0x413b00, rnd_hook, 2, pops=True)
e.hook_function(0x401740, lambda emu, first, last, loop, s: log.append(f"range {first & 0xffff} {i32(last)} {loop & 0xff} {s & 0xff}"), 4, pops=True)

def setup(count, wavs):
    world.update(now=0, skip=0, active=0, subs=0, base=0, ids=0, count=count, rcalls=0)
    e.write(OBJ, bytes(0x100)); e.write(OBJ, struct.pack("<I", VT))
    e.write(OBJ + 0x38, struct.pack("<I", PLAYER)); e.write(OBJ + 0x40, struct.pack("<I", LEVEL))
    e.write(OBJ + 0x4c, struct.pack("<ii", 100, 50)); e.write(OBJ + 0x5e, struct.pack("<h", 7)); e.write(OBJ + 0xfc, struct.pack("<I", 0x100))
    e.write(LEVEL + 0x80, struct.pack("<I", CAM)); e.write(CAM, bytes(0x100)); e.write(CAM + 0x34, struct.pack("<ii", 10, 20))
    e.write(PLAYER, bytes(0x100)); e.write(PLAYER + 0x1c, struct.pack("<I", SEQS)); e.write(PLAYER + 0x22, struct.pack("<H", count))
    e.write(SEQS, bytes(0x24 * 16)); e.write(SEQS + 0x18, struct.pack("<I", SLOT)); e.write(SLOT, struct.pack("<I", IMG))
    e.write(JIM, bytes(0x200)); e.write(0x4420fc, struct.pack("<I", JIM))
    e.write(0x442074, struct.pack("<I", KBD)); e.write(KBD + 0x34, struct.pack("<I", STATE)); e.write(STATE, bytes(4))
    e.write(0x44206c, struct.pack("<I", MOUSE)); e.write(MOUSE + 0x1c, struct.pack("<I", BTNS)); e.write(BTNS, bytes(4))
    for g, v in ((0x442080, SND), (0x442104, LIST), (0x442120, WORLD + 0x20000), (0x442108, WORLD + 0x21000)): e.write(g, struct.pack("<I", v))
    for k, w in enumerate(wavs): e.write(LINES + 16 * k, bytes(16)); e.write(LINES + 16 * k + 8, struct.pack("<I", w)); e.write(LINES + 16 * k + 0xc, struct.pack("<I", ARG))
def apply_world():
    e.write(STATE, bytes([world["skip"] & 1])); e.write(BTNS, bytes([0, 0])); e.write(JIM + 0x112, bytes([world["subs"]]))
def state():
    sp, held, pend = e.read(OBJ + 0xd4, 1)[0], e.read(OBJ + 0xc, 1)[0], e.read(OBJ + 0xd5, 1)[0]
    ln = e.u32(OBJ + 0xc4); idx = -1 if ln == 0 else (ln - LINES) // 16
    return f"{sp} {held} {pend} {idx} {e.u32(OBJ + 0xc8)} {i32(e.u32(OBJ + 0xcc))} {i32(e.u32(OBJ + 0xa8))}"

out = []; rnd = random.Random(20261104)
def op(text, addr, args, conv="thiscall"):
    del log[:]
    ret = e.call(addr, args, conv="thiscall", this=OBJ, max_insns=2_000_000)
    out.append("o " + text); out.append("r " + str(ret)); out.append("s " + state()); out.extend(". " + l for l in log)
for sid in range(1, 801):
    count = rnd.randint(2, 9); wavs = [rnd.choice([0, 0, 1000 + 7 * k, 5000 + k]) for k in range(5)]
    setup(count, wavs); out.append(f"S {sid} {count} " + " ".join(map(str, wavs)))
    for step in range(rnd.randint(4, 24)):
        if rnd.random() < 0.55:
            world["now"] = (world["now"] + rnd.choice([0, 20, 50, 100, 101, 150, 400, 5000])) & 0xffffffff
            world["skip"] = int(rnd.random() < 0.15); world["active"] = rnd.getrandbits(8) & rnd.getrandbits(8)
            world["subs"] = int(rnd.random() < 0.6); world["base"] = rnd.choice([0, 1, 3, 2])
            apply_world(); out.append(f"o E {world['now']} {world['skip']} {world['active']} {world['subs']} {world['base']}"); out.append("r -"); out.append("s -")
        r = rnd.random()
        if r < 0.25:
            ln, colour, pose = rnd.randrange(5), rnd.choice([0xffff, 0x1234, -0xffff, -0x1234, 0, 1, -1]), rnd.randint(0, 3)
            op(f"Y {ln} {colour} {pose}", 0x410330, [LINES + 16 * ln, colour & 0xffffffff, pose])
        elif r < 0.6: op("U", 0x4101f0, [])
        elif r < 0.75: op(f"D {rnd.randint(0, 1)}" if False else "D 0", 0x4102e0, [CAM])
        elif r < 0.85: op("D 1", 0x407ac0, [CAM])
        elif r < 0.92:
            v = int(rnd.random() < 0.5); e.write(OBJ + 0xd5, bytes([v])); out.append(f"o P {v}"); out.append("r -"); out.append("s " + state())
        elif r < 0.97:
            v = int(rnd.random() < 0.5); e.write(OBJ + 0xc, bytes([v])); out.append(f"o H {v}"); out.append("r -"); out.append("s " + state())
        else: op("C", 0x410400, [])
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/speaker.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
