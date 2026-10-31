#!/usr/bin/env python3
"""Condensed view of named functions: name keys as strings, level globals as level numbers. Usage: pretty_fn.py <addr>..."""
import re, struct, sys
d = open('/Users/gabriel/Desktop/ags/TESORO/TESORO.EXE', 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]; n = struct.unpack_from('<H', d, pe + 6)[0]; so = struct.unpack_from('<H', d, pe + 20)[0]
secs = []
for i in range(n):
    o = pe + 24 + so + 40 * i; vs, va, rs, ra = struct.unpack_from('<IIII', d, o + 8); secs.append((0x400000 + va, vs, ra))
def cstr(a):
    for b, vs, ra in secs:
        if b <= a < b + vs:
            o = a - b + ra; return d[o:d.index(b'\0', o)].decode('latin1')
LV = {0x442130 + 4 * i: i for i in range(47)}; LV[0x4421ec] = 'IFACE'
def pretty(t):
    t = re.sub(r'FUN_00415c40\((?:\(int\))?(0x[0-9a-f]+)\)', lambda m: 'K"%s"' % cstr(int(m.group(1), 16)), t)
    t = re.sub(r's_([A-Za-z0-9_]+)_([0-9a-f]{8})', lambda m: '"%s"' % cstr(int(m.group(2), 16)), t)
    t = re.sub(r'DAT_00([0-9a-f]{6})', lambda m: 'L%s' % LV.get(int(m.group(1), 16), m.group(1)), t)
    t = t.replace('FUN_00408780', 'find').replace('FUN_004060c0', 'findscript').replace('FUN_00405fd0', 'start').replace('FUN_00407b80', 'jimEnter').replace('FUN_00401d80', 'setVisible').replace('FUN_00401de0', 'selectBank')
    return t
for a in sys.argv[1:]:
    body = open(f're/export/{a}.c').read()
    body = '\n'.join(l for l in body.split('\n') if l.strip() and not re.match(r'^  (undefined\d*|int|uint|void|byte|char|ushort|ulonglong|longlong|short|bool)\s+[*a-zA-Z0-9_ ,\[\]]*;$', l))
    print('###', a); print(pretty(body))
