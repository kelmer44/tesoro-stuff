#!/usr/bin/env python3
"""Package the locally built engine; original game assets remain external."""
from pathlib import Path
import argparse
import os
import plistlib
import shutil

root = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--scummvm', type=Path,
                    default=Path(os.environ.get('TESORO_SCUMMVM_DIR', str(root.parent / 'scummvm-tesoro'))),
                    help='separate ScummVM source/build directory')
args = parser.parse_args()
source = args.scummvm.resolve()
app = root / 'prototype' / 'Tesoro Prototype.app' / 'Contents'
(app / 'MacOS').mkdir(parents=True, exist_ok=True)
(app / 'Resources').mkdir(exist_ok=True)
temporary_binary = app / 'MacOS' / 'tesoro.new'
shutil.copy2(source / 'scummvm', temporary_binary)
temporary_binary.replace(app / 'MacOS' / 'tesoro')
shutil.copy2(source / 'COPYING', app / 'Resources' / 'COPYING')
info = dict(CFBundleExecutable='LaunchTesoro',
            CFBundleIdentifier='org.scummvm.tesoro-prototype',
            CFBundleName='Tesoro Prototype', CFBundleDisplayName='Tesoro Prototype',
            CFBundlePackageType='APPL', CFBundleShortVersionString='0.1',
            NSHighResolutionCapable=True)
(app / 'Info.plist').write_bytes(plistlib.dumps(info))
launcher = app / 'MacOS' / 'LaunchTesoro'
launcher.write_text('''#!/bin/sh
set -eu
TESORO_APP_BIN=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
TESORO_ROOT=$(CDPATH= cd -- "$TESORO_APP_BIN/../../../.." && pwd)
TESORO_SCUMMVM_DIR=${TESORO_SCUMMVM_DIR:-"$TESORO_ROOT/../scummvm-tesoro"}
exec "$TESORO_APP_BIN/tesoro" --config="$TESORO_ROOT/prototype/tesoro.ini" --extrapath="$TESORO_SCUMMVM_DIR/dists/engine-data" --themepath="$TESORO_SCUMMVM_DIR/gui/themes" tesoro-prototype
''')
launcher.chmod(0o755)
print(app.parent)
