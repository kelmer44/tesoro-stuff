#!/bin/sh
# Patch one byte of the -O0 machine code: "i++" (add eax,1) becomes "i += 2".
set -e
cd "$(dirname "$0")"
cp build/sum_clamped_O0.bin build/sum_clamped_patched.bin
# Offset 0x62 holds the "1" in: 83 c0 01  add eax, 0x1
printf '\002' | dd of=build/sum_clamped_patched.bin bs=1 seek=$((0x62)) conv=notrunc 2>/dev/null
echo "original:"; xxd -s 0x60 -l 3 build/sum_clamped_O0.bin
echo "patched:";  xxd -s 0x60 -l 3 build/sum_clamped_patched.bin
