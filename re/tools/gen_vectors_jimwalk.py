#!/usr/bin/env python3
"""Vectors for Jim's walking: runs the ORIGINAL walk order (0x407600), per frame update (0x407790), collision callback (0x4079e0) and set facing
(0x407a80) in Unicorn on a fake Jim. What they call is a hook that logs (main list, animation advance and range, the standing frame, the scale of
the walk area, the position move, the route round the corners, the speaker update, the arrival call of the target); the world (movement the
animation gives, scale, answers of the hooks, whether a walk area exists) is set per step and recorded so that the port can be driven by the
same answers. The animation state is modelled as the current bank and which banks play.
Usage: gen_vectors_jimwalk.py (from the repository root). Output: local-media/vectors/jimwalk.txt (ignored), read by drivers/jimwalk_driver.cpp.
Lines: S <id> then  o <op> | r <result> | s <state> | . <log line>...  and #
Ops: B <cur> <playing mask> | J <x> <y> <facing> | E <mx> <my> <scale> <speakerUpdate> <findPath> <arrive> <hasArea> <otherHasArea> | W <x> <y> <target> <direct> | U | X | F <facing>
State: walking fixedX fixedY usedX usedY direct target scale destX destY facing x y prevX prevY currentBank playingMask"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

WORLD, FNS = 0x3100000, 0x7e000000
e = Emu(); e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
JIM, VT, LEVEL, SPECIAL, AREA, OTHER, TGT, TVT, BANKARR = [WORLD + 0x1000 * i for i in range(9)]
PL = [WORLD + 0x10000 + 0x100 * i for i in range(9)]
log = []; world = dict(mx=0, my=0, scale=0x100, su=1, fp=0, ret=1, has=1, ohas=1)
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
def tname(p): return "tgt" if p == TGT else "none" if p == 0 else "?"
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
e.write(VT + 0x8, struct.pack("<I", 0x407770)); e.write(VT + 0x74, struct.pack("<I", 0x407a80))
slot(TVT, 0x34, lambda emu: (log.append("arrived"), world["ret"])[1], 0)
e.hook_function(0x40fb20, lambda emu, o: log.append("remove"), 1, pops=True)
e.hook_function(0x40fa10, lambda emu, o, p: log.append(f"insert {p}"), 2, pops=True)
def setrange(emu, first, last, loop, s):
    log.append(f"range {first & 0xffff} {i32(last)} {loop & 0xff} {s & 0xff}")
    fl = e.u32(ecx() + 0x18); e.write(ecx() + 0x18, struct.pack("<I", (fl & ~6) | 4 | 1))
e.hook_function(0x401740, setrange, 4, pops=True)
def advance(emu):
    log.append("advance"); e.write(ecx() + 0x28, struct.pack("<hh", world["mx"], world["my"]))
e.hook_function(0x401860, advance, 0)
e.hook_function(0x414740, lambda emu, x, y: (log.append(f"scale {i32(x)} {i32(y)}"), world["scale"])[1], 2, pops=True)
e.hook_function(0x4101f0, lambda emu: (log.append("speakerupdate"), world["su"])[1], 0)
e.hook_function(0x4103d0, lambda emu, f: log.append(f"face {f}"), 1, pops=True)
e.hook_function(0x401d30, lambda emu, x, y, c: (log.append(f"move {i32(x)} {i32(y)}"), e.write(ecx() + 0x54, struct.pack("<ii", i32(x), i32(y))))[0], 3, pops=True)
e.hook_function(0x414f10, lambda emu, x, y, t: (log.append(f"findpath {i32(x)} {i32(y)} {tname(t)}"), world["fp"])[1], 3, pops=True)

def setup():
    e.write(0x4420fc, struct.pack("<I", JIM))
    e.write(JIM, bytes(0x120)); e.write(JIM, struct.pack("<I", VT)); e.write(TGT, struct.pack("<I", TVT))
    e.write(JIM + 0x40, struct.pack("<I", LEVEL)); e.write(LEVEL + 0x90, struct.pack("<I", SPECIAL))
    e.write(JIM + 0xd8, struct.pack("<I", BANKARR + 8)); e.write(BANKARR, b"".join(struct.pack("<I", p) for p in PL))
    for p in PL: e.write(p, bytes(0x60))
    e.write(JIM + 0xfc, struct.pack("<I", 0x100)); e.write(JIM + 0x10c, struct.pack("<I", 3))
def apply_area():
    e.write(SPECIAL + 0x78, struct.pack("<I", AREA if world["has"] else 0)); e.write(OTHER + 0x78, struct.pack("<I", AREA if world["ohas"] else 0))
def state():
    cur = e.u32(JIM + 0x38); idx = PL.index(cur) if cur in PL else -1
    mask = sum((e.u32(p + 0x18) & 1) << i for i, p in enumerate(PL))
    r = lambda off, n=1: struct.unpack("<" + "i" * n, e.read(JIM + off, 4 * n))
    return " ".join(map(str, [e.read(JIM + 0xd5, 1)[0], *r(0xe0, 2), *r(0xe8, 2), e.read(JIM + 0xf0, 1)[0], tname(e.u32(JIM + 0xf4)), r(0xfc)[0], *r(0x104, 2), r(0x10c)[0], *r(0x4c, 2), *r(0x54, 2), idx, mask]))

out = []; rnd = random.Random(20261106)
def op(text, addr, args, ret=True):
    del log[:]
    r = e.call(addr, args, conv="thiscall", this=JIM, max_insns=2_000_000)
    out.append("o " + text); out.append("r " + (str(r & 0xffffffff) if ret else "-")); out.append("s " + state()); out.extend(". " + l for l in log)
def note(text): out.append("o " + text); out.append("r -"); out.append("s " + state())
for sid in range(1, 1001):
    setup(); out.append(f"S {sid}")
    cur, mask = rnd.randint(0, 8), rnd.getrandbits(9)
    e.write(JIM + 0x38, struct.pack("<I", PL[cur]))
    for i, p in enumerate(PL): e.write(p + 0x18, struct.pack("<I", (mask >> i) & 1))
    out.append(f"o B {cur} {mask}"); out.append("r -"); out.append("s " + state())
    x, y, f = rnd.randint(-50, 640), rnd.randint(-50, 480), rnd.randint(1, 4)
    e.write(JIM + 0x4c, struct.pack("<ii", x, y)); e.write(JIM + 0x54, struct.pack("<ii", x, y)); e.write(JIM + 0x10c, struct.pack("<I", f))
    note(f"J {x} {y} {f}")
    world.update(mx=0, my=0, scale=0x100, su=1, fp=0, ret=1, has=1, ohas=1); apply_area()
    out.append("o E 0 0 256 1 0 1 1 1"); out.append("r -"); out.append("s " + state())
    for step in range(rnd.randint(5, 40)):
        if rnd.random() < 0.6:
            world.update(mx=rnd.randint(-3, 30), my=rnd.randint(-10, 12), scale=rnd.choice([0x100, 0x80, 0x180, 0x40, 0x200, 0x100]), su=rnd.choice([0, 1, 3, 2]),
                         fp=rnd.choice([0, 1]), ret=rnd.choice([1, 1, 0, 2, 3]), has=int(rnd.random() < 0.9), ohas=int(rnd.random() < 0.8))
            apply_area(); note(f"E {world['mx']} {world['my']} {world['scale']} {world['su']} {world['fp']} {world['ret']} {world['has']} {world['ohas']}")
        r = rnd.random()
        if r < 0.2:
            cx, cy = e.read(JIM + 0x4c, 4), e.read(JIM + 0x50, 4); px, py = struct.unpack("<i", cx)[0], struct.unpack("<i", cy)[0]
            dx, dy = px + rnd.choice([rnd.randint(-60, 60), rnd.randint(-400, 400), 0]), py + rnd.choice([rnd.randint(-60, 60), rnd.randint(-300, 300), 0])
            t = rnd.randint(0, 1); d = rnd.randint(0, 1)
            op(f"W {dx} {dy} {t} {d}", 0x407600, [dx & 0xffffffff, dy & 0xffffffff, TGT if t else 0, d])
        elif r < 0.78: op("U", 0x407790, [])
        elif r < 0.9: op("X", 0x4079e0, [OTHER])
        else:
            f = rnd.randint(1, 4); op(f"F {f}", 0x407a80, [f], ret=False)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/jimwalk.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
