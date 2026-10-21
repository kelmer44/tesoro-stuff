# Prototype validation — 20 October 2026

These results describe the preserved working prototype, now archived under
`prototype/source`. They do not describe the generated starter on the active
`tesoro` branch. The starter generation/build check is recorded separately in
`analysis/create-engine-check.json` and `analysis/create-engine-build.log`.

Built against local ScummVM commit `eb4ab9a8f8933d3d37158cfd461aab3673901093`.
Engine files are in the separate `../scummvm-tesoro/engines/tesoro` checkout; the pre-existing checkout at
`/Users/gabriel/Desktop/source/scummvm` was not edited.

- Build: succeeds with clang 21 and SDL 2.32.10. Log: `analysis/cpp-build.log`.
- C++ self-test: passes. Reads the 2,416 archive entries, parses 50 scenes/747
  objects, decodes 2,012 images/371 animation banks, checks both pickup branches
  against expected speech/message counts, and holds every asynchronous request
  to check that the script does not advance early. It also checks the unsupported
  conversation-menu boundary, animation completion, the captain's pose after
  speech, inventory switching, and the actual door transition in both directions.
  Log: `analysis/cpp-selftest.log`. The logged unsupported-menu warning is an
  expected test result.
- Full autoplay: passes using the actual engine host and unshortened voice/sleep
  timing. Plays all 25 first-pickup voice lines through SDL's dummy audio mixer,
  executes animation, movement and native exit, then asserts harbour destination,
  captain v0=1, inventory pen on, room pen off and letter overlay off.
  Log: `analysis/cpp-autoplay.log`.
- Native macOS UI: opened the packaged app, selected Coger with the mouse and
  clicked the pen on the desk. The letter appeared with Spanish subtitles and
  the pen appeared in the inventory bar. The native run completed to the harbour;
  clicking its left doorway returned to the office with the captain and inventory
  state preserved. R reset it, leaving the app ready to try.
- Existing six Python checks against the supplied archive: pass.
- Visual inspection: office, letter close-up, captain conversation and harbour
  screenshots checked. Saved as `analysis/prototype-{office,note,captain,harbour}.png`.
- Ghidra: added the scenery constructor to the selected exports; all 48 routines
  decompile successfully. Log: `analysis/ghidra-prototype-console.log`.

These establish a working first slice, not complete original-game fidelity.
In particular, walking/floor scaling, actor depth, exact animation composition
and full initialization still require comparison and further reverse engineering.
The harbour uses a temporary 25% Jim scale. Movies and CD music are not yet wired
into the runtime. Original data files and extracted media were not changed.
