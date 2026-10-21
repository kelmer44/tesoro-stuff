#!/usr/bin/env python3
"""Read-only evidence for FANPRESENT dispatch in the verified Tesoro edition.

Reads executable strings/jump-table bytes and cross-references the script corpus.
Does not execute native code or simulate scenes, timing or script scheduling.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct

from trace_script import load_game, without_comment

EXPECTED_EXE = '0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef'


def inspect(game):
    exe = (game / 'TESORO.EXE').read_bytes()
    digest = hashlib.sha256(exe).hexdigest()
    if digest != EXPECTED_EXE:
        raise ValueError('Native addresses apply only to the verified TESORO.EXE')
    pe = struct.unpack_from('<I', exe, 0x3c)[0]
    if exe[:2] != b'MZ' or exe[pe:pe + 4] != b'PE\0\0':
        raise ValueError('Expected a PE executable')
    optional = pe + 24
    if struct.unpack_from('<H', exe, optional)[0] != 0x10b:
        raise ValueError('Expected PE32')
    base = struct.unpack_from('<I', exe, optional + 28)[0]
    section_table = optional + struct.unpack_from('<H', exe, pe + 20)[0]
    section_count = struct.unpack_from('<H', exe, pe + 6)[0]

    def read(va, size):
        rva = va - base
        for i in range(section_count):
            _, start, length, offset = struct.unpack_from('<IIII', exe, section_table + 40 * i + 8)
            if start <= rva and rva + size <= start + length:
                data = exe[offset + rva - start:offset + rva - start + size]
                if len(data) != size:
                    raise ValueError('Truncated PE section')
                return data
        raise ValueError(f'Address has no file-backed bytes: {va:#x}')

    def string(va):
        return read(va, 64).split(b'\0', 1)[0].decode('cp850')

    # Matching assembly: CMP EAX,5; JA default; JMP [EAX*4+0x4198b8].
    if read(0x4197a5, 3) != b'\x83\xf8\x05':
        raise ValueError('Native ordinal range guard differs')
    targets = struct.unpack('<6I', read(0x4198b8, 24))
    default = 0x4198b4
    if read(default, 4) != b'\x5e\xc2\x04\x00':
        raise ValueError('Default path is not POP ESI; RET 4')
    blocks, texts, audio = load_game(game)
    block_name, entry_label = string(0x42dab4), string(0x42d4b4)
    block = blocks[block_name]
    native = []
    speech, movies, sounds, waits = [], [], [], []
    for index, (line, source) in enumerate(block.lines):
        clean = without_comment(source)
        if index in block.native_ordinals:
            ordinal = block.native_ordinals[index]
            target = targets[ordinal] if ordinal < len(targets) else default
            native.append(dict(line=line, ordinal=ordinal, target=f'0x{target:08x}',
                               no_effect=target == default,
                               dispatch='jump_table' if ordinal < len(targets) else 'range_guard'))
        match = re.fullmatch(r'(\w+),\s*(\d+)', clean)
        if match:
            text_id = match[1].upper()
            if text_id not in texts:
                raise ValueError(f'Missing speech text {text_id}')
            speech.append(dict(line=line, text_id=text_id, participant=int(match[2]),
                               audio=audio.get(text_id)))
        match = re.fullmatch(r'play avi (\d+)', clean)
        if match:
            movies.append(dict(line=line, number=int(match[1])))
        match = re.fullmatch(r"play sound '(\w+)'", clean)
        if match:
            sounds.append(dict(line=line, name=match[1], audio=audio.get(match[1].upper())))
        match = re.fullmatch(r'sleep (\d+)', clean)
        if match:
            waits.append(dict(line=line, milliseconds=int(match[1])))
    return dict(
        limitation='Static evidence only; no original runtime, animation, movement or scheduler simulation.',
        exe_sha256=digest, image_base=f'0x{base:08x}', block=block_name,
        entry_label=entry_label, native_type=block.native_type,
        participants=block.participants, native_handler='0x004197a0',
        jump_table_address='0x004198b8', jump_table=[f'0x{x:08x}' for x in targets],
        native_calls=native,
        resolved_native_object_names={'0x0042e91c': string(0x42e91c),
                                      '0x0042da50': string(0x42da50)},
        speech=speech, movies=movies, sounds=sounds, waits=waits,
        explicit_wait_total_ms=sum(w['milliseconds'] for w in waits))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('game', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Write research output outside the original game directory')
    report = inspect(args.game)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    no_effect = [c['ordinal'] for c in report['native_calls'] if c['no_effect']]
    print(f"{report['block']}: {len(report['native_calls'])} native markers; "
          f"confirmed no-effect ordinals {no_effect}; {len(report['speech'])} speech requests")


if __name__ == '__main__':
    main()
