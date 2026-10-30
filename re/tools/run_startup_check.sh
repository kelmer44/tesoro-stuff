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
    "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/anim_driver
local-media/bin/anim_driver local-media/vectors/anim.txt

# Cursor lists: original List / ObjectList code (Unicorn, pool allocator replaced) vs engine CursorList
re/.venv/bin/python re/tools/gen_vectors_list.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/list_driver.cpp -o local-media/bin/list_driver
local-media/bin/list_driver local-media/vectors/list.txt

# Room: original Room_Ctor / InsertSorted / RemoveObject (Unicorn) vs engine Room
re/.venv/bin/python re/tools/gen_vectors_room.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/room_driver.cpp \
    "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/room_driver
local-media/bin/room_driver local-media/vectors/room.txt

# Object list passes: original ObjectList_PassA / PassB (Unicorn, callee stubs edit the list) vs engine runUpdatePass / runUpdate2Pass
re/.venv/bin/python re/tools/gen_vectors_pass.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/pass_driver.cpp \
    "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/pass_driver
local-media/bin/pass_driver local-media/vectors/pass.txt

# Camera: original Camera_Ctor / SetBounds / SetValue0 / SetPositionClamped / MoveTo / UpdateToward (Unicorn) vs engine Camera
re/.venv/bin/python re/tools/gen_vectors_camera.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/camera_driver.cpp \
    "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/camera_driver
local-media/bin/camera_driver local-media/vectors/camera.txt

# Object save state: original Obj_SaveState / Obj_LoadState (Unicorn, fread/fwrite replaced) vs engine Object::saveState / loadState
re/.venv/bin/python re/tools/gen_vectors_state.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/state_driver.cpp \
    "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/state_driver
local-media/bin/state_driver local-media/vectors/state.txt

# Hover label: original Cursor_SetText (Unicorn) vs engine HoverText
re/.venv/bin/python re/tools/gen_vectors_hovertext.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/hovertext_driver.cpp \
    "$ENG/engines/tesoro/core/hovertext.cpp" -o local-media/bin/hovertext_driver
local-media/bin/hovertext_driver local-media/vectors/hovertext.txt

# Cursor: original Cursor_Ctor / ProcessCandidates (Unicorn, candidate verbs are stubs) vs engine Cursor
re/.venv/bin/python re/tools/gen_vectors_cursor.py
c++ -std=c++11 -O1 -fno-operator-names -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/cursor_driver.cpp \
    "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/cursor_driver
local-media/bin/cursor_driver local-media/vectors/cursor.txt

# Level: original Level_Load / AttachObjects / ObjectMoved / FindObjectByKey / CameraMoveTo / UpdateRoomCameras (Unicorn, CRT and pool replaced) vs engine Level
re/.venv/bin/python re/tools/gen_vectors_level.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/level_driver.cpp \
    "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/level_driver
local-media/bin/level_driver local-media/vectors/level.txt

# Sprite object (kind 3): original constructor / tick / draw (Unicorn, image blit recorded) vs engine SpriteObject
re/.venv/bin/python re/tools/gen_vectors_sprite.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/sprite_driver.cpp \
    "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/sprite_driver
local-media/bin/sprite_driver local-media/vectors/sprite.txt

# Menu item (kind 25): original constructor / tick-and-clear / draw (Unicorn, image blit recorded) vs engine MenuItem
re/.venv/bin/python re/tools/gen_vectors_menuitem.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/menuitem_driver.cpp \
    "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/menuitem_driver
local-media/bin/menuitem_driver local-media/vectors/menuitem.txt

# Render queue: original ResetRenderQueue / AddSprite / QueueRow (literal images; Unicorn) vs engine RenderQueue span lists
re/.venv/bin/python re/tools/gen_vectors_render.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/render_driver.cpp \
    "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -o local-media/bin/render_driver
local-media/bin/render_driver local-media/vectors/render.txt

