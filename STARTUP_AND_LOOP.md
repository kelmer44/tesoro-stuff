# Original startup and first engine loop

**Source-state caveat, 21 October 2026:** implementation and test results in this
report describe the earlier startup build. The active branch has subsequent
commits and local edits; the inspected source no longer contains the same test
hook/pacing and has unfinished edits. See [RE_STATUS.md](RE_STATUS.md) for the
current baseline and [MASTER_PLAN.md](MASTER_PLAN.md) for the governing plan.
The recovered executable findings remain research evidence for that edition.

21 October 2026. Recovered from the verified `TESORO.EXE` and original LVL/DLG data.
The active implementation is in `../scummvm-tesoro/engines/tesoro`; the preserved
prototype under `prototype/source` remains unchanged.

## What the game starts with

1. Initialize the archive, lookup table, fonts, text catalogues, interface,
   scripts, Jim, rooms and menu scenes. The original preloads the scene table;
   the new engine currently loads only the two menus.
2. Place Jim in `PRINCIPAL.LVL`, with his actor disabled, and play **movie 0**
   (`data00.bin`). This is an explicit zero argument at `0x4143f9`, not a guess
   from the movie filenames. The opening movie precedes the title menu.
3. Show the original **Nueva partida / Cargar partida** menu.
4. Nueva partida switches to `NUEVO.LVL` and starts `FANMMENU/nuevo`:
   **movie 1 → show CENSURA → wait 5,500 ms → hide CENSURA**.
5. The difficulty screen has three original buttons. Their action numbers come
   from the LVL parameters, not from their names or screen coordinates:
   **2 = easy joke, 3 = real adventure, 4 = hard joke**. Easy and hard run
   `FANMMENU/facil` and `FANMMENU/dificil`, respectively. They are not three
   variants of the full adventure.
6. The middle button calls `0x414560`, places Jim in the harbour and starts
   **FANPRESENT/main**, using native script type 1. The script requests movie 2,
   narrated harbour reveals, walking toward the door, movie 3, and the captain's
   office conversation. Its native action 5 changes to the office and positions
   Jim at `(550, 100)`. The captain eventually leaves; the pen puzzle follows.

The script's native action 1 is annotated “presentation and credits,” but the
recovered type-1 handler has no branch for that ordinal. An annotation alone
is not evidence that the executable performs the named action.

Evidence:

- [Initialization](analysis/startup/00413e60_tesoro_initialize_game.raw.c)
  and [matching instructions](analysis/startup/00413e60_tesoro_initialize_game.asm).
- [Menu button constructor](analysis/startup/00417ad0_tesoro_initialize_menu_button.raw.c),
  [menu actions](analysis/startup/00417b40_tesoro_menu_button_click.raw.c)
  and [new-game entry](analysis/startup/00414560_tesoro_start_new_game.raw.c).
- `ANIMS.DLG`: `FANMMENU` lines 17–64; `FANPRESENT` lines 71–121.
- [Introduction native handler](analysis/decompiled/004197a0_tesoro_native_type_01.raw.c).

## How the original loop works

The Windows entry point `0x415960` pumps window messages and repeatedly calls
`0x414420`. Each iteration polls two input services, freezes the frame's clock,
services another subsystem, resets rendering lists, updates active objects via
virtual method `+0x10`, renders objects via `+0x14`, and releases the clock
snapshot. An update result of 3 skips rendering that frame, as happens when a
menu action switches scenes. The loop continues while its active object list
is nonempty.

The update/render traversals tolerate changes to the object list, and track
which objects were already visited. This is a cooperative object scheduler,
not a function that runs the entire story from start to finish. Scripts yield
for speech, movement, animations and timers so later frames can resume them.
The original frame clock ignores single wall-clock deltas of 500 ms or more.
No fixed 60 Hz rate was established from this loop.

