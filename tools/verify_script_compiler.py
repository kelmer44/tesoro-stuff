#!/usr/bin/env python3
"""Compare the engine's script compiler with an independent reading of the DLG/TXT files.

The ignored C++ driver (local-media/script-implementation/driver.cpp) links the
production script.cpp and prints every compiled op. This tool parses the same original
files with ordinary regular expressions and checks block headers, op types, source lines
and operands, jump/menu links and text lookups. It does not execute the original
parser; see the notes for what is therefore not covered.
"""
import argparse
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from inspect_tesoro import read_index  # noqa: E402

EXPECTED_EXE = '0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef'
TEXTS = ['OBJETOS.TXT', 'ANIMS.TXT', 'CONVERS.TXT', 'ICONS.TXT']
SCRIPTS = ['OBJETOS.DLG', 'ANIMS.DLG', 'CONVERS.DLG', 'ICONS.DLG']
TYPES = dict(ret=0, speech=1, sleep=2, brk=3, jmp=4, menu=5, item=6, iff=7, run=8, msg=9,
             end=10, assign=11, do=12, avi=13, sound=14, cd=15)


def encode_name(text):
    parts = [0, 0]
    ended = False
    data = text.encode('cp850')
    for i in range(12):
        value = 0 if ended or i >= len(data) else data[i]
        ended = ended or value == 0
        code = 0
        if not ended:
            signed = value - 256 if value > 127 else value
            code = ((value + 11) if signed < ord('5') else value) & 31
        parts[i // 6] = (parts[i // 6] << 5) | code
    return parts[1] << 32 | parts[0]


def load_files(game):
    tre = (game / 'TESORO.TRE').read_bytes()
    dat = (game / 'TESORO.DAT').read_bytes()
    files = {}
    for entry in read_index(tre, dat):
        name = entry['name'].upper()
        if name in TEXTS + SCRIPTS:
            files[name] = dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]
    return files


def text_names(files):
    """Speech id -> name of the first table entry with the same 64-bit encoding."""
    found = {}
    duplicates = 0
    for table in TEXTS:
        seen = set()
        for line in files[table].decode('cp850').split('\n'):
            m = re.match(r'\s*([^,]+),\s*"', line)
            if not m:
                continue
            key = encode_name(m.group(1))
            if key in seen:
                duplicates += 1
            seen.add(key)
            found.setdefault(key, m.group(1))
    return found, duplicates


def parse_blocks(files):
    """Independent line-based reading of the scripts: a list of (file, name, type, participants, ops)."""
    blocks = []
    for script in SCRIPTS:
        lines = files[script].decode('cp850').split('\n')
        at = 0
        while at < len(lines):
            line = lines[at].rstrip('\r')
            if not line.startswith('.') or line == '.END':
                at += 1
                continue
            head = line[1:].split()
            name, native = head[0], int(head[1]) if len(head) > 1 else 0
            count = int(lines[at + 1].strip())
            participants = [lines[at + 2 + i].strip() for i in range(count)]
            at += 2 + count
            ops, labels = [], {}
            ordinal = 0
            while True:
                src = lines[at].rstrip('\r')
                number = at + 1
                at += 1
                if src == '.END':
                    ops.append(dict(type=TYPES['end'], line=number))
                    break
                if src.startswith('_'):
                    labels[encode_name(re.match(r'_([0-\u00ed]+)', src).group(1))] = len(ops)
                    continue
                ops.append(parse_op(src, number, participants, ordinal))
                if src.startswith('D'):
                    ordinal += 1
            link(ops, labels)
            blocks.append((script, name, native, participants, ops))
    return blocks


