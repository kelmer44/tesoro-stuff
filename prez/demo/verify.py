#!/usr/bin/env python3
"""Run raw x86-32 bytes of sum_clamped in Unicorn and compare with a Python hypothesis."""
import argparse
import random
import struct
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from unicorn import UC_ARCH_X86, UC_HOOK_CODE, UC_MODE_32, Uc
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP

CODE, STACK, DATA, RET = 0x10000000, 0x20000000, 0x30000000, 0x40000000
PAGE = 0x1000

def s32(x):
    x &= 0xFFFFFFFF
    return x - (1 << 32) if x & 0x80000000 else x

def hyp_faithful(a, lo, hi):
    # Same branch order as the original C.
    total = 0
    for v in a:
        if v < lo:
            v = lo
        elif v > hi:
            v = hi
        total = s32(total + v)
    return total

def hyp_plausible(a, lo, hi):
    # Looks equivalent, differs when lo > hi.
    return s32(sum(min(max(v, lo), hi) for v in a))

HYPOTHESES = {"faithful": hyp_faithful, "plausible": hyp_plausible}

def disassemble(code, limit):
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    for i, ins in enumerate(md.disasm(code, 0)):
        if limit and i >= limit:
            print("...")
            break
        print(f"{ins.address:04x}:  {ins.bytes.hex(' '):<24} {ins.mnemonic} {ins.op_str}")

class Runner:
    def __init__(self, code):
        self.code = code
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.uc.mem_map(CODE, (len(code) + PAGE - 1) // PAGE * PAGE)
        self.uc.mem_write(CODE, code)
        self.uc.mem_map(STACK, 0x10000)
        self.uc.mem_map(DATA, 0x10000)
        self.uc.mem_map(RET, PAGE)
        self.uc.hook_add(UC_HOOK_CODE, self._count)
        self.steps = 0

    def _count(self, uc, address, size, user):
        self.steps += 1

    def call(self, a, n, lo, hi):
        uc = self.uc
        uc.mem_write(DATA, b"".join(struct.pack("<i", v) for v in a) or b"\0\0\0\0")
        # cdecl: args pushed right to left, then the return address.
        args = [DATA, n & 0xFFFFFFFF, lo & 0xFFFFFFFF, hi & 0xFFFFFFFF]
        esp = STACK + 0x8000
        for v in reversed(args):
            esp -= 4
            uc.mem_write(esp, struct.pack("<I", v))
        esp -= 4
        uc.mem_write(esp, struct.pack("<I", RET))
        uc.reg_write(UC_X86_REG_ESP, esp)
        self.steps = 0
        uc.emu_start(CODE, RET, count=1_000_000)
        return s32(uc.reg_read(UC_X86_REG_EAX)), self.steps

def make_cases():
    rng = random.Random(1234)
    cases = [
        ([], 0, -10, 10),
        ([5], 1, -10, 10),
        ([50, -50, 3], 3, -10, 10),
        ([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 10, 3, 7),
        # lo > hi: the case a happy-path test would skip.
        ([1, 5, 9, 20], 4, 8, 2),
    ]
    for _ in range(300):
        n = rng.randint(0, 40)
        a = [rng.randint(-100, 100) for _ in range(n)]
        lo, hi = rng.randint(-60, 60), rng.randint(-60, 60)
        cases.append((a, n, lo, hi))
    return cases

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("binfile")
    ap.add_argument("--hypothesis", choices=HYPOTHESES, default="faithful")
    ap.add_argument("--happy-path-only", action="store_true",
                    help="only test lo <= hi, as a naive test suite would")
    ap.add_argument("--disasm", type=int, nargs="?", const=0, metavar="N",
                    help="print the Capstone disassembly (first N instructions) and exit")
    args = ap.parse_args()

    code = open(args.binfile, "rb").read()
    if args.disasm is not None:
        disassemble(code, args.disasm)
        return 0

    hyp = HYPOTHESES[args.hypothesis]
    runner = Runner(code)
    fails, total_steps = [], 0
    cases = make_cases()
    if args.happy_path_only:
        cases = [c for c in cases if c[2] <= c[3]]
    for a, n, lo, hi in cases:
        got, steps = runner.call(a, n, lo, hi)
        total_steps += steps
        want = hyp(a[:n], lo, hi)
        if got != want:
            fails.append((a, n, lo, hi, got, want))

    print(f"{args.binfile}: {len(code)} bytes, hypothesis={args.hypothesis}")
    print(f"{len(cases) - len(fails)}/{len(cases)} cases match, {total_steps / len(cases):.0f} instructions per call on average")
    for a, n, lo, hi, got, want in fails[:3]:
        print(f"  MISMATCH a={a[:n]} lo={lo} hi={hi}: emulator={got} hypothesis={want}")
    return 1 if fails else 0

if __name__ == "__main__":
    sys.exit(main())
