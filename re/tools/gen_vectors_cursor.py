#!/usr/bin/env python3
"""Vectors for the cursor: ORIGINAL Cursor_Ctor 0x402240 (with the base constructors it calls) and
Cursor_ProcessCandidates 0x402e80 in Unicorn, on a fake mouse (buttons byte) and fake candidate objects whose vtable slots
14-18 are machine-code stubs returning scripted values, vs engine Cursor. Lists are built with the original List code.
Output: local-media/vectors/cursor.txt (ignored), consumed by re/tools/drivers/cursor_driver.cpp.
Lines: C <flags> <sortValue> <label byte> / O <i> <key> <accept> <r14> <r15> <r17> / F <buttons> <verb> /
       H <ids> (hovered list) / K <ids> (previous-frame list) / T <ids> (touched list) / P /
       R <result> | t <slot:object[:verb]>... | H <ids> | T <ids> | K <ids>   #
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP
from emu import Emu

rnd = random.Random(20261104)
HEAP, CUR, MOUSE, BTN, PLAY, OBJS, VT, CODE = 0x2000000, 0x100100, 0x100600, 0x100700, 0x100800, 0x300000, 0x340000, 0x4000000
NOBJ = 6
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
for base, size in ((OBJS, 0x10000), (VT, 0x10000), (CODE, 0x200000)): e.uc.mem_map(base, size, 7)
st = {"heap": HEAP}
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
e.hook_function(0x40db40, alloc_zero, 2); e.hook_function(0x40f650, alloc_zero, 2)
u32 = e.u32
def oid(a): return (a - OBJS) // 0x100 if a else -1
trace = []
def on_code(uc, addr, size, _):
    if (addr - CODE) % 0x20 == 0:
        k = (addr - CODE) % 0x1000 // 0x20; i, slot = divmod(k, 5); slot += 14
        if slot in (14, 15):
            esp = uc.reg_read(UC_X86_REG_ESP); trace.append(f"{slot}:{i}:{struct.unpack('<I', uc.mem_read(esp + 4, 4))[0]}")
        else: trace.append(f"{slot}:{i}")
e.uc.hook_add(UC_HOOK_CODE, on_code, begin=CODE, end=CODE + 0x1fffff)
def ids(holder):
    L = u32(holder); node = u32(L + 4); n = []
    while node and len(n) < 100: n.append(oid(u32(node + 4))); node = u32(node)
    return " ".join(map(str, n)) or "-"
def fill(holder, lst):
    e.call(0x40f870, [], this=holder)
    for i in lst: e.call(0x40f800, [OBJS + i * 0x100], conv="thiscall", this=holder)
def stub(addr, ret, pop):
    code = b"\xb8" + struct.pack("<I", ret) + (b"\xc2\x04\x00" if pop else b"\xc3"); e.write(addr, code)

out = []
for case in range(400):
    st["heap"] = HEAP; e.write(CUR, bytes(0x130)); e.write(0x442060, bytes(4)); e.write(MOUSE, bytes(0x40)); e.write(BTN, bytes(4))
    e.call(0x402240, [], this=CUR)
    out.append(f"C {u32(CUR + 0x68)} {struct.unpack('<H', e.read(CUR + 0x5e, 2))[0]} {e.read(CUR + 0x78, 1)[0]}")
    HA, HB, SH = u32(CUR + 0x6c), u32(CUR + 0x70), u32(0x442060)
    e.write(MOUSE + 0x1c, struct.pack("<I", BTN)); e.write(0x44206c, struct.pack("<I", MOUSE))
    e.write(CUR + 0x38, struct.pack("<I", PLAY))
    for i in range(NOBJ):
        e.write(OBJS + i * 0x100, bytes(0x100)); e.write(OBJS + i * 0x100, struct.pack("<I", VT + i * 0x40))
        key = rnd.randint(0, 3); acc = rnd.choice([0, 1, 1, 5]); r = [rnd.choice([0, 0, 1, 2, 3]) for _ in range(4)]
        e.write(OBJS + i * 0x100 + 0x44, struct.pack("<II", key, 0))
        for k, (slot, ret, pop) in enumerate(((14, r[0], 1), (15, r[1], 1), (16, acc, 0), (17, r[2], 0), (18, r[3], 0))):
            a = CODE + case * 0x1000 + (i * 5 + k) * 0x20; stub(a, ret, pop); e.write(VT + i * 0x40 + slot * 4, struct.pack("<I", a))
        out.append(f"O {i} {key} {acc} {r[0]} {r[1]} {r[2]}")
    for frame in range(rnd.randint(1, 3)):
        buttons = rnd.choice([0, 0, 8, 0x10, 0x18, 0x28]); verb = rnd.randint(0, 9)
        e.write(BTN, bytes([buttons])); e.write(PLAY + 0x30, struct.pack("<H", verb)); out.append(f"F {buttons} {verb}")
        if frame == 0:
            hv = [rnd.randrange(NOBJ) for _ in range(rnd.choice([0, 1, 2, 4]))]; fill(HA, hv); out.append("H " + (" ".join(map(str, hv)) or "-"))
        kv = [rnd.randrange(NOBJ) for _ in range(rnd.choice([0, 1, 3]))]; fill(SH, kv); out.append("K " + (" ".join(map(str, kv)) or "-"))
        tv = [rnd.randrange(NOBJ) for _ in range(rnd.choice([0, 1, 2, 3, 5]))]; fill(HB, tv); out.append("T " + (" ".join(map(str, tv)) or "-"))
        trace.clear(); r = s = e.call(0x402e80, [], this=CUR)
        out += ["P", f"R {s} | t {' '.join(trace) or '-'} | H {ids(HA)} | T {ids(HB)} | K {ids(SH)}"]
    # switching the pointer on or off, with a label showing
    e.write(CUR + 0x78, b"ABC\0"); out.append("L")
    flag = rnd.choice([0, 0, 1]); trace.clear()
    e.call(0x4031a0, [flag], conv="thiscall", this=CUR)
    out += [f"SA {flag}", f"Q {u32(CUR + 0x68)} {e.read(CUR + 0x78, 1)[0]} | t {' '.join(trace) or '-'} | H {ids(HA)}"]
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/cursor.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
