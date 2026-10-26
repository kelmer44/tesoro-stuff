# Click walking (recovery in progress, 24 October 2026)

Exports: `analysis/click-walk/` (made with `ghidra/RecoverAddresses.java`, which also lists
virtual-call sites by vtable offset, instructions by field offset or text, and functions in a
range; invocation as in `skills/tesoro-re/references/headless-ghidra.md`). Addresses apply to
the verified `TESORO.EXE` only. Status words: **recovered** = read from code, **hypothesis** = inferred.

## Mouse

The input object (`DAT_0044206c`) is clamped by the pointer class constructor (`0x402300`) to
x in [-320, 319] and y in [-240, 239] (`+0x20..+0x2c`): **recovered** that its position
(`+0x38`, `+0x3c`) is relative to the screen centre, y growing downwards. Button byte 0 bit 3
(`& 8`) is the left button, bit 4 the right one.

## The click (`0x4030e0`, slot `0x14` of the pointer class)

Per frame the pointer runs the hit test over the interface objects (`0x402e80`: for each hit
object, left click calls its slot `0x38`, right click slot `0x3c`, hover slot `0x44`; a result
of 3 ends the frame). **Recovered:** if nothing consumed the click (result 0), the left
button is down and the pointer's own frame is 0 (the default arrow), then

1. if Jim is speaking (`+0xd4`), his slot `0x70` is called (ends the speech, `0x410400`);
2. Jim's slot `0x80` (start walk, `0x407600`) is called with
   `(mouseX + cameraX, cameraY - mouseY, waiter 0, leg flag 0)`, using the camera centre of
   Jim's scene.

Start walk stores the leg flag at Jim `+0xf0` and the waiter at `+0xf4`.

## Object approach (`0x40f1a0`, slot `0x4c` of interactive objects)

Walks Jim to the object's approach point (`+0x94`, `+0x98`) with the object as waiter and
leg flag 0; when he arrives the waiter's slot `0x34` (`0x40f0d0`) turns him to the object's
direction (`+0x90`) and runs the pending verb (`+0x9c`) through slots `0x50..0x68` (or
`0x6c` for inventory verbs). Left click on an object stores the verb and calls this.

## Floor routing (`0x414f10`, `0x414e80`, `0x4079e0`)

The floor (kind 7, class constructed at `0x414e00`, vtable `0x42bd60`) has a list of nodes in
its current frame (count and (x, y) pairs relative to the floor object, as for the scale).
**Recovered:**

- `0x414f10(floor, x, y, waiter)`: finds the node nearest to the click, but only a node
  strictly nearer to it than Jim is (squared distance); with none it returns false. Otherwise
  it stores the click point (`+0xa8`, `+0xac`), the waiter (`+0xb0`), the destination node
  (`+0xa4`) and the node nearest Jim (`+0xa0`), and starts Jim walking to that node with the
  floor as waiter and leg flag 1.
- `0x414e80` (the floor's slot `0x34`, run when a leg ends): if Jim's node equals the
  destination, walk to the click point (waiter `+0xb0`, leg flag 1); otherwise step the node
  index one towards the destination (up or down by index, so nodes are a chain) and walk to
  that node.
- Jim's step (`0x407790`) on arrival with a waiter calls the waiter's slot `0x34`; with none
  he turns himself.
- Jim's slot `0x30` (`0x4079e0`) when he is walking: pending = committed position, leave
  the update list; with leg flag 0 and a floor, call `0x414f10(floor, target, waiter)`; if
  that finds no route, turn (`0x407a80`, direction `+0x10c`) and resume the waiter; with leg
  flag 1 just clear the flag and turn.

## What calls Jim's slot `0x30`: collisions, not clicks (recovered 24 October 2026)

The scene's end-of-frame queue (scene `+0x78`, drained by `0x408890`) holds records of
(object a, object b, overlap, priority). They are pushed by the ordering routine `0x408c50`
(`tesoro_order_objects`, run for pairs of drawn objects whose flags have bit 3), kept sorted by
overlap, and each pair calls `a.slot 0x30(b, overlap)`; Jim's `0x4079e0` therefore receives the
*other* object (that is why it reads `+0x78` of its argument: a path object the other object
carries, with `RET 8`). So the `0x414f10` routing, the floor chain walk and the leg flag are the
**detour around an obstacle that Jim runs into while walking**, not the way a click walks.

Consequence: a click on empty ground starts a straight walk (`0x407600`) toward the clicked
point and nothing re-plans it unless he collides with an object that carries a path. Straight
click walking is therefore the original behaviour for open ground; detours need the collision
pair test (`0x408c50`, `0x409280` and friends, not recovered/ported).

## Engine status (24 October 2026)

`TesoroEngine::clickWalk` (branch `codex/camera-follow`): while input is enabled, a left click
sends Jim straight to `(mouseX - 320 + cameraX, cameraY - (mouseY - 240))`, the point clamped to
the screen first, with no waiter. **Implemented from the recovered handler; not compared with
the running original** (the walk itself is, see JIM_AND_WALKING.md). Not done: hit testing of
objects and the interface before the ground (`0x402e80`), the cursor's default-frame condition
(the engine has one cursor), ending Jim's speech on a click while input is enabled, obstacle
detours, and the object approach walk.

## Hotspot picking is the collision machinery (recovered 24 October 2026)

Exports in `analysis/click-walk/` (`hit/`, `overlap/`, `probe/`, `queue/`, `cursor/`, `floor2/`).

**There is no separate hit test for hotspots.** The pointer class (vtable `0x428038`, constructors
`0x402300` for the interface pointer and `0x402240` for its child) owns a *probe*: a second
instance of the same class at `+0x74`, whose position is set to the mouse's world point every
frame by `0x4029f0` (`mouse + camera`, then `0x401d30(probe, x, y, 1)`). Setting a position with
flag 1 puts the object on the scene's moved list (`0x408440`, only for objects whose flag bit 3 is
set; the probe sets it in its constructor). At the end of the frame the scene (`0x408890`) tests
every pair of such objects with the ordering routine `0x408c50`, queues the overlapping pairs
sorted by overlap (a record holds both objects, an overlap value in 16.16 and a priority) and
then calls `a.slot 0x30(b, overlap)` for each. For the pointer class, slot `0x30` (`0x402e60`)
simply adds the other object to the pointer's **candidate list** (`+0x70`); slot `0x2c`
(`0x402e50`) answers whether the object is "in front" (flag bit 10 of `+0x68`). Jim's slot `0x30`
(`0x4079e0`) is the walking detour from the previous section; most objects use the no-op `0x401fa0`.

