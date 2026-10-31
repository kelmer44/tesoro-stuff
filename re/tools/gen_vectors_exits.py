#!/usr/bin/env python3
"""Vectors for the exits (level kind 6, 10 subtypes and the plain one): the ORIGINAL constructor (0x414be0) with the factory's state size and vtable, then the verbs and
the right click through the object's own vtable, the arrival, and the exit's slot 28. Hooks log what the exit does to the rest of the game: object lookups, the changing of
levels step by step, Jim's face and scene entry, the script starts (with their level), the movie, Jim's bytes and position, and the objects at the other end.
Output: local-media/vectors/exits.txt, read by drivers/exits_driver.cpp.
Usage: gen_vectors_exits.py (from the repository root).
"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_EIP

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261113)
PL, ARCH, FP, DISP, HEAP = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def rebase")])

WORLD, FNS = 0x3200000, 0x7e000000
e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
OBJ, JIMO, CUR, HELD, VTJ, CVT, LIST, TP, ARR, REC, ROOM, IDX, CAM, TXT, KEYS, LINEP, PH, LEVEL, KBD, STATE, MOUSE, BTNS, SND = [WORLD + 0x1000 * i for i in range(23)]
log = []; world = dict(now=0, ids=0, rcalls=0, active=0)
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
def cstr(a, limit=64):
    b = bytearray()
    while len(b) < limit:
        c = e.read(a, 1)
        if c == b"\0": break
        b += c; a += 1
    return bytes(b).decode("latin-1")
phr = {}      # name -> record address
YWAV = [0, 0, 1007, 5003, 1000]   # the voices of the lines Y0..Y4
def phrase_record(name):
    if name not in phr:
        a = PH + 0x40 * (len(phr) + 1); phr[name] = a
        t = TXT + 0x40 * len(phr); e.write(t, ("T:" + name).encode() + b"\0")
        e.write(a, bytes(0x40)); e.write(a + 0xc, struct.pack("<I", t))
        if name[0] == "Y" and name[1:].isdigit(): e.write(a + 8, struct.pack("<I", YWAV[int(name[1:])]))
    return phr[name]
def pname(addr):
    for k, v in phr.items():
        if v == addr: return k
    return "?" if addr else "null"
def find_phrase(emu, namep):
    n = cstr(namep, 12); log.append(f"phrase {n}"); return phrase_record(n)
e.hook_function(0x413940, find_phrase, 1)
def tname(p): return "obj" if p == OBJ else "none" if p == 0 else "?"
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
def cond_hook(addr, pred, fn, nargs, pops):
    """Like hook_function, but only when pred() holds: otherwise the original code runs."""
    def h(uc, a, size, _):
        if not pred(): return
        import struct as s
        esp = uc.reg_read(UC_X86_REG_ESP); ret = s.unpack("<I", uc.mem_read(esp, 4))[0]
        args = [s.unpack("<I", uc.mem_read(esp + 4 + 4 * k, 4))[0] for k in range(nargs)]
        uc.reg_write(UC_X86_REG_EAX, (fn(e, *args) or 0) & 0xffffffff)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + (4 * nargs if pops else 0)); uc.reg_write(UC_X86_REG_EIP, ret)
    e.uc.hook_add(UC_HOOK_CODE, h, begin=addr, end=addr)
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
# Jim answers (verb replies) run through the same say; the character's own say runs for real.
cond_hook(0x410330, lambda: ecx() == JIMO, lambda emu, line, col, flag: log.append(f"say {pname(line)} {col & 0xffff} {flag}"), 3, True)
e.hook_function(0x40fb20, lambda emu, o: log.append("remove"), 1, pops=True)
e.hook_function(0x40f900, lambda emu, o, p: log.append(f"insert {p}"), 2, pops=True)
cond_hook(0x401de0, lambda: ecx() == CUR, lambda emu, a, b, c, d, f: log.append("drop"), 5, True)
def hover(emu, text, col): log.append("hover " + ("null" if text == 0 else cstr(text)));
e.hook_function(0x4023f0, hover, 2, pops=True)
def play(emu, off, a, b, c): world["ids"] += 1; log.append(f"play {off}"); return world["ids"]
e.hook_function(0x404110, play, 4, pops=True)
e.hook_function(0x4040d0, lambda emu, i: (log.append(f"find {i}"), REC if (world["active"] >> i) & 1 else 0)[1], 1, pops=True)
e.hook_function(0x4043b0, lambda emu: log.append("stop"), 0)
e.hook_function(0x404210, lambda emu: log.append("reap"), 0)
def rnd_hook(emu, lo, hi):
    world["rcalls"] += 1; v = lo + (world["rcalls"] * 7) % (hi - lo + 1); log.append(f"random {lo} {hi}"); return v
e.hook_function(0x413b00, rnd_hook, 2, pops=True)
e.hook_function(0x411c80, lambda emu, t, col, font, x, y, w, tag, cen, lh: log.append(f"text {cstr(t)} {col & 0xffff} {i32(font)} {i32(x)} {i32(y)} {i32(w)} {tag & 0xffff} {cen & 0xff} {i32(lh)}"), 9, pops=True)
e.hook_function(0x401fc0, lambda emu, d: log.append(f"toward {i32(d)}"), 1, pops=True)
e.hook_function(0x411f80, lambda emu, x, y, d, img, extra: log.append(f"queue {i32(x)} {i32(y)} {d & 0xffff}"), 5, pops=True)
LV = {}      # level pointer -> index (-1: the interface level)
LVP = {}
for i in range(47): e.write(0x442130 + 4 * i, struct.pack("<I", WORLD + 0x40000 + 0x100 * i)); LV[WORLD + 0x40000 + 0x100 * i] = i; LVP[i] = WORLD + 0x40000 + 0x100 * i
e.write(0x4421ec, struct.pack("<I", WORLD + 0x50000)); LV[WORLD + 0x50000] = -1
for i in range(47): e.write(LVP[i] + 0x98, struct.pack("<I", i))
e.write(0x4420b8, struct.pack("<I", WORLD + 0x51000))
FO = [WORLD + 0x52000 + 0x100 * k for k in range(8)]; FB = [WORLD + 0x53000 + 0x100 * k for k in range(8)]
FOVT = WORLD + 0x55000
e.write(FOVT, e.read(0x428000, 4 * 33))
slot(FOVT, 0x50, lambda emu: log.append(f"verb0 {[k for k in range(8) if FO[k] == ecx()][0]}"), 0)
e.write(FOVT + 0x70, struct.pack("<I", 0))
slot(FOVT, 0x70, lambda emu: (log.append(f"arrive {[k for k in range(8) if FO[k] == ecx()][0]}"), 0)[1], 0)
for k in range(8): e.write(FO[k] + 0x60, struct.pack("<I", FB[k])); e.write(FO[k], struct.pack("<I", FOVT))
salt = [0]
def pick(idx, lo, hi): return (lo * 3 + hi * 5 + (idx & 0xffffffff) * 7 + salt[0]) & 7
def find_obj(emu, lo, hi):
    idx = LV[ecx()]; log.append(f"find {idx} {lo} {hi}"); return FO[pick(idx, lo, hi)]
e.hook_function(0x408780, find_obj, 2, pops=True)
def find_script(emu, lo, hi): log.append(f"findscript {lo} {hi}"); return WORLD + 0x54000
e.hook_function(0x4060c0, find_script, 2, pops=True)
def vname(p):
    if p == 0: return "none"
    if p == e.u32(OBJ + 0x60): return "own"
    for k in range(8):
        if p == FB[k]: return f"o{k}"
        if p == FB[k] + 1: return f"o{k}+1"
    if 0x100000 <= p < 0x200000: return f"tmp{e.read(p, 1)[0]}"
    return "?"
def start_script(emu, lo, hi, vars_, level): log.append(f"start {lo} {hi} {vname(vars_)} L{LV.get(level, -9)}"); return 3
e.hook_function(0x405fd0, start_script, 4, pops=True)
e.hook_function(0x411f20, lambda emu: log.append("reset"), 0)
e.hook_function(0x408550, lambda emu: log.append(f"cameras {LV.get(ecx(), -9)}"), 0)
e.hook_function(0x412e10, lambda emu: log.append("compose"), 0)
e.hook_function(0x412bb0, lambda emu: log.append("clear"), 0)
e.hook_function(0x4123e0, lambda emu, a, b: log.append(f"fade {a} {b}"), 2, pops=True)
e.hook_function(0x4139e0, lambda emu, n: log.append(f"wait {n}"), 1, pops=True)
K1, FAO, FBO = WORLD + 0x58000, WORLD + 0x59000, WORLD + 0x5a000
e.write(K1 + 0x78, struct.pack("<I", FAO)); e.write(FAO + 0x78, struct.pack("<I", FBO))
for i in range(47): e.write(LVP[i] + 0x90, struct.pack("<I", K1))
cond_hook(0x401de0, lambda: ecx() in (FAO, FBO), lambda emu, a, b, c, d, f: log.append(f"frame {'levelA' if ecx() == FAO else 'levelB'} {b & 0xffff} {i32(c)}"), 5, True)
e.hook_function(0x4132b0, lambda emu, n: log.append(f"movie {n}"), 1, pops=True)
def rnd2(emu, lo, hi): world["rcalls"] += 1; v = lo + (world["rcalls"] * 5) % (hi - lo + 1); log.append(f"random {lo} {hi}"); return v
e.hook_function(0x413ac0, rnd2, 2, pops=True)
e.hook_function(0x407b80, lambda emu, lv, room, x, y: log.append(f"enter {LV.get(lv, -9)} {i32(x)} {i32(y)}"), 4, pops=True)
def cur_slot8(emu, on): log.append(f"pointer {on & 0xff}")
vt0 = e.read(0x42bad8, 4 * 28)
e.write(VTJ, vt0 + bytes(4 * 8))
slot(VTJ, 0x70, lambda emu: log.append("stoptalk"), 0)
slot(VTJ, 0x74, lambda emu, f: log.append(f"face {i32(f)}"), 1)
slot(VTJ, 0x80, lambda emu, x, y, t, d: log.append(f"walk {i32(x)} {i32(y)} {tname(t)} {d}"), 4)
slot(VTJ, 0x10, lambda emu: log.append("jimupdate"), 0)
slot(CVT, 0x20, cur_slot8, 1)
e.write(JIMO, struct.pack("<I", VTJ)); e.write(CUR, struct.pack("<I", CVT))
for g, v in ((0x442128, CUR), (0x442104, LIST), (0x4420fc, JIMO), (0x442074, KBD), (0x44206c, MOUSE), (0x442080, SND), (0x442120, WORLD + 0x30000)): e.write(g, struct.pack("<I", v))
e.write(KBD + 0x34, struct.pack("<I", STATE)); e.write(MOUSE + 0x1c, struct.pack("<I", BTNS))
e.write(HELD + 0x68, struct.pack("<I", 0))
RP = [f"RP{k}" for k in range(8)]
for n in RP: phrase_record(n)
for k in range(5): phrase_record(f"Y{k}")

import json
KINDS = json.load(open("re/object_kinds.json"))
SSZ = {1: 1, 2: 1, 3: 2, 4: 1, 5: 1, 6: 0, 7: 1, 8: 1, 9: 0, 10: 1}
EXITS = {}
for sb, bv in KINDS["6"]["sub"].items():
    if bv.get("vtable"): EXITS[int(sb)] = (int(bv["vtable"], 16), SSZ[int(sb)])
for plain in (0, 11, 12): EXITS[plain] = (0x42bce8, 0)
def tame(dat):
    b = bytearray(dat); p = 0x18
    for _ in range(u16(b, 0)):
        for off in (0, 2, 4, 6, 0xc, 0xe, 0x10, 0x12): struct.pack_into("<h", b, p + off, rnd.randint(-40, 40))
        p += 0x18 + u16(b, p + 0x16)
    return bytes(b)
NAMES = ["", "", "OBJ1", "OBJ2", "OBJ125T03", "A", "ABCDEFGHIJKL", "ZZZ99", "OBJ106T05", "COSA"]
TAGS = [b"MIR", b"HAB", b"HAB", b"HAB", b"XYZ", b"MIX", b"HAC", b"HA\0", b"mir", b"\0\0\0"]
def pad12(n): return n.encode()[:12].ljust(12, b"\0")
heldset = {"on": False}
cur = [0]
def state():
    r = lambda off: e.u32(OBJ + off)
    s = [r(0x68), e.read(OBJ + 0x8c, 1)[0], e.read(OBJ + 0x8d, 1)[0], i32(r(0x90)), i32(r(0x94)), i32(r(0x98)), i32(r(0x9c)), pname(r(0x6c))]
    s += [pname(r(0x70 + 4 * k)) for k in range(7)]
    s += [u16(e.read(OBJ + 0x30, 2), 0), e.u32(HELD + 0x68) & 4 if heldset["on"] else -1, "held" if e.u32(CUR + 0x12c) else "none"]
    p = e.read(e.u32(OBJ + 0x38), 0x40)
    s += [u32(p, 0x18), s32(u32(p, 8)), u32(p, 0xc), u16(p, 0x30), u16(p, 0x24), u16(p, 0x26), u16(p, 0x2c), u16(p, 0x2e), u16(p, 0x28), u16(p, 0x2a)]
    s += [r(4), r(8), LV.get(r(0x40), -9) if r(0x40) else -2, e.read(JIMO + 0x110, 4).hex(), i32(e.u32(JIMO + 0x4c)), i32(e.u32(JIMO + 0x50))]
    s += [e.read(OBJ + 0xa0, 1)[0], e.u32(OBJ + 0xa4), e.u32(OBJ + 0xa8), r(0xac)]
    ss = EXITS.get(cur[0], (0, 0))[1]
    s.append(e.read(e.u32(OBJ + 0x60), ss).hex() if ss else "-")
    for k in range(8): s += [e.u32(FO[k] + 0x68), e.read(FB[k], 16).hex()]
    return " ".join(map(str, s))
out = []
def op(text, addr, args, ret=True):
    del log[:]
    try: r = e.call(addr, args, conv="thiscall", this=OBJ, max_insns=3_000_000)
    except Exception as ex:
        print("FAULT", text, hex(addr), "eip", hex(e.uc.reg_read(UC_X86_REG_EIP)), "log", log, "sid", sid, file=sys.stderr); raise
    out.append("o " + text); out.append("r " + (str(r & 0xffffffff) if ret else "-")); out.append("s " + state()); out.extend(". " + l for l in log)
def apply_world(w):
    st["now"] = w["now"]
    for k, (fl, bl, bk, ax, ay) in enumerate(w["fo"]): e.write(FO[k] + 0x68, struct.pack("<I", fl)); e.write(FB[k], bl); e.write(FO[k] + 0x32, bytes([bk])); e.write(FO[k] + 0x94, struct.pack("<ii", ax, ay))
    e.write(JIMO + 0xd4, bytes([w["held"]])); e.write(JIMO + 0x10c, struct.pack("<I", w["facing"] & 0xffffffff))
    e.write(JIMO + 0x110, bytes([w["jphase"], w["jstep"], 0x77, w["jenter"]])); e.write(JIMO + 0x40, struct.pack("<I", LVP[w["jlevel"]]))
    if w["item"]:
        heldset["on"] = True
        e.write(CUR + 0x12c, struct.pack("<I", HELD)); e.write(HELD + 0x68, struct.pack("<I", 0))
        key = struct.unpack("<II", e.read(OBJ + 0x44, 8))
        n = len(w["replies"]); keys = b""; lines = b""
        for m, k in w["replies"]:
            keys += struct.pack("<II", *(key if m else (key[0] ^ 0x5555, key[1] ^ 3))); lines += struct.pack("<I", phr[RP[k]])
        e.write(KEYS, keys); e.write(LINEP, lines)
        e.write(HELD + 0xa0, struct.pack("<II", KEYS, LINEP)); e.write(HELD + 0xa8, struct.pack("<H", n))
    else:
        heldset["on"] = False; e.write(CUR + 0x12c, struct.pack("<I", 0))
def randomize_objects(w):
    w["fo"] = [(rnd.choice([0, 2, 4, 6, 0x400, 0x402]), bytes(rnd.choice([0, 0, 1, 2, 3, 4, 5, 6, rnd.getrandbits(8)]) for _ in range(16)), rnd.choice([0, 1, 2, 2, 3]), rnd.randint(-900, 900), rnd.randint(-900, 900)) for _ in range(8)]
def wline(w):
    return f"o E {w['now']} {w['held']} {w['facing']} {w['item']} " + (",".join(f"{int(m)}:{k}" for m, k in w["replies"]) or "-") + f" {w['jphase']} {w['jstep']} {w['jenter']} {w['jlevel']} " + ",".join(f"{fl}:{bl.hex()}:{bk}:{ax}:{ay}" for fl, bl, bk, ax, ay in w["fo"])

for sid in range(1, 1301):
    st["heap"] = HEAP
    while True:
        dat, sizes = make_file(0, False, True); dat = tame(dat)
        if u16(dat, 0) >= 5: break
    entry = rnd.randint(0, 40)
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0
    e.write(DISP + 0x67c, b"\0"); e.write(PL, bytes(0x40))
    e.call(0x401160, [entry, 0, 0], conv="thiscall", this=PL)
    out.append(f"C {st['data'].hex()} {entry} 0 0")
    nseq = u16(e.read(PL + 0x22, 2), 0)
    K = rnd.randint(1, 4); T = [TP + 0x40 * k for k in range(K)]
    tags = [rnd.choice(TAGS) for _ in range(K)]
    for k in range(K):
        e.write(T[k], bytes([0xcd]) * 0x40); e.call(0x401580, [PL], this=T[k]); e.write(ARR + 4 * k, struct.pack("<I", T[k]))
        e.write(T[k] + 0x32, tags[k] + b"\0")
    while True:
        nb = rnd.randint(1, 6); idxs = [rnd.randint(0, K - 1) for _ in range(nb)]
        if sum(1 for i in idxs if tags[i][:3] == b"HAB") <= 4: break
    sub = sorted(EXITS)[sid % len(EXITS)]; kd = 6
    first = rnd.randint(0, nseq - 1)
    data = bytearray(0x78)
    data[0] = first; data[1] = sub
    struct.pack_into("<hh", data, 2, rnd.randint(-300, 300), rnd.randint(-300, 300))
    data[6] = rnd.randint(0, 3)
    for k in range(7): data[7 + 12 * k:19 + 12 * k] = pad12(rnd.choice(NAMES))
    data[0x67] = rnd.choice([0, 1, 2, 6, 9, 16, 20, 33, 46])
    data[0x68:0x74] = pad12(rnd.choice(["DOOR1", "DOOR2", "PUERTA", "SALIDA_A", "ZZ"]))
    data[0x74] = rnd.randint(0, 3)
    cdata = bytes(data)
    t0 = rnd.choice([0, 1000, 123456]); st["now"] = t0
    ox, oy, so, ro = rnd.randint(-7000, 7000), rnd.randint(-7000, 7000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    name = "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") for _ in range(rnd.randint(1, 12)))
    e.write(IDX, bytes(rnd.getrandbits(8) for _ in range(0x100))); e.write(IDX, cdata + bytes(idxs))
    e.write(REC, bytes(0x20)); e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, bytes([nb]))
    e.write(REC + 0xc, name.encode() + b"\0"); e.write(REC + 0x1c, struct.pack("<I", len(data)))
    div = rnd.randint(1, 600)
    e.write(ROOM, bytes(0x10)); e.write(ROOM + 4, struct.pack("<i", div)); e.write(ROOM + 8, struct.pack("<h", ro))
    e.write(OBJ, bytes(0xe0)); e.write(JIMO + 0xd4, bytes(0x40)); e.write(JIMO + 0x4c, bytes(8)); e.write(CUR + 0x12c, struct.pack('<I', 0))
    world.update(ids=0, rcalls=0, active=0)
    for k in range(8): e.write(FO[k] + 0x68, bytes(4)); e.write(FB[k], bytes(16))
    del log[:]
    olv = rnd.choice([7, 7, 3, 11]); salt[0] = rnd.randrange(8)
    cur[0] = sub
    vtab, ssize = EXITS[sub]
    e.call(0x414be0, [REC, ROOM, ARR, IDX, ssize], conv="thiscall", this=OBJ, max_insns=5_000_000)
    e.write(OBJ, struct.pack("<I", vtab))
    ctor_log = list(log)
    own = bytes(rnd.choice([0, 0, 1, 2, 3, 4, 5, 6, rnd.getrandbits(8)]) for _ in range(ssize))
    if ssize: e.write(e.u32(OBJ + 0x60), own)
    e.write(OBJ + 0x40, struct.pack("<I", LVP[olv])); e.write(JIMO + 0x40, struct.pack("<I", LVP[1])); e.write(JIMO + 0x68, struct.pack("<I", 6))
    heldset["on"] = False
    out.append(f"eK {K} {ox} {oy} {so} {ro} {name} {nb} {len(cdata)} {t0} {','.join(map(str, idxs))} {cdata.hex()} {','.join(t.hex() for t in tags)} {entry} {div} {kd} {sub} {own.hex() or '-'} {olv} {salt[0]}")
    out.append("o K"); out.append("r -"); out.append("s " + state()); out.extend(". " + l for l in ctor_log)
    w = dict(now=t0, held=0, facing=1, item=0, replies=[], jphase=0, jstep=0, jenter=0, jlevel=1)
    randomize_objects(w); apply_world(w)
    out.append(wline(w)); out.append("r -"); out.append("s " + state())
    for step in range(rnd.randint(8, 50)):
        if rnd.random() < 0.5:
            w["now"] = (w["now"] + rnd.choice([0, 17, 100, 101, 150, 333, 1500, 5000])) & 0xffffffff
            w.update(held=int(rnd.random() < 0.25), facing=rnd.randint(0, 4), item=int(rnd.random() < 0.4), jphase=rnd.choice([0, 0, 0, 1, 5]), jstep=rnd.choice([0, 2, 6, 8]), jenter=rnd.choice([0, 1]), jlevel=rnd.choice([0, 1, 2, 16, 16, 22, 23, 23, 30, 34, 34, 43, 43]))
            w["replies"] = [(rnd.random() < 0.35, rnd.randrange(8)) for _ in range(rnd.choice([0, 1, 2, 4]))] if w["item"] else []
            if rnd.random() < 0.6: randomize_objects(w)
            apply_world(w)
            out.append(wline(w)); out.append("r -"); out.append("s " + state())
        r = rnd.random()
        if r < 0.30:
            v = rnd.choice([0, 0, 4]); op(f"P {v}", e.u32(e.u32(OBJ) + {0: 0x50, 4: 0x60}[v]), [])
        elif r < 0.50:
            v = rnd.choice([0, 0, 1, 4]); op(f"R {v}", e.u32(e.u32(OBJ) + 0x3c), [v])
        elif r < 0.75:
            verb = rnd.choice([0, 0, 0, 1, 3, 4, 7]); op(f"V {verb}", 0x40f020, [verb & 0xffffffff])
        elif r < 0.9: op("A", 0x40f0d0, [], ret=False)
        else: op("X", e.u32(e.u32(OBJ) + 0x70), [])
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/exits.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
