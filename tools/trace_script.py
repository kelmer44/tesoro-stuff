#!/usr/bin/env python3
"""Trace a supported path through original Tesoro scripts, without running the EXE.

This analysis tool emits requests for speech, movement and animation. It does
not simulate their timing or native scene code. Unknown commands fail loudly.
Only tail-position `run` calls are followed; general script scheduling is not
implemented. Native callbacks stop the trace unless confirmed to be no-ops.
"""
import argparse
from dataclasses import dataclass
import json
from pathlib import Path
import re

from inspect_tesoro import read_index


class UnsupportedScript(ValueError):
    pass


@dataclass
class Block:
    name: str
    native_type: int
    participants: list
    file: str
    lines: list
    labels: dict
    native_ordinals: dict


def without_comment(line):
    # The known scripts use // outside quoted object names for comments.
    return line.split('//', 1)[0].strip()


def load_game(path):
    dat = (path / 'TESORO.DAT').read_bytes()
    entries = read_index((path / 'TESORO.TRE').read_bytes(), dat)
    blocks, texts, audio = {}, {}, {}
    for entry in entries:
        name, offset, size = entry['name'], entry['offset'], entry['size']
        payload = dat[offset + 4:offset + 4 + size]
        suffix = Path(name).suffix.lower()
        if suffix == '.wav':
            audio[Path(name).stem.upper()] = name
        elif suffix == '.txt':
            for line in payload.decode('cp850').splitlines():
                match = re.fullmatch(r'(\w+),\s*"(.*)"\s*', line)
                if match:
                    texts[match[1].upper()] = match[2]
        elif suffix == '.dlg':
            source = payload.decode('cp850').splitlines()
            index = 0
            while index < len(source):
                if not source[index].strip():
                    index += 1
                    continue
                header = re.fullmatch(r'\.([\w]+)(?:\s+(\d+))?\s*', source[index])
                if not header or header[1] == 'END':
                    raise ValueError(f'Invalid block header at {name}:{index + 1}')
                index += 1
                count = int(source[index].strip())
                index += 1
                participants = [s.strip() for s in source[index:index + count]]
                index += count
                lines, labels, native_ordinals = [], {}, {}
                while index < len(source) and source[index].strip() != '.END':
                    line_number = index + 1
                    line = source[index]
                    clean = without_comment(line)
                    if clean.startswith('_'):
                        label = clean.split()[0][1:]
                        if label in labels:
                            raise ValueError(f'Duplicate label {name}:{line_number}: {label}')
                        labels[label] = len(lines)
                    if line.startswith('DO '):
                        native_ordinals[len(lines)] = len(native_ordinals)
                    lines.append((line_number, line))
                    index += 1
                if index == len(source):
                    raise ValueError(f'Unterminated block {header[1]}')
                index += 1
                key = header[1].upper()
                if key in blocks:
                    raise ValueError(f'Duplicate block {key}')
                blocks[key] = Block(key, int(header[2] or 0), participants,
                                    name, lines, labels, native_ordinals)
    return blocks, texts, audio


