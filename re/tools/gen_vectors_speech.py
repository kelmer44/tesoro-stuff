#!/usr/bin/env python3
"""Vectors for the speech tables: runs the ORIGINAL table loader (0x413570: reads `ID, "text"` lines, finds ID.WAV, sorts with 0x4137c0),
the table lookup (0x4138b0) and the four-table search (0x413940) in Unicorn on the shipped OBJETOS/ANIMS/CONVERS/ICONS.TXT and on 300 randomly
edited slices of them. The CRT file calls are hooked onto an in-memory text; the archive lookup of ID.WAV (0x403500) is replaced by a name hash.
Usage: gen_vectors_speech.py [TESORO dir] (from the repository root). Output: local-media/vectors/speech.txt (ignored), read by drivers/speech_driver.cpp.
Lines: F <name> <size> <hex text> | N <count> | L <keylo> <keyhi> <wav> <hex text> (in sorted order) | P <keylo> <keyhi> <index or -1> | Q <hex name> <table or -1> <index> | #"""
import os, random, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

d = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
tre = open(d + "/TESORO.TRE", "rb").read(); dat = open(d + "/TESORO.DAT", "rb").read()
files = {}
for i in range(0, len(tre) - 27, 28):
    off, size, flags = struct.unpack_from("<IIB", tre, i)
    nm = tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()
    if nm.endswith(".TXT"): files[nm] = dat[off + 4:off + 4 + struct.unpack_from("<I", dat, off)[0]]

FILEP, MGR, HEAP, TEXT, TAB = 0x3000200, 0x3000100, 0x2000000, 0x1000000, 0x3000900
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(TEXT, 0x100000, 3); e.uc.mem_map(0x3000000, 0x10000, 3)
st = {"heap": HEAP, "size": 0, "base": 0}
def alloc(emu, size, arena=0):
    a = st["heap"]; st["heap"] = (a + size + 7) & ~3; emu.write(a, bytes(size)); return a      # the pool hands out zeroed memory
def fread(emu, buf, size, count, fp): emu.write(buf, struct.pack("<I", st["size"])); return count
def cstr(emu, a):
    b = bytearray()
    while True:
        c = emu.read(a, 1)
        if c == b"\0": return bytes(b)
        b += c; a += 1
def fgetpos(emu, fp, out):
    emu.write(out, struct.pack("<II", st["base"] + (e.u32(FILEP) - TEXT), 0)); return 0
def fseek(emu, fp, posp):
    off = struct.unpack("<I", emu.read(posp, 4))[0] - st["base"]
    emu.write(FILEP, struct.pack("<IIIIIIII", TEXT + off, st["size"] - off, TEXT, 0x9, 3, 0, 0x1000, 0)); return 0
def wav_offset(name):
    h = 0
    for c in name: h = (h * 31 + c) & 0xffffffff
    return 0 if h % 5 == 0 else 1000 + h % 100000
def find_entry(emu, name, report): return wav_offset(cstr(emu, name))
errors = {"n": 0}
def message_box(emu, hwnd, text, caption, flags): errors["n"] += 1; return 0
e.hook_function(0x40db40, alloc, 2); e.hook_function(0x403320, lambda emu, off: 0, 1, pops=True)
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41b600, fseek, 2)
e.hook_function(0x41cc90, lambda emu, fd, buf, n: 0, 3); e.hook_function(0x403500, find_entry, 2, pops=True)
e.stub_import(0x444360, message_box, 4)
e.write(0x442064, struct.pack("<I", MGR)); e.write(MGR + 4, struct.pack("<I", FILEP))

out = []; rnd = random.Random(20261030)
def load(name, text, tab, base):
    st["size"] = len(text); st["base"] = base
    e.write(TEXT, text); e.write(FILEP, struct.pack("<IIIIIIII", TEXT, len(text), TEXT, 0x9, 3, 0, 0x1000, 0)); e.write(tab, bytes(8))
    e.call(0x413570, [base], conv="thiscall", this=tab, max_insns=300_000_000)
    n = e.u32(tab + 4); ents = e.u32(tab)
    out.append(f"F {name} {len(text)} {text.hex()}"); out.append(f"N {n}")
    keys = []
    for i in range(n):
        lo, hi, wav, tp = struct.unpack("<IIII", e.read(ents + 16 * i, 16)); keys.append((lo, hi))
        out.append(f"L {lo} {hi} {wav} {cstr(e, tp).hex() or '-'}")
    return n, ents, keys
