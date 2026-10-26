# Office transfer, character speech and the captain scene

23 October 2026. Addresses apply to `TESORO.EXE` SHA-256
`0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef`; names are research
labels. Engine: `scene.cpp/h`, `jim.cpp/h`, `scriptexec.cpp/h`, `renderer.cpp/h` and
`tesoro.cpp/h` in `../scummvm-tesoro` (branch `tesoro-engine`, uncommitted when written).
Exports: `ghidra/RecoverOffice.java` (output in `analysis/office-transfer/`).

## Scenes

The original loads all 47 scenes at startup (`0x413e60`) from a name table at `0x42d6c8` (15
bytes per entry) and keeps them, so a scene keeps its state while the player is elsewhere.
Index 0 is `DESPACHO` (the office), 1 `PUERTO` (the harbour), then `TABERNA`, `CUBIERTA`,
`CAMAROTE`, `ALMACEN`, `COCINA`, `PLAYA`, `MAPA`, `POBLADO`, `PANTANO`, `CAMPAMENTO`, `VOLCAN`,
`ARMARIO`, `TABLERO`, `TIERRA`, `RISCO`, `CUBIERTA2`, `CAMAROTE2`, `ALMACEN2`, `COCINA2`,
`LUCHA`, `TRONO`, `ENTRADA`, `1`-`20`, `RISCOFINAL`, `FINAL`, `CREDITOS`. A script's
`sendmessage ... in LEVELn` addresses scene n of this table. The scene stores its index at
`+0x98`, its camera at `+0x80`, its groups at `+0x70`, its room at `+0x90`. Every scene has its
own camera. The engine loads a scene when it is first needed and keeps it.

## Moving Jim (`0x407b80`)

Leaving a scene removes Jim from it and its group and takes the old scene out of the object
list; entering adds him to the new scene and group (group 0 for scripts), moves the camera to
his position, puts the new scene in the object list at priority `0x14`, resets the new scene's
objects, and sets his position and the committed position. The first update after a scene is
entered plays the room's CD cue (`0x408840`; the engine reports track N and does not play it).

## Native ordinal 5 of the introduction (`0x4197a0`)

Run after movie 3 and `PUERTA1 activate off`: the **harbour's** `PUERTA1` is activated and
its show flag cleared; the script's scene becomes the office (so that its participants, the
captain, are looked up there); Jim is moved to the office at (550, 100). Ordinal 6 does nothing.

## Characters speak

A character record's first four parameter bytes are its text colour (RGB565) and wrap width.
Its idle bank is the **last** bank whose name starts with `MIR`, its talking banks are the banks
whose names start with `HAB` (`0x40fec0`; the captain has `MIRDCAPI`, `MIRICAPI`, `MIRACAPI`,
`HABCAPI`, `HABCAPI2`). The factory subtracts 4 from a character's parameter size before it builds
it (`0x409fa2`), because its interactive part starts after those four bytes.

Speech (`0x410330`): the first time, the character's current animation is paused and its bank and
flags are saved; the talking bank is selected on frame 1 (the closed mouth); the speech WAV starts;
the character joins the update list until the sound ends or the player skips it (left click or a key),
when it leaves the list and the waiting script resumes after 201 ms. While it talks, a mouth that
moves picks a random open frame (1 to frames-1) at most every 100 ms; the narrator's Jim keeps
the closed mouth. After the speech the character keeps showing the talking bank on its closed mouth
until the script ends or an animation is played on it, when the saved animation is put back
(`0x401dc0`, `0x401a70`). This restore runs for every participant of a script that ends.

Drawing: while a character is in this talking state its talking bank's **frame 0 is its body**
(drawn at depth + 1) and the current frame the mouth (drawn at depth). The text is centred above the
head: x = the middle of the current frame's picture (or its hit box if it has none), y = 50 pixels
above the frame's hit box top, relative to the group's view of the camera. Jim differs: his text
is yellow (`0xFFC0`), wrapped at 300, and sits at `0xBE - hitY*scale/256 - y`; he talks in the bank for his
facing (left `JIMHABIZ`, right `JIMHABDER`, down `JIMHABAB`, up `JIMHABAR`). The **narrator is Jim**
(speaker 0): the text is blue (`0x1F`), centred on the screen, and he shows the closed mouth.

### Jim's walk ends with a look

The script's resume (`0x406140`) removes Jim from the update list, clears his walking flag and
calls his look (`0x407a80`) with the direction of the walk before the script goes on. The engine
did not do this before; the walk comparison now runs the original look after arrival.

## Movies (`0x4132b0`)

A script's movie plays to the end or until the left mouse button is pressed; it waits for all
buttons to be released and holds for 500 ms of real time (`0x4139e0`) before the script goes on.
The game clock ignores any step of 500 ms or more, so the movie takes no game time. The engine
skips with a left click as well as Escape, and leaves the last frame up for 500 ms.

## Verification (23 October 2026)

| Tool | Original routines run in the emulator | Result |
| --- | --- | --- |
| `tools/compare_text_layout.py` | `0x411c80`, `0x412c50` over the game's speech texts, random centre, height, width and line height | 2,000 cases, 614,400,000 pixels identical (width 120 or more; the original holds 30 lines) |
| `tools/compare_talk_draw.py` | `0x409e50`-style construction (`0x40fec0`, `0x40ed10`, `0x401aa0`), draw `0x4102e0`, queue `0x411f80` for the captain, at rest and after talking | 600 cases, 886 sprites identical in position, depth, order and image (also confirms the idle and talking banks that the constructor picks) |
| `tools/compare_jim_walk.py` | start `0x407600`, step `0x407790`, then the resume's look `0x407a80` | 1,300 walks identical, including the frame after arrival |

A headless run of the built engine (dummy video and audio, speech muted, movies absent) ran
`FANPRESENT/main` from New Game to its end in 110 s of game time with no error: harbour narration,
Jim's walk, the transfer, 18 lines of dialogue between Jim, the captain and the narrator, and the
captain leaving. Frames were inspected: captain with body while he talks, text above each speaker,
the closed-mouth Jim during narration, the empty chair at the end.

**Limits.** Jim's talking sprites are checked by reading the code, not by running it (his object
needs the resource system). The mouth frames are random in both programs and cannot match; the
original's random numbers come from the system tick. Movies, audio output and the CD music were
not exercised. Camera follow (`0x401fc0`) is now implemented; see [JIM_AND_WALKING.md](JIM_AND_WALKING.md).
