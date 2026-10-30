#!/usr/bin/env python3
"""Vectors for the script (.DLG) compiler: runs the ORIGINAL block loader (0x406190, which calls the block parser 0x404810)
and then the label resolver (0x406110) in Unicorn on the four shipped .DLG texts, on synthetic blocks and on 500 randomly
edited real blocks, and records every compiled block. Bytes the compiler never writes are found by running twice with
different stack fills (the parser keeps its commands in a stack array that is refilled before each block and each command).
Usage: gen_vectors_dlg.py [TESORO dir] (from the repository root); reads ANIMS/CONVERS/ICONS/OBJETOS.DLG from TESORO.TRE/.DAT (read-only).
Output: local-media/vectors/dlg.txt (ignored). Consumed by re/tools/drivers/dlg_driver.cpp.
Lines: F <name> <size> <hex text>   then per block  B <k0> <k1> <type> <vtable> | C <hex char keys> | L <hex labels> | E <hex entries> | M <mask>
       (mask 00 = byte the compiler never wrote, differs between two stack fills) and after the resolver  R <index> <hex entries>   # end of file
The phrase lookup 0x413940 is replaced by a keyed stand-in (it searches live levels): value = (k0 ^ k1 * 3 ^ 0x1234567) | 1."""
import os, struct, sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from emu import Emu

d = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
tre = open(d + "/TESORO.TRE", "rb").read(); dat = open(d + "/TESORO.DAT", "rb").read()
files = {}
for i in range(0, len(tre) - 27, 28):
    off, size, flags = struct.unpack_from("<IIB", tre, i)
    nm = tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()
    if nm.endswith(".DLG"): files[nm] = dat[off + 4:off + 4 + struct.unpack_from("<I", dat, off)[0]]

FILEP, MGR, HEAP, TEXT, TABLE = 0x3000200, 0x3000100, 0x2000000, 0x1000000, 0x3000800
e = Emu(); e.uc.mem_map(HEAP, 0x1000000, 3); e.uc.mem_map(TEXT, 0x100000, 3); e.uc.mem_map(0x3000000, 0x10000, 3)
st = {"heap": HEAP, "size": 0, "base": 0}
def alloc(emu, size, arena=0):
    a = st["heap"]; st["heap"] = (a + size + 7) & ~3; return a
def fread(emu, buf, size, count, fp): emu.write(buf, struct.pack("<I", st["size"])); return count
def fgetpos(emu, fp, out):
    ptr = e.u32(FILEP); emu.write(out, struct.pack("<II", st["base"] + (ptr - TEXT), 0)); return 0
def read_eof(emu, fd, buf, n): return 0
def name_key(b):                                  # 0x415c40: 5 bits per character, two 32-bit halves
    lo = hi = 0; n = min(len(b), 12)
    def ch(c): return (c + 0xb if c < 0x35 else c) & 0x1f     # signed char < '5' (bytes above 0x7f count as negative)
    def ch2(c): return ((c + 0xb) if (c < 0x35 or c >= 0x80) else c) & 0x1f
    for i in range(min(n, 6)): lo = ((lo << 5) | ch2(b[i])) & 0xffffffff
    if len(b) < 6: lo = ((lo << 5) << ((-5 * len(b) + 0x19) & 0x1f)) & 0xffffffff; return lo, 0
    for i in range(6, n): hi = ((hi << 5) | ch2(b[i])) & 0xffffffff
    if len(b) < 12: hi = ((hi << 5) << ((-5 * len(b) + 0x37) & 0x1f)) & 0xffffffff
    return lo, hi
def phrase(emu, s):
    b = bytearray(); a = s
    while True:
        c = emu.read(a, 1)
        if c == b"\0": break
        b += c; a += 1
    lo, hi = name_key(bytes(b)); return (lo ^ (hi * 3 & 0xffffffff) ^ 0x1234567) | 1
e.hook_function(0x40db40, alloc, 2); e.hook_function(0x41ba10, alloc, 1)
e.hook_function(0x403320, lambda emu, off: 0, 1, pops=True)
e.hook_function(0x41b4c0, fread, 4); e.hook_function(0x41b490, fgetpos, 2); e.hook_function(0x41cc90, read_eof, 3)
e.hook_function(0x413940, phrase, 1)
import random
for w in [b'A', b'JIM', b'CAPITAN', b'FANMMENU', b'nuevo', b'abcdefghijklm', b'_x1', b'\xe9a']:
    e.write(0x3000400, w + b'\0'); k = e.call(0x415c40, [0x3000400]); assert (k, e.edx_eax() >> 32) == name_key(w), w
