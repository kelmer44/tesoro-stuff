#!/usr/bin/env python3
"""Compare production LVL parsing, room entry and scenery with isolated x86 code.

Requires the ignored C++ driver, plus Unicorn in the local animation environment.
No original process or Windows environment is started. Renderer queue submission
is intercepted; this does not compare pixels, draw ordering or complete scenes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from unicorn import UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP,
    UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EFLAGS)
from compare_animation_native import NativeAnimation, EXPECTED_EXE
from inspect_tesoro import read_index, read_banks, read_scene_objects, u16, u32


def signed16(value):
    return (value + 32768) % 65536 - 32768


def scenes(game):
    data = (game / 'TESORO.DAT').read_bytes()
    for entry in read_index((game / 'TESORO.TRE').read_bytes(), data):
        if not entry['name'].lower().endswith('.lvl'):
            continue
        payload = data[entry['offset'] + 4:entry['offset'] + 4 + entry['size']]
        banks = read_banks(payload)
        start = 16
        for bank in banks:
            bank['interpolate'] = not bank['font'] and payload[start + 24] == 1
            at = start + 28
            bank['records'] = []
            for _ in range(bank['frames']):
                frame = list(struct.unpack_from('<12h', payload, at))
                bank['records'].append(frame)
                at += 24 + u16(payload, at + 22)
            if not bank['font'] and bank['records']:
                ox, oy = bank['records'][0][2:4]
                for frame in bank['records']:
                    frame[2:4] = [signed16(frame[2] - ox), signed16(frame[3] - oy)]
            start = bank['end']
        yield entry['name'], payload, banks, read_scene_objects(payload, banks)


class NativeScene(NativeAnimation):
    object = 0x280000
    record = 0x281000
    parameters = 0x282000
    group = 0x283000
    bank_table = 0x284000
    camera = 0x285000
    scene = 0x286000
    floor = 0x287000

    def __init__(self, exe):
        super().__init__(exe)
        self.uc.mem_map(0x500000, 0x1000000)
        self.uc.mem_map(0x3000000, 0x100000)
        for address in (0x40db40, 0x40f650):
            self.uc.hook_add(UC_HOOK_CODE, self.allocate, begin=address, end=address)
        self.uc.hook_add(UC_HOOK_CODE, self.submit, begin=0x411f80, end=0x411f80)
        self.uc.hook_add(UC_HOOK_CODE, self.find_floor, begin=0x40f680, end=0x40f680)
        self.heap = 0x290000
        self.sprite = None

    def ret(self, value=0, arguments=0):
        sp = self.uc.reg_read(UC_X86_REG_ESP)
        target = u32(self.uc.mem_read(sp, 4), 0)
        self.uc.reg_write(UC_X86_REG_EAX, value)
        self.uc.reg_write(UC_X86_REG_ESP, sp + 4 + arguments)
        self.uc.reg_write(UC_X86_REG_EIP, target)

    def allocate(self, uc, address, size, data):
        sp = uc.reg_read(UC_X86_REG_ESP)
        length = u32(uc.mem_read(sp + 4, 4), 0)
        result = self.heap
        self.heap += (length + 15) & ~15
        if self.heap >= 0x2f0000:
            raise ValueError('Native object heap exceeded its private region')
        uc.mem_write(result, bytes(length))
        self.ret(result)

    def submit(self, uc, address, size, data):
        sp = uc.reg_read(UC_X86_REG_ESP)
        x, y, depth, image, unused = struct.unpack('<iiIII', uc.mem_read(sp + 4, 20))
        self.sprite = [x, y, depth & 65535]
        self.sprite_image = self.images[image]
        self.ret(arguments=20)

    def find_floor(self, uc, address, size, data):
        self.ret(self.floor, arguments=8)

    def invoke(self, target, this=0, args=(), now=100, instruction_limit=1000000):
        self.now = now
        for reg in (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX,
                    UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP):
            self.uc.reg_write(reg, 0)
        self.uc.reg_write(UC_X86_REG_EFLAGS, 2)
        self.uc.reg_write(UC_X86_REG_ECX, this)
        values = [self.return_address] + [v & 0xffffffff for v in args]
        self.uc.mem_write(self.stack_top, struct.pack('<' + 'I' * len(values), *values))
        self.uc.reg_write(UC_X86_REG_ESP, self.stack_top)
        self.uc.emu_start(target, self.return_address, count=instruction_limit)
        if self.uc.reg_read(UC_X86_REG_EIP) != self.return_address:
            raise ValueError(f'Native call {target:#x} exceeded the instruction bound')
        return self.uc.reg_read(UC_X86_REG_EAX)

    def load_scene(self, banks):
        self.images = {}
        for i, bank in enumerate(banks):
            address = 0x500000 + i * 0x10000
            if i >= 256 or len(bank['records']) * 36 > 0x8000 or len(bank['images']) * 4 > 0x2000:
                raise ValueError('Native bank skeleton exceeds its private region')
            raw = bytearray(64)
            struct.pack_into('<I', raw, 0x18, 8 if bank['interpolate'] else 0)
            struct.pack_into('<I', raw, 0x1c, address + 0x1000)
            struct.pack_into('<HH', raw, 0x20, len(bank['images']), bank['frames'])
            self.uc.mem_write(address, bytes(raw))
            self.uc.mem_write(self.bank_table + i * 4, struct.pack('<I', address))
            for j, frame in enumerate(bank['records']):
                raw = bytearray(36)
                struct.pack_into('<12h', raw, 0, *frame)
                image = frame[5]
                if image >= 0:
                    wrapper = address + 0xa000 + image * 4
                    self.uc.mem_write(wrapper, struct.pack('<I', address + 0xc000 + image * 4))
                    self.images[address + 0xc000 + image * 4] = [i, image]
                    struct.pack_into('<I', raw, 24, wrapper)
                self.uc.mem_write(address + 0x1000 + j * 36, bytes(raw))

    def construct(self, obj, depth_base, target=0x40e5c0, reset_heap=True):
        if reset_heap:
            self.heap = 0x290000
        self.uc.mem_write(self.object, bytes(0x200))
        raw = struct.pack('<iihBB12sII', obj['x'], obj['y'], signed16(obj['field_08']),
                          obj['field_10'], len(obj['extra_bytes']),
                          obj['name'].encode('cp850'), obj['kind'], len(bytes.fromhex(obj['parameters_hex'])))
        self.uc.mem_write(self.record, raw)
        self.uc.mem_write(self.parameters, bytes.fromhex(obj['parameters_hex']) + bytes(obj['extra_bytes']))
        self.uc.mem_write(self.group, bytes(16))
        self.uc.mem_write(self.group + 8, struct.pack('<H', depth_base))
        self.invoke(target, self.object, (self.record, self.group, self.bank_table, self.parameters))

    def state(self):
        bank = u32(self.uc.mem_read(self.object + 0x38, 4), 0)
        raw = self.uc.mem_read(bank, 64)
        return [u32(raw, at) for at in (0x18, 8, 12, 16)] + [u16(raw, at) for at in (0x24, 0x26, 0x30)] + [
            struct.unpack_from('<h', raw, at)[0] for at in (0x28, 0x2a, 0x2c, 0x2e)]

    def name_key(self, name):
        self.uc.mem_write(self.record, name.encode('cp850') + b'\0')
        low = self.invoke(0x415c40, args=(self.record,))
        return low | self.uc.reg_read(UC_X86_REG_EDX) << 32


def compare(game, driver):
    corpus = list(scenes(game))
    input_data = b''.join(struct.pack('<I', len(p)) + p for _, p, _, _ in corpus)
    process = subprocess.run([str(driver.resolve())], input=input_data,
                             capture_output=True, check=True, timeout=120)
    output = iter(process.stdout.decode().splitlines())

    def row(tag):
        values = next(output).split()
        if values[0] != tag:
            raise ValueError(f'Expected {tag}, got {values}')
        return values[1:]

    def equal(actual, expected, context):
        if actual != expected:
            raise ValueError(json.dumps(dict(context=context, cpp=actual, expected=expected)))

    original = NativeScene((game / 'TESORO.EXE').read_bytes())
    totals = dict(scenes=len(corpus), banks=0, objects=0, scenery_objects=0,
                  scenery_states=0, bank_selections=0, camera_cases=0, name_keys=0,
                  scenery_queue_frames=0, queued_sprites=0)
    entries = {}
    for name, payload, banks, objects in corpus:
        equal(list(map(int, row('S'))), [len(banks), u32(payload, 4)], name)
        for b in banks:
            bank = row('B')
            equal(bank, [b['name'].encode('cp850').hex() or '-', str(b['frames']),
                         str(len(b['images'])), str(int(b['font'])), str(int(b['interpolate']))], name)
        totals['banks'] += len(banks)
        original.load_scene(banks)
        groups = []
        depth_base = 0
        for group_id in range(u32(payload, 4)):
            group_objects = [o for o in objects if o['group'] == group_id]
            # A group can be empty, so read its header independently of objects.
            at = banks[-1]['end'] if banks else 16
            for previous in groups:
                at += 40 + sum(32 + len(bytes.fromhex(o['parameters_hex'])) + len(o['extra_bytes'])
                               for o in previous['objects'])
            header = payload[at:at + 40]
            span, parallax = u16(header, 34), struct.unpack_from('<i', header, 36)[0]
            equal(row('G'), [str(group_id), header.hex(), str(depth_base), str(span), str(parallax)], name)
            groups.append(dict(objects=group_objects, base=depth_base, parallax=parallax))
            depth_base = (depth_base + span) & 65535
            for j, obj in enumerate(group_objects):
                key = original.name_key(obj['name'])
                expected = [str(group_id), str(j), obj['name'].encode('cp850').hex() or '-',
                            str(obj['x']), str(obj['y']), str(signed16(obj['field_08'])),
                            str(obj['field_10']), str(obj['kind']), obj['parameters_hex'] or '-',
                            bytes(obj['extra_bytes']).hex() or '-', str(key)]
                equal(row('O'), expected, f'{name}:{obj["name"]}')
                totals['objects'] += 1
                totals['name_keys'] += 1
        cpp_entry = list(map(int, row('E')))
        room = next((o for o in reversed(objects) if o['kind'] == 1), None)
        bounds = [-320, 239, 319, -240]
        if room and room['extra_bytes']:
            params = bytes.fromhex(room['parameters_hex'])
            floor_name = params[:12].split(b'\0', 1)[0].decode('cp850')
            matches = [(g, j, o) for g, group in enumerate(groups) for j, o in enumerate(group['objects'])
                       if original.name_key(o['name']) == original.name_key(floor_name)] if floor_name else []
            if len(matches) == 1:
                floor_group, floor_index, floor = matches[0]
                original.construct(room, groups[room['group']]['base'], 0x406980)
                original.uc.mem_write(original.object + 0x40, struct.pack('<I', original.scene))
                original.uc.mem_write(original.scene + 0x80, struct.pack('<I', original.camera))
                original.uc.mem_write(original.camera, bytes(64))
                original.uc.mem_write(original.floor, bytes(0x84))
                original.uc.mem_write(original.floor + 0x4c, struct.pack('<ii', floor['x'], floor['y']))
                original.invoke(0x4069e0, original.object)
                bounds = list(struct.unpack('<4i', original.uc.mem_read(original.camera + 0x1c, 16)))
                original.invoke(0x4021a0, original.camera, (floor['x'], floor['y'], 0))
                center = list(struct.unpack('<2i', original.uc.mem_read(original.camera + 0x2c, 8)))
                expected = [1, floor_group, floor_index, floor['x'], floor['y'], params[14]] + bounds + center
                equal(cpp_entry, expected, f'{name}:entry')
                entries[name] = dict(position=[floor['x'], floor['y']], camera=center, bounds=bounds,
                                     floor=floor['name'], music_cue=params[14])
            else:
                equal(cpp_entry, [0], f'{name}:entry')
        else:
            equal(cpp_entry, [0], f'{name}:entry')
        for g, group in enumerate(groups):
            for j, obj in enumerate(group['objects']):
                if obj['kind'] != 3:
                    continue
                original.construct(obj, group['base'])
                totals['scenery_objects'] += 1
                for now in (100, 101, 142, 143, 200, 1000):
                    original.invoke(0x401c50, original.object, now=now)
                    flags = u32(original.uc.mem_read(original.object + 0x68, 4), 0)
                    depth = u16(original.uc.mem_read(original.object + 0x5e, 2), 0)
                    hit = list(struct.unpack('<4i', original.uc.mem_read(original.object + 0x10, 16)))
                    # Native layout is left/top/right/bottom.
                    original.sprite = None
                    original.uc.mem_write(original.camera + 0x34, struct.pack('<ii', 320, 239))
                    if flags & 6 == 6:
                        original.invoke(0x40e6b0, original.object, (original.camera,), now=now)
                    sprite = [int(original.sprite is not None)] + (original.sprite or [0, 0, 0])
                    expected = [g, j, now, flags, depth] + original.state() + hit + sprite
                    equal(list(map(int, row('D'))), expected, f'{name}:{obj["name"]}@{now}')
                    totals['scenery_states'] += 1
                original.invoke(0x401d60, original.object, (0,))
                original.invoke(0x401d80, original.object, (0,))
                off = u32(original.uc.mem_read(original.object + 0x68, 4), 0)
                original.invoke(0x401d60, original.object, (1,))
                original.invoke(0x401d80, original.object, (1,))
                on = u32(original.uc.mem_read(original.object + 0x68, 4), 0)
                equal(list(map(int, row('F'))), [off, on], f'{name}:{obj["name"]}:flags')
                for k in range(len(obj['extra_bytes']) + 1):
                    bank = k % len(obj['extra_bytes'])
                    original.invoke(0x401de0, original.object, (bank, 0, 0, 0, 0), now=2000 + k)
                    equal(list(map(int, row('P'))), [bank, 1] + original.state(),
                          f'{name}:{obj["name"]}:bank={bank}')
                    totals['bank_selections'] += 1
        for g, group in enumerate(groups):
            original.uc.mem_write(original.camera, bytes(64))
            original.invoke(0x402200, original.camera, bounds)
            for j, (x, y) in enumerate(((0, 0), (-10000, 10000), (10000, -10000), (320, 240), (333, 239))):
                ok = original.invoke(0x402120, original.camera, (x, y)) & 255
                original.invoke(0x401fc0, original.camera, (group['parallax'],))
                center = list(struct.unpack('<2i', original.uc.mem_read(original.camera + 0x2c, 8)))
                view = list(struct.unpack('<2i', original.uc.mem_read(original.camera + 0x34, 8)))
                equal(list(map(int, row('C'))), [g, j, ok] + center + view, f'{name}:camera')
                totals['camera_cases'] += 1
        original.heap = 0x290000
        object_id = 0
        for g, group in enumerate(groups):
            linked = 0x3080000 + g * 0x2000
            original.uc.mem_write(linked, bytes(0x2000))
            original.uc.mem_write(linked, struct.pack('<I', linked + 0x40))
            original.uc.mem_write(linked + 0x48, struct.pack('<I', linked + 0x100))
            group['linked'] = linked
            for obj in group['objects']:
                if obj['kind'] != 3:
                    continue
                original.object = 0x3000000 + object_id * 0x200
                object_id += 1
                original.construct(obj, group['base'], reset_heap=False)
                original.uc.mem_write(original.group + 12, struct.pack('<I', linked))
                original.invoke(0x4157b0, original.object)
        original.uc.mem_write(original.camera, bytes(64))
        original.invoke(0x402200, original.camera, bounds)
        original.invoke(0x402120, original.camera, (320, 239))
        for now in (100, 143, 1000):
            sprites = []
            for group in groups:
                original.invoke(0x401fc0, original.camera, (group['parallax'],), now=now)
                node = u32(original.uc.mem_read(group['linked'] + 4, 4), 0)
                visited = 0
                while node:
                    visited += 1
                    if visited > object_id:
                        raise ValueError('Native group list did not terminate')
                    original.object = u32(original.uc.mem_read(node + 4, 4), 0)
                    flags = u32(original.uc.mem_read(original.object + 0x68, 4), 0)
                    if flags & 6 == 6:
                        original.invoke(0x401c50, original.object, now=now)
                        original.sprite = None
                        original.invoke(0x40e6b0, original.object, (original.camera,), now=now)
                        if original.sprite is not None:
                            sprites.append(original.sprite_image + original.sprite)
                    node = u32(original.uc.mem_read(node, 4), 0)
            equal(list(map(int, row('Q'))), [now, len(sprites)], f'{name}:scenery-queue@{now}')
            for sprite in sprites:
                equal(list(map(int, row('J'))), sprite, f'{name}:queued-sprite@{now}')
            totals['scenery_queue_frames'] += 1
            totals['queued_sprites'] += len(sprites)
    if next(output, None) is not None:
        raise ValueError('Unexpected extra output from the production driver')
    return dict(exe_sha256=EXPECTED_EXE, driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest(),
                comparison_tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                matched=totals, room_entries=entries,
                limitation='Isolated routines, injected clock and queue capture; no full scene, actor, scheduler or pixel comparison.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--game', type=Path, required=True)
    parser.add_argument('--driver', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--engine', type=Path, required=True, help='Active ScummVM source checkout')
    args = parser.parse_args()
    if args.output.resolve().is_relative_to(args.game.resolve()):
        parser.error('Keep reports outside the original game directory')
    report = compare(args.game, args.driver)
    report['source_sha256'] = {name: hashlib.sha256(
        (args.engine / 'engines' / 'tesoro' / name).read_bytes()).hexdigest()
        for name in ('scene.cpp', 'scene.h', 'resources.cpp', 'resources.h', 'animation.cpp', 'animation.h')}
    report['resource_sha256'] = {name: hashlib.sha256((args.game / name).read_bytes()).hexdigest()
                                 for name in ('TESORO.TRE', 'TESORO.DAT')}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(matched=report['matched'], harbour=report['room_entries'].get('Puerto.lvl'))))
