#!/usr/bin/env python3
"""Vectors for the menu item click: runs the ORIGINAL 0x417b40 (vtable 0042c000 slot 14) and 0x40f040 (slot 15, the right click) in Unicorn on a fake item with each
action number. The things a click calls (fade 0x4123e0, enter level 0x407b80, name key 0x415c40, script lookup and start 0x4060c0 / 0x405fd0, 0x4026d0, 0x414560)
are Python hooks that log what they were asked.
Output: local-media/vectors/click.txt (ignored). Consumed by re/tools/drivers/click_driver.cpp.
Lines: <action> <slot: 14|15> = <return> | <events>"""
import os, random, struct, sys, zlib
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EDX

e = Emu(); OBJ, NUEVO, ROOMS, JIM, JLVL, TABLE, DISP, CUR = 0x101200, 0x101400, 0x101500, 0x101600, 0x101700, 0x101800, 0x101900, 0x101a00
ev = []; keys = {}
def key(emu, s):
    a, b = s, bytearray()
    while emu.read(a, 1) != b"\0": b.append(emu.read(a, 1)[0]); a += 1
    name = bytes(b).decode(); lo = zlib.crc32(name.encode()); hi = 0x1000 + len(name); keys[(lo, hi)] = name
    emu.uc.reg_write(UC_X86_REG_EDX, hi); return lo
e.hook_function(0x415c40, key, 1)
e.hook_function(0x4123e0, lambda emu, steps, delay: ev.append(f"fade {steps} {delay}"), 2, pops=True)
e.hook_function(0x407b80, lambda emu, lvl, room, x, y: ev.append(f"enter {'NUEVO.LVL' if lvl == NUEVO and room == ROOMS else '?'} {x} {y}"), 4, pops=True)
def script(emu, bl, bh, ll, lh, z, level):
    ev.append(f"script {keys.get((bl, bh), '?')} {keys.get((ll, lh), '?')} {z} {'jim' if level == JLVL else 'item' if level == 0x77 else '?'}"); return 0x5555
e.hook_function(0x4060c0, script, 6, pops=True)
e.hook_function(0x405fd0, lambda emu: ev.append("run"), 0)
e.hook_function(0x4026d0, lambda emu: ev.append("unported thumbnail"), 0)
e.hook_function(0x414560, lambda emu: ev.append("unported start"), 0)
e.write(0x4421f4, struct.pack("<I", NUEVO)); e.write(NUEVO + 0x70, struct.pack("<I", ROOMS)); e.write(0x4420fc, struct.pack("<I", JIM)); e.write(JIM + 0x40, struct.pack("<I", JLVL))
e.write(0x4420b8, struct.pack("<I", TABLE)); e.write(0x442108, struct.pack("<I", DISP)); e.write(0x442128, struct.pack("<I", CUR)); e.write(0x442128 - 0x442128 + OBJ + 0x40, struct.pack("<I", 0x77))
out = []
for action in list(range(0, 7)) + [200, 0xffffffff]:
    for slot in (14, 15):
        ev.clear(); e.write(OBJ + 0xa0, struct.pack("<I", action))
        if slot == 14:
            r = e.call(0x417b40, [], this=OBJ)
        else:
            # the right click calls slot 14 through the vtable: give the object one
            e.write(OBJ, struct.pack("<I", OBJ + 0x100)); e.write(OBJ + 0x100 + 0x38, struct.pack("<I", 0x417b40))
            r = e.call(0x40f040, [0], conv="thiscall", this=OBJ)
        out.append(f"{action} {slot} = {r & 0xffffffff} | " + (" ; ".join(ev) or "-"))
os.makedirs("local-media/vectors", exist_ok=True); open("local-media/vectors/click.txt", "w").write("\n".join(out) + "\n"); print(len(out), "lines")
