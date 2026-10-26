# Script executor: start, run, wait, resume

22 October 2026. Addresses apply to `TESORO.EXE` SHA-256
`0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef`; names are
research labels. Engine implementation: `engines/tesoro/scriptexec.h/.cpp` and the
`ScriptHost` part of `tesoro.cpp` in `../scummvm-tesoro` (branch `tesoro-engine`,
uncommitted when written). Exports are in `analysis/script-compiler/`
(`ghidra/RecoverScriptCompiler.java`). Compiled form: [SCRIPT_COMPILER.md](SCRIPT_COMPILER.md).

## Model

A started block is an object in the global object list (priority 0x1E), like Jim or
a scene. The list update calls the script's virtual `+0x10` (`0x405850`) every frame.
That function waits on the current op and, when the wait is over, advances the
program counter and calls the executor `0x405a60`, which runs ops until one has to
wait. Characters that speak or walk add themselves to the list while they do so and,
when done, call the waiting script's virtual `+0x34` (`0x406140`), which removes the
character from the list, advances the counter and runs the executor again.

`tesoro_start_script` (`0x405fd0`) resolves the block's participants in the scene it is
given, sets the break target to -1, finds the label (a missing label starts at op 0),
stores the caller's variable pointer, switches input off, inserts the script and
executes at once. The game clock is `0x413a10`: it ignores a single host delta of
500 ms or more.

## Per-op behaviour recovered

| Op | Executor | Wait (update) |
| --- | --- | --- |
| return / end | remove script, switch input on; restore participants whose `+0xC` flag was set | none |
| speech | speaker slot > 0: that participant speaks; slot 0: `0x407ce0` on the player object (narration) | wait while the speaker's `+0xD4` flag is set, then 201 ms more |
| sleep | store the clock | wait until `now - start >= ms` |
| break | end if there is no menu, otherwise `pc = menu op` | none |
| menu | store `pc` as the break target, list the visible items, leave `pc` on the last item | choices shown with `0x407d00`, the player says the chosen line, then jump to the item's line + 2 |
| run | start the target block with the caller's variables and scene | next frame: input off, `pc++` (the following `return` ends the caller) |
| run FANMTRONO | native action 20, 0 if the label is `ir`, else 1; `pc++` | none |
| sendmessage play | play on the target; `pc++` unless the mode is `wait` | `wait`: prepare the object, wait while it plays |
| sendmessage walk | virtual `+0x80(x, y, script, 0)` | wait for the object's resume call |
| sendmessage look | virtual `+0x74(direction)`; `pc++` | none |
| sendmessage activate / show | virtual `+0x20`; `0x401d80`; `pc++` | none |
| movie | `0x4132b0`, blocking in the original; `pc++` | none |
| sound | play `NAME.WAV` | none |
| cd | `0x403cb0(track, 1)` | none |
| DO | script class virtual `+0x70(ordinal)`; the counter is read after the call | none |

An item is listed when its variable is 1, or 0 if it has `!`, or always when the
variable is 0xFF. The original's list has room for 16 items (from its array size).

## Implemented and approximated

Implemented by the engine: every op above except the two waiting on characters
(walk, character speech) and the choice display, which stop with a clear error. The
player's `speech` is a host approximation: the narrator plays `ID.WAV`, shows the text
centred in blue and is skipped by click or Space. Whether the original lets the player
skip, and how it times text without audio, is not recovered. Movies are an engine
state that the script waits on instead of a nested blocking loop; the scene clock is
frozen while one plays. Input enable is recorded but nothing reads it yet.

Variables: scripts receive a pointer from whoever starts them; the engine passes one
256-byte block. Which object owns it for each start site is not recovered.

## Verification

`tools/verify_script_executor.py` with the ignored driver
`local-media/script-implementation/exec_driver.cpp` (production `script.o`,
`scriptexec.o`, `scene.o`, `animation.o`, `resources.o` against the real PUERTO
scene, a fake host and a 1 ms clock):

```sh
python3 tools/verify_script_executor.py --game /Users/gabriel/Desktop/ags/TESORO \
  --driver local-media/script-implementation/exec_driver \
  --data local-media/script-implementation/data \
  --output analysis/script-executor-verification.json
```

Result: the 13 host events of `FANPRESENT/main` up to Show Jim (DO ordinal 4 at
24,847 ms) occur at the times predicted from the script text and the WAV lengths
(sleeps, speech end plus 201 ms, movies not advancing the clock), and the final
active/shown states of the objects it switches match. All 18 speech lines and the
`ANM1BCAP0` sound have audio in the archive.

**Limits.** The prediction is derived from the same understanding of the wait rules,
so it checks the implementation, not the original. The ScummVM executable was built
but not run through the menus in this session. Other-script paths (menus, `run`,
walking, character speech) are not exercised. No original-runtime comparison.

## Found while testing the live opening (22 October 2026)

- Only the Show Jim step stops the opening (`doNative` error). A headless run of the
  built engine (dummy SDL drivers, speech muted, movies absent) reaches it and enters
  ScummVM's error debugger there; no other crash occurred.
- The recruiter did not appear because kind 5 and the interactive kinds were inert
  placeholders. Recovered: the draw method (vtable `+0x18`) of kinds 6, 12, 13, 17 and 20
  is `0x40efa0`, kind 5's is `0x4102e0` (which calls it), and `0x40efa0` computes the
  same screen position and depth as scenery (`0x40e6b0`). The interactive constructor
  `0x40ed10` sets the draw-list flag `0x100`, shows the object only if frame 0 has an
  image, and starts frame `parameters[0]` (`parameters[4]` for characters, whose first
  four bytes are two other values). The character constructor `0x40fec0` always shows
  the object. The engine now builds these kinds as `InteractiveObject` / `CharacterObject`.
  Behaviour, idle-bank selection, hit testing and verbs remain unimplemented.
- Group draw lists therefore hold every object with flag `0x100`, not only scenery.
- Subtitles: ScummVM's global default for `subtitles` is off, so the narration text was
  not drawn. The engine now registers a default of on.

## Script order within a frame (25 October 2026)

The update loop (`0x40fc20`) walks the object list (scripts are added at priority 30 by `0x40fa10`) once per frame, marking each visited
entry (`+0x68 & 0x200`) and restarting when an update changes the list. An update that returns 2 (a script waiting, pc unchanged) or 3 (it
yielded) ends the pass for the frame, so a waiting script holds back everything behind it. With `run`, the started script is reached first: the
caller resumes (its `run` op becomes `pc++`) only after the callee has finished. The engine implements this as newest-script-first with an early stop.
