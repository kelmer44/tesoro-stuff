#!/usr/bin/env python3
"""Generate re/CHECKLIST.md from re/ledger.json (run automatically by mark.py)."""
import json
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ORDER = ["untouched", "decompiled", "understood", "ported", "emu-compared", "original-compared", "replaced"]
DONE = {"ported", "emu-compared", "original-compared", "replaced"}   # box checked
RANK = {s: i for i, s in enumerate(ORDER)}

rows = json.loads((ROOT / "ledger.json").read_text())
game = [r for r in rows if r["tier"] == "game"]
other = [r for r in rows if r["tier"] != "game"]
cnt = lambda rs, s: sum(r["status"] == s for r in rs)
at_least = lambda rs, s: sum(RANK[r["status"]] >= RANK[s] and r["status"] != "replaced" or r["status"] == "replaced" for r in rs)

def box(r): return "x" if r["status"] in DONE else " "
def label(r):
    n = r["name"] or r["ghidra_name"]
    return f"`{r['addr']}` {n}"

out = ["# Function checklist", "",
       "Generated from `ledger.json` by `tools/make_checklist.py`. Update with `tools/mark.py` (see its docstring).",
       "`[x]` means a C++ port exists (status ported or later). The status after each entry says how far it got and "
       "how it was checked: decompiled < understood < ported < emu-compared < original-compared.", ""]
tot = len(game)
out.append(f"## Overall (game code: {tot} functions)")
out.append("")
out.append("| Stage | Count | Share |"); out.append("|---|---|---|")
for s in ORDER[:-1]:
    k = sum(RANK[r["status"]] >= RANK[s] for r in game if r["status"] != "replaced")
    k += cnt(game, "replaced") if s != "untouched" else cnt(game, "replaced")
    out.append(f"| {s} or later | {k} | {100*k//tot if tot else 0}% |")
out.append(f"| replaced by a ScummVM service | {cnt(game, 'replaced')} | |")
out += ["", "## Game code by cluster", ""]
cl = defaultdict(list)
for r in game: cl[r["cluster"]].append(r)
for c in sorted(cl, key=lambda x: (x is None, x)):
    rs = sorted(cl[c], key=lambda r: (r["depth"], r["addr"]))
    d = sum(r["status"] in DONE for r in rs)
    out.append(f"### Cluster {c}: {rs[0]['addr']}–{max(r['addr'] for r in rs)} ({d}/{len(rs)} ported)")
    out.append("")
    for r in rs:
        extra = f" — {r['notes']}" if r["notes"] else ""
        eng = f" → `{r['engine']}`" if r["engine"] else ""
        vt = " (vtable)" if r["tables"] else ""
        out.append(f"- [{box(r)}] {label(r)}{vt} · depth {r['depth']} · {r['lines']} lines · **{r['status']}**{eng}{extra}")
    out.append("")
out += ["## Runtime, CRT and import thunks", "",
        f"{len(other)} functions (C runtime, SEH helpers, import stubs). They are not ported one by one; mark "
        "`replaced` when a ScummVM service covers them or `understood` when the game depends on their behaviour.", ""]
for r in other:
    out.append(f"- [{box(r)}] {label(r)} · {r['tier']} · **{r['status']}**")
(ROOT / "CHECKLIST.md").write_text("\n".join(out) + "\n")
print("checklist:", tot, "game functions;", sum(r["status"] in DONE for r in game), "done")
