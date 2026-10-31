#!/usr/bin/env python3
"""Vectors for the interactive object (the clickable base, vtable 0x42bad8): runs the ORIGINAL constructor (0x40ed10) over a synthetic animation bank, then the
click (0x40f020 / 0x40f040), the first-pass update (0x40ee70), the arrival (0x40f0d0) and the hover text (0x40f050 / 0x40f0b0), with the verb handlers
(0x40f210..0x40f480) running for real through the object's vtable. Hooks log what leaves the object: phrase lookups, Jim's say/face/walk/stop/update, the
pointer switch, the main list, the carried item and the hover label. Jim, the pointer and the carried item are fake objects in memory.
Lines: C <hex file> | eK ... | state   eE <now> <held> <talking> <facing> <isJim> <heldItem> <replies> | ...   o <op> / r <result> / s <state> / . <log>   #
Usage: gen_vectors_interactive.py (from the repository root). Output: local-media/vectors/interactive.txt, read by drivers/interactive_driver.cpp."""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
rnd = random.Random(20261112)
PL, ARCH, FP, DISP, HEAP = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000
e = Emu(); e.uc.mem_map(DISP, 0x2000, 3); e.uc.mem_map(HEAP, 0x1000000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def rebase")])

WORLD, FNS = 0x3200000, 0x7e000000
e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
OBJ, JIMO, CUR, HELD, VT, VTJ, CVT, LIST, TP, ARR, REC, ROOM, IDX, CAM, TXT, KEYS, LINEP, PH = [WORLD + 0x1000 * i for i in range(18)]
log = []; world = dict(now=0)
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
def cstr(a, limit=64):
    b = bytearray()
    while len(b) < limit:
        c = e.read(a, 1)
        if c == b"\0": break
        b += c; a += 1
    return bytes(b).decode("latin-1")
phr = {}      # name -> record address
def phrase_record(name):
    if name not in phr:
        a = PH + 0x40 * (len(phr) + 1); phr[name] = a
        t = TXT + 0x40 * len(phr); e.write(t, ("T:" + name).encode() + b"\0")
        e.write(a, bytes(0x40)); e.write(a + 0xc, struct.pack("<I", t))
    return phr[name]
def pname(addr):
    for k, v in phr.items():
        if v == addr: return k
    return "?" if addr else "null"
def find_phrase(emu, namep):
    n = cstr(namep, 12); log.append(f"phrase {n}"); return phrase_record(n)
e.hook_function(0x413940, find_phrase, 1)
def tname(p): return "obj" if p == OBJ else "none" if p == 0 else "?"
fn = {}
def slot(vt, offset, f, nargs):
    a = FNS + 0x10 * (len(fn) + 1); fn[a] = f
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, f, nargs, pops=True)
e.hook_function(0x410330, lambda emu, line, col, flag: log.append(f"say {pname(line)} {col & 0xffff} {flag}"), 3, pops=True)
e.hook_function(0x40fb20, lambda emu, o: log.append("remove"), 1, pops=True)
e.hook_function(0x40f900, lambda emu, o, p: log.append(f"insert {p}"), 2, pops=True)
e.hook_function(0x401de0, lambda emu, a, b, c, d, f: log.append("drop"), 5, pops=True)
def hover(emu, text, col): log.append("hover " + ("null" if text == 0 else cstr(text)));
e.hook_function(0x4023f0, hover, 2, pops=True)
def cur_slot8(emu, on): log.append(f"pointer {on & 0xff}")
vt0 = e.read(0x42bad8, 4 * 28)
for v in (VT, VTJ): e.write(v, vt0 + bytes(4 * 8))
slot(VT, 0x70, lambda emu: log.append("stoptalk"), 0)
slot(VT, 0x74, lambda emu, f: log.append(f"face {i32(f)}"), 1)
slot(VT, 0x80, lambda emu, x, y, t, d: log.append(f"walk {i32(x)} {i32(y)} {tname(t)} {d}"), 4)
slot(VTJ, 0x70, lambda emu: log.append("stoptalk"), 0)
slot(VTJ, 0x74, lambda emu, f: log.append(f"face {i32(f)}"), 1)
slot(VTJ, 0x80, lambda emu, x, y, t, d: log.append(f"walk {i32(x)} {i32(y)} {tname(t)} {d}"), 4)
slot(VTJ, 0x10, lambda emu: log.append("jimupdate"), 0)
slot(CVT, 0x20, cur_slot8, 1)
e.write(JIMO, struct.pack("<I", VTJ)); e.write(CUR, struct.pack("<I", CVT))
for g, v in ((0x442128, CUR), (0x442104, LIST)): e.write(g, struct.pack("<I", v))
e.write(HELD + 0x68, struct.pack("<I", 0))
RP = [f"RP{k}" for k in range(8)]
for n in RP: phrase_record(n)

