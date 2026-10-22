# Current reverse engineering checkpoint

Updated 21 October 2026. Governing direction: [MASTER_PLAN.md](MASTER_PLAN.md).

## Current target

**M0 source/build baseline is verified.** See [the baseline report](M0_BASELINE.md).
The original-game reference part remains open and on hold until Gabriel explicitly
authorizes its environment and paths. The normal New Game path now reaches the
animated harbour through `FANPRESENT`; it stops at Show Jim, `ANIMS.DLG:92`.
Continue with Jim initialization, scaled rendering and walking, then office
transfer. Current evidence is in [LIVE_OPENING.md](LIVE_OPENING.md).

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
| Active engine | `../scummvm-tesoro`, `tesoro`, HEAD `d98a6b45d3a` plus opening integration. Build passes and normal UI reaches the harbour. Gabriel's menu/save scaffold is preserved; `syncGame` still returns an unsupported-state error. | Jim, office/captain, general scheduler and remaining game systems. |
| Startup evidence | Four runs of the earlier instrumented baseline pass; 12 snapshots match the independent decoder. Reports identify that source/binary. The harness has since been removed from engine code. | Validate subsequent implementation externally; original-runtime comparison remains open. |
| Media | Complete-disc movie and PCM inventories exist; movies are locally present in the game `bin/` directory. | Complete playback, cue/loop/skip/pause and original timing comparison. |
| Introduction | Action 3 starts the original script, movie 2 and offscreen WAV narration, revealing harbour/ship/gull/recruiter. Normal executable observed through line 92. | Show Jim, walking/scale, movie 3 handoff, transfer and captain; full original reference comparison. |
| Animation | Historical core report matches 1,200 isolated scenarios across 17,942 states. Current player feeds the visible harbour loop; character frame preparation clears movement. | Jim and other subtype methods; whole-game clock/scheduling comparison. |
| Scenes/scenery | Historical report covers 50 LVLs/747 records and scenery/queue routines. Current SceneObjects adds drawable kinds 5/6/9 and name lookup; unscaled depth rendering is connected. | General object methods, persistent registry/transfer, actor-follow camera, scaled rendering and shadows. |
| Pixels/text | 28 additional function exports complete. Five harbour stages and all 18 narration texts match isolated original compositor/font code: 7,065,600 pixels. | Same-input sprites do not prove actor logic. No scaled/shadow, Windows display or audible-output comparison. |
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

1. Recover and implement Jim's persistent initialization/banks, floor/depth/scale
   update and scaled composition; unblock type-1 ordinal 4 in the connected
   normal New Game route. See [LIVE_OPENING.md](LIVE_OPENING.md).
2. Implement the original walk to the door, actor-follow camera and transfer;
   then movie 3, office scene context, captain speech/animation and lazy lookup.
3. Complete required scheduler semantics incrementally, preserving native no-ops
   and visible unsupported boundaries. Do not substitute an office teleport.
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
- The current normal executable reaches the animated harbour through actual UI
  actions. [Build/identity evidence](analysis/live-opening-verification.json)
  distinguishes this from external component checks. The current
  [pixel report](analysis/opening-pixel-comparison.json) compares exact unscaled
  composition and narration layout against isolated original instructions.
  Earlier scene/animation reports retain their historical source identities.

## Known approximation ledger

| Location | Approximation / gap | Removal target |
| --- | --- | --- |
| Preserved prototype | Straight walking, temporary harbour scale, limited hotspots and toolbar, unverified talk/shadow timing. | M3 general movement/UI/composition; preserve snapshot as historical evidence. |
| Script VM/tracer, including active subset | Tail-only `run` handling; incomplete command forms and variable context. | M2/M4 scheduler/parser recovery. |
| Active startup | Temporary cursor, fixed censorship sequence, unavailable joke branches; save scaffold advertises capabilities but syncGame errors. | M2 scripts; M3 original UI; M4 persistence. |
| Active adventure | Stops at Show Jim; ordinal 0 omits absent Jim state; only drawable base behavior; no CD music or character speech; unscaled kinds 1/3 only. | Jim, movement/scaling/shadows, transfer and captain before full M2 completion. |
| Historical startup notes | Old results apply to their old source state. | Closed for the current startup baseline by the four matching-build checks; retain historical captures separately. |

Add rows for new approximations with precise implementation location and reason.
Close a row only with replacement evidence. Store future original captures with
edition hash, environment, trigger/input recipe, state and checkpoint time.
