---
name: tesoro-re
description: Recover and validate El Tesoro de Isla Alcachofa engine behaviour from its original archives, scripts and Windows executable for the ScummVM port. Use for Tesoro formats, native object methods, script scheduling, movement, rendering or original-versus-port comparisons.
---

# Tesoro reverse engineering

**Restart, 28 October 2026.** Work follows `re/README.md`: fresh, function-by-function
recovery from the Ghidra program, tracked in `re/ledger.json`. All earlier reports,
`analysis/`, `tools/compare_*.py` results and engine ports are **legacy reference only**:
compare against them after a fresh result, never derive from them. Use `re/.venv/bin/python`
(Unicorn, Capstone). The comparison techniques below remain valid as methods.

Research root on this machine: `/Users/gabriel/Desktop/source/tesoro`. Read its
`MASTER_PLAN.md` and `RE_STATUS.md` first. Follow the current dependency and
update the checkpoint when evidence or implementation changes. The skill may
be installed as a symlink; resolve project files against the research root,
not the installation directory.

Keep task-file inspection inside the named Tesoro project folders. Do not
inventory personal folders, installed applications or VM configurations. Ask
before accessing additional locations; reference setup needs explicit approval
of its environment and paths. Use project-local scratch directories when possible.
Keep test classes and test-only hooks out of the submitted ScummVM engine.
Use external research tools or local ignored instrumentation for verification.
Keep addresses and provenance in research notes rather than engine comments.

## Locate the correct evidence

- Originals: `/Users/gabriel/Desktop/ags/TESORO`; preserve unchanged. Identity
  hashes and executable addresses are in `TECHNICAL_NOTES.md`.
- Active engine: `/Users/gabriel/Desktop/source/scummvm-tesoro`, branch `tesoro`.
  Verify `git status`, HEAD and local edits. The main `scummvm-fork` checkout
  can be on unrelated work. The preserved `prototype/source` is reference only.
- Structural readers: `tools/inspect_tesoro.py`; script catalogue/tracer:
  `tools/trace_script.py`. Both have intentionally limited semantic scope.
- Rebuild scope inventory with `python3 tools/catalogue_game.py
  /Users/gabriel/Desktop/ags/TESORO --output analysis/full-game-catalogue.json`
  from the research root. It enumerates all scenes/kinds, blocks/labels and
  lexical forms without claiming runtime coverage.
- Assembly, C and xrefs: `analysis/decompiled/` and `analysis/startup/`.
  Native site identities: `analysis/native-callbacks.csv`. Pen state and native
  door evidence: `FIRST_PUZZLE_TRACE.md`. Startup: `STARTUP_AND_LOOP.md`.
  Historical notes/test logs can describe a different source state; verify
  against current code before reusing their claims.
- Current startup integration evidence: `M0_BASELINE.md`. Introduction dispatch
  evidence: `INTRODUCTION_CONTRACT.md`; regenerate it with
  `tools/inspect_introduction.py`. This demonstrates hash-guarded PE virtual
  address mapping to native jump-table bytes and object-name strings, which can
  distinguish ignored annotations and different same-named scene objects.
- For animation, read `ANIMATION_FINDINGS.md` and `analysis/animation/`. Native
  loader normalization and elapsed-time advancement differ from naive frame
  iteration. Missing-image frames can delimit sequences. Do not infer playback
  rate from host redraw rate, and verify research labels against function bodies.
  For isolated original-instruction comparisons with production C++, read
  [references/native-comparison.md](references/native-comparison.md). This method
  has verified setup, advancement and pause/resume without a Windows guest.
- For scene entry, binding, group order or scenery, read `SCENE_LIFECYCLE.md` and
  `analysis/scene-lifecycle/`. Default new-game entry uses the named floor's
  coordinates; room transitions can supply explicit positions. The scene
  comparison captures original group insertion and sprite submissions with
  simultaneously live objects. It does not validate the pixel compositor or
  actor scheduler. Its invocation is in the native comparison reference.
- For the connected normal New Game route and current stopping point, read
  `LIVE_OPENING.md`. The live executable now reveals the harbour and stops at
  Show Jim. `tools/compare_opening_pixels.py` executes original RLE composition
  and font layout/drawing against the production renderer. Its same-input sprite
  comparison proves pixels, not actor logic or a Windows reference session.
  Reproduction and live-UI capture guidance are in the native comparison reference.

