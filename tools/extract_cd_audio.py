#!/usr/bin/env python3
"""Extract this single-BIN mixed-mode Tesoro CD to lossless WAV track files.

Keeps original physical track numbers. Does not change the BIN/CUE, normalize,
resample, byte-swap or remove silence/pregaps. Rejects unsupported CUE layouts.
"""
import argparse
import hashlib
import json
import math
from pathlib import Path
import re
import struct
import wave

SECTOR_BYTES = 2352
FRAMES_PER_SECTOR = 588


def sha256(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        while data := stream.read(1024 * 1024):
            digest.update(data)
    return digest.hexdigest()


def parse_cue(cue, binary):
    tracks = []
    files = []
    for line in cue.read_text(encoding='utf-8-sig').splitlines():
        line = line.strip()
        if not line or line.startswith('REM '):
            continue
        match = re.fullmatch(r'FILE "([^"]+)" BINARY', line)
        if match:
            files.append(match[1])
            continue
        match = re.fullmatch(r'TRACK (\d+) (MODE1/2352|AUDIO)', line)
        if match:
            tracks.append(dict(track=int(match[1]), mode=match[2]))
            continue
        match = re.fullmatch(r'INDEX 01 (\d+):(\d+):(\d+)', line)
        if match and tracks:
            minutes, seconds, frames = map(int, match.groups())
            if seconds >= 60 or frames >= 75 or 'start_sector' in tracks[-1]:
                raise ValueError('Invalid or duplicate INDEX 01')
            tracks[-1]['start_sector'] = (minutes * 60 + seconds) * 75 + frames
            continue
        raise ValueError(f'Unsupported CUE statement: {line}')
    if files != [binary.name]:
        raise ValueError('Expected one BINARY FILE matching the supplied BIN name')
    size = binary.stat().st_size
    if size % SECTOR_BYTES:
        raise ValueError('BIN is not sector-aligned')
    if not tracks or tracks[0] != dict(track=1, mode='MODE1/2352', start_sector=0):
        raise ValueError('Expected a data track starting at sector zero')
    for index, track in enumerate(tracks):
        if track['track'] != index + 1 or 'start_sector' not in track:
            raise ValueError('Missing index or non-contiguous track numbers')
        end = tracks[index + 1]['start_sector'] if index + 1 < len(tracks) else size // SECTOR_BYTES
        if not 0 <= track['start_sector'] < end <= size // SECTOR_BYTES:
            raise ValueError('Invalid track boundary')
        if index and track['mode'] != 'AUDIO':
            raise ValueError('Expected audio tracks after the first data track')
        track.update(sectors=end - track['start_sector'], end_sector=end)
    return tracks


def sample_stats(binary, track):
    with binary.open('rb') as stream:
        sector = track['start_sector'] + min(5 * 75, track['sectors'] // 2)
        stream.seek(sector * SECTOR_BYTES)
        data = stream.read(min(44100 * 4, (track['end_sector'] - sector) * SECTOR_BYTES))
    results = {}
    for label, endian in [('little', '<'), ('big', '>')]:
        samples = struct.unpack(endian + str(len(data) // 2) + 'h', data)[::2]
        rms = math.sqrt(sum(x * x for x in samples) / len(samples))
        delta = math.sqrt(sum((y - x) ** 2 for x, y in zip(samples, samples[1:])) / (len(samples) - 1))
        results[label] = dict(rms=rms, delta_rms=delta, delta_ratio=delta / rms if rms else None)
    return results


def extract(cue, binary, output):
    tracks = parse_cue(cue, binary)
    output.mkdir(parents=True, exist_ok=True)
    targets = [output / f"track{track['track']:02d}.wav" for track in tracks[1:]]
    if any(path.exists() or path.with_suffix('.wav.part').exists() for path in targets):
        raise FileExistsError('Output tracks already exist; choose a new output directory')
    report = dict(cue=str(cue.resolve()), binary=str(binary.resolve()),
        cue_text=cue.read_text(encoding='utf-8-sig'),
        cue_sha256=sha256(cue), binary_sha256=sha256(binary),
        source_bytes=binary.stat().st_size, sector_bytes=SECTOR_BYTES,
        source_audio_byte_order='little-endian, consistent with sampled waveform continuity; bytes preserved unchanged',
        wav_format=dict(channels=2, rate=44100, bits=16), tracks=tracks)
    with binary.open('rb') as source:
        for track, target in zip(tracks[1:], targets):
            track['sample_stats'] = sample_stats(binary, track)
            stats = track['sample_stats']
            if (stats['little']['delta_ratio'] is not None and stats['big']['delta_ratio'] is not None
                    and stats['little']['delta_ratio'] > stats['big']['delta_ratio']):
                raise ValueError(f"Track {track['track']} does not match the expected byte order")
            remaining = track['sectors'] * SECTOR_BYTES
            source.seek(track['start_sector'] * SECTOR_BYTES)
            digest = hashlib.sha256()
            temporary = target.with_suffix('.wav.part')
            with wave.open(str(temporary), 'wb') as wav:
                wav.setnchannels(2)
                wav.setsampwidth(2)
                wav.setframerate(44100)
                while remaining:
                    data = source.read(min(1024 * 1024, remaining))
                    if not data:
                        raise ValueError('Unexpected end of BIN')
                    digest.update(data)
                    wav.writeframesraw(data)
                    remaining -= len(data)
            temporary.rename(target)
            with wave.open(str(target), 'rb') as wav:
                assert wav.getnframes() == track['sectors'] * FRAMES_PER_SECTOR
                check = hashlib.sha256()
                while data := wav.readframes(65536):
                    check.update(data)
                if check.hexdigest() != digest.hexdigest():
                    raise ValueError(f'Extracted PCM differs from BIN: {target}')
            track.update(output=str(target.resolve()), frames=track['sectors'] * FRAMES_PER_SECTOR,
                duration_seconds=track['sectors'] / 75,
                pcm_sha256=digest.hexdigest(), wav_sha256=sha256(target))
            print(f"Track {track['track']:02d}: {track['duration_seconds']:.2f}s; PCM verified")
    (output / 'manifest.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cue', type=Path)
    parser.add_argument('binary', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    extract(args.cue, args.binary, args.output)
