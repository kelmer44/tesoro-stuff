# Scene entry, ownership and scenery playback

21 October 2026. This increment recovers scene initialization and implements the
scene reader, default floor-origin entry, immediate camera placement, scenery
ownership and scenery sprite submissions. This document records the earlier
scenery increment and its source-specific checks. The adventure button now
reaches the harbour through the connected runtime described in
[LIVE_OPENING.md](LIVE_OPENING.md). No Windows reference session was started.

## Evidence and scene initialization

Addresses apply to EXE SHA-256
`0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef`.
[RecoverSceneLifecycle.java](ghidra/RecoverSceneLifecycle.java) exported 33
functions with C, assembly and xrefs; all completed. See
[status](analysis/scene-lifecycle/decompile-status.tsv). Names are research labels.
An initially guessed draw label on `0x401e20` was corrected: its body reads saved
object state. `0x40e6b0` is scenery draw submission, not message dispatch.

1. `0x4080e0` reads the LVL header and banks, then the 40-byte groups and 32-byte
   object records plus variable parameters and bank references. Header offsets
   `+4/+8/+12` contain group/bank/object counts. All 50 LVLs agree with the total
   object count. The factory at `0x409e50` selects actual classes by kind/subtype.
2. `0x410420` builds group state. Disk group `+32` is object count, `+34` is its
   depth span and `+36` is the camera projection divisor. The next group's depth
   base advances by the current span. Object depth is its signed record depth
   plus group base plus five, stored in 16 bits. Other group bytes remain raw.
3. Scene object references are appended by `0x40f800`. This is a generic list
   append, not a constructor or parent assignment. `0x408540` calls `0x40f710`
   to bind the completed collection. `0x401bf0` assigns the parent scene, inserts
   objects carrying flag `0x100` into their group, then calls virtual `+0`.
   Scenery's `+0` method, `0x4157b0`, inserts it into the group separately.
4. Group insertion `0x4104a0` orders by descending base X, preserving insertion
   order for equal X. Removal is `0x410530`. Scenery's virtual `+8` advances its
   selected bank; virtual `+0xc` prepares frame/hit information; `+0x18` submits
   a sprite; `+0x10` is a no-op logic update. These are distinct phases.
5. Room kind 1 uses `0x406980` and virtual initializer `0x4069e0`. The initializer
   resolves the twelve-byte floor name, derives camera bounds from frame zero's
   hit rectangle plus the room record's position, then positions the camera.
6. New game `0x414560` passes the room's resolved floor coordinates to Jim's
   placement method `0x407b80`. It supplies the first scene group, activates and
   shows Jim, then starts `FANPRESENT/main`. The first native marker hides him.

## Recovered opening positions

Bounds below use positive world Y upwards: left, top, right, bottom.

| Scene | Floor origin | Camera center after placement | Bounds | Room music cue |
| --- | --- | --- | --- | --- |
| Puerto | `(213,104)` | `(320,239)` | `(0,479,657,-1)` | 3 |
| Despacho | `(387,97)` | `(362,240)` | `(42,480,682,0)` | 2 |

The office floor origin is not the scripted introduction destination. Native
ordinal 5 explicitly places Jim at `(550,100)` there. Room exits can supply their
own coordinates; the floor origin is not a universal entry location for every
transition. The cue values are room fields, not a verified CD extraction index.

`0x407b80` reuses the existing Jim object. It removes him from the old scene and
group, updates parent/group references, appends him to the new scene, inserts him
into its group and reattaches the camera. It makes per-bank residency calls,
sets both committed and pending position, updates the scene view, then calls his
logic update. The port has not yet implemented this actor transfer or the full
effects of those residency calls. It must not create a second Jim per room.

## Implemented scenery path

The active ScummVM checkout now contains:

- `resources.h/.cpp`: `SceneData`, groups and object records, all banks, retained
  parameters/references/unknown bytes, checked record boundaries, and original
  64-bit name encoding. It parses every object without assigning generic scenery
  behaviour to unsupported kinds. Bounds checks also avoid addition overflow.
- `scene.h/.cpp`: `findRoomEntry`, `SceneCamera`, `SceneryObject` and
  `SceneScenery`. Each scenery object owns one player per referenced bank, while
  frame/image resources remain shared and alive through shared ownership.
- `SceneScenery::appendSprites`: group traversal, descending-X insertion order,
  visibility/activity gating, frame preparation and sprite queue output. The
  caller still has to provide the game clock and render the queued images.

Scenery constructor `0x40e5c0` uses parameter byte 0 as the starting frame and
byte 1 as its mode: 0 hides a fixed frame, 1 shows a fixed frame when images are
present, 2 plays once and 3 loops. Fixed modes clear that frame's movement in
the shared frame data, matching the original shallow bank clones. BARCO starts
hidden in mode 0. Automatic ship reveal/play still needs native/script dispatch.

Immediate camera clamping follows `0x402120`, including clamp order for bounds
smaller than the viewport. For each group the view coordinates are camera center
multiplied by 300 and divided by its projection divisor, with signed division.
The actor-follow and timed movement portions of `0x401fc0` are in the camera class (see
the camera follow section of [JIM_AND_WALKING.md](JIM_AND_WALKING.md)).

Scenery draw submission `0x40e6b0` produces:

```text
screenX = objectX - groupViewX + movementX + 320 + frameX
screenY = groupViewY - objectY - frameY - movementY + 240
depth   = objectDepth + groupDepthBase + 5   (low 16 bits)
```

The sprite retains its image and depth for the renderer. This increment does not
replace the original per-pixel depth compositor with painter sorting, nor claim
that shadows, clipping, pixels or actor scaling have been matched.

## Validation and limits

The production C++ code builds. An ignored standalone driver links the actual
`resources.cpp`, `animation.cpp` and `scene.cpp`. The
[external comparison](tools/compare_scene_native.py) feeds all original LVLs to
that reader and compares structural results with the independent Python reader.
It also executes the original object, animation, camera, name-encoding and group
insertion routines in Unicorn. The clock and allocator are supplied; floor lookup
is supplied from independently decoded records; sprite submission is captured.

[The report](analysis/scene-native-comparison.json) records exact source, driver,
tool, TRE/DAT and EXE hashes. Matched results:

| Check | Coverage |
| --- | ---: |
| Parsed LVLs / embedded banks / object records | 50 / 360 / 747 |
| Native name keys | 747 |
| Native room initialization and immediate camera bounds | 38 room/floor layouts |
| Scenery construction and frame states | 217 objects / 1,302 states |
| Object bank selections | 495 |
| Immediate camera/group projection cases | 260 |
| Integrated scenery queue samples / submitted sprites | 150 / 519 |

Queue comparison preserves simultaneously live objects and separate bank clones;
the native group-binding routine establishes the expected ordering. Selected
bank/image identity, coordinates, depth and submission order match. Hidden or
inactive scenery is omitted from the draw traversal. The driver also checks
activation/show operations separately, so the two flags remain distinct.

These historical checks do not execute a complete original scene, actor
collision/update schedule, room transition, rendering backend or Windows session.
The current collection is named `SceneObjects`/`SceneObject` and also includes
drawable base state for kinds 5, 6 and 9. The old driver must be adapted before
rerunning against that API. Current build, native pixel checks and visible
harbour evidence are in [LIVE_OPENING.md](LIVE_OPENING.md). Jim lifecycle and the
rest of the introduction remain next; reference-run comparison remains open.
