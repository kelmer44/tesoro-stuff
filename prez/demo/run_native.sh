#!/bin/sh
# Usage: ./run_native.sh path/to/sum_to.c [n]   (compiles the same C for this machine and runs it)
set -e
cd "$(dirname "$0")"
mkdir -p build
clang -O0 native_main.c "${1:?usage: ./run_native.sh file.c [n]}" -o build/native_demo
./build/native_demo "${2:-10}"
