#!/usr/bin/env python3
"""Vectors for Jim as an object: builds Jim with the ORIGINAL constructor (0x4070c0) from nine synthetic animation files (found by name through a hook
on the archive lookup, loaded by the original bank loader 0x401160 and the deferred picture loader 0x401510), then runs the original script line (0x407ce0),
verb answer (0x410330 with Jim's colour), update (0x407790), walk order (0x407600), touch (0x4079e0), facing (0x407a80), draw (0x407ac0) and bounds
(0x407730) with REAL animation players. Hooks log what leaves Jim: main list, voices, subtitle, base update, walk area scale and route, the arrival call of
the target, the position move and the scaled picture. After every operation Jim's fields and the state of all nine players are dumped.
Not covered: saving and loading, what the hooked calls do inside.
Usage: gen_vectors_jim.py (from the repository root). Output: local-media/vectors/jim.txt (ignored), read by drivers/jim_driver.cpp.
Lines: S <id> <hex file> | then  o <op> | r <result> | s <state> | . <log> ...  | #   Ops: I (constructed) | E <now> <skip> <voices> <subs> <base> <scale> <findpath> <ret> <hasArea> <otherHasArea> <mx> <my> <camX> <camY>
| Y <line> (script line) | A <line> (verb answer) | U | W <x> <y> <target> <direct> | X | F <facing> | D | B"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261109)
PL, ARCH, FP, DISP, HEAP = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def rebase")])

WORLD, FNS = 0x3200000, 0x7e000000
e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
JIM, VT, LEVEL, SPECIAL, AREA, OTHER, TGT, TVT, CAM, KBD, STATE, MOUSE, BTNS, LINES, ARG, REC = [WORLD + 0x1000 * i for i in range(16)]
NAMES = [b"JIMHABIZ.ANM", b"JIMHABDER.ANM", b"JIMQUIETO.ANM", b"JIMARRIBA.ANM", b"JIMABAJO.ANM", b"JIMIZQUIERDA.ANM", b"JIMDERECHA.ANM", b"JIMHABAB.ANM", b"JIMHABAR.ANM"]
OFFS = [0x4000 * (k + 1) for k in range(9)]
log = []; world = dict(now=0, skip=0, voices=0, subs=0, base=0, scale=0x100, fp=0, ret=1, has=1, ohas=1, ids=0, rcalls=0)
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
def tname(p): return "tgt" if p == TGT else "none" if p == 0 else "?"
def cstr(a):
    b = bytearray()
    while True:
        c = e.read(a, 1)
        if c == b"\0": return bytes(b)
        b += c; a += 1
e.hook_function(0x403500, lambda emu, namep, flag: OFFS[NAMES.index(cstr(namep))], 2, pops=True)
e.hook_function(0x413940, lambda emu, namep: LINES, 1)
e.write(0x442128, struct.pack("<I", WORLD + 0x30000))     # the cursor is not used here
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
slot(TVT, 0x34, lambda emu: (log.append("arrived"), world["ret"])[1], 0)
e.hook_function(0x40fb20, lambda emu, o: log.append("remove"), 1, pops=True)
e.hook_function(0x40fa10, lambda emu, o, p: log.append(f"insert {p}"), 2, pops=True)
e.hook_function(0x40f900, lambda emu, o, p: log.append(f"insert {p}"), 2, pops=True)
e.hook_function(0x414740, lambda emu, x, y: (log.append(f"scale {i32(x)} {i32(y)}"), world["scale"])[1], 2, pops=True)
e.hook_function(0x401d30, lambda emu, x, y, c: e.write(ecx() + 0x54, struct.pack("<ii", i32(x), i32(y))), 3, pops=True)    # not logged: the port moves the object itself, and the previous position is in the state
e.hook_function(0x414f10, lambda emu, x, y, t: (log.append(f"findpath {i32(x)} {i32(y)} {tname(t)}"), world["fp"])[1], 3, pops=True)
def play(emu, off, a, b, c): world["ids"] += 1; log.append(f"play {off}"); return world["ids"]
e.hook_function(0x404110, play, 4, pops=True)
e.hook_function(0x4040d0, lambda emu, i: (log.append(f"find {i}"), REC if (world["voices"] >> i) & 1 else 0)[1], 1, pops=True)
e.hook_function(0x4043b0, lambda emu: log.append("stop"), 0)
e.hook_function(0x404210, lambda emu: log.append("reap"), 0)
e.hook_function(0x40ee70, lambda emu: (log.append("iupdate"), world["base"])[1], 0)
e.hook_function(0x40ffa0, lambda emu, ln: log.append(f"subtitle {(ln - LINES) // 16}"), 1, pops=True)
def rnd_hook(emu, lo, hi):
    world["rcalls"] += 1; v = lo + (world["rcalls"] * 7) % (hi - lo + 1); log.append(f"random {lo} {hi}"); return v
e.hook_function(0x413b00, rnd_hook, 2, pops=True)
imgname = {}
def blit(emu, x, y, d, img, sx, sy, sh, ex): log.append(f"scaled {i32(x)} {i32(y)} {d & 0xffff} {imgname.get(img, '?')} {i32(sx)} {i32(sy)} {i32(sh)} {ex}")
e.hook_function(0x4120c0, blit, 8, pops=True)
e.write(TGT, struct.pack("<I", TVT))
for g, v in ((0x442080, WORLD + 0x20000), (0x442104, WORLD + 0x21000), (0x442120, WORLD + 0x22000), (0x4420fc, JIM), (0x442074, KBD), (0x44206c, MOUSE)): e.write(g, struct.pack("<I", v))
e.write(KBD + 0x34, struct.pack("<I", STATE)); e.write(MOUSE + 0x1c, struct.pack("<I", BTNS))
for k in range(6): e.write(LINES + 16 * k, bytes(16)); e.write(LINES + 16 * k + 8, struct.pack("<I", [0, 1000, 2000, 0, 3000, 4000][k])); e.write(LINES + 16 * k + 0xc, struct.pack("<I", ARG))

def tame(dat):
    """The random sequence fields (positions, movement, hit rectangle) are made small: the game's are, and large ones only test integer overflow."""
    b = bytearray(dat); p = 0x18
    for _ in range(u16(b, 0)):
        for off in (0, 2, 4, 6, 0xc, 0xe, 0x10, 0x12): struct.pack_into("<h", b, p + off, rnd.randint(-40, 40))
        p += 0x18 + u16(b, p + 0x16)
    return bytes(b)