def probes(tab, ents, keys, extra):
    for lo, hi in keys + extra:
        p = e.call(0x4138b0, [lo, hi], conv="thiscall", this=tab)
        out.append(f"P {lo} {hi} {(p - ents) // 16 if p else -1}")
order = ["OBJETOS.TXT", "ANIMS.TXT", "CONVERS.TXT", "ICONS.TXT"]
tabs = []
for i, nm in enumerate(order):
    tab = TAB + 0x10 * i; n, ents, keys = load(nm, files[nm], tab, 0x1000 + 0x100000 * i); tabs.append((tab, ents, n, keys))
    extra = [(rnd.getrandbits(30), rnd.getrandbits(30)) for _ in range(20)] + [(k[0], rnd.getrandbits(30)) for k in keys[:10]]
    probes(tab, ents, keys, extra); out.append("#")
for i, (tab, ents, n, keys) in enumerate(tabs): e.write(0x442110 + 4 * i, struct.pack("<I", tab))
names = []
for nm in order:
    for ln in files[nm].split(b"\n"):
        if b"," in ln: names.append(ln.split(b",")[0].strip())
random.Random(5).shuffle(names)
names = names[:600] + [b"NOEXISTE", b"ANM1OFF0", b"", b"ABCDEFGHIJKLMNOP", b"cnv01t02"]
for w in names:
    e.write(0x3000400, w + b"\0"); errors["n"] = 0
    p = e.call(0x413940, [0x3000400])
    t, idx = -1, 0
    for k, (tab, ents, n, keys) in enumerate(tabs):
        if p and ents <= p < ents + 16 * n: t, idx = k, (p - ents) // 16
    out.append(f"Q {w.hex() or '-'} {t} {idx}")
out.append("#")
def long_id(t):                 # walk the file like the loader does and report an ID over 19 characters (it overflows the original's ID buffer)
    pos = 0; n = len(t); ws = b" \t\n\v\f\r"
    while pos <= n - 8:
        while pos < n and t[pos] in ws: pos += 1
        q = pos
        while q < n and t[q] != 0x2c: q += 1
        if q - pos > 19: return True
        pos = q
        if pos < n and t[pos] == 0x2c:                                       # literal ',' then white space then literal '"' then the text
            pos += 1
            while pos < n and t[pos] in ws: pos += 1
            if pos < n and t[pos] == 0x22:
                pos += 1
                while pos < n and t[pos] != 0x22: pos += 1
        pos += 2
    return False
src = [files[n] for n in order]
for case in range(300):
    t = rnd.choice(src); lines = t.split(b"\n"); a = rnd.randrange(len(lines)); sl = b"\n".join(lines[a:a + rnd.randint(1, 40)]) + b"\n"
    sl = bytearray(sl)
    for _ in range(rnd.choice([0, 1, 2, 3])):
        k = rnd.randrange(len(sl)); m = rnd.random()
        if m < 0.35: del sl[k]
        elif m < 0.7: sl[k] = rnd.choice(b' ,"ab1_\r\n\xa8')
        else: sl[k:k] = bytes([rnd.choice(b' ,"a\n')])
        if not sl: sl = bytearray(b"A, \"x\"\r\n")
    if rnd.random() < 0.3: sl = bytearray(sl.replace(b"\r\n", b"\n"))
    if max(len(x) for x in bytes(sl).split(b'"')) > 900: continue        # the original overflows its 1000 byte line buffer: no meaning to compare
    if long_id(bytes(sl)): continue                  # an ID over 19 characters overflows the original's ID buffer into the next local
    mark = len(out); st["heap"] = HEAP + 0x800000
    try:
        n, ents, keys = load(f"FUZZ{case}", bytes(sl), TAB + 0x80, 0x900000 + 0x1000 * case)
        probes(TAB + 0x80, ents, keys[:30], [(rnd.getrandbits(30), 0)]); out.append("#")
    except Exception:
        del out[mark:]
# Crafted texts whose second line starts exactly eight bytes before the end (the loader stops one byte later), plus empty and one-line files.
for k, text in enumerate([b'A, "x"\r\nB, "y"\r', b'A, "x"\r\nB, "y"\r\n', b'A, "x"\r\nB, "y"', b'A, "x"\nB, "y"', b'AB, "x"\r\nC, "yz"\r', b'', b'A, "x"\r\n']):
    mark = len(out); st["heap"] = HEAP + 0x800000
    try:
        n, ents, keys = load(f"FUZZB{k}", text, TAB + 0x80, 0xa00000 + 0x1000 * k); probes(TAB + 0x80, ents, keys, []); out.append("#")
    except Exception: del out[mark:]
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/speech.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines;", sum(1 for l in out if l == "#"), "cases")
