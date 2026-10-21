#!/bin/sh
set -eu
TESORO_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TESORO_LAUNCHER="$TESORO_ROOT/prototype/Tesoro Prototype.app/Contents/MacOS/LaunchTesoro"
if [ ! -x "$TESORO_LAUNCHER" ]; then
    echo 'The preserved prototype app is not available locally. See prototype/source/README.md.' >&2
    exit 1
fi
exec "$TESORO_LAUNCHER"