def tame(dat):
    b = bytearray(dat); p = 0x18
    for _ in range(u16(b, 0)):
        for off in (0, 2, 4, 6, 0xc, 0xe, 0x10, 0x12): struct.pack_into("<h", b, p + off, rnd.randint(-40, 40))
        p += 0x18 + u16(b, p + 0x16)
    return bytes(b)
NAMES = ["", "", "OBJ1", "OBJ2", "OBJ125T03", "A", "ABCDEFGHIJKL", "ZZZ99", "OBJ106T05", "COSA"]
def pad12(n): return n.encode()[:12].ljust(12, b"\0")
def state(plist):
    r = lambda off: e.u32(OBJ + off)
    s = [r(0x68), e.read(OBJ + 0x8c, 1)[0], e.read(OBJ + 0x8d, 1)[0], i32(r(0x90)), i32(r(0x94)), i32(r(0x98)), i32(r(0x9c)), pname(r(0x6c))]
    s += [pname(r(0x70 + 4 * k)) for k in range(7)]
    s += [u16(e.read(OBJ + 0x30, 2), 0), e.u32(HELD + 0x68) & 4 if heldset["on"] else -1, "held" if e.u32(CUR + 0x12c) else "none"]
    p = e.read(e.u32(OBJ + 0x38), 0x40)
    s += [u32(p, 0x18), s32(u32(p, 8)), u32(p, 0xc), u16(p, 0x30), u16(p, 0x24), u16(p, 0x26), u16(p, 0x2c), u16(p, 0x2e)]
    return " ".join(map(str, s))
heldset = {"on": False}
out = []
def op(text, addr, args, ret=True, conv="thiscall"):
    del log[:]
    try: r = e.call(addr, args, conv=conv, this=OBJ, max_insns=3_000_000)
    except Exception as ex:
        from unicorn.x86_const import UC_X86_REG_EIP
        print("FAULT", text, hex(addr), "eip", hex(e.uc.reg_read(UC_X86_REG_EIP)), "log", log, "sid", sid, file=sys.stderr); raise
    out.append("o " + text); out.append("r " + (str(r & 0xffffffff) if ret else "-")); out.append("s " + state(None)); out.extend(". " + l for l in log)
def apply_world(w):
    st["now"] = w["now"]
    e.write(JIMO + 0xd4, bytes([w["held"]])); e.write(JIMO + 0x10c, struct.pack("<I", w["facing"] & 0xffffffff))
    e.write(0x4420fc, struct.pack("<I", OBJ if w["isjim"] else JIMO))
    if w["isjim"]: e.write(OBJ + 0xd4, bytes([0])); e.write(OBJ + 0x10c, struct.pack("<I", w["facing"] & 0xffffffff))
    # the carried item: its table of (object key, reply line) pairs
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