Next frame the pointer update (`0x4030e0`, which runs `0x402e80`) walks the candidate list:
an object that was not hovered before is added to the hovered list (`+0x6c`) after its slot
`0x40` (hit test; the common one `0x417b30` answers yes) and gets slot `0x44` (hover); an object
already hovered gets slot `0x38` on a left click (argument: the pointer's current frame),
`0x3c` on a right click (which calls `0x38`), else `0x44`. A result of 3 ends the frame's input.
Interactive objects: hover (`0x40f050`) shows the object's name through the pointer
(`0x4023f0` stores the text at pointer `+0x78`), left click (`0x40f020`) stores the verb at
`+0x9c` and calls slot `0x4c` (`0x40f1a0`: walk Jim to `+0x94/+0x98` with the object as waiter),
and arrival (`0x40f0d0`) turns Jim to `+0x90` and runs the verb through slots `0x50..0x68`.

**The overlap routines** (all pure): `0x408c50(a, b)` first sweeps the boxes (`0x409280`:
returns -1 for no contact, 0 for already overlapping, else the fraction of the relative
movement at which they touch, 16.16), then refines with the frames' polygons (`0x409560` point in
polygon, `0x409790` polygon against polygon, `0x409c10` clip) when both frames have more than one
point; objects with a single point are treated as points. It pushes (a, b, overlap, max(depth
of a, depth of b), or 100 if either is Jim). Object fields used: box `+0x10..+0x1c`, previous box
`+0x20..+0x2c`, bank `+0x38` (frame polygon at frames `+0x1c`, count and point pointer), position
`+0x4c/+0x50`, depth `+0x5e`.

### Plan (nothing ported yet)

1. Port `0x409280`, `0x409560`, `0x409790`, `0x409c10` and `0x408c50` as a pure geometry module and
   compare them against the original in the emulator on real frames' polygons and random boxes.
2. Add the moved list, the pair queue, the end-of-frame dispatch and the pointer probe/candidate
   list to `Scene` (also gives depth ordering between moving objects and the walking detour).
3. Interactive object classes (kinds 17 and 20 and the others that draw only now): hover text,
   left/right click, approach walk, verb dispatch and the pointer's frames; then the UI.

## Overlap geometry ported (25 October 2026)

Engine (branch `codex/hotspots`, uncommitted): `engines/tesoro/overlap.h/.cpp`.
`sweptBoxOverlap` (`0x409280`), `pointInPolygon` (`0x409560`), `polygonsOverlap` (`0x409790`),
`clipPolygon` (`0x409c10`) and `testOverlap` (`0x408c50` up to the queue push, returning the
overlap and priority the scene would queue). Polygons are the point lists of type 2 (hit
outline) and type 4 (floor) frames; the loader's split index (the last point of the first run that
does not go back in x, `0x401160`) is computed by `OverlapPolygon::fromPoints`.

**Comparison** (`tools/compare_overlap.py`, driver `local-media/jim-implementation/overlap_driver.cpp`):
the original routines run in the emulator on the same inputs, over every type 2/4 frame polygon of
the game (104 usable, translated and clipped at random) and random outlines. The pair test runs with
a stand-in for the scene's queue and reads the queued record back. Results: see
`analysis/overlap-comparison-seed*.json` (per-kind counts, how many results were positive, and the
skipped cases). Breaking the sweep or the priority makes it fail.

