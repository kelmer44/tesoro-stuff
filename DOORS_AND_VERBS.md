# Doors, per-object verbs and variable blocks (25 October 2026)

Engine branch `codex/doors`, off `tesoro-engine` `491457e17eb`. Evidence and exports: `analysis/doors/` (decompilation of every
routine named below). **Status:** compiled, and the production scene, object and script code was driven headless with a fake game
around it (`analysis/doors/scenarios/`). **Not run in a window and not compared with the original game.**

## What was recovered

**Variable blocks.** Every object owns a block of N bytes (`+0x60`, size in `+0x64`), allocated by its constructor
(`0x401aa0`, the last argument of the class constructors). Scripts started by an object take a block as their variables:
`tesoro_start_script(script, label, block, scene)` (`0x405fd0`). So `v0`, `v1`... in a script are bytes of the block of
the object that started it, not of one global array (this answers the open "variable ownership" question for object-started scripts;
the introduction's `_variables` block is still the engine's own). The captain's block (4 bytes) is shared by the scripts of the
ballpoint, the letter and the captain's talk. A block of 0 means the script gets no variables.

| Class | Sizes | Where |
| --- | --- | --- |
| Characters, by byte 5 of the record | 4 14 4 0 10 3 6 4 3 0 1 1 (class 12: 0) 15 2 11 7 | factory `0x409e50`, case 5 |
| Doors (kind 6), by byte 1 | 0 1 1 2 1 1 0 1 1 0 1 | case 6 |
| Objects (kind 12), by byte 1 | 1 0 0 1 1 1 0 0 1 0 0 0 | case 0xc |