def make_bank():
    while True:
        dat, sizes = make_file(0, False, True)
        if u16(dat, 0) >= 5 and u16(dat, 4) >= 1: return tame(dat)
def players():
    arr = e.u32(JIM + 0x34); return [e.u32(arr + 4 * k) for k in range(9)]
def state():
    ps = players(); cur = e.u32(JIM + 0x38); idx = ps.index(cur) if cur in ps else -1
    r = lambda off, n=1: struct.unpack("<" + "i" * n, e.read(JIM + off, 4 * n))
    s = [*r(0x4c, 2), *r(0x54, 2), r(0x10c)[0], e.read(JIM + 0xd5, 1)[0], r(0xfc)[0], *r(0xe0, 2), *r(0xe8, 2), e.read(JIM + 0xf0, 1)[0], tname(e.u32(JIM + 0xf4)), *r(0x104, 2),
         e.read(JIM + 0xd4, 1)[0], e.read(JIM + 0xc, 1)[0], ((e.u32(JIM + 0xc4) - LINES) // 16 if e.u32(JIM + 0xc4) else -1), e.u32(JIM + 0xc8), r(0xcc)[0], r(0xa8)[0], *r(0xa0, 2), idx, *r(0x10, 4)]
    for p in ps:
        s += [e.u32(p + 0x18), struct.unpack("<H", e.read(p + 0x30, 2))[0], struct.unpack("<H", e.read(p + 0x24, 2))[0], struct.unpack("<H", e.read(p + 0x26, 2))[0], i32(e.u32(p + 8)), i32(e.u32(p + 0x10)),
              *struct.unpack("<hhhh", e.read(p + 0x28, 8)), e.u32(p + 0xc)]
    return " ".join(map(str, s))
def apply_world():
    e.write(STATE, bytes([world["skip"] & 1])); e.write(BTNS, bytes([0, 0])); e.write(JIM + 0x112, bytes([world["subs"]]))
    e.write(SPECIAL + 0x78, struct.pack("<I", AREA if world["has"] else 0)); e.write(OTHER + 0x78, struct.pack("<I", AREA if world["ohas"] else 0))
    e.write(CAM + 0x34, struct.pack("<ii", world["camx"], world["camy"]))

out = []
def op(text, addr, args, ret=True):
    del log[:]
    r = e.call(addr, args, conv="thiscall", this=JIM, max_insns=3_000_000)
    out.append("o " + text); out.append("r " + (str(r & 0xffffffff) if ret else "-")); out.append("s " + state()); out.extend(". " + l for l in log)
def note(text): out.append("o " + text); out.append("r -"); out.append("s " + state())
for sid in range(1, 401):
    st["heap"] = HEAP; world.update(now=0, ids=0, rcalls=0, skip=0, voices=0, subs=0, base=0, scale=0x100, fp=0, ret=1, has=1, ohas=1, mx=0, my=0, camx=0, camy=0)
    files = [make_bank() for _ in range(9)]
    data = bytearray(0x4000 * 10)
    for k, f in enumerate(files): data[OFFS[k] + 4:OFFS[k] + 4 + len(f)] = f
    st["data"] = bytes(data); st["pos"] = 0
    e.write(DISP + 0x67c, b"\0"); e.write(JIM, bytes(0x130)); e.write(LEVEL, bytes(0x100)); e.write(CAM, bytes(0x100)); e.write(OTHER, bytes(0x100)); e.write(SPECIAL, bytes(0x100))
    e.write(0x4421ec, struct.pack("<I", 0)); st["now"] = 0
    e.call(0x4070c0, [], conv="thiscall", this=JIM, max_insns=20_000_000)
    for p in players(): e.call(0x401510, [], conv="thiscall", this=p, max_insns=5_000_000)
    for k, p in enumerate(players()):
        for i in range(u16(e.read(p + 0x20, 2), 0)): imgname[e.u32(e.u32(p) + 4 * i)] = f"{k}.{i}"
    e.write(JIM + 0x40, struct.pack("<I", LEVEL)); e.write(LEVEL + 0x80, struct.pack("<I", CAM)); e.write(LEVEL + 0x90, struct.pack("<I", SPECIAL))
    # Jim as the constructor leaves him: object fields and the points of every sequence of the banks
    r32 = lambda off: e.u32(JIM + off); ps = players()
    pts = " ".join(f"{k}:" + ",".join(str(u16(e.read(e.u32(p + 0x1c) + 0x24 * i + 0x1c, 2), 0)) for i in range(u16(e.read(p + 0x22, 2), 0))) for k, p in enumerate(ps))
    out.append(f"S {sid} {st['data'].hex()}")
    out.append("o C " + " ".join(map(str, [r32(0x68), struct.unpack('<h', e.read(JIM + 0x5e, 2))[0], r32(0x44), r32(0x48), e.read(JIM + 0x5c, 1)[0], u16(e.read(JIM + 0xd0, 2), 0), u16(e.read(JIM + 0xd2, 2), 0),
                                              u16(e.read(JIM + 0x100, 2), 0), r32(0xfc), r32(0x10c), r32(0x4c), r32(0x50), r32(0xc0), r32(0xac), *[e.read(JIM + 0xb0 + 4 * i, 1)[0] for i in range(4)], pts])))
    out.append("r -"); out.append("s " + state())
    px, py = rnd.randint(-50, 640), rnd.randint(-50, 480)
    e.write(JIM + 0x4c, struct.pack("<ii", px, py)); e.write(JIM + 0x54, struct.pack("<ii", px, py))
    world["now"] = 0
    apply_world(); note(f"I {px} {py}")
    for step in range(rnd.randint(8, 50)):
        if rnd.random() < 0.55:
            world["now"] = (world["now"] + rnd.choice([0, 20, 50, 100, 101, 150, 400, 5000])) & 0xffffffff; st["now"] = world["now"]
            world.update(skip=int(rnd.random() < 0.12), voices=rnd.getrandbits(8) & rnd.getrandbits(8), subs=int(rnd.random() < 0.6), base=rnd.choice([0, 1, 3, 2]),
                         scale=rnd.choice([0x100, 0x80, 0x180, 0x40, 0x200]), fp=rnd.choice([0, 1]), ret=rnd.choice([1, 1, 0, 2, 3]), has=int(rnd.random() < 0.9), ohas=int(rnd.random() < 0.8),
                         camx=rnd.randint(-100, 100), camy=rnd.randint(-100, 100))
            apply_world()
            out.append(f"o E {world['now']} {world['skip']} {world['voices']} {world['subs']} {world['base']} {world['scale']} {world['fp']} {world['ret']} {world['has']} {world['ohas']} {world['camx']} {world['camy']}"); out.append("r -"); out.append("s " + state())
        r = rnd.random()
        if r < 0.08: k = rnd.randrange(6); op(f"Y {k}", 0x407ce0, [LINES + 16 * k], ret=False)
        elif r < 0.15: k = rnd.randrange(6); op(f"A {k}", 0x410330, [LINES + 16 * k, 0xffc0, 0], ret=False)
        elif r < 0.5: op("U", 0x407790, [])
        elif r < 0.62:
            cx, cy = struct.unpack("<ii", e.read(JIM + 0x4c, 8))
            dx, dy = cx + rnd.choice([rnd.randint(-60, 60), rnd.randint(-300, 300), 0]), cy + rnd.choice([rnd.randint(-60, 60), rnd.randint(-250, 250), 0])
            t = rnd.randint(0, 1); d = rnd.randint(0, 1)
            op(f"W {dx} {dy} {t} {d}", 0x407600, [dx & 0xffffffff, dy & 0xffffffff, TGT if t else 0, d])
        elif r < 0.7: op("X", 0x4079e0, [OTHER])
        elif r < 0.76: f = rnd.randint(1, 4); op(f"F {f}", 0x407a80, [f], ret=False)
        elif r < 0.9: op("D", 0x407ac0, [CAM], ret=False)
        else: op("B", 0x407730, [], ret=False)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/jim.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
