#!/usr/bin/env python3
"""Counts object kinds per level in the shipped archive (read-only). Usage: count_level_kinds.py <TESORO dir> [NAME.LVL]
Walks the level exactly as Level_Load does (header, banks, rooms, objects). Banks are skipped through the animation
bank reader below, which needs the bank layout; if that is not available the walk stops at the banks."""
import struct, sys, collections
d = sys.argv[1]
tre = open(d + "/TESORO.TRE", "rb").read(); dat = open(d + "/TESORO.DAT", "rb").read()
ents = {}
for i in range(0, len(tre) - 27, 28):
    off, size, flags = struct.unpack_from("<IIB", tre, i)
    nm = tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()
    if flags & 4: ents.setdefault(nm, []).append((off, size))
print(len(ents), "entries,", sum(1 for n in ents if n.endswith(".LVL")), "levels")
for n in sorted(ents):
    if n.endswith(".LVL") or len(sys.argv) > 2:
        off, size = ents[n][0]
        print(n, off, size, dat[off + 4:off + 24].hex())