# Compose: original Display_Compose (surface pre-locked) on queued literal sprites over a full-screen background vs engine ScreenDisplay::compose
re/.venv/bin/python re/tools/gen_vectors_compose.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/compose_driver.cpp \
    "$ENG/engines/tesoro/core/screendisplay.cpp" "$ENG/engines/tesoro/core/textqueue.cpp" "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -lz -o local-media/bin/compose_driver
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
    "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/collide_driver
local-media/bin/collide_driver local-media/vectors/collide.txt

# Pointer draw: original 0x402d80 (queue of scaled images recorded) vs engine Cursor::draw
re/.venv/bin/python re/tools/gen_vectors_cursordraw.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/cursordraw_driver.cpp \
    "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/cursordraw_driver
local-media/bin/cursordraw_driver local-media/vectors/cursordraw.txt

# Pointer IsHitter: original 0x402e50 (reads the OTHER object's flag bit 10) vs engine Cursor::isHitter
re/.venv/bin/python re/tools/gen_vectors_hitter.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/hitter_driver.cpp \
    "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/hitter_driver
local-media/bin/hitter_driver local-media/vectors/hitter.txt

# Menu item click: original 0x417b40 / 0x40f040 (the calls a click makes are logged by hooks) vs engine MenuItem::setVerbAndClick / rightClick
re/.venv/bin/python re/tools/gen_vectors_click.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/click_driver.cpp \
    "$ENG/engines/tesoro/core/menuitem.cpp" "$ENG/engines/tesoro/core/spriteobject.cpp" "$ENG/engines/tesoro/core/level.cpp" "$ENG/engines/tesoro/core/cameraframe.cpp" "$ENG/engines/tesoro/core/collision.cpp" "$ENG/engines/tesoro/core/camera.cpp" "$ENG/engines/tesoro/core/object.cpp" "$ENG/engines/tesoro/core/room.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/cursor.cpp" "$ENG/engines/tesoro/core/hovertext.cpp" "$ENG/engines/tesoro/core/mouse.cpp" "$ENG/engines/tesoro/core/inputstate.cpp" -o local-media/bin/click_driver
local-media/bin/click_driver local-media/vectors/click.txt

# Fade to black: original Display_StartEffect (DirectDraw calls hooked) vs engine ScreenDisplay::fadeOut (about a minute: the emulator runs millions of instructions per step)
re/.venv/bin/python re/tools/gen_vectors_fade.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/fade_driver.cpp \
    "$ENG/engines/tesoro/core/screendisplay.cpp" "$ENG/engines/tesoro/core/textqueue.cpp" "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -lz -o local-media/bin/fade_driver
local-media/bin/fade_driver local-media/vectors/fade.txt

# Script compiler: original .DLG loader 0x406190 / parser 0x404810 / label resolver 0x406110 vs engine ScriptLibrary (shipped scripts, synthetic and randomly edited blocks)
re/.venv/bin/python re/tools/gen_vectors_dlg.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/dlg_driver.cpp \
    "$ENG/engines/tesoro/core/scriptblock.cpp" "$ENG/engines/tesoro/core/stdiotext.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/dlg_driver
local-media/bin/dlg_driver local-media/vectors/dlg.txt

# Speech tables: original 0x413570 / 0x4137c0 / 0x4138b0 / 0x413940 vs engine SpeechTable / SpeechTables (shipped texts, randomly edited slices, boundary cases)
re/.venv/bin/python re/tools/gen_vectors_speech.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/speech_driver.cpp \
    "$ENG/engines/tesoro/core/speechtable.cpp" "$ENG/engines/tesoro/core/stdiotext.cpp" "$ENG/engines/tesoro/core/namekey.cpp" -o local-media/bin/speech_driver
local-media/bin/speech_driver local-media/vectors/speech.txt

# Script executor: original start 0x405fd0 / tick 0x405850 / executor 0x405a60 / resume 0x406140 vs engine ScriptRunner (every shipped block, random worlds; the calls to the rest of the game are compared)
re/.venv/bin/python re/tools/gen_vectors_run.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/run_driver.cpp \
    "$ENG/engines/tesoro/core/scriptrunner.cpp" "$ENG/engines/tesoro/core/scriptblock.cpp" "$ENG/engines/tesoro/core/stdiotext.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/screendisplay.cpp" "$ENG/engines/tesoro/core/textqueue.cpp" "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -lz -o local-media/bin/run_driver
