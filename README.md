# Tesoro research and supporting tools

Aim: use AI-assisted reverse engineering to build ScummVM support for **El Tesoro de Isla Alcachofa**. Original game files are kept in `/Users/gabriel/Desktop/ags/TESORO` and are read without modification.

**Project direction:** [the complete-game master plan](MASTER_PLAN.md) is the
main guideline for all new work. [Current research status and next actions](RE_STATUS.md)
separate recovered behaviour, implementation and original-game comparison.
[Project instructions](AGENTS.md) preserve this direction between sessions;
[the research skill](skills/tesoro-re/SKILL.md) captures proven reusable methods.
The plan includes establishing a working original Windows reference environment.

**Current-source caveat, 21 October:** startup test results below describe an
earlier source state. The active worktree has subsequent commits/local edits;
its current build and harness must be reconciled before reusing those results.
See [the live checkpoint](RE_STATUS.md).

The engine code lives in [kelmer44/scummvm — tesoro branch](https://github.com/kelmer44/scummvm/tree/tesoro/engines/tesoro). This repository contains
notes, Ghidra analysis exports, format inventories, script traces, screenshots,
tests, supporting tools and the preserved prototype source. Original game assets, extracted media, the local
Ghidra database and generated app binaries are excluded.

**21 October 2026:** the active `tesoro` branch now implements the opening
movies and original title/difficulty menu navigation in a paced game loop.
See [startup and loop findings, implementation and checks](STARTUP_AND_LOOP.md).
The harbour introduction and adventure gameplay are not yet ported into that
branch. The previous playable implementation remains preserved in
[prototype/source](prototype/source/README.md); results below describe it.

**Current milestone: establish the source/reference baseline, then implement
the original harbour introduction through the office.** The pen path remains
the first interaction regression scenario.

- [Current findings and first-puzzle trace](FIRST_PUZZLE_TRACE.md)
- [Historical feasibility assessment](FEASIBILITY_AND_PLAN.md)
- [First annotated pseudocode examples](PSEUDOCODE_FIRST_BATCH.md)
- [Technical addresses and format notes](TECHNICAL_NOTES.md)
- [Original-disc media inspection](MEDIA_STATUS.md)

As of 20 October 2026:

- Image/audio readers verified: 2,012 images and 2,346 WAV clips.
- Scene structure read through the exact end of all 50 levels: 747 object records.
- Ghidra project saved; 48 selected routines decompiled with matching assembly exports.
- Read-only tracer follows the pen script's first and later pickup branches and two pen-return branches.
- Six checks against original data pass.
- Complete original disc identified: resource archives match, all 13 Indeo 5 AVI movies are available, and all 19 CD audio tracks are extracted and PCM-verified.
- A C++ ScummVM prototype now runs the office pen sequence with original artwork, voiced dialogue, subtitles, inventory changes, animation and the native exit to the harbour.
- The C++ controlled-host self-test and full real-time autoplay pass. Mouse selection and picking up the pen were also checked in the native macOS window.
- [Run the prototype and read its limits](prototype/README.md).

Next actions and milestone exits are maintained in [the live checkpoint](RE_STATUS.md)
and [master plan](MASTER_PLAN.md). Keep original game data and extracted assets local.

```sh
python3 tests/test_pen_trace.py --game /Users/gabriel/Desktop/ags/TESORO
python3 tools/inspect_tesoro.py /Users/gabriel/Desktop/ags/TESORO --output analysis
python3 tools/trace_script.py /Users/gabriel/Desktop/ags/TESORO --vars v0=0 --output analysis/traces/pen-first-pickup.json
```
