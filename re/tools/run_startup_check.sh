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
    "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/anim_driver
local-media/bin/anim_driver local-media/vectors/anim.txt

# Cursor lists: original List / ObjectList code (Unicorn, pool allocator replaced) vs engine CursorList
re/.venv/bin/python re/tools/gen_vectors_list.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/list_driver.cpp -o local-media/bin/list_driver
local-media/bin/list_driver local-media/vectors/list.txt

# Room: original Room_Ctor / InsertSorted / RemoveObject (Unicorn) vs engine Room
re/.venv/bin/python re/tools/gen_vectors_room.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/room_driver.cpp \
    "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/room_driver
local-media/bin/room_driver local-media/vectors/room.txt

# Object list passes: original ObjectList_PassA / PassB (Unicorn, callee stubs edit the list) vs engine runUpdatePass / runUpdate2Pass
re/.venv/bin/python re/tools/gen_vectors_pass.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/pass_driver.cpp \
    "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/pass_driver
local-media/bin/pass_driver local-media/vectors/pass.txt

# Camera: original Camera_Ctor / SetBounds / SetValue0 / SetPositionClamped / MoveTo / UpdateToward (Unicorn) vs engine Camera
re/.venv/bin/python re/tools/gen_vectors_camera.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/camera_driver.cpp \
    "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/camera_driver
local-media/bin/camera_driver local-media/vectors/camera.txt

# Object save state: original Obj_SaveState / Obj_LoadState (Unicorn, fread/fwrite replaced) vs engine Object::saveState / loadState
re/.venv/bin/python re/tools/gen_vectors_state.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/state_driver.cpp \
    "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/state_driver
local-media/bin/state_driver local-media/vectors/state.txt

# Hover label: original Cursor_SetText (Unicorn) vs engine HoverText
re/.venv/bin/python re/tools/gen_vectors_hovertext.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/hovertext_driver.cpp \
    "$ENG/engines/tesoro/core/hovertext.cpp" -o local-media/bin/hovertext_driver
local-media/bin/hovertext_driver local-media/vectors/hovertext.txt

# Cursor: original Cursor_Ctor / ProcessCandidates (Unicorn, candidate verbs are stubs) vs engine Cursor
re/.venv/bin/python re/tools/gen_vectors_cursor.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/cursor_driver.cpp \
    "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/cursor_driver
local-media/bin/cursor_driver local-media/vectors/cursor.txt