## Follow the actual executable dispatch

1. Start with a concrete input/script/message or LVL record. Link original
   resource, line/offset, object kind/subtype and state owner to the native path.
2. For script text, parsing and linking, read `SCRIPT_COMPILER.md`; verify the
   engine compiler with `tools/verify_script_compiler.py`. The executor is in
   `SCRIPT_EXECUTOR.md` with `tools/verify_script_executor.py`. Scene transfer, speech and the captain scene are in
   `OFFICE_TRANSFER.md` with `tools/compare_text_layout.py` and `compare_talk_draw.py`.
   Jim, floors and scaled
   drawing are in `JIM_AND_WALKING.md` with `tools/compare_scaled_sprites.py`,
   `compare_floor_scale.py` and `compare_jim_walk.py` (emulator comparisons). For scripts, `.BLOCK N` selects the script class; **DO is numbered in source
   order per block**, independent of annotation words. The parser is `0x404810`,
   executor `0x405a60`, registration `0x406190`, callback vtable offset `+0x70`.
   Use the CSV to retain ordinals. Types 0, 11 and 13 share the established
   `0x401d20` no-op; specialized handlers can ignore individual ordinals too.
3. For objects, follow scene factory `0x409e50`, constructors, vtable writes and
   overrides. Object callbacks contain rules absent from DLG/DO handlers.
   Recover subtype layout per kind; a pen/door byte offset is not universal.
4. Identify missing virtual targets explicitly before trusting the decompiler.
   Existing Java exporters refuse to create a target inside another function;
   investigate that boundary rather than forcing a misleading export. Check
   matching instructions, calling convention, field widths and switch cases.
5. Track initialization, writes, reads and lifetime of each field. Pen `v0`
   belongs to the office CAPITAN context on its known pickup path. Do not infer
   all variable contexts from that one case. `activate` and `show` differ.

For reproducible Ghidra exports and the demonstrated cache-directory fix, read
[references/headless-ghidra.md](references/headless-ghidra.md). It includes a
tested read-only rerun that does not save changes to the analysis database.

## Known traps that change implementation decisions

- TRE has no header: 28-byte records; retain `flags & 4`, including flags 6.
  DAT offset zero is valid. Payload starts after a four-byte size prefix.
- Walk LVL/ANM structures sequentially; do not signature-scan inside pixels.
  Signed frame image index `-1` is valid for invisible menu frames.
- Script annotations are not native function names or evidence of effects.
  Catalogue spelling anomalies such as `menutiem0`; inspect parser acceptance
  before treating them as normal syntax or editing the game data.
- The tracer and prototype support only limited script paths and tail transfers.
  Request traces do not model original timing, walking or general scheduling.
- The original loop uses cooperative object updates and resumable actions.
  Host refresh/pacing does not establish original simulation frequency.
- Kind-2 image previews are diagnostic grayscale, not the original shadow blend.
  CP850 readability does not prove the original font/glyph mapping.
- A past C++ build/test pass applies to that source/binary only. Verify harness
  hooks before running an integration driver; absence can make it hang.

## Turn a finding into reusable evidence

Write a small behaviour contract: trigger, context, state changes, output,
wait/resume/cancel conditions and save lifetime. Attach exact binary identity,
addresses, assembly and data lines. Mark inferred semantics separately from
observations in the running original and comparisons with ScummVM. Use matched
action recipes/checkpoints for differential comparisons when reference runs exist.

Implement a general engine rule rather than a puzzle-specific shortcut. Unknown
reachable instructions/methods remain visible. Run the relevant data checks and
current build/replay; do not call asset equality a complete fidelity check.

When a new technique succeeds, preserve the minimal runnable invocation, required
inputs/version, output interpretation and limitations in this skill or a linked
reference. Add scripts when repeated execution benefits from them. Untried
debugger/emulator ideas stay in the master plan until demonstrated. Keep original
assets, full extracted scripts/media, VM disks and Ghidra databases local.

