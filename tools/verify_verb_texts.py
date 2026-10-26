#!/usr/bin/env python3
"""Check that every verb line named by an interactive object (and the generic fallbacks) exists
in the dialogue tables and has a WAV. Usage: verify_verb_texts.py GAME_DIR [output.json]"""
import json
import sys
from pathlib import Path

from inspect_tesoro import read_index, read_scene_objects
from trace_script import load_game

GENERIC = ['OBJ125T03', 'OBJ096T07', 'OBJ106T05', 'ICON01T058', 'MODELO2']


def main():
    game = Path(sys.argv[1])
    scripts = load_game(game)
    texts, audio = scripts[1], scripts[2]
    dat = (game / 'TESORO.DAT').read_bytes()
    entries = read_index((game / 'TESORO.TRE').read_bytes(), dat)
    report = dict(objects=0, verb_lines=0, missing_text=[], missing_audio=[], generic={})
    for name in GENERIC:
        report['generic'][name] = dict(text=name in texts, audio=name in audio)
    for entry in entries:
        # INTERFACE.LVL holds the inventory items, which have their own layout.
        if not entry['name'].upper().endswith('.LVL') or entry['name'].upper() == 'INTERFACE.LVL':
            continue
        payload = dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]
        for obj in read_scene_objects(payload):
            if obj['kind'] not in (5, 6, 9, 12, 13, 17, 20):
                continue
            p = bytes.fromhex(obj['parameters_hex'])
            base = 4 if obj['kind'] == 5 else 0
            if len(p) < base + 0x13 + 72:
                continue
            report['objects'] += 1
            for k in range(6):
                at = base + 0x13 + 12 * k
                line = p[at:at + 12].split(b'\0')[0].decode('cp850')
                if not line:
                    continue
                report['verb_lines'] += 1
                key = line.upper()
                where = f"{entry['name']}:{obj['name']}:{line}"
                if key not in texts:
                    report['missing_text'].append(where)
                if key not in audio:
                    report['missing_audio'].append(where)
    out = json.dumps(report, indent=1)
    if len(sys.argv) > 2:
        Path(sys.argv[2]).write_text(out + '\n')
    print(out[:3000])


main()