local-media/bin/run_driver local-media/vectors/run.txt local-media/dlg

# Text: original Display_DrawText 0x411c80 (layout) and Compose's text overlay 0x412c50 vs engine TextQueue through ScreenDisplay (synthetic fonts, random texts, boundary cases)
re/.venv/bin/python re/tools/gen_vectors_text.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/text_driver.cpp \
    "$ENG/engines/tesoro/core/screendisplay.cpp" "$ENG/engines/tesoro/core/textqueue.cpp" "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -lz -o local-media/bin/text_driver
local-media/bin/text_driver local-media/vectors/text.txt

# Speaker: original say 0x410330 / update 0x4101f0 / clear 0x410400 / talk 0x410060 / draws 0x4102e0 and 0x407ac0 vs engine Speaker (callees are logging hooks)
re/.venv/bin/python re/tools/gen_vectors_speaker.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/speaker_driver.cpp \
    "$ENG/engines/tesoro/core/speaker.cpp" -o local-media/bin/speaker_driver
local-media/bin/speaker_driver local-media/vectors/speaker.txt

# Walk area: original ScaleAt 0x414740 / FindPath 0x414f10 / NextWaypoint 0x414e80 vs engine WalkArea (random polylines, corner and tie cases)
re/.venv/bin/python re/tools/gen_vectors_walkarea.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/walkarea_driver.cpp \
    "$ENG/engines/tesoro/core/walkarea.cpp" -o local-media/bin/walkarea_driver
local-media/bin/walkarea_driver local-media/vectors/walkarea.txt

# Jim's walking: original walk order 0x407600 / update 0x407790 / collision callback 0x4079e0 / set facing 0x407a80 vs engine JimWalker (callees are logging hooks)
re/.venv/bin/python re/tools/gen_vectors_jimwalk.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/jimwalk_driver.cpp \
    "$ENG/engines/tesoro/core/jimwalker.cpp" -o local-media/bin/jimwalk_driver
local-media/bin/jimwalk_driver local-media/vectors/jimwalk.txt

# Sound device: original play 0x404110 / find 0x4040d0 / stop 0x4043b0 / update 0x404210 vs engine SoundDevice (record bookkeeping; streaming is a hook)
re/.venv/bin/python re/tools/gen_vectors_sound.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/sound_driver.cpp \
    "$ENG/engines/tesoro/core/sounddevice.cpp" -o local-media/bin/sound_driver
local-media/bin/sound_driver local-media/vectors/sound.txt

# Choice menu: original Dialogue_ChoiceMenu 0x407d00 with the original text layout vs engine ChoiceMenu (bands scanned with the mouse, edges, button up/down)
re/.venv/bin/python re/tools/gen_vectors_choice.py
c++ -std=c++11 -O1 -fno-operator-names -fno-rtti -DHAVE_CONFIG_H -DMACOSX -DSDL_BACKEND -DPOSIX -I"$ENG" -I"$ENG/engines" -I/opt/homebrew/include re/tools/drivers/choice_driver.cpp \
    "$ENG/engines/tesoro/core/scriptrunner.cpp" "$ENG/engines/tesoro/core/scriptblock.cpp" "$ENG/engines/tesoro/core/stdiotext.cpp" "$ENG/engines/tesoro/core/namekey.cpp" "$ENG/engines/tesoro/core/screendisplay.cpp" "$ENG/engines/tesoro/core/textqueue.cpp" "$ENG/engines/tesoro/core/renderqueue.cpp" "$ENG/engines/tesoro/core/animbank.cpp" "$ENG/engines/tesoro/core/animimage.cpp" "$ENG/engines/tesoro/core/animset.cpp" -lz -o local-media/bin/choice_driver
local-media/bin/choice_driver local-media/vectors/choice.txt