def parse_op(src, number, participants, ordinal):
    op = dict(line=number)
    if src.startswith(' '):
        m = re.match(r' ([^,]*), *(-?\d+)(?: *, *(-?\d+))?', src)
        op.update(type=TYPES['speech'], id=m.group(1), speaker=int(m.group(2)),
                  option=int(m.group(3)) if m.group(3) else 0)  # the original parser stores 0 when no option follows (0x404810)
    elif src.startswith('D'):
        op.update(type=TYPES['do'], value=ordinal)
    elif src.startswith('break'):
        op['type'] = TYPES['brk']
    elif src.startswith('return'):
        op['type'] = TYPES['ret']
    elif src.startswith('sleep'):
        op.update(type=TYPES['sleep'], value=int(re.match(r'sleep (-?\d+)', src).group(1)))
    elif src.startswith('jmp'):
        op.update(type=TYPES['jmp'], label=encode_name(re.match(r'jmp (\S+)', src).group(1)))
    elif src.startswith('if'):
        m = re.match(r'if v(\d+) == (-?\d+) jmp ([0-\u00ed]+)', src)
        op.update(type=TYPES['iff'], var=int(m.group(1)) & 255, value=int(m.group(2)) & 255,
                  label=encode_name(m.group(3)))
    elif src.startswith('v'):
        m = re.match(r'v(\d+) = (-?\d+)', src)
        op.update(type=TYPES['assign'], var=int(m.group(1)) & 255, value=int(m.group(2)) & 255)
    elif src.startswith('menuitem') or src.startswith('menutiem'):
        m = re.match(r'menu\w+ (next|stop)(?: +([!v])v?(\d+))?', src)
        cond = re.match(r'menu\w+ (?:next|stop) +(!?)v(\d+)', src)
        op.update(type=TYPES['item'], chain=m.group(1) == 'next',
                  var=int(cond.group(2)) & 255 if cond else 255, invert=bool(cond and cond.group(1)))
    elif src.startswith('menu'):
        op['type'] = TYPES['menu']
    elif src.startswith('run'):
        m = re.match(r"run '([^']*)' ([0-\u00ed]+)", src)
        op.update(type=TYPES['run'], block=m.group(1), label=encode_name(m.group(2)))
    elif src.startswith('play'):
        m = re.match(r"play (avi|cd|sound) '?([^'\s]*)", src)
        kind = m.group(1)
        op.update(type=TYPES[kind], value=int(m.group(2)) if kind != 'sound' else 0,
                  name=m.group(2) if kind == 'sound' else '')
    elif src.startswith('sendmessage'):
        op.update(type=TYPES['msg'], **parse_message(src, participants))
    else:
        raise ValueError(f'unexpected line {number}: {src!r}')
    return op


def signed8(value):
    return value - 256 if value > 127 else value


def parse_message(src, participants):
    m = re.match(r"sendmessage (?:'([^']*)'|(\d)) +(?:in (\S+) +)?(.*)", src)
    target = encode_name(m.group(1)) if m.group(1) is not None else encode_name(participants[int(m.group(2)) - 1])
    scope, level = 0, 0
    if m.group(3):
        scope, level = (1, 0) if m.group(3) == 'INVENTARIO' else (2, int(m.group(3)[5:]))
    rest = m.group(4)
    out = dict(object=target, scope=scope, level=level)
    if rest.startswith('activate') or rest.startswith('show'):
        out.update(kind=3 if rest.startswith('activate') else 4, on=rest.split()[1] == 'on')
    elif rest.startswith('ira'):
        x, y = map(int, rest.split()[1:3])
        out.update(kind=1, x=x, y=y)
    elif rest.startswith('mirar'):
        out.update(kind=2, direction=dict(ARRIBA=1, ABAJO=2, IZQUIERDA=3, DERECHA=4)[rest.split()[1]])
    else:
        _, bank, first, mode = rest.split()[:4]
        out.update(kind=0, bank=int(bank) & 255, first=signed8(int(first) & 255),
                   mode=dict(stop=0, play=1, loop=2, wait=3)[mode])
    return out


