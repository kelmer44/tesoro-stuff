---
name: tesoro-re
description: Recover and validate El Tesoro de Isla Alcachofa engine behaviour from its original archives, scripts and Windows executable for the ScummVM port. Use for Tesoro formats, native object methods, script scheduling, movement, rendering or original-versus-port comparisons.
---

# Tesoro reverse engineering

Research root on this machine: `/Users/gabriel/Desktop/source/tesoro`. Read its
`MASTER_PLAN.md` and `RE_STATUS.md` first. Follow the current dependency and
update the checkpoint when evidence or implementation changes. The skill may
be installed as a symlink; resolve project files against the research root,
not the installation directory.

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

## Follow the actual executable dispatch

1. Start with a concrete input/script/message or LVL record. Link original
   resource, line/offset, object kind/subtype and state owner to the native path.
2. For scripts, `.BLOCK N` selects the script class; **DO is numbered in source
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
