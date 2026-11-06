#!/usr/bin/env python3
"""Vectors for the items of the inventory (level kind 13, 14 subtypes): the ORIGINAL constructor (0x406a90) with the factory's data tweak, state size and vtable, then the update,
the pick-up (slot 8), the click with a verb (slot 14), the verbs and the item uses through the object's own vtable. The pointer, the carried item (with its own combinations and a
use that answers as the world says), Jim, the objects of the levels and the scripts are hooked and logged.
Output: local-media/vectors/items.txt, read by drivers/items_driver.cpp.
Usage: gen_vectors_items.py (from the repository root).
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
log = []; world = dict(now=0, ids=0, rcalls=0, active=0, hu=0)
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
cond_hook(0x401de0, lambda: ecx() == CUR, lambda emu, a, b, c, d, f: log.append(f"cursor {b & 0xffff} {i32(c)}"), 5, True)
cond_hook(0x401de0, lambda: ecx() != CUR, lambda emu, a, b, c, d, f: log.append(f"frame {FO.index(ecx()) if ecx() in FO else 'held' if ecx() == HELD else 'obj'} {b & 0xffff} {i32(c)}"), 5, True)
e.hook_function(0x4028c0, lambda emu: log.append("pageleft"), 0)
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
e.write(0x4420b8, struct.pack("<I", WORLD + 0x51000))
FO = [WORLD + 0x52000 + 0x100 * k for k in range(8)]; FB = [WORLD + 0x53000 + 0x100 * k for k in range(8)]
FOVT = WORLD + 0x55000
e.write(FOVT, e.read(0x428000, 4 * 33))
slot(FOVT, 0x50, lambda emu: log.append(f"verb0 {[k for k in range(8) if FO[k] == ecx()][0]}"), 0)
for k in range(8): e.write(FO[k] + 0x60, struct.pack("<I", FB[k])); e.write(FO[k], struct.pack("<I", FOVT))
salt = [0]
special = {}
def pick(idx, lo, hi):
    if (lo, hi) in special: return {1: 8, 2: 10}.get(special[(lo, hi)], (lo * 3 + hi * 5 + (idx & 0xffffffff) * 7 + salt[0]) & 7)
    return (lo * 3 + hi * 5 + (idx & 0xffffffff) * 7 + salt[0]) & 7
def namekey(name):
    e.write(TXT + 0x3f00, name.encode() + b"\0"); e.call(0x415c40, [TXT + 0x3f00]); v = e.edx_eax(); return v & 0xffffffff, v >> 32
def find_obj(emu, lo, hi):
    idx = LV[ecx()]; log.append(f"find {idx} {lo} {hi}"); v = pick(idx, lo, hi); return FO[v] if v < 8 else HELD if v == 8 else OBJ
e.hook_function(0x408780, find_obj, 2, pops=True)
HVT = WORLD + 0x56000
e.write(HVT, e.read(0x428b00, 4 * 28))
slot(HVT, 0x6c, lambda emu, n: (log.append(f"heldUse {n & 0xffff}"), world["hu"])[1], 1)
e.write(HELD, struct.pack("<I", HVT))
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
def start_script(emu, lo, hi, vars_, level):
    where = "jim" if level == e.u32(JIMO + 0x40) else "own" if level == e.u32(OBJ + 0x40) else "other"      # which level the script is started in
    log.append(f"start {lo} {hi} {vname(vars_)} {where}"); return 3
e.hook_function(0x405fd0, start_script, 4, pops=True)
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
SSZ = {9: 0, 10: 1, 15: 1, 16: 3, 19: 2, 26: 1, 28: 1, 29: 1, 33: 1, 36: 1, 37: 1, 42: 0, 44: 1}
ITEMS = {}
for sb, bv in KINDS["13"]["sub"].items():
    if bv.get("vtable"): ITEMS[int(sb)] = (int(bv["vtable"], 16), SSZ[int(sb)])
for plain in (1, 2, 50): ITEMS[plain] = (0x428b00, 0)
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
own_special = {}
cur = [0]
def state():
    r = lambda off: e.u32(OBJ + off)
    s = [r(0x68), e.read(OBJ + 0x8c, 1)[0], i32(r(0x9c)), i32(r(0x94)), i32(r(0x98)), pname(r(0x6c))]
    s += [pname(r(0x70 + 4 * k)) for k in range(7)]
    n = u16(e.read(OBJ + 0xa8, 2), 0)
    s += [n, u16(e.read(OBJ + 0xaa, 2), 0)]
    combos = []
    for i in range(n):
        kk = e.read(e.u32(OBJ + 0xa0) + 8 * i, 8); combos.append(f"{u32(kk, 0)}:{u32(kk, 4)}:{pname(e.u32(e.u32(OBJ + 0xa4) + 4 * i))}")
    s.append(",".join(combos) or "-")
    s += [struct.unpack("<h", e.read(OBJ + 0x5e, 2))[0], e.read(OBJ + 0x5d, 1)[0]]
    p = e.read(e.u32(OBJ + 0x38), 0x40)
    s += [u32(p, 0x18), s32(u32(p, 8)), u32(p, 0xc), u16(p, 0x30), u16(p, 0x24), u16(p, 0x26), u16(p, 0x2c), u16(p, 0x2e), u16(p, 0x28), u16(p, 0x2a)]
    s += [r(4), r(8)]
    s += ["held" if e.u32(CUR + 0x12c) == HELD else "obj" if e.u32(CUR + 0x12c) == OBJ else "none", e.u32(HELD + 0x68), e.u32(0x4420f8), e.read(JIMO + 0x110, 2).hex()]
    ss = ITEMS.get(cur[0], (0, 0))[1]
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
CAMI = WORLD + 0x57000
e.write(WORLD + 0x50000 + 0x80, struct.pack("<I", CAMI))
def apply_world(w):
    st["now"] = w["now"]
    for k, (fl, bl, bk) in enumerate(w["fo"]): e.write(FO[k] + 0x68, struct.pack("<I", fl)); e.write(FB[k], bl); e.write(FO[k] + 0x32, bytes([bk]))
    e.write(JIMO + 0xd4, bytes([w["held"]]))
    e.write(JIMO + 0x110, bytes([0x55, w["jstep"]])); e.write(JIMO + 0x40, struct.pack("<I", LVP[w["jlevel"]]))
    e.write(CAMI + 0x2c, struct.pack("<i", w["camx"]))
    world["hu"] = w["hu"]
    keys = b""; lines = b""
    for lo, hi, k in w["hcombos"]: keys += struct.pack("<II", lo, hi); lines += struct.pack("<I", phr[RP[k]])
    e.write(KEYS, keys); e.write(LINEP, lines)
    e.write(HELD + 0xa0, struct.pack("<II", KEYS, LINEP)); e.write(HELD + 0xa8, struct.pack("<H", len(w["hcombos"]))); e.write(HELD + 0xaa, struct.pack("<H", w["hnum"]))
    e.write(HELD + 0x68, struct.pack("<I", w["hflags"]))
    special.clear(); special.update(w["special"])
    e.write(CUR + 0x12c, struct.pack("<I", HELD if w["item"] else 0))
def randomize_objects(w):
    w["fo"] = [(rnd.choice([0, 2, 4, 6, 0x400, 0x402]), bytes(rnd.choice([0, 0, 1, 2, 3, 4, 5, 6, rnd.getrandbits(8)]) for _ in range(16)), rnd.choice([0, 1, 2, 2, 3])) for _ in range(8)]
def randomize_held(w):
    hc = []
    for _ in range(rnd.choice([0, 0, 1, 2, 3])):
        lo, hi = rnd.getrandbits(32), rnd.getrandbits(32)
        hc.append((lo, hi, rnd.randrange(8)))
    w["hcombos"] = hc; w["special"] = {(lo, hi): rnd.choice([0, 1, 2, 2]) for lo, hi, _ in hc}; w["special"].update(own_special)
    w["hnum"] = rnd.choice([0, 1, 5, 7, 8, 9, 10, 0x19, 0x29, rnd.randrange(80)]); w["hu"] = rnd.choice([0, 0, 1, 5]); w["hflags"] = rnd.choice([0, 2, 4, 6])
def wline(w):
    return f"o E {w['now']} {w['held']} {w['item']} {w['hnum']} {w['hu']} {w['hflags']} " + (",".join(f"{lo}:{hi}:{k}" for lo, hi, k in w["hcombos"]) or "-") + f" {w['jstep']} {w['jlevel']} {w['camx']} " + (",".join(f"{lo}:{hi}:{c}" for (lo, hi), c in w["special"].items()) or "-") + " " + ",".join(f"{fl}:{bl.hex()}:{bk}" for fl, bl, bk in w["fo"])

for sid in range(1, 1401):
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
    sub = sorted(ITEMS)[sid % len(ITEMS)]; kd = 13
    first = rnd.randint(0, nseq - 1)
    ncombo = rnd.choice([0, 1, 2, 3])
    data = bytearray(0x7c + 24 * ncombo + 4)
    data[0] = first; data[1] = sub
    data[2:14] = pad12("I" + rnd.choice(["NAME", "ABC", "XYZ"]))
    for k in range(6): data[0xe + 12 * k:0x1a + 12 * k] = pad12(rnd.choice(NAMES))
    struct.pack_into("<H", data, 0x62, ncombo)
    cnames = ["OBJ1", "OBJ2", "ICACA", "IBOLA", "ZZZ99", "IJARRA", "ICUBO"]
    for k in range(ncombo):
        data[0x7c + 24 * k:0x7c + 24 * k + 12] = pad12(rnd.choice(cnames)); data[0x7c + 24 * k + 12:0x7c + 24 * k + 24] = pad12(rnd.choice(["OBJ125T03", "OBJ96T07", "COSA", "A1"]))
    cdata = bytes(data)
    own_special.clear()
    for k in range(ncombo):
        nm = bytes(data[0x7c + 24 * k:0x7c + 24 * k + 12]).rstrip(b"\0").decode()
        own_special[namekey(nm)] = rnd.choice([0, 1, 2])
    t0 = rnd.choice([0, 1000, 123456]); st["now"] = t0
    ox, oy, so, ro = rnd.randint(-7000, 7000), rnd.randint(-7000, 7000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    name = "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") for _ in range(rnd.randint(1, 12)))
    e.write(IDX, bytes(rnd.getrandbits(8) for _ in range(0x100))); e.write(IDX, cdata + bytes(idxs))
    e.write(REC, bytes(0x20)); e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, bytes([nb]))
    e.write(REC + 0xc, name.encode() + b"\0"); e.write(REC + 0x1c, struct.pack("<I", len(data)))
    div = rnd.randint(1, 600)
    e.write(ROOM, bytes(0x10)); e.write(ROOM + 4, struct.pack("<i", div)); e.write(ROOM + 8, struct.pack("<h", ro))
    e.write(OBJ, bytes(0xe0)); e.write(JIMO + 0xd4, bytes(0x40)); e.write(CUR + 0x12c, struct.pack('<I', 0))
    world.update(ids=0, rcalls=0, active=0)
    for k in range(8): e.write(FO[k] + 0x68, bytes(4)); e.write(FB[k], bytes(16))
    e.write(HELD + 0x68, bytes(4))
    del log[:]
    olv = rnd.choice([7, 7, 3, 11]); salt[0] = rnd.randrange(8)
    cur[0] = sub
    vtab, ssize = ITEMS[sub]
    e.write(0x4420f8, struct.pack("<I", 0))
    e.call(0x406a90, [REC, ROOM, ARR, IDX, ssize], conv="thiscall", this=OBJ, max_insns=5_000_000)
    e.write(OBJ, struct.pack("<I", vtab))
    ctor_log = list(log)
    own = bytes(rnd.choice([0, 0, 1, 2, 3, 4, 5, 6, 0x0f, 0x17, 0x1b, 0x1e, 0x1f, rnd.getrandbits(8)]) for _ in range(ssize))
    if ssize: e.write(e.u32(OBJ + 0x60), own)
    e.write(JIMO + 0x40, struct.pack("<I", LVP[1])); e.write(JIMO + 0x68, struct.pack("<I", 6)); e.write(JIMO + 0xd0, bytes(2))
    heldset["on"] = False
    out.append(f"eK {K} {ox} {oy} {so} {ro} {name} {nb} {len(cdata)} {t0} {','.join(map(str, idxs))} {cdata.hex()} {','.join(t.hex() for t in tags)} {entry} {div} {kd} {sub} {own.hex() or '-'} {olv} {salt[0]}")
    out.append("o K"); out.append("r -"); out.append("s " + state()); out.extend(". " + l for l in ctor_log)
    w = dict(now=t0, held=0, item=0, jstep=0, jlevel=1, camx=0, special={})
    randomize_objects(w); randomize_held(w); apply_world(w)
    out.append(wline(w)); out.append("r -"); out.append("s " + state())
    for step in range(rnd.randint(8, 50)):
        if rnd.random() < 0.5:
            pend, vb = e.read(OBJ + 0x8c, 1)[0], i32(e.u32(OBJ + 0x9c))
            w["now"] = (w["now"] + rnd.choice([0, 17, 100, 333, 1500])) & 0xffffffff
            w.update(held=int(rnd.random() < 0.25), jstep=rnd.choice([0, 2, 3, 4, 5, 6, 7, 8, rnd.getrandbits(8)]), jlevel=rnd.choice([1, 6, 9, 6, 20]), camx=rnd.choice([0, 0x100, 0x280, 0x281, 0x500, -5]))
            w["item"] = 1 if (pend and vb >= 7) else int(rnd.random() < 0.55)
            if rnd.random() < 0.5: randomize_objects(w)
            if rnd.random() < 0.5: randomize_held(w)
            apply_world(w)
            out.append(wline(w)); out.append("r -"); out.append("s " + state())
        r = rnd.random()
        NOS = [0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0x14, 0x16, 0x19, 0x1f, 0x29, 0xff, rnd.randint(0, 0x60), w['hnum']]
        if r < 0.20:
            v = rnd.choice([0, 1, 2, 3, 4, 5, 6]); op(f"P {v}", e.u32(e.u32(OBJ) + {0: 0x50, 1: 0x54, 2: 0x5c, 3: 0x58, 4: 0x60, 5: 0x64, 6: 0x68}[v]), [])
        elif r < 0.40:
            item = rnd.choice(NOS); op(f"I {item}", e.u32(e.u32(OBJ) + 0x6c), [item])
        elif r < 0.70:
            verb = rnd.choice([0, 1, 2, 3, 4, 5, 6, 1, 4, 7 + rnd.choice(NOS)])
            if verb >= 7 and not e.u32(CUR + 0x12c): verb = 3
            op(f"V {verb}", e.u32(e.u32(OBJ) + 0x38), [verb & 0xffffffff])
        elif r < 0.88:
            if not (e.read(OBJ + 0x8c, 1)[0] and i32(e.u32(OBJ + 0x9c)) >= 7 and not e.u32(CUR + 0x12c)): op("U", e.u32(e.u32(OBJ) + 0x10), [])
        else:
            v = rnd.randint(0, 1); op(f"S {v}", e.u32(e.u32(OBJ) + 0x20), [v], ret=False)
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/items.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
