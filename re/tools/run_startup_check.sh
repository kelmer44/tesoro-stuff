#!/bin/sh
# Regenerates the vectors from the original code and checks the engine's core primitives against them.
# Usage: re/tools/run_startup_check.sh [engine-worktree]   (default: the Codex worktree)
set -e
ENG=${1:-/Users/gabriel/Desktop/source/scummvm-tesoro-codex}
cd "$(dirname "$0")/../.."
re/.venv/bin/python re/tools/gen_vectors_startup.py
mkdir -p local-media/bin
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/startup_driver.cpp \
    "$ENG/engines/tesoro/core/clock.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/random.cpp" \
    -o local-media/bin/startup_driver
local-media/bin/startup_driver local-media/vectors/startup.txt

# Archive index: original ReadIndex/FindEntry (Unicorn, CRT hooked) vs engine ArchiveIndex
re/.venv/bin/python re/tools/gen_vectors_archive.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/archive_driver.cpp \
    "$ENG/engines/tesoro/core/archiveindex.cpp" -o local-media/bin/archive_driver
local-media/bin/archive_driver local-media/vectors/archive.txt

# Animation bank: original Load/LoadImage/LoadDeferredImages/ScaleBrightness/SetRange (Unicorn) vs engine AnimPlayer
re/.venv/bin/python re/tools/gen_vectors_anim.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/anim_driver.cpp \
    "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/anim_driver
local-media/bin/anim_driver local-media/vectors/anim.txt

# Cursor lists: original List / ObjectList code (Unicorn, pool allocator replaced) vs engine CursorList
re/.venv/bin/python re/tools/gen_vectors_list.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/list_driver.cpp -o local-media/bin/list_driver
local-media/bin/list_driver local-media/vectors/list.txt
