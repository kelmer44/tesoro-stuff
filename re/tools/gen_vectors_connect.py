#!/usr/bin/env python3
"""Vectors for the board game's logic (connect four): runs the ORIGINAL line scoring (0x418d20), best-move search (0x419050) and the machine's turn (0x4191c0) in Unicorn
on boards at the game's own address (0x442278), with the random number function (0x413b00) answered by a fixed formula. Boards come from random play, so they contain
wins, near wins and full columns. Each operation records its result, the board, the cells of the last line and the search's score.
Usage: gen_vectors_connect.py (from the repository root). Output: local-media/vectors/connect.txt, read by drivers/connect_driver.cpp.
Lines: B <57 board bytes> | o <op> | r <result> | b <board> | l <14 line ints> | s <score out> | . <log>  #"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
e = Emu()
BOARD, LINE, OUT = 0x442278, 0x442238, 0x442300
rnd = random.Random(20261201)
log = []; world = {"r": 0}
def rnd_hook(emu, lo, hi):
    world["r"] += 1; v = lo + (world["r"] * 7) % (hi - lo + 1); log.append(f"random {lo} {hi}"); return v
e.hook_function(0x413b00, rnd_hook, 2, pops=True)
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
out = []
def snapshot():
    out.append("b " + e.read(BOARD, 57).hex()); out.append("l " + e.read(LINE, 56).hex())
def op(text, addr, args):
    del log[:]; world["r"] = 0
    r = e.call(addr, args, conv="cdecl", this=None)
    out.append("o " + text); out.append("r " + str(i32(r))); snapshot(); out.append("s " + str(i32(e.u32(OUT)))); out.extend(". " + l for l in log)
for case in range(1, 1201):
    board = bytearray(rnd.getrandbits(8) for _ in range(57)); board[0] = rnd.randrange(7)
    counts = [0] * 7; who = 1 if rnd.random() < 0.5 else 2
    for _ in range(rnd.randint(0, 49)):
        free = [c for c in range(7) if counts[c] < 7]
        if not free: break
        c = rnd.choice(free); board[2 + c * 8 + counts[c]] = who; counts[c] += 1; who = 3 - who
    for c in range(7): board[1 + c * 8] = counts[c]
    e.write(BOARD, bytes(board)); e.write(LINE, bytes(64)); e.write(BOARD + 57, bytes(8)); e.write(OUT, bytes(4))
    out.append("B " + bytes(board).hex()); out.append("")
    for step in range(rnd.randint(1, 6)):
        k = rnd.random()
        if k < 0.35: col, w = rnd.randrange(7), rnd.choice([1, 2]); op(f"S {col} {w}", 0x418d20, [BOARD, col, w])
        elif k < 0.6: w = rnd.choice([1, 2]); op(f"F {w}", 0x419050, [BOARD, OUT, w])
        else: col = rnd.randrange(7); op(f"C {col}", 0x4191c0, [BOARD, col])
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/connect.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
