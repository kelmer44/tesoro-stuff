#!/usr/bin/env python3
"""Runs the ORIGINAL Level_Load (0x4080e0, banks deferred) on the REAL level files and dumps every animation bank the level holds as the original built it:
for each bank the header values and, for every sequence, its fields, its points and scales (the part a level comparison by counts does not see), and the
image positions. Same line format as gen_vectors_anim.py. Object kinds other than the camera frame are built as the plain object (the factory entry
is hooked), as in gen_vectors_reallevels.py.
Output: local-media/vectors/realbanks.txt (ignored), read by drivers/realbank_driver.cpp.
Lines: R <name> <entry> / B <bank> / H seqs images h0 h1 h2 flags / S ... / P ...
Usage: gen_vectors_realbanks.py [TESORO folder]"""
import os, struct, sys
folder = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
here = os.path.dirname(os.path.abspath(__file__))
src = open(os.path.join(here, "gen_vectors_level.py")).read()
exec(src[:src.index("out = []\nfor case")])
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP
def factory_entry(uc, addr, size, _):
    record = e.u32(uc.reg_read(UC_X86_REG_ESP) + 4)
    if e.u32(record + 0x18) != 1: e.write(record + 0x18, struct.pack("<I", 0))
e.uc.hook_add(UC_HOOK_CODE, factory_entry, begin=0x409e50, end=0x409e50)
def hx(b): return b.hex() if b else "-"
dat = open(os.path.join(folder, "TESORO.DAT"), "rb").read(); tre = open(os.path.join(folder, "TESORO.TRE"), "rb").read()
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
    e.call(0x4080e0, [entry, 2], conv="thiscall", this=LV, max_insns=400_000_000)
    out.append(f"R {name} {entry}")
    banks = e.u32(LV + 0x6c)
    for b in range(e.u32(LV + 0x88)):
        PL = e.u32(banks + 4 * b)
        nseq, nimg = u16(e.read(PL + 0x22, 2), 0), u16(e.read(PL + 0x20, 2), 0)
        h = e.read(PL + 0x32, 12)
        out.append(f"B {b}"); out.append(f"H {nseq} {nimg} {u32(h,0)} {u32(h,4)} {u32(h,8)} {e.u32(PL + 0x18)}")
        seqs, pos = e.u32(PL + 0x1c), e.u32(PL + 4)
        for i in range(nseq):
            r = e.read(seqs + 0x24 * i, 0x24); typ = u16(r, 0xa); cnt = u16(r, 0x1c); slot = u32(r, 0x18)
            idx = (slot - e.u32(PL)) // 4 if slot else -1
            ent = scl = b""; fd = 0
            if typ in (2, 4) and cnt: ent = e.read(u32(r, 0x20), 4 * cnt); fd = u16(r, 0x1e)
            if typ == 4 and cnt: scl = e.read(u32(r, 0x14), 2 * cnt)
            out.append(f"S {u32(r,0)} {s16(r,4)} {s16(r,6)} {u16(r,8)} {typ} {u32(r,0xc)} {u32(r,0x10)} {idx} {cnt if typ in (2,4) else 0} {fd} {hx(ent)} {hx(scl)}")
        out.append("P " + (",".join(str(e.u32(pos + 4 * k)) for k in range(nimg)) or "-"))
    print(name, file=sys.stderr)
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/realbanks.txt", "w").write("\n".join(out) + "\n")
print(len(levels), "levels")
