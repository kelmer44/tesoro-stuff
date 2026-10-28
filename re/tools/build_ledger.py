#!/usr/bin/env python3
"""Build re/ledger.json (+ ledger.csv, clusters.md) from re/export (see export_ghidra.py).

Hash-guarded against the original executable. Per function: tier, callers, callees,
data references (vtable/function-pointer use), bottom-up depth, suggested cluster.
Never overwrites hand-maintained fields (status, name, notes, engine) of an existing ledger.
"""
import csv, hashlib, json, re, struct, sys
from pathlib import Path

EXE = Path("/Users/gabriel/Desktop/ags/TESORO/TESORO.EXE")
SHA = "0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef"
GAME_END = 0x41a070           # DirectDrawCreate thunks start; everything after is CRT/runtime
CLUSTER_MAX = 12

def main():
    raw = EXE.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == SHA, "wrong executable"
    exp = Path("re/export")
    funcs = json.loads((exp / "functions.json").read_text())
    byaddr = {int(f["addr"], 16): f for f in funcs}
    for a, f in byaddr.items():
        f.update(callers=set(), callees=set(), data_refs=[])
    pat = re.compile(r"From ([0-9a-f]{8}) in (\S+) \[(\w+)\]")
    for a, f in byaddr.items():
        txt = (exp / f"{f['addr']}.xrefs").read_text()
        for m in pat.finditer(txt):
            src, kind = int(m.group(1), 16), m.group(3)
            owner = max((x for x in byaddr if x <= src), default=None)
            if "CALL" in kind or "JUMP" in kind:
                if owner is not None and owner != a:
                    f["callers"].add(owner); byaddr[owner]["callees"].add(a)
            else:
                f["data_refs"].append({"from": f"{src:08x}", "kind": kind})
        c = (exp / f"{f['addr']}.c").read_text()
        f["lines"] = c.count("\n")
        f["decompile_ok"] = "Decompilation failed" not in c and len(c) > 20
    # PE sections: scan data sections for dwords that equal a function entry (vtables / callback tables)
    pe = struct.unpack_from("<I", raw, 0x3c)[0]
    nsec = struct.unpack_from("<H", raw, pe + 6)[0]
    opt = struct.unpack_from("<H", raw, pe + 20)[0]
    tables = []
    for i in range(nsec):
        o = pe + 24 + opt + 40 * i
        name = raw[o:o+8].rstrip(b"\0").decode()
        vsz, va, rsz, rptr = struct.unpack_from("<IIII", raw, o + 8)
        if name not in (".rdata", ".data"): continue
        run = []
        for off in range(0, min(vsz, rsz) - 3, 4):
            v = struct.unpack_from("<I", raw, rptr + off)[0]
            if v in byaddr: run.append((0x400000 + va + off, v))
            else:
                if len(run) >= 2: tables.append(run)
                run = []
        if len(run) >= 2: tables.append(run)
    tid = {}
    for n, run in enumerate(tables):
        for slot, fa in run:
            byaddr[fa].setdefault("tables", []).append({"table": f"{run[0][0]:08x}", "slot": (slot - run[0][0]) // 4})
    # bottom-up depth via SCC condensation (iterative Tarjan)
    idx, low, onst, st, comp, cnt = {}, {}, set(), [], {}, [0]
    def scc(root):
        work = [(root, iter(sorted(byaddr[root]["callees"])))]
        idx[root] = low[root] = cnt[0]; cnt[0] += 1; st.append(root); onst.add(root)
        while work:
            v, it = work[-1]
            for w in it:
                if w not in idx:
                    idx[w] = low[w] = cnt[0]; cnt[0] += 1; st.append(w); onst.add(w)
                    work.append((w, iter(sorted(byaddr[w]["callees"])))); break
                elif w in onst: low[v] = min(low[v], idx[w])
            else:
                work.pop()
                if work: low[work[-1][0]] = min(low[work[-1][0]], low[v])
                if low[v] == idx[v]:
                    while True:
                        w = st.pop(); onst.discard(w); comp[w] = v
                        if w == v: break
    for a in sorted(byaddr):
        if a not in idx: scc(a)
    depth = {}
    def d(a):
        c = comp[a]
        if c in depth: return depth[c]
        depth[c] = 0
        members = [x for x in byaddr if comp[x] == c]
        ds = [d(w) + 1 for m in members for w in byaddr[m]["callees"] if comp[w] != c]
        depth[c] = max(ds, default=0)
        return depth[c]
    sys.setrecursionlimit(10000)
    for a in byaddr: d(a)
    # suggested clusters: contiguous game-code blocks, split where fewest call edges cross
    game = [a for a in sorted(byaddr) if a < GAME_END]
    def cross(i):  # edges between game[:i] and game[i:]
        left = set(game[:i]); return sum(1 for a in game[max(0, i-CLUSTER_MAX):i] for w in byaddr[a]["callees"] if w >= game[i]) + \
                                      sum(1 for a in game[i:i+CLUSTER_MAX] for w in byaddr[a]["callees"] if w < game[i] and w in left)
    cl, i, n = {}, 0, 1
    while i < len(game):
        end = min(i + CLUSTER_MAX, len(game))
        if end < len(game):
            end = min(range(max(i + 4, end - 5), end + 1), key=lambda j: (cross(j), -j)) if end - 5 > i else end
        for a in game[i:end]: cl[a] = n
        i, n = end, n + 1
    old = {}
    lp = Path("re/ledger.json")
    if lp.exists(): old = {r["addr"]: r for r in json.loads(lp.read_text())}
    rows = []
    for a in sorted(byaddr):
        f = byaddr[a]; o = old.get(f["addr"], {})
        tier = "game" if a < GAME_END and a not in range(0x41a070, 0x41a080) else "runtime"
        if f["name"] in ("DirectDrawCreate", "DirectInputCreateA", "Ordinal_1"): tier = "import-thunk"
        rows.append(dict(addr=f["addr"], ghidra_name=f["name"], name=o.get("name", ""), tier=tier,
            lines=f["lines"], decompile_ok=f["decompile_ok"], depth=depth[comp[a]],
            callers=[f"{x:08x}" for x in sorted(f["callers"])], callees=[f"{x:08x}" for x in sorted(f["callees"])],
            tables=f.get("tables", []), data_refs=f["data_refs"],
            cluster=cl.get(a), status=o.get("status", "untouched"), engine=o.get("engine", ""), notes=o.get("notes", "")))
    lp.write_text(json.dumps(rows, indent=1))
    with open("re/ledger.csv", "w", newline="") as fh:
        w = csv.writer(fh); w.writerow(["addr","name","tier","depth","cluster","lines","callers","callees","in_table","status"])
        for r in rows: w.writerow([r["addr"], r["name"] or r["ghidra_name"], r["tier"], r["depth"], r["cluster"], r["lines"], len(r["callers"]), len(r["callees"]), bool(r["tables"]), r["status"]])
    print(len(rows), "functions;", sum(r["tier"]=="game" for r in rows), "game;", len(tables), "pointer tables;", n-1, "clusters")
main()
