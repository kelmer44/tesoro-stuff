#!/usr/bin/env python3
"""Phase 3: derive the class model from the fresh exports.

Inputs : re/ledger.json, re/export/*.c, re/vtable_map.json, re/object_kinds.json
Outputs: re/classes.json, re/STRUCTS_GENERATED.md, re/types/tesoro_types.h
Method : every vtable base is a class; the functions that store it are its constructors (or the factory for
subtype classes); the parent is the vtable stored by the first constructor it calls; field accesses are collected
from the constructor and from the class's own virtual methods (slots not inherited unchanged from the parent).
Confidence: c = written in a constructor, w = written and read, r = read only. Widths come from the decompiler's
access type, so a field read as dword in one place and word in another appears twice.
"""
import re, json, glob, collections
from pathlib import Path
R = Path(__file__).resolve().parent.parent
L = {f["addr"]: f for f in json.loads((R / "ledger.json").read_text())}
VM = json.loads((R / "vtable_map.json").read_text()); VM.pop("00424146", None)
OK = json.loads((R / "object_kinds.json").read_text())
SRC = {a: (R / "export" / f"{a}.c").read_text() for a in L if (R / "export" / f"{a}.c").exists()}

# ---- who stores which vtable (ordered) ------------------------------------------------------
STORE = re.compile(r"(?:\*\((?:undefined \*\*\*|undefined4 \*|undefined \*\*)\)(?:this|param_1|\(\(int\)this \+ 0\))|\*(?:param_1|puVar\d+|this_00|this|piVar\d+)) = &PTR_(?:FUN|LAB)_([0-9a-f]{8})")
stores = collections.defaultdict(list)   # func -> [vtable...] in source order
assigners = collections.defaultdict(list)
for a, c in SRC.items():
    for m in STORE.finditer(c):
        v = m.group(1)
        if v in VM: stores[a].append(v); assigners[v].append(a)

# ---- class names ----------------------------------------------------------------------------
NAMES = {"00428000": "Obj_Kind00", "0042ba68": "Obj_MidBase", "0042bad8": "Obj_Kind09Family", "0042bb48": "Obj_Kind05Family",
         "00428b70": "Jim", "00428038": "Cursor", "00428ac8": "Obj_Kind01", "00428070": "Keyboard", "00428074": "Mouse",
         "00428bf8": "Level", "00428078": "DlgScriptEntry", "0042806c": "InputDevice", "0042b9c0": "Obj_Kind03", "0042b9f8": "Obj_Kind04",
         "0042bd60": "WalkArea", "0042bcb0": "Obj_Kind02", "0042bc78": "Obj_Kind07", "0042bce8": "Obj_Kind06Base", "0042bdd0": "Obj_Kind14",
         "0042beb0": "Obj_Kind15", "0042ba30": "Obj_Kind16", "0042bf90": "Obj_Kind19", "0042c070": "Obj_Kind21", "0042b950": "Obj_Kind22",
         "0042b8e0": "Obj_Kind23", "0042bf20": "Obj_Kind24", "0042c000": "Obj_Kind25", "0042be40": "Obj_Kind11", "00428b00": "Obj_Kind13Base",
         "0042be08": "TopObject", "0042bc40": "InterfaceLevel", "0042bbd0": "CubiertaLevel", "0042bc08": "MapaLevel"}
KIND = {}      # vtable -> "kind K sub S"
CTOR_OF_KIND = {}
for k, v in OK.items():
    if v["ctor"]: CTOR_OF_KIND[int(k)] = v["ctor"]
    for s, x in v["sub"].items():
        if "vtable" in x: KIND[x["vtable"]] = (int(k), s, v["ctor"], x.get("size"))
    if not v["sub"] and v["ctor"]:
        pass
SIZE = {"00428b70": 0x114, "00428038": 0x130, "00428000": 0x6c}
for k, v in OK.items():
    if v["ctor"] and v["ctor"] in stores:
        vt = stores[v["ctor"]][-1]
        if v["size"]: SIZE.setdefault(vt, v["size"])
