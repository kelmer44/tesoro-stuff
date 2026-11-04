#!/usr/bin/env python3
"""Draws the REAL levels with the ORIGINAL code: Level_Load (0x4080e0, pictures loaded), Level_AttachObjects (0x408540), then for three
frame times Display_ResetRenderQueue (0x411f20), Level_UpdateRoomCameras (0x408550, which makes every object draw) and Display_Compose
(0x412e10), in Unicorn on the shipped Tesoro.DAT/TRE. The CRC-32 of each of the 480 frame rows is recorded.
Object kinds other than 0, 1 and 3 (plain, camera frame, sprite) are built as the plain object: the kinds that need the rest of the game are not run.
Output: local-media/vectors/realscenes.txt (ignored), read by drivers/realscene_driver.cpp.
Lines: R <name> <entry>   T <frame time> then F <crc of row 0> ... <crc of row 479>
Usage: gen_vectors_realscenes.py [TESORO folder]
"""
import os, struct, sys, zlib
folder = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
here = os.path.dirname(os.path.abspath(__file__))
src = open(os.path.join(here, "gen_vectors_level.py")).read()
exec(src[:src.index("out = []\nfor case")])          # emulator, hooks, state()
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP
ROWS, FRAME = 0xb000000, 0xb200000
e.uc.mem_map(HEAP + 0x1000000, 0x7000000, 3)         # room for the pictures of the largest levels
e.uc.mem_map(ROWS, 0x150000, 3); e.uc.mem_map(FRAME, 0xa0000, 3)
e.write(DISP + 0x670, struct.pack("<I", ROWS)); e.write(DISP + 0x674, struct.pack("<I", ROWS + 240000 * 4))

def factory_entry(uc, addr, size, _):
    record = e.u32(uc.reg_read(UC_X86_REG_ESP) + 4)
    if e.u32(record + 0x18) not in (0, 1, 3): e.write(record + 0x18, struct.pack("<I", 0))
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
        e.call(0x4080e0, [entry, 0], conv="thiscall", this=LV, max_insns=800_000_000)
        e.call(0x408540, [], this=LV, max_insns=100_000_000)
    except Exception as ex:
        print("FAULT loading", name, ex, file=sys.stderr); continue
    block = [f"R {name} {entry}"]
    try:
        for t in (0, 700, 5000):
            st["now"] = t
            e.write(FRAME, bytes([0xab]) * 614400)
            e.call(0x411f20, [], this=DISP)
            e.call(0x408550, [], this=LV, max_insns=400_000_000)
            e.write(DISP + 0x66a, b"\x01"); e.write(DISP + 0x678, struct.pack("<I", FRAME))
            e.call(0x412e10, [], this=DISP, max_insns=400_000_000)
            f = e.read(FRAME, 614400)
            block += [f"T {t}", "F " + " ".join(str(zlib.crc32(f[1280 * y:1280 * (y + 1)])) for y in range(480))]
    except Exception as ex:
        print("FAULT drawing", name, "at time", t, ex, "eip=%x" % e.uc.reg_read(__import__("unicorn.x86_const", fromlist=["x"]).UC_X86_REG_EIP), file=sys.stderr); continue
    out += block
    print(name, "objects:", len(objects()), file=sys.stderr)
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/realscenes.txt", "w").write("\n".join(out) + "\n")
print(len(levels), "levels")
