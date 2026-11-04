#!/bin/sh
# Takes pictures of one program's window at a steady rate, for runs that cannot be played in step (the opening movie, a walk, a fade).
# Usage: re/reference/burst.sh <dosbox|scummvm> <name> <seconds> [interval seconds, default 1]
# Writes re/captures/<name>/<seconds-since-start>.png (e.g. 003.4.png) and re/captures/<name>/log.txt. Only that program's window is captured.
set -e
cd "$(dirname "$0")/../.."
case "$1" in dosbox) APP=dosbox-x ;; scummvm) APP=scummvm ;; *) echo "first argument: dosbox or scummvm" >&2; exit 1 ;; esac
NAME=${2:?name}; SECS=${3:?seconds}; STEP=${4:-1}
FIND=local-media/reference/windowid
[ -x "$FIND" ] || { mkdir -p local-media/reference; swiftc -O re/reference/windowid.swift -o "$FIND"; }
ID=$("$FIND" "$APP"); [ -n "$ID" ] || { echo "$APP has no window" >&2; exit 1; }
OUT=re/captures/$NAME; mkdir -p "$OUT"
START=$(python3 -c 'import time; print(time.time())')
: > "$OUT/log.txt"
while :; do
  NOW=$(python3 -c 'import time,sys; print("%.1f" % (time.time()-float(sys.argv[1])))' "$START")
  python3 -c 'import sys; sys.exit(0 if float(sys.argv[1]) <= float(sys.argv[2]) else 1)' "$NOW" "$SECS" || break
  F=$(printf '%05.1f' "$NOW")
  screencapture -x -o -l"$ID" "$OUT/$F.png" && echo "$F" >> "$OUT/log.txt"
  sleep "$STEP"
done
echo "$(wc -l < "$OUT/log.txt") pictures in $OUT"
