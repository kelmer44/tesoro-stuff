#!/usr/bin/env python3
"""Historical startup driver for a locally instrumented ScummVM build.

The submitted engine deliberately excludes its C++ test class and runtime hook.
No game assets are changed. Uses SDL dummy output and project-local scratch.
"""
import argparse
import hashlib
import json
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
    parser.add_argument('--broken-movies', action='store_true',
                        help='Exercise invalid AVI headers and decoder-owned parse failure')
    parser.add_argument('--skip-movie', action='store_true')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.movies and args.broken_movies:
        parser.error('--movies and --broken-movies are mutually exclusive')
    binary, game, output = (p.resolve() for p in (args.scummvm, args.game, args.output))
    if not (binary.parent / 'engines' / 'tesoro' / 'startup_test.cpp').is_file():
        parser.error('This historical driver requires local startup instrumentation; '
                     'the production engine deliberately excludes it.')
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='.startup-', dir=output) as temp:
        config = Path(temp) / 'scummvm.ini'
        # Isolate the no-movie case from any movies installed under game/bin.
        data_path = Path(temp) / 'game'
        data_path.mkdir()
        for name in ('TESORO.EXE', 'TESORO.TRE', 'TESORO.DAT'):
            (data_path / name).symlink_to(game / name)
        if args.broken_movies:
            # Header accepted, then no supported video track: the decoder takes
            # ownership and frees the stream on failure. Also cover pre-validation.
            (data_path / 'data00.bin').write_bytes(b'RIFF' + struct.pack('<I', 4) + b'AVI ')
            (data_path / 'data01.bin').write_bytes(b'not an AVI')
        values = {
            'engineid': 'tesoro', 'gameid': 'tesoro', 'path': str(data_path),
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
        summary = next(line for line in transcript.splitlines() if 'TESORO_STARTUP_TEST_PASS' in line)
        sources = binary.parent / 'engines' / 'tesoro'
        identity = {
            'binary': str(binary),
            'binary_sha256': hashlib.sha256(binary.read_bytes()).hexdigest(),
            'source_sha256': {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                              for p in sorted(sources.iterdir())
                              if p.suffix in ('.cpp', '.h') or p.name in ('module.mk', 'configure.engine')},
            'movies': str(args.movies.resolve()) if args.movies else ('malformed fixtures' if args.broken_movies else 'absent'),
            'skip_movie': args.skip_movie,
            'result': summary,
            'limitation': 'Host integration and resource pixels; not a comparison with the running Windows original.',
        }
        (output / 'verification.json').write_text(json.dumps(identity, indent=2) + '\n')
        print(summary)


if __name__ == '__main__':
    main()
