#!/usr/bin/env python3
"""Vectors for the board game's flow: runs the ORIGINAL script routines of the conversation CONV33 (0x419530, phases 0 and 1) and
of the story block FCONECTA (0x4193b0, phases 0 and 1) in Unicorn, together with the machine's turn (0x4191c0 and its scoring)
on the game's own globals (state 0x442228, busy 0x442230, move stamp 0x442220, blink stamp 0x442224, blink phase 0x442270,
saved byte 0x44222c, board 0x442278, winning cells 0x442238). Replaced by Python: the clock (0x413a10), the random function
(0x413b00, a fixed formula), the pieces update (0x418e90), Jim's and the pointer's set-active slot, Jim entering a level
(0x407b80), the script table lookup and start (0x4060c0, 0x405fd0); all logged.
Output: local-media/vectors/boardgame.txt (ignored). Consumed by re/tools/drivers/boardgame_driver.cpp.
Lines: K variable2 savedByte board(hex 57) state busy moveStamp blinkStamp
       O <op> <arg> now | busy state moveStamp blinkStamp saved variable2 board winning(8) | log
  ops: S (CONV33 phase 0: the port's BoardGame::start), T (FCONECTA phase 0: turn), D <column> (the player's click, applied to the globals as the
       column button does), F (FCONECTA phase 1) and C (CONV33 phase 1), the last two have no effect on the game itself and only
       record the calls into the rest of the game
       #   end of case
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_ECX

rnd = random.Random(20261105)
M = 0xffffffff
HEAP = 0x2000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"heap": HEAP, "now": 0, "r": 0}
def mk(size):
    a = (st["heap"] + 15) // 16 * 16; st["heap"] = a + size; e.write(a, bytes(size)); return a
def s32(v): return v - (1 << 32) if v & 0x80000000 else v
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
log = []

def rnd_hook(emu, lo, hi):
    st["r"] += 1; v = lo + (st["r"] * 7) % (hi - lo + 1); log.append(f"random {lo} {hi}"); return v
e.hook_function(0x413b00, rnd_hook, 2, pops=True)
e.hook_function(0x413a10, lambda emu: st["now"])
e.hook_function(0x418e90, lambda emu: log.append("update"), 0)

FNS = 0x4000000; e.uc.mem_map(FNS, 0x10000, 7)
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
JVT, CVT = mk(0x100), mk(0x100)
JIM, CUR = mk(0x100), mk(0x100)
e.write(JIM, struct.pack("<I", JVT)); e.write(CUR, struct.pack("<I", CVT))
slot(JVT, 0x20, lambda emu, v: log.append(f"jim {v & 0xff}"), 1)
slot(CVT, 0x20, lambda emu, v: log.append(f"pointer {v & 0xff}"), 1)
e.write(0x4420fc, struct.pack("<I", JIM)); e.write(0x442128, struct.pack("<I", CUR))
L14, L9 = mk(0x100), mk(0x100)
e.write(L14 + 0x70, struct.pack("<I", 0x7000)); e.write(L9 + 0x70, struct.pack("<I", 0x9000))
e.write(0x442168, struct.pack("<I", L14)); e.write(0x442154, struct.pack("<I", L9))
LV = {L14: 14, L9: 9}
e.write(0x4420b8, struct.pack("<I", mk(0x100)))
BLOCK, VARS = mk(0x100), mk(0x100)
e.write(BLOCK + 0xd0, struct.pack("<I", VARS))
def enter(emu, level, rooms, x, y): log.append(f"enter {LV.get(level, -9)} {s32(x)} {s32(y)} {int(rooms == (0x7000 if level == L14 else 0x9000))}"); return 0
e.hook_function(0x407b80, enter, 4, pops=True)
def find_script(emu, lo, hi): log.append(f"findscript {lo} {hi}"); return 0x100500
e.hook_function(0x4060c0, find_script, 2, pops=True)
def start_script(emu, lo, hi, vars_, level): log.append(f"start {lo} {hi} {int(vars_ == VARS)} {LV.get(level, -9)}"); return 3
e.hook_function(0x405fd0, start_script, 4, pops=True)
WORLD_END = st["heap"]

def play_board():
    board = bytearray(57); who = 1 if rnd.random() < .5 else 2
    for _ in range(rnd.choice([0, 3, 8, 14, 20, 30, 40, 48, 49])):
        c = rnd.randrange(7)
        if board[1 + 8 * c] < 7:
            board[2 + 8 * c + board[1 + 8 * c]] = who; board[1 + 8 * c] += 1; board[0] = c; who = 3 - who
    return board

def snapshot():
    g = lambda a, n=1: e.read(a, n)
    win = struct.unpack("<8i", g(0x442238, 32))
    return (f"{g(0x442230)[0]} {g(0x442228)[0]} {s32(e.u32(0x442220))} {s32(e.u32(0x442224))} {g(0x44222c)[0]} {g(VARS + 2)[0]} {g(0x442278, 57).hex()} {' '.join(map(str, win))}")

out = []
for case in range(300):
    st["r"] = 0; del log[:]
    board = play_board(); vars2 = rnd.choice([0, 1, 2, 3, 0xff, rnd.getrandbits(8)]); saved = rnd.getrandbits(8)
    state = rnd.choice([0, 0, 0, 1, 2]); busy = rnd.choice([0, 1]); mstamp = rnd.choice([0, 0, 1000, 2000]); bstamp = rnd.choice([0, 500, 1000])
    now = rnd.randint(1500, 6000)
    e.write(0x442278, bytes(board)); e.write(0x442228, bytes([state])); e.write(0x442230, bytes([busy])); e.write(0x442220, struct.pack("<I", mstamp))
    e.write(0x442224, struct.pack("<I", bstamp)); e.write(0x44222c, bytes([saved])); e.write(VARS + 2, bytes([vars2]))
    e.write(0x442238, bytes(32)); e.write(0x442270, bytes([rnd.choice([0, 1])]))
    e.write(JIM + 0x4c, struct.pack("<ii", rnd.randint(-500, 500), rnd.randint(-500, 500)))
    out.append(f"K {vars2} {saved} {board.hex()} {state} {busy} {mstamp} {bstamp} {e.read(0x442270,1)[0]}")
    for step in range(rnd.randint(4, 14)):
        del log[:]
        r = rnd.random()
        if r < 0.1: op, arg = "S", 0
        elif r < 0.2: op, arg = "F", 0
        elif r < 0.25: op, arg = "C", 0
        elif r < 0.55: op, arg = "D", rnd.randrange(7)
        else: op, arg = "T", 0
        if op == "T":
            now += rnd.choice([0, 10, 500, 1400, 1500, 1501, 1600, 2999, 3000, 3001, 4000])
        st["now"] = now
        if op == "S": e.call(0x419530, [0], conv="thiscall", this=BLOCK)
        elif op == "C": e.call(0x419530, [1], conv="thiscall", this=BLOCK)
        elif op == "T": e.call(0x4193b0, [0], conv="thiscall", this=BLOCK)
        elif op == "F": e.call(0x4193b0, [1], conv="thiscall", this=BLOCK)
        else:
            # the column button's click, with the original code (kind 21 slot 14 is not needed here: its effect is applied directly)
            b = e.read(0x442278, 57)
            if e.read(0x442230, 1)[0] == 0 and b[1 + 8 * arg] != 7:
                bb = bytearray(b); bb[2 + 8 * arg + bb[1 + 8 * arg]] = 1; bb[1 + 8 * arg] += 1; bb[0] = arg
                e.write(0x442278, bytes(bb)); e.write(0x442230, b"\1"); e.write(0x442220, struct.pack("<I", now & M))
        kept = [l for l in log if l == "update"] if op == "S" else list(log)
        out.append(f"O {op} {arg} {now} | {snapshot()} | {','.join(l.replace(' ', '_') for l in kept) or '-'} | world: {','.join(l.replace(' ', '_') for l in log if l != 'update') or '-'}")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/boardgame.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
