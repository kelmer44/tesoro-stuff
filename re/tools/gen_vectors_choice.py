#!/usr/bin/env python3
"""Vectors for the script menu's list of choices: runs the ORIGINAL Dialogue_ChoiceMenu (0x407d00) in Unicorn together with the original text layout
(0x411c80) on synthetic fonts (as gen_vectors_text.py makes them). The pointer's label (0x4023f0), the pointer draw (vtable slot 6) and the removal
from the main list (0x40fb20) are logging hooks; the mouse is a fake object. Each case lays the choices out with the mouse far away to find
where each choice's band starts and ends, then runs the menu at random places and at the edges of the bands, with the left button up or down.
Usage: gen_vectors_choice.py (from the repository root). Output: local-media/vectors/choice.txt (ignored), read by drivers/choice_driver.cpp.
Lines: C <hex font file> <entry> | Z (frame reset) | A <text args as in gen_vectors_text.py> (a text queued before the menu) | P <n> <hex text> ... |
       M <mouseX> <mouseY> <button> then r <result> | . <log> | N <count> | R <hex text> <x> <y> <colour> <font> | T <layer tag> | #"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu
from unicorn.x86_const import UC_X86_REG_ECX

src = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_anim.py").read()
tsrc = open(__file__.rsplit("/", 1)[0] + "/gen_vectors_text.py").read()
rnd = random.Random(20261108)
PL, ARCH, FP, DISP, HEAP, ROWS = 0x100800, 0x100100, 0x100200, 0x1200000, 0x2000000, 0x3000000
e = Emu(); e.uc.mem_map(DISP, 0x40000, 3); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(ROWS, 0x150000, 3); e.uc.mem_map(0x3400000, 0x100000, 3)
st = {"data": b"", "pos": 0, "heap": HEAP, "now": 0}
exec(src[src.index("def fread"):src.index("e.write(0x442064")])
e.write(0x442064, struct.pack("<I", ARCH)); e.write(ARCH + 4, struct.pack("<I", FP))
e.write(0x442100, struct.pack("<I", 0x100300)); e.write(0x442108, struct.pack("<I", DISP))
exec(src[src.index("def u16"):src.index("def make_blob")])
e.write(DISP + 0x670, struct.pack("<I", ROWS)); e.write(DISP + 0x674, struct.pack("<I", ROWS + 240000 * 4))
exec(tsrc[tsrc.index("def make_glyph"):tsrc.index("def make_background")])
exec(tsrc[tsrc.index("def load_font"):tsrc.index("def crcs")])

WORLD, FNS = 0x3200000, 0x7e000000
e.uc.mem_map(WORLD, 0x100000, 3); e.uc.mem_map(FNS, 0x1000, 5)
CUR, CVT, MOUSE, BTNS, IFACE, JIM, LINES, TEXTS = [WORLD + 0x1000 * i for i in range(8)]; PHR = WORLD + 0x10000
CAMERA = 0x777000
log = []
def i32(v): return struct.unpack("<i", struct.pack("<I", v & 0xffffffff))[0]
e.write(CVT + 0x18, struct.pack("<I", FNS + 0x10))
e.hook_function(FNS + 0x10, lambda emu, cam: log.append(f"cursor {i32(e.u32(CUR + 0x4c))} {i32(e.u32(CUR + 0x50))} {cam}"), 1, pops=True)
e.hook_function(0x4023f0, lambda emu, t, c: log.append(f"label {'null' if t == 0 else 'text'} {i32(c)}"), 2, pops=True)
e.hook_function(0x40fb20, lambda emu, o: log.append("remove jim" if o == JIM else "remove ?"), 1, pops=True)
e.write(CUR, struct.pack("<I", CVT)); e.write(0x442128, struct.pack("<I", CUR))
e.write(0x44206c, struct.pack("<I", MOUSE)); e.write(MOUSE + 0x1c, struct.pack("<I", BTNS))
e.write(0x4421ec, struct.pack("<I", IFACE)); e.write(IFACE + 0x80, struct.pack("<I", CAMERA)); e.write(0x442104, struct.pack("<I", WORLD + 0x20000))

def cstr(a):
    b = bytearray()
    while True:
        c = e.read(a, 1)
        if c == b"\0": return bytes(b)
        b += c; a += 1
WORDS = ["a", "un", "el", "tesoro", "Alcachofa", "isla", "capitan", "pirata", "!hola!", "?", "ab,cd", "0123456789", "supercalifragilistico"]
def make_text():
    return (" ".join(rnd.choice(WORDS) for _ in range(rnd.randint(1, 12)))).encode("latin1")

out = []
def run(mx, my, btn, phrases):
    e.write(MOUSE + 0x38, struct.pack("<ii", mx, my)); e.write(BTNS, bytes([8 if btn else 0, 0]))
    for k, t in enumerate(phrases):
        e.write(TEXTS + 0x400 * k, t + b"\0"); e.write(LINES + 16 * k, bytes(16)); e.write(LINES + 16 * k + 0xc, struct.pack("<I", TEXTS + 0x400 * k))
        e.write(PHR + 4 * k, struct.pack("<I", LINES + 16 * k))
    del log[:]
    try: return i32(e.call(0x407d00, [PHR, len(phrases)], conv="thiscall", this=JIM, max_insns=5_000_000))
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP
        from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESI, UC_X86_REG_EBP
        print("menu fault eip %x eax %x esi %x ebp %x PHR %x LINES %x" % (e.uc.reg_read(UC_X86_REG_EIP), e.uc.reg_read(UC_X86_REG_EAX), e.uc.reg_read(UC_X86_REG_ESI), e.uc.reg_read(UC_X86_REG_EBP), PHR, LINES), file=sys.stderr); raise
def dump(ret):
    out.append(f"r {ret}"); out.extend(". " + l for l in log)
    n = e.u32(DISP + 0x13a0); out.append(f"N {n}")
    for i in range(n):
        r = DISP + 0x680 + 0x70 * i; x, yy = struct.unpack("<ii", e.read(r + 0x64, 8)); col, fnt = struct.unpack("<HH", e.read(r + 0x6c, 4))
        out.append(f"R {cstr(r).hex() or '-'} {x} {yy} {col} {fnt}")
    out.append("T " + str(struct.unpack("<H", e.read(DISP + 0x668, 2))[0]))
dropped = 0
for case in range(300):
    nglyph = rnd.choice([95, 130, 190])
    e.write(DISP + 0x67c, b"\0"); st["heap"] = HEAP
    font = make_font(nglyph)
    out.append(load_font(PL, font, rnd.randint(0, 20))); start = len(out) - 1
    e.write(DISP + 0x20, struct.pack("<II", PL, PL)); e.write(DISP + 0x67c, b"\0")
    n = rnd.randint(1, 6); phrases = [make_text() for _ in range(n)]
    pre = rnd.random() < 0.4
    def prepare():
        e.call(0x411f20, [], this=DISP)
        if pre:
            e.write(0x3480000, b"hola mundo\0"); e.call(0x411c80, [0x3480000, 0xffff, 0, 320, 100, 400, 0, 1, 0], conv="thiscall", this=DISP)
    # scan the mouse over every row to find the bands
    hits = {}
    ok = True
    def probe(my):
        global ok
        if my in hits: return
        prepare(); r = run(100, my, True, phrases)
        if e.u32(DISP + 0x13a0) > 29 or any(len(cstr(DISP + 0x680 + 0x70 * i)) >= 100 for i in range(e.u32(DISP + 0x13a0))): ok = False
        hits[my] = r
    coarse = list(range(-230, 701, 5))
    for my in coarse: probe(my)
    for a, b in zip(coarse, coarse[1:]):
        if hits[a] != hits[b]:
            for my in range(a + 1, b): probe(my)
    if not ok: del out[start:]; dropped += 1; continue
    edges = sorted({y for y in hits if y - 1 in hits and hits[y] != hits[y - 1]})
    places = [(rnd.randint(-320, 320), rnd.randint(-250, 700)) for _ in range(6)] + [(rnd.randint(-320, 320), y + d) for y in edges for d in (-1, 0)]
    out.append("P " + str(n) + " " + " ".join(p.hex() for p in phrases))
    for mx, my in places:
        for btn in (0, 1):
            out.append("Z"); prepare()
            if pre: out.append("A " + b"hola mundo".hex() + " 65535 0 320 100 400 0 1 0 0")
            out.append(f"M {mx} {my} {btn}"); dump(run(mx, my, btn, phrases))
    out.append("#")
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/choice.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines,", dropped, "cases dropped", file=sys.stderr)
