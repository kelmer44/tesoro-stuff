#!/usr/bin/env python3
"""Export every function from the running Ghidra MCP bridge (read-only HTTP GETs).

Writes re/export/<addr>.c (decompiled), <addr>.asm (disassembly), <addr>.xrefs
(callers, from get xrefs_to) and re/export/functions.json (index).
Usage: python3 re/tools/export_ghidra.py [--base http://127.0.0.1:8089] [--out re/export]
"""
import argparse, json, re, urllib.parse, urllib.request
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

def get(base, path, **q):
    url = f"{base}/{path}?{urllib.parse.urlencode(q)}"
    with urllib.request.urlopen(url, timeout=120) as r:
        return r.read().decode("utf-8", "replace")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", default="http://127.0.0.1:8089")
    ap.add_argument("--out", default="re/export")
    a = ap.parse_args()
    out = Path(a.out); out.mkdir(parents=True, exist_ok=True)
    funcs = []
    text = get(a.base, "functions")  # "name at addr" lines
    for line in text.splitlines():
        m = re.match(r"(.+) at ([0-9a-fA-F]{8})$", line.strip())
        if m: funcs.append({"name": m.group(1), "addr": m.group(2).lower()})
    def work(f):
        ad = f["addr"]
        for kind, path, key in (("c", "decompile_function_by_address", "address"),
                                ("asm", "disassemble_function", "address")):
            try: t = get(a.base, path, **{key: ad})
            except Exception as e: t = f"// ERROR {e}"
            (out / f"{ad}.{kind}").write_text(t)
        x = []; off = 0
        while True:
            try: t = get(a.base, "xrefs_to", address=ad, offset=off, limit=200)
            except Exception as e: t = ""
            lines = [l for l in t.splitlines() if l.strip()]
            x += lines
            if len(lines) < 200: break
            off += 200
        (out / f"{ad}.xrefs").write_text("\n".join(x))
        return ad
    with ThreadPoolExecutor(8) as ex: list(ex.map(work, funcs))
    (out / "functions.json").write_text(json.dumps(funcs, indent=1))
    print(len(funcs), "functions exported to", out)
main()