def link(ops, labels):
    previous = -1
    for index, op in enumerate(ops):
        if op['type'] in (TYPES['jmp'], TYPES['iff']):
            op['target'] = labels.get(op['label'], 0)
            op['missing'] = op['label'] not in labels
        elif op['type'] == TYPES['menu']:
            previous = -1
        elif op['type'] == TYPES['item']:
            op['next'] = -1
            if previous > 0:
                ops[previous]['next'] = index
            previous = index if op['chain'] else -1


def expected_lines(blocks, texts, by_name):
    out = []
    for script, name, native, participants, ops in blocks:
        out.append(f'B {script} {name} {native} {len(participants)} {len(ops)}')
        for index, op in enumerate(ops):
            t = op['type']
            fields = ''
            if t == 1:
                fields = f" {texts[encode_name(op['id'])]} {op['speaker']} {op['option']}"
            elif t in (2, 13, 15, 12):
                fields = f" {op['value']}"
            elif t == 4:
                fields = f" {op['target']}"
            elif t == 7:
                fields = f" {op['target']} {op['var']} {op['value']}"
            elif t == 11:
                fields = f" {op['var']} {op['value']}"
            elif t == 6:
                fields = f" {op['next']} {op['var']} {int(op['invert'])}"
            elif t == 14:
                fields = f" {op['name']}"
            elif t == 8:
                # The original links a run of FANMTRONO to no block at all.
                target = '-' if op['block'] == 'FANMTRONO' else by_name.get(encode_name(op['block']), '-')
                fields = f" {target} {op['label']}"
            elif t == 9:
                fields = f" {op['object']} {op['scope']} {op['level']} {op['kind']}"
                if op['kind'] == 0:
                    fields += f" {op['bank']} {op['first']} {op['mode']}"
                elif op['kind'] == 1:
                    fields += f" {op['x']} {op['y']}"
                elif op['kind'] == 2:
                    fields += f" {op['direction']}"
                else:
                    fields += f" {int(op['on'])}"
            out.append(f'O {index} {t} {op["line"]}{fields}')
    return out


def actual_lines(driver, directory):
    process = subprocess.run([str(driver), str(directory)], capture_output=True, check=True)
    lines = []
    for line in process.stdout.decode('cp850').splitlines():
        if line.startswith('B '):
            parts = line.split()
            lines.append(' '.join(parts[:6]))  # drop the label count
        else:
            lines.append(line)
    return lines


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--data', type=Path, required=True, help='directory with the extracted DLG/TXT files')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()

    files = load_files(args.game)
    for name, payload in files.items():
        (args.data / name).write_bytes(payload)
    names, duplicates = text_names(files)
    blocks = parse_blocks(files)
    by_name = {}
    for _, name, _, _, _ in blocks:
        by_name.setdefault(encode_name(name), name)
    expected = expected_lines(blocks, names, by_name)
    actual = actual_lines(args.driver, args.data)

    mismatches = [(i, e, a) for i, (e, a) in enumerate(zip(expected, actual)) if e != a]
    report = dict(
        driver_sha256=hashlib.sha256(args.driver.read_bytes()).hexdigest(),
        inputs={n: hashlib.sha256(p).hexdigest() for n, p in sorted(files.items())},
        blocks=len(blocks), ops=sum(len(b[4]) for b in blocks),
        expected_lines=len(expected), actual_lines=len(actual),
        mismatching_lines=len(mismatches) + abs(len(expected) - len(actual)),
        missing_jump_labels=[dict(block=b[1], line=o['line']) for b in blocks for o in b[4] if o.get('missing')],
        duplicate_text_ids=duplicates,
        first_mismatches=[dict(line=i, expected=e, actual=a) for i, e, a in mismatches[:10]])
    text = json.dumps(report, indent=2, ensure_ascii=False)
    print(text)
    if args.output:
        args.output.write_text(text + '\n')
    return 1 if report['mismatching_lines'] else 0


if __name__ == '__main__':
    sys.exit(main())
