#!/usr/bin/env python3
"""Vectors for the cursor lists: runs the ORIGINAL list code in Unicorn on real list memory (pool allocator replaced
by a zeroing bump allocator) with random operation sequences.
  List (8 byte nodes): Ctor 0x40f590, PushBack 0x40f800, Remove 0x40f780, Clear 0x40f870, Rewind 0x40f670, insert after cursor 0x415200
  ObjectList (12 byte nodes with priority): Ctor 0x40f890, InsertByPriority 0x40f900, ...Front 0x40fa10, RemoveByObject 0x40fb20,
  Clear 0x40fe20, EndFrame 0x40fe40
Output: local-media/vectors/list.txt (ignored). Consumed by re/tools/drivers/list_driver.cpp.
Lines: C <L|O> <capacity> / op lines (pb v, rm v ret, cl, rw, ia v, io v p, if v p, ro v ret, ef ret) each followed by
       S <n> <value:priority>... | dataValue dataPriority      (state after the op)
       F <key> <found value|->      (find by predicate, no cursor change)      #  end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261029)
HEAP, OBJ = 0x2000000, 0x100100
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"heap": HEAP}
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
e.hook_function(0x40db40, alloc_zero, 2)
def u32(a): return e.u32(a)
def s32(v): return v - (1 << 32) if v & 0x80000000 else v

def state(kind):
    L = u32(OBJ); n = []; node = u32(L + 4)
    step, vo, po = (8, 4, None) if kind == "L" else (12, 4, 8)
    while node and len(n) < 100:
        n.append(f"{u32(node + vo)}:{s32(u32(node + po)) if po else 0}"); node = u32(node)
    dv = u32(L + (0x18 if kind == "L" else 0x1c)); dp = 0 if kind == "L" else s32(u32(L + 0x20))
    return f"S {len(n)} {' '.join(n) or '-'} | {dv} {dp}", n

def room(kind, cap):
    L = u32(OBJ); P = u32(L)
    freeHead = u32(P); bumpOff = 8 if kind == "L" else 12
    used = (u32(P + bumpOff) - (P + 0x10 if kind == "L" else P + 0x18)) // (8 if kind == "L" else 12)
    return freeHead != 0 or used < cap + (1 if kind == "L" else 3)

out = []
for case in range(600):
    kind = rnd.choice("LO"); cap = rnd.randint(1, 14)
    st["heap"] = HEAP; e.write(OBJ, bytes(8))
    if kind == "L": e.call(0x40f590, [cap, 1, 0], conv="thiscall", this=OBJ)
    else: e.call(0x40f890, [cap, 1], conv="thiscall", this=OBJ)
    out.append(f"C {kind} {cap}"); s, live = state(kind); out.append(s)
    vals = [rnd.randint(1, 9) for _ in range(6)] + [rnd.getrandbits(32) for _ in range(3)]
    for _ in range(rnd.randint(2, 40)):
        if kind == "L":
            op = rnd.choice(["pb", "pb", "rm", "rm", "cl", "rw", "ia", "ia"])
            v = rnd.choice(vals)
            if op == "pb" and room(kind, cap): e.call(0x40f800, [v], conv="thiscall", this=OBJ); out.append(f"pb {v}")
            elif op == "rm":
                if rnd.random() < 0.8 and live: v = int(rnd.choice(live).split(":")[0])
                r = e.call(0x40f780, [v], conv="thiscall", this=OBJ) & 0xff; out.append(f"rm {v} {r}")
            elif op == "cl": e.call(0x40f870, [], this=OBJ); out.append("cl")
            elif op == "rw": e.call(0x40f670, [], this=OBJ); out.append("rw")
            elif op == "ia" and room(kind, cap): e.call(0x415200, [v], conv="thiscall", this=u32(OBJ)); out.append(f"ia {v}")
            else: continue
        else:
            op = rnd.choice(["io", "io", "if", "ro", "ro", "cl", "ef"])
            v = rnd.choice(vals); p = rnd.randint(-3, 6)
            if op in ("io", "if") and room(kind, cap):
                e.call(0x40f900 if op == "io" else 0x40fa10, [v, p], conv="thiscall", this=OBJ); out.append(f"{op} {v} {p}")
            elif op == "ro":
                if rnd.random() < 0.8 and live: v = int(rnd.choice(live).split(":")[0])
                r = e.call(0x40fb20, [v], conv="thiscall", this=OBJ) & 0xff; out.append(f"ro {v} {r}")
            elif op == "cl": e.call(0x40fe20, [], this=OBJ); out.append("cl")
            elif op == "ef": r = e.call(0x40fe40, [], this=OBJ) & 0xff; out.append(f"ef {r}")
            else: continue
        s, live = state(kind); out.append(s)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/list.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
