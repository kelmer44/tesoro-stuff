# Isolated x86 comparison with production C++

Demonstrated on 21 October 2026 for four animation routines. Use when a small
native algorithm has a known ABI, state layout and controllable dependencies.
This does not authorize a Windows/VM session or additional filesystem access.

Research root: `/Users/gabriel/Desktop/source/tesoro`. Tool:
`tools/compare_animation_native.py`. Results and limits: `ANIMATION_FINDINGS.md`.
Unicorn 2.1.4 lives in ignored `local-media/animation-venv/`. No global install.

## Reproduce the animation comparison

1. Build the current ScummVM engine. Compile the ignored
   `local-media/animation-implementation/driver.cpp` against the actual production
   `animation.cpp` and its common libraries. Do not substitute a Python model of
   C++ or copy runtime algorithms into a separate verifier. The driver uses the
   production API and is not included in engine sources or build files.

   ```sh
   c++ -std=c++11 -DHAVE_CONFIG_H \
     -I/Users/gabriel/Desktop/source/scummvm-tesoro \
     -I/Users/gabriel/Desktop/source/scummvm-tesoro/engines \
     local-media/animation-implementation/driver.cpp \
     /Users/gabriel/Desktop/source/scummvm-tesoro/engines/tesoro/animation.cpp \
     /Users/gabriel/Desktop/source/scummvm-tesoro/common/libcommon.a \
     /Users/gabriel/Desktop/source/scummvm-tesoro/common/formats/libformats.a \
     -o local-media/animation-implementation/driver
   ```

2. Run from the research root:

   ```sh
   local-media/animation-venv/bin/python tools/compare_animation_native.py \
     --game /Users/gabriel/Desktop/ags/TESORO \
     --driver local-media/animation-implementation/driver \
     --output analysis/animation-native-comparison.json
   ```

   Unicorn's JIT traps with exit 132 in the command sandbox on this machine;
   the same minimal invocation works with approved execution outside that
   sandbox. Use the tool approval mechanism when needed; do not change security
   settings or interpret that trap as an original-game instruction fault.

## Scene, room and scenery comparison

Use `tools/compare_scene_native.py` for production LVL parsing and selected
constructor, room/camera, frame-preparation, bank-selection and sprite routines.
It extends the animation emulator with bounded allocation and queue capture.
All 50 LVLs have passed; exact counts and limits are in `SCENE_LIFECYCLE.md`.

Build the ignored scene driver against current production sources:

```sh
c++ -std=c++11 -DHAVE_CONFIG_H \
  -I/Users/gabriel/Desktop/source/scummvm-tesoro \
  -I/Users/gabriel/Desktop/source/scummvm-tesoro/engines \
  local-media/scene-implementation/driver.cpp \
  /Users/gabriel/Desktop/source/scummvm-tesoro/engines/tesoro/scene.cpp \
  /Users/gabriel/Desktop/source/scummvm-tesoro/engines/tesoro/resources.cpp \
  /Users/gabriel/Desktop/source/scummvm-tesoro/engines/tesoro/animation.cpp \
  /Users/gabriel/Desktop/source/scummvm-tesoro/common/libcommon.a \
  /Users/gabriel/Desktop/source/scummvm-tesoro/common/formats/libformats.a \
  -lz -o local-media/scene-implementation/driver

local-media/animation-venv/bin/python tools/compare_scene_native.py \
  --game /Users/gabriel/Desktop/ags/TESORO \
  --driver local-media/scene-implementation/driver \
  --engine /Users/gabriel/Desktop/source/scummvm-tesoro \
  --output analysis/scene-native-comparison.json
```

The same JIT permission constraint applies. Keep the driver local and ignored;
there are no engine test classes, flags or hooks. If reconstructing the thin
driver, the Python tool's `row`/`equal` calls define its textual result protocol:
input is a series of uint32 little-endian length-prefixed raw LVLs; output rows
are S/B/G/O for resources, E for room entry, D/F/P for scenery and flags/banks,
C for cameras, then Q/J for integrated queue samples. Use production APIs only.

Preserve bank frame sharing while allocating separate mutable playback states
for each live object. Fixed-frame scenery constructors can zero shared movement
data, so sequential independent fresh-bank experiments can miss real behaviour.
For ordered queues, execute the original group-insertion routine on native list
skeletons, keeping objects and clones live simultaneously, then traverse that
list. Capture image identity as well as coordinates/depth and order.

