# Porting order for the rewrite in `tesoro-scratch`

Source to copy from: branch `codex/remove-hosts` (worktree `/Users/gabriel/Desktop/source/scummvm-tesoro-nohosts`).
Not the history, and not `codex/object-kinds` (it still has the host interfaces).
Keep the matching `re/contracts/<address>.md` open while copying a function: it says what must not change.
Sizes are approximate line counts of the `.cpp`/`.h` pair on that branch.

Rule for every step: copy one slice, make it build, check the small result named under "You see", then go on.
The order follows the dependency chain in `re/SUBSYSTEMS.md`: each step only needs the ones above it.

---

## 1. Files and time (about 400 lines)

Files: `core/namekey.*`, `core/archiveindex.*`, `core/archive.*`, `core/clock.*`. Random numbers are ScummVM's `RandomSource` (no `core/random.*`).
These depend on nothing else in the engine, so they come first.

**Goal:** open `TESORO.TRE` and `TESORO.DAT`, find an entry by name, and keep game time.

| Slice | Files | You see |
|---|---|---|
| 0. Wiring | `tesoro.cpp` and `console.cpp` (in `engines/tesoro`, not `core/`): give `TesoroEngine` an `Archive` member, open `TESORO.DAT`/`TESORO.TRE` at the start of `run()` (replacing the template's sample boxes and palette cycling), and add console commands. Nothing calls `Archive` yet, so this is needed before any later check can be seen | console `entries` lists the first entries from `index().count()` and `entry(i)`; `entry NAME` prints the offset from `findEntry` |
| Name keys | `core/namekey.*` | hash a few names |
| Archive index | `core/archiveindex.*` (in progress) | look up one known entry and print its offset. Records are kept only when flag bit 4 is set; names are upper-cased and sorted largest first, which is what `find()` searches |
| Archive | `core/archive.*` | seek to an entry in `TESORO.DAT` |
| Clock | `core/clock.*` | game time ticks. The clock is first needed by `AnimPlayer` (step 2, slice d). Random numbers: use a `Common::RandomSource` in the engine; there is no `random.*` to copy |

---

## 2. Animations (about 1,300 lines for the whole layer; the first picture needs about 550)

Files: `core/animimage.*`, `core/animbank.*` (337 lines in the `.cpp`, which holds both `AnimBank` and `AnimPlayer`), `core/animset.*`.

**Goal:** load one animation, show a picture from it, then make it play over time.

The four classes, and how they depend on each other:
- `AnimImage` is one picture, kept as the blob stored in the archive. It does **not** expand the spans: it only reads the blob and converts the
  pixel format in place (5-5-5 or 5-6-5). The spans (runs of pixels per row, stored as RGB565) are expanded only when the picture is drawn, in
  `RenderQueue::rasterImage` (step 3).
- `AnimBank` is the data of one animation file: its sequences (steps: image, offset, duration, hit rectangle) and their images. It is shared.
- `AnimPlayer` is playback state over a bank: which sequence is current, when it started, loop and pause flags, and the movement the current
  sequence gives the object. The bank is held through a shared pointer, so copies of a player share the bank and keep their own state.
- `AnimSet` is the animations of one game object: one `AnimPlayer` per bank, of which one is current.

| Slice | Where | You see |
|---|---|---|
| a. Image blob | `AnimImage::load`, `parse`, `convert` and the small accessors (`core/animimage.cpp`, about 100 lines). Leave `scaleBrightness` and `BrightnessTable` for later | nothing yet: `AnimImage::load` reads a size-prefixed blob at the stream's current position, and the only thing that knows where an image sits is the bank file (slice b). Copy it, make it build, and check it with slice b |
| b. Bank file | `AnimBank::load` (lines 38-140 of `animbank.cpp`): header (sequence and image counts, header flag, three header dwords), the sequences, then the images through `AnimImage::load`. Load in `kLoadNow` mode only. Remember the data starts 4 bytes after the archive offset | with the wiring from step 1 (slice 0): a console command `bank NAME` that prints the sequence count, image count and, for each image, its type, span count and pixel size |
| c. First picture | a throwaway loop that walks one image's spans into a `Graphics::Surface`. Use `RenderQueue::rasterImage` (renderqueue.cpp line 466) as the model, but copy only the unscaled path | **first milestone: a real picture from the game in the window** |
| d. `AnimPlayer` | see below | the picture animates |
| e. `AnimSet` | see below | an object owns several animations and switches between them |
| f. When a caller asks | `AnimBank::makeSinglePoints` (Jim's walking banks), `clearSequenceMovement`, `clearSequenceHitRect`, `loadDeferredImages` and the `kLoadDeferred` mode, `scaleBrightness` (needs the `BrightnessTable` in `animimage`), `AnimPlayer::pause`/`resume`, `AnimPlayer::saveState`/`loadState`, `AnimSet::shareFrom`, `stash`/`restoreStashed`, `switchPlayingTo` | not needed for the first scenes |

### When to add `AnimPlayer`

Add it right after the first picture works (slice c) and **before** the screen queue (step 3). Three reasons:

1. **Nothing in the engine uses a bare `AnimBank`.** `AnimPlayer::load` is what creates the shared bank, `Level` keeps its banks as an array of
   `AnimPlayer` (`Level::banks()`), and every object copies players from that array. If you build the draw loop on `AnimBank` directly, you
   will have to redo it when `Level` arrives in step 4.
2. **It is the first thing that needs the clock.** `setRange` and `advance` take the game time (`now`), so pull in `core/clock.*` from step 1 at
   this point if you haven't yet.
3. **It is what makes the picture move.** Copy it in this order, each with a visible check:

| Order | Function (`animbank.cpp`) | You see |
|---|---|---|
| 1 | `AnimPlayer::load` (line 184) | slice c now draws through a player, same picture as before |
| 2 | `AnimPlayer::setRange` (194-242): plays sequences `first..last`; a `last` of -1 extends to the next sequence without an image | print the range and total duration for one animation |
| 3 | `AnimPlayer::advance` (243-310): moves the play head to the sequence the game time has reached and updates the movement | the picture steps through its frames in the window |
| 4 | the accessors in the header (`current`, `first`, `last`, `movementX`, `movementY`, `flags`) | enough for step 4's objects |
| later | `pause`/`resume` (326-336): needed when the game pauses or a scene holds the clock. `saveState`/`loadState` (312-325): step 10 | |

### When to add `AnimSet`

Add it at the start of step 4, together with `core/object.*`. `Object::initAnimated` builds an `AnimSet` from the level's banks, one player per
bank the object uses. Copy `init`, `current`, `selectBank`, `tick` and `tickClearMovement` first. `switchTo`/`switchPlayingTo`, `stash`/`restoreStashed`
and `shareFrom` wait for the objects that need them (Jim's walk, speech, and the shared floor of the labyrinth levels).

You don't need the whole layer for the first picture: slices a, b and c (about 550 lines, with the clock arriving at slice d) are enough.

---

## 3. Screen (about 900 lines)

Files: `core/renderqueue.*` (about 650 lines, copy by function), `core/screendisplay.*`, `core/display.h`.

**Goal:** draw several pictures in the right order in a ScummVM window. This replaces the throwaway draw loop of step 2 slice c; the span expansion you copied there is the unscaled part of `RenderQueue::rasterImage`, so now copy the scaled and sheared paths too.

| Slice | Files | You see |
|---|---|---|
| Screen queue | `core/renderqueue.*` | several pictures sorted by depth |
| Compose | `core/screendisplay.*`, `core/display.h` | the queued spans turned into pixels each frame |

---

## 4. Level and objects (menu level first)

Files: `core/level.*` (about 470 lines, copy by function), `core/room.cpp`, `core/camera.*`, `core/cameraframe.*`, `core/object.*`,
`core/outline.*`, then the simple kinds `core/spriteobject.cpp`, `core/walkfloor.*`, `core/pathobject.*`, `core/menuitem.*`.

**Goal:** load a level and draw it with its camera. The main menu level (`PRINCIPAL.LVL`) is the easiest one to try, because it needs
only a few object kinds.

| Slice | Files | You see |
|---|---|---|
| Base object | `core/object.*` | an object holds a bank and draws |
| Room, camera | `core/room.cpp`, `core/camera.*`, `core/cameraframe.*` | the camera follows |
| Simple kinds | `core/spriteobject.cpp`, `core/walkfloor.*`, `core/pathobject.*`, `core/menuitem.*` | menu pictures appear |
| Level | `core/level.*`, `core/outline.*` | `PRINCIPAL.LVL` loads and draws |

The object classes mirror the original's 25 object kinds. Keep them as separate classes.

---

## 5. Pointer

Files: `core/mouse.*`, `core/inputstate.*`, `core/cursor.*`, `core/collision.cpp`, `core/hovertext.*`.

**Goal:** hover and click on the menu. Menu clicks call `TesoroEngine` directly (the old `MenuHost` is gone).

---

## 6. Jim and walking

Files: `core/jimwalker.*` (Jim and `JimWalker`), `core/walkarea.*`, `core/speaker.*` (this keeps `SpeakerHost`, which both Jim and
characters implement).

**Goal:** Jim walks to a click and turns to face the way he went.

---

## 7. World hub

Write `World` last, in pieces. Give each object only the methods it actually asks for, and add the rest as the later kinds need them.
Objects reach the rest of the game through `World`; scripts reach it through `Scripts`.

---

## 8. Speech and scripts

Files: `core/speechtable.*`, `core/scriptblock.*`, `core/scriptrunner.*`, `scripts.h` (the `Scripts` class), `core/scenes.*`,
`core/sounddevice.*`.

**Goal:** the opening scene runs; a line is spoken with its subtitle.

---

## 9. Remaining object kinds (any order)

`character`, `storycharacter`, `interactive`, `scriptedprop`, `inventoryitem`, `inventorystrip`, `exitdoor`, `itemgroup`, `shadowsprite`,
`depthzone`, `clickcatcher`, `pathsprite`, `mapplace`, `noteobject`, `menubutton`, `slotlabel`, and the board game (`boardcolumn`,
`boardpieces`, `boardgame`, `connectfour`).

---

## 10. Saves

`World::saveGame`/`loadGame` plus each class's `saveState`/`loadState`. Keep the file layout as it is: it matches the original's save.

---

## Notes
- Match the original's behaviour first; the contracts mark open questions and port deviations.
- Do not add test hooks to the engine (`AGENTS.md`). Compare by running the game.
- Copy function by function, in the order above, and build after each.
