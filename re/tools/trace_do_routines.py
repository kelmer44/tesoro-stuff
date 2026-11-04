#!/usr/bin/env python3
"""Runs the original DO routines of the script block types (FANCAMAROTE, FANMOTIN, FCAPITAN, FANLUCHDICK, FFINAL, FANMMENU, FANPRESENT,
FANEMBARKE, FANMLGISLA, FANPLAYA, FEXPLOSION, FSALPANTA, CONV35, FIXCUERDA, FBARCO, CONV31, EnterStageLevel) in Unicorn for every phase,
with the callees replaced by recorders, and prints the calls and the fields written, one line per (routine, phase).
Measurement tool only. Run: re/.venv/bin/python re/tools/trace_do_routines.py [out.txt]
"""
import struct, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EDX, UC_X86_REG_ECX

A = 0x03000000
e = Emu(); e.uc.mem_map(A, 0x100000, 7)
ecx = lambda em: em.uc.reg_read(UC_X86_REG_ECX)
JIM = A + 0x200; DISPLAY = A + 0x300; POINTER = A + 0x400; BLOCK = A + 0x500
LEVEL_BASE = 0x442130
cells = {}                                           # address -> readable name
def level_index(p): return (p - LEVEL_BASE) // 4
levels = {}                                          # level object address -> index
for slot in range(47):
    lv = A + 0x2000 + 0x200 * slot; levels[lv] = slot; e.write(LEVEL_BASE + 4 * slot, struct.pack("<I", lv)); e.write(lv + 0x70, struct.pack("<I", 0x70000 + slot))
cells.update({JIM: "Jim", DISPLAY: "Display", POINTER: "Pointer"})
for lv, i in levels.items(): cells[lv] = f"L{i}"
e.write(0x4420fc, struct.pack("<I", JIM)); e.write(0x442108, struct.pack("<I", DISPLAY)); e.write(0x442128, struct.pack("<I", POINTER))
names, log = {}, []
def nm(a):
    return cells.get(a, f"@{a:x}")
def nhash(em, p):
    n = em.read(p, 16).split(b"\0")[0].decode(); k = (len(n) << 8 | sum(n.encode()), 0x5000 + len(n)); names[k] = n
    em.uc.reg_write(UC_X86_REG_EDX, k[1]); return k[0]
e.hook_function(0x415c40, nhash, 1)
objects = {}                                         # (level, name) -> object address
VT = A + 0x71000
def obj_for(level, name):
    key = (level, name)
    if key not in objects:
        o = A + 0x8000 + 0x100 * len(objects); objects[key] = o; cells[o] = f"{nm(level)}.{name}"
        e.write(o, struct.pack("<I", VT)); e.write(o + 0x5e, struct.pack("<h", 0x1234)); e.write(o + 0x4c, struct.pack("<ii", 777 + len(objects), 888 + len(objects)))
    return objects[key]
chars = {}
def find(em, lo, hi):
    lvl = ecx(em); n = names[(lo, hi)]; log.append(f"find({nm(lvl)}, {n})"); return obj_for(lvl, n)
e.hook_function(0x408780, find, 2, pops=True)
e.hook_function(0x401d80, lambda em, v: log.append(f"{nm(ecx(em))}.setVisible({v & 0xff})") or 0, 1, pops=True)
e.hook_function(0x401de0, lambda em, a, b, c, d, f: log.append(f"{nm(ecx(em))}.selectAnimBank({a & 0xff},{b & 0xffff},{c - (1 << 32) if c & 0x80000000 else c},{d & 0xff},{f & 0xff})") or 0, 5, pops=True)
e.hook_function(0x407b80, lambda em, lv, rooms, x, y: log.append(f"Jim.enterLevel({nm(lv)}, rooms={rooms:#x}, {x - (1 << 32) if x & 0x80000000 else x}, {y - (1 << 32) if y & 0x80000000 else y})") or 0, 4, pops=True)
e.hook_function(0x408520, lambda em, x, y, d: log.append(f"{nm(ecx(em))}.cameraMoveTo({x - (1<<32) if x & 0x80000000 else x},{y - (1<<32) if y & 0x80000000 else y},{d})") or 0, 3, pops=True)
e.hook_function(0x4123e0, lambda em, a, b: log.append(f"fadeOut({a},{b})") or 0, 2, pops=True)
e.hook_function(0x4139e0, lambda em, ms: log.append(f"wait({ms})") or 0, 1, pops=True)
e.hook_function(0x411f20, lambda em: log.append("resetRenderQueue") or 0, 0)
e.hook_function(0x40fe20, lambda em: log.append(f"clearList({ecx(em):#x})") or 0, 0)
def slot(i, label):
    tmp = A + 0x70000 + 4 * i; e.stub_import(tmp, lambda em, *a: log.append(f"{nm(ecx(em))}.{label}({','.join(str(x) for x in a)})") or 0, 1 if label != "verb" else 0)
    e.write(VT + 4 * i, e.read(tmp, 4))
