#!/usr/bin/env python3
"""Minimal Unicorn harness for TESORO.EXE (x86-32): maps the PE sections of the verified executable,
lets a test call a function with chosen arguments and stub imports through patched IAT slots.
Measurement tool only; nothing here is part of the engine.

    from emu import Emu
    e = Emu(); e.stub_import(0x44426c, lambda emu: tick)   # GetTickCount slot
    eax = e.call(0x413b00, [min, max], conv="stdcall")
"""
import hashlib, struct
from pathlib import Path
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UcError
from unicorn.x86_const import *

EXE = Path("/Users/gabriel/Desktop/ags/TESORO/TESORO.EXE")
SHA = "0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef"
BASE = 0x400000
STACK_TOP = 0x00200000
RET_MAGIC = 0x7ffff000          # return address that stops emulation
STUB_BASE = 0x7f000000

class Emu:
    def __init__(self):
        raw = EXE.read_bytes(); assert hashlib.sha256(raw).hexdigest() == SHA, "wrong executable"
        pe = struct.unpack_from("<I", raw, 0x3c)[0]
        n = struct.unpack_from("<H", raw, pe + 6)[0]; opt = struct.unpack_from("<H", raw, pe + 20)[0]
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.uc.mem_map(BASE, 0x1000, 3)  # headers page
        self.uc.mem_write(BASE, raw[:0x1000])
        for i in range(n):
            o = pe + 24 + opt + 40 * i
            vs, va, rs, rp = struct.unpack_from("<IIII", raw, o + 8)
            size = (max(vs, rs) + 0xfff) & ~0xfff
            self.uc.mem_map(BASE + va, size, 7)
            self.uc.mem_write(BASE + va, raw[rp:rp + rs])
        self.uc.mem_map(STACK_TOP - 0x100000, 0x100000, 3)
        self.uc.mem_map(RET_MAGIC & ~0xfff, 0x1000, 5)
        self.uc.mem_map(STUB_BASE, 0x10000, 5)
        self.stubs = {}
        self.uc.hook_add(UC_HOOK_CODE, self._hook, begin=STUB_BASE, end=STUB_BASE + 0x10000)
        self.uc.hook_add(UC_HOOK_CODE, self._stop, begin=RET_MAGIC, end=RET_MAGIC + 1)

    def _hook(self, uc, addr, size, _):
        fn, nargs = self.stubs[addr]
        esp = uc.reg_read(UC_X86_REG_ESP); ret = struct.unpack("<I", uc.mem_read(esp, 4))[0]
        args = [struct.unpack("<I", uc.mem_read(esp + 4 + 4 * k, 4))[0] for k in range(nargs)]
        uc.reg_write(UC_X86_REG_EAX, fn(self, *args) & 0xffffffff)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * nargs); uc.reg_write(UC_X86_REG_EIP, ret)   # stdcall stub

    def _stop(self, uc, addr, size, _): uc.emu_stop()

    def stub_import(self, iat_slot, fn, nargs=0):
        """Point the IAT slot at a fake stdcall function implemented in Python."""
        a = STUB_BASE + 0x10 * (len(self.stubs) + 1)
        self.stubs[a] = (fn, nargs); self.uc.mem_write(a, b"\x90")
        self.uc.mem_write(iat_slot, struct.pack("<I", a))

    def hook_function(self, addr, fn, nargs=0):
        """Replace the code at addr (a cdecl CRT function) by a Python function; the caller cleans the stack."""
        def h(uc, a, size, _):
            esp = uc.reg_read(UC_X86_REG_ESP); ret = struct.unpack("<I", uc.mem_read(esp, 4))[0]
            args = [struct.unpack("<I", uc.mem_read(esp + 4 + 4 * k, 4))[0] for k in range(nargs)]
            uc.reg_write(UC_X86_REG_EAX, (fn(self, *args) or 0) & 0xffffffff)
            uc.reg_write(UC_X86_REG_ESP, esp + 4); uc.reg_write(UC_X86_REG_EIP, ret)
        self.uc.hook_add(UC_HOOK_CODE, h, begin=addr, end=addr)

    def read(self, addr, n): return bytes(self.uc.mem_read(addr, n))
    def write(self, addr, data): self.uc.mem_write(addr, bytes(data))
    def u32(self, addr): return struct.unpack("<I", self.read(addr, 4))[0]

    def call(self, addr, args=(), conv="cdecl", this=None, max_insns=2_000_000):
        uc = self.uc; esp = STACK_TOP - 0x1000
        for a in reversed(args): esp -= 4; uc.mem_write(esp, struct.pack("<I", a & 0xffffffff))
        esp -= 4; uc.mem_write(esp, struct.pack("<I", RET_MAGIC))
        uc.reg_write(UC_X86_REG_ESP, esp); uc.reg_write(UC_X86_REG_EBP, 0)
        if this is not None: uc.reg_write(UC_X86_REG_ECX, this)
        uc.emu_start(addr, RET_MAGIC, count=max_insns)
        return uc.reg_read(UC_X86_REG_EAX)

    def edx_eax(self):
        return (self.uc.reg_read(UC_X86_REG_EDX) << 32) | self.uc.reg_read(UC_X86_REG_EAX)
