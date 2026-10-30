#!/usr/bin/env python3
"""Vectors for the pointer's IsHitter (0x402e50, vtable 00428038 slot 11): runs the ORIGINAL on a fake pointer and a fake other object with random flags.
Output: local-media/vectors/hitter.txt. Line: <pointer flags> <other flags> = <result>   (the function reads the OTHER object's flag bit 10)"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
rnd = random.Random(20261108); e = Emu(); P, O = 0x100c00, 0x100d00; out = []
for _ in range(2000):
    pf, of = rnd.getrandbits(32), rnd.getrandbits(32)
    e.write(P, bytes(0x80)); e.write(O, bytes(0x80)); e.write(P + 0x68, struct.pack("<I", pf)); e.write(O + 0x68, struct.pack("<I", of))
    out.append(f"{pf} {of} = {e.call(0x402e50, [O], conv='thiscall', this=P) & 0xffffffff}")
os.makedirs("local-media/vectors", exist_ok=True); open("local-media/vectors/hitter.txt", "w").write("\n".join(out) + "\n"); print(len(out), "lines")
