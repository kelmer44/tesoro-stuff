#!/usr/bin/env python3
"""Vectors for the level's collision pass: builds synthetic levels with the ORIGINAL Level_Load (0x4080e0) and Level_AttachObjects (0x408540),
gives every object a fake vtable whose slots 3 (UpdateBounds), 11 (IsHitter) and 12 (OnTouched) are Python hooks that log their calls,
then runs the ORIGINAL collision pass (0x408890) with ResolveOverlap (0x408c50) and the swept rectangle test (0x409280) in Unicorn.
Objects have no shape entries (the pixel-accurate part is not covered). Output: local-media/vectors/collide.txt (ignored).
Consumed by re/tools/drivers/collide_driver.cpp.
Lines: C <hex> <entry> <mode> <is555> <now>   level file
       F <i> <flags>      G <i> <hit x1 y1 x2 y2> <moved x1 y1 x2 y2> <x y> <prevx prevy> <sort>     object state
       H <n*n bits>       isHitter(i, j) table        P <i|-1> player object        A attach     M <i> ObjectMoved
       N <i> <x> <y> <setCurrent>   Obj_SetPosition (0x401d30), then = <state>
       R <now>            run the pass, then  E <events>  and  = <state>
       #                  end of case
Events: B<i> UpdateBounds of object i; T<i>:<j>:<time> object i told that it touches j.
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_level.py").read()
exec(src[src.index("rnd = random.Random"):src.index("def make_bank")].replace("20261104", "20261106"))
exec(src[src.index("def walk"):src.index("def state")])

def make_bank2():
    out = bytearray(rb(4))
    nseq = rnd.choice([1, 2, 3])
    out += struct.pack("<HHHHIIIB3s", nseq, rnd.getrandbits(16), 0, rnd.getrandbits(16), rnd.getrandbits(32), rnd.getrandbits(32), rnd.getrandbits(32), 1, b"\0\0\0")
    for _ in range(nseq):
        extra = rb(rnd.randint(0, 6))
        out += struct.pack("<IHHHhIIHH", rnd.getrandbits(32), rnd.getrandbits(16), rnd.getrandbits(16), rnd.randint(1, 400), -1, rnd.getrandbits(32), rnd.getrandbits(32), rnd.choice([0, 1]), len(extra)) + extra
    return bytes(out)

def make_level2(entry):
    nbank = rnd.choice([1, 2]); nroom = rnd.choice([1, 1, 2, 3])
    banks = [make_bank2() for _ in range(nbank)]; rooms = []; total = 0
    for r in range(nroom):
        nobj = rnd.randint(1, 6); total += nobj
        rec = bytearray(rb(0x28)); struct.pack_into("<HHI", rec, 0x20, nobj, rnd.randint(0, 60), rnd.randint(1, 900))
        objs = bytearray()
        for k in range(nobj):
            nb = rnd.randint(1, 2); name = f"O{r}_{k}"; extra = rb(rnd.randint(0, 6))
            objs += struct.pack("<iihBB12sII", rnd.randint(-700, 700), rnd.randint(-500, 500), rnd.randint(-40, 40), rnd.getrandbits(8), nb,
                                name.encode(), 0, len(extra)) + extra + bytes(rnd.randint(0, nbank - 1) for _ in range(nb))
        rooms.append(bytes(rec) + bytes(objs))
    return rb(entry + 4) + struct.pack("<III", nroom, nbank, total) + b"".join(banks) + b"".join(rooms) + rb(rnd.randint(0, 9))

VT, H3, H11, H12 = 0x101a00, 0x6f000000, 0x6f000010, 0x6f000020
e.uc.mem_map(0x6f000000, 0x1000, 5)
events, hit = [], {"table": {}, "objs": []}
def idx(a): return hit["objs"].index(a) if a in hit["objs"] else -1
def on_bounds(emu): events.append(f"B{idx(emu.uc.reg_read(UC_X86_REG_ECX))}")
def on_hitter(emu, other): return int(hit["table"].get((idx(emu.uc.reg_read(UC_X86_REG_ECX)), idx(other)), 0))
def on_touched(emu, other, extra): events.append(f"T{idx(emu.uc.reg_read(UC_X86_REG_ECX))}:{idx(other)}:{s32(extra)}"); return 0
e.hook_function(H3, on_bounds, 0); e.hook_function(H11, on_hitter, 1, pops=True); e.hook_function(H12, on_touched, 2, pops=True)
e.write(VT, e.read(0x428000, 14 * 4)); e.write(VT + 12, struct.pack("<I", H3)); e.write(VT + 44, struct.pack("<I", H11)); e.write(VT + 48, struct.pack("<I", H12))

def snap(objs):
    s = []
    for a in objs:
        o = e.read(a, 0x70); s.append(f"{s32(u32(o,0x4c))},{s32(u32(o,0x50))},{u32(o,0x68)}")
    mv = len(walk(e.u32(LV + 0x7c))); pr = 0
    return " ".join(s) + f" | mv {mv} t {e.u32(LV + 0x8c)}"

out = []
for case in range(300):
    entry = rnd.randint(0, 40); now = rnd.choice([0, 1000, 123456])
    e.write(HEAP, bytes(max(0, st["heap"] - HEAP)))
    st["data"] = make_level2(entry); st["pos"] = 0; st["heap"] = HEAP; st["now"] = now
    e.write(DISP + 0x67c, b"\0"); e.write(LV, bytes([0xcd]) * 0x20); e.write(LV + 0x20, bytes(0x100))
    e.call(0x4080e0, [entry, 0], conv="thiscall", this=LV)
    out.append(f"C {st['data'].hex()} {entry} 0 0 {now}")
    objs = objects(); n = len(objs); hit["objs"] = objs
    for i in range(n):
        f = 0x100 | (rnd.choice([0, 2, 2, 2]) ) | rnd.choice([0, 8, 8, 8]) | rnd.choice([0, 0, 0, 1]) | 0x10
        e.write(objs[i] + 0x68, struct.pack("<I", f)); out.append(f"F {i} {f}")
        e.write(objs[i], struct.pack("<I", VT))
    e.call(0x408540, [], this=LV); out.append("A")
    for p in range(rnd.randint(1, 3)):
        for i in range(n):
            span = rnd.choice([5, 20, 60])
            x1, y1 = rnd.randint(-span, 0), rnd.randint(0, span); x2, y2 = x1 + rnd.randint(1, 2 * span), y1 - rnd.randint(1, 2 * span)
            mx1, my1 = rnd.randint(-span, span), rnd.randint(-span, span); mx2, my2 = mx1 + rnd.randint(-span // 2 - 1, span // 2 + 1), my1 + rnd.randint(-span // 2 - 1, span // 2 + 1)
            px, py = rnd.randint(-span, span), rnd.randint(-span, span); ppx, ppy = rnd.randint(-span, span), rnd.randint(-span, span); sort = rnd.randint(-5, 400)
            if rnd.random() < 0.15: sort = rnd.choice([32767, -32768, -1, 1, 100])
            o = objs[i]
            e.write(o + 0x10, struct.pack("<iiii", x1, y1, x2, y2)); e.write(o + 0x20, struct.pack("<iiii", mx1, my1, mx2, my2))
            e.write(o + 0x4c, struct.pack("<ii", px, py)); e.write(o + 0x54, struct.pack("<ii", ppx, ppy)); e.write(o + 0x5e, struct.pack("<h", sort))
            out.append(f"G {i} {x1} {y1} {x2} {y2} {mx1} {my1} {mx2} {my2} {px} {py} {ppx} {ppy} {sort}")
        hit["table"] = {(i, j): rnd.random() < 0.5 for i in range(n) for j in range(n)}
        out.append("H " + "".join("1" if hit["table"][(i, j)] else "0" for i in range(n) for j in range(n)))
        pl = rnd.choice([-1, -1] + list(range(n))); e.write(0x4420fc, struct.pack("<I", objs[pl] if pl >= 0 else 0)); out.append(f"P {pl}")
        for _ in range(rnd.randint(0, 2 * n)):
            i = rnd.randrange(n); e.call(0x408440, [objs[i]], conv="thiscall", this=LV); out.append(f"M {i}")
        for _ in range(rnd.randint(0, 3)):
            i = rnd.randrange(n); nx, ny, sc = rnd.randint(-50, 50), rnd.randint(-50, 50), rnd.choice([0, 1])
            e.call(0x401d30, [nx & 0xffffffff, ny & 0xffffffff, sc], conv="thiscall", this=objs[i]); out.append(f"N {i} {nx} {ny} {sc}"); out.append("= " + snap(objs))
        now += rnd.choice([0, 17, 500]); st["now"] = now
        events.clear(); e.call(0x408890, [], this=LV)
        out.append(f"R {now}"); out.append("E " + (" ".join(events) or "-")); out.append("= " + snap(objs))
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/collide.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
