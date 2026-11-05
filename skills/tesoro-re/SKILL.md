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

## Technique: compile-time garbage and unwritten fields (demonstrated 30 October 2026)
Code that builds records in a stack array (the script parser 0x404810 keeps its commands there) leaves the fields it does not write as stack garbage, so a byte-exact comparison fails on bytes that mean nothing. Find them by running the original twice with different fills (0x00 and 0xa5) of the whole stack **and** a code hook that refills the function's frame at entry and the scratch locals before each command (`UC_HOOK_CODE` writing below ESP), then compare only bytes that are equal in both runs (`re/tools/gen_vectors_dlg.py`, mask line `M`). Without the refill, stale values from the previous block or command look like written data. Fake structures must not live in the emulator's stack range (0x100000-0x200000); `Emu.call` resets ESP/EBP, so a hook must never call back into emulated code (use a Python copy of the function and assert it against the original first).
Text formats read with the C runtime: hook the file layer (seek, read size, fgetpos, `_read` returning 0 for end of file) onto an in-memory buffer and let the original `fscanf`/`_input` code run; probe its exact semantics (e.g. `%[0-\xa1]s` accepts bytes 0x30..0xa1, `%i` takes octal and hex) with a few direct calls before porting. Add synthetic inputs for features the shipped data never uses and random edits of real inputs (mutation fuzzing: delete, replace, insert characters, duplicate lines; keep the cases where the original reports an error as "must be rejected" only).

## Technique: call-log differential for a stateful object method (demonstrated 30 October 2026)
For a method that mostly calls other unread functions (the script executor 0x405a60/0x405850), compare the *calls it makes* instead of reading the callees: hook every callee (`Emu.hook_function`, `slot()` for vtable entries pointing at hooked addresses; thiscall `this` is `UC_X86_REG_ECX`) so each appends one line (name, normalized arguments, objects numbered by first use) to a log, make the fake world a pure function of a per-step setting that the vector file records (clock, speaking and animation bitmasks, menu choice; fields are rewritten into the fake objects before each step), and have the port's host interface answer from the same setting and write the same lines (`re/tools/gen_vectors_run.py`, `drivers/run_driver.cpp`). Vectors are operation lists (set world, start, tick, resume) with the expected log after each. Drop sessions the original cannot repeat: a loop that exceeds the instruction count, an emulator memory error, a program counter past the commands (the original reads stale heap), two scripts using one shared menu. Check the jump table in the assembly, not only the decompiler: Ghidra folded the `case 6` (skip) into `default` (which loops forever). Mutation-test the port (`local-media/mutrun.sh`): 25 mutations, 24 caught, one equivalent (a chain value that cannot occur after resolving).

## Technique: pixel-level text and boundary cases from a dry run (demonstrated 30 October 2026)
For drawing code that writes straight into the frame (the text overlay 0x412c50), compare the CRC of every frame row after the original's own Compose over a solid background, then dump one failing frame from both sides and decode the glyph data in Python: a mismatch that looked like a drawing bug was the loader's 5-5-5 conversion because the colour-mode flag (display +0x67c) was left set from the previous case while the fonts were loaded. To hit exact boundaries (`<` against `<=` at a margin) without knowing widths in advance, do a dry call to the original first, read the width back from its records, and place the next call from it (`gen_vectors_text.py`, boundary section). Mutants that survive are worth checking for equivalence before adding cases (three clamps gave the same value on equality).


## Decompiler pointer arithmetic: check the element size
`DAT_004420fc[0x35]` on an `int *` is byte offset 0xd4, not 0x35, and `*(char *)(p + 0xd4)` on an `int` variable is also 0xd4 bytes. When writing a call-log generator that sets fields of a fake object, convert every index by the declared pointer type; a write at the wrong offset corrupted the object under test (found 31 Oct 2026 in `gen_vectors_interactive.py`: the fake "talking" and "held" flags turned out to be the same byte).

