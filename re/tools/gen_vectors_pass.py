#!/usr/bin/env python3
"""Vectors for the object-list passes: ORIGINAL ObjectList_PassA 0x40fc20 / PassB 0x40fd20 in Unicorn on a real object list.
Fake objects have flags at +0x68 and a vtable whose slots 4 and 5 point at small machine-code stubs. A stub calls the
ORIGINAL InsertByPriority 0x40f900 and/or RemoveByObject 0x40fb20 on the list being walked, then returns 0, 1, 2 or 3.
Output: local-media/vectors/pass.txt (ignored), consumed by re/tools/drivers/pass_driver.cpp.
Lines: C / O <i> <flags> / L <i> <prio> / A <i> <slot 4|5> <ret> <nops> [ins|rm <j> <prio>]... / P <a|b> /
       R <ret> | t <ids in call order> | S <n> <id:prio>... | F <flags of every object> / #
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from unicorn import UC_HOOK_CODE
from emu import Emu

rnd = random.Random(20261031)
HEAP, OBJ, OBJS, VT, CODE = 0x2000000, 0x100100, 0x300000, 0x340000, 0x4000000
NOBJ = 7
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
for base, size in ((OBJS, 0x10000), (VT, 0x10000), (CODE, 0x100000)): e.uc.mem_map(base, size, 7)
st = {"heap": HEAP}
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
e.hook_function(0x40db40, alloc_zero, 2)
u32 = e.u32
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def oid(a): return (a - OBJS) // 0x100 if a else -1
trace = []
def on_code(uc, addr, size, _):
    if CODE <= addr < CODE + 0x100000 and (addr - CODE) % 0x40 == 0: trace.append((addr - CODE) % 0x400 // 0x40 // 2)
e.uc.hook_add(UC_HOOK_CODE, on_code, begin=CODE, end=CODE + 0xfffff)

def build_stub(addr, ops, ret):
    code = b""
    for op, j, p in ops:
        part = b"\xb9" + struct.pack("<I", OBJ)
        if op == "ins": part += b"\x68" + struct.pack("<i", p)
        part += b"\x68" + struct.pack("<I", OBJS + j * 0x100) + b"\xe8"
        target = 0x40f900 if op == "ins" else 0x40fb20
        rel = target - (addr + len(code) + len(part) + 4)
        code += part + struct.pack("<i", rel)
    code += b"\xb8" + struct.pack("<I", ret) + b"\xc3"
    assert len(code) <= 0x40
    return code

def state():
    L = u32(OBJ); node = u32(L + 4); n = []
    while node and len(n) < 200:
        n.append(f"{oid(u32(node + 4))}:{s32(u32(node + 8))}"); node = u32(node)
    return f"S {len(n)} {' '.join(n) or '-'}"
def flags(): return "F " + " ".join(str(u32(OBJS + i * 0x100 + 0x68)) for i in range(NOBJ))

out = []
for case in range(500):
    st["heap"] = HEAP; e.write(OBJ, bytes(8)); e.call(0x40f890, [100, 1], conv="thiscall", this=OBJ)
    out.append("C")
    for i in range(NOBJ):
        f = rnd.choice([2, 2, 2, 0, 0x4, 0x202, 0x100 | 2, rnd.getrandbits(12)])
        e.write(OBJS + i * 0x100, bytes(0x100)); e.write(OBJS + i * 0x100, struct.pack("<I", VT + i * 0x20))
        e.write(OBJS + i * 0x100 + 0x68, struct.pack("<I", f)); out.append(f"O {i} {f}")
    for _ in range(rnd.randint(1, 8)):
        i = rnd.randrange(NOBJ); p = rnd.randint(-2, 5)
        e.call(0x40f900, [OBJS + i * 0x100, p], conv="thiscall", this=OBJ); out.append(f"L {i} {p}")
    for i in range(NOBJ):
        for slot in (4, 5):
            ops = []
            for _ in range(rnd.choice([0, 0, 0, 1, 1, 2])):
                ops.append((rnd.choice(["ins", "rm"]), rnd.randrange(NOBJ), rnd.randint(-2, 5)))
            ret = rnd.choice([0, 0, 0, 1, 1, 0, 0, 2, 3]) if rnd.random() < 0.3 else rnd.choice([0, 1])
            addr = CODE + case * 0x400 + (i * 2 + slot - 4) * 0x40   # fresh addresses: Unicorn keeps translated code
            e.write(addr, build_stub(addr, ops, ret)); e.write(VT + i * 0x20 + slot * 4, struct.pack("<I", addr))
            out.append(f"A {i} {slot} {ret} {len(ops)}" + "".join(f" {o} {j} {p}" for o, j, p in ops))
    out.append(state())
    for _ in range(rnd.randint(1, 3)):
        which = rnd.choice("ab"); trace.clear()
        r = e.call(0x40fc20 if which == "a" else 0x40fd20, [], this=OBJ)
        out.append(f"P {which}"); out.append(f"R {r} | t {' '.join(map(str, trace)) or '-'}"); out.append(state()); out.append(flags())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/pass.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
