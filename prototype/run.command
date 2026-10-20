#!/bin/sh
set -eu
TESORO_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TESORO_SCUMMVM_DIR=${TESORO_SCUMMVM_DIR:-"$TESORO_ROOT/../scummvm-tesoro"}
exec "$TESORO_SCUMMVM_DIR/scummvm" --config="$TESORO_ROOT/prototype/tesoro.ini" --extrapath="$TESORO_SCUMMVM_DIR/dists/engine-data" --themepath="$TESORO_SCUMMVM_DIR/gui/themes" tesoro-prototype
