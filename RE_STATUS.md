# Current reverse engineering checkpoint

Updated 21 October 2026. Governing direction: [MASTER_PLAN.md](MASTER_PLAN.md).

## Current target

**M0 source/build baseline is verified.** See [the baseline report](M0_BASELINE.md).
The original-game reference part remains open and on hold until Gabriel explicitly
authorizes its environment and paths. Continue with M1 static recovery needed for
the real-adventure button → `FANPRESENT` harbour introduction → office.

Keep task-file inspection inside the named Tesoro project folders. Do not inspect
personal directories, installed applications or VM configurations.
Keep test classes and instrumentation out of the submitted ScummVM engine.
The startup harness has been removed from engine source and retained only in
ignored local storage; previous startup reports refer to that instrumented build.

## Evidence at this checkpoint

| Area | Established | Remaining |
| --- | --- | --- |
| Original resources | Existing readers account for all 50 LVLs/747 records, 23 observed kinds, 145 script blocks; all 87 DO sites indexed. | Kind/subtype methods, field semantics, reachability and complete behaviour. |
| Executable | 48 general + 11 startup exports report completed decompilation; assembly/xrefs retained. | Full object/vtable map, walking, animation, scheduler, UI, save and special-system code. |
| Prototype | Preserved office pen implementation and documented prior host checks. | General gameplay systems and original comparison; do not treat approximations as recovered rules. |
| Active engine | `../scummvm-tesoro`, `tesoro`, HEAD `1f1b4ddf867` plus current scene/scenery changes. Build passes. Gabriel's menu/save scaffold is preserved; `syncGame` still returns an unsupported-state error. | Add actor, script and renderer integration without replacing the user's changes. |
| Startup evidence | Four runs of the earlier instrumented baseline pass; 12 snapshots match the independent decoder. Reports identify that source/binary. The harness has since been removed from engine code. | Validate subsequent implementation externally; original-runtime comparison remains open. |
| Media | Complete-disc movie and PCM inventories exist; movies are locally present in the game `bin/` directory. | Complete playback, cue/loop/skip/pause and original timing comparison. |
| Introduction | Seven native markers mapped; no-effect ordinals 1, 2 and 6 confirmed. Harbour floor entry is `(213,104)`; Jim is reused across scenes. See [scene findings](SCENE_LIFECYCLE.md). | Actor transfer/movement, native messages and general resumable execution. |
| Animation | Eight native routines exported. Playback/loop/interpolation/pause core matches 1,200 isolated scenarios across 17,942 states. It now feeds scenery objects and their sprite queue. | Actor integration, actual game clock/scheduling and original visual comparison. |
| Scenes/scenery | 33 lifecycle exports. Production reader covers 50 LVLs/747 records. 217 scenery objects match 1,302 native states; 150 integrated queue samples match 519 sprites. | Other object kinds, master registry and transfer, actor-follow camera, gameplay rasterizer and gameplay-loop connection. |
| Original runtime | No verified original-game reference session. | Explicitly authorized environment/paths, working installation, captures and checkpoints. |

The [full structural catalogue](analysis/full-game-catalogue.json) is reproducible:

```sh
python3 tools/catalogue_game.py /Users/gabriel/Desktop/ags/TESORO \
  --output analysis/full-game-catalogue.json
```

It includes lexical counts, not a complete grammar. In particular, one
`menutiem0` source line needs comparison with the original parser. Never silently
correct the game data or assume this spelling has ordinary `menuitem0` semantics.

## Next work queue

1. Scene reading, harbour entry and scenery playback/queue are implemented and
   compared in [SCENE_LIFECYCLE.md](SCENE_LIFECYCLE.md). Recover and implement
   Jim/character initialization and transfer, then the registry and scheduling
   needed by [INTRODUCTION_CONTRACT.md](INTRODUCTION_CONTRACT.md).
2. Port the required resumable script and scene/object behaviour in reviewable
   increments, preserving native no-ops and lazy participant lookup.
3. Connect the real-adventure button to the authentic introduction when these
   dependencies are ready; do not substitute an office teleport or guessed walk.
4. Resume original reference setup only after explicit environment/path approval;
   then capture startup → office and record repeatable inputs and checkpoints.
5. Replace provisional pen mechanics with general walking/UI/object dispatch,
   then compare first/later pickup and both pen-return paths.

## Verification during planning

- Six original-data pen/scene trace tests pass on 21 October 2026.
- A fresh read-only Ghidra startup re-export completed all 11 targets with the
  installed Ghidra/JDK. Redirecting application cache/settings/temp directories
  also resolved the earlier sandbox cache-maintenance error; the tested
  invocation is preserved in the research skill.
- The full catalogue was regenerated from the original TRE/DAT and identity
  hashes; counts are structural evidence, not runtime coverage.
- The source/build part of M0 is now complete: current C++ builds; full, skipped,
  absent and malformed movie scenarios pass against the same source/binary.
  Full details and reproducible commands are in [M0_BASELINE.md](M0_BASELINE.md).
- Introduction jump-table/range-guard bytes and native object strings were read
  directly from the verified EXE. The [generated report](analysis/introduction-contract.json)
  also confirms all 18 speech lines and the sound effect have matching resources.
- Current animation runtime builds without engine test hooks. External native
  comparison matches 1,200 scenarios; eight invalid synthetic scenarios are
  rejected and documented separately. No original scene/visual comparison yet.
- Scene/scenery runtime builds. Independent parsing covers all 50 LVLs, and
  isolated native comparisons cover name keys, room bounds, scenery state/bank
  selection, camera projection and ordered sprite submissions. Exact counts,
  source hashes and remaining gaps are in [SCENE_LIFECYCLE.md](SCENE_LIFECYCLE.md).

## Known approximation ledger

| Location | Approximation / gap | Removal target |
| --- | --- | --- |
| Preserved prototype | Straight walking, temporary harbour scale, limited hotspots and toolbar, unverified talk/shadow timing. | M3 general movement/UI/composition; preserve snapshot as historical evidence. |
| Preserved script VM/tracer | Tail-only `run` handling; incomplete command forms. | M2/M4 scheduler/parser recovery. |
| Active startup | Temporary cursor, fixed censorship sequence, unavailable adventure/joke branches, no actual gameplay saves. Unfinished saves are now disabled. | M2 scripts; M3 original UI; M4 persistence. |
| Historical startup notes | Old results apply to their old source state. | Closed for the current startup baseline by the four matching-build checks; retain historical captures separately. |

Add rows for new approximations with precise implementation location and reason.
Close a row only with replacement evidence. Store future original captures with
edition hash, environment, trigger/input recipe, state and checkpoint time.
