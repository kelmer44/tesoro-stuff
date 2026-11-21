"""Tiny Unicorn wrapper: run a flat 32-bit x86 function and read its result."""
import struct

from unicorn import UC_ARCH_X86, UC_HOOK_CODE, UC_MODE_32, Uc
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP

CODE, STACK, RET = 0x10000000, 0x20000000, 0x40000000
PAGE = 0x1000

def s32(x):
    x &= 0xFFFFFFFF
    return x - (1 << 32) if x & 0x80000000 else x

class Runner:
    def __init__(self, code):
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.uc.mem_map(CODE, (len(code) + PAGE - 1) // PAGE * PAGE)
        self.uc.mem_write(CODE, code)
        self.uc.mem_map(STACK, 0x10000)
        self.uc.mem_map(RET, PAGE)
        self.uc.hook_add(UC_HOOK_CODE, self._count)
        self.steps = 0

    def _count(self, uc, address, size, user):
        self.steps += 1

    def call(self, *args):
        # cdecl: push the arguments right to left, then the return address.
        esp = STACK + 0x8000
        for v in reversed(args):
            esp -= 4
            self.uc.mem_write(esp, struct.pack("<I", v & 0xFFFFFFFF))
        esp -= 4
        self.uc.mem_write(esp, struct.pack("<I", RET))
        self.uc.reg_write(UC_X86_REG_ESP, esp)
        self.steps = 0
        self.uc.emu_start(CODE, RET, count=1_000_000)
        return s32(self.uc.reg_read(UC_X86_REG_EAX)), self.steps
