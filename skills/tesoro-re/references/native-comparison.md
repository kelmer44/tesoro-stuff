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
