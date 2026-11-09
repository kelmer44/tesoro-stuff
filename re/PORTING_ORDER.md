# Porting order for the rewrite in `tesoro-scratch`

Source to copy from: branch `codex/remove-hosts` (worktree `/Users/gabriel/Desktop/source/scummvm-tesoro-nohosts`).
Not the history, and not `codex/object-kinds` (it still has the host interfaces).
Keep the matching `re/contracts/<address>.md` open while copying a function: it says what must not change.
Sizes are approximate line counts of the `.cpp`/`.h` pair on that branch.

## Progress (from the commits on `tesoro-scratch`)

Done: **step 1** (wiring the archive in, the archive index and archive, name keys, the clock) and **step 2, slices a, b and c** (the `AnimImage` blob, `AnimBank` parsing, and the first image on screen).
Next: **step 2, slice d** (`AnimPlayer`), then **step 3** (screen). Slice e (`AnimSet`) is not done in step 2: it waits for step 4, because it only exists to be a member of `Object`.
Naming: an animation step is `AnimStep` (accessors `AnimBank::step(i)`, `stepCount()`), in both this branch and `codex/remove-hosts`. Older docs say "sequence".
Skipped on purpose: `core/random.*` (ScummVM's `RandomSource` is used) and `AnimImage::parse` (nothing calls it).
Commits so far: "Wire archive in", "Game clock", "Models AnimImage blob", "Adds AnimBank parsing", "Display first image".

Rule for every step: copy one slice, make it build, check the small result named under "You see", then go on.
The order follows the dependency chain in `re/SUBSYSTEMS.md`: each step only needs the ones above it.

---

## 1. Files and time (about 400 lines) ✅ done

Files: `core/namekey.*`, `core/archiveindex.*`, `core/archive.*`, `core/clock.*`. Random numbers are ScummVM's `RandomSource` (no `core/random.*`).
These depend on nothing else in the engine, so they come first.

**Goal:** open `TESORO.TRE` and `TESORO.DAT`, find an entry by name, and keep game time.

| Slice | Files | You see |
|---|---|---|
| 0. Wiring ✅ | `tesoro.cpp` and `console.cpp` (in `engines/tesoro`, not `core/`): give `TesoroEngine` an `Archive` member, open `TESORO.DAT`/`TESORO.TRE` at the start of `run()` (replacing the template's sample boxes and palette cycling), and add console commands. Nothing calls `Archive` yet, so this is needed before any later check can be seen | console `entries` lists the first entries from `index().count()` and `entry(i)`; `entry NAME` prints the offset from `findEntry` |
| Name keys ✅ | `core/namekey.*` | hash a few names |
| Archive index ✅ | `core/archiveindex.*` | look up one known entry and print its offset. Records are kept only when flag bit 4 is set; names are upper-cased and sorted largest first, which is what `find()` searches |
| Archive ✅ | `core/archive.*` | seek to an entry in `TESORO.DAT` |
| Clock ✅ (copied earlier than planned) | `core/clock.*` | game time ticks. The clock is first needed by `AnimPlayer` (step 2, slice d). Random numbers: use a `Common::RandomSource` in the engine; there is no `random.*` to copy |

---

## 2. Animations (about 1,300 lines for the whole layer; slices a to c are done)

Files: `core/animimage.*`, `core/animbank.*` (337 lines in the `.cpp`, which holds both `AnimBank` and `AnimPlayer`), `core/animset.*`.

**Goal:** load one animation, show a picture from it, then make it play over time.

The four classes, and how they depend on each other:
- `AnimImage` is one picture, kept as the blob stored in the archive. It does **not** expand the spans: it only reads the blob and checks it
  (the offsets must fit inside it), and rewrites the items of shadow images. The spans (runs of pixels per row, stored as RGB565) are expanded only when the picture is drawn, in
  `RenderQueue::queueRows` and `Display::compose` (step 3).
- `AnimBank` is the data of one animation file: its steps (`AnimStep`: image, offset, movement, duration, hit rectangle; some have no image) and their images. It is shared.
- `AnimPlayer` is playback state over a bank: which step is current, when it started, loop and pause flags, and the movement the current
  step gives the object. The bank is held through a shared pointer, so copies of a player share the bank and keep their own state.
- `AnimSet` is the animations of one game object: one `AnimPlayer` per bank, of which one is current.

| Slice | Where | You see |
|---|---|---|
| a. Image blob ✅ | `AnimImage::load`, `convert` and the small accessors (`core/animimage.cpp`, about 100 lines). Leave `scaleBrightness` and `BrightnessTable` for later | nothing yet: `AnimImage::load` reads a size-prefixed blob at the stream's current position, and the only thing that knows where an image sits is the bank file (slice b). Copy it, make it build, and check it with slice b |
| b. Bank file ✅ | `AnimBank::load` (lines 38-140 of `animbank.cpp`): header (step and image counts, header flag, three header dwords), the steps, then the images through `AnimImage::load`. Load in `kLoadNow` mode only. Remember the data starts 4 bytes after the archive offset | done with the console command `animbank NAME.ANM` (step count, image count, and per image its type, size and pixel bytes) |
| c. First picture ✅ | a throwaway loop that walks one image's spans into a `Graphics::Surface`. Its model is `RenderQueue::queueRows` (renderqueue.cpp, around line 427), unscaled path only. It lives in `console.cpp` as `drawImage`, used by the console command `showimage NAME.ANM N` | **first milestone: a real picture from the game in the window** (done) |
| d. `AnimPlayer` ← **next** | see below | the picture animates |
| e. `AnimSet` → **moved to step 4** | see below. Needs `Object` (its `initAnimated` builds the set), so skip it until step 4 | an object owns several animations and switches between them (checked there, not here) |
| f. When a caller asks | Add each one in the slice named in brackets, which is its first caller: `AnimBank::clearStepMovement` (4f, `SpriteObject::init`), `clearStepHitRect` (5c, the pointer's `onAttach`), `makeSinglePoints` and `scaleBrightness` (needs the `BrightnessTable` in `animimage`) (6b, Jim), `AnimSet::shareFrom` (7d, the shared floor of the island levels), `AnimSet::switchTo` and `switchPlayingTo` (6b and 6c, Jim's poses and walk), `AnimBank::loadDeferredImages` and the `kLoadDeferred` mode (7a, loading levels), `AnimPlayer::pause`/`resume` (8d, Jim's and the characters' speech), `AnimSet::stash`/`restoreStashed`/`isStashed`/`setStashed` (8d and 9b), `AnimPlayer::saveState`/`loadState` and `AnimSet::saveState`/`loadState`/`selectedBank` (step 10) | not needed for the first scenes |

### When to add `AnimPlayer`

Add it right after the first picture works (slice c) and **before** the screen queue (step 3). Three reasons:

1. **Nothing in the engine uses a bare `AnimBank`.** `AnimPlayer::load` is what creates the shared bank, `Level` keeps its banks as an array of
   `AnimPlayer` (`Level::levelPlayers()`), and every object copies players from that array. If you build the draw loop on `AnimBank` directly, you
   will have to redo it when `Level` arrives in step 4.
2. **It is the first thing that needs the clock.** `setRange` and `advance` take the game time (`now`), so pull in `core/clock.*` from step 1 at
   this point if you haven't yet.
3. **It is what makes the picture move.** Copy it in this order, each with a visible check:

| Order | Function (`animbank.cpp`) | You see |
|---|---|---|
| 1 | `AnimPlayer::load` (line 184) | slice c now draws through a player, same picture as before |
| 2 | `AnimPlayer::setRange` (194-242): plays steps `first..last`; a `last` of -1 extends to the next step without an image | print the range and total duration for one animation |
| 3 | `AnimPlayer::advance` (243-310): moves the play head to the step the game time has reached and updates the movement | the picture steps through its frames in the window |
| 4 | the accessors in the header (`current`, `first`, `last`, `movementX`, `movementY`, `flags`) | enough for step 4's objects |
| later | `pause`/`resume` (326-336): needed when the game pauses or a scene holds the clock. `saveState`/`loadState` (312-325): step 10 | |

### When to add `AnimSet`

Add it at the start of step 4, together with `core/object.*`. `Object::initAnimated` builds an `AnimSet` from the level's banks, one player per
bank the object uses. Copy `init`, `current`, `selectBank`, `tick` and `tickClearMovement` first. `switchTo`/`switchPlayingTo`, `stash`/`restoreStashed`
and `shareFrom` wait for the objects that need them (Jim's walk, speech, and the shared floor of the labyrinth levels).

You don't need the whole layer for the first picture: slices a, b and c (about 550 lines, with the clock arriving at slice d) are enough.

---

## 3. Screen (about 1,050 lines: `renderqueue.*` 644, `display.*` 168, `textqueue.*` 196)

Files: `core/renderqueue.*`, `core/display.*`, `core/textqueue.*`.
Needs: step 2 (images and the player), and the clock for the check of 3e.

**Goal:** replace the throwaway `drawImage` by the real screen: objects hand images to the screen during the frame, then one `compose` turns them into pixels.

How it works, so the pieces make sense:
- The screen is **not** painted image by image. Each of the 480 rows keeps a list of horizontal **spans** (a start column, an end column, a depth, and where the pixels come from).
- Queuing an image adds its runs to the rows. A run with a smaller **depth** is nearer: it cuts the spans behind it, and what is behind it never needs drawing.
- `compose` then walks each row left to right and copies the pixels of each span into the frame. The spans of a row are laid down one after the other from the row's start, so **a row must be covered end to end**: the game always draws a full-screen background first (and the old code leaves an empty row untouched).
- Scaled images (the pointer, characters) are different: they are painted straight into the frame during `compose`, and only their visible pieces are entered in the row lists.
- The screen class is `Display` (`core/display.*`). Its three requests are named `queueImage`, `queueImageScaled` and `queueText`, because they only queue: nothing is drawn until `compose` (a plain `draw` is kept for an object's own `draw` method). On `codex/remove-hosts` it is one plain class: it used to be `Display` behind an abstract `Display` base with no other implementer, and the two were merged. Objects take a `Display &` in `draw`; forward-declare `class Display;` in object headers and include `display.h` only in the `.cpp` files that draw.

| Slice | Where | You see |
|---|---|---|
| 3a. Row and spans | `RenderQueue::Row` (`renderqueue.cpp` lines 29-163): constructor, `clear`, `allocate` (the row's spans live in a fixed array of 125 slots, 50 for mask rows; slot 0 means no span) and `insert` (about 105 lines: cuts the spans behind a nearer one and is cut by what is in front) | a console command that inserts spans by hand and prints the row. Insert 10..50 at depth 5, then 30..80 at depth 3: expect 10..29 at depth 5 and 30..80 at depth 3. Insert them in the other order: same result |
| 3b. Queue an image | `RenderQueue` constructor, `reset`, `addSprite` (lines 165-205) and `queueRows` (427-463): turns a literal or solid image into row spans. This is your throwaway `drawImage` loop, but writing spans instead of pixels. Leave `queueMaskRows` for 3g, which you may skip | a console command that queues one image and prints the spans of one row. They should match the runs `drawImage` walked (the same gaps and lengths) |
| 3c. Compose | `Display` (`display.*`): the constructor, `queueImage`, `startFrame`, `frame()` and `compose`. Copy the **plain row loop only** (finished `display.cpp`, the `for (uint y ...)` loop inside `compose`, about 20 lines): per span, work out `count`, then fill with `span.colour` when `span.pixels == kSolid`, or copy 16-bit pixels from `span.pixels` when it is an address; skip the span when it is `kNoPixels`. **Leave all of this out for now**, because it is tangled into `compose` and the header: in `compose`, the scaled-image sort, the first scaled loop (which declares `drawn`), the shadow merge, the shadow-span branch (`span.x0 == -1`), the final scaled loop (which uses `drawn`, so it cannot stay without the first one) and `_texts.draw`; in `startFrame`, `_texts.clear()`, `_scaled.clear()` and `_layerDepth = 0`; in the header, `Scaled`, `kMaxScaled`, `_scaled`, `_layerDepth`, `_shades`, `texts()` and `TextQueue _texts`. Keep only `RenderQueue _queue` and the frame array. In the engine: a `Display _display` member and, each frame, `copyRectToSurface(_display.frame(), 640 * 2, 0, 0, 640, 480)` into `_screen` | `playanim` now goes through `_display` (`startFrame`, `queueImage`, `compose`, copy) and shows the same animation as before. For the background, queue one full-width solid span per row at the largest depth (a throwaway 480-row loop with a grey colour), because rows must be covered |
| 3d. Depth order | nothing new to copy | draw two animations (for example `JIMDERECHA.ANM` and `JIMIZQUIERDA.ANM`) overlapping, at depths 10 and 20, then swap the order you draw them in. The depth-10 one is always in front |
| 3e. Scaled images | **needs `core/wrap.h` first** (the 16.16 fixed-point maths in `rasterScaledImage` uses `wrapMul`); **adds back** to `display.h`: `Scaled`, `kMaxScaled`, `_scaled`, `_layerDepth`; to `startFrame`: `_scaled.clear()` and `_layerDepth = 0`; to `compose`: the sort, the first loop (with `drawn`) and the final loop that uses it, as a pair. `Display::queueImageScaled` and the `_scaled` list (at most 50 a frame), the `compose` loop that sorts it deepest first and the layer depth rule (images deeper than the layer depth are painted before the rows, the rest after, on top); `RenderQueue::rasterScaledImage` (466-535: 16.16 fixed-point scale, negative scaleY turns the image upside down) and `rasterScaledRows` (577-644). Leave `queueScaledRows` for 3g | a console command `scale 0x8000` draws one frame at half size, `0x20000` double size, a negative scaleY upside down. The picture stays sharp and moves with the clock |
| 3f. Text | **adds back** `#include "tesoro/core/textqueue.h"`, `TextQueue _texts`, `texts()`, `_texts.clear()` in `startFrame` and `_texts.draw(&_frame[0])` at the end of `compose`. `TextQueue` (`textqueue.cpp`): `setFont` (takes the font bank only: the game has a single font, so there is no font number; a font is a bank, glyph c is step `c - 0x21`, and the space glyph is `0x7f`), `add` (lines 40-131: wraps to a maximum width, centres, colour in 5-6-5, returns the layer depth), `shiftLines`, `draw` (137-196); `Display::queueText` and `setFont`. The font bank is `FUENTES.ANM`; `queueText` has no font argument, and no line stores one | a console command `text 0xffff HELLO` queues the text and shows it over the frame. Try a long line and a narrow width to see the wrapping |
| 3g. Shadows (needed before the inventory strip, step 9e) | **adds back** `_shades`, `setShadeTable`, the shadow merge in `compose` and the `span.x0 == -1` branch in the row loop.  `queueMaskRows` (206-238), `shade` (239), `mergeMask` (244-426, about 180 lines), `queueScaledRows` (536-576), `setShadeTable` (loads the 32 KB table `TABLA1.TBL` from the archive) and the shadow-span branch of `compose` | nothing visible until step 9e: the one shadow image in the game is `CUADRADO` in `INTERFACE.LVL` (bank 9, image 0, 640 by 100 pixels, an ordinary kind 3 sprite). The inventory strip plays its steps 0..1 when it rises and 2..3 when it lowers, so the band is what darkens the bottom of the picture behind the strip (from the code; not seen on screen). You can do 3g later, but not skip it |
| 3h. Fade | `Display::fadeOut` (lines 48-82): the brightness drops in equal steps. It takes the `Graphics::Screen *` and shows each step itself: copy the frame to the screen, `update`, then wait `delayMs` | a console command `fade 8 40` fades the picture to black in 8 steps of 40 ms |
| 3i. Do not copy | any 5-5-5 support: the game runs in 5-6-5 and nothing ever turns 5-5-5 on. On `codex/remove-hosts` it is gone completely: no `is555` parameter in `AnimImage::load`, `convert` or `scaleBrightness`, `AnimBank::load`, `loadDeferredImages` or `scaleBrightness`, `AnimPlayer::load`, `Level::load` or `reloadDeferredAnims`, no `to555` helper, no `_is555` or `setPixelFormat555` in `Display` and `TextQueue`, and no `World::is555()`. Write those loading calls without the extra argument | |

---

## 4. Level and objects (menu level first)

Files: `core/objectlist.h`, `core/object.*`, `core/camera.*`, `core/room.*`, `core/cameraframe.*`, `core/spriteobject.*`, `core/level.*` (473 lines in the `.cpp`), and `core/animset.*` from step 2.
Needs: steps 2 and 3.

**Goal:** load a level file and draw it with its camera. `PRINCIPAL.LVL`, the main menu, is the best first target: it has one camera frame, one background picture and two menu items.

The pieces:
- A **level** is itself an object. It holds the animation banks (one `AnimPlayer` each, the prototypes that objects copy), the rooms, and every object. A **room** keeps its objects ordered by x. Each object record says its position, name, which banks it uses, its kind, and kind-specific data bytes.
- `Level::createObject` is a factory that switches on the kind. Start it with only the kinds you have (0, 1 and 3) and let the others warn and be skipped; the finished code does the same, so a level loads even if you have not ported its other kinds.
- The **camera** has bounds and a position; each room scrolls with a divisor (parallax), and everything is drawn at `x - camera.scaledX + 0x140`, `camera.scaledY - y + 0xf0` (world Y points up, the screen centre is 320, 240).

| Slice | Where | You see |
|---|---|---|
| 4a. Object lists | No list class: the original's cursor list becomes `Common::Array`. New `core/objectlist.h`, header only, which forward-declares `Object` so it needs nothing from 4b: `typedef Common::Array<Object *> ObjectList` (levels, rooms, moved objects, the pointer's lists), `removeFromList(list, object)` (first match), and `MainList` for the main list: an array of (object, priority) with `insertByPriority`, `insertByPriorityFront`, `remove`, `clear`, `count`, `at`, `priorityAt` and an `edits()` counter that grows on every insert or remove. Walk lists with an index loop. Known difference: the original restarts a pass when its shared cursor moved during a turn, the port when the list was edited (`runUpdatePass`, step 5d); a nested walk of the main list inside a turn would not restart, and the port has none. The pointer's "previous hovered" copy keeps the order of `_hovered` (the original reversed all but the first), which only changes the order the labels are cleared in | a console command that inserts placeholder pointers (addresses of local bytes) with priorities into a `MainList` and prints them in order, equal priorities keeping insertion order (and in front with the Front variant) |
| 4b. `Object` base and `AnimSet` | `Object` (`object.*`), stripped to what this slice runs. Keep: `ObjectRect`; `ObjectRecord` (x, y, sort offset, name, banks, kind, extra bytes, `dataByte(i)` which reads on into the bank indices); the whole `Flags` enum; the virtual destructor; `initPlain`; `initAnimated` (without the `stateSize` parameter); `setActive`, `setVisible`; virtual `updateAnimation` and `updateBounds` (kinds override them from 4e and 4f); the accessors (`flags`, `setFlags`, `x`, `y`, `previousX`, `previousY`, `sortValue`, `setSortValue`, `nameKey`, `hitRect`, `movedArea`, `animation`, a non-virtual `currentPlayer` (it becomes virtual in 5c, for the pointer's companion)); `setPosition`; `room`, `setRoom` (4d needs them); `moveTo` without the call that tells the level (5e). With it, slice 2e: `AnimSet::init`, `current`, `selectBank`, `tick`, `tickClearMovement`. Leave for later: `drawAnimation` and `draw` (4c, they read the camera), `attachToLevel`, `onAttach`, `level`, `attachAll`, `kindId`, `setKindId` (4g), `update` (5c, the pointer's step), `update2` and `runUpdatePass` (5d), `isHitter`, `onTouched` (5e), the pointer calls `setVerbAndClick`, `rightClick`, `acceptsPointer`, `showHoverText`, `clearHoverText` (5g), `verb` (9a), `shiftX`, `shiftY` (9e), `approachX`, `approachY`, `facing`, `exitArrival` (step 6), `saveState`, `loadState`, `stateSize`, `stateBlob` (step 10) | a console command `object NAME.ANM` builds an `Object` from a hand-made record and a bank, ticks it with `updateBounds` and prints the current step, the position and the hit rectangle. It cannot draw yet: drawing needs the camera (4c) |
| 4c. Camera | `core/wrap.h` if you do not have it yet (`wrapMul`, `wrapAdd`, `wrapSub`), then `Camera` (`camera.*`, 150 lines): `init`, `setBounds`, `setPositionClamped`, `moveTo` (timed), `updateToward(divisor, now)` which sets `scaledX`/`scaledY`. It needs `Object::x()` for its target, so it comes after 4b. With it, add `Object::drawAnimation` and the virtual `draw` (they position the image from `scaledX`/`scaledY`) | the 4b test object is drawn every frame through `_display` (this replaces `playanim`'s direct drawing); arrow keys (or a console `camera x y`) scroll it across the view, and it stops at the bounds |
| 4d. Room | `Room` (`room.*`, 50 lines): `init`, `insertSorted` (finds the first object further left and `insert_at`s in front of it), `removeObject` (`removeFromList`); sort offset and camera divisor | two test objects at different x end up in x order |
| 4e. Camera frame | `CameraFrame` kind 1 (`cameraframe.*`, 57 lines), without a level: `init` (reads the record: the walk-floor object's 12-byte name `_targetName`, `count` = byte 0xd + 1, `flag`, `value`), the accessors, and `frameCamera(Camera &)`: the rectangle of the first step (`hitX`/`hitY`/`hitW`/`hitH` plus the frame's position) becomes the camera's bounds, and the camera moves to the frame's position. `frameCamera` is the camera half of the original's `onAttach`, split out so it runs without a `Level`. Leave for later: `onAttach` (it looks the target up with `Level::findObject` and calls `frameCamera`, 4g), `update` (it only returns 1, step 5d), `target()` while there is no level to search | a console command `cameraframe NAME.ANM X Y` builds a `CameraFrame` from a hand-made record whose bank has a hit rectangle on its first step, calls `frameCamera(_camera)`, and prints the camera position; `camera 99999 99999` then ends at the rectangle's corner |
| 4f. Sprite | `SpriteObject` kind 3 (77 lines): the first two data bytes are the step to start on and a mode (0 holds that step hidden, 1 shown, 2 plays it on to the step without an image, 3 loops that run); `init`, `updateAnimation`, `draw`. `init` calls `AnimBank::clearStepMovement(step)` (it zeroes one step's movement), so add that to `AnimBank` in this slice (it was in 2f). Leave `onAttach` (it puts the sprite in its room's list, which needs the level: 4g) | the test object of 4c becomes a `SpriteObject`: `object NAME.ANM FIRST MODE` builds it from a record whose data bytes are `FIRST` and `MODE`, and it is drawn through the camera as before. Mode 0 draws nothing (hidden), 1 holds the step, 2 plays on to the step without an image, 3 loops |
| 4g. Read the level file | `Level` (a subclass of `Object`; `load` calls `initPlain`) and `Level::load` (`level.cpp` 53-133, in `kLoadNow` mode): header (room count, bank count), then the banks one after the other (each `AnimBank::load` starts where the last ended), then for each room its record (object count, extent, camera divisor) and each object record (x, y, sort offset, a 12-byte name, bank count, kind, data size, then the data bytes and bank indices). `createObject` with kinds 0, 1 and 3 only, `findObject(nameKey)`. `load` also sets the default camera bounds (-320, 239, 319, -240), remembers the kind 1 object (`_special`), and keeps every object it builds in `_owned` so the level's destructor can delete them. Now that a level exists, add `Level::camera()`, `Object::attachToLevel` (it puts an object flagged `kInRoomList` into its room's list, then calls `onAttach`), `level()`, the virtual `onAttach`, `attachAll` and `Level::attachObjects` (run after the objects are built, in list order), `SpriteObject::onAttach` (puts the sprite in its room's list) and `CameraFrame::onAttach`: it finds its `target` with `findObject`, then calls `frameCamera(_level->camera())` | a console command `level NAME.LVL` prints rooms, banks and a count per kind. Expect `PRINCIPAL.LVL`: 1 room, 2 banks, k1:1 k3:1 k25:2. `NUEVO.LVL`: 13 banks, k1:1 k3:12 k25:3. `TABERNA.LVL`: 26 banks, k1:1 k3:7 k5:3 k6:1 k7:1 k8:2 k9:3 k12:3 k16:3. `INTERFACE.LVL`: 32 banks, with k13:46. (Those counts are from my tally with the finished loader.) After loading `PRINCIPAL.LVL` the camera bounds equal its frame's rectangle |
| 4h. Draw the level | `Level::updateRoomCameras` (445-462): for each room, move its camera, then for every visible and active object that is not already processed call `updateBounds` and `draw`. The engine's frame is: `startFrame`, `updateRoomCameras`, `compose`, copy to the screen | `level PRINCIPAL.LVL` shows the title picture. `level TABERNA.LVL` shows the tavern with its window and doorway as holes onto `FONDO2`, the view outside |
| 4i. Later in this step | `Object::saveState`/`loadState` (step 10), the main-list passes (`runPass`, `runUpdatePass`, `runUpdate2Pass`, in step 5) | |

---

## 5. Pointer and the menu

Files: `core/mouse.*`, `core/inputstate.*`, `core/keyboard.*`, `core/hovertext.*`, `core/cursor.*` (340 lines), `core/collision.cpp`, `core/outline.*` (325 lines), more of `core/level.cpp`, `core/menuitem.*`, and the engine's event handling (`tesoro.cpp`: `handleEvent`, `updateKeys`, `runFrame`, `hookMenuItems`, `enterLevel`, `startGame`, `fadeOut`).
Needs: steps 3 and 4 (the text queue for the hover label, the level for the menu).

**Goal:** hover over and click the menu buttons. `PRINCIPAL.LVL` has two `MenuItem`s and `NUEVO.LVL` three. A menu item's data number says what a click does: 0 fades to black, enters `NUEVO.LVL` and starts the script `FANMMENU`/`nuevo`; 1 fades and opens the original's saved-games screen; 2 and 4 start the scripts `FANMMENU`/`facil` and `dificil` (easy and hard); 3 fades and starts the game.

| Slice | Where | You see |
|---|---|---|
| 5a. Input | `InputStates` (62 lines): per-button state with a release delay and a repeat delay; `Mouse` (48 lines): position and three buttons, `update(down, now)`; `Keyboard::fromHost` maps host keys to the keys the game watches; the engine's `handleEvent`, `updateKeys`, and `_buttonsDown[3]` | a debug print of mouse position and button states each frame; press and release show with their delays |
| 5b. Hover text | `HoverText` (51 lines): the label next to the pointer (up to 160 bytes) and its colour; drawn with `queueText` from slice 3f | `hover HELLO` shows a label next to the pointer that follows it |
| 5c. Interface level and pointer picture | the `Level::createObject` case for kind 10, then load `INTERFACE.LVL` through the engine (its other kinds warn and are skipped) and the basic `Cursor` (`cursor.cpp`, a `SpriteObject`): `initMain` (the pointer picture is a sprite object of kind 10), `init(ObjectList *previous)` for its plain companion in the current level, `configureMouse`, `updateAnimation`, `onAttach` (adds `AnimBank::clearStepHitRect`), `draw` (the picture, and the hover label with `queueText`), `setSurroundings(mouse, level)`, and a short `update`: only its last lines, which put the pointer at the mouse and the companion in the current level (`moveTo` as in 4b). That makes `Object::update` virtual and `Object::currentPlayer` virtual (the companion borrows the main pointer's). The engine's ad-hoc frame calls `_cursor->update()` until 5d. Leave for later: the page and menu-key branches of `update` (they ask `World`: step 7, with 9e), `update2` and `processCandidates` (5g), `setHost`, `setStrip`, `shiftStrip`, `pageLeft`, `pageRight`, `scrollOut`, `openMenu` (inventory and saves, steps 9 and 10) and `sampleScene` (step 10) | the pointer picture follows the mouse over the menu |
| 5d. The main list and the frame | `MainList _main` (from 4a; priority order: scripts, the pointer, the interface level, the current level), kept as a member of the engine until 7b moves it into `World`. `runUpdatePass` and `runUpdate2Pass` (`object.cpp`: the first pass runs every object's `update`; the second runs only if the first did not ask for a state change), with `Object::update2`. `Level::update` (it returns 1 for now; 5e adds the collision pass) and `Level::update2`. The second step composes the picture: in the finished code `WorldLevel::update2` (7d) calls the engine's `levelSecondStep`, so until then the engine builds its levels as a small `MenuLevel : Level` whose `update2` does that (`levelSecondStep`, `tesoro.cpp` 205-217: `updateRoomCameras`, then compose and copy to the screen, except for the interface level). And `TesoroEngine::runFrame` without its `_world` calls (events, keys, mouse, clock freeze, `startFrame`, the two passes, clock unfreeze). This replaces the ad-hoc frame of step 4h | a counter printed in the console shows each object's `update` called once per frame; the picture is unchanged |
| 5e. Rectangle collision | `core/wrap.h` if you do not have it yet, then `sweepRects` (`collision.cpp`, 130 lines: when two moving rectangles first touch; it uses `wrapMul`), `Level::collisionPass` (197-263) with `Level::update` calling it, `Level::resolveOverlap` (345-383) **without** its outline refinement (`shapeOf`, `outlineOf` and `shapedContactTime` are 5f) and without the depth-100 rule for the player (`_player`, 6b), `Level::objectMoved`, `_movedObjects` and `_touches`; the call to `objectMoved` that 4b left out of `Object::moveTo`; the virtual `Object::isHitter` and `onTouched`; and in `Cursor`, `isHitter`, `onTouched` and the `_touched` list | `touching` prints the name of any object under the pointer; with only menu items it names them as you hover |
| 5f. Pixel-accurate collision | `Outline` (`outline.cpp`: `contains`, `clipColumns`, `overlaps`), `Level::shapeOf`, `outlineOf`, `shapedContactTime` (277-344), and the refinement lines of `resolveOverlap` that 5e left out. Floors and walk areas use it, so you can wait for step 6 | not needed for the menu |
| 5g. Hover and click | `Cursor::update2`, `setActive`, `processCandidates` (190-340), `clearTouched`, and the label (`HoverText`): the object that is newly under the pointer gets its label, one that stays under it gets the click, one that was left gets its label cleared. Add the virtual `Object::setVerbAndClick`, `rightClick`, `acceptsPointer`, `showHoverText` and `clearHoverText` (the defaults do nothing) that 4b left out | hovering a button prints or shows its name; clicking calls its click handler |
| 5h. Menu items | the `Level::createObject` case for kind 25, then `MenuItem` kind 25 (111 lines; its host is the engine, `TesoroEngine *`, not `World`): `init`, `updateAnimation`, `draw`, `update`, `setVerbAndClick` (the action number above), `rightClick` (does what a left click does); and the engine side: `hookMenuItems`, `enterLevel` (the two menu levels stay loaded as `_mainMenu` and `_newGameMenu`), `fadeOut`, `clearHoverText`, `startGame`, `openSavedGames`, `notPorted`. Stub what you do not have: the scripts (step 8), `startGame` (steps 7c and 8) and the saved-games screen (steps 9h and 10) | **milestone: click the first menu button and the screen fades to black and `NUEVO.LVL` appears with its three buttons** (the script it starts and the other actions just warn "not ported" for now) |

---

## 6. Floors, Jim and walking

Files: `core/walkfloor.*`, `core/pathobject.*`, `core/walkarea.*`, `core/jimwalker.*` (379 lines: `JimWalker` and `Jim`), and the walking helpers in `world.cpp` (705-777).
Needs: steps 4 and 5 (a level with a floor, the pointer to click, outlines from 5f).

**Goal:** Jim stands in a level at the right size, walks to where you click, and goes round obstacles.

Floors and walk areas are shapes stored as **points inside an animation step** (type 4: each point has x, y and a scale, so Jim shrinks far away). The route round obstacles is the corners of such a shape.

| Slice | Where | You see |
|---|---|---|
| 6a. Floor and path objects | `WalkArea` (`walkarea.*`) first part only: the `WalkTarget` base, `setShape`, `setPosition` and `scaleAt` (the rest needs `JimWalker`, 6c and 6d). `WalkFloor` kind 7 (92 lines): `init`, `scaleAt`, and in `onAttach` only the lookup of its path object by name; leave the shared floor of the island levels (levels 0x17 to 0x2b: `Level::sharedFloorLevel`, `AnimSet::shareFrom`, `CameraFrame::setValueCount`, `Level::index`, which come with 7a and 7d). `PathObject` kind 8 (63 lines) without `area(JimWalker &)` (6d). The factory cases for kinds 7 and 8, `Level::floor()` (through `CameraFrame::target`) | `scale x y` in the console prints the size factor at a point of the tavern floor (256 is life size, which is the value for `0x100`) |
| 6b. Jim's banks and picture | start `world.h` here as a small `World` class holding only what Jim asks for (his `_world` calls: `now`, `archiveStream`, `insertInMainList`, `insertInMainListFront`, `removeFromMainList`, `setHoverText`, `randomNumber`, and the walk-area calls of 6c and 6d); each later slice adds what it needs. `Jim` (`jimwalker.cpp` 150-352) **without** `SpeakerHost`: the interface and everything that implements it (`placeSpeech`, `setPose`, `queueFrame`, `drawSubtitle`, `interactiveDraw`, the voice calls) come with 8d. Keep: the nine `JIM*.ANM` banks in the order of `Jim::Bank` (listed in `kJimFiles` in `world.cpp`), `init`, `enterLevel`, `updateBounds`, `draw` (scaled by the floor's factor through `queueImageScaled`), `AnimBank::makeSinglePoints` on the walking banks, `AnimSet::switchTo`, `JimWalker` with only `setFacingValue`, `facing` and `scale` (the walk is 6c), `isHitter`. (Jim's `loadDeferredImages` and `scaleBrightness` lines wait for 7a.) | `jim x y` puts Jim in the tavern at the right size, standing |
| 6c. Straight walk | `JimWalker::walkTo`, `update`, `setFacing` (lines 38-149): fixed-point position, the animation's own movement kept on the straight line to the target, facing chosen by the longer distance; `Jim::startWalkAnimation`, `advanceAnimation`, `tickAnimation`, `showFacing`, `AnimSet::switchPlayingTo`; `World::walkJimToPointer` | **click on the floor and Jim walks there, facing the way he goes, stops, and turns to stand** |
| 6d. Walk area route | the rest of `WalkArea` (it calls `JimWalker::walkTo`, so it comes after 6c): `findPath` (first to the corner nearest Jim, then corner by corner to the one nearest the destination, then the destination) and `arrived`; `PathObject::area(JimWalker &)` and `World::hasWalkArea`, `scaleAt`, `findPath` | `route x y` makes Jim walk the corners of the walk area to the point, one corner at a time |
| 6e. Round the corners | `JimWalker::touched`: when Jim touches the walk area's edge a straight walk is replaced by the route of 6b; needs `Outline` collision from 5f | in the tavern, click behind the table: Jim walks round it |
| 6f. Jim as a clickable object | `Jim::showHoverText`, `setVerbAndClick`, `rightClick`, `clearHoverText`, `useItem` | hovering Jim shows his name |

---

## 7. World hub

Files: `world.h`, `world.cpp` (954 lines), `core/level.*` (factory), `tesoro.cpp`.
Needs: steps 4 to 6.

**Goal:** one object that owns the game's levels, Jim, the pointer and the lists, and that every object can ask for what it needs. Write it **last and in pieces**: give each object only the methods it actually asks for, and add the rest as the later kinds need them.

| Slice | Where | What it holds |
|---|---|---|
| 7a. The list of levels | the 47 names in `kLevelNames` (`world.cpp` 55-65: the order scripts and objects number them), `loadLevelFile`, `levelIndexOf`, `level(index)` (loads a level the first time it is asked for and keeps it with whatever state the game left in it), `applyStartState` | `level N` in the console loads a level by number |
| 7b. The main list helpers | `insertInMainList`, `insertInMainListFront`, `removeFromMainList`, `mainList()`, `now()`, `setPointerActive` | objects join and leave the frame loop |
| 7c. Jim's hosting | `jim()`, `jimLevel`, `jimEnterLevel`, `jimMoveTo`, `jimShow`, `jimFace`, `jimWalkTo`, `jimUpdate`, `enterLevel`, `startGame` | Jim can move between levels |
| 7d. The level factory | `WorldLevel` (a `Level` subclass, `world.cpp` 71-156) and `World::createObject` (381-476): the kinds above 3 (2, 4, 5, 6, 9 to 17, 19 to 24) are built here; `Level::createObject` keeps 0, 1, 3, 7, 8, 10 and 25 | every kind a level contains is built or warns clearly |
| 7e. The rest, as needed | speaking and sound helpers (553-704), the pointer and what it carries (805-841), scripts, scene changes and movies (842-954), `captureThumbnail`, the save-slot helpers | grows with steps 8 to 10 |

---

## 8. Speech, sound and scripts

Files: `core/stdiotext.*`, `core/speechtable.*`, `core/sounddevice.*`, `core/speaker.*`, `core/scriptblock.*`, `core/scriptrunner.*`, `scripts.h` (the `Scripts` class, 355 lines), `core/scenes.*`, and `World::startMovie`.
Needs: step 7 (the `Scripts` class asks `World` for objects, levels and Jim).

**Goal:** the opening scene runs; a spoken line plays with its subtitle; a conversation shows its choices.

| Slice | Where | You see |
|---|---|---|
| 8a. Speech tables | `StdioText` (160 lines: a small `sscanf` that reads the original's number and string formats) and `SpeechTable`/`SpeechTables` (110 lines): the four files `OBJETOS.TXT`, `ANIMS.TXT`, `CONVERS.TXT`, `ICONS.TXT`, each line `ID, "text"`; the voice of a line is the archive entry `ID.WAV`; lookup by a 64-bit name key | `say ID` prints the text and the voice's archive offset |
| 8b. Voices | `SoundDevice` (`sounddevice.cpp` 31-107): up to three sounds at once, each with an id; the WAV starts at an archive offset with a 44-byte header; `update` drops finished sounds; the `Voices` holder in `world.h` | `voice ID` plays a line |
| 8c. Music | `CdAudio::playTrack` through ScummVM's audio CD support (the tracks are files such as `track04.flac` next to the game files) | `music 4` plays track 4 |
| 8d. Speaking | `Speaker` (112 lines: `say`, `update`, `talkAnimate`, `drawJim`, `drawCharacter`, `clearSpeech`) and `Jim::placeSpeech`, `setPose`, `queueFrame`, `drawSubtitle`, `interactiveDraw`; the voice, mouth frames chosen at random, the skip key (Escape or the right button), the subtitles option. `SpeakerHost` stays an interface because `Jim` and characters both implement it | `jimsay ID`: Jim's mouth moves, his subtitle shows over his head and the voice plays |
| 8e. Read the scripts | `ScriptBlock::parse` (`scriptblock.cpp` 123-303: the characters that speak in a block, its commands as 24-byte entries, its labels) and `ScriptLibrary` (`load`, `find`, `resolve`) over the four files `ANIMS.DLG`, `CONVERS.DLG`, `ICONS.DLG`, `OBJETOS.DLG` | `scripts` prints the block and label counts (the notes record 145 blocks with 452 labels) |
| 8f. Run a script | `ScriptRunner` (`scriptrunner.cpp`: `start`, `resume`, `execute`, `tick`, `collectMenu`, `tickMenu`, 328 lines), `ScriptTask` and the `Scripts` class (`scripts.h`: what a running block asks of the game), `ChoiceMenu` (the list of choices, drawn with the text queue) | `run BLOCK LABEL` starts a block; a block that only says lines and sleeps plays out in order |
| 8g. Scene routines | `SceneRoutines` (`scenes.cpp`, 245 lines: the native routines behind the numbered blocks' `DO` lines) and `SceneAdapter` in `scripts.h` (their view of the game); the board game's two routines wait for step 9f | the opening scene (port, Jim, the first conversation) runs to its end |
| 8h. Movies | `World::startMovie` (883-927): the movies are Indeo 5 AVI files in the disc's `bin/dataNN.bin`, played with `Video::AVIDecoder`, centred, until the end or Escape is let go; `releaseInputAfterMovie` | `movie 1` plays an intro movie (check that ScummVM's Indeo 5 support is built in) |

---

## 9. Remaining object kinds (any order that suits; this order follows what unlocks what)

Counts below are the number of objects of the kind in all shipped levels (my tally).

| Slice | Kinds and files | Size | In the game | You see |
|---|---|---|---|---|
| 9a. Clickable objects | `InteractiveObject` kind 9 (`interactive.cpp`, 164 lines: verbs, hover name, `answer`, `useItem`) | 164 | 124 objects | hover a tavern object (the notice, the spittoon) and click it: Jim answers with his default line |
| 9b. Characters | `Character` (140) and `StoryCharacter` kind 5 (315: a lot of per-character story code, 27 objects) | 455 | 27 | talk to a tavern character and a conversation starts |
| 9c. Exits | `ExitDoor` kind 6 (219) | 219 | 75 | click a door: Jim walks to it, the screen fades, and the next level appears |
| 9d. Story props | `ScriptedProp` kinds 12, 17 and 20 (646 lines: a table of subtypes) | 646 | 12 + 30 + 44 | props react to verbs and story state |
| 9e. Inventory | needs slice 3g (the shadow band `CUADRADO`) first. `InventoryItem` kind 13 (381, 46 in the interface level), `InventoryStrip` kind 11 (153), `ItemGroup` kind 2 (48), `ClickCatcher` kind 15 (47), and the pointer's `shiftStrip`, `pageLeft`, `pageRight`, `scrollOut` | about 680 | interface level | the strip rises when the pointer goes to the bottom, an item sticks to the pointer |
| 9f. Map and notes | `MapPlace` kind 19 (83, 7 on the map), `NoteObject` kind 24 (42, 7 levels) | 125 | 14 | the map shows places and a click travels |
| 9g. Board game | `BoardColumn` kind 21 (65), `BoardGame` (88), `BoardPieces` (70), `ConnectFour` (142) and the game's two scene routines (`DO` lines of types 8 and 14) | 365 | `TABLERO.LVL` | play the connect-four board |
| 9h. Menu buttons | `MenuButton` kind 22 (92) and `SlotLabel` kind 23 (71), the original's save and load screens | 163 | interface level | the screen shows the slot labels (their actions come with step 10) |
| 9i. Decoration | `PathSprite` kind 16 (169; the three tavern walkers, which need the "Enable cut content" option to ever walk), `DepthZone` kind 14 (68) and `ShadowSprite` kind 4 (49) | 286 | 3, none, none | the tavern walkers pass behind the window |

---

## 10. Saves

Files: `World::saveGame`/`loadGame` (`world.cpp` 323-380), `saveState`/`loadState` in `Object`, `AnimSet`, `AnimPlayer` and `Jim`, `Level::saveRecords`/`loadRecords` (415-444), `World::captureThumbnail`, the slot helpers (587-704), and `TesoroEngine::saveGameStream`/`loadGameStream`.
Needs: steps 7 to 9 (the state to save exists).

The file keeps the layout of the original's: a thumbnail (every third pixel of every third row), the level Jim is in, Jim's state, then a count and, for each level visited, its object states and its records, then the interface's records.

| Slice | Where | You see |
|---|---|---|
| 10a. Thumbnail | `Cursor::sampleScene` (106-111), `World::captureThumbnail`, `setSlotPicture`, `showSlotPicture` | the save picture of the current scene is captured |
| 10b. Animation state | `AnimPlayer::saveState`/`loadState` (first and last step and only the low bytes of the current step and flags), `AnimSet::saveState`/`loadState`, `Object::saveState`/`loadState`, and the `pause`/`resume` from step 2 | an object's animation survives a round trip through a memory stream |
| 10c. Levels and Jim | `Jim::saveState`/`loadState`, `Level::saveRecords`/`loadRecords` (object states by kind), per-kind state blobs | saving and loading in the same scene leaves it unchanged |
| 10d. The engine hooks | `World::saveGame`/`loadGame`, `TesoroEngine::saveGameStream`/`loadGameStream`, `runPendingLoad`, and the save list ScummVM shows (the thumbnail and the slot names) | ScummVM's save and load dialogs work in game |
| 10e. The original's screens | `MenuButton` and `SlotLabel` actions (`previousSlot`, `nextSlot`, `loadSlot`, `saveSlot`), `slotTime`, `Cursor::openMenu` | the original's save screen lists and writes slots |

---

## After the port is complete

- **Paint everything instead of queuing spans** (untried, optional): see `re/RENDER_SIMPLIFICATION.md`. It could remove `renderqueue.*` if a
  side-by-side comparison of recorded frames shows no difference.

---

## Notes
- The branch has no accessors that only the comparison drivers used. Do not look for `AnimPlayer::startTime`, `totalDuration`, `loopX`, `loopY` or
  `pausedAt` (the player keeps those as private fields and reads them itself), nor `JimWalker::fixedX`/`usedMoveX`, `Cursor::companion` or
  `SoundDevice::lastId`/`activeCount`. Write only the accessors a caller needs.
- No class has a host interface any more. Objects hold a `World &` (or `World *`) and call it directly; scripts call `Scripts` (`scripts.h`).
  `SpeakerHost` is the one interface left, because both `Jim` and characters implement it.
- `core/wrap.h` holds `wrapMul`, `wrapAdd` and `wrapSub`: 32-bit arithmetic that wraps like the original's (a plain signed operation would be undefined behaviour on overflow). It replaces three private copies the code used to have (`mulw` in the render queue, `mul` in collision, and the set in the camera). Copy it before the first file that needs it: step 3e, 4c or 5e, whichever comes first.
- Random numbers: use a `Common::RandomSource` in the engine (`randomBetween(min, max)`); there is no `core/random.*` to copy.
- Option "Enable cut content" (config key `cut_content`, on by default). It needs four pieces: the flag `GAMEOPTION_CUT_CONTENT` in `detection.h`, the
  game entry listing it in `detection_tables.h`, the entry in `metaengine.cpp`'s option list, and `World::cutContent()`. Only `PathSprite`'s wait test
  reads it (step 9): on, the wait is measured in seconds; off, it is the original's test, which never lets the three tavern walkers start.
  A game added to ScummVM before the flag existed must be re-added to show the option; the default is on.
- Match the original's behaviour first; the contracts mark open questions and port deviations.
- Do not add test hooks to the engine (`AGENTS.md`). Compare by running the game.
- Copy function by function, in the order above, and build after each.
