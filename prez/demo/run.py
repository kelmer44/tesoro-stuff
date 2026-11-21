#!/usr/bin/env python3
"""Run the raw x86-32 bytes of a function on integer arguments, live.
Example: run.py build/sum_to_O0.bin 10"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from emu import Runner  # noqa: E402

def main():
    if len(sys.argv) < 2:
        sys.exit("usage: run.py file.bin [int args...]")
    args = [int(a) for a in sys.argv[2:]]
    result, steps = Runner(open(sys.argv[1], "rb").read()).call(*args)
    print(f"f({', '.join(map(str, args))}) = {result}   [{steps} instructions]")

if __name__ == "__main__":
    main()
