# Tesoro research and supporting tools

Aim: use AI-assisted reverse engineering to build ScummVM support for **El Tesoro de Isla Alcachofa**. Original game files are kept in `/Users/gabriel/Desktop/ags/TESORO` and are read without modification.

The engine code lives in [kelmer44/scummvm](https://github.com/kelmer44/scummvm),
currently on the dedicated Tesoro development branch. This repository contains
notes, Ghidra analysis exports, format inventories, script traces, screenshots,
tests and supporting tools. Original game assets, extracted media, the local
Ghidra database and generated app binaries are excluded.

**Current milestone: pick up the pen in the office and leave through the door.**

- [Current findings and first-puzzle trace](FIRST_PUZZLE_TRACE.md)
- [Overall feasibility and plan](FEASIBILITY_AND_PLAN.md)
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

Next: compare the working path with the original Windows game, then recover floor/pathfinding, actor scaling and general hotspots. Keep original game data and extracted assets local.

```sh
python3 tests/test_pen_trace.py --game /Users/gabriel/Desktop/ags/TESORO
python3 tools/inspect_tesoro.py /Users/gabriel/Desktop/ags/TESORO --output analysis
python3 tools/trace_script.py /Users/gabriel/Desktop/ags/TESORO --vars v0=0 --output analysis/traces/pen-first-pickup.json
```