The floor lookup and allocator are supplied to isolated native routines. Sprite
submission is captured before the renderer consumes pixels. This validates those
routine boundaries, not the whole original collection, residency, actor update,
scene transition or compositor. The report hashes production sources, the
driver, tool and game resources; rebuild the driver after source edits.

## Pixel composition, font layout and normal executable checks

Demonstrated by `tools/compare_opening_pixels.py`; see `LIVE_OPENING.md` and
`analysis/opening-pixel-comparison.json`. Build ignored
`local-media/opening-implementation/pixels.cpp` against production renderer,
scene, resources and animation sources plus graphics/common/formats libraries
and zlib. Keep this thin driver external; no engine classes, flags or hooks for QA.
The tool defines the binary protocol: length-prefixed Puerto LVL and font ANM,
text count and length-prefixed strings; output staged sprite lists/pixel buffers,
then text/pixel buffers. Frame pixels are 640x480 little-endian ARGB32.

```sh
local-media/animation-venv/bin/python tools/compare_opening_pixels.py \
  --game /Users/gabriel/Desktop/ags/TESORO \
  --driver local-media/opening-implementation/pixels \
  --engine /Users/gabriel/Desktop/source/scummvm-tesoro \
  --output analysis/opening-pixel-comparison.json
```

Independently relocate original RLE row span pointers; kind-1 span data also
needs literal-pixel pointer relocation. Do not replace original span processing
with a second reconstructed renderer. Execute `0x411f80` to queue sprites and
`0x412e10` to compose. Renderer `+0x670/+0x674/+0x678` point to private row,
shadow-row and output buffers; byte `+0x66a=1` bypasses DirectDraw acquisition.
The base scene emulator intercepts sprite submission: override that callback
without changing registers/stack so the actual queue routine runs for pixels.

Font layout `0x411c80` reads native frame stride 36, not disk stride 24; each
frame `+24` points to an image wrapper. Font bank is renderer `+0x20`; pending
text count is `+0x13a0`; records start at `+0x680`, stride `0x70`. Execute
`0x412c50` for each record. Draw over an RGB565-exact nonblack background to
verify black outlines. The demonstrated narrator color is `0x001f`. These
addresses are edition-specific and guarded by the EXE hash.

Five scene stages and all 18 introduction texts matched exactly after the same
RGB565 expansion. Same submitted sprite lists isolate the compositor; they do
not independently validate newly added character logic, scheduling, shadows,
scaling or whole-game behavior. The original Windows session remains separate.

Also verify the actual executable through normal UI. On macOS, a project-local
ignored `.app` with a unique bundle identifier can launch a copy of the current
binary and a local config. Match binary hashes before capturing; select the
exact app by bundle ID or path. A generic `ScummVM` selection can choose a
different installed version. Open no application inventories. The observed
recipe is Nueva partida, movie/card, Modo Regular, movie 2, narration and
harbour reveals. Escape can skip movies. Logs identify script source lines;
they alone do not establish visible or audible output. Save actual window
captures separately from driver-generated pixels, with hashes and the action
recipe. No special start-at-scene config is needed.

## Extend the method safely

- Verify the exact EXE SHA-256 before using addresses. Map PE sections at their
  preferred virtual addresses, respecting raw versus virtual sizes. Populate
  only the native state/frame fields the selected routine actually reads.
- Set the calling convention, registers, stack arguments and return sentinel
  from assembly. Intercept only established dependencies; animation injects the
  game clock at `0x413a10`. Emulated code gets no guest OS or host filesystem.
- Bound instructions per call. Check the return sentinel, then compare all
  observable fields at every operation, including exact boundaries and wrapping.
  Keep failures visible. After an emulated exception, create a fresh emulator
  before another case so pending exception state does not contaminate results.
- Classify rejected C++ calls by executing the matching native operations. A
  valid native sequence rejected by C++ is a defect, not coverage to skip. Keep
  deliberate defensive divergences separate from matches, with failing inputs.
- Driver protocol: each case starts with frame count and interpolation flag,
  followed by `image dx dy duration` per frame, operation count, then operations
  `p now first last loop cache`, `u now`, `a now`, or `r now`. Emit operation,
  success flag, and the 11 fields named by `FIELDS` in the Python tool after
  each operation. Recreate only this thin local driver if it is absent.
- Preserve source/binary identity and rebuild the driver after runtime edits.
  Matching isolated routines does not verify actual callers, asset loading,
  game clock integration, rendering, scheduling or complete-game fidelity.