for vt, (k, s, ctor, size) in KIND.items():
    if size: SIZE.setdefault(vt, size)
def cname(vt):
    if vt in NAMES: return NAMES[vt]
    if vt in KIND:
        k, s, _, _ = KIND[vt]; return f"Obj_Kind{k:02d}_Sub{s}"
    return f"Cls_{vt}"

# ---- parents ----------------------------------------------------------------------------------
def final_vt(fn): return stores[fn][-1] if stores.get(fn) else None
parent = {}
for vt in VM:
    if vt in KIND:                                   # subtype class: parent = vtable left by the kind constructor
        ctor = KIND[vt][2]; pv = final_vt(ctor); parent[vt] = pv if pv != vt else None; continue
    cands = [a for a in assigners.get(vt, []) if a != "00409e50" and a != "00413e60"]
    for a in cands:
        for cal in L[a]["callees"]:
            if cal in stores and cal != a:
                pv = final_vt(cal) if len(stores[cal]) else None
                # take the first vtable stored by the callee that is not ours
                pv = next((x for x in reversed(stores[cal]) if x != vt), None)
                if pv: parent.setdefault(vt, pv); break
        if vt in parent: break
    parent.setdefault(vt, None)
for vt in list(parent):
    if parent[vt] == vt: parent[vt] = None
# a constructor that stores several vtables in sequence has inlined its base constructors: each is the parent of the next
for fn, vts in stores.items():
    if fn in ("00409e50", "00413e60", "00406190"): continue
    seq = [v for i, v in enumerate(vts) if i == 0 or v != vts[i - 1]]
    for a, b in zip(seq, seq[1:]):
        if a != b: parent[b] = a
# vtables stored by InitGameWorld over a Level object (the Level constructor is 004080e0): per-level subclasses
for vt in stores.get("00413e60", []):
    if vt != "00428bf8": parent[vt] = "00428bf8"; SIZE.setdefault(vt, 0x9c)
SIZE.setdefault("00428bf8", 0x9c)

# ---- field access extraction ------------------------------------------------------------------
WID = {"undefined4": 4, "uint": 4, "int": 4, "ulonglong": 8, "undefined8": 8, "undefined2": 2, "ushort": 2, "short": 2, "undefined1": 1,
       "byte": 1, "char": 1, "bool": 1, "DWORD": 4, "LONG": 4, "float": 4, "undefined": 1}
def this_var(src):
    first = src.split("{")[0]
    if "__thiscall" in first and "this" in first: return "this", True
    if "__fastcall" in first and "param_1" in first: return "param_1", True
    return None, False
def var_scale(src, v):
    """Element size when `v + N` is used without an (int) cast: pointer parameters scale N by the pointee size."""
    first = src.split("{")[0]
    m = re.search(r"(\w+) (\*+)\s*" + v + r"\b", first)
    if m and m.group(1) == "void": return 1
    if m:
        return {"undefined4": 4, "int": 4, "uint": 4, "undefined1": 1, "byte": 1, "char": 1, "undefined2": 2, "short": 2, "ushort": 2, "undefined": 1}.get(m.group(1), 4)
    return 1
def accesses(src):
    v, ok = this_var(src)
    if not ok: return []
    out = []
    sc = var_scale(src, v)
    # explicit byte offsets: *(T *)((int)v + N)
    pat1 = re.compile(r"\*\((\w+) ?(\*+) ?\)\(\(int\)" + v + r" \+ (0x[0-9a-f]+|\d+)\)( ?[|&+\-]?= |)")
    # pointer arithmetic: *(T *)(v + N) scaled by the pointee size of v
    pat2 = re.compile(r"\*\((\w+) ?(\*+) ?\)\(" + v + r" \+ (0x[0-9a-f]+|\d+)\)( ?[|&+\-]?= |)")
    for pat, scale in ((pat1, 1), (pat2, sc)):
        for m in pat.finditer(src):
            t, ptr = m.group(1), m.group(2)
            w = 4 if len(ptr) > 1 else WID.get(t, 4)
            off = int(m.group(3), 0) * scale
            out.append((off, w, m.group(4).strip() in ("=", "|=", "&=", "+=", "-=")))
    first = src.split("{")[0]
    if re.search(r"(?:int|undefined4|uint) \*\s*" + v + r"\b", first):
        for m in re.finditer(r"\b" + v + r"\[(0x[0-9a-f]+|\d+)\]( = |)", src):
            out.append((int(m.group(1), 0) * 4, 4, m.group(2) != ""))
    return out