**Known differences and limits**

- The original reads past the end of a point list on some inputs (for instance when one edge of the
  outline spans both clip lines; the clip then copies whatever memory follows). That result depends on
  neighbouring memory, so it cannot be reproduced; the tool re-runs the original with three different
  fills around the points and skips cases whose answer changes (counted in the report). The port reads
  zeros there and its loops stop past the end of the points. Whether real game outlines reach this is
  not yet known.
- The original traps on a zero divisor and on loops that do not end; those cases are skipped and the
  port returns 0 instead of crashing.
- The queue push, the moved list, the end-of-frame dispatch, the probe and the candidate list are not
  ported (next step of the plan above). Frame polygons are not yet attached to `SceneObject`.
- Overlap compared on synthetic bodies: positions, previous positions and boxes are random, not taken
  from a running scene; flag bits (`+0x68`) that decide which objects are tested are not ported.

## Correction and the end-of-frame rules (25 October 2026)

**`0x402e50` and `0x4079d0` take the other object as a stack argument.** Ghidra shows them as
`uint f(int param_1)` (no `this`), so `param_1` is the argument of the call `a.slot0x2c(b)`, i.e.
*the other object*. Therefore:

- The pointer class's "in front" test (`0x402e50`) is true when **the other object has flag bit 10
  (`0x400`)**, not the pointer itself. Every interactive object (constructor `0x40ed10`) sets
  `0x400`, and nothing sets it on the pointer or the probe. So the pair test runs as
  `order(probe, hotspot)` and the probe's slot `0x30` (`0x402e60`) adds the hotspot to the
  pointer's candidate list. (The earlier paragraph above that says the pointer's own bit 10 is
  tested is wrong.)
- Jim's `0x4079d0` is true when the other object's byte at `+0x5c` is 7 (kind 7, the objects that
  carry the path), which is why only those objects send Jim round a detour.
- All 98 factory-built classes (interactive objects, doors, characters, ...) have the default
  `0x2c` (`0x401f90`, false) and `0x30` (`0x401fa0`, nothing): the only classes with their own are
  the pointer class (`0x428038`) and Jim (`0x428b70`).

**Object flags (`+0x68`)** seen so far: bit 0 (1) "prepared this frame" (scratch), bit 1 (2)
active, bit 2 (4) shown (set from "has an image" for interactive objects and scenery), bit 3 (8)
**collidable** (only these join the moved list and pair tests; the probe, the pointer, Jim's kind
and interactive objects set it), bit 4 (0x10) has its own animation banks, bit 6 (0x40) set by
interactive objects, bit 8 (0x100) grouped, bit 10 (0x400) **selectable** (interactive objects).

**Per-object fields the pair test reads**, written by the object's prepare (`0x401c50`, ported
as `SceneObject::prepareFrame`): box `+0x10..+0x1c` = the frame's hit rectangle (left = hitX,
top = hitY, right = left + hitW, bottom = top - hitH; Jim's are multiplied by his scale >> 8,
`0x407730`); `+0x20/+0x24` = position + the bank's movement **before** the animation advance;
`+0x28/+0x2c` = after it, or the pending position (`+0x54/+0x58`) plus the movement when the
pending position differs from the committed one; `+0x30` = the frame. `set_object_position`
(`0x401d30`) sets the pending position, and the committed one too when its flag is 1, then adds
the object to the scene's moved list (`0x408440`, collidable objects only, once). Only the
pointer, its probe and Jim call it.

**End of frame (`0x408890`)**: prepare every moved object and the collidable, active objects of its
group that were not yet prepared; then for every moved collidable object X and every other
collidable active object Y in X's group: `X.inFront(Y)` -> `order(X, Y)`, else `Y.inFront(X)` ->
`order(Y, X)`; the queue is sorted by overlap then priority and each pair calls
`a.slot0x30(b, overlap)`; finally every moved object's committed position becomes its pending one
and the moved list and queue are cleared.

**Not yet verified:** this reading is from the decompilation and assembly only; the queue, moved list and
dispatch have not been compared with the original (they need the scene's list structures laid
out in the emulator).

## Engine: moved list, pair queue, end-of-frame dispatch, pointer probe (25 October 2026)

Branch `codex/hotspots-queue` (from `1448d35b23b`), uncommitted: `scene.h/.cpp`, `jim.h/.cpp`,
`tesoro.h/.cpp`. **Verified by reading the decompilation and by compiling only. Not run in a window,
not compared with the original.** The scene's lists (moved list, queue) were not laid out in the emulator.

What was added:

- Flags `kCollidable` (8) and `kSelectable` (0x400); interactive objects, characters and the floor get
  `kGrouped | kCollidable | kSelectable | 0x40` as their constructors do; Jim gets `kCollidable | kSelectable`.
