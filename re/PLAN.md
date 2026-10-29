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
  `fscanf`, `_strncpy`, `_strncmp`, `_strupr`, `ctime`, `_execl`, `_findfirst`/`_findclose`, `__ftol`, `operator new`/`delete`, `__chkstk`.
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

### Phase 2, object constructors (28 Oct 2026)
`re/names/objects.tsv` (27 functions) and `re/object_kinds.json` (kind -> constructor, size, per-subtype vtables, parsed from `Level_CreateObject`).
- The factory has **25 kinds** (values 0..25, kind 18 absent and creates nothing), not 23. Legacy notes said 23; treat the fresh number as authoritative until the scene-record format is read.
- Constructors: ~22 distinct ctors. Kinds 9, 12, 17 and 20 share constructor `0040ed10` and differ only in per-subtype vtables (98 subtype vtables across kinds 5, 6, 12, 13, 17, 20 and 9's alias).
  Puzzle and scene behaviour therefore lives in many small classes in the executable, selected by a subtype byte in the level record; this is why the game tier has ~450 functions.
- Hierarchy: `Obj_Kind00` (sprite/animated base) <- `Obj_MidBase` <- kinds 8, 11, 13, 15, 21, 22, 23, 25; `Kind00` <- kinds 1, 2, 3 (<- 4, 10, 16), 7, 14, 19; `Kind09` <- kinds 5, 6, 24 (and 12, 17, 20 as aliases).
- **Random numbers:** `RandomRange(min,max)` derives its value from bit-shuffled low bits of `GetTickCount()`; it is not seeded and cannot be replayed. Anything the original does "randomly" is time-driven. The port needs an equivalent time-driven source, and original-runtime comparisons must not assume determinism.
- Phrase text is looked up by name hash across four tables (`PhraseLookup`); a missing phrase shows a message box.
- Kind 10 sets mouse bounds, so it is probably the pointer/cursor object (hypothesis).

### Phase 2, base class methods (28 Oct 2026)
`re/names/baseclasses.tsv` (30 functions) and `re/vtables.md` (slot tables for `Obj_Kind00`, `Obj_MidBase`, the Kind09 family and the start of the Kind05 family).
- Every object serialises itself (`LoadState`/`SaveState` slots 9/10), so save games are per-object streams; the port must reproduce each class's stream order.
- Interaction model: a click calls `OnClick` (slot 19) which sends the player to the object's approach point; when the player arrives the object's pending flag makes
  `Interactive_Update` run the handler for the selected verb (slots 20-26). Verb 0-6 are fixed; 7 and above are inventory items passed to slot 27 as (verb - 7).
  Unset verb phrases fall back to default phrases.
- Drawing goes through a per-scanline render queue (`Display_AddSprite`), sorted by each object's +0x5e value.

### Phase 2, player object (28 Oct 2026)
`re/names/jim.tsv` (22 functions) and the Jim table in `re/vtables.md`.
- Jim (`004070c0`, global 004420fc) loads JIM*.ANM banks (walk/idle per direction, talk) and moves by **root motion**: each frame the animation's own offset is
  scaled by a perspective factor from the walk area and added to a 24.8 fixed-point position. There is no speed constant; walking speed is a property of the animations.
- A walk ends when the squared distance to the target reaches exactly zero (the step is clamped to the target when it would overshoot), then the target object's `RunVerb` runs.
- Facing is one of four, chosen from the dominant axis of the displacement. Talking picks a random mouth frame every 100 ms of game time (time-driven RNG).
- The kind 1 object stored at level +0x90 holds the walk area with perspective data (`WalkArea_ScaleAt`, `WalkArea_FindPath`, both only named, not yet ported/understood).
- Caution: `Jim_WalkTo` calls the target's slot 13 unconditionally at its end; meaning to be understood before porting click-walking.
- Legacy walking notes (`CLICK_WALKING.md`) exist; compare only after the fresh walk-area functions are read.

### Phase 2, walk area (28 Oct 2026)
`re/names/walkarea.tsv`. Fresh finding: movement across a room is not graph or grid search. The walk area (kind 8) is a polyline whose vertices carry a perspective scale.
Click -> `FindPath` -> Jim walks to the vertex nearest him -> `NextWaypoint` runs on each arrival and steps along vertex indices toward the vertex nearest the destination
-> final straight walk to the clicked point, then the target object's verb runs. This explains why `Jim_WalkTo` ends by calling the target's slot 13.
Direct lines between waypoints use Jim's root-motion step with clamping (see `Jim_Update`). Object slot 0 is `OnAttach`, not a destructor.
Open: how the engine decides a click is on walkable ground versus on an object (hit-testing) and the cursor object's input handling are still unread.

### Phase 2, cursor and clicks (28 Oct 2026)
`re/names/cursor.tsv` (10 functions). Established: the cursor keeps a list of candidate objects (those under the pointer); each frame `Cursor_ProcessCandidates` sends
left-click as slot 14 (verb = cursor animation index), right-click as slot 15 and plain hover as slot 17 (hover text). Carried inventory items are cursor animation indices >= 7.
`Dialogue_StepUpdate` is the per-frame executor of dialogue/cutscene action lists (24-byte steps) and is a high-priority reading target.
**Not yet found: the geometric hit test** that decides which objects register as candidates; the call is made through a virtual slot that the decompiler does not show. Next step: scan the
object classes' slot 5 / draw code for `Cursor_AddCandidate` calls via the disassembly.
Possible save-game feature: `Cursor_CaptureThumbnail` (213x160) suggests save slots carry a thumbnail.

### Phase 2, hit test search (28 Oct 2026)
Result: **ground click located, object hit test still not found.**
- Ground click: `Cursor` slot 5 (`004030e0`) runs `Cursor_ProcessCandidates` for the cursor and its companion; if the left button went down, no candidate reacted and the cursor animation index is 0,
  Jim is told to walk (slot 32) to world position (mouse x + camera x, camera y - mouse y), after interrupting speech.
- Objects flagged 8 (collidable) and 2 (active) are tested pairwise per room by `Level_CollisionPass`/`Level_ResolveOverlap` (swept polygon and segment tests). The cursor object sets flag 8, so the pointer
  is probably tested through this pass; no virtual call registers candidates with the cursor, and the registration into the candidate list (`Cursor_AddCandidate`, slot 12) still has no caller by name or
  by `[reg+0x30]` call in the disassembly. Candidates may be added through a copy of the list pointer. Next: trace writes to the cursor's lists (+0x6c/+0x70) and read `00409280`, `00409560`, `00409790`, `00409c10`.
- `Dialogue_ChoiceMenu` (`00407d00`) was named from its mouse-y selection.

### Phase 2, hit test resolved (28 Oct 2026)
The "missing" hit test is the generic collision protocol: the cursor is an ordinary collidable object whose frame polygon is tested against other objects by `Level_ResolveOverlap`, and
`Cursor_AddCandidate` is reached as `hitter.slot12(other, extra)` from the queued-contact flush in `Level_CollisionPass` (the earlier search missed it because slot 12 takes two arguments and the
flush pushes them from a list entry). Details in `re/vtables.md`. Remaining unknowns: how the queue prioritises contacts (it special-cases the player object), where the cursor's flag 0x400 is set, and the geometry routines' exact algorithms.
Consequence for the port: hovering and clicking must be reproduced with the original polygon tests and sort-value priority, not with simple rectangle hit-tests.

### Phase 2, dialogue executor (28 Oct 2026)
`re/names/dlgscript.tsv` (11 functions) and `re/SCRIPT_LANGUAGE.md`. Story/puzzle sequencing runs in a text script language compiled at load time into 24-byte steps and executed by `DlgScript_RunSteps`/`Update`.
This is a distinct script system from the per-subtype object classes; both must be reproduced. Parser `DlgScript_Load` (691 lines) is the next large reading target in the script cluster.
The step executor drives movies (DirectShow), WAV playback, CD audio tracks, actor walking and animations, and byte-sized script variables.

### Phase 2, script parser (28 Oct 2026)
`DlgScript_Load` read: first-character dispatch of ten command families (speech line, end, option marker, label, stop, if-jump, jump, menu, play, run, sleep/sendmessage, variable assign). Grammar and step mapping in `re/SCRIPT_LANGUAGE.md`, now marked as read.
Facing numbers in scripts (ARRIBA 1, ABAJO 2, IZQUIERDA 3, DERECHA 4) agree with Jim's facing values, which cross-checks both readings.
Open: the exact meaning of the `m` menu flags (`n`), the `D` marker and the second token in the block header; verify with a real script from the data archive (need the extracted script text; reference environment is on hold).

### Phase 2/4, startup contracts (28 Oct 2026)
18 contracts in `re/contracts/` (WinMain, FindCdAndSetup, CreateMainWindow, MainWndProc, RunFrame, Shutdown, InitSubsystems, Clock x6, RandomRange, NameHash64, SetInitFlags, Pool_AllocZero, Pool_ResetArena), now `understood`.
`InitGameWorld` stays `decompiled` (its callees and the level data format are unread). Layouts for Clock, Pool, Obj base and AnimPlayer are in `STRUCTS.md`.
New measurement tools: `re/tools/emu.py` (Unicorn harness over the verified executable, IAT stubs) and `re/tools/check_startup.py` (RandomRange 3000/3000 and NameHash64 212/212 equal to independent models).
These checks validate the *recovered semantics* against the original machine code; they do not make any function `ported` or `emu-compared`, since no engine code exists yet.
Correction made while writing contracts: `0041c3b0` is an exec-family wrapper (launches SETUP.EXE via CreateProcessA), not `_stat`.

### Phase 2, naming pass complete (28 Oct 2026)
All 447 game-tier functions and all 256 runtime-tier functions now have a recorded name or a recorded reason. Breakdown of the 447 game functions:
- 18 `understood` (contracts), 216 `decompiled` (read; name backed by reading), 189 `untouched` with a **structure-based name** only (`inferred_C<vtable>_<role>`: first vtable that contains the function and the base-class slot role; bodies unread, listed in `re/names/vtable_slots.tsv`),
  plus 24 more `untouched` with a descriptive name from call sites only (listed in their ledger notes as "body not yet read").
- `re/vtable_map.json` maps every vtable base to its slot functions (153 bases found by scanning decompiled constructors; one bogus base in the CRT range).
- Corrections made on the way: `Display_Init` was a mistaken label (00411c00 is a ready-check; the Display constructor does the DirectDraw setup); two time-driven random functions exist (`RandomRange`, `RandomRangeAlt`); a board mini-game (7-column, four-in-a-row style) exists in the 0x418d20 region (hypothesis).
Remaining naming debt is therefore **quality**, not coverage: semantic names for the 189 structure-named methods require reading them, which is Phase 4 work cluster by cluster.

### Phase 3, structures and classes (28 Oct 2026)
Tool: `re/tools/build_classes.py` -> `re/classes.json`, `re/STRUCTS_GENERATED.md`, `re/types/tesoro_types.h` (header compiles with `cc -fsyntax-only`). Hand-assigned field names in `re/field_names.json`.
Result: 152 classes (every vtable found), 150 with a parent, inheritance tree rooted in `Obj_Kind00` (children: Level family, Kind03 family, `Obj_MidBase` -> {script entries, Kind13 family, Kind09 family -> Kind06 family, Kind05 family -> Jim}) plus the separate `InputDevice` (Keyboard, Mouse) root.
Field confidence over 559 field records: {'w?': 40, 'c': 227, 'r': 219, 'w': 73}. Jim, Cursor, Obj base layouts agree with the layouts read by hand.
Honest gaps (Phase 3 exit is **not fully met**):
- 29 classes have no field accesses of their own (leaf subtype classes that only override behaviour) and 30 have no known allocation size (device classes, script entry subclasses).
- Field widths are the decompiler's access widths; 9 child/parent width disagreements remain unresolved; names exist only for the Obj base, Kind09 family, Jim, Cursor and Level fields.
- Types were NOT applied inside Ghidra (the bridge cannot create data types). To apply: Ghidra > File > Parse C Source > `re/types/tesoro_types.h`, then set `this` types on methods.
- Level subclasses (Cubierta, Mapa, Interface) and the 21 script-entry subclasses are placed by their constructor stores; the Level layout itself is only partly known because `Level_Load` decompiles badly.
Also renamed `004035b0` to `InputDevice_Ctor` (it is the base-class constructor of Keyboard and Mouse).

### Phase 3, gaps closed (28 Oct 2026)
- **Sizes:** all 150 concrete classes now have an allocation size (evidence: factory sizes, InitSubsystems for keyboard 0x38 and mouse 0x40, InitGameWorld for the top object 0x6c and level family 0x9c, DialogueTable_Load for every script entry 0xd8). `InputDevice` and `Obj_MidBase` are marked abstract (never allocated alone).
- **Width conflicts:** the 9 reported conflicts were the 64-bit name key at +0x44 accessed as two dwords (+0x44/+0x48) in some functions; the generator now marks such accesses `part of` the wider field (36 records marked). No real conflicts remain.
- **Classes without fields:** 25 classes; 6 override nothing (their vtable equals the parent's slots, they are type tags) and the rest override one or two methods that touch no field of `this` (verb or item-use handlers that only call phrase functions). Nothing further to recover from field accesses.
- **Field names:** added for Keyboard, Mouse, Cursor, script entries (`DlgScriptEntry`), the Kind05 family and Level (`re/field_names.json`).
- **Plain (non-virtual) classes** are now covered by `re/plain_classes.json` and a section of `STRUCTS_GENERATED.md`: Clock, Pool, Archive, AnimPlayer, Camera, Display, SoundManager, SoundSlot, CdAudio, DialogueTable, TextTable, List, ObjectList. These are derived from hand-listed member functions and are weaker: members that reach fields only through pointer arithmetic (e.g. the dialogue table's entry array at +0x54, the pooled list nodes) are missing; CdAudio has no size. Treat them as lower bound layouts.
- Still not done: applying the types inside Ghidra (bridge limitation), field names for most subtype classes, and the pooled-list/node layouts.

### Phase 4 planning (29 Oct 2026)
`re/PHASE4_PLAN.md` written (plan only). Key facts: Gabriel's checkout is a detached HEAD at `b68a4c748e0`; `tesoro-engine` is `98415538c49` (== `my-fork/tesoro-engine`); the codex worktree is on `tesoro-engine-re` at the bare engine skeleton `cd6decf664f`.
Open decision for Gabriel: base the fresh port on that skeleton (recommended, `codex/fresh-core`) or audit/fix on top of the `tesoro-engine` tip (`codex/fidelity-core`). First increment: GameClock, nameKey64, time-driven random functions and the frame-order skeleton, validated against the original machine code with vectors from `re/tools/check_startup.py`.

### Phase 4, increment 1 done (29 Oct 2026)
Base A chosen. Engine worktree `/Users/gabriel/Desktop/source/scummvm-tesoro-codex`, branch **`codex/fresh-core`** cut from `cd6decf664f` (the skeleton), all changes **uncommitted**:
`engines/tesoro/core/{clock,namekey,random}.{h,cpp}`, `module.mk`, and `tesoro.{h,cpp}` (640x480 16-bit setup, `runFrame()` skeleton in the original's order, clock suspend/resume via `pauseEngineIntern`, `gameTime()`, `randomBetween()`). Stale `.o` and `libtesoro.a` from older builds in that directory were deleted before building; the engine builds (`make -j12`).
Validation: `re/tools/run_startup_check.sh` (vectors from the original code in Unicorn vs the C++ driver): RandomRange 4000/4000, RandomRangeAlt 4000/4000, nameKey64 522/522, clock `now` 4141/4141.
Ledger: 9 functions `emu-compared` (6 clock, 2 random, name hash). `re/AUDIT.md` started; two divergences with `tesoro-engine` recorded (clock only runs in gameplay there; seeded RNG).
Renamed `00413ac0` to `RandomRangeAlt`: the earlier name (RandomRange7) and "7 bits" description were wrong (it also uses 10 tick bits).
Next: increment 2 (archive index and lookup). It needs contracts first for `Archive_ReadIndex`, `Archive_Tell` and `Archive_SeekSavePos`, plus a look at how the data files are laid out (`Tesoro.TRE`/`.DAT`, whose location is outside the files I may read without asking).

### Phase 4, increment 2 contracts (29 Oct 2026)
Contracts written (status `understood`) for the archive cluster: Ctor 00403220, Open 00403250, SeekEntry 004032e0, Tell 00403300, SeekSavePos 00403320, Close 00403360, ReadIndex 00403380, FindEntry 00403500. Finding: the index is sorted **descending** (selection sort, first maximum wins) and FindEntry's binary search is written for that order; the older note said only "sorted by name". Not yet done: C++ port, Unicorn vectors, and the shipped `Tesoro.TRE` has not been read (duplicate names, >2300 entries, meaning of record +4 and of the 4-byte offset skip stay open).

### Phase 4, increment 2 ported (29 Oct 2026)
`engines/tesoro/core/archiveindex.{h,cpp}` (pure: load + sort + find) and `archive.{h,cpp}` (Common::File wrapper: open, seekEntry, pos, seekSavePos) on `codex/fresh-core`, uncommitted; engine builds. `run_startup_check.sh` now also runs the archive check: original ReadIndex/FindEntry in Unicorn vs `ArchiveIndex` on synthetic .TRE images, 400/400 and 35527/35527. ReadIndex/FindEntry are `emu-compared`; the file wrappers are `ported` only. The real Tesoro.TRE was not read.

### Phase 4, increment 3 contracts (29 Oct 2026)
Contracts (status `understood`) for the animation bank cluster: ReturnThis 00401000, Copy64Bytes 00401580, SetRange 00401740, ScaleBrightness 00401010, LoadDeferredImages 00401510, LoadImage 004015a0, Load 00401160. ScaleBrightness, Load and SetRange were read from the assembly because the decompiles mis-assign stack offsets. Findings: Load seeks to FindEntry-result + 4 (a second 4-byte skip); sequence-record extra data starts at +6 (type 2) and +8 (type 4); the sequence's image pointer is a pointer to the bank's image *slot* (so deferred loading fills it later); images are allocated from the arena numbered by the load mode; LoadImage's 5-5-5 conversion runs over twice the pixel data (overrun into free pool space); ScaleBrightness does not clamp blue. Not ported or validated. Unread: the player update (advances +0x2c/+0x2e), callers of ScaleBrightness, where flag +0x67c is set.

### Phase 4, increment 3 ported and validated (29 Oct 2026)
`engines/tesoro/core/animimage.{h,cpp}` (image blob: load, 5-5-5 conversion, brightness) and `animbank.{h,cpp}` (AnimBank: sequences and images, shared; AnimPlayer: play state, setRange) on `codex/fresh-core`, uncommitted; the engine builds. `run_startup_check.sh` now also runs the animation check (gen_vectors_anim.py, drivers/anim_driver.cpp): original Load, LoadImage, LoadDeferredImages, ScaleBrightness and SetRange in Unicorn vs the port on 700 synthetic animation files; every comparison matches (sequences 2509, images 1396/861/832, SetRange 1748; see contracts for the full table and limits). Five deliberate mutations of the port were caught. `emu.hook_function` gained `pops=True` for functions that return with `RET n`.
Findings from the vectors: none contradicted the contracts. Still open: the pool (allocator 0040dc10 was replaced by a bump allocator in the test), the player update that advances the counters, callers of ScaleBrightness, where the 5-5-5 flag is set, and everything about the real .DAT.

### Pending renames (engine names kept neutral until a caller confirms)
Rename in `animbank.{h,cpp}`, the contracts and `tools/drivers/anim_driver.cpp`, then rerun `run_startup_check.sh`.
| Name now | Likely meaning (legacy engine, unconfirmed) | Evidence needed |
|---|---|---|
| `AnimSequence::field0` (file +0, dword) | frame position x, y (two u16) | code that reads record +0/+2 when drawing |
| `AnimSequence::field0c`, `field10` (file +0xc, +0x10) | hit rectangle x, y, w, h (four u16) | click/hover test that reads them |
| `AnimSequence::entries` (type 2 and 4) | path points (type 2), floor points x, y (type 4) | callers of the sequence's +0x20 array |
| `AnimSequence::scales` (type 4) | scale per floor point | callers of the sequence's +0x14 array |
| `AnimSequence::firstDescent` | index where the points' x stops ascending | code that reads record +0x1e |
| `AnimBank::headerDword(0..2)` | a 12 byte name | any code that reads player +0x32..+0x3d |

### Phase 4, player update done (29 Oct 2026)
Contracts, port and validation for `AnimPlayer_Advance` 00401860, `Pause` 00401a50 and `Resume` 00401a70 (`AnimPlayer::advance/pause/resume`, engine builds, uncommitted). Unicorn vectors: advance 1131/1131, pause 276/276, resume 101/101; six mutations caught. Findings: +0x30 is the sequence being played, +0x2c/+0x2e the movement gathered by completed loops, +0x28/+0x2a the movement offset computed each update (interpolated between sequences when header flag 3 is set), +0xc a cycle length used to skip whole cycles. The engine fields were renamed accordingly (`current`, `loopX/Y`, `movementX/Y`, `pausedAt`). Not covered: zero-duration sequences (division by zero or endless loop in the original), resume before any pause (reads an uninitialised field).
