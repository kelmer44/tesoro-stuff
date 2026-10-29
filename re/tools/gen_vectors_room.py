#!/usr/bin/env python3
"""Vectors for Room: ORIGINAL Room_Ctor 0x410420, Room_InsertSorted 0x4104a0 and Room_RemoveObject 0x410530 in Unicorn
(pool allocator replaced by a zeroing bump allocator) on fake objects whose x (+0x4c) is random, vs engine Room.
Output: local-media/vectors/room.txt (ignored), consumed by re/tools/drivers/room_driver.cpp.
Lines: C <capacity> <index> <field4> <sortOffset> <extent> / in <id> <x> / rm <id> / S <n> <id:x>... | <last value id or -1> / #
"""
import os, random, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261030)
HEAP, ROOM, LISTHDR, REC, OBJS = 0x2000000, 0x100100, 0x100200, 0x100300, 0x300000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(OBJS, 0x10000, 3)
st = {"heap": HEAP}
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
e.hook_function(0x40db40, alloc_zero, 2)
u32 = e.u32
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def oid(addr): return (addr - OBJS) // 0x100 if addr else -1
def state():
    L = u32(ROOM + 0xc); node = u32(L + 4); n = []
    while node and len(n) < 100:
        n.append(f"{oid(u32(node + 4))}:{s32(u32(OBJS + oid(u32(node + 4)) * 0x100 + 0x4c))}"); node = u32(node)
    return f"S {len(n)} {' '.join(n) or '-'} | {oid(u32(L + 0x18))}"
NOBJ = 12
out = []
for case in range(600):
    cap = rnd.randint(1, 12)
    st["heap"] = HEAP; e.write(ROOM, bytes(0x10)); e.write(LISTHDR, bytes(0x20)); e.write(REC, bytes(0x40))
    e.write(LISTHDR + 0x10, cap.to_bytes(4, "little"))
    f4, so, ex, idx = rnd.getrandbits(32), rnd.randint(-100, 300), rnd.randint(0, 50), rnd.randint(0, 9)
    e.write(REC + 0x24, f4.to_bytes(4, "little")); e.write(REC + 0x22, ex.to_bytes(2, "little"))
    e.call(0x410420, [LISTHDR, REC, idx, so & 0xffff], conv="thiscall", this=ROOM)
    out.append(f"C {cap} {idx} {f4} {so} {ex}"); out.append(state())
    xs = {}; inlist = 0
    for _ in range(rnd.randint(2, 30)):
        if rnd.random() < 0.65 and inlist < cap + 1:
            i = rnd.randrange(NOBJ); x = rnd.choice([rnd.randint(-5, 5), rnd.randint(-2**31, 2**31 - 1), rnd.randint(-640, 640)])
            e.write(OBJS + i * 0x100 + 0x4c, (x & 0xffffffff).to_bytes(4, "little"))
            # an object already in the list keeps one node per insert; later ones are separate nodes (as the game allows)
            e.call(0x4104a0, [OBJS + i * 0x100], conv="thiscall", this=ROOM); inlist += 1
            out.append(f"in {i} {x}")
        else:
            i = rnd.randrange(NOBJ)
            e.call(0x410530, [OBJS + i * 0x100], conv="thiscall", this=ROOM); out.append(f"rm {i}")
            inlist = max(0, inlist - 0)  # nodes return to the free list, so the bound above stays safe
        out.append(state())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/room.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