**Building an ignored production driver without the full ScummVM link (demonstrated 26 October 2026).** From the engine worktree, after
`make` has produced `engines/tesoro/*.o`: `ar rcs libc.a $(find common graphics -name '*.o')`, then
`g++ -std=c++11 -g -fno-exceptions -DHAVE_CONFIG_H -DMACOSX -DPOSIX -DENABLE_TESORO=STATIC_PLUGIN -I. -I./engines -I/opt/homebrew/include
driver.cpp <engines/tesoro/*.o except metaengine, detection, console, tesoro.o, natives.o> libc.a -o driver` (the archive lets the linker take only the
`common` objects it needs). Drivers read binary requests on stdin and write int32 results on stdout; `#define private public` before the engine headers
lets a driver read members without touching engine code. In zsh keep the object list in an array (no word splitting) and write temporary output in the
scratchpad. Pairs with a Unicorn harness (`tools/compare_path_route.py` is the smallest example: capture a virtual call by hooking its stub address).
Pitfall found: a generic point-in-polygon is not a reference for the original's x-monotone test; compare engine and original code, not a third implementation.

## Technique: original code calling back into original code (machine-code stubs)
To test a function that calls virtual methods (object-list passes), build fake objects in emulated memory whose
vtable slots point at tiny hand-encoded x86 stubs (`mov ecx,imm; push imm; call rel32; mov eax,imm; ret`). The stubs
can call other original functions on the same data, so a pass is tested against objects that edit the list they are
walked from. See `re/tools/gen_vectors_pass.py` (3530/3530 against the engine). Limitation: Unicorn keeps translated
code, so rewriting a stub at the same address between cases silently runs the old one (symptom: stale return
values). Give every case fresh stub addresses.

## Technique: render a shipped level with the engine classes (demonstrated 30 October 2026)
`re/tools/drivers/show_driver.cpp` (build like the other drivers, plus `archiveindex level cameraframe spriteobject menuitem screendisplay renderqueue`)
reads `TESORO.TRE`/`TESORO.DAT` from the original folder (read-only), loads `<NAME>.LVL`, attaches its objects, runs
`updateRoomCameras` and `compose` once and writes a PPM (`sips -s format png` converts it on macOS). Third argument `555` selects 5-5-5 pixels.
Limits: one frame at time 0, only the object kinds and image types ported so far (kinds 0, 1, 3, 25; literal images); other kinds warn and are skipped.

## Technique: run the engine headless (demonstrated 30 October 2026)
From the engine worktree after `make`: `./scummvm --config=<local ini> --add --path=/Users/gabriel/Desktop/ags/TESORO` (a project-local ini keeps the user's own
ScummVM config untouched), then `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy timeout 8 ./scummvm --config=<local ini> --gfx-mode=surfacesdl tesoro-win-es`.
Exit status 124 (timeout) means it kept running its frame loop; warnings print to stdout. The default OpenGL mode fails under the dummy driver.
This checks that the engine starts, loads the archive and loops without crashing; it does not capture the picture (use show_driver for that).

## Technique: scripted object methods in the emulator (demonstrated 30 October 2026)
To test a pass that calls object virtuals (the collision pass), build a real level with the original loader, then overwrite every object's vtable pointer with a
fake table (copy of the base vtable 0x428000) whose chosen slots point at addresses in a page mapped r-x outside the stub range (`e.uc.mem_map(0x6f000000, 0x1000, 5)`;
the 0x7f0xxxxx range belongs to `Emu` import stubs). Install Python callbacks there with `Emu.hook_function(addr, fn, nargs, pops)`; read the receiver with
`emu.uc.reg_read(UC_X86_REG_ECX)`. The callbacks log calls and answer from a scripted table; the engine side subclasses `Level` (createObject) and `Object` with the same script.
See `re/tools/gen_vectors_collide.py` and `drivers/collide_driver.cpp`. Zero the pool heap between cases (stale words leak into fields the loader leaves unset).
Limit: the real UpdateBounds is not exercised (the stub only logs), shaped objects are excluded.

## Trap: decompiled `param_1` of a thiscall method can be the stack argument (found 30 October 2026)
Ghidra showed Cursor_IsHitter as `fastcall(param_1)` reading `param_1+0x68`; the assembly (`MOV EAX,[ESP+4]`, `RET 4`) shows it reads its stack argument (the other
object) and never touches `this`. A wrong calling convention hides which object a field belongs to. For any method with a stack argument and a RET n, check the first
instructions of the asm for ESP-relative reads before porting, and give the emulator a test where this and the argument differ (the old cursor test never did).