def trace(blocks, texts, audio, script, label, variables, max_steps=10000):
    variables = {int(k): int(v) & 255 for k, v in variables.items()}
    initial_variables = dict(variables)
    events, object_requests = [], {}
    script = script.upper()
    block = blocks[script]
    pc = block.labels[label]
    sleep_requested = 0
    status = 'step_limit'

    def event(kind, **fields):
        row = dict(kind=kind, script=block.name, file=block.file,
                   line=line_number, **fields)
        events.append(row)
        return row

    for _ in range(max_steps):
        if pc >= len(block.lines):
            status = 'returned'
            break
        line_number, source = block.lines[pc]
        clean = without_comment(source)
        original_pc = pc
        pc += 1
        if not clean or clean.startswith('_'):
            continue
        if clean == 'return':
            event('return')
            status = 'returned'
            break
        match = re.fullmatch(r'if v(\d+) == (-?\d+) jmp (\w+)', clean)
        if match:
            actual = variables.get(int(match[1]), 0)
            taken = actual == (int(match[2]) & 255)
            event('condition', variable=int(match[1]), actual=actual,
                  expected=int(match[2]) & 255, taken=taken, target=match[3])
            if taken:
                pc = block.labels[match[3]]
            continue
        match = re.fullmatch(r'jmp (\w+)', clean)
        if match:
            event('jump', target=match[1])
            pc = block.labels[match[1]]
            continue
        match = re.fullmatch(r'v(\d+) = (-?\d+)', clean)
        if match:
            number, value = int(match[1]), int(match[2]) & 255
            variables[number] = value
            event('set_variable', variable=number, value=value)
            continue
        match = re.fullmatch(r'(\w+),\s*(\d+)(?:,\s*(\d+))?', clean)
        if match:
            text_id, participant = match[1].upper(), int(match[2])
            if text_id not in texts:
                raise ValueError(f'Missing text {text_id}')
            if participant > len(block.participants):
                raise ValueError(f'Invalid participant {participant}')
            event('speech_request', text_id=text_id, text=texts[text_id],
                  participant=participant,
                  actor=block.participants[participant - 1] if participant else None,
                  wav=audio.get(text_id), extra=match[3])
            continue
        match = re.fullmatch(r'sleep (\d+)', clean)
        if match:
            milliseconds = int(match[1])
            sleep_requested += milliseconds
            event('sleep_request', milliseconds=milliseconds)
            continue
        match = re.fullmatch(r"sendmessage ('[^']+'|\d+)(?: in (\w+))? (.+)", clean)
        if match:
            target, scope, message = match.groups()
            if target.startswith("'"):
                target = target[1:-1]
            else:
                slot = int(target)
                if not 1 <= slot <= len(block.participants):
                    raise ValueError(f'Invalid object participant {slot}')
                target = block.participants[slot - 1]
            scope = scope or 'CURRENT_SCENE'
            switch = re.fullmatch(r'(activate|show) (on|off)', message)
            if switch:
                object_requests.setdefault(f'{scope}:{target}', {})[switch[1]] = switch[2] == 'on'
            elif not (re.fullmatch(r'play \d+ \d+ (play|stop|loop|wait)', message)
                      or re.fullmatch(r'ira -?\d+ -?\d+', message)
                      or re.fullmatch(r'mirar (ARRIBA|ABAJO|IZQUIERDA|DERECHA)', message)):
                raise UnsupportedScript(f'{block.file}:{line_number}: unsupported message {message}')
            event('object_request', scope=scope, target=target, message=message)
            continue
        match = re.fullmatch(r"run '(\w+)' (\w+)", clean)
        if match:
            # The pen path transfers to FCAPITAN/salir immediately before return.
            # General parent/child scheduling needs separate reverse engineering.
            remainder = [without_comment(s) for _, s in block.lines[pc:]]
            next_statement = next((s for s in remainder if s and not s.startswith('_')), None)
            if next_statement != 'return':
                raise UnsupportedScript(f'{block.file}:{line_number}: non-tail run is not modeled')
            event('tail_script_transfer', destination=match[1], label=match[2],
                  shares_variable_context=True,
                  note='Analysis-only tail traversal; original scheduler is not reproduced.')
            block = blocks[match[1].upper()]
            pc = block.labels[match[2]]
            continue
        if source.startswith('DO '):
            ordinal = block.native_ordinals[original_pc]
            row = event('native_callback', native_type=block.native_type,
                        ordinal=ordinal, annotation=source[3:].strip())
            if block.native_type in (0, 11, 13):
                row.update(handler='0x00401d20', effect='confirmed no-op')
                continue
            if block.native_type == 21 and ordinal == 0:
                row.update(handler='0x00416b50', lookup_scene_global='0x00442130',
                           target='PUERTA1', virtual_method_offset='0x50')
                status = 'needs_native_door_transition'
            else:
                status = 'needs_native_callback'
            break
        raise UnsupportedScript(f'{block.file}:{line_number}: unsupported instruction {source!r}')
    return dict(script=script, label=label, initial_variables=initial_variables,
                variable_context_owner='CAPITAN in the office' if script == 'FBOLI' and label == 'coger' else 'explicit caller-supplied context; ownership not inferred here',
                variables=variables, status=status, event_count=len(events),
                sleep_requested_ms=sleep_requested, object_requests=object_requests,
                limitation='Requests only; no original speech/movement timing, graphics or native transition executed.',
                events=events)


def parse_variables(value):
    return {int(k.removeprefix('v')): int(v) for k, v in
            (part.split('=', 1) for part in value.split(',') if part)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('game', type=Path)
    parser.add_argument('--script', default='FBOLI')
    parser.add_argument('--label', default='coger')
    parser.add_argument('--vars', default='v0=0', help='Explicit input state, e.g. v0=0,v1=0')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    blocks, texts, audio = load_game(args.game)
    result = trace(blocks, texts, audio, args.script, args.label, parse_variables(args.vars))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
    print(f"{len(blocks)} blocks catalogued; {result['event_count']} events; status={result['status']}; variables={result['variables']}")
