#!/usr/bin/env python3
"""Vectors for the board game's piece display (0x418e90): runs the ORIGINAL routine in Unicorn on real list memory (the level's
object list with its cursor, header at list+0, sentinel at +4, nodes of next and value), real name keys (0x415c40) and the
real board, blink and winning-cell globals (0x442228 state, 0x442270 blink phase, 0x442224 blink stamp, 0x442278 board,
0x442238 winning cells). Replaced by Python: the clock (0x413a10), the visibility setter (0x401d80) and the bank selector
(0x401de0), both logged per object.
Output: local-media/vectors/pieces.txt (ignored). Consumed by re/tools/drivers/pieces_driver.cpp.
Lines: B <hex bank file> (once)
       K now state blink stamp board(hex 57) winning(8 ints, four pairs) stale names(comma separated, one per object) | blink stamp objects
  objects: for each object "v:b" (v the last visibility set, b the last bank selected or -)
       #   end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_ECX

rnd = random.Random(20261104)
M = 0xffffffff
HEAP = 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"heap": HEAP, "now": 0}
def mk(size):
    a = (st["heap"] + 15) // 16 * 16; st["heap"] = a + size; e.write(a, bytes(size)); return a
def s32(v): return v - (1 << 32) if v & 0x80000000 else v

log = []
objs = {}
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
e.hook_function(0x413a10, lambda emu: st["now"])
e.hook_function(0x401d80, lambda emu, v: log.append(("vis", objs.get(ecx(), -9), v & 0xff)), 1, pops=True)
def sel(emu, bank, first, last, loop, total):
    log.append(("bank", objs.get(ecx(), -9), bank & 0xff, first, s32(last), loop & 0xff, total & 0xff)); return 0
e.hook_function(0x401de0, sel, 5, pops=True)

def hash_name(name):
    p = mk(16); e.write(p, name.encode() + b"\0"); e.call(0x415c40, [p]); return e.uc.reg_read(UC_X86_REG_EAX), e.uc.reg_read(UC_X86_REG_EDX)

NAMES = ["SUELO", "FONDO", "MARCO", "BOTON", "X1", "TAPA"]
JIM, LEVEL, LISTP, HDR = mk(0x100), mk(0x100), mk(0x10), mk(0x40)
e.write(0x4420fc, struct.pack("<I", JIM)); e.write(JIM + 0x40, struct.pack("<I", LEVEL)); e.write(LEVEL + 0x74, struct.pack("<I", LISTP))
e.write(LISTP, struct.pack("<I", HDR))
WORLD_END = st["heap"]
def bank_file(nseq):
    out = bytearray(struct.pack("<HHHHIIIB3s", nseq, 0, 0, 0, 0, 0, 0, 0, b"\0\0\0"))
    for _ in range(nseq):
        out += struct.pack("<hhhhHhIIHH", 0, 0, 0, 0, rnd.randint(1, 300), -1, 0, 0, 0, 0)
    return bytes(out)
out = ["B " + (bytes(4) + bank_file(3)).hex()]
for case in range(400):
    st["heap"] = WORLD_END; del log[:]; objs.clear()
    pre = rnd.choice([0, 0, 1, 3, 10, 40])
    nficha = rnd.choice([49, 49, 49, 49, 50, 60, 48, 40, 20, 7, 1, 0])
    post = rnd.choice([0, 0, 2, 5])
    names = [rnd.choice(NAMES) for _ in range(pre)] + ["FICHA"] * nficha + [rnd.choice(NAMES + ["FICHA"]) for _ in range(post)]
    nodes, ops = [], []
    for i, nm in enumerate(names):
        o = mk(0x100); lo, hi = hash_name(nm); e.write(o + 0x44, struct.pack("<II", lo, hi)); objs[o] = i; ops.append(o)
        nodes.append(mk(8))
    for i, n in enumerate(nodes):
        e.write(n, struct.pack("<II", nodes[i + 1] if i + 1 < len(nodes) else 0, ops[i]))
    e.write(HDR + 4, struct.pack("<I", nodes[0] if nodes else 0))
    stale = rnd.randrange(len(names)) if names and (rnd.random() < 0.5 or nficha == 0) else -1
    if nficha == 0 and not names: continue
    e.write(HDR + 0x18, struct.pack("<I", ops[stale] if stale >= 0 else 0))
    e.write(HDR + 0xc, struct.pack("<I", HDR + 4 + 0)); e.write(HDR + 0x14, bytes([rnd.choice([0, 1])]))
    now = rnd.randint(1000, 9000); st["now"] = now
    state = rnd.choice([0, 0, 1, 2]); blink = rnd.choice([0, 1]); stamp = now - rnd.choice([0, 100, 199, 200, 201, 250, 500, 3000, -50])
    board = bytearray(57); board[0] = rnd.randint(0, 6)
    for c in range(7):
        n = rnd.choice([0, 1, 2, 3, 4, 5, 6, 7, 7]); board[1 + 8 * c] = n
        for r in range(7): board[2 + 8 * c + r] = rnd.randint(1, 2) if r < n else rnd.choice([0, 0, 1, 2])
    win = []
    for _ in range(4):
        win += [rnd.randint(0, 6), rnd.randint(0, 6)]
    e.write(0x442228, bytes([state])); e.write(0x442270, bytes([blink])); e.write(0x442224, struct.pack("<I", stamp & M))
    e.write(0x442278, bytes(board)); e.write(0x442238, struct.pack("<8i", *win))
    try:
        e.call(0x418e90, [])
    except Exception as ex:
        print("fail case", case, len(names), nficha, ex); raise
    last = {}
    for l in log:
        d = last.setdefault(l[1], {})
        if l[0] == "vis": d["v"] = l[2]
        else:
            d["b"] = l[2]
            assert l[3:] == (0, 0, 0, 0), l
    res = []
    for i in range(len(names)):
        d = last.get(i, {})
        res.append(f"{d.get('v', 0)}:{d.get('b', '-')}")
    out.append(f"K {now} {state} {blink} {s32(stamp & M)} {board.hex()} {' '.join(map(str, win))} {stale} {','.join(names) or '-'} | {e.read(0x442270,1)[0]} {s32(e.u32(0x442224))} {' '.join(res) or '-'}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/pieces.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