- `SceneObject::updateOverlap` (the part of `0x401c50` the pair test needs): box from the frame's hit
  rectangle, position before and after the animation advance (pending position when it differs), the
  frame's outline (type 2 points, or type 4 floor points). Jim's box is scaled by his scale (`0x407730`).
- `Scene::objectMoved` (`0x408440`), `Scene::resolveOverlaps` (the rules of `0x408890` recorded above):
  prepare moved objects and their collidable, active group neighbours; pair each moved collidable object
  with the others of its group using the in-front test; queue touching pairs (`testOverlap`) in order of
  contact then depth; deliver each to the first object (`onOverlap`); commit positions; clear.
- `PointerProbe`: a point object (no box, no outline) put on the mouse's world position each frame while
  input is enabled; it counts as in front of any selectable object and records what it is paired with as
  its candidates (`candidates()`), cleared at the start of the next frame.
- Jim registers as moved when he steps; the engine calls `resolveOverlaps` where it used to call
  `commitPosition`.

**Assumptions and gaps (do not treat as recovered):**

1. **Probe shape.** The original probe's bank and frame data were not found (its constructor `0x402240` sets
   none). I modelled it as a point with no outline, so it is tested by hit boxes only (a stationary overlap
   of boxes counts, `t = 0`). If the original probe has a frame, picking would use outlines for hotspots
   that have them.
2. **Probe group.** The probe and Jim go in the scene's first group (the original puts both in the group
   at scene `+0x70`); hotspots in other groups are never paired with the probe. Whether hotspots live in
   that group is not checked.
3. **Stationary pairs.** `testOverlap` queues a stationary overlap of two outlines only when the second
   object has a single point, and never when the first has one. The probe has none, so only box overlap
   registers it, as above.
4. **Jim's detours not ported.** Jim's in-front test (other object's kind byte 7) and his collision response
   (`0x4079e0`, the floor-node routing) are not implemented. **Important consequence to check:** the floor
   (kind 7) is selectable and collidable, so in the original Jim probably overlaps it while walking, and
   if its `+0x78` is set then click walks are re-planned along the floor's node chain (`0x414f10`), not
   straight. The straight click walk in the engine may therefore differ from the original.
5. List order inside a group is not kept sorted as objects move; the pair loop's order within a group
   follows the engine's list.
6. Nothing consumes the candidates yet (hover text, click, cursor frames, approach walk).

## Engine: hover, click and approach (25 October 2026, branch `codex/hotspots-hover`)

Built on `codex/hotspots-queue` (uncommitted, same status). **Verified by reading and compiling only; not
run in a window, not compared with the original.**

Recovered from the executable and used:

- **Pointer reaction (`0x402e80`)**: the candidates (what the probe overlapped on the previous frame, in
  queue order) are asked one after another until one answers non-zero. An object not hovered on the
  previous frame is hovered (`0x44`) and nothing else happens to it that frame (so a click on that frame
  is swallowed); one already hovered gets `0x38` on a left click, `0x3c` on a right click (which just
  calls `0x38`), else `0x44`. Result 3 ends the frame's input. Objects that were hovered and are no longer
  candidates get `0x48`. Only if nothing took a left click is it a click on the ground (`0x4030e0`).
- **Interactive object parameters (`0x40ed10`, checked against the scene files: 104-byte records of kinds
  9, 12, 17, 20; 118 for kind 6; 121 for characters, which start 4 bytes later)**: `+2/+3` approach x
  offset and `+4/+5` y offset (signed 16 bit, added to the object's position), `+6` direction minus one
  (1 up, 2 down, 3 left, 4 right), `+7` its name in the dialogue tables (a number written as text, such as
  `167`, or empty), and from `+0x13` six 12-byte names of the lines its verbs answer with (`OBJ053T03`
  style), for verbs 0, 1, 3, 4, 5, 2 in that order.
- **Hover (`0x40f050`)**: the pointer shows the dialogue table text of the object's name; with none, the
  object's own name (the original decodes the 64-bit name id back to a string, `0x415cb0`; the engine uses
  the scene file's name, which should be the same text). **Unhover (`0x40f0b0`)** clears it. The text is
  drawn white (`0xFFFF`) above the pointer, centred on it, 37 pixels up, 200 wide (`0x402d80`).
- **Click (`0x40f020` -> `0x40f1a0`)**: the verb is the pointer's current frame (0 for the default arrow);
  Jim walks to the approach point with the object as waiter. **Arrival (`0x40f0d0`)**: Jim turns to the
  object's direction and the verb runs through the object's verb slots (`0x50..0x68`; verbs 7 and up are
  inventory items, slot `0x6c`).
- **Default verb answers (`0x40f210`, `0x40f280`, `0x40f300`, `0x40f380`, `0x40f400`, `0x40f480`)**: Jim
  says the object's line for the verb, or for most verbs a generic line from the string table if it has
  none; verb 0 answers only if the object has a line. Input is switched off (`pointer slot 0x20(0)`) and the
  object stays in the update list at priority 5 until Jim has finished speaking. 98 of the factory-built
  classes share these slots; a few objects override one verb (puzzle objects, doors); that table is not yet
  mapped.

Ported: the pointer reaction above (`updatePointer`), hover text and its drawing, the unhover, the click
and the approach walk with the object as waiter (`ActionWaiter`, which scripts also are), the turn on
arrival. **Not ported, and visible:** the verbs. Arrival ends in `warning("verb N of NAME does nothing
yet")`, so nothing visible happens after Jim turns.

**Assumptions and gaps**

1. Everything in the previous section's list still applies (point-shaped probe, first group only, Jim's
   detour not ported). If nothing is ever hovered, the first thing to check is whether the objects are in
   the probe's group.
2. The pointer's frame is always 0 (the engine has one cursor), so every click is verb 0.
3. Jim speaking when a click arrives (the original ends his speech first, `0x40f1a0`), clicking on Jim
   himself (his `0x40f1a0` path skips the walk), the immediate first walking step in the same frame, and
   the pointer's input switch (`0x20`) are not ported.
4. Kinds 5, 6, 9, 12, 17 and 20 share one parameter layout in the engine; it was checked for the sizes
   listed, not for each class's constructor (doors use `0x414be0`).
5. The interface scene (inventory items, toolbar) has its own pointer pass in the original; not ported.

## Hotspot positions: interactive objects drop the animation's movement (25 October 2026)

Found by running the office scene through the engine's own scene code (ignored driver
`local-media/jim-implementation/probe_driver.cpp`: each object's drawn rectangle and hit rectangle on the
screen, camera at the office entry). Objects with no movement matched their pictures exactly; the invisible
hotspots (`CARTA`, `LAMPARA`, `MAPA`, `PEZ`, ...) were displaced by their starting frame's cumulative offset
(for example (306, 143) for `CARTA`), which put `CARTA` at the top right instead of the middle of the desk.

