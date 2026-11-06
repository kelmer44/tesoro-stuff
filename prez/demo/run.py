#!/usr/bin/env python3
"""Run the raw x86-32 bytes of sum_clamped on chosen inputs, live."""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from verify import Runner  # noqa: E402

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("binfile")
    ap.add_argument("--array", default="1,2,3,4,5,6,7,8,9,10", help="comma-separated numbers")
    ap.add_argument("--lo", type=int, default=0)
    ap.add_argument("--hi", type=int, default=100)
    args = ap.parse_args()

    a = [int(x) for x in args.array.split(",") if x.strip()]
    code = open(args.binfile, "rb").read()
    result, steps = Runner(code).call(a, len(a), args.lo, args.hi)
    print(f"sum_clamped({a}, n={len(a)}, lo={args.lo}, hi={args.hi}) = {result}   [{steps} instructions]")

if __name__ == "__main__":
    main()
