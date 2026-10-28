#!/usr/bin/env python3
"""Differential check of small startup helpers: original machine code (Unicorn) vs independent Python models.
Run: re/.venv/bin/python re/tools/check_startup.py"""
import random, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

def s32(v): return v - (1 << 32) if v & 0x80000000 else v

# --- RandomRange (00413b00), model written from the decompilation, not from the assembly -------------
def random_range_model(tick, lo, hi):
    a = ((tick & 0x200) | ((tick >> 2) & 0x100)) >> 2 | (tick & 0x100)
    a = (a >> 2) | (tick & 0x80)
    a = (a >> 2) | (tick & 0x20)
    a >>= 1
    b = ((tick & 2) | ((tick & 1) << 2)) << 2 | (tick & 4)
    b = (b << 2) | (tick & 8)
    b = ((b << 2) | (tick & 0x10)) << 1
    frac = a | b
    return ((frac * ((hi - lo) + 1)) >> 10) + lo

# --- NameHash64 (00415c40): 5 bits per char, 12 chars, '<' '5' gets +11 -------------------------
def name_hash_model(name):
    s = name.encode("latin1") + b"\0"
    lo = hi = 0
    i = 0
    while i < 6:
        c = s[i]
        if c == 0: return ((lo << 5) << ((i * -5 + 0x19) & 0x1f)) & 0xffffffff
        if c >= 0x80: v = c + 0xb if c - 256 < 0x35 else c    # signed char compare against '5'
        else: v = c + 0xb if c < 0x35 else c
        lo = ((lo << 5) | (v & 0x1f)) & 0xffffffff; i += 1
    while i < 12:
        c = s[i]
        if c == 0:
            hi = ((hi << 5) << ((i * -5 + 0x37) & 0x1f)) & 0xffffffff
            return (hi << 32) | lo
        v = c + 0xb if (c < 0x35 or c >= 0x80) else c
        hi = ((hi << 5) | (v & 0x1f)) & 0xffffffff; i += 1
    return (hi << 32) | lo

def main():
    rnd = random.Random(1)
    e = Emu(); tick = [0]
    e.stub_import(0x44426c, lambda emu: tick[0])      # GetTickCount slot
    bad = 0; n = 0
    for _ in range(3000):
        tick[0] = rnd.getrandbits(32); lo = rnd.randint(-5, 50); hi = lo + rnd.randint(0, 300)
        got = s32(e.call(0x413b00, [lo, hi], conv="stdcall"))
        want = s32(random_range_model(tick[0], lo, hi) & 0xffffffff)
        n += 1
        if got != want: bad += 1; print("RandomRange mismatch", hex(tick[0]), lo, hi, got, want) if bad < 5 else None
    print(f"RandomRange: {n - bad}/{n} match")
    names = ["A", "JIM", "MAPA", "PRINCIPAL", "ABCDEFGHIJKL", "ABCDEFGHIJKLMNOP", "jim", "0123", "ZZ9", "frase_01", "x", ""]
    names += ["".join(chr(rnd.choice(list(range(0x30, 0x5b)) + [0x5f, 0x61, 0x7a])) for _ in range(rnd.randint(1, 14))) for _ in range(200)]
    bad = 0
    for nm in names:
        e.write(0x100000, nm.encode("latin1") + b"\0"); e.call(0x415c40, [0x100000])
        got = e.edx_eax(); want = name_hash_model(nm)
        if got != want: bad += 1; print("NameHash64 mismatch", repr(nm), hex(got), hex(want)) if bad < 5 else None
    print(f"NameHash64: {len(names) - bad}/{len(names)} match")
main()
