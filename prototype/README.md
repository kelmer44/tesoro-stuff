# Office prototype

**21 October 2026:** this describes the preserved prototype. Its source is in
[source/engines/tesoro](source/engines/tesoro), with a [copying guide](source/README.md).
The active ScummVM `tesoro` branch has been reset to a generated starter.

This is a working first slice of a ScummVM engine, using the supplied original
TESORO.TRE and TESORO.DAT. It starts in the office after the introduction.
It does not run TESORO.EXE.

## Play

The existing local `Tesoro Prototype.app` keeps the playable prototype binary.
Double-click it, or run `run.command` from Terminal:

```sh
/Users/gabriel/Desktop/source/tesoro/prototype/run.command
```

The small dark pen is on the papers on the desk. `Coger` is already selected;
click the pen. Jim reads the letter, the captain returns, their original voiced
conversation plays, and Jim leaves for the harbour. The pen appears in the
bottom inventory bar. Click the left doorway in the harbour to return.

- `Coger`: pick up the pen.
- `Mirar`: click the pen to hear its original description.
- `Andar`: click a point to try provisional straight-line walking.
- Space skips the current voice line.
- R resets this prototype to its starting state.
- Escape exits.

The artwork, letter, Spanish dialogue, voice recordings and animation frames are
read from the original archive. The three-button toolbar is temporary.

## What remains approximate or absent

This covers one puzzle path, not the full game. The original floor/pathfinding,
camera controller, actor depth and complete animation rules still need work.
Walking uses a straight line and measured background widths constrain scrolling. Jim uses a temporary 25% scale in the harbour; the original floor-based scaling is not implemented.
Talking uses the original talk banks with a first-frame body beneath smaller
head patches; exact mouth timing and shadow blending are unverified. The captain
starts absent and Jim starts near the right door, explicitly chosen for this
slice rather than recovered from the full introduction. Hotspots are limited to
the pen and the two connecting doorways. Other puzzles, conversations, general
native callbacks, saves, original interface, movies and CD music playback are
not implemented. The complete local CD music/movie inventory is preserved for
later work; this prototype does not guess the scene music track.

Puzzle state belongs to the office captain, as in the recovered pickup handler.
The script's transfer to `FCAPITAN/salir` preserves that state. The office keeps
its object state when the harbour is entered. `activate` and `show` have separate
state, and unsupported script instructions stop with a source location instead
of being skipped. Only tail-position script transfers are supported.

## Source and build

The active [ScummVM `tesoro` branch](https://github.com/kelmer44/scummvm/tree/tesoro/engines/tesoro)
contains only the generated starter. Its local checkout is a linked worktree at
`../scummvm-tesoro`, sharing Git metadata with `../scummvm-fork` and the Igor and
Darkseed 2 worktrees. The notes repository contains the preserved source at
[source/engines/tesoro](source/engines/tesoro), not a full ScummVM checkout.

Use [the copying guide](source/README.md) to port selected components into the
starter. `source/manifest.json` identifies the exact saved files. The local
`archive/tesoro-prototype` branch retains the old working commit as well.

`prototype/run.command` now runs the existing preserved app binary, independently
of the starter build. `tools/package_prototype.py` can still package a specified
build, but do not use the starter build to overwrite the preserved app unless
that is what you intend.

The configuration used for the first prototype build is saved in
`build-command.txt`. The original working engine commit is
`56e25ababf42611c91fa35ba1c716f3a6e703b74`, based on
`0b87092bc08a9f4f9d5cfb6637d0c10448774c1f`.

## Validation

The preserved prototype has a controlled-host self-test and a full real-time autoplay test.
These commands require a build of that prototype, not the generated starter.
Both use the real resource archive; neither modifies it. Automated runs use SDL's
dummy display/audio outputs, so voices are decoded and mixed without playing
through speakers. These checks do not establish perfect fidelity to the original
Windows game. For the local packaged binary, use:

```sh
cd /Users/gabriel/Desktop/source/tesoro
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy SDL_RENDER_DRIVER=software \
  'prototype/Tesoro Prototype.app/Contents/MacOS/tesoro' --config=prototype/selftest.ini tesoro-prototype
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy SDL_RENDER_DRIVER=software \
  'prototype/Tesoro Prototype.app/Contents/MacOS/tesoro' --config=prototype/autoplay.ini tesoro-prototype
python3 tests/test_pen_trace.py --game /Users/gabriel/Desktop/ags/TESORO
```

Self-test: every resource index entry is checked, all 2,012 images decode, all 50
scenes/747 object records parse, both pickup branches wait before advancing, and
the native door transition goes to the harbour and back. Autoplay runs the actual
host through speech, sleeps, movement, animation and the native exit, then checks
the room, captain's variable, inventory, pen visibility and letter visibility.
See the saved logs and screenshots under `analysis/`.

Next useful milestone: compare this path with the running Windows original,
then recover the floor/path code and general object hotspots before adding more
puzzles. This will improve the foundation instead of accumulating guessed
behaviour.

## Launcher appearance

The initial minimal build disabled FreeType, so the launcher could fall back to
bitmap fonts. The corrected local build enables it; the exact configure command
is in `launcher-build-command.txt`. This changes only ignored local build
configuration, not the generated engine source.

Run ScummVM from its source root so it can find its theme/font resources, or pass
the paths explicitly:

```sh
cd ../scummvm-tesoro
./scummvm --themepath=gui/themes --extrapath=dists/engine-data
```

Restart an already running launcher to use the rebuilt binary. The separately
preserved prototype app still contains its original binary.
