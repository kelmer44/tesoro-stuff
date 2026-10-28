#!/usr/bin/env python3
"""Phase 1: find code Ghidra did not turn into functions.

Inputs: original EXE (hash-checked), re/export/*.asm (covered ranges). Output: re/missing_functions.json with
candidates and the evidence for each (data pointers, code immediates, call/jmp targets, gap scan).
"""
import hashlib, json, re, struct
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

EXE = Path("/Users/gabriel/Desktop/ags/TESORO/TESORO.EXE")
SHA = "0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef"
BASE = 0x400000
d = EXE.read_bytes(); assert hashlib.sha256(d).hexdigest() == SHA
pe = struct.unpack_from("<I", d, 0x3c)[0]; n = struct.unpack_from("<H", d, pe + 6)[0]; opt = struct.unpack_from("<H", d, pe + 20)[0]
secs = {}
for i in range(n):
    o = pe + 24 + opt + 40 * i
    nm = d[o:o+8].rstrip(b"\0").decode(); vs, va, rs, rp = struct.unpack_from("<IIII", d, o + 8)
    secs[nm] = (BASE + va, vs, rp, rs)
tva, tvs, trp, trs = secs[".text"]
TEXT = (tva, tva + tvs)
def off(va):
    for nm, (sva, vs, rp, rs) in secs.items():
        if sva <= va < sva + vs: return rp + va - sva
def in_text(v): return TEXT[0] <= v < TEXT[1]

# 1. covered ranges from the export
funcs = json.loads(Path("re/export/functions.json").read_text())
starts = sorted(int(f["addr"], 16) for f in funcs)
ranges = []
for f in funcs:
    a = int(f["addr"], 16); last = a
    for line in Path(f"re/export/{f['addr']}.asm").read_text().splitlines():
        m = re.match(r"([0-9a-f]{8}):", line)
        if m: last = max(last, int(m.group(1), 16))
    ranges.append((a, last))
ranges.sort()
def covering(v):
    for a, b in ranges:
        if a <= v <= b + 15 and v >= a: return (a, b)  # +15: last instruction length slack
    return None
fset = set(starts)

cand = {}
def add(v, why):
    if not in_text(v) or v in fset: return
    cand.setdefault(v, []).append(why)

# 2. pointers in data sections (and .text jump tables are inside functions, so only data)
for nm in (".rdata", ".data"):
    sva, vs, rp, rs = secs[nm]
    for o in range(0, min(vs, rs) - 3, 4):
        v = struct.unpack_from("<I", d, rp + o)[0]
        add(v, f"data pointer at {sva+o:08x} ({nm})")

# 3. code immediates, call and jmp targets over the whole of .text (linear sweep is unreliable on data in
#    code, so only instructions inside known function ranges are used)
md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = False
for a, b in ranges:
    o = off(a)
    for i in md.disasm(d[o:o + (b - a) + 16], a):
        if i.address > b: break
        mn, ops = i.mnemonic, i.op_str
        if mn in ("call", "jmp") and re.fullmatch(r"0x[0-9a-f]+", ops):
            add(int(ops, 16), f"{mn} from {i.address:08x}")
        elif mn in ("push", "mov") and re.search(r"0x4[0-2][0-9a-f]{5}\b", ops) and "[" not in ops.split(",")[-1] or (mn == "push" and re.fullmatch(r"0x4[0-2][0-9a-f]{5}", ops)):
            m = re.search(r"0x(4[0-2][0-9a-f]{5})\b", ops)
            if m: add(int(m.group(1), 16), f"immediate in {mn} at {i.address:08x}")

# 4. split candidates: inside an existing function body => a label/jump target, not a function
res = []
for v in sorted(cand):
    c = covering(v)
    kind = "inside-function" if c else "uncovered"
    res.append({"addr": f"{v:08x}", "kind": kind, "inside": f"{c[0]:08x}-{c[1]:08x}" if c else None, "evidence": cand[v][:4], "n": len(cand[v])})

# 5. uncovered gaps in .text: report spans of bytes not inside any function range
gaps = []; cur = TEXT[0]
for a, b in ranges:
    if a > cur: gaps.append((cur, a))
    cur = max(cur, b + 1)
if cur < TEXT[1]: gaps.append((cur, TEXT[1]))
gaps = [(g0, g1) for g0, g1 in gaps if g1 - g0 > 0]
out = {"text": [f"{TEXT[0]:08x}", f"{TEXT[1]:08x}"], "functions": len(funcs), "candidates": res,
       "gaps": [{"from": f"{a:08x}", "to": f"{b:08x}", "size": b - a, "bytes": d[off(a):off(a)+min(b-a,24)].hex()} for a, b in gaps]}
Path("re/missing_functions.json").write_text(json.dumps(out, indent=1))
unc = [r for r in res if r["kind"] == "uncovered"]
print(f"{len(res)} candidates: {len(unc)} uncovered, {len(res)-len(unc)} inside existing functions; {len(gaps)} gaps, {sum(g['size'] for g in out['gaps'])} bytes")
