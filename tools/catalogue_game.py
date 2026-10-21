#!/usr/bin/env python3
"""Catalogue full-game research scope from original data; never claim coverage.

Uses the existing structural readers. Command/message forms are lexical counts,
not a substitute for recovering the executable's parser and dispatch semantics.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

from inspect_tesoro import read_index, read_scene_objects
from trace_script import load_game, without_comment


def catalogue(game):
    data = (game / 'TESORO.DAT').read_bytes()
    entries = read_index((game / 'TESORO.TRE').read_bytes(), data)
    scenes, kinds = [], Counter()
    for entry in entries:
        if not entry['name'].lower().endswith('.lvl'):
            continue
        start = entry['offset'] + 4
        objects = read_scene_objects(data[start:start + entry['size']])
        counts = Counter(obj['kind'] for obj in objects)
        kinds.update(counts)
        scenes.append(dict(name=entry['name'], objects=len(objects),
                           kinds=dict(sorted(counts.items()))))
    blocks, texts, audio = load_game(game)
    commands, messages = Counter(), Counter()
    for block in blocks.values():
        for _, source in block.lines:
            clean = without_comment(source)
            if not clean or clean.startswith('_'):
                continue
            head = clean.split()[0]
            if re.match(r'^v\d+\s*=', clean):
                head = 'variable_assignment'
            elif re.match(r'^\w+,', clean):
                head = 'speech'
            commands[head] += 1
            if head == 'sendmessage':
                match = re.match(r"sendmessage (?:'[^']+'|\d+)(?: in \w+)? (\w+)", clean)
                messages[match[1] if match else 'UNCLASSIFIED'] += 1
    return dict(
        notice='Structural catalogue only. Counts do not imply implemented or original-runtime-verified behavior.',
        source='TESORO.TRE/DAT via existing structural readers; lexical command classification',
        hashes={name: hashlib.sha256((game / name).read_bytes()).hexdigest()
                for name in ('TESORO.EXE', 'TESORO.TRE', 'TESORO.DAT')},
        scene_count=len(scenes), object_count=sum(kinds.values()),
        object_kinds=dict(sorted(kinds.items())), scenes=scenes,
        script_blocks=len(blocks), text_entries=len(texts), audio_entries=len(audio),
        command_forms=dict(sorted(commands.items())),
        message_forms=dict(sorted(messages.items())),
        blocks=[dict(name=block.name, file=block.file, native_type=block.native_type,
                     labels=list(block.labels), do_count=len(block.native_ordinals))
                for block in blocks.values()])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('game', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    # Never allow an accidental output argument to replace an input game file.
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Write research output outside the original game directory')
    report = catalogue(args.game)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
    print(f"{report['scene_count']} LVLs, {report['object_count']} objects, "
          f"{len(report['object_kinds'])} object kinds, {report['script_blocks']} script blocks")


if __name__ == '__main__':
    main()