for sid in range(1, 501):
    st["heap"] = HEAP
    while True:
        dat, sizes = make_file(0, False, True); dat = tame(dat)
        if u16(dat, 0) > 0: break
    entry = rnd.randint(0, 40)
    st["data"] = bytes(rnd.getrandbits(8) for _ in range(entry + 4)) + dat; st["pos"] = 0
    e.write(DISP + 0x67c, b"\0"); e.write(PL, bytes(0x40))
    e.call(0x401160, [entry, 0, 0], conv="thiscall", this=PL)
    out.append(f"C {st['data'].hex()} {entry} 0 0")
    nseq = u16(e.read(PL + 0x22, 2), 0)
    K = rnd.randint(1, 3); T = [TP + 0x40 * k for k in range(K)]
    for k in range(K):
        e.write(T[k], bytes([0xcd]) * 0x40); e.call(0x401580, [PL], this=T[k]); e.write(ARR + 4 * k, struct.pack("<I", T[k]))
    nb = rnd.randint(1, 3); idxs = [rnd.randint(0, K - 1) for _ in range(nb)]
    first = rnd.randint(0, nseq - 1)
    data = bytearray(0x5b)
    data[0] = first; data[1] = rnd.getrandbits(8)
    struct.pack_into("<hh", data, 2, rnd.randint(-300, 300), rnd.randint(-300, 300))
    data[6] = rnd.randint(0, 3)
    for k in range(7): data[7 + 12 * k:19 + 12 * k] = pad12(rnd.choice(NAMES))
    # the phrases of the verbs start at 0x13: name 0x7 is the hover name (12 bytes) so verb names are k = 1..6
    data = bytes(data)
    t0 = rnd.choice([0, 1000, 123456]); st["now"] = t0
    ox, oy, so, ro = rnd.randint(-7000, 7000), rnd.randint(-7000, 7000), rnd.randint(-300, 300), rnd.randint(-300, 300)
    name = "".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") for _ in range(rnd.randint(1, 12)))
    e.write(IDX, bytes(rnd.getrandbits(8) for _ in range(0x80))); e.write(IDX, data + bytes(idxs))
    e.write(REC, bytes(0x20)); e.write(REC, struct.pack("<ii", ox, oy)); e.write(REC + 8, struct.pack("<h", so)); e.write(REC + 0xb, bytes([nb]))
    e.write(REC + 0xc, name.encode() + b"\0"); e.write(REC + 0x1c, struct.pack("<I", len(data)))
    e.write(ROOM, bytes(0x10)); e.write(ROOM + 8, struct.pack("<h", ro))
    e.write(OBJ, bytes(0x100)); e.write(JIMO + 0xd4, bytes(0x40)); e.write(CUR + 0x12c, struct.pack('<I', 0))
    del log[:]
    e.call(0x40ed10, [REC, ROOM, ARR, IDX, 0], conv="thiscall", this=OBJ, max_insns=5_000_000)
    ctor_log = list(log)
    e.write(OBJ, struct.pack("<I", VT))                  # same slots, plus Jim's
    heldset["on"] = False
    out.append(f"eK {K} {ox} {oy} {so} {ro} {name} {nb} {len(data)} {t0} {','.join(map(str, idxs))} {data.hex()}")
    out.append("o K"); out.append("r -"); out.append("s " + state(None)); out.extend(". " + l for l in ctor_log)
    w = dict(now=t0, held=0, talk=0, facing=1, isjim=0, item=0, replies=[])
    apply_world(w)
    out.append(f"o E {w['now']} {w['held']} {w['talk']} {w['facing']} {w['isjim']} {w['item']} -"); out.append("r -"); out.append("s " + state(None))
    for step in range(rnd.randint(6, 40)):
        if rnd.random() < 0.5:
            w["now"] = (w["now"] + rnd.choice([0, 17, 100, 333, 1500, 5000])) & 0xffffffff
            w.update(held=int(rnd.random() < 0.35), talk=0, facing=rnd.randint(0, 4), isjim=int(rnd.random() < 0.12), item=int(rnd.random() < 0.5))
            w["replies"] = [(rnd.random() < 0.35, rnd.randrange(8)) for _ in range(rnd.choice([0, 1, 2, 4]))] if w["item"] else []
            apply_world(w)
            out.append(f"o E {w['now']} {w['held']} {w['talk']} {w['facing']} {w['isjim']} {w['item']} " + (",".join(f"{int(m)}:{k}" for m, k in w["replies"]) or "-"))
            out.append("r -"); out.append("s " + state(None))
        r = rnd.random()
        if r < 0.08:
            item = rnd.choice([0, 0, 1, 2, 5, 37]); op(f"I {item}", 0x40f480, [item])
        elif r < 0.3:
            verb = rnd.choice([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 0, 1, 2, 3, 4, rnd.randint(-3, 20)])
            op(f"V {verb}", 0x40f020, [verb & 0xffffffff])
        elif r < 0.4:
            verb = rnd.randint(0, 9); op(f"R {verb}", 0x40f040, [verb])
        elif r < 0.65: op("U", 0x40ee70, [])
        elif r < 0.8: op("A", 0x40f0d0, [], ret=False)
        elif r < 0.9: op("H", 0x40f050, [])
        else: op("C", 0x40f0b0, [])
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/interactive.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines", file=sys.stderr)