## Hooking a function only for some receivers (conditional hook)
When one original function is called both on an object you want to run for real and on a fake (e.g. 0x401de0 is the animation switch of a character and of the pointer; 0x410330 is say on a character and on Jim), install a code hook that tests `ecx` first and returns without touching registers otherwise: the original code then runs. See `cond_hook` in `re/tools/gen_vectors_character.py`. Check which object each call really receives (the pointer, not the carried item, got the drop): a wrong receiver shows as a fault inside the hooked function. Mutants that survive because the original never reaches that state (a player's movement is cleared every tick, so it is zero at draw time) are equivalent; record them as such.

## Done means documented in `re/SUBSYSTEMS.md`
A cluster, port or audit is not finished until `re/SUBSYSTEMS.md` has a plain-language entry for it: what it does for the game, which engine classes are involved (and where they sit in the diagrams), what was checked against the original and what was not. Also update it when the level factory or the frame loop gains wiring (kinds built, what a frame runs), and fix any older section that your result makes false. No addresses or jargon; the technical evidence belongs in the contracts and notes.

## Done also means the checklist is current
`re/CHECKLIST.md` is generated from `re/ledger.json`. Whenever a function changes status (decompiled, understood, ported, emu-compared, original-compared), gets a name, or gets an engine port, record it with `re/.venv/bin/python re/tools/mark.py <addr> <status> [--name ..] [--engine ..]`, which regenerates the checklist. After any direct ledger edit run `re/tools/make_checklist.py`. Never edit the checklist by hand, and check `git diff re/CHECKLIST.md` before finishing a cluster.

## Technique: an object that needs two animation banks and a second object (demonstrated 31 October 2026)
For a class that reads another object's animation (the kind 16 sprite follows a path of its anchor) load both files with the original loader (0x401160), copy each into a player with Copy64Bytes (0x401580), and build the object with the original constructor from a fake record: the data pointer plus the record's size field must land on the bank index bytes, so set the size field to `indices - data`. Replace the list search (0x40f680) by a hook that returns the fake anchor for the expected key, Room_InsertSorted by a no-op, and the random functions by hooks that return a value taken from the vector and log (kind, min, max, result) so the port's driver can replay the results and check the arguments (`gen_vectors_pathsprite.py`, `drivers/pathsprite_driver.cpp`). Pitfalls met: the loader reads a 4 byte size in front of the entry (prefix it, also in the file handed to the port); do not reset the fake heap between the two loads or the second file overwrites the first; keep fake objects at addresses that do not overlap (a 0x100 byte zeroing erased the record data); an inner `|` in a state string breaks line splitting. A random time function that is only read at some ticks needs steps of 1 to 3 ms in the vector or the interesting branch (a 6 ms window here) is never reached. Use `#define private public` in the driver, not accessors in the engine, to compare internal fields.

## Technique: functions that install an SEH frame need an FS segment (demonstrated 31 October 2026)
Compiled C++ with `try`/destructors starts with `MOV EAX,FS:[0]`, which faults in the bare harness. `Emu.set_fs()` (re/tools/emu.py) builds a small GDT with a flat data segment for SS/DS/ES and a second one for FS (base 0x1f0000) and loads them: writing only FS_BASE does nothing in 32-bit mode, and loading a GDT without flat SS/DS/ES makes every stack write fail. Call it once after creating `Emu`. Also when walking original lists to read results back, raise any safety cap above the largest case (a cap of 100 made a 260-entry case look like a port bug), and check that a mutation is not "caught" only because of a harness problem by re-running the mutations after fixing it.

## Technique: compose originals you already validated as the fixture of the next one (demonstrated 31 October 2026)
The inventory strip works on a group of pictures. Instead of faking the group's list memory, the vector generator builds it with the original kind 2 constructor and attach (already validated) inside the same emulation, and fake objects only where the original merely reads fields (pictures, square, pointer: a few offsets, with real players copied by Copy64Bytes so SelectAnimBank and SetRange run for real). Put a baseline `SelectAnimBank` on every fake object in both worlds before the first operation so garbage from the harness cannot show up as a difference, and include the exact boundary values of every threshold in the random operations (a reach of 0x23 survived until pointer offsets of 34, 35, 36 were added). A hook for a thiscall function takes its stack arguments without `this`: `settext(emu, text, colour)` not `(emu, cursor, text, colour)`, a wrong arity shows as a fault at the hooked address.

## Lesson: a class built on a shared base must call the same constructor level as the original (31 October 2026)
The kind 15 port derived from the clickable class and called its full `init` (which reads approach point, names and lines and starts a sequence), but the original constructor calls only the MidBase base (animated base plus flags). Nothing in the contract said so; the comparison found it at once (approach point, facing and play state differed in all 200 cases). When a contract says "MidBase ctor", check which constructor address is called (0x40ecb0 base, 0x40ed10 with data) before choosing the port's `init`. For constructors that leave fields untouched, fill the fake memory with zeros (like the zeroing pool) rather than 0xcd, or the garbage hides the difference.

## Pitfall: engine classes that use `Common::String` cannot be linked into a stand-alone driver (31 October 2026)
The drivers compile the engine sources they test together with the driver and no ScummVM library: any use of `Common::String` pulls `str.cpp`, the memory pool, the mutex and finally the translation code. Keep the core classes to `const char *`, `char[]` and `snprintf` (the save screen caption does), or the driver cannot be built; do not link `common/libcommon.a` (it needs the whole GUI). Also check every vtable slot of a new class (`vtable_map.json`) before porting: the "inherited" hover text of two classes was in fact a stub that sets no text, found only because the logged text events differed.


## Technique: play-test a late scene with a local, ignored patch (demonstrated 4 November 2026)
For a scene the story reaches late (the board game), do not add hooks to the engine. Keep a patch outside the engine tree (`local-media/patches/board-test-jump.patch`, ignored): a `TESORO_BOARD` start that enters a level and runs the scene's start routine through `World::Scripts::runDo`, plus a frame-driven script `TESORO_DRIVE="frame:cmd[,a,b];..."` with `move x y`, `down`, `up`, `log`, `objs`, `shot` (the screen as a PPM, converted with `sips -s format png`) and `quit`. Apply it, build, run, then restore the files (back them up first; the tree may hold uncommitted work).
Run command: `TESORO_BOARD=1 TESORO_SHOTS=<dir inside the project> TESORO_DRIVE="..." SDL_AUDIODRIVER=dummy ./scummvm -c <throwaway.ini> -p /Users/gabriel/Desktop/ags/TESORO --gfx-mode=surfacesdl --no-fullscreen tesoro`.
Pitfalls found: (1) put `enable_unsupported_game_warning=false` under `[scummvm]` in the throwaway ini, otherwise the run sits in a modal dialog and looks like an idle main menu; (2) `SDL_VIDEODRIVER=dummy` and `offscreen` do not work (blit failure, no OpenGL): use the real driver, a window appears for a few seconds; startup fails now and then ("SDL_BlitSurface failed"), so retry; (3) write screenshots under the project (the scratchpad did not receive files from the run) and keep `name` buffers long enough; (4) pointer coordinates are the window's: column buttons sat at screen x = 310 + 41 * column, y about 60 for a hover; (5) log with `warning()` in engine files that have no `FORBIDDEN_SYMBOL_ALLOW_ALL`.
Limits: it shows that the wiring runs and the screen changes; it does not prove fidelity.

## Technique: a vector generator that dumps queue memory as well as the picture (demonstrated 5 November 2026)
For the shadow images the original's mask queue was compared twice: the generator walks the original's row lists in the emulated memory (header, nodes of next/pixels/x0/x1/depth/colour) and writes one line per non-empty row; the driver prints its own rows in the same form before composing. That checks the structure when the picture cannot (batches with overlapping shadows are undefined in the original and would fault, so those batches are only compared as structure). Pitfalls: reusing the compose generator by string replacement is fast but check the batch loop resets the queue per batch; and a vector that makes the ORIGINAL fault means the input is outside what the game does: change the input, not the port. Local test patches applied with `patch -p1` leave a `world.cpp.orig` behind; delete it. Use `timeout -s KILL` for the engine run, plain `timeout` left a stuck process once.

## Technique: tell a drawing bug from the game's own art by dumping the raw picture (demonstrated 5 November 2026)
When a scene looks wrong, dump the pictures the objects hold before any compositing: in a local test patch, write the current image of each object (rows of items: pixel offset, gap, length; 5-6-5 pixels; magenta for gaps) to a PPM. If the oddity is already in the dump, it is the art. Also list a level's objects with kind, key, position, sort value and the current image (type, size, offset): hiding objects with `setVisible(false)` is not a reliable way to find who draws what (kind 0 objects draw nothing; the big pictures may be drawn by other objects). The board scene's scrambled holes were the art.

## Technique: compare glue routines through a host of primitives (demonstrated 5 November 2026)
For routines that only call into the rest of the game (the scene routines behind the script `DO` lines), put the port behind a small host interface of primitives named like the original callees (`SceneHost`: enter level, set visible, select bank, fade, ...), compile only the routine file, and compare traces: `re/tools/gen_vectors_scenes.py` runs the original with every callee replaced by a recorder (fake name hash that remembers the string, so objects are known by name; Jim's fields set as the real callee leaves them) and writes one line per run (start state | ordered calls | final fields); `drivers/scenes_driver.cpp` implements the host to log the same tokens. Mutations (inverted flag, swapped order, wrong level, dropped write) were all caught. Limits: what the host does with each call (the world wiring) is not compared; the objects' positions are by formula, the characters are fake names. A read-only check of these routines had missed nothing but one hidden argument (the decompile drops stack arguments of virtual calls: check the disassembly).



## Comparing a real saved game of the original (4 Nov 2026)
Technique that worked, with the DOSBox-X reference environment (`re/reference/README.md`):
1. Play the original to the moment to compare and save; save the engine at the same moment.
2. Read the original's file from the disk image with `re/reference/fatread.py <img> find "*.SAV"` and `cat` (no mounting; shut Windows down first if the file is recent).
3. ScummVM saves are gzip: `python3 -c "import gzip,sys; ..."`. Strip the engine's own count and level numbers to line the formats up.
4. Load the original's file in the engine from the command line (`--savepath`, `-x`, target `tesoro`) to see whether it parses; the engine fails silently, so a local-only print (reverted afterwards) of the load result and of each level's record keys is needed.
Limits: the two saves differ in time-dependent values (animation frames); the original saves all 47 levels, the engine only visited ones.


## Sanitizer builds and the script smoke test (4-5 Nov 2026)
1. **Drivers under ASan and UBSan:** `re/.venv/bin/python re/tools/sanitize_drivers.py [filter]` builds each comparison driver of `run_startup_check.sh` with `-fsanitize=address,undefined` and runs it on the existing vectors (run the suite first). Found: negative-value left shifts in `JimWalker` (fixed with `* 256`). 57 of 59 drivers were clean before that fix.
2. **The whole engine under the sanitizers:** out-of-tree build, `mkdir -p local-media/sanitizer/engine && cd there && <engine>/configure <the usual options> --enable-asan --enable-ubsan && make -j6`.
3. **Script smoke test:** `re/tools/smoke_scripts.py` starts every label of every script block (452 labels, 145 blocks) in its own process, in a level that holds the block's characters, for 1,200 frames in fast mode with the player skipping lines, and reports warnings, sanitizer findings and crashes. It needs a **local-only patch** (never committed; revert it afterwards from backups) that reads `TESORO_SMOKE=BLOCK:label:frames`: starts the game without its opening script, puts Jim in the chosen level, starts the label, runs the frames without the frame limiter, makes `World::skipPressed` true every 30th call, and (needs `#define FORBIDDEN_SYMBOL_EXCEPTION_getenv`). Each run needs its own `--config=` file containing `enable_unsupported_game_warning=false` and `gfx_mode=surfacesdl` (the dialog and the OpenGL default block a headless run), and `stdin` closed. 8 parallel jobs: about 60 minutes.
4. A stack for an `assert` abort: `ASAN_OPTIONS=handle_abort=1` makes ASan print it (lldb could not launch the process here).
Limits: blocks whose characters are in different levels cannot be placed (15 labels of `FIBOLI` and `FIHLFDBLN`); a block run in the wrong level can misbehave only because the level lacks what the story gives it (the fisherman's script sets a floor picture the despacho lacks); choice menus are not answered.

## Scripts against the data (5 Nov 2026)
`re/tools/check_script_data.py` (needs `local-media/vectors/reallevels.txt` from `gen_vectors_reallevels.py`) computes name keys with the original's own hash and checks every `sendmessage '<OBJECT>'`, `run '<BLOCK>' <label>` and `play avi N` of the shipped scripts against the shipped levels and disc. Result: one dangling object name in 370 references (a typo in `FIPLATO`).

## What playing found that the comparisons could not (5 Nov 2026)
The whole game was played to the end in the engine. The bugs it exposed (Jim moving during conversations, an error loading a game, the inventory strip in the main menu, some items not observable, the save/load menu staying open after a load, Jim not selectable, Jim's walking) were all in the wiring between ported pieces (`World` hosts, menu state), not in a ported function. The function-level and real-data comparisons found a different class (save bytes, integer overflow, input conditions, undefined behaviour). Both are needed; neither replaces the other.
