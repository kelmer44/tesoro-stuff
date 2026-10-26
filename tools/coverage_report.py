#!/usr/bin/env python3
"""How much of the game's object behaviour the engine covers, from the data.

Every object record of the 50 scene files is classed by the factory's choice (`0x409e50`, exported in
analysis/factory/): its kind and, for the kinds that switch on a byte, its class. The classes that the
factory gives a vtable of their own override some behaviour; classes it does not are the default. The
engine's status per kind/class is the table STATUS below, kept by hand (update it when a class is ported).

"complete"   the behaviour recovered is ported (default behaviour counts when there is no override)
"partial"    ported for the verbs/natives recovered, other overrides of the class not recovered
"unported"   the class exists in the game but the engine keeps it inert or default with a warning
"""
import json
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from inspect_tesoro import read_index, read_scene_objects  # noqa: E402

FACTORY = Path(__file__).resolve().parent.parent / 'analysis/factory/00409e50_factory.raw.c'
SELECTOR = {5: 5, 6: 1, 12: 1, 13: 1, 17: 1, 20: 1}   # the record byte that picks the class

# Classes the engine ports (by kind), the others being "unported" when the class has its own vtable.
PORTED = {
    5: {c: 'partial' for c in (0, 1, 2, 3, 4, 5, 6, 7, 8)},   # talk and item verbs: captain, recruiter, tavern, fisherman, gambler, kitchen
    6: {0: 'complete', 2: 'partial', 3: 'partial', 4: 'partial', 8: 'partial', 9: 'partial'},   # doors: generic use, the tavern, cabin, storeroom and office doors
    12: {c: 'complete' for c in (0, 1, 2, 3, 4, 5, 6, 10, 11)},
    13: {},                                           # items: core only, no per-item class
    17: {c: 'complete' for c in range(17)},           # the ship's objects: every class ported
    20: {c: 'complete' for c in (18, 19, 23, 24, 25, 26)},
}
WHOLE_KIND = {0: 'complete', 1: 'complete', 2: 'complete', 3: 'complete', 7: 'complete', 8: 'complete', 9: 'complete',
              10: 'complete', 11: 'complete', 15: 'complete', 13: 'partial'}
INTERFACE_ONLY = {2, 10, 11, 13, 15, 22, 23}


def factory_classes():
    """kind -> set of selector values that have a vtable of their own."""
    text = FACTORY.read_text()
    result = {}
    cases = list(re.finditer(r'\n  case (0x[0-9a-f]+|\d+):\n', text))
    for i, m in enumerate(cases):
        kind = int(m.group(1), 0)
        seg = text[m.end():cases[i + 1].start() if i + 1 < len(cases) else len(text)]
        labels = set()
        for inner in re.finditer(r'\n    case (0x[0-9a-f]+|\d+):\n(.*?)(?=\n    case |\n    default:|\Z)', seg, re.S):
            if '*puVar1 = &PTR_' in inner.group(2):
                labels.add(int(inner.group(1), 0))
        result[kind] = labels
    return result


CHAR_VTABLES = {0: 0x42b280, 1: 0x42b4a0, 2: 0x42b308, 3: 0x42b390, 4: 0x42b418, 5: 0x42b6c0, 6: 0x42b638, 7: 0x42b5b0, 8: 0x42b528,
                9: 0x42b748, 10: 0x42b858, 11: 0x42b7d0, 13: 0x42b1f8, 14: 0x42b170, 15: 0x42b0e8, 16: 0x42b060}


def classes_without_overrides(game):
    """(kind, class) whose vtable is a copy of the default one: they need no code."""
    exe = (game / 'TESORO.EXE').read_bytes()
    pe = struct.unpack_from('<I', exe, 0x3c)[0]
    n = struct.unpack_from('<H', exe, pe + 6)[0]
    opt = struct.unpack_from('<H', exe, pe + 20)[0]
    secs = []
    for i in range(n):
        o = pe + 24 + opt + 40 * i
        vs, va, rs, ra = struct.unpack_from('<IIII', exe, o + 8)
        secs.append((va, max(vs, rs), ra))

    def dw(a):
        r = a - 0x400000
        for va, sz, ra in secs:
            if va <= r < va + sz:
                return struct.unpack_from('<I', exe, ra + r - va)[0]
    base = [dw(0x42bb48 + o) for o in range(0, 0x78, 4)]
    result = {(5, c) for c, a in CHAR_VTABLES.items() if [dw(a + o) for o in range(0, 0x78, 4)] == base}
    ship = Path(__file__).resolve().parent.parent / 'analysis/ship/class-overrides.json'
    if ship.exists():
        for key, info in json.loads(ship.read_text()).items():
            k, c = map(int, key.split('/'))
            if not info['diff']:
                result.add((k, c))
    return result


def main():
    game = Path(sys.argv[1])
    plain = classes_without_overrides(game)
    dat = (game / 'TESORO.DAT').read_bytes()
    classes = factory_classes()
    rows = []
    for e in read_index((game / 'TESORO.TRE').read_bytes(), dat):
        if not e['name'].lower().endswith('.lvl'):
            continue
        for o in read_scene_objects(dat[e['offset'] + 4:e['offset'] + 4 + e['size']]):
            p = bytes.fromhex(o['parameters_hex'])
            kind = o['kind']
            sel = p[SELECTOR[kind]] if kind in SELECTOR and len(p) > SELECTOR[kind] else None
            custom = kind in SELECTOR and sel in classes.get(kind, set()) and (kind, sel) not in plain
            if kind in WHOLE_KIND and kind not in SELECTOR:
                status = WHOLE_KIND[kind]
            elif kind in SELECTOR:
                if not custom:
                    status = 'complete'                    # default class: only the default verbs
                else:
                    status = PORTED.get(kind, {}).get(sel, 'unported')
            else:
                status = 'unported'
            rows.append(dict(scene=e['name'], name=o['name'], kind=kind, cls=sel, custom=custom, status=status))
    total = Counter(r['status'] for r in rows)
    by_kind = defaultdict(Counter)
    for r in rows:
        by_kind[r['kind']][r['status']] += 1
    scenes = defaultdict(Counter)
    for r in rows:
        scenes[r['scene']][r['status']] += 1
    out = dict(objects=len(rows), by_status=dict(total),
               by_kind={k: dict(v) for k, v in sorted(by_kind.items())},
               unported_classes=sorted({(r['kind'], r['cls']) for r in rows if r['status'] == 'unported' and r['cls'] is not None}, key=lambda x: (x[0], x[1] or 0)),
               scenes={k: dict(v) for k, v in sorted(scenes.items())})
    if len(sys.argv) > 2:
        Path(sys.argv[2]).write_text(json.dumps(out, indent=1, default=list) + '\n')
    print(json.dumps({k: v for k, v in out.items() if k != 'scenes'}, default=list))


main()