def own_methods(vt):
    slots = VM[vt]; pv = parent.get(vt); ps = VM.get(pv, []) if pv else []
    own = []
    for i, a in enumerate(slots):
        if i < len(ps) and ps[i] == a: continue
        if a in L and L[a]["tier"] == "game": own.append((i, a))
    return own
classes = {}
for vt in sorted(VM):
    ctors = [a for a in assigners.get(vt, []) if a in L]
    methods = [a for _, a in own_methods(vt)]
    fields = {}
    for src_fn, is_ctor in [(a, True) for a in ctors if a not in ("00409e50", "00413e60", "00406190")] + [(a, False) for a in methods]:
        for off, w, wr in accesses(SRC.get(src_fn, "")):
            e = fields.setdefault((off, w), {"off": off, "w": w, "reads": 0, "writes": 0, "ctor": False, "fns": set()})
            e["writes" if wr else "reads"] += 1; e["ctor"] |= bool(is_ctor and wr); e["fns"].add(src_fn)
    fl = []
    for e in sorted(fields.values(), key=lambda e: (e["off"], e["w"])):
        conf = "c" if e["ctor"] else ("w" if e["writes"] and e["reads"] else ("r" if e["reads"] else "w?"))
        fl.append({"off": e["off"], "width": e["w"], "conf": conf, "reads": e["reads"], "writes": e["writes"], "fns": sorted(e["fns"])})
    classes[vt] = {"vtable": vt, "name": cname(vt), "parent": parent.get(vt), "slots": VM[vt], "size": SIZE.get(vt),
                   "kind": f"kind {KIND[vt][0]} subtype {KIND[vt][1]}" if vt in KIND else None,
                   "constructors": ctors, "own_methods": [{"slot": i, "fn": a} for i, a in own_methods(vt)], "fields": fl}
(R / "classes.json").write_text(json.dumps(classes, indent=1))

FN = json.loads((R / "field_names.json").read_text()) if (R / "field_names.json").exists() else {}
ROLE = {0:'OnAttach',2:'AnimTick',3:'UpdateBounds',4:'Update',5:'Update2',6:'Draw',8:'SetActive',9:'LoadState',10:'SaveState',11:'IsHitter',12:'OnTouched',13:'RunVerb',14:'SetVerbAndClick',16:'IsInteractive',17:'ShowHoverText',18:'ClearHoverText',19:'OnClick',20:'Verb0',21:'Verb1',22:'Verb3',23:'Verb2',24:'Verb4',25:'Verb5',26:'Verb6',27:'UseItem'}
def field_name(vt, off):
    v = vt; seen = set()
    while v and v not in seen:
        n = FN.get(v, {}).get(hex(off))
        if n: return n
        seen.add(v); v = classes[v]["parent"] if v in classes else None
    return None
# ---- markdown -------------------------------------------------------------------------------------
def chain(vt):
    out = []; seen = set()
    while vt and vt not in seen: out.append(vt); seen.add(vt); vt = classes[vt]["parent"] if vt in classes else None
    return out
md = ["# Generated class model (do not edit; produced by `re/tools/build_classes.py`)\n",
      "Source: constructors/vtable stores found in the fresh decompiles, parent = vtable stored by the first constructor the class calls, "
      "fields = `this` accesses in the class's constructors and **own** virtual methods (slots not inherited unchanged).\n",
      "Confidence: `c` written in a constructor · `w` written and read · `r` read only. Widths are the decompiler's access widths, a field accessed with two widths appears twice. Offsets inside the parent's range belong to the parent.\n",
      f"{len(classes)} classes.\n"]
