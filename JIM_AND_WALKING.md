# Jim, floors, scaled drawing and walking

22 October 2026. Addresses apply to `TESORO.EXE` SHA-256
`0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef`; names are research
labels. Engine: `engines/tesoro/jim.h/.cpp`, `FloorObject` in `scene.h/.cpp`, scaled drawing
in `renderer.cpp` (`../scummvm-tesoro`, branch `tesoro-engine`, uncommitted when written).
Exports: `ghidra/RecoverJim.java` (output in `analysis/jim/`).

## Jim

One persistent object (`0x4070c0`, global `DAT_004420fc`), built once and moved between scenes
(`0x407b80`). Nine banks, loaded from `JIMHABIZ`, `JIMHABDER`, `JIMQUIETO`, `JIMARRIBA`,
`JIMABAJO`, `JIMIZQUIERDA`, `JIMDERECHA`, `JIMHABAB`, `JIMHABAR` (all `.ANM`, all literal
images). Bank 2 is the idle bank (one frame per direction, frame = direction - 1), banks 3-6
are the walks for up, down, left, right (`bank = 2 + direction`; frame 0 has no image, frames
1.. loop). Directions: 1 up (ARRIBA), 2 down, 3 left, 4 right. Initial values: position
(350, 20), depth 0x23 (35), scale 0x100, speech colour 0x1F, facing left (`look(3)`).
Jim's depth is never changed by walking: scene objects in groups whose depth is below 35 are
drawn in front of him.

Placement inserts him into the group's list by descending x, then sets the position and
scale (`floor_scale_at`), makes the camera follow him and queues the scene at list priority
`0x14`. Per-frame order in the object list is ascending priority: walking Jim (5), speaking
characters (6), scene (`0x14`), scripts (`0x1E`).

### Walking

`0x407600` (start, from `sendmessage ... ira x y`): needs the room's floor
(`room + 0x78`); with none it resumes the waiting script at once. Otherwise it sets the 24.8
fixed position to the committed position, stores the target and the waiting script, picks the
direction (`|dy| < |dx|` gives left or right by the sign of dx, else down if dy < 0, else up),
and starts the direction's bank from frame 1, looping, unless that bank is already selected
and playing; then the last-movement counters are cleared.

`0x407790` (every frame while walking): the squared distance to the target before moving;
advance the bank; the step is the change of the animation's movement since the last update
times his scale (24.8); the axis the bank does not move along follows the slope
(`moveX += moveY * dx / dy` for up/down, `moveY += moveX * dy / dx` for left/right); add to
the fixed position. Arrival is detected on the frame after the position equals the target
(distance 0): he is placed on the target, and the waiting script is resumed (its return
code 3 ends his update without refreshing his scale) or, with no waiter, he faces the walk's
direction (`0x407a80`, the idle bank frame `direction - 1`). If the step would pass the
target (remaining squared distance < squared step) he snaps to it. The new position is the
*pending* position (`+0x54/+0x58`); the scene commits pending positions to the drawn position
(`+0x4c/+0x50`) at the end of its update (`0x408890`). The scale is then recomputed from the
committed position. No pathfinding or floor check applies to script walks; `0x414f10` routes
click walks and is not recovered.

### Drawing

`0x407ac0`: the frame's x offset and y offset scaled by his scale (`* scale >> 8`), added to
his position, queued as a scaled sprite with scale `scale << 8` (16.16) at depth 35. The
animation's movement is not added (it is already in his position).

## Floors

`FloorObject` is kind 7: it shows the frame named by `parameters[0]` and is not drawn. The room
(kind 1) names its floor in the first 12 bytes of its record; `room + 0x78` holds it. A frame of
type 4 carries a count and then, from byte 8, entries of 6 bytes: x, y and scale as three
16-bit values (type 2 frames carry x, y pairs from byte 6: a route, not used yet). Scale 256 is
full size; the harbour's floor runs from 35 at the back to 85 at the front.

`0x414740` (scale at a point): the point relative to the floor's position; walk the outline's
edges, each ordered by x; the first edge that spans the point's x gives a scale (interpolated
along x) and whether the point is below its y; the next spanning edge on the other side gives a
second; the result is the blend of the two scales by the point's y between the edges (the first
scale alone if no second edge is found, 256 if none). An edge with equal x at both ends divides
by zero in the original (two probe points in Tierra hit this); the engine skips such edges.
The kitchen's frames claim a point they do not hold; the engine keeps a zero point.

## Scaled sprites

Scaled sprites are queued (`0x4120c0`) and the composition (`0x412e10`) draws them after all
unscaled sprites, sorted from the farthest depth to the nearest, through `0x412120`. For
literal images the rows are chosen with a vertical step of `0xffffffff / scaleY` in 16.16, and
each run of pixels is resampled horizontally: its start and end in the destination come from
its source start and end times the scale (`end = (sourceEnd * scale - 0x7fff) >> 16`), the first
pixel starts part way into a source pixel (`-(fraction * inverse)`), and each destination pixel
takes the source pixel at `(position + k * inverse) >> 16` with `inverse = 0x1000000 /
(scale >> 8)`. Scaled pixels are written straight into the frame before the span lists decide
visibility, which is why they are drawn far to near. Shadow (kind 2) and solid (kind 3) scaled
variants (`0x410fb0`, `0x411580`) are not ported. The composition assumes that every row has a run
(a full-screen backdrop); an empty row crashes the original.

## Verification

All three use ignored drivers in `local-media/jim-implementation/` that link the production
code, and the Unicorn emulator over the original executable (Python of the local animation
environment); see `skills/tesoro-re/references/native-comparison.md` for the setup.

| Tool | Original routines executed | Result, 22 October 2026 |
| --- | --- | --- |
| `tools/compare_scaled_sprites.py` | `0x411f20`, `0x411f80`, `0x4120c0`, `0x412e10` (-> `0x412120`, `0x4108a0`) over Jim's images and the harbour backdrop | 500 frames (2 seeds), 153,600,000 pixels identical; 11 scales 20-300, off-screen positions, unscaled sprites in front at depths below, equal and above |
| `tools/compare_floor_scale.py` | `0x414740` on a floor laid out from each scene file | 38 floors in 50 scenes, 7,879 points identical; 2 points skipped where the original divides by zero |
| `tools/compare_jim_walk.py` | `0x407600`, `0x407790`, `0x401740`, `0x401860`, `0x414740` | 1,500 walks (3 seeds), about 116,000 steps identical in committed position, scale, animation frame and arrival, in all four directions, with jittered frame times; one walk that never arrives matches too |

**Limits.** Object-list and scene bookkeeping are stubbed in the walk comparison, and the
waiting script is a stand-in that returns 3. The scaled comparison draws one scaled sprite at a
time; overlapping scaled sprites rely on the far-to-near order read from the composition. The
live executable was run headless (no window, speech muted, no movies), not on screen. Walking
uses the engine's game clock, not a recovered 60 Hz or host rate.

## Not done

Camera following Jim (`0x401fc0`: the camera target jumps 100 pixels, over 750 ms, when he is
within 75 pixels of the target's edge, horizontally only), the talk overlay and speech
(`0x407f30`, `0x410330`), click walking and routes, depth reordering between moving objects
(`0x408c50`), shadow and solid scaled images, and the office transfer (native ordinal 5) that
follows the walk. Jim's scene-view update at native ordinal 0 only repeats the entry position.
