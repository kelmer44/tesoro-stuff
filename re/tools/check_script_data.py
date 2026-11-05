#!/usr/bin/env python3
"""Static consistency check of the shipped scripts against the shipped levels: every object a script names with `sendmessage '<NAME>'`, every block a `run '<BLOCK>' <label>`
starts (and that label), every movie a `play avi N` names must exist. Name keys are computed by the original's own name hash (0x415c40, Unicorn); object names
come from the levels' object records (local-media/vectors/reallevels.txt, written by gen_vectors_reallevels.py: run that first).
Usage: check_script_data.py [TESORO folder]"""
import os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from emu import Emu
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EDX
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
folder = sys.argv[1] if len(sys.argv) > 1 else "/Users/gabriel/Desktop/ags/TESORO"
e = Emu(); e.uc.mem_map(0x3000000, 0x1000, 3)
def key(name):
    e.write(0x3000000, name.upper().encode("latin1") + b"\0"); e.call(0x415c40, [0x3000000])
    return (e.uc.reg_read(UC_X86_REG_EDX) << 32) | e.uc.reg_read(UC_X86_REG_EAX)
have = set(); where = {}
for line in open("local-media/vectors/reallevels.txt"):
    if line.startswith("R "): level = line.split()[1]
    elif line.startswith("= "):
        for m in re.finditer(r"\| obj (\d+) (\d+) ", line):
            k = int(m[1]) | (int(m[2]) << 32); have.add(k); where.setdefault(k, set()).add(level)
tre = open(folder + "/TESORO.TRE", "rb").read(); dat = open(folder + "/TESORO.DAT", "rb"); ents = {}
for i in range(0, len(tre) - 27, 28):
    off, _, fl = struct.unpack_from("<IIB", tre, i)
    if fl & 4: ents[tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()] = off
blocks = {}; refs = []
for f in ("ANIMS.DLG", "CONVERS.DLG", "ICONS.DLG", "OBJETOS.DLG"):
    dat.seek(ents[f]); size = struct.unpack("<I", dat.read(4))[0]; text = dat.read(size).decode("latin1"); cur = None
    for line in text.split("\n"):
        line = line.split("//")[0].strip()
        if line.startswith(".") and not line.startswith(".END"): cur = line[1:].split()[0].upper(); blocks.setdefault(cur, set())
        elif line.startswith("_") and cur: blocks[cur].add(line[1:].split()[0].upper())
        elif cur:
            for m in re.finditer(r"sendmessage\s+'([^']+)'", line): refs.append(("object", m[1], cur))
            m = re.match(r"run\s+'([^']+)'\s+(\S+)", line)
            if m: refs.append(("run", (m[1].upper(), m[2].upper()), cur))
            m = re.match(r"play\s+avi\s+(\d+)", line)
            if m: refs.append(("avi", int(m[1]), cur))
missing = {"object": {}, "block": {}, "label": {}, "avi": {}}
for kind, what, blk in refs:
    if kind == "object" and key(what) not in have: missing["object"].setdefault(what, set()).add(blk)
    elif kind == "run":
        b, l = what
        if b not in blocks: missing["block"].setdefault(b, set()).add(blk)
        elif l not in blocks[b]: missing["label"].setdefault(f"{b} {l}", set()).add(blk)
    elif kind == "avi" and not os.path.exists(f"{folder}/bin/data{what:02d}.bin"): missing["avi"].setdefault(str(what), set()).add(blk)
print(f"{len(blocks)} blocks, {sum(len(v) for v in blocks.values())} labels, {len(refs)} references, {len(have)} distinct object names in the levels")
for k, v in missing.items():
    print(f"{k}s mentioned but not found: {len(v)}")
    for n, b in sorted(v.items())[:25]: print(f"   {n}   (in {', '.join(sorted(b))[:80]})")