**Doors** (base `0x414be0`, vtable `0x42bce8`). Record bytes `0x67` = destination scene (index in the scene table, the same as
the scripts' scene numbers), `0x68..0x73` = name of the door it leads to, `0x74` + 1 = the direction Jim faces when he arrives.
The default verb (`0x414c40`) works only when the door was last clicked with verb 0: Jim turns to the door, the screen fades out
(`0x4123e0`, 4 steps, 100) and 300 ms pass, Jim is put in the destination scene at the approach point of the destination door facing
the arrival direction, and the destination door's slot `0x70` runs (`0x414dc0`: plays an external movie in one scene only). A right click
(`0x414d50`) puts Jim at the door's approach point at once and uses the door, if Jim is in a scene with index below 23 and the pointer shows
verb 0.
- Class 2 (`0x418560`, the harbour's tavern door): after the first use, runs `FANTABERNA/main` in the tavern's scene.
- Class 8 (`0x42aef8`, the office door): its block byte is a lock. The default verb does nothing while it is set; verb 4 (open) toggles it and shows
  the door's picture when it is set; the right click works only while it is set (the opposite of the default verb).
- Other classes (1, 3, 4, 5, 6, 7, 9, 10) override only the default verb (`0x418b80`, `0x415f90`, `0x40d1a0`, `0x418050`, `0x417dd0`,
  `0x40daf0`, `0x415d20`) or the arrival hook (`0x416030`, `0x417640`, `0x401fb0`); not recovered.

**Verb overrides of the office (kind 12, by byte 1).** Slots as in CLICK_WALKING.md (verb 0 `0x50`, 1 `0x54`, 2 `0x5c`, 3 `0x58`, 4 `0x60`,
6 `0x68`). The captain "looks away" when his current bank is 2 (`ESPIZQCAPI`, third bank), set by `FCAPITAN` (`play 2 0 wait`).

| Class | Object | Verb | What it does |
| --- | --- | --- | --- |
| 0 `0x42ab40` | TABURETE (stool) | 3, 6 | own byte = whether the vase is active; `FTABURETE` `coger` / `mover` |
| 1 | PEZ | 1 | `FPEZESP/mirar`, no variables |
| 2 | CARTA | 1 | `FCARTA/mirar` with the captain's block |
| 3 | CAJONES | 4 | captain away: `FCAJON/abrir` with its own block; else `FNOCOGER/coger` |
| 4 | PLACA | 1 | `FPLACA/mirar` with its own block |
| 5 | BOLI (ballpoint) | 1, 3, 4 | `FBOLI` `mirar`, `coger` (captain's block), `abrir` |
| 6 | JARRON (vase) | 3 | captain away: item `IJARRON` activated and the vase deactivated; else `FNOCOGER/coger` **and the vase deactivated** |
| 10 | PLUMA (pen) | 3 | the same with `IPLUMA` |
| 11 | PUERTA2 | 0 | `FPUERTARARA/ir1` if the captain is away, else `ir2` |

Characters: talk (verb 2) of the captain (class 0, `0x416a20`: copies the tavern's `NUDERO` byte 4 and the harbour's `RECLUTADOR`
byte 3 into its own bytes 1 and 2, then `FCAPITAN/hablar`) and of the recruiter (class 1, `0x4185c0`: moves its state to 4 once
the captain's byte 0 is set and to 6 once `IHALFDOBLON` is active, then `FRECLUTADOR/hablar`). Their item verbs (`0x416ac0`, `0x418670`:
item 0 -> `FIBOLI`, 7 -> `FICNUDO`, 45 -> `FIHLFDBLN`) wait for the inventory.

**Native type 21, ordinal 0** (`0x416b50`, `FCAPITAN/salir`): runs the office door's default verb (`PUERTA1`, slot `0x50`), so the captain's exit
sends Jim through the office door; it depends on the verb the door was last clicked with.

## What was ported

`objects.h/.cpp` (`DoorObject`, `ScriptedObject`), the characters' talk in `scene.cpp`, per-object variable blocks and a scene pointer in
`SceneObject`, `PointerHost` additions (`levelScene`, `interfaceScene`, `startObjectScript`, `transferJim`, `jimLevel`),
the inventory scope of scripts resolving to the interface scene, and the type 21 native. Items such as `IPLUMA` are just the
interface scene's objects being activated; nothing draws them yet.

## Verification

`analysis/doors/scenarios/doors_driver.cpp` (copy of the ignored `local-media/jim-implementation/doors_driver.cpp`) links the
production scenes, objects, jim and script executor and fakes the rest: a click arrives at once, speech lasts 200 ms, walks end at
once, menus choose the first choice. Transcripts (`scenario*.txt` and `.log`):
1. Office: pen and vase refuse while the captain looks at Jim; with his back turned (bank 2) the item is activated; letter ->
   whole `FBOLI/primera` sequence -> `FCAPITAN/salir` -> native 21/0 -> Jim goes through the office door; stool, plaque, fish,
   drawers, the office's second door and default verbs behave as in the table.
2. Doors: harbour <-> office, the lock toggle, the right click in both lock states, the tavern door and its one-time scene.
3. Recruiter conversation chain through `FRECLUTADOR`, `CNVRECLUTA`, `CONV5`, ... (stops at the menus, see below).

## Gaps and approximations (ledger)

1. **No fade.** The door transfer in the original fades out, waits 300 ms and fades in; the engine cuts. (No fade routine is ported.)
2. **Dialogue menus** are not implemented in the game (`pickDialogueChoice` stops with an error); every conversation stops at its first menu.
3. **Item verbs** (cursor frames 5 and up), the inventory display and the held item are not ported; the verb 6 `mover` of the stool is reachable only
   through them.
4. **Captain's bank 2** is reached only through his script; whether anything else changes the captain's bank was not checked, and
   the testing shortcut that starts in the office deactivates him.
5. **Pen deactivation.** With the captain not looking away, the pen and the vase are deactivated after `FNOCOGER` (read from the
   assembly: `PUSH 0; CALL [obj+0x20]` after the script start). It looks odd and has no original confirmation.
6. Unrecovered door and object classes use the default behaviour with a one-time warning; the door's `+0x110` flag (Jim busy) is assumed clear.
7. `FANTABERNA` runs with the tavern as its scene while Jim is still being moved there; the order relative to the original's fade was not compared.
8. **Black harbour in the office shortcut (fixed 25 October).** The testing shortcut `kStartInOffice` skipped the introduction without
   its effect on the harbour (`FONDONEGRO` off, `GAVIOTA` and `RECLUTADOR` on and looping, `BARCO` shown and looping), so leaving the
   office showed the black backdrop. The shortcut now applies those states. A normal new game runs the script and was not affected.
9. **Correction (INVENTORY.md):** slot `0x3c` is the left **double click**, not the right click: the door shortcut works on a double click.
