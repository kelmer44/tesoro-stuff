#!/usr/bin/env python3
"""Runs the ORIGINAL Level_Load (0x4080e0) and Level_AttachObjects (0x408540) in Unicorn on the REAL level files of the shipped game
(Tesoro.DAT/Tesoro.TRE read from the game folder) and records the loaded structure of every level: camera, rooms, animation banks and
every object (name key, position, sort value, flags, animation players). Same harness and state format as gen_vectors_level.py.
Every object kind except the camera frame (kind 1) is built as the plain animated object (kind 0): the factory entry is hooked to
rewrite the record's kind, so the real constructors for the other kinds (which need the rest of the game) are not run. Banks are loaded
deferred (mode 2: pictures skipped).
Output: local-media/vectors/reallevels.txt (ignored), read by drivers/reallevel_driver.cpp.
Lines: R <name> <entry> / = <state after loading> / A / = <state after attaching>
Usage: gen_vectors_reallevels.py [TESORO folder]   (default /Users/gabriel/Desktop/ags/TESORO)
"""
import os, struct, sys
folder = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
here = os.path.dirname(os.path.abspath(__file__))
src = open(os.path.join(here, "gen_vectors_level.py")).read()
exec(src[:src.index("out = []\nfor case")])          # the harness: emulator, hooks, state()
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP

def factory_entry(uc, addr, size, _):
    record = e.u32(uc.reg_read(UC_X86_REG_ESP) + 4)
    if e.u32(record + 0x18) != 1: e.write(record + 0x18, struct.pack("<I", 0))
e.uc.hook_add(UC_HOOK_CODE, factory_entry, begin=0x409e50, end=0x409e50)

dat = open(os.path.join(folder, "TESORO.DAT"), "rb").read()
tre = open(os.path.join(folder, "TESORO.TRE"), "rb").read()
levels = []
for i in range(0, len(tre) - 27, 28):
    off, _, flags = struct.unpack_from("<IIB", tre, i)
    name = tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()
    if (flags & 4) and name.endswith(".LVL"): levels.append((name, off + 4))
st["data"] = dat
out = []
for name, entry in levels:
    st["pos"] = 0; st["heap"] = HEAP; st["now"] = 0
    e.write(DISP + 0x67c, b"\0"); e.write(LV, bytes([0xcd]) * 0x20); e.write(LV + 0x20, bytes(0x100))
    try:
        e.call(0x4080e0, [entry, 2], conv="thiscall", this=LV, max_insns=400_000_000)
    except Exception as ex:
        print("FAULT loading", name, ex, file=sys.stderr); continue
    out.append(f"R {name} {entry}"); out.append("= " + state())
    e.call(0x408540, [], this=LV, max_insns=100_000_000)
    out.append("A"); out.append("= " + state())
    print(name, "objects:", len(objects()), file=sys.stderr)
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/reallevels.txt", "w").write("\n".join(out) + "\n")
print(len(levels), "levels")
