#!/usr/bin/env python3
"""Vectors for the kind 2 object (named group): runs the ORIGINAL constructor (0x414930, with the animated base 0x401aa0
and the name hash 0x415c40) and OnAttach (0x414a90, with the list insert 0x415200) in Unicorn. Replaced by Python: pool
allocator, game clock, the object search (0x40f680: answers a handle for the names of the fake level, 0 for others).
Output: local-media/vectors/itemgroup.txt (ignored). Consumed by re/tools/drivers/itemgroup_driver.cpp.
Lines: K option count levelNames(comma, hex each) data(hex) | keys lo:hi,... | members h,... (0 = not found, else index+1)
       #   end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261031)
M = 0xffffffff
OBJ, REC, ROOM, LV, DATA, KEYBUF, TBL = 0x101200, 0x101600, 0x101680, 0x101a00, 0x101c00, 0x101b00, 0x101400
FP, ARCH, HEAP = 0x100200, 0x100100, 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3); e.set_fs()
st = {"heap": HEAP, "now": 0, "keys": {}}

def alloc(emu, size, align, arena):
    a = (st["heap"] + align - 1) // align * align; st["heap"] = a + size; return a
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
def find(emu, lo, hi): return st["keys"].get((lo, hi), 0)
e.hook_function(0x40dc10, alloc, 3, pops=True); e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
e.hook_function(0x413a10, lambda emu: st["now"]); e.hook_function(0x40f680, find, 2, pops=True)
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442120, struct.pack("<I", 0x100380))

def key_of(name):
    e.write(KEYBUF, name + b"\0" * 14); e.call(0x415c40, [KEYBUF]); k = e.edx_eax(); return (k & M, k >> 32)
def rname():
    return "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") for _ in range(rnd.choice([1, 1, 2, 5, 8, 11, 12]))).encode()

out = []
for case in range(300):
    st["heap"] = HEAP
    M_ = rnd.choice([0, 1, 3, 5, 8]); lnames = []
    while len(lnames) < M_:
        n = rname()
        if n not in lnames: lnames.append(n)
    st["keys"] = {key_of(n): 1 + i for i, n in enumerate(lnames)}
    N = rnd.choice([0, 1, 2, 3, 5, 9, 45, 45, 260]); gnames = [rnd.choice(lnames) if lnames and rnd.random() < .7 else rname() for _ in range(N)]
    option = rnd.getrandbits(8)
    data = bytes([option, rnd.getrandbits(8)]) + struct.pack("<H", N) + bytes(rnd.getrandbits(8) for _ in range(12)) + b"".join(n + b"\0" * (12 - len(n)) for n in gnames)
    e.write(DATA, data + bytes(0x20)); e.write(REC, bytes(0x20)); e.write(REC + 0xc, b"GRP\0"); e.write(REC + 0x1c, struct.pack("<I", 0x20))
    e.write(ROOM, bytes(0x10)); e.write(LV, bytes(0x100)); e.write(OBJ, bytes(0x100)); e.write(LV + 0x74, struct.pack("<I", 0x108000)); e.write(0x108000, struct.pack("<I", 0x108040))
    try:
        e.call(0x414930, [REC, ROOM, TBL, DATA], conv="thiscall", this=OBJ)
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP
        sys.exit(f"fault in ctor, case {case}, at {e.uc.reg_read(UC_X86_REG_EIP):#x}, N {N}")
    e.write(OBJ + 0x40, struct.pack("<I", LV)); e.write(OBJ + 0x3c, struct.pack("<I", ROOM))
    try:
        e.call(0x414a90, [], this=OBJ)
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP
        sys.exit(f"fault in attach, case {case}, at {e.uc.reg_read(UC_X86_REG_EIP):#x}, N {N}")
    # walk the key list and the member list through their nodes
    K = e.u32(OBJ + 0x6c); n = e.u32(K + 4); keys = []
    while n and len(keys) < 1000: keys.append((e.u32(n + 4), e.u32(n + 8))); n = e.u32(n)
    L = e.u32(OBJ + 0x74); n = e.u32(L + 4); mem = []
    while n and len(mem) < 1000: mem.append(e.u32(n + 4)); n = e.u32(n)
    opt = e.read(OBJ + 0x70, 1)[0]; cnt = struct.unpack("<H", e.read(OBJ + 0x72, 2))[0]
    out.append(f"K {opt} {cnt} {','.join(n.hex() for n in lnames) or '-'} {data.hex()} | {','.join(f'{a}:{b}' for a, b in keys) or '-'} | {','.join(map(str, mem)) or '-'}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/itemgroup.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
