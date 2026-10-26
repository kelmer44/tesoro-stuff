# How far from full support (25 October 2026)

Measured from the game data, not guessed: `tools/coverage_report.py` (reads the 50 scene files and the factory `0x409e50`, exported in
`analysis/factory/`), result in `analysis/coverage-objects.json`. The status table inside the tool is kept by hand; update it when a
class is ported. Branch `codex/tavern` (off `tesoro-engine` `db7eea2744d`). "Ported" means compiled and driven headless with the production code; almost
nothing has been seen in a window or compared with a running original (there is no reference session yet).

## What "scripted" buys

All 145 script blocks compile and the executor implements every operation (2,224 speech lines, 517 messages, 253 conditions, 74 `run`s, 36 menus,
122 sleeps, 139 assignments); menus, sub-conversations, speech and animation waits run. So the **content volume** (2,687 texts, 2,346 voice files, all
conversations, all puzzle dialogue) is already reachable. What the scripts cannot supply is the native side: what each *object class* does when
clicked, the 87 `DO` callbacks, and the systems around them. That is what is left.

## The native side, in numbers

| Layer | Total | Done | Left |
| --- | --- | --- | --- |
| Object records (747) behaving as recovered, defaults included | 747 | 642 (86%) | 105 inert (updated 26 October: ship scenes and the path graph done) |
| Object classes with their own code (kinds 5, 6, 12, 13, 17, 20 and the single-class kinds) | about 100 | about 20 | 81 classes in kinds 20 (29), 17 (17), 13 (13), 5 (11), 6 (8), 12 (3), plus kinds 16, 19, 21, 24, 25 whole |
| `DO` callbacks (87 markers) | 87 | 39 (31 are no-ops by design, the introduction's seven, the captain's exit) | 48 in 17 script types |
| Scenes with no inert object | 47 playable | 11 (credits, final, throne, the kitchen, the ship's decks and storerooms) | the rest have between 1 and 16 |

By kind, what is inert: kind 20
(44 objects) and kind 17 (30): interactive objects with 29 and 17 classes of their own, spread through the island and ship; 11 character classes (talk and items) and 8
door classes; 13 item classes; crowd extras (kind 16) and the kinds 19, 21, 24, 25 (the special games and the fight: 22 objects).

## Systems still missing

1. **Saves.** `syncGame` still reports an unsupported state; nothing is serialized. Every object, script, variable block, held item and scene state has to be (the variable blocks recovered
   this week make that tractable).
2. **Music and transitions.** CD music (19 tracks) only warns; the fade at doors and the scene fades of the `FANMMENU`-type callbacks do not exist; movies other than the two of the introduction are not wired.
3. ~~The path graph and Jim's collisions~~ done on `codex/paths` (26 October): see CLICK_WALKING.md. Still open there: the unported callers that swap the path (`sendmessage 'PATH' play` works through the ordinary message path) and the original-session check of the detours.
4. **The special games and the fight** (Tablero, Lucha, numbered scenes): rules, AI, input, win and lose are unrecovered.
5. **Menus of the game** (the options page on the left of the interface, save/load buttons, kinds 22 and 23), the title flow beyond the shortcuts, speech skipping on the right button.
6. **Original reference.** No original-game session exists, so nothing is compared with real behaviour except isolated routines (scaled sprites, text, shadows, menus, walking).

## Against the master plan

| Milestone | State |
| --- | --- |
| M0 source and reference | Source part done; original reference on hold. |
| M1 formats and inventory of behaviour | Resources, scripts, factory and class table done; per-class methods recovered for about a fifth. |
| M2 runtime and new game to the office | Done in code (introduction, speech, transfer, captain). Not compared; fades missing. |
| M3 interaction and movement | Click walking, hover, verbs, bar, inventory, doors, path graph and detours done; item classes, fades, depth/shadow of Jim open. |
| M4 scripts, dialogue and saves | Scripts and dialogue done; saves not started. |
| M5 finish the adventure by sections | The opening (harbour, office, tavern's conversations) is in range; ship, island, special systems and finale are not. |
| M6 media and closure | Not started. |

## How far, honestly

By volume of recovered work the project is somewhat over **a third** of the way to a playable full game: everything that is *data or script* works, and
the first three rooms (harbour, office, tavern) are covered. By the number of distinct pieces of native behaviour left (about 80 object classes and
48 callbacks plus saves, music and four special systems) it is a **large remainder**, but a regular one: each class and callback is a small routine of the kind
already recovered (a few lines starting scripts or toggling flags), and the same objects repeat across scenes. The hard, irregular parts are the fight and the board games, saves, and the verification against an original that does not exist yet. I give no calendar estimate (the project plan rules them out); the useful
number is that kinds 20 and 17 (74 objects, 46 classes) and the 48 callbacks are the next big lumps, in the order the story reaches them: ship and mutiny first.
