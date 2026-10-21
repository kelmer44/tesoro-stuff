#!/usr/bin/env python3
"""Run the active ScummVM startup loop against local original game data.

No game assets are changed. Uses SDL dummy display/audio and a temporary config.
"""
import argparse
import os
from pathlib import Path
import subprocess
import tempfile
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from inspect_tesoro import read_index, read_banks, decode_image


def validate_snapshots(game, output):
    """Compare C++ output with the independent Python RGB565/span decoder."""
    dat = (game / 'TESORO.DAT').read_bytes()
    entries = {e['name'].upper(): e for e in read_index((game / 'TESORO.TRE').read_bytes(), dat)}
    for filename, resource, bank_name in (
        ('startup-main.bmp', 'PRINCIPAL.LVL', 'FONDO'),
        ('startup-difficulty.bmp', 'NUEVO.LVL', 'NUEVO'),
        ('startup-censorship.bmp', 'NUEVO.LVL', 'CENSURA'),
    ):
        entry = entries[resource]
        payload = dat[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]
        bank = next(bank for bank in read_banks(payload) if bank['name'] == bank_name)
        expected = decode_image(payload, bank['images'][0])
        bmp = (output / filename).read_bytes()
        offset = struct.unpack_from('<I', bmp, 10)[0]
        width, height, planes, depth, compression = struct.unpack_from('<iiHHI', bmp, 18)
        if bmp[:2] != b'BM' or (width, height, planes, depth, compression) != (640, 480, 1, 24, 0):
            raise SystemExit(f'Unexpected screenshot format: {filename}')
        rgb = bytearray()
        for y in range(height - 1, -1, -1):
            row = bmp[offset + y * width * 3:offset + (y + 1) * width * 3]
            for x in range(0, len(row), 3):
                rgb.extend(row[x:x + 3][::-1])
        if rgb != expected:
            raise SystemExit(f'Screenshot differs from original resource decoder: {filename}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--scummvm', type=Path, required=True)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--movies', type=Path)
    parser.add_argument('--skip-movie', action='store_true')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    binary, game, output = (p.resolve() for p in (args.scummvm, args.game, args.output))
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='tesoro-startup-') as temp:
        config = Path(temp) / 'scummvm.ini'
        values = {
            'engineid': 'tesoro', 'gameid': 'tesoro', 'path': str(game),
            'platform': 'windows', 'language': 'es',
            'tesoro_startup_test': 'true',
            'tesoro_test_snapshot_path': str(output),
        }
        if args.movies:
            values['tesoro_movie_path'] = str(args.movies.resolve())
        if args.skip_movie:
            values['tesoro_test_skip_movie'] = 'true'
        config.write_text('[scummvm]\ngfx_mode=surfacesdl\nenable_unsupported_game_warning=false\nconfirm_exit=false\nfullscreen=false\nautosave_period=0\n'
                          f'savepath={temp}\n'
                          f'themepath={binary.parent / "gui/themes"}\n'
                          f'extrapath={binary.parent / "dists/engine-data"}\n'
                          '\n[tesoro-startup-test]\n'
                          + ''.join(f'{key}={value}\n' for key, value in values.items()))
        environment = dict(os.environ, SDL_VIDEODRIVER='dummy',
                           SDL_AUDIODRIVER='dummy', SDL_RENDER_DRIVER='software')
        log = output / 'runtime.log'
        with log.open('w') as stream:
            try:
                result = subprocess.run([str(binary), '--debuglevel=0', f'--config={config}', 'tesoro-startup-test'],
                                        cwd=binary.parent, env=environment, text=True,
                                        stdout=stream, stderr=subprocess.STDOUT, timeout=70)
            except subprocess.TimeoutExpired:
                raise SystemExit(f'Startup check timed out; see {log}')
        transcript = log.read_text()
        if result.returncode or 'TESORO_STARTUP_TEST_PASS' not in transcript:
            raise SystemExit(f'Startup check failed ({result.returncode}); see {log}')
        for name in ('startup-main.bmp', 'startup-censorship.bmp', 'startup-difficulty.bmp'):
            if not (output / name).is_file():
                raise SystemExit(f'Missing snapshot: {name}')
        validate_snapshots(game, output)
        print(next(line for line in transcript.splitlines() if 'TESORO_STARTUP_TEST_PASS' in line))


if __name__ == '__main__':
    main()
