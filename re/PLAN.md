# Project plan: fresh function-by-function recovery and port

Started 28 October 2026. Legacy material is reference only (see `AGENTS.md`). Progress is tracked per function in
`CHECKLIST.md` (generated from `ledger.json`); this file tracks phases.

## Phase 0 — Preparation  (DONE)
Ghidra program verified (PE32, base 0x400000, SHA-256 matches). Export of 537 functions, ledger, clusters,
contract template, session protocol. Python venv with Unicorn/Capstone. Reference environments:
DOSBox-X Win95 (faithful; works, slow; real CD audio and mouse) and Wine copies (fast; patched, no mouse/music).

## Phase 1 — Complete the function inventory  (DONE 28 Oct 2026)
Ghidra only knows functions it found by calls. Code reached only through vtables or function-pointer tables is
missed (example: the mouse update routine at `0x403aa0`). Steps: (a) scan the data sections for pointers into
`.text`, (b) define those targets as functions in Ghidra, (c) look for undefined code between functions,
(d) re-export and rebuild the ledger, (e) list pointer tables and which class each belongs to.
Exit: every code byte in `.text` belongs to a function or is documented data; ledger regenerated.
Result: 700 exported functions (441 game tier, 46 clusters). Not yet re-run: the gap scan over the new function set to confirm no
undefined code remains; do that first thing in Phase 2 if any caller points into a gap.

## Phase 2 — Triage and naming  (IN PROGRESS: CRT step done)
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
Phases 0 and 1 done. Phase 2 (triage and naming) is next. Nine game functions already read (see checklist).

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

### Phase 1 closing pass (28 Oct 2026)
- Gap scan re-run on the 700-function set found 7 more starts (`new_functions.txt`; first batch kept in `new_functions_batch1.txt`). Defined 6 in Ghidra (`00401d20`, `00401f90`, `004087a0`, `0040f270`, `00417b30`, `00419340`; function count 785 -> 791). The seventh, `00408880` (a lone `ret`), is the shared target of `jmp` from SEH unwind stubs at `00426ff0`/`00427090`; Ghidra counts it as part of those, so it is left as is.
- Remaining uncovered starts are `0041a340` and `0041b340`: data in `.text` (CRT tables), not code. Other gaps are jump tables, `ret N` tails, exception-handler epilogues and padding.
- Re-exported: 706 functions, 447 game tier, 47 clusters. Pointer tables dropped from 361 to 64 because runs of function pointers that were broken by missing functions are now contiguous, which is the expected effect.
- Phase 1 exit criterion met. Phase 2 can start.

### Phase 2 status (28 Oct 2026)
**Step 1, CRT/runtime tier: done.** 256 runtime-tier functions (address >= 0x41b3c0 plus EH funclets), all marked `replaced`.
- Only 23 are called directly from game code: file I/O (`fopen`, `fread`, `fwrite`, `fseek`, `fgetpos`, `fsetpos`, `fclose`, `ungetc`), `sprintf`, `sscanf`,
  `fscanf`, `_strncpy`, `_strncmp`, `_strupr`, `ctime`, `_stat`, `_findfirst`/`_findclose`, `__ftol`, `operator new`/`delete`, `__chkstk`.
  Names in `re/names/crt.tsv` (Ghidra signature matches have no prefix, behavioural IDs carry `inferred_`).
- The CRT has no `rand`; the game must use its own random generator in the game tier (to find during triage).
- Fidelity-relevant CRT behaviour to remember: `__ftol` truncation (every float-to-int in game code goes through it), `sprintf`/`sscanf` formats
  (string and script text handling), `_strupr` (ASCII vs locale), `fseek`/`fread` on the data archive.
- 16 functions at 0x426f80..0x427090 are compiler EH unwind funclets inside game function frames (delete/destructor calls), not logic.
- About 190 CRT-internal functions are marked `replaced` without individual names (note says so): the named 23 reach 125 of the 256, the rest are startup/SEH.
  They are not ported; revisit one only if a game function turns out to depend on it.
- 63 names pushed to the Ghidra database through `rename_function_by_address`; save the project (Ctrl+S) to keep them.
**Next, step 2: name the 447 game functions cluster by cluster** (start with cluster 1 / startup), from callers, callees and strings.
Tool: `re/tools/apply_names.py <tsv>` applies name/status/notes in bulk.

### Phase 2, startup path named (28 Oct 2026)
`re/names/startup.tsv` (36 functions) plus the 9 earlier ones. Note: cluster numbers in `clusters.md` are stale after the inventory change; use `ledger.json`.
Startup structure read from the fresh decompilation:
- `WinMain` (no argument: CD check and installer menu; `_$_$_`+drive letter: game): `CreateMainWindow` -> `InitSubsystems` -> `InitGameWorld` -> loop {PeekMessage/GetMessage; `RunFrame`} -> `Shutdown`.
- Global singletons: clock 00442120, memory pool 00442100 (4 arenas: 0x17a6b0, 3,000,000, 9,600,000 and 650,000 bytes), archive manager 00442064, keyboard 00442074, mouse 0044206c, sound 00442080, display 00442108, main object list 00442104 (capacity 100).
- Per frame: input updates, clock freeze, sound update, pool arena 3 reset (per-frame scratch), render queue reset, object list pass A (vtable slot 4) then pass B (slot 5), clock unfreeze.
- The game clock ignores any single tick gap of 500 ms or more (a stall does not advance game time). Timing in the port must reproduce this.
- Resources are found by name in a sorted index (`Tesoro.TRE`), data read from `Tesoro.DAT`. Name hash: 5 bits per character, 12 characters, 64-bit.
- Still unread: `InitGameWorld` callees (level loader 004080e0, dialogue loader 00406190, 004070c0 game-state constructor, 00415230), the object-list insert/remove functions, and the render path.

### Phase 2, InitGameWorld callees (28 Oct 2026)
`re/names/startup2.tsv`: 28 more functions (level loader, object factory, camera, lists, dialogue table, room records, movie setup).
- Everything game-visible derives from one base object (`Object_BaseCtor`, vtable 00428000, flags word at +0x68 with bit 1 = active, 0x200 = processed this frame).
  A Level is itself such an object and sits in the main object list (00442104) with a priority; its own children live in an object list at +0x74, room records
  (0x10 bytes) at +0x70 and anim banks at +0x6c.
- The scene object factory `Level_CreateObject` switches on the record kind; kind 5 has a second switch. This is the 23-kind dispatch to port early.
- Linked lists use a pooled node array with a built-in iterator (current node at +0x10 or +0xc, "first" flag); the same layout appears in several list types
  with different node sizes. A port can use plain containers but must keep ordering semantics (insert by priority, insert by +0x4c, push back).
- Caveat: `Level_Load` is garbled in the decompiler output because of its huge stack frame; port from disassembly.
- Still unread: `Level_CreateObject` per-kind constructors, `004070c0` (game-state object, 0x114 bytes), `Movie_Start` remainder, dialogue entry class (`00404810`).
