#!/usr/bin/env python3
"""Check the script executor's timeline for FANPRESENT/main against the script text.

The ignored C++ driver (local-media/script-implementation/exec_driver.cpp) links the
production script.cpp, scriptexec.cpp and scene.cpp, runs the block from the real
PUERTO scene against a fake host with a 1 ms clock, and logs every host-visible event
with its game time. This tool predicts the same events from the DLG text alone:
sleeps add their duration, a speech line ends when its audio does and then pauses
201 ms, and a movie does not advance the clock. It also checks the final active/shown
state of the objects that the script switches.

It validates the executor's waiting rules and ordering. It does not execute the
original, and the audio lengths are the WAV data sizes, not decoded streams.
"""
import argparse
import hashlib
import json
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import verify_script_compiler as compiler  # noqa: E402

SPEECH_PAUSE = 201


def predict(game, data):
    files = compiler.load_files(game)
    names, _ = compiler.text_names(files)
    blocks = compiler.parse_blocks(files)
    block = next(b for b in blocks if b[1] == 'FANPRESENT')
    ops = block[4]
    tokens = (data / 'speech.txt').read_text().split()
    speech_ms = {tokens[i]: int(tokens[i + 1]) for i in range(0, len(tokens), 2)}

    events = [('run', 0, 'FANPRESENT'), ('input', 0, '0')]
    t = 0
    stopped_at = None
    active, shown = {}, {}
    for index, op in enumerate(ops):
        kind = op['type']
        if kind == compiler.TYPES['do']:
            events.append(('native', t, f"1 {op['value']}"))
            if op['value'] == 3:
                shown['BARCO'] = 1
            if op['value'] == 4:
                stopped_at = t
        elif kind == compiler.TYPES['avi']:
            events.append(('movie', t, str(op['value'])))
        elif kind == compiler.TYPES['sound']:
            events.append(('sound', t, op['name']))
        elif kind == compiler.TYPES['speech']:
            name = names[compiler.encode_name(op['id'])]
            events.append(('speech', t, f'{name} {speech_ms[name]}'))
            if stopped_at is not None:
                break  # the driver stops after the tick that reached Show Jim
            t += speech_ms[name] + SPEECH_PAUSE
        elif kind == compiler.TYPES['sleep']:
            t += op['value']
        elif kind == compiler.TYPES['msg']:
            for name in ('FONDONEGRO', 'RECLUTADOR', 'GAVIOTA'):
                if op['object'] == compiler.encode_name(name) and op['kind'] == 3:
                    active[name] = int(op['on'])
        elif kind in (compiler.TYPES['ret'], compiler.TYPES['end']):
            break
    return events, active, shown, stopped_at


def run_driver(driver, data):
    out = subprocess.run([str(driver), str(data)], capture_output=True, check=True).stdout.decode()
    events, states = [], {}
    for line in out.splitlines():
        parts = line.split()
        if parts[0] == 'E' and parts[2] in ('run', 'input', 'native', 'movie', 'sound', 'speech'):
            detail = ' '.join(parts[3:])
            if parts[2] == 'input' and parts[3] == '1':
                continue
            events.append((parts[2], int(parts[1]), detail))
        elif parts[0] == 'S':
            states[parts[1]] = dict(p.split('=') for p in parts[2:])
    return events, states


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--data', type=Path, required=True)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()

    expected, active, shown, stopped_at = predict(args.game, args.data)
    actual, states = run_driver(args.driver, args.data)
    mismatches = [dict(index=i, expected=e, actual=a)
                  for i, (e, a) in enumerate(zip(expected, actual)) if e != a]
    mismatches += [dict(index=len(expected), expected=None, actual=a) for a in actual[len(expected):]]
    mismatches += [dict(index=i, expected=e, actual=None) for i, e in enumerate(expected) if i >= len(actual)]
    state_errors = []
    for name, value in active.items():
        if states[name].get('active') != str(value):
            state_errors.append(dict(object=name, expected_active=value, actual=states[name]))
    for name, value in shown.items():
        if states[name].get('shown') != str(value):
            state_errors.append(dict(object=name, expected_shown=value, actual=states[name]))
    report = dict(
        driver_sha256=hashlib.sha256(args.driver.read_bytes()).hexdigest(),
        events=len(expected), stopped_at_show_jim_ms=stopped_at,
        mismatching_events=len(mismatches), state_errors=state_errors,
        first_mismatches=mismatches[:10], timeline=[list(e) for e in actual])
    text = json.dumps(report, indent=2)
    print(text)
    if args.output:
        args.output.write_text(text + '\n')
    return 1 if mismatches or state_errors else 0


if __name__ == '__main__':
    sys.exit(main())
