# Office prototype

This is a working first slice of a ScummVM engine, using the supplied original
TESORO.TRE and TESORO.DAT. It starts in the office after the introduction.
It does not run TESORO.EXE.

## Play

Double-click `Tesoro Prototype.app` in this folder, or run `run.command` from Terminal:

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

Engine code lives in the separate [ScummVM fork, branch `tesoro`](https://github.com/kelmer44/scummvm/tree/tesoro/engines/tesoro).
Locally, use the sibling checkout `../scummvm-tesoro`; this notes repository
contains no engine checkout. Set `TESORO_SCUMMVM_DIR` to use a different source
location.

To rebuild and package the local app, from this repository:

```sh
make -C ../scummvm-tesoro -j8
python3 tools/package_prototype.py --scummvm ../scummvm-tesoro
```

The configuration used for the first build is saved in `build-command.txt`.
For a new clone, create `prototype/tesoro.ini` from `prototype/tesoro.ini.example`
and set the game path. The local generated app is not committed; package it
with the command above. On this machine, `prototype/run.command` and the
packaged app remain available.

The initial engine was built on commit `eb4ab9a8f8933d3d37158cfd461aab3673901093`.
The code was then committed separately on the fork's `master` base as
`56e25ababf42611c91fa35ba1c716f3a6e703b74`. The original game files and the
pre-existing ScummVM checkout were not changed.

## Validation

The engine has a controlled-host self-test and a full real-time autoplay test.
Both use the real resource archive; neither modifies it. Automated runs use SDL's
dummy display/audio outputs, so voices are decoded and mixed without playing
through speakers. These checks do not establish perfect fidelity to the original
Windows game.

```sh
cd /Users/gabriel/Desktop/source/tesoro
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy SDL_RENDER_DRIVER=software \
  ../scummvm-tesoro/scummvm --config=prototype/selftest.ini tesoro-prototype
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy SDL_RENDER_DRIVER=software \
  ../scummvm-tesoro/scummvm --config=prototype/autoplay.ini tesoro-prototype
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
