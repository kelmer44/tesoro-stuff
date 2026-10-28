#!/usr/bin/env python3
"""Phase 1 step 2: turn gap/pointer evidence into a verified list of new function starts.

Reads re/missing_functions.json and re/gaps.json (find_missing_functions.py + gap scan), decodes each candidate
with Capstone (no invalid opcodes, reaches a ret within 4 KB, branches stay sane), writes re/new_functions.json.
MSVC aligns function starts to 16 bytes; non-aligned starts are accepted only with data-pointer or call evidence.
"""
import json, re
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_RET, CS_GRP_JUMP

d = Path("/Users/gabriel/Desktop/ags/TESORO/TESORO.EXE").read_bytes()
off = lambda va: 0x400 + va - 0x401000
md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
TEXT = (0x401000, 0x4271ff)
funcs = {int(f["addr"], 16) for f in json.loads(Path("re/export/functions.json").read_text())}
miss = json.loads(Path("re/missing_functions.json").read_text())
gaps = json.loads(Path("re/gaps.json").read_text())

def plausible(a):
    """Decode linearly from a; return (ok, n_instr, end) where ok means it reaches a ret cleanly."""
    n, p, rets, seen_ret = 0, a, 0, False
    maxtarget = a
    while p < TEXT[1] and n < 2000:
        ins = list(md.disasm(d[off(p):off(p) + 15], p, 1))
        if not ins: return (False, n, p)
        i = ins[0]; n += 1
        if i.mnemonic in ("int3", "hlt", "in", "out", "insb", "insd", "outsb", "outsd", "lodsd", "aam", "aad", "sahf", "lahf", "salc", "arpl", "bound"):
            return (False, n, p)
        if CS_GRP_JUMP in i.groups and re.fullmatch(r"0x[0-9a-f]+", i.op_str):
            t = int(i.op_str, 16)
            if not (TEXT[0] <= t < TEXT[1]): return (False, n, p)
            if i.mnemonic != "jmp" or t > p: maxtarget = max(maxtarget, t)
        if CS_GRP_RET in i.groups:
            if p >= maxtarget: return (True, n, p + i.size)
        if i.mnemonic == "jmp" and re.fullmatch(r"0x[0-9a-f]+", i.op_str) and p >= maxtarget and int(i.op_str, 16) < a:
            return (True, n, p + i.size)  # tail jump out of the function
        p += i.size
        if p in funcs and p > a: return (True, n, p)   # fell into a known function: treat as boundary
    return (False, n, p)

cands = {}
for c in miss["candidates"]:
    if c["kind"] == "uncovered":
        a = int(c["addr"], 16); cands[a] = {"src": "pointer/call", "evidence": c["evidence"][:3]}
for g in gaps:
    a0, b0 = int(g["from"], 16), int(g["to"], 16)
    start = (a0 + 15) & ~15
    for p in range(start, b0, 16):
        if p in cands or p in funcs: continue
        ok, n, end = plausible(p)
        if ok and n >= 3 and end <= b0 + 16:
            cands[p] = {"src": "gap-scan", "evidence": [f"{n} instructions to ret at {end:08x}"]}
res, rej = [], []
for a in sorted(cands):
    ok, n, end = plausible(a)
    row = {"addr": f"{a:08x}", "src": cands[a]["src"], "evidence": cands[a]["evidence"], "instr": n, "end": f"{end:08x}", "decodes": ok}
    (res if ok else rej).append(row)
Path("re/new_functions.json").write_text(json.dumps({"accepted": res, "rejected": rej}, indent=1))
print(len(res), "accepted,", len(rej), "rejected;", sum(r["src"] == "pointer/call" for r in res), "from pointers,", sum(r["src"] == "gap-scan" for r in res), "from gap scan")
for r in rej[:20]: print("rejected", r["addr"], r["src"], r["evidence"][0][:50], "stopped at", r["end"], "after", r["instr"])
