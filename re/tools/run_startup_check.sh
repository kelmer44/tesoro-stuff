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

# Level: original Level_Load / AttachObjects / ObjectMoved / FindObjectByKey / CameraMoveTo / UpdateRoomCameras (Unicorn, CRT and pool replaced) vs engine Level
re/.venv/bin/python re/tools/gen_vectors_level.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/level_driver.cpp \
    "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/level_driver
local-media/bin/level_driver local-media/vectors/level.txt

# Sprite object (kind 3): original constructor / tick / draw (Unicorn, image blit recorded) vs engine SpriteObject
re/.venv/bin/python re/tools/gen_vectors_sprite.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/sprite_driver.cpp \
    "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/sprite_driver
local-media/bin/sprite_driver local-media/vectors/sprite.txt

# Menu item (kind 25): original constructor / tick-and-clear / draw (Unicorn, image blit recorded) vs engine MenuItem
re/.venv/bin/python re/tools/gen_vectors_menuitem.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/menuitem_driver.cpp \
    "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/menuitem_driver
local-media/bin/menuitem_driver local-media/vectors/menuitem.txt

# Render queue: original ResetRenderQueue / AddSprite / QueueRow (literal images; Unicorn) vs engine RenderQueue span lists
re/.venv/bin/python re/tools/gen_vectors_render.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/render_driver.cpp \
    "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -o local-media/bin/render_driver
local-media/bin/render_driver local-media/vectors/render.txt

# Compose: original Display_Compose (surface pre-locked) on queued literal sprites over a full-screen background vs engine ScreenDisplay::compose
re/.venv/bin/python re/tools/gen_vectors_compose.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/compose_driver.cpp \
    "$ENG/engines/tesoro/core/screendisplay.cpp" "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -lz -o local-media/bin/compose_driver
local-media/bin/compose_driver local-media/vectors/compose.txt

# Input state machine: original InputDevice state update (GetTickCount replaced) vs engine InputStates
re/.venv/bin/python re/tools/gen_vectors_input.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/input_driver.cpp \
    "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/input_driver
local-media/bin/input_driver local-media/vectors/input.txt

# Swept rectangle test: original 0x409280 (Unicorn) vs engine sweepRects
re/.venv/bin/python re/tools/gen_vectors_sweep.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/sweep_driver.cpp \
    "$ENG/engines/tesoro/core/collision.cpp" -o local-media/bin/sweep_driver
local-media/bin/sweep_driver local-media/vectors/sweep.txt

# Collision pass: original 0x408890 / 0x408c50 on synthetic levels with scripted object methods vs engine Level::collisionPass
re/.venv/bin/python re/tools/gen_vectors_collide.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/collide_driver.cpp \
    "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/collide_driver
local-media/bin/collide_driver local-media/vectors/collide.txt
