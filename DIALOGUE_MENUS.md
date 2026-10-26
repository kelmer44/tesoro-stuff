# Dialogue menus and the speech option (25 October 2026)

Engine branch `codex/menus`, off `tesoro-engine` `6fdf961cafb`. **Status:** the layout and drawing are compared with the original
text routines in the emulator; the pointer handling and the script wait are read from the decompilation and ported; **not run in
a window**, and `0x407d00` itself was not executed.

## Recovered

**The menu op while it waits** (`0x405850`, cases 5 and 6; the choices are the visible items collected when the op ran, which the
executor already did). Until a choice is made, each frame: the pointer's held item is dropped and its frame is set to 0 (the
arrow; the previous frame is saved), then `0x407d00(count)` shows the choices and polls the pointer. When it returns a choice, Jim
**speaks the chosen line** (`0x410330`, the same routine as every character, option 0), not the narrator. When he has stopped, the saved pointer
frame is put back and the script continues at the item's line + 2.

**`0x407d00`.** Sets the pointer to the mouse, clears its text and draws it. Each choice is laid out with the text routine (`0x411c80`) in
white (`0xFFFF`) at x = 320, width 640, line height 18, **not centred** (the box's `param_8` is 0: the lines start at the left margin
and the block starts at the y given, instead of being centred on it). The first choice starts at y = 50; the next starts 30 below the last
line of the one before. After the last, the whole block is moved down by `465 - y` so that it ends at 465 (the last line at 435). A choice answers to
the pointer from its start to the start of the next (the last: to the bottom of the screen). Under the pointer the choice is drawn
again, in yellow (`0xFFE0`). A left click (button state) over a choice returns its index.

**The speech option** (op `+0xC`, the third number of `OBJ020T06, 2, 1`). The original parser stores **0** when no option follows
(`0x404810`, the branch that does not find a comma); the engine had assumed 1. For a character, `speak` calls its slot `0x78(option)`
(`0x410130`): it picks the **talking bank of that index** from the list of banks whose names start with `HAB` (`0x40fec0` builds the list in bank
order) and shows its frame 1. So option 0 is the first talking bank (what the engine always used), and the 14 lines in the game with `, 1` or `, 2`
use the second or third (`HABCAPI2` for the captain). Jim ignores the option (his bank follows his facing). An option beyond the list reads an
empty slot in the original (bank 0 is assumed).

## Ported

`SceneRenderer::layoutMenu/drawMenu` and the `centred` flag of `drawText` (renderer), `TesoroEngine::pickDialogueChoice` (polled
each frame by the script, returns -1 until a click on a choice), `dialogueChoiceDone`, the pointer showing during the menu, the pointer frame saved and restored,
`ScriptHost::player`, Jim saying the chosen line, the option selecting the talking bank, the parser default 0.
Verb answers (`jimSays`) now pass option 0 as the original does.

## Verification

`tools/compare_menu.py` + the ignored driver `local-media/jim-implementation/menu_driver.cpp`: the text routine `0x411c80` and glyph
drawing `0x412c50` run in the Unicorn emulator, the menu's arithmetic is applied to its entries as `0x407d00` does, and the frame is compared with the production
`layoutMenu`/`drawMenu`: three seeds (100, 200, 200 frames: 153,600,000 pixels) identical, with 1-5 choices of real game text (wrapping onto several lines) and
a hovered choice in 70% of them. `analysis/menu-comparison-seed{1,2,3}.json`. The earlier text layout comparison (`compare_text_layout.py`) still passes
and `verify_script_compiler.py` (updated for the default option 0) still matches all 145 blocks. The doors driver ran the recruiter's conversation chain through its menus with Jim
speaking the chosen lines.

## Gaps

1. The pick uses mouse-down events; the original polls the button state (a button held when the menu appears would choose at once). The original also
   removes something from the update list when a choice is picked (`0x40fb20` on a stack value), not understood.
2. `0x407d00` was not executed: its screen arithmetic comes from the decompilation (the emulator applies it by hand).
3. The cursor is shown during the menu, but how the original treats the interface (the verb bar) while a menu is open was not checked; the engine runs no interface pass.
4. A talking-bank index beyond the object's list uses bank 0.
5. Sound: the chosen line's WAV plays as any speech; subtitles are shown as for any speech by Jim (yellow, above his head) in addition to the menu.

## Fixes after the first play test (25 October 2026)

1. **Sub-conversations.** `CNVRECLUTA` ends a menu item with `run 'CONV5' main` and then `break` (back to its own menu). The engine ran both
   scripts at once, so both polled their menus every frame, the first-level list was the one that answered clicks, and parents were never
   finished. The original's update loop (`0x40fc20`) stops the pass for the whole frame as soon as a script's update returns 2 (still waiting)
   or 3; the new script is the one reached first, so the script that started it is held back until it ends (a call stack in effect). The
   engine now updates scripts newest first and stops at the first one still waiting (`TesoroEngine::updateScripts`). Driven headless
   (`doors_driver`, a menu picked a frame after it appears): the recruiter's conversation runs menu -> `CONV5` menu -> back to the
   `CNVRECLUTA` menu -> leave, at most one menu polled per frame, no script left.
   The order of two scripts at the same priority (`0x40fa10`) was read but not proved to put the new one first; the stack behaviour is what the scripts need.
2. **The menu never went away.** It stayed while any script that had shown it was alive. It is now shown only on frames where a script asked for it.
3. **The pointer during a conversation.** The verb stays at the arrow for the whole conversation (from its first menu until no script is left),
   not only while a list is up, and the arrow is drawn only while the list is shown; the chosen verb comes back when the conversation ends.

## Right click

Looked for what a right click does in the original: on a hovered object or an interface object it is the same as a left click
(slot `0x3c` calls `0x38`); on bare ground nothing happens (`0x4030e0` looks only at the left button). The pointer's own update
(`0x4029f0`) calls `0x4025b0` on a click edge of the input device's second button byte (`& 0x18`) while the interface is on its main page: that
routine hides the held item and slides the interface 640 pixels to the inventory page (`0x4028c0` slides back and shows the item again). No
path found resets the pointer to frame 0 on a right click; frame 0 is set by using or putting back an item (`0x40f0d0`, `0x40c3c0`, `0x419b30`),
by opening the options page (`0x4026d0`) and by the menu. So the right click is, as far as read, the inventory toggle; which of the byte's bits is the
right button was not proved. Not ported.