slot(8, "setActive"); slot(20, "slot20")
ROUTINES = [("FANCAMAROTE", 0x416300, 10), ("FANMOTIN", 0x4169a0, 3), ("FCAPITAN", 0x416b50, 2), ("FANLUCHDICK", 0x416b80, 3), ("FFINAL", 0x416c80, 6),
            ("FANMMENU", 0x417c80, 6), ("FANPRESENT", 0x4197a0, 7), ("FANEMBARKE", 0x4198d0, 4), ("FANMLGISLA", 0x4199a0, 2), ("FANPLAYA", 0x419a10, 2),
            ("FEXPLOSION", 0x419ac0, 2), ("FSALPANTA", 0x419b30, 3), ("CONV35", 0x419b90, 3), ("FIXCUERDA", 0x419cf0, 2), ("FBARCO", 0x419d40, 2),
            ("CONV31", 0x419d90, 5), ("EnterStageLevel", 0x419f10, 3)]
out = []
for name, addr, nphases in ROUTINES:
    for phase in range(-1, nphases + 1):
        objects.clear()
        e.write(JIM + 0x4c, struct.pack("<ii", 11, 22)); e.write(JIM + 0x110, bytes([0x55, 0x66]))
        e.write(POINTER + 0x12c, struct.pack("<I", 0x99))
        # the block: level 0xB1, two characters with keys
        chp, keyp = A + 0x600, A + 0x700; e.write(BLOCK + 0x40, struct.pack("<I", 0xB1)); e.write(BLOCK + 0xb0, struct.pack("<I", chp)); e.write(BLOCK + 0xb4, struct.pack("<I", keyp)); e.write(BLOCK + 0xb8, struct.pack("<I", 2))
        for i, n in enumerate(("AAA", "BBB")): k = (len(n) << 8 | sum(n.encode()), 0x5000 + len(n)); names[k] = n; e.write(keyp + 8 * i, struct.pack("<II", *k))
        e.write(chp, bytes(8))
        # floor object of the map: camera frame +0x90 -> +0x78 -> object with a position
        for lv in levels:
            fr = lv + 0x100; floor = lv + 0x150; e.write(lv + 0x90, struct.pack("<I", fr)); e.write(fr + 0x78, struct.pack("<I", floor)); e.write(floor + 0x4c, struct.pack("<ii", 1000 + levels[lv], 2000 + levels[lv]))
        cells[BLOCK] = "block"; log.clear()
        try: e.call(addr, [phase & 0xffffffff], this=BLOCK)
        except Exception as ex: print("FAULT", name, phase, ex, hex(e.uc.reg_read(__import__("unicorn.x86_const", fromlist=["x"]).UC_X86_REG_EIP)), log); raise
        fields = []
        lv = e.u32(BLOCK + 0x40)
        if lv != 0xB1: fields.append(f"block.level={nm(lv)}")
        cache = [e.u32(chp + 4 * i) for i in range(2)]
        if any(cache): fields.append("cache=" + ",".join(nm(c) for c in cache))
        jp = struct.unpack("<ii", e.read(JIM + 0x4c, 8))
        if jp != (11, 22): fields.append(f"Jim.pos={jp}")
        b = e.read(JIM + 0x110, 2)
        if list(b) != [0x55, 0x66]: fields.append(f"Jim.+110/111={b[0]},{b[1]}")
        if e.u32(POINTER + 0x12c) != 0x99: fields.append(f"pointer.+12c={e.u32(POINTER + 0x12c)}")
        for (l, n), o in objects.items():
            if struct.unpack("<h", e.read(o + 0x5e, 2))[0] != 0x1234: fields.append(f"{nm(l)}.{n}.sort={struct.unpack('<h', e.read(o + 0x5e, 2))[0]}")
            if struct.unpack("<ii", e.read(o + 0x4c, 8)) != (777 + list(objects).index((l, n)) + 1 - 1 + 0, 888 + list(objects).index((l, n)) + 1 - 1 + 0): pass
        out.append(f"{name} phase {phase}: " + "; ".join(log + fields) if (log or fields) else f"{name} phase {phase}: -")
text = "\n".join(out)
if len(sys.argv) > 1: Path(sys.argv[1]).write_text(text + "\n")
print(text)
