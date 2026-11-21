#!/bin/sh
# Usage: ./patch_demo.sh jge|step      (scripted version of the hex-editor patch)
#   jge : 7f -> 7d at 0x1d, "i <= n" becomes "i < n"
#   step: 01 -> 02 at 0x2d, "i++" becomes "i += 2"
set -e
cd "$(dirname "$0")"
case "${1:-}" in
  jge)  off=$((0x1d)); want=7f; new='\175' ;;
  step) off=$((0x2d)); want=01; new='\002' ;;
  *) echo "usage: ./patch_demo.sh jge|step"; exit 1 ;;
esac
src=build/sum_to_O0.bin
out=build/sum_to_patched.bin
have=$(xxd -s $off -l 1 -p "$src")
if [ "$have" != "$want" ]; then
  echo "byte at $(printf '0x%x' $off) is $have, expected $want: the live-typed code compiled differently"; exit 1
fi
cp "$src" "$out"
printf "$new" | dd of="$out" bs=1 seek=$off conv=notrunc 2>/dev/null
echo "patched byte at $(printf '0x%x' $off): $want -> $(xxd -s $off -l 1 -p "$out")  (file: $out)"
