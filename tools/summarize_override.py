#!/usr/bin/env python3
"""Readable summaries of exported native routines: strings and well-known globals resolved.

Usage: summarize_override.py EXE EXPORT_DIR [function addresses...]
Replaces tesoro_encode_name(0xADDR) and string symbols by the string in the executable, the scene table
(DAT_00442130 + 4n) by SCENE[n], and the well-known globals. Output is the decompiler's C, tidied.
"""
import re
import struct
import sys
from pathlib import Path

GLOBALS = {0x4420fc: 'JIM', 0x442128: 'POINTER', 0x4421ec: 'INTERFACE', 0x4420b8: 'SCRIPTS', 0x442104: 'UPDATELIST',
           0x442108: 'COMPOSITOR', 0x442120: 'CLOCK', 0x44206c: 'MOUSE', 0x442074: 'KEYS', 0x442080: 'SOUND'}


def main():
    exe = Path(sys.argv[1]).read_bytes()
    pe = struct.unpack_from('<I', exe, 0x3c)[0]
    n = struct.unpack_from('<H', exe, pe + 6)[0]
    opt = struct.unpack_from('<H', exe, pe + 20)[0]
    secs = []
    for i in range(n):
        o = pe + 24 + opt + 40 * i
        vs, va, rs, ra = struct.unpack_from('<IIII', exe, o + 8)
        secs.append((va, max(vs, rs), ra))

    def string(a):
        r = a - 0x400000
        for va, sz, ra in secs:
            if va <= r < va + sz:
                return exe[ra + r - va:ra + r - va + 40].split(b'\0')[0].decode('cp850', 'replace')
        return '?'

    for path in sorted(Path(sys.argv[2]).glob('*.raw.c')):
        addr = path.name[:8]
        if len(sys.argv) > 3 and addr.lstrip('0') not in [a.lower().replace('0x', '') for a in sys.argv[3:]]:
            continue
        text = path.read_text()
        text = text[text.index('\n\n') + 2:] if '\n\n' in text else text
        text = re.sub(r'tesoro_encode_name\(\(?(?:int\))?(?:&)?(?:DAT_|s_\w+_|PTR_\w+_)?(?:0x)?([0-9a-f]{6,8})\)?\)',
                      lambda m: '"%s"' % string(int(m.group(1), 16)), text)
        text = re.sub(r'tesoro_encode_name\((0x[0-9a-f]+)\)', lambda m: '"%s"' % string(int(m.group(1), 16)), text)
        text = re.sub(r'&?s_\w+_([0-9a-f]{8})', lambda m: '"%s"' % string(int(m.group(1), 16)), text)
        text = re.sub(r'&?DAT_0042([0-9a-f]{4})', lambda m: '"%s"' % string(0x420000 + int(m.group(1), 16)), text)
        text = re.sub(r'\(&DAT_00442130\)\[([^\]]+)\]', r'SCENE[\1]', text)
        text = re.sub(r'DAT_0044(21[0-9a-f]{2}|2130)', lambda m: ('SCENE[%d]' % ((0x440000 + int(m.group(1), 16) - 0x442130) // 4)) if 0x442130 <= 0x440000 + int(m.group(1), 16) < 0x4421ec else m.group(0), text)
        text = re.sub(r'DAT_0044(2[0-9a-f]{3})', lambda m: GLOBALS.get(0x440000 + int(m.group(1), 16), m.group(0)), text)
        text = re.sub(r'\n\s*\n+', '\n', text)
        text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
        print('//====', addr)
        print(text.strip())


main()