**Recovered:** all 98 factory-built classes (interactive objects, doors, characters, the floor) and Jim have the
same virtual slot 8, `0x407770`, which advances the bank and then **zeroes the bank's movement** (`+0x28/+0x2a`);
the prepare (`0x401c50`) calls it, and every shown object is prepared before it is drawn (`0x408550`). So after the
first prepare their movement is 0: they are drawn and tested at their position plus the frame's own offsets. The
pointer class and scenery use `0x40e6a0` instead and keep their movement (scenery's draw adds it).

**Engine:** `SceneObject::dropsMovement()` (true for `InteractiveObject`, so characters too, and `FloorObject`;
Jim and props keep theirs) makes `prepareFrame` and `updateOverlap` clear the player's movement after the animation
update (`AnimationPlayer::clearMovement`). The first overlap test of an object still sees the starting offset in its
"from" position, as in the original. Result in the office: every hotspot sits where its picture or area is
(`CARTA` 274..322 x 217..262 on the screen; `PEZ` 567..628 x 60..106).

**Side effect to check:** the drawn position of interactive objects and characters no longer includes a cumulative
animation offset. This follows the original code, but the characters' animations in the introduction (the captain
leaving) were checked against the original only at rest and while talking; watch that scene.

## Hotspot debug layer and a note on subtitles (25 October 2026)

**Debug layer** (uncommitted on `tesoro-engine`, over layer 2): debug channel `Hotspots`
(`scummvm -d Hotspots`, or `debugflag_enable Hotspots` in the debugger). `Scene::debugShapes` and
`TesoroEngine::drawHotspots` draw, over the scene, the hit box of every collidable active object as the last
overlap pass saw it (yellow: what the probe overlapped on the last frame, cyan: Jim, green: the rest), the
frame outline of those that have one (magenta, drawn through its points), and a red cross on the mouse.
Boxes are those of the last `updateOverlap`, so objects of a group that was not prepared on that frame show
what they showed last.

