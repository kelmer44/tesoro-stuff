# Current reverse engineering checkpoint

Updated 21 October 2026. Governing direction: [MASTER_PLAN.md](MASTER_PLAN.md).

## Current target

**M0: establish a reproducible current engine build and a working original-game
reference.** Next implementation target: real adventure button → `FANPRESENT`
harbour introduction → office. Reference runtime setup is required work: Gabriel
confirmed that no working original session exists yet.

## Evidence at this checkpoint

| Area | Established | Remaining |
| --- | --- | --- |
| Original resources | Existing readers account for all 50 LVLs/747 records, 23 observed kinds, 145 script blocks; all 87 DO sites indexed. | Kind/subtype methods, field semantics, reachability and complete behaviour. |
| Executable | 48 general + 11 startup exports report completed decompilation; assembly/xrefs retained. | Full object/vtable map, walking, animation, scheduler, UI, save and special-system code. |
| Prototype | Preserved office pen implementation and documented prior host checks. | General gameplay systems and original comparison; do not treat approximations as recovered rules. |
| Active engine | `../scummvm-tesoro`, `tesoro`, inspected HEAD `33316cacbf8`; local edits to `tesoro.cpp/.h` changed during inspection. The latest draw conditional is corrected. | Reconcile source/build/harness. Dummy saves are advertised; current loop lacks the earlier documented pacing. |
| Startup evidence | Historical logs/snapshots demonstrate earlier startup work. | The current source lacks the harness hook used by `tests/test_startup.py`; old pass logs are not current-build verification. |
| Media | Complete-disc movie and PCM inventories exist; movies are locally present in the game `bin/` directory. | Complete playback, cue/loop/skip/pause and original timing comparison. |
| Original runtime | Wine, 86Box, UTM, OBS and ImHex installed; Ghidra/JDK present. | Working game installation, captures, checkpoints and debugger selection. |

The [full structural catalogue](analysis/full-game-catalogue.json) is reproducible:

```sh
python3 tools/catalogue_game.py /Users/gabriel/Desktop/ags/TESORO \
  --output analysis/full-game-catalogue.json
```

It includes lexical counts, not a complete grammar. In particular, one
`menutiem0` source line needs comparison with the original parser. Never silently
correct the game data or assume this spelling has ordinary `menuitem0` semantics.

## Next work queue

1. Inspect the active engine's local diff with Gabriel's current coding work in
   mind; retain the intended edits and establish a buildable baseline. Resolve
   real save feature declarations and movie ownership/pause/pacing while porting
   the useful startup harness to the current architecture.
2. Launch a separate installed copy in an isolated Wine prefix; document results.
   If unsuitable, inventory available guest setups and use 86Box/UTM with the
   needed Windows media. Do not change the original evidence copy.
3. Capture startup → real adventure → office; record repeatable inputs, original
   saves/checkpoints and audio/video limitations of the reference environment.
4. Recover scene-object/ANM fields and scheduler dependencies used by
   `FANPRESENT/main`; implement its new-game path in small C++ increments.
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
- Current C++ was inspected, not modified or built during planning. Local edits
  changed during inspection; current source/harness differences prevent adopting
  earlier startup passes. Recheck current source before starting M0 work.

## Known approximation ledger

| Location | Approximation / gap | Removal target |
| --- | --- | --- |
| Preserved prototype | Straight walking, temporary harbour scale, limited hotspots and toolbar, unverified talk/shadow timing. | M3 general movement/UI/composition; preserve snapshot as historical evidence. |
| Preserved script VM/tracer | Tail-only `run` handling; incomplete command forms. | M2/M4 scheduler/parser recovery. |
| Active startup | Temporary cursor, fixed censorship sequence, unavailable adventure/joke branches, no actual gameplay saves. | M0/M2 baseline and scripts; M3 original UI; M4 persistence. |
| Historical startup notes | Source state differs from current branch and local changes. | M0 matching build and current evidence; retain old captures as history. |

Add rows for new approximations with precise implementation location and reason.
Close a row only with replacement evidence. Store future original captures with
edition hash, environment, trigger/input recipe, state and checkpoint time.
