# Virtual tables of the object classes (fresh, 28 Oct 2026)

Slot = offset / 4. All object classes descend from `Obj_Kind00`. Names are `inferred_`. Globals: 004420fc = the player-like actor
(walks to approach points, speaks phrases; built by `004070c0`, hypothesis), 00442128 = pointer/cursor object (kind 10, hypothesis),
00442104 = main object list.

## Obj_Kind00 (vtable 00428000, 13 slots)
| Slot | Function | Role |
|---|---|---|
| 0, 1, 7 | 00401c20 | no-op (slots 0/1 are probably destructor-style hooks; unverified) |
| 2 | 00401c30 | AnimTick: advance current animation if the object has one (flag bit 4) |
| 3 | 00401c50 | UpdateBounds from the current frame record |
| 4, 5 | 00401fb0 | update / secondary pass stubs, return 0 (slot 4 = object-list pass A, 5 = pass B) |
| 6 | 00401d20 | draw(camera) stub |
| 8 | 00401d60 | SetActive(flag) |
| 9 | 00401e20 | LoadState(stream) (save game) |
| 10 | 00401ee0 | SaveState(stream) (save game) |
| 11 | 00401f90 | stub, returns false (1 arg) |
| 12 | 00401fa0 | stub, returns 0 (2 args) |

## Obj_MidBase (vtable 0042ba68, 28 slots) — interactive objects
Slots 0-12 as above except 2/3 = 00407770 (AnimTick + reset offset), 4 = 0040ee70 (Interactive_Update), 6 = 0040efa0 (Draw).
| Slot | Function | Role |
|---|---|---|
| 13 | 0040f0d0 | RunVerb: select verb on the player and dispatch to the verb handler |
| 14 | 0040f020 | SetVerbAndClick (stores verb +0x9c, calls slot 19) |
| 15 | 0040f040 | forwards to slot 14 |
| 16 | 00417b30 | returns true |
| 17 / 18 | 0040f050 / 0040f0b0 | show / clear hover text on the cursor |
| 19 | 0040f1a0 | OnClick: send the player to the object's approach point (+0x94,+0x98) |
| 20 | 0040f210 | verb 0 handler (default: say phrase key +0x70) |
| 21..26 | 00401fb0 | verb handlers 1-6 not overridden (return 0) |
| 27 | 0040f270 | use-item hook, default returns 0 |

Verb index (`+0x9c`, 0..6) maps to handler slot: 0->20, 1->21, 2->23, 3->22, 4->24, 5->25, 6->26. Values 7 and above are inventory items: handler slot 27 is called with (verb - 7).
Phrase key fields per verb: +0x70 (verb 0), +0x74 (1), +0x78 (2), +0x7c (3), +0x80 (4).

## Kind09-family base (vtable 0042bad8, 28 slots)
Same as MidBase, but slots 21 = 0040f280, 22 = 0040f300, 23 = 0040f380, 24 = 0040f400 (default verb responses using the phrase fields above, falling back to default phrases), 27 = 0040f480 (default item use).

## Kind 5 family (base vtable 0042bb48)
Slot 4 = 004101f0 (Kind05_Update), slot 6 = 004102e0 (Kind05_Draw); other slots inherited from 0042bad8 (to be read).

Per-subtype classes (98 subtype vtables, see `re/object_kinds.json`) override verb handlers, update and state methods; they are the next reading target.

## Jim (vtable 00428b70, 33 slots; instance in global 004420fc)
Slots inherited unchanged from the Kind09 family base: 0, 1, 7 (noop), 5, 8, 13-26. Overrides:
| Slot | Function | Role |
|---|---|---|
| 2 | 00407770 | AnimTick + reset offset |
| 3 | 00407730 | UpdateBounds scaled by perspective |
| 4 | 00407790 | per-frame walk step (root motion, 24.8 fixed point) |
| 6 | 00407ac0 | Draw scaled sprite + talk state |
| 9 / 10 | 00407580 / 00407510 | LoadState / SaveState (position, facing, bank, +0x110, flags) |
| 11 | 004079d0 | returns (+0x5c == 7) |
| 12 | 004079e0 | walk segment finished |
| 27 | 004080a0 | forward item use to the interface object |
| 28 | 00410400 | clear speech |
| 29 | 00407a80 | set facing 1..4 |
| 30 | 00408030 | set idle pose for facing |
| 31 | 00407f30 | talk animation (random mouth frame every 100 ms) |
| 32 | 00407600 | WalkTo(x, y, target, flag) |

Facing 1..4: from WalkTo, |dy| < |dx| gives 3 (dx < 0) or 4 (dx >= 0); otherwise 1 (dy >= 0) or 2 (dy < 0). Idle animation index from facing: 3->0, 4->1, 2->2, 1->3.
Perspective: a walk-area object (the kind 1 instance stored at level +0x90, field +0x78) gives a scale (0x100 = 1.0) by (x, y).