for i in range(4): e.write(0x442130 + 4 * i, struct.pack("<I", 0x71000000 + 0x100 * i))   # levels for LEVEL%i
e.write(0x4421ec, struct.pack("<I", 0x72000000))                                          # inventory level
e.write(0x442064, struct.pack("<I", MGR)); e.write(MGR + 4, struct.pack("<I", FILEP))

out = []
STACK = (0x00200000 - 0x100000, 0x100000)
cur = {"fill": 0}
def refill(uc, addr, size, _):           # the parser's frame is reused for every block: make its unwritten bytes the fill again
    from unicorn.x86_const import UC_X86_REG_ESP
    esp = uc.reg_read(UC_X86_REG_ESP); uc.mem_write(esp - 0x4000, bytes([cur["fill"]]) * 0x3ff0)
from unicorn import UC_HOOK_CODE
e.uc.hook_add(UC_HOOK_CODE, refill, begin=0x404810, end=0x404810)
def refill_locals(uc, addr, size, _):    # scratch variables and the text buffer are reused by every command: make stale values the fill too
    from unicorn.x86_const import UC_X86_REG_ESP
    esp = uc.reg_read(UC_X86_REG_ESP); f = bytes([cur["fill"]])
    uc.mem_write(esp + 0x14, f * 0xc); uc.mem_write(esp + 0x24, f * (0x80 - 0x24))
e.uc.hook_add(UC_HOOK_CODE, refill_locals, begin=0x404929, end=0x404929)
def load_file(fill):
    cur["fill"] = fill
    e.write(STACK[0], bytes([fill]) * (STACK[1] - 0x2000))      # leave the top page (return slots) alone
    text = cur["text"]; e.write(TEXT, text); e.write(FILEP, struct.pack("<IIIIIIII", TEXT, len(text), TEXT, 0x9, 3, 0, 0x1000, 0))
    st["heap"] = HEAP; e.write(HEAP, bytes(0x100000))
    e.write(TABLE, bytes(0x2b0)); e.call(0x4060b0, [], conv="thiscall", this=TABLE)
    e.write(0x4420b8, struct.pack("<I", TABLE))
    try: e.call(0x406190, [st["base"]], conv="thiscall", this=TABLE, max_insns=400_000_000)
    except Exception:
        from unicorn.x86_const import UC_X86_REG_EIP, UC_X86_REG_EBP
        print("FAIL fill=%x eip=%x ebp=%x" % (fill, e.uc.reg_read(UC_X86_REG_EIP), e.uc.reg_read(UC_X86_REG_EBP)), file=sys.stderr); raise
def blocks():
    res = []
    for i in range(e.u32(TABLE + 0x2ac)):
        o = e.u32(TABLE + 0x54 + 4 * i); ne = e.u32(o + 0xc0)
        res.append((o, ne, e.read(e.u32(o + 0xbc), 24 * ne) if ne else b""))
    return res
