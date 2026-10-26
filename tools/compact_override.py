#!/usr/bin/env python3
"""Compacts the output of summarize_override.py: script starts and object lookups on one line each."""
import re
import sys


def compact(block):
    lines = block.split('\n')
    names = {}
    out = []
    pending_block = {}
    for line in lines:
        s = line.strip()
        if re.match(r'(undefined\d?|int|uint|void|byte|short|ushort|char|bool|ulonglong|longlong|code|void \*|int \*)\b.*;$', s) and '=' not in s:
            continue
        m = re.match(r'(\w+) = "(.*)";$', s)
        if m:
            names[m.group(1)] = m.group(2)
            continue
        m = re.match(r'(\w+) = \(void \*\)FUN_004060c0\(SCRIPTS,\(int\)(\w+)', s) or re.match(r'(\w+) = \(void \*\)FUN_004060c0\(SCRIPTS,(\w+),(\w+)\);', s)
        if m:
            key = m.group(2)
            name = names.get(key)
            if name is None and key.startswith('local_'):
                name = names.get('uVar%s' % '4')  # fallback not reliable
            pending_block[m.group(1)] = name or key
            continue
        m = re.match(r'tesoro_start_script\((\w+),\(int\)(\w+),\(int\)\(\w+ >> 0x20\),(.+?),(.+?)\);$', s)
        if m:
            out.append('START(%s / %s, vars=%s, scene=%s)' % (pending_block.get(m.group(1), '?'), names.get(m.group(2), m.group(2)), m.group(3), m.group(4)))
            continue
        m = re.search(r'tesoro_find_scene_object\(([^,]+),\(int\)(\w+),', s)
        if m:
            s = re.sub(r'tesoro_find_scene_object\(([^,]+),\(int\)(\w+),\(int\)\(\w+ >> 0x20\)\)', lambda mm: 'FIND(%s,"%s")' % (mm.group(1), names.get(mm.group(2), mm.group(2))), s)
        s = re.sub(r'\(\*\*\(code \*\*\)\(\*(\w+) \+ (0x[0-9a-f]+)\)\)', r'\1.slot\2', s)
        s = re.sub(r'\(\*\*\(code \*\*\)\(\*\*(\w+) \+ (0x[0-9a-f]+)\)\)', r'\1.slot\2', s)
        out.append(s)
    return '\n'.join(out)


text = sys.stdin.read()
for part in re.split(r'(?m)^//==== ', text)[1:]:
    head, body = part.split('\n', 1)
    print('//====', head)
    print(compact(body))
