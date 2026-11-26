#!/bin/sh
# Usage: ./build_native.sh path/to/sum_to.c   (compiles the same C for this machine; run it yourself: ./build/native_demo 10)
set -e
cd "$(dirname "$0")"
mkdir -p build
clang -O0 native_main.c "${1:?usage: ./build_native.sh file.c}" -o build/native_demo
echo "built build/native_demo, run it with: ./build/native_demo 10"
