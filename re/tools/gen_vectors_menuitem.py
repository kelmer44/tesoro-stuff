#!/usr/bin/env python3
"""Vectors for object kind 25 (menu item): runs the ORIGINAL constructor (0x417ad0, with its base 0x40ecb0), the tick-and-clear step (0x407770,
vtable slot 2) and the draw method (0x40efa0, vtable slot 6) in Unicorn. The display's image blit (0x411f80) is replaced by a
function that records its arguments. Banks are synthetic animation files read by the original loader (0x401160).
Output: local-media/vectors/menuitem.txt (ignored). Consumed by re/tools/drivers/menuitem_driver.cpp.
Lines: C <hex dat> <entry> <mode> <is555> | eK K x y sort roomSort name nb N t0 idxs data | state
       eT t | state    eM mx my (set the current player's movement)    eX camX camY x y sort | n x y depth imageIndex extra
       #  end of case
state: flags vtableOk action, then per player flags start total current first last loopX loopY, then x y of every sequence
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261031)
M = 0xffffffff
PL, ARCH, FP, DISP, HEAP = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])           # CRT / pool / clock hooks as in the animation vectors
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def rebase")])                     # helpers, make_blob, make_file

calls = []
e.hook_function(0x411f80, lambda emu, x, y, d, img, extra: calls.append((x, y, d, img, extra)), 5, pops=True)

OBJ, ARR, REC, ROOM, IDX, TP, CAM = 0x101200, 0x101400, 0x101440, 0x101480, 0x1014c0, 0x101500, 0x101a00
out = []
for case in range(600):
    while True:
        dat, sizes = make_file(0, False, True)
        if u16(dat, 0) > 0: break
    entry = rnd.randint(0, 40); is555 = False
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0; st["heap"] = HEAP
    e.write(DISP + 0x67c, b"\0"); e.write(PL, bytes(0x40))      # the pool hands out zeroed memory
    e.call(0x401160, [entry, 0, 0], conv="thiscall", this=PL)
    out.append(f"C {st['data'].hex()} {entry} 0 0")
    nseq = u16(e.read(PL + 0x22, 2), 0); seqs, imgs = e.u32(PL + 0x1c), e.u32(PL)
    K = rnd.randint(1, 3); T = [TP + 0x40 * k for k in range(K)]
    for k in range(K):
        e.write(T[k], bytes([0xcd]) * 0x40); e.call(0x401580, [PL], this=T[k]); e.write(ARR + 4 * k, struct.pack("<I", T[k]))
    while True:
        nb = rnd.randint(1, 3); idxs = [rnd.randint(0, K - 1) for _ in range(nb)]
        N = rnd.choice([0, 1, 2, 3, 3, 3, 6]); data = bytes([rnd.randint(0, nseq - 1) if rnd.random() < 0.9 else rnd.randint(0, 255), rnd.getrandbits(8), rnd.choice([0, 1, 2, 3, 4, 5, 200])]) + bytes(rnd.getrandbits(8) for _ in range(3))
        data = data[:N]
        eff = list((data + bytes(idxs))[:3])
        if N + nb >= 3 and eff[0] < nseq: break
    t0 = rnd.choice([0, 1000, 123456]); st["now"] = t0
    ox, oy, so, ro = rnd.randint(-70000, 70000), rnd.randint(-70000, 70000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    name = "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") for _ in range(rnd.randint(1, 12)))
    e.write(IDX, bytes(rnd.getrandbits(8) for _ in range(32))); e.write(IDX, data + bytes(idxs))
    e.write(REC, bytes(0x20)); e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, bytes([nb]))
    e.write(REC + 0xc, name.encode() + b"\0"); e.write(REC + 0x1c, struct.pack("<I", N))
    e.write(ROOM, bytes(0x10)); e.write(ROOM + 8, struct.pack("<h", ro))
    e.write(OBJ, bytes(0x100))
    e.call(0x417ad0, [REC, ROOM, ARR, IDX], conv="thiscall", this=OBJ)

    def state():
        arr = e.u32(OBJ + 0x34); n = e.read(OBJ + 0x33, 1)[0]
        s = f"{e.u32(OBJ + 0x68)} {int(e.u32(OBJ) == 0x42c000)} {e.u32(OBJ + 0xa0)}"
        for i in range(n):
            p = e.read(e.u32(arr + 4 * i), 0x40)
            s += f" {u32(p,0x18)} {s32(u32(p,8))} {u32(p,0xc)} {u16(p,0x30)} {u16(p,0x24)} {u16(p,0x26)} {u16(p,0x2c)} {u16(p,0x2e)}"
        for i in range(nseq):
            r = e.read(seqs + 0x24 * i, 8); s += f" {s16(r,4)} {s16(r,6)}"
        return s
    out.append(f"eK {K} {ox} {oy} {so} {ro} {name} {nb} {N} {t0} {','.join(map(str, idxs))} {data.hex() or '-'} | {state()}")
    t = t0; moved = False
    for _ in range(rnd.randint(2, 10)):
        op = rnd.choice("TTXXXM")
        cur = e.u32(OBJ + 0x38)
        if op == "T":
            t += rnd.choice([0, 1, 17, 100, 333, 1500, 5000]); st["now"] = t
            e.call(0x407770, [], this=OBJ); moved = True; out.append(f"eT {t} | {state()}")
        elif op == "M":
            mx, my = rnd.randint(-300, 300), rnd.randint(-300, 300); e.write(cur + 0x28, struct.pack("<hh", mx, my)); moved = True; out.append(f"eM {mx} {my}")
        elif not moved or u16(e.read(cur + 0x30, 2), 0) >= nseq: continue     # sequence never selected: the original reads a garbage record
        else:
            cx, cy, x, y, sort = (rnd.randint(-2000, 2000) for _ in range(5)); sort = rnd.randint(-300, 300)
            if rnd.random() < 0.3: x, y = rnd.randint(-70000, 70000), rnd.randint(-70000, 70000)
            if rnd.random() < 0.4:      # put the left edge right at the screen's limit
                rcur = e.u32(cur + 0x1c) + 0x24 * u16(e.read(cur + 0x30, 2), 0)
                mvx, imx = s16(e.read(cur + 0x28, 2), 0), s16(e.read(rcur, 2), 0)
                cx = x + mvx + 320 + imx - rnd.choice([0x27f, 0x280, 0x281, 0x282])
            e.write(CAM + 0x34, struct.pack("<ii", cx, cy)); e.write(OBJ + 0x4c, struct.pack("<ii", x, y)); e.write(OBJ + 0x5e, struct.pack("<h", sort))
            seq = u16(e.read(cur + 0x30, 2), 0); slot = e.u32(seqs + 0x24 * seq + 0x18); idx = (slot - imgs) // 4 if slot else -1
            calls.clear(); e.call(0x40efa0, [CAM], conv="thiscall", this=OBJ)
            assert len(calls) <= 1
            if calls:
                cx_, cy_, d, img, ex = calls[0]; assert img == e.u32(slot)
                out.append(f"eX {cx} {cy} {x} {y} {sort} | 1 {s32(cx_)} {s32(cy_)} {d & 0xffff} {idx} {ex}")
            else: out.append(f"eX {cx} {cy} {x} {y} {sort} | 0")
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/menuitem.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
