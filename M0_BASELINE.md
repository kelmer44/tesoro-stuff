# Current engine baseline: source and integration checks

21 October 2026. This completes the **source/build part** of master-plan M0.
Original-game comparison remains open; reference-environment setup is on hold
until Gabriel authorizes its environment and paths.

**Instrumentation removed:** Gabriel requires the submitted engine to exclude
test classes and test-only hooks. `StartupTest`, its friend declaration, runtime
calls and build entry have been removed. The harness is preserved only under
ignored `local-media/startup-harness/`. The results below describe the instrumented
baseline build; they must not be presented as a test of subsequent source hashes.

## Changes in the active engine

Active checkout: `../scummvm-tesoro`, branch `tesoro`, HEAD `33316cacbf8` plus
local changes. Gabriel's censorship deadline and movie/menu drawing changes
were retained. The preserved prototype was not changed.

- Added ScummVM's host frame limiter at a nominal 60 Hz. This caps CPU use;
  it does not establish the original simulation rate.
- Connected pause/resume to the AVI decoder and compensate the 5.5-second card
  deadline for paused time. Updates do not advance while the engine is paused.
- Corrected stream ownership on AVI parse failure: after header validation,
  the decoder owns the stream and closes it even when loading fails. The
  previous extra delete could free the same stream twice.
- Scoped resource read streams so archive reads release their temporary streams.
- Restored movie lookup through the configured movie path, game directory,
  `bin/`, and `Install/bin/`.
- Disabled unfinished save/load features and the unused original-save-screen
  option. Calls to unfinished save/load handlers now return an error instead of
  reporting success after serializing a dummy integer.
- Used a temporary local integration driver to verify startup. It was then
  removed from the engine; it is not part of the submitted implementation.

The engine changes are uncommitted and available for review. Build output is
local in `analysis/m0-build.log`; the build completed successfully, with the
existing linker warning about duplicate `-lz`.

## Validation

| Scenario | Decoded movie frames | Host elapsed | Result |
| --- | ---: | ---: | --- |
| Full movies 0 and 1 | 461 | 28.019 s | Pass |
| Escape during movie 0, then movie 1 | 65 | 11.021 s | Pass |
| Movies absent | 0 | 5.655 s | Pass |
| Invalid header and decoder-owned parse failure | 0 | 5.653 s | Pass |

Each scenario verifies title-menu navigation, an ignored outside click, the
card wait, the difficulty menu, Escape back to the title and an injected quit.
The driver checks movie pause when a movie is present and card pause in every
scenario. It also rejects advertised unfinished save/load support.

The three rendered menu/card snapshots from each run match the independent
Python resource decoder pixel for pixel: **12 snapshots checked**. The six
existing pen/scene trace checks also pass. SDL dummy outputs exercise decoding
and mixing without speaker output or a native window; these checks establish
host integration, not original Windows runtime fidelity.

Reports in `analysis/m0-startup-{full,skip,missing,broken}/verification.json`
record the binary and individual engine-source SHA-256 values. All four reports
were checked against the current source/binary and match the same build.
Screenshots and raw logs remain local. Integration scratch/config files now
live under the requested project-local output folder and are removed on exit.

For a separately instrumented local build only (the driver refuses to run
against the normal engine):

```sh
python3 tests/test_startup.py \
  --scummvm ../scummvm-tesoro/scummvm \
  --game /Users/gabriel/Desktop/ags/TESORO \
  --movies /Users/gabriel/Desktop/ags/TESORO/bin \
  --output analysis/m0-startup-full
```

Add `--skip-movie` for the Escape scenario. Omit `--movies` to test a deliberately
isolated three-file installation, even when the real game folder contains
movies. Use `--broken-movies` without `--movies` to test both stream-failure paths.

## Next dependency

Recover the shared object/animation and resumable script behaviour required by
the introduction before connecting the real-adventure button. The new
[introduction contract](INTRODUCTION_CONTRACT.md) records native dispatch and
remaining dependencies. Native `DO` annotations do not supply missing effects.
