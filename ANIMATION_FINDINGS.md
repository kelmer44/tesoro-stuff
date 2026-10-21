# Original animation behaviour recovered

21 October 2026. Static recovery and isolated x86 comparisons of the verified original executable. The
ScummVM engine contains no test classes or test-only startup hooks.

## Code change supported by the original

The original ANM loader at `0x401160` subtracts frame zero's signed movement
coordinates from every later frame and sets frame zero's movement to `(0, 0)`.
These fields are sequence-relative positions, not a velocity to add each frame.
The active C++ animation reader now performs that normalization for ANM version
1. Font banks keep their metrics unchanged; font loading is a separate question.

Across the original data, the two non-font banks with nonzero movement origins
are `JIMCOGER.ANM` at `(222, -318)` and `JIMQUIETO.ANM` at `(1, 11)`. Keeping these
raw coordinates as movement would place the actor incorrectly once those fields
are used by the animation runtime. This change fixes the loaded representation;
it does not by itself establish scene playback.

Evidence: [loader C](analysis/decompiled/00401160_tesoro_load_animation_bank.raw.c),
[matching assembly](analysis/decompiled/00401160_tesoro_load_animation_bank.asm),
and [data observations](analysis/animation-movement-observations.json).

## Recovered playback rules

Eight additional routines were exported with matching assembly and xrefs using
[RecoverAnimation.java](ghidra/RecoverAnimation.java). All eight decompilations
completed; inferred names remain research labels, not original symbols.

- `0x401de0` selects the object's animation bank, then calls `0x401740` with
  start frame, end frame, loop and duration-precomputation arguments.
- `0x401740` stores those parameters, starts from the requested frame and records
  the game clock. For automatic end selection, it scans the **next frame's image
  pointer**. A missing image is an animation boundary; loop mode includes this
  boundary as an endpoint used for displacement.
- `0x401860` advances frames from elapsed game time and each frame's duration.
  Its boundary comparison is strictly `elapsed > duration`. It can pass through
  multiple frames in one call; it is not one animation step per rendered frame.
- Looping preserves accumulated displacement from endpoint minus start. A bank
  flag selects position interpolation; integer division and signed 16-bit output
  are part of that path. End-frame handling differs from the active loop path.
- `0x401a70` adjusts the animation's clock origin when resuming; calling it
  “stop animation” was an incorrect initial research label and has been corrected.
- `0x401a50` clears playing, sets paused and records pause time. Updating while
  paused still produces the current frame's stored movement.
- `0x401580` clones a bank's 64-byte mutable state; playback belongs to each
  object-owned bank. `0x401aa0` initializes the base object and its bank references.
- `0x401dc0` restores a saved object bank and its flags, rather than implementing
  a general “stop animation” operation.

The updater establishes elapsed-time animation semantics. It does not establish
a rendering FPS limit or DirectDraw presentation/vblank behaviour. Individual
sampled Jim frames have duration 42 ms; that is not evidence of an engine-wide
24 FPS or 60 FPS limit.

See [playback setup](analysis/animation/00401740_tesoro_set_animation_playback.raw.c),
[frame advancement](analysis/animation/00401860_tesoro_advance_animation.raw.c),
[resume](analysis/animation/00401a70_tesoro_resume_animation.raw.c), and
[status](analysis/animation/decompile-status.tsv).

## Validation and remaining work

The active engine now includes `animation.h`/`animation.cpp` and builds with the
new player. It implements automatic/explicit sequence bounds, elapsed-time
catch-up, optional cached-duration folding, loop displacement, interpolation,
pause/resume and frame/image access. Callers supply the game clock. Arithmetic
preserves 32-bit wrapping, signed division toward zero and signed 16-bit movement.
Playback setup preserves movement outputs until the next update. Comments in
engine source explain behaviour; addresses and provenance stay in these notes.

The [external comparison tool](tools/compare_animation_native.py) executes the
four original setup/update/pause/resume routines in Unicorn 2.1.4 and compares
11 state fields with a local ignored driver compiled against production C++.
The [report](analysis/animation-native-comparison.json) records executable,
driver and comparison-tool hashes. **1,200 scenarios match, covering 17,942
states and 289 non-font banks with visible images.** Inputs include duration
boundaries, automatic and explicit endpoints, loop/cache combinations, negative
movement, integer/clock wrap, pauses, restarts and zero-duration internal frames.
The corpus consists of candidate sequence calls, not recovered actual callers.

Eight additional synthetic scenarios are deliberately rejected by C++: starting
an interpolated multi-frame sequence with zero duration, or a loop whose live
frames all take zero time. Their matched original calls fault or exceed the
instruction bound. The report lists each case and its failing operation; they
are not counted as matches. There are no rejected corpus scenarios. Internal
zero-duration frames and zero-duration endpoints remain supported; rejecting
every zero-duration interpolated segment was too strict and was corrected using
the native comparison. Invalid ranges and font playback are also rejected by
the C++ API rather than permitting invalid access.

The normal engine builds without the startup harness. Previous startup replay
results apply to the earlier instrumented binary; they are historical evidence.
No test classes or test hooks should be reintroduced into submitted engine code.
The currently restored save API scaffold has an error-returning `syncGame` body;
real persistence is still unimplemented.

Next: recover harbour entry and object lifecycle, then connect this player to
scene objects and rendering. It is not connected to the introduction yet.
The isolated comparison injects the clock and constructs native frame records;
it does not validate loader integration, whole-game scheduling, visual output or
a running Windows reference. Original-runtime comparison remains open. The
reproducible technique is in the research skill's
[native comparison reference](skills/tesoro-re/references/native-comparison.md).
