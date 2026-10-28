#!/usr/bin/env python3
"""Apply a TSV of names to the ledger: addr<TAB>name<TAB>status<TAB>note (lines starting # ignored).
Only fills name/status/notes; never lowers a status that is further along. Then regenerates the checklist."""
import json, subprocess, sys
from pathlib import Path
ORDER = ["untouched", "decompiled", "understood", "ported", "emu-compared", "original-compared", "replaced"]
root = Path(__file__).resolve().parent.parent
rows = json.loads((root / "ledger.json").read_text()); by = {r["addr"]: r for r in rows}
for path in sys.argv[1:]:
    for line in Path(path).read_text().splitlines():
        if not line.strip() or line.startswith("#"): continue
        ad, name, st, note = (line.split("\t") + ["", "", "", ""])[:4]
        r = by[ad.zfill(8)]; r["name"] = name
        if st and (st == "replaced" or ORDER.index(st) > ORDER.index(r["status"])): r["status"] = st
        if note and note not in r["notes"]: r["notes"] = (r["notes"] + " | " if r["notes"] else "") + note
(root / "ledger.json").write_text(json.dumps(rows, indent=1))
subprocess.run([sys.executable, str(root / "tools" / "make_checklist.py")], check=True)
