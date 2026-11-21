#!/bin/sh
# Usage: ./build.sh path/to/file.c
# Builds -O0 and -O2 as 32-bit x86 (Windows target) and writes the bare machine code next to them:
#   build/<name>_O0.obj, build/<name>_O0.bin, build/<name>_O2.obj, build/<name>_O2.bin
set -e
cd "$(dirname "$0")"
src="${1:?usage: ./build.sh file.c}"
name="$(basename "$src" .c)"
mkdir -p build
for o in O0 O2; do
  clang --target=i386-pc-windows-msvc -$o -fno-asynchronous-unwind-tables \
    -c "$src" -o "build/${name}_$o.obj"
  python3 extract_text.py "build/${name}_$o.obj" "build/${name}_$o.bin"
done
