#!/usr/bin/env python3
"""Check a one-argument function's machine code against a hypothesis typed on the command line.
Example: verify.py build/sum_to_O2.bin --expr "n*(n+1)//2"   (the variable is called n)"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ensure_venv  # noqa: E402,F401
from capstone import CS_ARCH_X86, CS_MODE_32, Cs  # noqa: E402
from emu import Runner, s32  # noqa: E402

SAFE = {"__builtins__": {}, "min": min, "max": max, "abs": abs, "sum": sum, "range": range}

def make_hypothesis(expr):
    code = compile(expr, "<hypothesis>", "eval")
    return lambda n: s32(eval(code, dict(SAFE, n=n)))

def test_inputs(happy_path_only):
    ns = list(range(0, 101)) + [500, 1000]
    if not happy_path_only:
        ns += list(range(-1, -31, -1))  # negative n: the awkward case
    return ns

def disassemble(code):
    for ins in Cs(CS_ARCH_X86, CS_MODE_32).disasm(code, 0):
        print(f"{ins.address:04x}:  {ins.bytes.hex(' '):<24} {ins.mnemonic} {ins.op_str}")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("binfile")
    ap.add_argument("--expr", help='Python expression in n, e.g. "n*(n+1)//2"')
    ap.add_argument("--happy-path-only", action="store_true", help="only test n >= 0")
    ap.add_argument("--disasm", action="store_true", help="print the Capstone disassembly and exit")
    args = ap.parse_args()

    code = open(args.binfile, "rb").read()
    if args.disasm:
        disassemble(code)
        return 0
    if not args.expr:
        ap.error('--expr is required (or use --disasm). Example: --expr "sum(range(1, n+1))" (the variable is called n)')

    hyp = make_hypothesis(args.expr)
    runner = Runner(code)
    fails, steps_at = [], {}
    ns = test_inputs(args.happy_path_only)
    for n in ns:
        got, steps = runner.call(n)
        steps_at[n] = steps
        want = hyp(n)
        if got != want:
            fails.append((n, got, want))

    print(f"{args.binfile}: {len(code)} bytes   hypothesis: {args.expr}")
    print(f"{len(ns) - len(fails)}/{len(ns)} inputs match   (n = {min(ns)} to {max(ns)})")
    print(f"for reference, n=1000 took {steps_at[1000]} instructions")
    for n, got, want in fails[:3]:
        print(f"  MISMATCH n={n}: emulator={got} hypothesis={want}")
    return 1 if fails else 0

if __name__ == "__main__":
    sys.exit(main())