Evidence: [Windows entry point](analysis/startup/00415960_tesoro_win_main.raw.c),
[loop step](analysis/startup/00414420_tesoro_main_loop_step.raw.c),
[object updates](analysis/startup/0040fc20_tesoro_update_objects.raw.c),
[object rendering](analysis/startup/0040fd20_tesoro_render_objects.raw.c),
and [frame clock](analysis/startup/00413a50_tesoro_begin_frame_time.raw.c).
All 11 focused exports completed; see [status](analysis/startup/decompile-status.tsv).
[Ghidra script](ghidra/RecoverStartup.java) reproduces the exports.
Decompiler types remain inferred, not original source.

## Implemented first slice

The active engine now has a single input → update → draw → present loop, paced
at 60 Hz with ScummVM's frame limiter. This pacing is a host implementation
choice. AVI decoding and the censorship timer advance through that loop;
there are no nested movie loops or busy waits. Escape skips a movie, returns
from the difficulty menu, or opens ScummVM's main menu from the title menu.
Movie playback pauses with the engine, and exiting closes its decoder/audio.

Original title/difficulty/censorship artwork and button rectangles/actions are
read from the two LVLs. Invisible button frames use a signed image index of
`-1`. The TRE filename buffer is terminated, and resource read streams now
have scoped ownership. Fake generated save/load support has been disabled.

This is a startup slice, not the playable pen prototype. Starting the adventure,
the easy/hard joke sequences and save/load currently display an availability
message. Menu fades, original cursor, scene music, general animation/script
scheduling and the harbour introduction remain to be ported. The 5.5-second
card duration comes from the script; the original executable has not yet been
run for side-by-side visual/timing comparison.

Movies can be placed in the game directory, `bin/`, or `Install/bin/`, or set
`tesoro_movie_path` in the game's ScummVM configuration to a directory such as
`/Volumes/TESORO/Install/bin`. No media is copied or modified. Missing/invalid
movies log a warning and continue to the next menu stage. Indeo 4/5 is enabled
as an engine component. The three-file installation has no external movies.

For this machine, `prototype/startup.ini` is an ignored local configuration;
`prototype/startup.ini.example` is its editable example. From the ScummVM
worktree, launch the new startup build with:

```sh
./scummvm --config=../tesoro/prototype/startup.ini tesoro-startup
```

## Validation

Build from the active worktree with `make -j8`. The integration check drives the
real event/update/render loop with original assets, checks original menu
hotspots and actions, ignores an outside click, plays both movies when supplied,
checks the censorship wait, returns to the title menu, and exits through an
injected quit event. Screenshots and runtime logs go to the requested output.

```sh
python3 tests/test_startup.py \
  --scummvm ../scummvm-tesoro/scummvm \
  --game /Users/gabriel/Desktop/ags/TESORO \
  --movies /Volumes/TESORO/Install/bin \
  --output analysis/startup-full
```

Use `--skip-movie` to check Escape during movie 0. Omit `--movies` to check the
three-file installation's missing-media path. SDL uses dummy video/audio, so
these runs decode/mix audio without producing sound through speakers.
The tests establish engine behavior, not original-game fidelity.

Verified on 21 October 2026:

| Run | Decoded movie frames | Elapsed | Result |
| --- | ---: | ---: | --- |
| Full movies 0 and 1 | 461 | 27.9 s | Pass |
| Escape skips movie 0; movie 1 completes | 67 | 11.0 s | Pass |
| Movies absent | 0 | 5.6 s | Pass |

All nine rendered menu/card snapshots match the independent Python resource
decoder pixel for pixel. The difficulty and censorship screenshots were also
visually inspected. The existing six pen-script trace checks still pass.
Runtime logs are in `analysis/startup-full/`, `analysis/startup-skip/` and
`analysis/startup-missing/`. Build output is `analysis/startup-build.log`;
its warnings concern the existing generated OpenGL function-pointer bindings.

The next implementation target is action 3 → `FANPRESENT/main` → harbour
sequence → office. Introduce the resumable script layer and the necessary
scene/object updates there, preserving native `DO` ordinals. The preserved
prototype can supply its proven readers/speech/script pieces without replacing
the active starter wholesale.
