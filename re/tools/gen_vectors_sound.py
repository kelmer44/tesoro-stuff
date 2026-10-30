#!/usr/bin/env python3
"""Vectors for the sound device's records: runs the ORIGINAL play (0x404110: takes a record, reads and checks the 44 byte WAV header, gives an id),
find by id (0x4040d0), the stop-and-tidy sequence the speaker uses (find, 0x4043b0, update 0x404210) and update (0x404210, which takes finished sounds out
of the active list) in Unicorn on a fake device with three records. The archive file is a Python function (the header of the WAV at an offset), the
DirectSound buffers are no-ops, loading the first data (0x4042c0) and stopping a record (0x4043b0) log a line, and whether a record still plays
(0x4043e0) is the world's answer. Not covered: the streaming of long sounds and the exact moment a voice ends.
Usage: gen_vectors_sound.py (from the repository root). Output: local-media/vectors/sound.txt (ignored), read by drivers/sound_driver.cpp.
Lines: W <hex wav 0> ... (one line per WAV file, in order; the archive offset of file k is 0x1000 * (k + 1)) | S <session> then o <op> | r <result> | s <state> | . <log> ... | #
Ops: P <wav> | F <id> | T <id> (find, stop, update) | U | M <mask> (bit i = the sound in record i still plays)
State: lastId count order0 order1 order2 id0 id1 id2"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

WORLD, FNS = 0x3100000, 0x7e000000
e = Emu(); e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
DEV, MGR, REC, COM, VT = WORLD, WORLD + 0x1000, [WORLD + 0x2000 + 0x100 * i for i in range(3)], WORLD + 0x3000, WORLD + 0x4000
log = []; world = {"mask": 0, "pos": 0}; loaded = [False] * 3
rnd = random.Random(20261107)

def make_wav(kind):
    n = rnd.randint(0, 3000); data = bytes(rnd.getrandbits(8) for _ in range(n))
    h = bytearray(b"RIFF" + struct.pack("<I", 36 + n) + b"WAVEfmt " + struct.pack("<IHHIIHH", 16, 1, 1, 22050, 22050, 1, 8) + b"data" + struct.pack("<I", n))
    if kind == 1: h[0:4] = b"RIFX"
    elif kind == 2: h[12:16] = b"fmt1"
    elif kind == 3: h[36:40] = b"dat "
    elif kind == 4: struct.pack_into("<H", h, 20, 2)
    return bytes(h) + data
WAVS = [make_wav(rnd.choice([0, 0, 0, 0, 1, 2, 3, 4])) for _ in range(10)]
OFFSETS = [0x1000 * (k + 1) for k in range(len(WAVS))]
def recidx(p): return REC.index(p) if p in REC else -1

def seek(emu, off): world["pos"] = off; return MGR + 4
e.hook_function(0x4032e0, seek, 1, pops=True)
def fread(emu, buf, size, count, fp):
    k = OFFSETS.index(world["pos"]); e.write(buf, WAVS[k][:size * count]); return count
e.hook_function(0x41b4c0, fread, 4)
def strncmp(emu, a, b, n):
    x, y = e.read(a, n), e.read(b, n)
    for p, q in zip(x, y):
        if p != q: return (p - q) & 0xffffffff
        if p == 0: break
    return 0
e.hook_function(0x41be80, strncmp, 3)
e.hook_function(0x403300, lambda emu: world["pos"] + 0x2c, 0)
def load(emu, z):
    i = recidx(e.uc.reg_read(__import__("unicorn.x86_const", fromlist=["x"]).UC_X86_REG_ECX)); loaded[i] = True; log.append(f"load {i}")
e.hook_function(0x4042c0, load, 1, pops=True)
def stop(emu):
    i = recidx(e.uc.reg_read(__import__("unicorn.x86_const", fromlist=["x"]).UC_X86_REG_ECX)); loaded[i] = False; log.append(f"stop {i}")
e.hook_function(0x4043b0, stop, 0)
def playing(emu):
    i = recidx(e.uc.reg_read(__import__("unicorn.x86_const", fromlist=["x"]).UC_X86_REG_ECX)); return ((world["mask"] >> i) & 1) if loaded[i] else 0
e.hook_function(0x4043e0, playing, 0)
for off, nargs in ((0x30, 4), (0x3c, 2), (0x40, 2)):
    e.write(VT + off, struct.pack("<I", FNS + off)); e.hook_function(FNS + off, lambda emu, *a: 0, nargs, pops=True)
e.write(COM, struct.pack("<I", VT)); e.write(0x442064, struct.pack("<I", MGR))

def setup():
    e.write(DEV, bytes(0x40)); e.write(DEV + 4, struct.pack("<I", MGR + 4))
    for i, r in enumerate(REC):
        e.write(r, bytes(0x34)); e.write(r, struct.pack("<I", COM)); e.write(DEV + 8 + 4 * i, struct.pack("<I", r))
    world["mask"] = 0; loaded[:] = [False] * 3
def state():
    order = [recidx(e.u32(DEV + 8 + 4 * i)) for i in range(3)]
    return f"{e.u32(DEV + 0x14)} {e.u32(DEV + 0x18)} " + " ".join(map(str, order)) + " " + " ".join(str(e.u32(r + 0x2c)) for r in REC)

out = ["W " + w.hex() for w in WAVS]
def op(text, fn):
    global _t
    _t = text
    del log[:]
    try: r = fn()
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP
        print('fault eip %x op %s' % (e.uc.reg_read(UC_X86_REG_EIP), text), file=sys.stderr); raise
    out.append("o " + text); out.append("r " + str(r)); out.append("s " + state()); out.extend(". " + l for l in log)
for sid in range(1, 801):
    setup(); out.append(f"S {sid}")
    for _ in range(rnd.randint(4, 30)):
        c = rnd.random()
        if c < 0.3 and e.u32(DEV + 0x18) < 3:
            k = rnd.randrange(len(WAVS)); loaded[recidx(e.u32(DEV + 8 + 4 * e.u32(DEV + 0x18)))] = False; op(f"P {k}", lambda: e.call(0x404110, [OFFSETS[k], 0, 0, 0], conv="thiscall", this=DEV) & 0xffffffff)
        elif c < 0.5:
            i = rnd.randint(0, e.u32(DEV + 0x14) + 1); op(f"F {i}", lambda: int(e.call(0x4040d0, [i], conv="thiscall", this=DEV) != 0))
        elif c < 0.65:
            i = rnd.randint(0, e.u32(DEV + 0x14) + 1)
            def stop():
                r = e.call(0x4040d0, [i], conv="thiscall", this=DEV)
                if r: e.call(0x4043b0, [], conv="thiscall", this=r); e.call(0x404210, [], conv="thiscall", this=DEV)
                return int(r != 0)
            op(f"T {i}", stop)
        elif c < 0.8: op("U", lambda: (e.call(0x404210, [], conv="thiscall", this=DEV), 0)[1])
        else:
            m = rnd.getrandbits(3); world["mask"] = m; out.append(f"o M {m}"); out.append("r 0"); out.append("s " + state())
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/sound.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