def fname(fn): return L[fn]["name"] or fn
kids = collections.defaultdict(list)
for vt, c in classes.items(): kids[c["parent"]].append(vt)
md.append("\n## Inheritance tree (class (vtable) x number of direct subclasses)\n\n```\n")
def tree(vt, d=0):
    ks = sorted(kids.get(vt, []))
    sub = [k for k in ks if len(kids.get(k, []))]
    leaf = [k for k in ks if not len(kids.get(k, []))]
    md.append("  " * d + f"{classes[vt]['name']} ({vt})" + (f"  [{len(leaf)} leaf classes]" if leaf else "") + "\n")
    for k in sub: tree(k, d + 1)
for r in sorted(vt for vt, c in classes.items() if not c["parent"] or c["parent"] not in classes): tree(r)
md.append("```\n")
for vt, c in sorted(classes.items(), key=lambda kv: (len(chain(kv[0])), kv[0])):
    ch = chain(vt)
    md.append(f"\n## {c['name']}  (vtable {vt})\n")
    md.append(f"- parent: {classes[c['parent']]['name'] + ' (' + c['parent'] + ')' if c['parent'] in classes else 'none found'}"
              f" · size: {hex(c['size']) if c['size'] else 'unknown'} · slots: {len(c['slots'])}"
              f" · constructors: {', '.join(fname(a) for a in c['constructors']) or 'none found'}"
              + (f" · {c['kind']}" if c['kind'] else "") + "\n")
    if c["own_methods"]:
        md.append("- own methods: " + ", ".join(f"[{m['slot']}{(' ' + ROLE[m['slot']]) if m['slot'] in ROLE and len(c['slots']) >= 13 else ''}] {fname(m['fn'])}" for m in c["own_methods"]) + "\n")
    if c["fields"]:
        md.append("\n| Off | W | Conf | R/W | Seen in |\n|---|---|---|---|---|\n")
        for f in c["fields"]:
            md.append(f"| +0x{f['off']:02x}{(' ' + field_name(vt, f['off'])) if field_name(vt, f['off']) else ''} | {f['width']} | {f['conf']} | {f['reads']}/{f['writes']} | {', '.join(fname(x) for x in f['fns'][:3])}{'…' if len(f['fns']) > 3 else ''} |\n")
(R / "STRUCTS_GENERATED.md").write_text("".join(md))

# ---- C header for Ghidra's data type manager ---------------------------------------------------------
TY = {1: "unsigned char", 2: "unsigned short", 4: "unsigned int", 8: "unsigned long long"}
hdr = ["/* Generated by re/tools/build_classes.py. Parse in Ghidra: File > Parse C Source. Fields are union-of-accesses (offset, width) along the parent chain; gaps are padding. */\n"]
for vt, c in sorted(classes.items(), key=lambda kv: (len(chain(kv[0])), kv[0])):
    flds = {}
    for cv in reversed(chain(vt)):
        for f in classes[cv]["fields"]:
            flds.setdefault(f["off"], f)
    size = c["size"] or (max([f["off"] + f["width"] for f in flds.values()] + [4]))
    hdr.append(f"\ntypedef struct {c['name']} {{\n    void *vtable; /* {vt} */\n")
    pos = 4
    for off in sorted(flds):
        f = flds[off]
        if off < pos or off == 0: continue
        if off > pos: hdr.append(f"    unsigned char pad_{pos:02x}[{off - pos}];\n")
        nm = field_name(vt, off); hdr.append(f"    {TY.get(f['width'], 'unsigned int')} {(nm.rstrip('?') if nm else 'f_%02x' % off)}; /* {f['conf']} */\n"); pos = off + f["width"]
    if size > pos: hdr.append(f"    unsigned char pad_{pos:02x}[{size - pos}];\n")
    hdr.append(f"}} {c['name']};\n")
(R / "types").mkdir(exist_ok=True); (R / "types" / "tesoro_types.h").write_text("".join(hdr))
print(len(classes), "classes;", sum(1 for c in classes.values() if c["parent"]), "with parent;", sum(1 for c in classes.values() if c["size"]), "with size;",
      sum(len(c["fields"]) for c in classes.values()), "field records")
