# New Game reaches the harbour

21 October 2026. The active engine at `../scummvm-tesoro`, branch `tesoro`,
HEAD `d98a6b45d3a` plus the changes identified in
[the build record](analysis/live-opening-verification.json), now connects the
normal New Game button to the original introduction. This is a partial M2
increment. It does not complete the introduction or reach the office.

## Visible result and reproduction

Build the active checkout with `make -j4`, then run its `scummvm` executable
with the original Spanish game and movie files. Choose **Nueva partida**, wait
through the censorship card, then choose **Modo Regular**. The engine starts
`FANPRESENT/main` and movie 2, followed by the offscreen narration, harbour
reveal, ship, gull and recruiter. Escape skips a movie; Space or left-click
skips the current narration. These are normal UI actions, with no injected scene
or debug start position.

This route was exercised in the actual newly built executable. Movies 0, 1 and
2 were skipped with Escape during the captured run; their full completion and
audio output have not been verified in this increment. The live window showed
the ship, gull, clouds, quay and recruiter. Screenshot and runtime logs remain
ignored under `local-media/opening-implementation/`; hashes are in the build
record. No original Windows process, Wine prefix or VM was started.

**Current boundary:** execution stops visibly at `ANIMS.DLG:92`, native type 1,
ordinal 4, the Show Jim action. The diagnostic dialog can be dismissed; Escape
then returns to the difficulty menu. Jim, his walk to the door, movie 3's
handoff, office transfer and captain scene remain unimplemented. The engine
does not skip those actions to claim an office state.

## Connected runtime

- `script.h/.cpp` load the original text/script catalogue, preserve source lines
  and per-block DO ordinals, and yield for host requests. Only a subset is
  implemented; tail-only `run` is not a complete scheduler.
- `tesoro.cpp` connects action 3, the harbour, script requests, movies, offscreen
  WAV narration, sleeps, sound and object activation/show/play to the main loop.
  The game clock accumulates host deltas below 500 ms and freezes during movies
  and engine pauses. No original timing comparison is claimed.
- Native type-1 ordinal 3 reveals/loops BARCO. Ordinals 1, 2 and 6 retain the
  recovered no-effect behavior. Ordinal 0 currently has no host work because Jim
  is absent and the camera is already positioned; this is not Jim lifecycle
  implementation.
- `SceneObjects` replaces the scenery-only collection. It adds name lookup and
  drawable base state for kinds 5, 6 and 9 alongside scenery kind 3. The harbour
  recruiter uses the original character idle-bank selection and clears animation
  movement outputs. This does not implement all subtype logic or interaction.
- `SceneRenderer` draws original image data with per-pixel depth: lower wins,
  equal keeps the first submitted pixel. Font glyphs, spacing, wrapping,
  baseline, centering and narrator blue come from the original font routines.
  Shadows and scaled drawing remain unsupported. Puerto's banks contain no
  kind-2 shadow images. CD music and character speech remain pending.

The user’s save API scaffold is preserved. Real save/load is still unsupported.
No test classes, test configuration flags, harness hooks or per-line provenance
comments were added to the engine.

## Native pixel comparison

[RecoverGameplayOpening.java](ghidra/RecoverGameplayOpening.java) exported 28
functions with completed decompilation, assembly and xrefs. Labels remain
research names. `0x407d00` is dialogue-choice layout/picking, not Jim movement;
`0x412120` is scaled sprite drawing, not text layout. Actual Jim movement logic
is at `0x407790`, with scale lookup through `0x414740`.

[The external comparison tool](tools/compare_opening_pixels.py) links an ignored
driver against the production renderer and scene code. It independently
relocates original RLE row/span/literal pointers into Unicorn memory and executes
the original queue (`0x411f80`) and row composition (`0x412e10`) routines. Setting
renderer byte `+0x66a` bypasses DirectDraw surface acquisition; it does not
replace the composition code. Subtitle comparison executes layout `0x411c80`
and glyph drawing `0x412c50`, using original FUENTES.ANM frame wrappers and a
nonblack background to check black outlines as well as colored spans.

[Results](analysis/opening-pixel-comparison.json): five staged harbour frames
and all 18 FANPRESENT narration texts match exactly after RGB565-to-ARGB32
conversion, **7,065,600 pixels**. The scene comparison feeds both compositors
the same submitted sprites, so it validates composition rather than independently
proving actor initialization, animation scheduling or script behavior. The 18
font cases include later text that the live script has not reached. It does not
compare shadows, scaling, full Windows output or audible output.

Prior animation and scenery reports retain their historical source identities.
The new object collection has changed names and coverage; rebuild/adapt their
external drivers before treating them as current-source checks. Current build,
pixel and live-UI evidence are distinct and recorded separately.

## Next dependency

Recover Jim's persistent object initialization and bank loading, position,
floor/depth/scale update and scaled sprite composition. Implement ordinal 4 and
the subsequent walk using those rules. Continue the same normal New Game route
past the current boundary, then recover transfer and captain speech/exit before
claiming the full introduction. Original reference setup stays on hold pending
explicit authorization of its environment and paths.
