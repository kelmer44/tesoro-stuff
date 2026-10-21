#!/usr/bin/env python3
"""Compare production AnimationPlayer with original x86 instructions in Unicorn.

No game process, guest OS or host filesystem is exposed to the emulated code.
The C++ command-line driver is local/ignored, not part of the submitted engine.
Requires Unicorn in a project-local Python environment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP,
    UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EFLAGS)

from inspect_tesoro import read_index, read_banks, u16

EXPECTED_EXE = '0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef'
FIELDS = ['flags', 'frame_started_at', 'cycle_duration', 'paused_at', 'first',
          'last', 'frame', 'movement_x', 'movement_y', 'loop_x', 'loop_y']


class NativeAnimation:
    """Only the four small original animation routines and an injected clock."""
    bank = 0x200000
    frames = 0x210000
    stack_top = 0x30f000
    return_address = 0x100000

    def __init__(self, exe):
        if hashlib.sha256(exe).hexdigest() != EXPECTED_EXE:
            raise ValueError('Wrong executable edition for the native addresses')
        pe = struct.unpack_from('<I', exe, 0x3c)[0]
        optional = pe + 24
        base = struct.unpack_from('<I', exe, optional + 28)[0]
        size = struct.unpack_from('<I', exe, optional + 56)[0]
        table = optional + struct.unpack_from('<H', exe, pe + 20)[0]
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.uc.mem_map(base, (size + 4095) & ~4095)
        self.uc.mem_write(base, exe[:0x400])
        for i in range(struct.unpack_from('<H', exe, pe + 6)[0]):
            _, va, length, offset = struct.unpack_from('<IIII', exe, table + 40 * i + 8)
            if length:
                self.uc.mem_write(base + va, exe[offset:offset + length])
        self.uc.mem_map(self.return_address, 0x1000)
        self.uc.mem_map(self.bank, 0x100000)
        self.uc.mem_map(0x300000, 0x10000)
        self.now = 0
        self.uc.hook_add(UC_HOOK_CODE, self.clock, begin=0x413a10, end=0x413a10)

    def clock(self, uc, address, size, data):
        sp = uc.reg_read(UC_X86_REG_ESP)
        target = struct.unpack('<I', uc.mem_read(sp, 4))[0]
        uc.reg_write(UC_X86_REG_EAX, self.now)
        uc.reg_write(UC_X86_REG_ESP, sp + 4)
        uc.reg_write(UC_X86_REG_EIP, target)

    def load(self, frames, interpolate):
        self.uc.mem_write(self.bank, bytes(64))
        self.uc.mem_write(self.bank + 0x18, struct.pack('<I', 8 if interpolate else 0))
        self.uc.mem_write(self.bank + 0x1c, struct.pack('<I', self.frames))
        self.uc.mem_write(self.bank + 0x22, struct.pack('<H', len(frames)))
        for i, (image, dx, dy, duration) in enumerate(frames):
            raw = bytearray(36)
            struct.pack_into('<hhH', raw, 4, dx, dy, duration)
            struct.pack_into('<I', raw, 24, 0 if image < 0 else 1)
            self.uc.mem_write(self.frames + i * 36, bytes(raw))

    def call(self, operation):
        op, now, *args = operation
        self.now = now & 0xffffffff
        targets = {'p': 0x401740, 'u': 0x401860, 'a': 0x401a50, 'r': 0x401a70}
        for register in (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
                         UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP):
            self.uc.reg_write(register, 0)
        self.uc.reg_write(UC_X86_REG_EFLAGS, 2)
        self.uc.reg_write(UC_X86_REG_ECX, self.bank)
        stack = [self.return_address] + [v & 0xffffffff for v in args]
        self.uc.mem_write(self.stack_top, struct.pack('<' + 'I' * len(stack), *stack))
        self.uc.reg_write(UC_X86_REG_ESP, self.stack_top)
        self.uc.emu_start(targets[op], self.return_address, count=1000000)
        if self.uc.reg_read(UC_X86_REG_EIP) != self.return_address:
            raise ValueError('Original routine exceeded the instruction bound')
        raw = self.uc.mem_read(self.bank, 64)
        return [struct.unpack_from('<I', raw, offset)[0] for offset in (0x18, 8, 12, 16)] + [
            struct.unpack_from('<H', raw, offset)[0] for offset in (0x24, 0x26, 0x30)] + [
            struct.unpack_from('<h', raw, offset)[0] for offset in (0x28, 0x2a, 0x2c, 0x2e)]


def banks(game):
    data = (game / 'TESORO.DAT').read_bytes()
    for entry in read_index((game / 'TESORO.TRE').read_bytes(), data):
        if Path(entry['name']).suffix.lower() not in ('.lvl', '.anm'):
            continue
        start = entry['offset'] + 4
        payload = data[start:start + entry['size']]
        bank_start = 16 if payload[:4] == b'LVL\1' else 0
        for index, bank in enumerate(read_banks(payload)):
            at = bank_start + 28
            frames = []
            for _ in range(bank['frames']):
                dx, dy, duration, image = struct.unpack_from('<hhHh', payload, at + 4)
                frames.append([image, dx, dy, duration])
                at += 24 + u16(payload, at + 22)
            if not bank['font'] and frames and any(f[0] >= 0 for f in frames):
                origin = frames[0][1:3]
                for f in frames:
                    f[1:3] = [(v - o + 32768) % 65536 - 32768 for v, o in zip(f[1:3], origin)]
                yield f"{entry['name']}:{index}:{bank['name']}", payload[bank_start + 24] == 1, frames
            bank_start = bank['end']


def operations(frames, first, last, loop, cache, clock=100):
    duration = frames[first][3]
    times = sorted({0, 1, max(duration - 1, 0), duration, duration + 1, 250, 1000})
    result = [('p', clock, first, last, int(loop), int(cache))]
    result += [('u', (clock + t) & 0xffffffff) for t in times]
    pause = clock + max(times) + 100
    result += [('a', pause & 0xffffffff), ('u', (pause + 80) & 0xffffffff),
               ('r', (pause + 200) & 0xffffffff), ('u', (pause + 201) & 0xffffffff),
               ('u', (pause + 5000) & 0xffffffff),
               ('p', (pause + 6000) & 0xffffffff, first, last, int(loop), int(cache)),
               ('u', (pause + 6001) & 0xffffffff)]
    return result


def compare(game, driver):
    cases = []
    corpus_banks = 0
    for name, interpolate, frames in banks(game):
        corpus_banks += 1
        starts = [i for i, f in enumerate(frames) if f[0] >= 0 and (i == 0 or frames[i - 1][0] < 0)]
        for first in starts:
            for loop in (False, True):
                for cache in (False, True):
                    cases.append((f'{name}/start={first}/loop={loop}/cache={cache}',
                                  interpolate, frames, operations(frames, first, -1, loop, cache)))
    synthetic = [
        ('negative-interpolation', True, [[0, 0, 0, 7], [1, -20, 13, 9], [-1, -30, 20, 0]]),
        ('movement-wrap', False, [[0, -30000, 30000, 1], [1, 30000, -30000, 1], [-1, 31000, -31000, 0]]),
        ('zero-duration-nonloop', False, [[0, 0, 0, 0], [1, 10, 10, 5], [-1, 20, 20, 0]]),
        ('zero-duration-interpolation', True, [[0, 0, 0, 0], [1, 10, 10, 5], [-1, 20, 20, 0]]),
        ('internal-zero-interpolation', True, [[0, 0, 0, 7], [1, 10, 10, 0], [2, 20, 20, 9], [-1, 30, 30, 0]]),
        ('zero-time-loop', False, [[0, 0, 0, 0], [-1, 10, 10, 0]]),
        ('single-frame', True, [[0, 12, -12, 42]]),
        ('all-visible', True, [[0, 0, 0, 42], [1, 10, -10, 42], [2, 20, -20, 42]]),
    ]
    for name, interpolate, frames in synthetic:
        for loop in (False, True):
            for cache in (False, True):
                cases.append((f'{name}/loop={loop}/cache={cache}', interpolate, frames,
                              operations(frames, 0, -1, loop, cache)))
        cases.append((f'{name}/clock-wrap', interpolate, frames,
                      operations(frames, 0, -1, False, False, clock=0xffffff00)))
        cases.append((f'{name}/explicit-last', interpolate, frames,
                      operations(frames, 0, len(frames) - 1, False, False)))

    lines = []
    for _, interpolate, frames, ops in cases:
        lines.append(f'{len(frames)} {int(interpolate)}')
        lines.extend(' '.join(map(str, f)) for f in frames)
        lines.append(str(len(ops)))
        lines.extend(' '.join(map(str, op)) for op in ops)
    process = subprocess.run([str(driver.resolve())], input='\n'.join(lines) + '\n',
                             text=True, capture_output=True, check=True, timeout=60)
    output = iter(process.stdout.splitlines())
    executable = (game / 'TESORO.EXE').read_bytes()
    original = NativeAnimation(executable)
    compared = rejected = passed = 0
    rejected_cases = []
    for name, interpolate, frames, ops in cases:
        original.load(frames, interpolate)
        skip = False
        failure = None
        for op in ops:
            row = next(output).split()
            if row[0] != op[0]:
                raise ValueError('Driver output lost operation alignment')
            if row[1] == '0':
                rejected += 1
                skip = True
            if skip:
                if failure is None:
                    try:
                        original.call(op)
                    except (UcError, ValueError) as error:
                        failure = dict(operation=op, reason=str(error))
                        # Do not carry emulator exception state into another case.
                        original = NativeAnimation(executable)
                continue
            native = original.call(op)
            cpp = list(map(int, row[2:]))
            if native != cpp:
                mismatch = {field: {'original': a, 'cpp': b}
                            for field, a, b in zip(FIELDS, native, cpp) if a != b}
                raise ValueError(json.dumps(dict(case=name, operation=op, mismatch=mismatch)))
            compared += 1
        if not skip:
            passed += 1
        else:
            rejected_cases.append(dict(case=name, native_failure=failure))
    if next(output, None) is not None:
        raise ValueError('Unexpected extra driver output')
    for case in rejected_cases:
        if case['native_failure'] is None:
            raise ValueError(f"C++ rejected a sequence completed by the original: {case['case']}")
    return dict(exe_sha256=EXPECTED_EXE, corpus_banks=corpus_banks, scenarios=len(cases),
                driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest(),
                comparison_tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                passed_scenarios=passed, compared_states=compared, rejected_play_requests=rejected,
                rejected_cases=rejected_cases,
                routines=['0x00401740', '0x00401860', '0x00401a50', '0x00401a70'],
                limitation='Isolated original routines; no whole-game scene or visual fidelity claim.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep output outside the original game directory')
    report = compare(args.game, args.driver)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'rejected_cases'}))
