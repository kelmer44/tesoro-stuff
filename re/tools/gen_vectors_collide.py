#!/usr/bin/env python3
"""Vectors for the level's collision pass: builds synthetic levels with the ORIGINAL Level_Load (0x4080e0) and Level_AttachObjects (0x408540),
gives every object a fake vtable whose slots 3 (UpdateBounds), 11 (IsHitter) and 12 (OnTouched) are Python hooks that log their calls,
then runs the ORIGINAL collision pass (0x408890) with ResolveOverlap (0x408c50) and the swept rectangle test (0x409280) in Unicorn.
Objects carry shape entries (convex, shipped and x-monotone non-convex outlines). Output: local-media/vectors/collide.txt (ignored).
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

def hull(pts):
    pts = sorted(set(pts))
    if len(pts) < 3: return None
    def cross(o, a, b): return (a[0]-o[0])*(b[1]-o[1]) - (a[1]-o[1])*(b[0]-o[0])
    lo = []
    for p in pts:
        while len(lo) >= 2 and cross(lo[-2], lo[-1], p) <= 0: lo.pop()
        lo.append(p)
    up = []
    for p in reversed(pts):
        while len(up) >= 2 and cross(up[-2], up[-1], p) <= 0: up.pop()
        up.append(p)
    return lo, up

import os
SHIPPED = []     # the multi-point outlines of the shipped levels (written by drivers/outlinescan_driver.cpp); about a third are not convex
if os.path.exists("local-media/vectors/shipped_outlines.txt"):
    for line in open("local-media/vectors/shipped_outlines.txt"):
        SHIPPED.append([tuple(map(int, t.split(":"))) for t in line.split(":", 1)[1].split()])

def make_shape():
    """An outline as the animation files store it: left-to-right chain, then right-to-left chain (maybe closed); mostly convex, some from the shipped data or x-monotone and not convex."""
    if rnd.random() < .2: return [(rnd.randint(-20, 20), rnd.randint(-20, 20))]       # a single point
    r0 = rnd.random()
    if r0 < .2 and SHIPPED:
        k = rnd.choice([1, 4, 8]); return [(x // k, y // k) for x, y in rnd.choice(SHIPPED)]
    if r0 < .4:
        r = rnd.choice([6, 20, 60]); a = sorted(set(rnd.randint(-r, r) for _ in range(rnd.randint(2, 6))))
        b = sorted(set(rnd.randint(-r, r) for _ in range(rnd.randint(1, 5))), reverse=True)
        if len(a) >= 2: return [(x, rnd.randint(-r // 2, r // 2)) for x in a] + [(x, rnd.randint(-r // 2, r // 2)) for x in b if a[0] <= x <= a[-1]]
    while True:
        r = rnd.choice([6, 20, 60]); h = hull([(rnd.randint(-r, r), rnd.randint(-r, r)) for _ in range(rnd.randint(1, 9))])
        if h:
            lo, up = h
            return lo + up[1:] if rnd.random() < .5 else lo + up[1:-1]

def make_bank2():
    out = bytearray(rb(4))
    nseq = rnd.choice([1, 2, 3])
    out += struct.pack("<HHHHIIIB3s", nseq, rnd.getrandbits(16), 0, rnd.getrandbits(16), rnd.getrandbits(32), rnd.getrandbits(32), rnd.getrandbits(32), 1, b"\0\0\0")
    for _ in range(nseq):
        typ = rnd.choice([0, 1, 2, 4, 4, 4])
        if typ in (2, 4):
            pts = make_shape()
            if typ == 2: extra = struct.pack("<H", len(pts)) + rb(4) + b"".join(struct.pack("<hh", x, y) for x, y in pts)
            else: extra = struct.pack("<H", len(pts)) + rb(6) + b"".join(struct.pack("<hhH", x, y, rnd.getrandbits(16)) for x, y in pts)
        else: extra = rb(rnd.randint(0, 6))
        out += struct.pack("<IHHHhIIHH", rnd.getrandbits(32), rnd.getrandbits(16), rnd.getrandbits(16), rnd.randint(1, 400), -1, rnd.getrandbits(32), rnd.getrandbits(32), typ, len(extra)) + extra
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

from unicorn import UC_HOOK_MEM_READ, UC_HOOK_CODE, UcError
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP
guard = {"on": False, "ok": [], "arrays": [], "stray": False, "shaped": 0}
def poly_ranges(a):
    n, m, ptr = struct.unpack("<HHI", e.read(a, 8)); return [(a, a + 8), (ptr, ptr + 4 * n)], (ptr, ptr + 4 * n)
def enter(nargs):
    def h(uc, addr, size, _):
        esp = uc.reg_read(UC_X86_REG_ESP); ret = e.u32(esp); args = [e.u32(esp + 4 + 4 * k) for k in range(nargs)]
        guard["ok"] = []; guard["on"] = True; guard["shaped"] += 1
        for a in args:
            if HEAP <= a < HEAP + 0x400000:
                r, arr = poly_ranges(a); guard["ok"] += r; guard["arrays"].append(arr)
        def leave(uc2, a2, s2, _2): guard["on"] = False
        hk = uc.hook_add(UC_HOOK_CODE, leave, begin=ret, end=ret)
    return h
def on_read(uc, access, addr, size, value, _):
    if guard["on"] and HEAP <= addr < HEAP + 0x400000 and not any(lo <= addr and addr + size <= hi for lo, hi in guard["ok"]): guard["stray"] = True
for fn, na in ((0x409560, 1), (0x409790, 2), (0x409c10, 1)): e.uc.hook_add(UC_HOOK_CODE, enter(na), begin=fn, end=fn)
e.uc.hook_add(UC_HOOK_MEM_READ, on_read)
class Drop(Exception): pass

def snap(objs):
    s = []
    for a in objs:
        o = e.read(a, 0x70); s.append(f"{s32(u32(o,0x4c))},{s32(u32(o,0x50))},{u32(o,0x68)}")
    mv = len(walk(e.u32(LV + 0x7c))); pr = 0
    return " ".join(s) + f" | mv {mv} t {e.u32(LV + 0x8c)}"

out = []
dropped = 0
for case in range(2500):
    start = len(out); skip = False
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
            if rnd.random() < .4: mx2, my2 = mx1, my1       # no movement
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
        events.clear(); guard["stray"] = False; guard["on"] = False
        try: e.call(0x408890, [], this=LV); crashed = False
        except UcError: crashed = True
        if crashed or guard["stray"] or e.uc.reg_read(UC_X86_REG_EIP) != 0x7ffff000: del out[start:]; skip = True; dropped += 1; break
        out.append(f"R {now}"); out.append("E " + (" ".join(events) or "-")); out.append("= " + snap(objs))
    if not skip: out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/collide.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines;", dropped, "cases dropped (read outside an outline or ran away); outline routine calls:", guard["shaped"])
