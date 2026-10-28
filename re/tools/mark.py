#!/usr/bin/env python3
"""Update hand-maintained ledger fields and regenerate the checklist.

  re/.venv/bin/python re/tools/mark.py 00415960 decompiled --name WinMain --note "launcher / game start"
  re/.venv/bin/python re/tools/mark.py 00415960 ported --engine "engines/tesoro/foo.cpp::bar"
Several addresses may be given before the status: mark.py 00401000 00401010 decompiled
Statuses: untouched decompiled understood ported emu-compared original-compared replaced
"""
import argparse, json, subprocess, sys
from pathlib import Path

STATUSES = ["untouched", "decompiled", "understood", "ported", "emu-compared", "original-compared", "replaced"]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("items", nargs="+", help="addresses (hex, no 0x) followed by a status")
    ap.add_argument("--name"); ap.add_argument("--note"); ap.add_argument("--engine")
    a = ap.parse_args()
    status, addrs = a.items[-1], [x.lower().removeprefix("0x").zfill(8) for x in a.items[:-1]]
    if status not in STATUSES: sys.exit(f"status must be one of {STATUSES}")
    p = Path(__file__).resolve().parent.parent / "ledger.json"
    rows = json.loads(p.read_text()); by = {r["addr"]: r for r in rows}
    for ad in addrs:
        if ad not in by: sys.exit(f"unknown function {ad}")
        r = by[ad]; r["status"] = status
        if a.name and len(addrs) == 1: r["name"] = a.name
        if a.note: r["notes"] = (r["notes"] + " | " if r["notes"] else "") + a.note
        if a.engine and len(addrs) == 1: r["engine"] = a.engine
    p.write_text(json.dumps(rows, indent=1))
    subprocess.run([sys.executable, str(Path(__file__).with_name("make_checklist.py"))], check=True)

main()
