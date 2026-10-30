#!/usr/bin/env python3
"""Vectors for the script executor: loads the four shipped .DLG files with the ORIGINAL loader (0x406190, 0x4060b0) and resolver (0x406110)
into one block table, then runs sessions on the original start (0x405fd0), per-frame tick (0x405850), executor (0x405a60) and resume (0x406140).
Everything the executor calls outside itself (object lookup, speech, animations, cursor, sound, video, menu, DO routines, the clock) is
replaced by Python hooks that append one line to a log. The world is a fake: objects are found by (level, name) and numbered in order of first
use; their speaking, animation and busy flags and the clock follow per-step settings that the vector file records, so the port can be driven
by the same answers. Sessions start on every shipped block at some of its labels, with random variables, and tick every running script each step.
Usage: gen_vectors_run.py [TESORO dir] (from the repository root). Output: local-media/vectors/run.txt and the four texts in local-media/dlg (ignored), read by drivers/run_driver.cpp.
Lines: S <session> <block> <lo> <hi> <hex variables>   then per operation  o <op...> | r <result> | . <log line>...    x <hex variables>   #
Operations: E <step> <now> <speakmask> <animmask> <menuchoice> (sets the world for the next operations) | K <block> <lo> <hi> | T <block> | R <block>"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu, RET_MAGIC
from unicorn import UC_HOOK_CODE, UcError
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP

d = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
tre = open(d + "/TESORO.TRE", "rb").read(); dat = open(d + "/TESORO.DAT", "rb").read()
files = {}
for i in range(0, len(tre) - 27, 28):
    off, size, flags = struct.unpack_from("<IIB", tre, i)
    nm = tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()
    if nm.endswith(".DLG"): files[nm] = dat[off + 4:off + 4 + struct.unpack_from("<I", dat, off)[0]]

FILEP, MGR, HEAP, TEXT, TABLE = 0x3000200, 0x3000100, 0x2000000, 0x1000000, 0x3000800
WORLD, FNS = 0x3100000, 0x7e000000
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(TEXT, 0x100000, 3); e.uc.mem_map(0x3000000, 0x10000, 3)
e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
st = {"heap": HEAP, "size": 0, "base": 0}
def alloc(emu, size, arena=0):
    a = st["heap"]; st["heap"] = (a + size + 7) & ~3; return a
def fread(emu, buf, size, count, fp): emu.write(buf, struct.pack("<I", st["size"])); return count
def fgetpos(emu, fp, out):
    ptr = e.u32(FILEP); emu.write(out, struct.pack("<II", st["base"] + (ptr - TEXT), 0)); return 0
def name_key(b):
    lo = hi = 0; n = min(len(b), 12)
    def ch2(c): return ((c + 0xb) if (c < 0x35 or c >= 0x80) else c) & 0x1f
    for i in range(min(n, 6)): lo = ((lo << 5) | ch2(b[i])) & 0xffffffff
    if len(b) < 6: lo = ((lo << 5) << ((-5 * len(b) + 0x19) & 0x1f)) & 0xffffffff; return lo, 0
    for i in range(6, n): hi = ((hi << 5) | ch2(b[i])) & 0xffffffff
    if len(b) < 12: hi = ((hi << 5) << ((-5 * len(b) + 0x37) & 0x1f)) & 0xffffffff
    return lo, hi
def cstr(a):
    b = bytearray()
    while True:
        c = e.read(a, 1)
        if c == b"\0": return bytes(b)
        b += c; a += 1
def phrase(emu, s):
    lo, hi = name_key(cstr(s)); return (lo ^ (hi * 3 & 0xffffffff) ^ 0x1234567) | 1
e.hook_function(0x40db40, alloc, 2); e.hook_function(0x41ba10, alloc, 1)
e.hook_function(0x403320, lambda emu, off: 0, 1, pops=True)
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41cc90, lambda emu, fd, buf, n: 0, 3)
e.hook_function(0x413940, phrase, 1)
for i in range(4): e.write(0x442130 + 4 * i, struct.pack("<I", 0x71000000 + 0x100 * i))
e.write(0x4421ec, struct.pack("<I", 0x72000000))
e.write(0x442064, struct.pack("<I", MGR)); e.write(MGR + 4, struct.pack("<I", FILEP))
e.stub_import(0x444360, lambda emu, hwnd, text, caption, flags: 0, 4)

# Load the four files into one table, then resolve.
e.write(TABLE, bytes(0x2b0)); e.call(0x4060b0, [], conv="thiscall", this=TABLE); e.write(0x4420b8, struct.pack("<I", TABLE))
for index, nm in enumerate(["ANIMS.DLG", "CONVERS.DLG", "ICONS.DLG", "OBJETOS.DLG"]):
    text = files[nm]; st["size"] = len(text); st["base"] = 0x1000 + 0x100000 * index
    e.write(TEXT, text); e.write(FILEP, struct.pack("<IIIIIIII", TEXT, len(text), TEXT, 0x9, 3, 0, 0x1000, 0))
    e.call(0x406190, [st["base"]], conv="thiscall", this=TABLE, max_insns=400_000_000)
e.call(0x406110, [], conv="thiscall", this=TABLE, max_insns=50_000_000)
NB = e.u32(TABLE + 0x2ac)
blk = [e.u32(TABLE + 0x54 + 4 * i) for i in range(NB)]
print(NB, "blocks", file=sys.stderr)
out = []
ptr2idx = {o: i + 1 for i, o in enumerate(blk)}
for i, o in enumerate(blk):                                       # resolved commands (run: block pointer -> index + 1), checked against the port
    ne = e.u32(o + 0xc0); raw = bytearray(e.read(e.u32(o + 0xbc), 24 * ne))
    for j in range(ne):
        if struct.unpack_from("<I", raw, 24 * j)[0] == 8:
            v = struct.unpack_from("<I", raw, 24 * j + 4)[0]; struct.pack_into("<I", raw, 24 * j + 4, ptr2idx.get(v, 0) if v else 0)
    out.append(f"B {i} {ne} " + raw.hex())

# The fake world.
OVT, JVT, CVT, FBVT = WORLD + 0x4000, WORLD + 0x4100, WORLD + 0x4200, WORLD + 0x4300
J, C, P, LIST = WORLD + 0x2000, WORLD + 0x3000, WORLD + 0x3800, WORLD + 0x5000
OBJ, ANIM = WORLD + 0x10000, WORLD + 0x50000
JIM_VOICE, JIM_REST = 0x77, 0x1234
fnaddr = {}
def slot(vt, offset, fn, nargs):
    a = FNS + 0x10 * (len(fnaddr) + 1); fnaddr[a] = fn
    e.write(vt + offset, struct.pack("<I", a)); e.hook_function(a, fn, nargs, pops=True)
log = []; world = {"objs": {}, "now": 0, "speak": 0, "anim": 0, "menu": -1, "active": set()}
names = {J: "jim", C: "cursor", LIST: "list"}
for i, b in enumerate(blk): names[b] = f"script{i}"
def ecx(): return e.uc.reg_read(UC_X86_REG_ECX)
def nm(p): return names.get(p, "null" if p == 0 else "?%x" % p)
def obj_index(p): return -1 if p == J else (p - OBJ) // 0x400
def speaking(i): return (world["speak"] >> i) & 1       # bit 0 is Jim, object n is bit n
def busy_init(i): return (i * 7 + 3) % 5 == 0
def apply_flags():
    e.write(J + 0xd4, bytes([speaking(0)]))
    for p, i in world["objs"].values(): e.write(p + 0xd4, bytes([speaking(i)])); e.write(ANIM + 0x40 * i + 0x18, bytes([(world["anim"] >> i) & 1]))
def find_object(emu, lo, hi):
    key = (ecx(), lo, hi)
    if key not in world["objs"]:
        i = len(world["objs"]) + 1; p = OBJ + 0x400 * i; names[p] = f"o{i}"; names[ANIM + 0x40 * i] = f"o{i}"
        e.write(p, bytes(0x400)); e.write(p, struct.pack("<I", OVT)); e.write(p + 0xc, bytes([busy_init(i)]))
        e.write(p + 0x38, struct.pack("<I", ANIM + 0x40 * i)); e.write(p + 0xd0, struct.pack("<H", 0x100 + i))
        world["objs"][key] = (p, i); apply_flags()
    log.append(f"find {ecx():x} {lo} {hi} {nm(world['objs'][key][0])}"); return world["objs"][key][0]
e.hook_function(0x408780, find_object, 2, pops=True)
def say(emu, ph, voice, extra): log.append(f"say {nm(ecx())} {ph} {voice} {extra}")
e.hook_function(0x410330, say, 3, pops=True)
e.hook_function(0x407ce0, lambda emu, ph: log.append(f"sayjim {nm(ecx())} {ph}"), 1, pops=True)
e.hook_function(0x413a10, lambda emu: world["now"], 0)
def remove(emu, o):
    if names.get(o, "").startswith("script"): world["active"].discard(blk.index(o))
    log.append(f"remove {nm(o)}")
e.hook_function(0x40fb20, remove, 1, pops=True)
def insert(emu, o, pr):
    world["active"].add(blk.index(o)); log.append(f"insert {nm(o)} {pr}")
e.hook_function(0x40fa10, insert, 2, pops=True)
e.hook_function(0x401dc0, lambda emu: log.append(f"release {nm(ecx())}"), 0)
e.hook_function(0x401a70, lambda emu: log.append(f"release2 {nm(ecx())}"), 0)
def animate(emu, a, b, c, dd, ee):
    if ecx() != C: log.append(f"animate {nm(ecx())} {a & 0xff} {b & 0xffff} {c} {dd & 0xff} {ee & 0xff}")
e.hook_function(0x401de0, animate, 5, pops=True)
e.hook_function(0x401d80, lambda emu, v: log.append(f"show {nm(ecx())} {v & 0xff}"), 1, pops=True)
slot(OVT, 0xc, lambda emu: log.append(f"update {nm(ecx())}"), 0)
slot(OVT, 0x20, lambda emu, v: log.append(f"activate {nm(ecx())} {v & 0xff}"), 1)
slot(OVT, 0x74, lambda emu, v: log.append(f"look {nm(ecx())} {struct.unpack('<i', struct.pack('<I', v))[0]}"), 1)
def walk(emu, x, y, s, z): log.append(f"walk {nm(ecx())} {struct.unpack('<i', struct.pack('<I', x))[0]} {struct.unpack('<i', struct.pack('<I', y))[0]} {nm(s)} {z}")
slot(OVT, 0x80, walk, 4)
slot(JVT, 0x74, lambda emu, v: log.append(f"resumejim {v}"), 1)
slot(CVT, 0x20, lambda emu, v: log.append(f"cursor {v & 0xff}"), 1)
slot(FBVT, 0x70, lambda emu, v: log.append(f"do {nm(ecx())} {v}"), 1)
e.hook_function(0x4132b0, lambda emu, v: log.append(f"video {v}"), 1, pops=True)
sound = {"name": b""}
def sprintf(emu, dst, fmt, s): n = cstr(s) + b".WAV"; e.write(dst, n + b"\0"); return len(n)
e.hook_function(0x41c140, sprintf, 3)
def find_sound(emu, namep, flag): sound["name"] = cstr(namep); return 1
e.hook_function(0x403500, find_sound, 2, pops=True)
e.hook_function(0x404110, lambda emu, h, a, b, c: log.append(f"sound {sound['name'].decode('latin1')}"), 4, pops=True)
e.hook_function(0x403cb0, lambda emu, t, f: log.append(f"track {t & 0xff} {f & 0xff}"), 2, pops=True)
e.hook_function(0x419f10, lambda emu, v: log.append(f"throne {nm(ecx())} {v}"), 1, pops=True)
def menu(emu, lst, n):
    ph = [e.u32(lst + 4 * k) for k in range(n)]; log.append(f"menu {nm(ecx())} {n} " + " ".join(map(str, ph)))
    c = world["menu"]; return c if 0 <= c < n else 0xffffffff
e.hook_function(0x407d00, menu, 2, pops=True)
e.uc.hook_add(UC_HOOK_CODE, lambda uc, a, s, u: log.append("beginmenu"), begin=0x4058f9, end=0x4058f9)
e.uc.hook_add(UC_HOOK_CODE, lambda uc, a, s, u: log.append("endmenu"), begin=0x4059b7, end=0x4059b7)

def reset():
    world["objs"].clear(); world["now"] = 0; world["speak"] = 0; world["anim"] = 0; world["menu"] = -1; world["active"].clear(); world["menu_owner"] = None
    e.write(J, bytes(0x400)); e.write(J, struct.pack("<I", JVT)); e.write(J + 0xd0, struct.pack("<H", JIM_VOICE)); e.write(J + 0x10c, struct.pack("<I", JIM_REST))
    e.write(C, bytes(0x400)); e.write(C, struct.pack("<I", CVT)); e.write(C + 0x38, struct.pack("<I", P)); e.write(P, bytes(0x40))
    e.write(OBJ, bytes(0x400 * 64)); e.write(ANIM, bytes(0x40 * 64))
    for a, v in ((0x442128, C), (0x4420fc, J), (0x442104, LIST), (0x442120, WORLD + 0x5200), (0x442108, WORLD + 0x5300), (0x442080, WORLD + 0x5400)):
        e.write(a, struct.pack("<I", v))
    e.write(WORLD + 0x5400 + 0x1c, struct.pack("<I", WORLD + 0x5500))
    e.write(0x442090, bytes(0x28)); e.write(0x4420d0, bytes(0x28))
    for g in (0x4420c0, 0x4420c4, 0x4420c8, 0x442088, 0x4420bc): e.write(g, bytes(4))
    for b in blk:
        e.write(b, struct.pack("<I", FBVT)); e.write(b + 0xa0, bytes(0x10)); e.write(b + 0xcc, bytes(8)); e.write(b + 0x8c, bytes(1))
        nc = e.u32(b + 0xb8)
        if nc: e.write(e.u32(b + 0xb0), bytes(4 * nc))
    e.write(VARS, bytes(256))
VARS = WORLD + 0x8000

def run(addr, args, this, text):
    # The menu arrays and flags are shared: a second script touching them while one menu is shown makes the original read stale memory.
    if world.get("menu_owner") not in (None, this) and (addr == 0x405fd0 or world["active"]): return None
    del log[:]
    try: ret = e.call(addr, args, conv="thiscall", this=this, max_insns=3_000_000)
    except UcError: return None
    if e.uc.reg_read(UC_X86_REG_EIP) != RET_MAGIC: return None
    # Two scripts sharing the menu arrays can send one past the end of its commands, where the original reads whatever memory follows.
    for l in log:
        if l == "beginmenu": world["menu_owner"] = this
        elif l == "endmenu": world["menu_owner"] = None
    if any(e.u32(blk[b] + 0xcc) >= e.u32(blk[b] + 0xc0) for b in world["active"] | {blk.index(this)}): return None
    return ret

def op(text, addr, args, this, expect_ret=True):
    ret = run(addr, args, this, text)
    if ret is None: return False
    out.append("o " + text); out.append("r " + (str(ret) if expect_ret else "-"))
    out.extend(". " + l for l in log); return True

rnd = random.Random(20261031)
DELTAS = [0, 0, 20, 100, 200, 201, 250, 800, 3000, 6000]
def session(sid, bi, label, variables):
    reset(); e.write(VARS, bytes(variables) + bytes(256 - len(variables)))
    mark = len(out); out.append(f"S {sid} {bi} {label[0]} {label[1]} {bytes(variables).hex()}")
    level = 0x70000000 + 0x100 * (sid % 7)
    ok = op(f"K {bi} {label[0]} {label[1]} {level}", 0x405fd0, [label[0], label[1], VARS, level], blk[bi])
    steps = 0
    while ok and world["active"] and steps < 80:
        steps += 1
        world["now"] = (world["now"] + rnd.choice(DELTAS)) & 0xffffffff
        world["speak"] = rnd.getrandbits(40) & rnd.getrandbits(40) if rnd.random() < 0.7 else 0
        world["anim"] = rnd.getrandbits(40) & rnd.getrandbits(40) if rnd.random() < 0.7 else 0
        world["menu"] = rnd.choice([-1, -1, 0, 1, 2, 3, 5])
        apply_flags()
        out.append(f"o E {steps} {world['now']} {world['speak']} {world['anim']} {world['menu']}"); out.append("r -")
        for b in sorted(world["active"]):
            if b not in world["active"]: continue
            pcb = e.u32(blk[b] + 0xcc); ent = e.u32(blk[b] + 0xbc)
            walking = struct.unpack("<II", e.read(ent + 24 * pcb, 4) + e.read(ent + 24 * pcb + 0x10, 4)) == (9, 1)
            if walking and rnd.random() < 0.3: ok = op(f"R {b}", 0x406140, [], blk[b], False)
            else: ok = op(f"T {b}", 0x405850, [], blk[b])
            if not ok: break
    if not ok:
        del out[mark:]; return False
    out.append("x " + e.read(VARS, 64).hex()); out.append("#"); return True

sid = 0; dropped = 0
for bi in range(NB):
    labels = []
    nl = e.u32(blk[bi] + 0xc8); lt = e.u32(blk[bi] + 0xc4)
    for k in range(nl): labels.append(struct.unpack("<II", e.read(lt + 12 * k, 8)))
    picks = labels[:1] + rnd.sample(labels, min(2, len(labels))) + [(rnd.getrandbits(30), rnd.getrandbits(30))]
    for lab in picks:
        for rep in range(2):
            vs = [rnd.choice([0, 0, 1, 1, 2]) for _ in range(64)]
            sid += 1
            if not session(sid, bi, lab, vs): dropped += 1
os.makedirs("local-media/vectors", exist_ok=True); os.makedirs("local-media/dlg", exist_ok=True)
for nm, text in files.items(): open("local-media/dlg/" + nm, "wb").write(text)          # the driver compiles the same texts with the port
open("local-media/vectors/run.txt", "w").write("\n".join(out) + "\n")
print(sid, "sessions,", dropped, "dropped,", len(out), "lines", file=sys.stderr)
