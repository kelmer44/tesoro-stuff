#!/bin/sh
# Build the demo function as 32-bit x86 at -O0 and -O2, plus flat byte dumps.
set -e
cd "$(dirname "$0")"
mkdir -p build
for o in O0 O2; do
  clang --target=i386-pc-windows-msvc -$o -fno-asynchronous-unwind-tables \
    -c sum_clamped.c -o build/sum_clamped_$o.obj
  python3 extract_text.py build/sum_clamped_$o.obj build/sum_clamped_$o.bin
done