**Subtitles** (found while looking at a report that the narration of the opening shows no text): the narration
over the dark harbour at the start is the audio of movie 2 (`data02.bin`, 147 s, with an audio track), and the
captain opening the door is movie 3 (`data03.bin`, also with audio). The original plays movies through DirectShow
(`0x4132b0`) and draws no text over them. The captain's voice behind the door afterwards is `play sound
'ANM1BCAP0'`: the original's handler for that op (`0x405a60` case 0xe, `0x404110`) only plays the WAV; it never
looks up the text, although `ANIMS.TXT` has a line for it. So the original shows no subtitle in either place.
Showing them would be a ScummVM addition: for the sound line a matching text entry exists; for the movie audio
the engine has no timing for what is said.

## Default verb answers (25 October 2026, branch `codex/verbs`, off `tesoro-engine` `ce3f10c8bfc`)

Built on Gabriel's `tesoro-engine`, which already has the hover, click and approach walk (his integration of
the earlier `codex/hotspots-*` work; the old uncommitted hover worktree diff is saved in
`local-media/hotspots-hover-wip.patch`, superseded). **Compiles; not run in a window, not compared with the
original.** Data checked by `tools/verify_verb_texts.py` (`analysis/verb-texts.json`).

Recovered (`0x40f0d0` arrival, vtable `0x42bad8`, `0x40ee70` object update; read directly from the EXE):

- Arrival switches on the verb to the object's virtual slots: verb 0 `0x50` (`0x40f210`), 1 `0x54`
  (`0x40f280`), 2 `0x5c` (`0x40f380`), 3 `0x58` (`0x40f300`), 4 `0x60` (`0x40f400`), 5 and 6 `0x64`/`0x68`
  (`0x401fb0`, `xor eax,eax`: nothing happens, result 0), 7 and up `0x6c` (`0x40f480`, inventory item
  `verb - 7`, after clearing the pointer's held item).
- The object stores its six verb lines at `+0x70 + 4 * verb` (the constructor `0x40ed10` writes line *k* of the
  record to verb `{0,1,3,4,5,2}[k]`), so the engine's `_verbTexts` is indexed by verb. Each default handler
  reads its verb's line: if there is none, verb 0 does nothing (result 0), the others use a generic line:
  verb 1 `OBJ125T03`, verb 2 `OBJ096T07`, verb 3 and 4 `OBJ106T05`.
- With a line: Jim speaks it (`0x410330`: sets the talk state, starts the sound of the entry if it has one),
  the pointer's input is switched off (`pointer slot 0x20(0)`), the object sets `+0x8c` and joins the update
  list at priority 5, and the result is 3 (which ends the frame's input handling). Its update (`0x40ee70`)
  waits until Jim's `+0xd4` (speaking) is clear, then switches input back on and leaves the list.
- Inventory item verbs (`0x40f480`): checks the held item's table (`+0xa0` pairs of name ids with `+0xa4`
  lines) for this object's name id, else answers `ICON01T058` for item 0 or `MODELO2` for the rest. The
  arrival code clears the held item first, so the table lookup is only reached if something sets it again;
  not ported (there is no inventory yet).

Ported: `InteractiveObject::runVerb` for verbs 0-6 (`answerVerb`), Jim speaking with his mouth moving
(`PointerHost::jimSays`), input off while he speaks and on again afterwards (`TesoroEngine::_inputAfterSpeech`).

**Data check:** 312 interactive objects outside `INTERFACE.LVL` name 216 verb lines; all 216 and the five
generic lines exist in the dialogue tables. Seven lines have **no WAV** (`BAULOPEN`/`OBJ075T11` in
`Camarote`/`Camarote2`, `ESPADA`/`OBJ145T05`, `FOGATA`/`OBJ128T05`, `COCO`/`OBJ083T05`,
`SERVICIOH`/`OBJ106T03`, `CALDERO`/`OBJ112T05`). The engine stops with an error when it reaches one (same as a
script speech); what the original does for a text without sound was not checked (`0x410330` skips the sound
when the entry's sound field is 0).

**Assumptions and gaps**

1. **(Superseded by the next section.)** The verb is the pointer's frame, and the engine's cursor had one frame. The
   original changes it through the interface scene (`INTERFACE.LVL`: `ACCION0..4` kind 3 at the bottom edge,
   `CURSOR` kind 10, `CONTROL_ICO` kind 11, `ICONOS`, `TEXTOICONOS`), whose classes are not recovered. That is
   the next dependency (also needed for the inventory, whose items are kind 13 there).
2. The object's `+0x8c`/`+0x8d` flags and its place in the update list are not modelled; the engine restores
   the pointer when Jim's speech ends (one frame granularity may differ). The click-while-Jim-speaks path
   (`0x40f1a0` ends his speech) and the second dispatch in `0x40ee70` (verb run from the update when `+0x8d`
   is clear) are not ported.
3. The speech option passed to Jim (`+0xd0`) is passed as 1; its source is not recovered (the engine ignores it).
4. Objects that override a verb (doors, puzzle objects) still use the defaults.

## The verb bar and the pointer (25 October 2026, branch `codex/verbs`)

**Compiles; not run in a window, not compared with the original.** Exports: `/private/tmp/tesoro-ctrl*`
(control icon `0x415250`, vtable `0x42be40`; hover `0x415490`, unhover `0x415640`, click `0x415440`, slot `0x40`
`0x415350`), pointer update `0x4029f0`, pointer pass `0x4030e0`, play `0x401de0`, icon group `0x414930`.

Recovered (read from the executable and from the frame records of `INTERFACE.LVL`):

- The interface is a scene of its own (`INTERFACE.LVL`, camera at its centre, x to the right and y **upwards** from the
  bottom-left corner; the mouse maps to `(x, 479 - y)`). Its pointer pass runs **before** the room's (`0x4030e0`): if an
  interface object takes the pointer (result not 0), the room does not see it, and a click on the ground only walks Jim
  when the pointer's frame is 0.
- Kinds: 2 `ICONOS` (a list of children names at `+0x10`, count at `+2`: `ACCION0..4` in that order), 3 `ACCIONn`
  and `CUADRADO` (scenery), 10 `CURSOR` (the pointer, bank `CURSORES`: **frame = verb**; 0 arrow, 1 look, 2 talk, 3 take, 4 open,
  then inventory item cursors), 11 `CONTROL_ICO` (the strip along the top edge).
- The bar hides above the screen (icons at y 526..527). `CONTROL_ICO` frame 0 is a strip at the top edge of the screen
  (y 468..583); its frame 1 is a taller one (405..520) that keeps the pointer on the bar.
  - First overlap (slot `0x40`, also the hit test): clear the pointer text, control icon -> frame 1, icons and `CUADRADO` play bank 0
    frames 0..1 (a 200 ms slide with 85 px of movement), nothing lit; then hover.
  - Hover: nothing happens while `CUADRADO` is still playing. After that, if the first icon is still above 480, every icon moves
    down 85 and shows bank 1 frame 0. The icon with `|icon.x - pointer.x| < 35` (the last that matches) is lit (bank 1
    frames 0..3); the one that was lit plays bank 2 from `3 - its frame` to 3 (the same pictures backwards).
  - Click (left or right): if an icon is under the pointer, the pointer plays frame `index` of its bank 0. The verb persists
    until another is chosen (nothing in the verb code resets it; choosing an inventory item resets the frame to 0 in `0x40f480`'s caller).
  - Unhover: icons go back up 85 if they were down, then play bank 0 frames 2..3 (stay visible 200 ms, then vanish), same for
    `CUADRADO`, and the control icon returns to frame 0.
- The verb of a click on an object is the pointer's frame; objects dispatch it as in the previous section.

Ported: `interface.h/.cpp` (kinds 2, 10, 11), `TesoroEngine::loadInterface/updateInterface/reactPointer`, interface drawn over the
room (pointer in front of the bar), the game's own pointer sprite replacing the system cursor in the adventure, the ground click
only with verb 0, and the verb passed to `onLeftClick`.

**Assumptions and gaps**

1. **`CUADRADO`'s image is a shadow (kind 2)**: composed since the next section.
2. `TEXTOICONOS`, the inventory (kind 13 items, `INVENTARIO`, `IOBJETOS`), the options pages (kinds 15, 22, 23) and Jim's `PRINCIPAL`
   record (kind 1) in `INTERFACE.LVL` are kept inert (they print one warning per kind).
3. The hover code's condition on the menu scenes (`Jim.scene == DAT_00442168/4421f0/4421f4`) is not ported.
4. The pointer text slot `ptext` (`0x4023f0`) is only cleared; the verb names are not shown. The interface pointer's depth
   (in front of everything) is a choice, not recovered.
5. The pointer image anchor uses the frame offsets like any object; its hot spot was not compared with the original.

## Shadow images and the verb bar's labels (25 October 2026, branch `codex/shadow-labels`, off `tesoro-engine` `545da5e492e`)

**Verb labels: nothing to port.** The control icon looks up `TEXTOICONOS` and stores its position (`+0xac`, `+0xb0`), but nothing
in the executable reads those fields back for this class (only inventory-related classes at `0x4163xx`/`0x416bxx` use their own
`+0xb0`), hover and unhover never set the pointer text, and the pointer text drawn by `0x402d80` is the hover name of room
objects. So the original shows **no verb names** on the bar; the proposal to add them rested on an unchecked assumption.

**Shadows, recovered** (`0x411f80` queue, `0x4112d0` shadow span list, `0x412640` merge, `0x412e10` composition, `0x411c20` table
loader, `0x411870` renderer init; `TABLA1.TBL` is the only 32,768-byte table in the archive, read at renderer `+0x66c`):

- The only kind-2 image in the whole game is `CUADRADO` in `INTERFACE.LVL` (640 x 100, 100 spans, every span value 0): the
  shade behind the verb bar. Scenery submits it with depth window 0 (`0x40e6b0`), so it applies to everything behind it.
- The table holds 32 levels of 512 RGB565 colours. A pixel's entry is `G3 << 6 | R3 << 3 | B3` (the top three bits of each
  channel, from the 565 value), `0x100` bytes further for each unit of the span's value. In a 555 display (`+0x67c` set) the
  loader converts the table; the engine is always 565.
- The shadow lists keep, per row, spans without overlap: **where shadows overlap only the nearest counts** (strictly smaller
  depth wins, the first submitted wins ties), they do not stack. A picture pixel is darkened when its depth is **not smaller than** the
  shadow's (equal depth is darkened); what is nearer is left alone.

Ported: `SceneRenderer::setShadowTable/drawShadow` (after the unscaled sprites, before the scaled ones), `TABLA1.TBL` loaded at
start, and the interface draws `CUADRADO` at depth 2 (icons 1, pointer 0).

**Compared with the original routines** (`tools/compare_shadow.py`, ignored driver `local-media/jim-implementation/shadow_driver.cpp`,
Unicorn over `0x411f20`, `0x411f80`, `0x412e10`): three seeds of 200 frames, 184,320,000 pixels identical, with Jim's sprites below, equal
to and above the shadows, one or two overlapping shadows at different depths and orders, and levels 0-3 (synthetic copies of
`CUADRADO`; only level 0 exists in the game data). Reports: `analysis/shadow-comparison-seed{1,2,3}.json`.
Harness note: the original resets its shadow lists at `rows + 480 * 2000`, so the emulator must place them there.

**Gaps:** shadows over **scaled** sprites (the engine darkens only the unscaled picture; Jim is scaled), over solid-colour (kind 3)
images, and in a 555 display were not compared; real levels above 0 do not occur in the data. The band on screen was not
compared with a Windows capture.

## The path graph and Jim's detours (26 October 2026, branch `codex/paths`)

Built on `tesoro-engine` `a48ba515fa3`; uncommitted. Files: `path.h/.cpp`, `jim.h/.cpp`, `scene.h/.cpp`, `module.mk`.

**Recovered** (`analysis/paths/`, `analysis/jim/004079e0*`, `analysis/click-walk/00414f10*`):

- **Kind 8, the room's path** (`0x414e00`, 0xb4 bytes, built on the interactive base `0x40ecb0` with the selectable flag cleared, so flags
  `0x108`): its first parameter is the frame to show; the frame's `type 2` points are the **nodes** (relative to the path's position).
  Scripts swap the frame (`sendmessage 'PATH' play`) when the room changes, so the route changes with it.
- **The floor (kind 7, `0x414620`)** names its path in the twelve bytes after its first frame (floor `+0x6c..+0x77`; the pointer lands at
  `+0x78`) and clears the selectable flag (`0x108`), so it is collidable but cannot be picked.
- **Jim's walking banks (`0x4070c0`)** get, for banks 3 to 6 (up, down, left, right), a **one-point outline at (0, 0)** in every frame.
  This is what makes the floor test a point test: the pair test sees a one-point first object against the floor's outline
  (`0x409560`: if the point starts inside, a step that ends outside is a hit; if it starts outside, any movement is a hit).
  Without it the boxes alone overlap the floor all the time (the first attempt stopped every walk at once).
- **Jim's in-front test** (`0x4079d0`): true for an object whose kind is 7. His **collision response** (`0x4079e0`), for a walk in
  progress (`+0xd5`): the pending position becomes the current one (the step is dropped); if the walk is not itself a leg of a route
  (`+0xf0` clear) and the floor has a path, the **route** (`0x414f10`) is tried: the node nearest to the point if it is nearer to it than
  Jim is, then the node nearest to Jim; Jim walks to that node as a leg (start-walk with the leg flag, the path being the waiter).
  If there is no route he turns to face the way he was going and whoever waits for him (`+0xf4`, slot `0x34`) goes on. A collision during a leg
  clears the flag and turns him; nobody continues, so the route is abandoned.
- **Leg end** (`0x414e80`, the path as waiter): from Jim's node, one node at a time towards the node nearest the point, then straight to the point
  (each as a leg), the last one with the original waiter.

**Ported:** `PathObject::route/actionDone`, `JimObject::startWalk(..., leg)`, `inFront`, `onOverlap`, `updateOverlap` (the one-point outline for the
walking banks), `FloorObject::path()`, floor no longer selectable.

**Compared with the original:** `tools/compare_path_route.py` (ignored driver `local-media/jim-implementation/route_driver.cpp`): the production path
and the original route and leg-end routines (Unicorn, Jim's start-walk captured) make the same routes, leg by leg, over 7,600 and 11,400 random
positions and targets in the 38 rooms with a path (about 8,000 routed, 23,000 legs), reports `analysis/paths/path-route-comparison-seed{1,2}.json`.
`tools/compare_jim_walk.py` (the walking step) still matches on two seeds of 100 walks after the change. `tools/test_detours.py` (driver
`detour_driver.cpp`) is a smoke test through the whole scene (collision, re-plan, legs): no walk fails to end.

**Not compared:** the collision response `0x4079e0` itself and the pair delivery were not run in the emulator (the flow is by reading the decompilation);
the walk-stops-at-the-edge behaviour has not been seen against a running original; the floor test relies on `pointInPolygon`, which was compared on the
frames' polygons but not on every floor.
**Observation:** a walk whose route ends in a leg that leaves the floor at its first step (nodes on or beyond an edge) stops there, as the original
would given the same data; I cannot tell whether the original ever shows that.
