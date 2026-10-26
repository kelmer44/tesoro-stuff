# The ship and the mutiny (26 October 2026)

Engine branch `codex/ship`, off `tesoro-engine` `745414d5480`. Exports and the per-class listing: `analysis/ship/` (112 routines, `by-scene.txt`
joins each object of the eight ship scenes with the code of its class, `natives/` the callbacks, `class-overrides.json` the 69 classes of kinds 6, 12, 17
and 20 with the slots they override). `tools/summarize_override.py` and `tools/compact_override.py` turn an export into readable pseudo-code with the strings resolved.
**Status:** compiled; every object class of the ship scenes and the characters were driven headless with the production objects and scripts
(`analysis/doors/scenarios/scenario6.*`); the new natives and the fade are compiled only. Not run in a window, not compared with the original.

## What the story keeps in Jim

Four bytes of the player object (`+0x110..0x113`) are the story's state, written and read by natives and object classes (now `JimState`):
- `+0x110` **busy**: while it is set (the mutiny phases) the doors of the storeroom and the cabin do not lead through but start `FSALCAMAROTE/ira`; the natives clear and set it.
- `+0x111` **phase**: 0 before the cabin scene, 1 and 2 at the start of the mutiny, 3 after the cabin chest is opened (`BAUL`, needs phase 2), 4 and 5 around the storeroom chest (`COFRE`:
  phase 4 -> 5 on taking), 6 for the papers, 8 for the rug; the object classes only test it and the natives and scripts' callbacks move it.
- `+0x112`, `+0x113`: a flag used by the speech end routine and one that makes a door play a movie once in `RISCO` (not ported).

## Doors, fades, transitions

The generic door use fades the screen out in 4 steps of 100 ms (each step multiplies each RGB565 channel by `256 - 64k` over 256: 192, 128, 64, 0; `0x4123e0`),
waits 300 ms, then puts Jim in the other scene and runs the **arrival hook** of the door he comes out of (slot `0x70`). The engine now does the same: a frozen frame is
darkened for `steps * delay + pause` ms, the pointer and scripts wait. The native callbacks that fade use the same routine (`natives.cpp`); with the
fade the script's scene and participants are set when it ends.
- Door classes: 3 (cabin) and 9 (storeroom) start `FSALCAMAROTE/ira` while `busy` (with the map item as a variable for class 3); 4 (storeroom's second door) starts `FANPATATAS/main` in the kitchen the first time, with the kitchen Dick's variables;
  class 3's arrival hook (the first time Jim comes in, phase 0): sets the cabin's floor and path frames, takes the state from the plate item (`IPLATO` variable 0 = 2 -> phase 1 and the door's variable 6; plate owned -> variable at least 4;
  otherwise a random 0-3 and other frames) and starts `FANCAMAROTE/main`. The call `Jim.slot0x10()` in the hook was not understood.
- Natives ported: type 3 (embarkation: ordinals 1 and 2 place Jim on the deck), type 2 (cabin: 0-5 use the cabin door, 6 the captain's depth, 7 the mutiny starts with a fade to the deck and the kitchen emptied, 8 Jim shown, 9 back to the cabin with `busy` and phase 2),
  type 4 (to the storeroom or the cabin with `busy`), type 5 (arrival on the beach at the floor's origin; ordinal 1 does nothing). Types 6-20 are exported (`analysis/ship/natives/`) and follow the same shapes; not ported.

## Object classes (kinds 17 and 20)

All 17 classes of kind 17 and six of kind 20 are ported (`ScriptedObject::runKind17/20`): they test items and the phase, set the object's own variables, activate items
(powder, potion, rope) and start scripts `FLIBRO`, `FLIBROS` (a random 0-4 line), `FBANDERA`, `FALFOMBRA`, `FMESA`, `FBAULOPEN`, `FVENTOPEN`, `FCIRUELAS`, `FCOFRE`, `FCUERDA`, `FPATATAS`, `FPUCHERO`, `FCUBO`, `FPAPELES`, `FBAUL`,
`FESPADA`, `FCAPIMUERTO`, `FNOTARIP`, `FSUICIDA`; items: pot + fish, chest + key, rope + sword. Characters: the fisherman, the gambler, the kitchen's Dick and parrot (talk, look and items).
The full table is `analysis/ship/by-scene.txt`; `tools/coverage_report.py` now counts the eight ship scenes as complete except for their path object.

## Gaps

1. **PATH (kind 8)** is inert in every scene, so walking there is straight lines; the cutscenes of the ship rely on `ira` (walk) messages and path swaps (`sendmessage 'PATH' play`), which only change an animation frame here.
2. The natives were not driven (they live in the engine class); the object classes, scripts and door logic were. The fade's arithmetic is quantised through RGB565 and not compared with the original.
3. `Jim.slot0x10()` in the cabin hook, the cabin hook's use of the floor's path link (found by the name `PATH`), the type 2 ordinal 6 depth and the exact timing of the fade relative to the cutscene scripts are unverified.
4. Only the natives of types 2 to 5 are ported; the mutiny's later natives (`FANMLGISLA` ord 1, `FEXPLOSION`, `FSALPANTA`, the Risco ones) and the CD music cues are not.
5. Item classes and the remaining kind 20 classes (island and caves) are not recovered.
