# Phase 4 plan: first port increments (29 Oct 2026) — plan only, nothing changed in the engine

## Facts checked (read-only)
| Item | Finding |
|---|---|
| Gabriel's checkout `/Users/gabriel/Desktop/source/scummvm-tesoro` | **detached HEAD at `b68a4c748e0`** ("Speech and talking animations"), clean tree; an ancestor of `tesoro-engine`. Not on a branch, so not a base for anything. |
| `tesoro-engine` | `98415538c49` ("comments", 28 Oct); identical to `my-fork/tesoro-engine`. Contains the earlier, analysis-derived engine (~8k lines: scene, objects, overlap, jim, script, interface, natives, testing shortcuts). |
| Codex worktree `/Users/gabriel/Desktop/source/scummvm-tesoro-codex` | branch **`tesoro-engine-re` at `cd6decf664f` "TESORO: Create engine skeleton"**, clean, 0 commits ahead / 37 behind `tesoro-engine`, ~260 lines (detection, metaengine, console, bare `tesoro.cpp`), stale `.o` files from an earlier build. It is an ancestor of `tesoro-engine`, so branching from it still "branches off `tesoro-engine`'s history". |
| Remote | only `my-fork` (kelmer44/scummvm) and `scummvm` (upstream). Nothing to push now; per project rules nothing is committed or pushed by Codex/Claude unless Gabriel asks, and no Claude trailers. |

## Decision needed from Gabriel (base of the fresh port)
- **A (recommended): fresh code from the skeleton.** New branch `codex/fresh-core` cut from `cd6decf664f` in the codex worktree. Matches the restart rule ("implementation based on newly decompiled functions"; legacy engine consulted only afterwards, to compare). Cost: re-creates what `tesoro-engine` already has, but with a contract behind every function.
- **B: audit and fix in place.** Branch `codex/fidelity-core` from the `tesoro-engine` tip. Faster to playable, but inherits analysis-derived behaviour and the `testing.h` shortcuts that the project rules forbid in a submission. Better as a *comparison* source than as a base.
Either way: new branch, tree left dirty, no commit, no push, no trailers.

## Strategy
Per cluster (bottom-up): contract exists (`understood`) -> write engine C++ from the contract -> validate externally -> mark `ported` then `emu-compared`. After a cluster is ported fresh, diff it against `tesoro-engine`'s implementation and record agreements and divergences in `re/AUDIT.md` (each divergence is a finding resolved with evidence).
Design rule for testability without engine hooks: pure functions and classes that take time and I/O as arguments (no `g_system` inside the primitives).

## Increment 1 — core primitives (smallest reviewable unit, ~300 lines)
Files (new): `engines/tesoro/core/clock.{h,cpp}`, `core/namekey.{h,cpp}`, `core/random.{h,cpp}`, plus the frame-order skeleton in `tesoro.cpp`.
| Piece | Contract | Behaviour to reproduce |
|---|---|---|
| `GameClock` | Clock_Ctor/Now/Freeze/Unfreeze/Suspend/Resume (0x4139c0-0x413ab0) | accumulate `(int)(tick - last)` only when `< 500`; frozen value during the frame; suspend drops inactive time; **runs in every state**, not only gameplay |
| `nameKey64(const char *)` | NameHash64 (0x415c40) | 5 bits per char, 12 chars, `+11` below `'5'` (signed), left-justified padding; collisions are real (case folds) |
| `randomRange(min, max)` and `randomRange7` | RandomRange (0x413b00), RandomRangeAlt (0x413ac0) | permuted tick bits, `(f * (max-min+1)) >> 10` + min; tick supplied by caller |
| frame skeleton | RunFrame (0x414420) | order: input update, clock freeze, sound update, scratch reset, render-queue reset, object pass A, pass B unless A returned 3, unfreeze, end-of-frame test |
Open choice inside this increment: time-driven randomness (exact original) vs `Common::RandomSource`. The original is **not seeded and not reproducible**; recommend the exact function fed with `g_system->getMillis()` so behaviour correlates with time the same way, and keep `RandomSource` out of gameplay.
Validation (external, no engine hooks): extend `re/tools/check_startup.py` to write test vectors (ticks/ranges/names/clock traces from the Unicorn run of the original), and a driver under `local-media/` (ignored) that links the engine's `libtesoro.a` and compares. Clock: replay random tick sequences, compare `Now` after each freeze/unfreeze/suspend/resume op against the original code run in Unicorn with GetTickCount stubbed.
Exit: all vectors equal; contracts updated with `Port:` and `Validation:` lines; ledger -> `ported`, then `emu-compared` for the pure functions.

## Increment 2 — archive and name lookup
`Archive_ReadIndex`, `Archive_FindEntry`, `Archive_SeekEntry` (0x403380, 0x403500, 0x4032e0): parse `Tesoro.TRE` (0x1c-byte records, flag 4, uppercased 19-byte names, offset+4), sort by name as the original does, binary search with the original compare. Needs contracts for `ReadIndex` and `Tell/SeekSavePos` first (currently decompiled).
## Increment 3 — animations
`AnimBank_Load` (0x401160), `AnimBank_LoadImage` (0x4015a0), `AnimPlayer_*` (0x401740, 0x401860, 0x401a50/70), colour conversion and `ScaleBrightness`. Contracts first (86-line `Advance` unread in detail).
## Increment 4 — object model and level loader
`Obj_Kind00` fields and base methods, `Level_Load` (read from the disassembly, decompile is garbled), `Level_CreateObject` factory with the 25 kinds. Subtype classes follow the vtable map in subsets (kind by kind).
## Increment 5 — interaction core
Collision protocol (`Level_CollisionPass`/`ResolveOverlap` plus the geometry routines), cursor candidates, `Jim_*` walking with `WalkArea_*`, verb dispatch.
## Increment 6 — script system
`DlgScript_Load/RunSteps/Update`, label and variable handling, choice menus.
Then rendering (display queue and rasterisers), sound/CD/movies glue, save/load (`LoadState`/`SaveState` per class), special screens (map, board game).

## Per-increment checklist
1. Read the contracts and callee contracts; read unread callees first (their bodies are currently only structure-named).
2. Write C++ in `engines/tesoro/` following the contract; keep addresses out of source comments; comments describe behaviour.
3. Build in the codex worktree (`make -j12`), run the external comparison.
4. Update contract (`Port:` / `Validation:`), ledger status, `re/AUDIT.md` entry against `tesoro-engine`.
5. Leave changes uncommitted; report the diff to Gabriel.

## Known obstacles
- Unread callee bodies: 189 virtual methods are structure-named only; contracts for anything they touch require reading them.
- `Level_Load`, `Display_Compose` etc. decompile badly (large frames); work from disassembly.
- The original reference environment (DOSBox-X Win95 image) is on hold per project rules; until authorised, validation is Unicorn emulation of the original machine code only (no whole-game or timing comparisons).
- Engine stale objects (`*.o`, `libtesoro.a`) in the codex worktree predate the branch state; do a clean build.
