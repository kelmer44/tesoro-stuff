# Project plan: fresh function-by-function recovery and port

Started 28 October 2026. Legacy material is reference only (see `AGENTS.md`). Progress is tracked per function in
`CHECKLIST.md` (generated from `ledger.json`); this file tracks phases.

## Phase 0 — Preparation  (DONE)
Ghidra program verified (PE32, base 0x400000, SHA-256 matches). Export of 537 functions, ledger, clusters,
contract template, session protocol. Python venv with Unicorn/Capstone. Reference environments:
DOSBox-X Win95 (faithful; works, slow; real CD audio and mouse) and Wine copies (fast; patched, no mouse/music).

## Phase 1 — Complete the function inventory
Ghidra only knows functions it found by calls. Code reached only through vtables or function-pointer tables is
missed (example: the mouse update routine at `0x403aa0`). Steps: (a) scan the data sections for pointers into
`.text`, (b) define those targets as functions in Ghidra, (c) look for undefined code between functions,
(d) re-export and rebuild the ledger, (e) list pointer tables and which class each belongs to.
Exit: every code byte in `.text` belongs to a function or is documented data; ledger regenerated.

## Phase 2 — Triage and naming
Classify each function: game logic, C runtime/SEH helper, DirectX/Windows glue, import thunk. Give C runtime
functions their standard names (they are not ported). Then name game functions in batches by subsystem, from
callers/callees and strings, every name prefixed `inferred_` until contract and validation support it.
Exit: no unnamed function without a recorded reason; CRT/glue marked `replaced` or `understood`.

## Phase 3 — Structures and classes
Recover object layouts and vtables (constructors, sizes from allocations, repeated offsets) into `STRUCTS.md`
and Ghidra data types. Starts with the device classes (CD audio, keyboard, mouse, sound, video), then the
scene/object base class and the 23 object kinds, then actors.
Exit: every class has a layout with per-field confidence and a vtable with named slots.

## Phase 4 — Cluster loop (the bulk of the work)
Per cluster, bottom-up: decompile, write contract, port to ScummVM C++, validate (Unicorn emulation vs port;
later DOSBox-X reference), update ledger and checklist. Subsystem order (earliest dependency first):
1. Startup: WinMain, argument handling, init sequence, main loop and clock.
2. Platform glue: CD audio, input, sound, video — mapped to ScummVM services (mostly `replaced`).
3. Resources: TRE/DAT archive, image/animation/sound loaders.
4. Scripts: compiler, executor, native handlers.
5. Scenes and objects: the 23 kinds, scene factory (`0x409e50`), lifecycle.
6. Actors: Jim, walking, paths, camera.
7. Rendering: compositor, scaling, shadows, text.
8. UI: verbs, inventory, dialogue menus, transitions.
9. Specials: map, board game, fight; media/movies and music cues; saves.
Exit per subsystem: all its functions ported and emu-compared; open uncertainties listed.

## Phase 5 — Whole-game validation
Replay controlled sequences in the DOSBox-X reference and the port, compare state (positions, flags, frames, script
points), then full route to credits plus second route; save/load; timing.

## Phase 6 — ScummVM readiness
Platform builds, detection entries, code style, documentation, commits for Gabriel's review on `codex/*` branches.

## Where we are
Phase 0 done. Phase 1 next (small: a few hours of work). Nine game functions already read (see checklist).

## Phase 1 status (28 October 2026)
Candidates found by `tools/find_missing_functions.py` (data pointers + call targets + gap scan) and filtered by
`tools/plan_new_functions.py` -> `new_functions.txt` (370 starts: 102 pointer-backed, 268 from code gaps that start like
compiled code). `ghidra/DefineMissingFunctions.java` creates them and follows direct calls. Dry run on a read-only copy
of the legacy project (644 functions): created 141, 21 already existed, 6 failed, rest fell inside created functions.
Next: run the script in the open Ghidra project (Script Manager, add directory `re/ghidra`), re-export with
`tools/export_ghidra.py`, rebuild the ledger with `tools/build_ledger.py`, then review `FAILED` lines in the log.

### Phase 1 result (28 Oct 2026)
- Ran `re/ghidra/DefineMissingFunctions.java` in the GUI project: Ghidra's function count went from 622 to 784 (161 created, 1 already existing). The bridge lists 700 of those; the other 84 are not exported (externals/imports), the same offset as before the script (622 vs 537).
- 6 candidates failed to disassemble (`00406780`, `0040c000`, `0040c020`, `0040c030`, `00419520`, `00425000`), all from the gap scan. Treated as data or padding, not functions. Re-examine only if a caller or table turns out to point at one.
- About 100 candidates were skipped because they fall inside another function (jump-table targets and internal labels). See `re/define_missing_functions.log`.
- Re-exported to `re/export/` (700 functions). Ledger now has 700 functions, 441 game tier, 46 clusters. The 9 earlier `decompiled` statuses and names were preserved.
- Phase 1 is complete. Next is Phase 2 (triage and `inferred_` naming).
