#!/usr/bin/env python3
"""Vectors for the cursor's hover label: ORIGINAL Cursor_SetText 0x4023f0 in Unicorn on a fake cursor (text buffer at +0x78,
colour at +0x118) vs engine HoverText. Texts are at most 100 characters (the original does not limit the copy).
Output: local-media/vectors/hovertext.txt (ignored), consumed by re/tools/drivers/hovertext_driver.cpp.
Lines: C / set <colour> <hex text | -null> / S <colour> <160 byte buffer hex>   #
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261103)
CUR, STR = 0x100100, 0x100800
e = Emu()
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
out = []
for case in range(500):
    e.write(CUR, bytes(0x130)); out.append("C")
    def state(): return f"S {struct.unpack('<H', e.read(CUR + 0x118, 2))[0]} {e.read(CUR + 0x78, 160).hex()}"
    out.append(state())
    for _ in range(rnd.randint(2, 12)):
        colour = rnd.choice([-1, 0, 1, 0xffff, 0x7fe0, 70000, -5, rnd.randint(1, 65535), 0x12345678])
        if rnd.random() < 0.2: text = None
        else: text = bytes(rnd.choice(b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefgh 0123456789\xe1\xf1") for _ in range(rnd.choice([0, 1, 3, 4, 5, 8, 20, rnd.randint(0, 100)])))
        if text is None: ptr = 0
        else: e.write(STR, text + b"\0"); ptr = STR
        e.call(0x4023f0, [ptr, colour], conv="thiscall", this=CUR)
        out.append(f"set {colour} {text.hex() or '-' if text is not None else '-null'}"); out.append(state())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/hovertext.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
