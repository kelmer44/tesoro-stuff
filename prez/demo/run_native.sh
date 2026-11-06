#!/bin/sh
# Compile and run the C source natively (arm64 or whatever this machine is).
set -e
cd "$(dirname "$0")"
mkdir -p build
clang -O0 native_main.c -o build/native_demo
./build/native_demo