errors = {"n": 0}
def message_box(emu, hwnd, text, caption, flags): errors["n"] += 1; return 0
e.stub_import(0x444360, message_box, 4)
def process(name, text, index):
    cur["text"] = text; st["size"] = len(text); st["base"] = 0x1000 + 0x100000 * index; errors["n"] = 0
    load_file(0x00); other = [raw for _, _, raw in blocks()]       # entries when unwritten bytes are 0x00
    load_file(0xa5)
    n = e.u32(TABLE + 0x2ac); bl = blocks()
    out.append(f"F {name} {len(text)} {text.hex()}")
    ptr2idx = {o: i + 1 for i, (o, _, _) in enumerate(bl)}
    def mask(a, b): return bytes(0xff if x == y else 0 for x, y in zip(a, b))
    def dump(tag):
        for i, (o, ne, raw) in enumerate(bl):
            if tag == "B":
                k0, k1, ty = e.u32(o + 0x44), e.u32(o + 0x48), e.u32(o + 0xd4); nc, nl = e.u32(o + 0xb8), e.u32(o + 0xc8)
                out.append(f"B {k0} {k1} {ty} {e.u32(o)}")
                out.append("C " + (e.read(e.u32(o + 0xb4), 8 * nc).hex() if nc else "-"))
                out.append("L " + (e.read(e.u32(o + 0xc4), 12 * nl).hex() if nl else "-"))
                out.append(f"E {i} " + (raw.hex() if ne else "-")); out.append(f"M {i} " + (mask(raw, other[i]).hex() if ne else "-"))
            else:
                if ne:
                    r = bytearray(raw)
                    for j in range(ne):                                    # run: block pointer -> block index + 1
                        if struct.unpack_from("<I", r, 24 * j)[0] == 8:
                            v = struct.unpack_from("<I", r, 24 * j + 4)[0]; struct.pack_into("<I", r, 24 * j + 4, ptr2idx.get(v, 0) if v else 0)
                    raw = bytes(r)
                out.append(f"R {i} " + (raw.hex() if ne else "-"))
    if errors["n"]: out.append("Z"); out.append("#"); return n       # the parser reported a bad line: only that is compared
    dump("B"); e.call(0x406110, [], conv="thiscall", this=TABLE, max_insns=50_000_000)
    bl = blocks(); dump("R")
    out.append("#"); return n
for index, name in enumerate(["ANIMS.DLG", "CONVERS.DLG", "ICONS.DLG", "OBJETOS.DLG"]):
    print(name, process(name, files[name], index), "blocks", file=sys.stderr)

# Synthetic blocks for what the shipped scripts never use (octal and hex numbers, LEVEL<n>, run FANMTRONO), then mutated real blocks.
synth = (b".TESTA 3\r\n2\r\nJIM\r\nDICK\r\n_start\r\nsendmessage 'OBJ' in LEVEL2 show on\r\nsendmessage 'OBJ' in LEVEL0 play 007 0x10 loop\r\n"
         b"run 'FANMTRONO' ir\r\nrun 'TESTA' start\r\nsendmessage 2 mirar ABAJO\r\nif v010 == -1 jmp start // c\r\nv010 = 010\r\nsleep 0x20\r\n"
         b"sendmessage 'X' in INVENTARIO play 1 2 stop\r\nplay sound 'SND1'\r\nplay cd 0x7\r\nsendmessage 1 ira -5 +6\r\n.END\r\n\r\n"
         b".FANMTRONO 20\r\n0\r\n_ir\r\nDO ir\r\nreturn\r\n.END\r\n\r\n")
process("SYNTH", synth, 5)
import random
rnd = random.Random(20261030)
blocks_text = []
for nm in files:
    t = files[nm]; starts = [i for i in range(len(t)) if t[i:i + 1] == b"." and (i == 0 or t[i - 1:i] == b"\n") and t[i + 1:i + 5] != b"END\r"]
    for k, st0 in enumerate(starts):
        en = t.find(b".END", st0); en = t.find(b"\n", en) + 1
        blocks_text.append(t[st0:en] + b"\r\n")
for case in range(500):
    t = bytearray(rnd.choice(blocks_text))
    for _ in range(rnd.choice([1, 1, 2, 3])):
        k = rnd.randrange(len(t)); m = rnd.random()
        if m < 0.3: del t[k]
        elif m < 0.6: t[k] = rnd.choice(b" abcdefimnprsuvxyz_'0123456789,.=/!-\r\n\t")
        elif m < 0.8: t[k:k] = bytes([rnd.choice(b" a_'0,\n9-")])
        else:
            lines = bytes(t).split(b"\n"); j = rnd.randrange(len(lines)); lines.insert(rnd.randrange(len(lines)), lines[j]); t = bytearray(b"\n".join(lines))
    try: process(f"FUZZ{case}", bytes(t), 6 + case)
    except Exception as ex:
        while out and not out[-1] == "#": out.pop()          # drop the half-written case
        print("skipped fuzz case", case, type(ex).__name__, file=sys.stderr)
os.makedirs("local-media/vectors", exist_ok=True)
open("local-media/vectors/dlg.txt", "w").write("\n".join(out) + "\n")
print(len(out), "lines")
