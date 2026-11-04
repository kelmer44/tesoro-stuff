#!/bin/sh
# Saves a picture of the DOSBox-X window only (never the rest of the screen) as re/captures/<name>.png.
# Usage: re/reference/capture.sh <name>      (needs Screen Recording for the app that runs it; builds the window finder on first use)
set -e
cd "$(dirname "$0")/../.."
FIND=local-media/reference/windowid
[ -x "$FIND" ] || { mkdir -p local-media/reference; swiftc -O re/reference/windowid.swift -o "$FIND"; }
ID=$("$FIND")
[ -n "$ID" ] || { echo "DOSBox-X has no window" >&2; exit 1; }
screencapture -x -o -l"$ID" "re/captures/${1:?name}.png"
echo "re/captures/$1.png"
