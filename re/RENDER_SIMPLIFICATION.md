# Idea for after the port: paint everything instead of queuing spans

Status: **untried**. Do this only after the whole engine is ported and playtested. It is an optional simplification, not a fix.
Written on 7 Nov 2026 from reading the code of `codex/remove-hosts`; nothing here has been run.

## The question

The finished engine draws unscaled images by queuing spans (`RenderQueue`, then `Display::compose`) and only paints scaled literal images
straight into the frame (`rasterScaledImage`). Could one simpler path, painting every image, give the same picture?

## Why the original queues spans

1. **Each visible pixel is written once.** A scene is a full-screen background with sprites in front. Painting every image in turn writes the
   hidden background pixels and then overwrites them. At 640x480 in 16-bit on a mid-1990s PC that overdraw was expensive.
2. **Draw order does not matter.** Objects submit images in whatever order the lists run. `Row::insert` resolves depth (smaller is nearer) when
   a span is inserted, so nothing has to be sorted first.
3. **Shadows.** A shadow darkens only what is behind it, between a lowest and a highest depth (`queueMaskRows`, `mergeMask`). The queue knows
   which span is under which.

## Why painting everything is probably equivalent

On modern hardware overdraw costs nothing. Either of these should give the same final frame:
- **Sort and paint:** sort all images by depth, farthest first, and paint in that order.
- **Depth buffer:** keep a depth per pixel and write a pixel only if it is nearer than what is there.

What would go away: `core/renderqueue.*` (about 650 lines of `.cpp`, plus the header), the per-row span pool, and most of `Display::compose`.

## What must match (these are the traps)

- **Equal depths.** In `Row::insert` a new span wins only if `depth < node.depth`, so for equal depths the span queued first stays in front.
  Sort-and-paint must draw equal depths in reverse queue order, or the depth buffer must use a strict "nearer" test.
- **The two-phase compose.** Scaled images deeper than the layer depth are painted before the queued rows, the rest after, on top. The layer
  depth is set by the text call (to its tag), so this is not a pure depth order. A painter's version has to reproduce the same rule.
- **Shadows.** Each shadow only darkens a range of depths. Painting needs "darken what is already there if its depth is in this range", which
  needs a depth value for each pixel. The shipped data has exactly one shadow image (type 2) and no shadow object (kind 4), so this may never
  matter in practice (not checked image by image).
- **Scaled images in the game** are, as far as the three call sites show, the pointer, Jim and the tavern walkers, all ordinary literal
  pictures. Scaled solid and shadow images are supported by the code but I do not know of any in the shipped data.
- **The 125-span row limit** (50 for mask rows). The port warns "row is full" and drops the span; the original wrote past the row. Painting has
  no such limit, so that case cannot match the original in either design. Normal scenes should not reach it.

## How to test it (outside the engine)

`AGENTS.md` forbids test hooks in the submitted engine, so the comparison must be external:

1. Locally and uncommitted, log every `queueImage`, `queueImageScaled` and `queueText` call of a frame (arguments plus which image), together
   with the layer depth, while playing. Keep that patch ignored, as the notes require.
2. In a separate small program, replay the recorded calls through two renderers: the existing queue and the new sort-and-paint (or depth
   buffer) version. A headless driver of this kind was used during this session (it loads a bank, queues images, composes, writes a PPM).
3. Compare the two frames byte for byte.
4. Record frames from every level (the game has 47), from walking, from conversations with text on screen, and from the pointer with and
   without a held item. Include a frame with two sprites at the same depth.

**Accept the simplification if** all recorded frames are identical. If some differ, look at each difference before deciding: it is most
likely an equal-depth tie, the layer-depth rule, or a shadow.

## What to do if it works

Delete `renderqueue.*`, replace `compose` with the painter, keep `TextQueue` (text is drawn over the finished frame either way), update
`re/SUBSYSTEMS.md` and `re/CHECKLIST.md` (record the ledger changes with `re/tools/mark.py`), and note in the plan that the queue was removed on
purpose.

## Not part of this

The slices of step 3 in `re/PORTING_ORDER.md` still port the queue as it is. This document only matters once that work is done and the game is
playable.
