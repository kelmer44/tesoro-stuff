#!/usr/bin/env python3
"""Vectors for the fade to black: runs the ORIGINAL Display_StartEffect (0x4123e0) in Unicorn on a frame filled with a known pattern. The DirectDraw
surface calls (IsLost, GetSurfaceDesc, Lock, Unlock, Blt) are fake COM objects whose methods are Python hooks; GetTickCount advances on every call.
After every Unlock (the end of one fade step) the CRC-32 of the frame is recorded.
Output: local-media/vectors/fade.txt (ignored). Consumed by re/tools/drivers/fade_driver.cpp.
Lines: Z <seed> <steps> <is555> <delay>     F <crc> ... one per step      #"""
import os, random, struct, sys, zlib
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

rnd = random.Random(20261109)
DISP, HEAP, FRAME, VT1, VT2, OBJ1, OBJ2, HK = 0x1200000, 0x2000000, 0x3200000, 0x101a00, 0x101b00, 0x101c00, 0x101d00, 0x6f000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(FRAME, 0xa0000, 3); e.uc.mem_map(HK, 0x1000, 5)
st = {"heap": HEAP}; crcs = []; tick = [0]
def alloc_zero(emu, size, arena):
    a = (st["heap"] + 3) // 4 * 4; st["heap"] = a + size; emu.write(a, bytes(size)); return a
e.hook_function(0x40db40, alloc_zero, 2)
e.stub_import(0x44426c, lambda emu: (tick.__setitem__(0, tick[0] + 30)) or tick[0])
def is_lost(emu, obj, a, b): return 0
def get_desc(emu, obj, desc): return 0
def lock(emu, obj, rect, desc, flags, ev):
    emu.write(desc + 0x10, struct.pack("<I", 1280)); emu.write(desc + 0x24, struct.pack("<I", FRAME)); return 0
def unlock(emu, obj, p): crcs.append(zlib.crc32(emu.read(FRAME, 614400))); return 0
def blt(emu, obj, a, b, src, rect, flags): return 0
for off, fn, n, addr in ((0x2c, is_lost, 3, HK), (0x58, get_desc, 2, HK + 0x10), (0x64, lock, 5, HK + 0x20), (0x80, unlock, 2, HK + 0x30)):
    e.hook_function(addr, fn, n, pops=True); e.write(VT2 + off, struct.pack("<I", addr))
e.hook_function(HK + 0x40, blt, 6, pops=True); e.write(VT1 + 0x1c, struct.pack("<I", HK + 0x40))
e.write(OBJ1, struct.pack("<I", VT1)); e.write(OBJ2, struct.pack("<I", VT2))
e.write(DISP + 8, struct.pack("<I", OBJ1)); e.write(DISP + 0xc, struct.pack("<I", OBJ2))

def pattern(seed, n):
    x, out = seed, bytearray()
    for _ in range(n):
        x = (x * 1103515245 + 12345) & 0xffffffff; out.append((x >> 16) & 0xff)
    return bytes(out)

out = []
for case in range(16):      # each fade step is about five million emulated instructions
    seed = rnd.getrandbits(31); steps = rnd.choice([1, 2, 3, 4, 4, 5, 7, 8]); is555 = rnd.choice([0, 1]); delay = rnd.choice([0, 100])
    st["heap"] = HEAP; crcs.clear(); tick[0] = rnd.getrandbits(20)
    e.write(FRAME, pattern(seed, 614400)); e.write(DISP + 0x66a, b"\x01"); e.write(DISP + 0x678, struct.pack("<I", FRAME)); e.write(DISP + 0x67c, bytes([is555]))
    e.call(0x4123e0, [steps, delay], conv="thiscall", this=DISP, max_insns=500_000_000)
    out.append(f"Z {seed} {steps} {is555} {delay}"); out.append("F " + " ".join(map(str, crcs))); out.append("#")
os.makedirs("local-media/vectors", exist_ok=True); open("local-media/vectors/fade.txt", "w").write("\n".join(out) + "\n"); print(len(out), "lines")
