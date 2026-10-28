# Tesoro project direction

## Restart, 28 October 2026: fresh function-by-function analysis

Gabriel restarted the reverse engineering. Everything produced before this date
(`MASTER_PLAN.md` task ordering, `RE_STATUS.md` evidence tables, topic reports such
as `CLICK_WALKING.md`, `analysis/`, `tools/compare_*.py`, `prototype/`, and the
existing engine ports on `codex/*` branches) is **legacy, for reference only**. It
is not evidence for new work. The implementation is based on newly decompiled
functions, recovered one at a time with the process in [re/README.md](re/README.md)
(ledger `re/ledger.json`, contracts in `re/contracts/`, structures in `re/STRUCTS.md`).
Do not copy a legacy conclusion into a contract or port. Read legacy notes only
after a cluster's fresh result exists, to compare; record any disagreement and
resolve it with evidence. Legacy validation tooling (emulator harness ideas) may
be reused as measurement tools. The fidelity rules below still apply.
Python tools for the fresh work run from `re/.venv` (Unicorn, Capstone).
The Ghidra program is the original `TESORO.EXE` (SHA-256 verified, base 0x400000).

The user's goal is a complete ScummVM implementation of **El Tesoro de Isla
Alcachofa**, with the greatest practical fidelity to the original. The user and
Codex will implement it together; Codex leads the reverse engineering.

- Read [MASTER_PLAN.md](MASTER_PLAN.md) at the start of project work. It is the
  main implementation guideline. Use [RE_STATUS.md](RE_STATUS.md) for the current
  milestone, evidence, uncertainties and next actions. Update these when findings
  change the plan. Earlier feasibility/prototype notes are supporting history.
- Original files in `/Users/gabriel/Desktop/ags/TESORO` and original disc media
  are evidence and remain unchanged. Extracted media, VM disks, save captures
  and Ghidra databases stay local. Keep reusable analysis tools and findings.
- The engine worktree has been `/Users/gabriel/Desktop/source/scummvm-tesoro`;
  `tesoro-engine` is Gabriel's own working branch. For future engine work,
  branch off `tesoro-engine` into a separate `codex/<topic>` branch and push
  implementation commits only to that separate branch. Do not edit, commit,
  merge or push directly on `tesoro-engine` unless Gabriel explicitly requests
  it. Verify the checkout, base revision and remote before starting. Gabriel
  reviews and copies pieces from the Codex branch into `tesoro-engine` at his
  own pace. Keep changes small and reviewable; do not perform that integration
  for him unless requested. This supersedes older branch instructions and does
  not request a branch switch or push now.
  `scummvm-fork` is the
  shared repository's other checkout and may be on unrelated work. Preserve
  uncommitted changes. `prototype/source` is an immutable reference snapshot.
- Keep task-file inspection within the named Tesoro project folders. Do not
  inventory personal folders, installed applications or VM configurations.
  Ask before accessing any additional location. Original-game reference setup
  is on hold until Gabriel explicitly authorizes its environment and paths.
- Do not add test classes, test configuration flags or harness hooks to the
  submitted ScummVM engine. Keep verification external; experimental C++
  instrumentation, if needed, stays local and ignored rather than in engine code.
- Keep executable addresses and reverse-engineering provenance in research notes,
  not in engine source comments. Source comments should explain useful behaviour.
- Read [skills/tesoro-re/SKILL.md](skills/tesoro-re/SKILL.md) for Tesoro research.
  Capture newly successful reusable research techniques there or in its linked
  references, with commands, evidence and limitations. Untried ideas belong in
  the plan until demonstrated.
- Separate recovered semantics, original-runtime observations, implementation
  status and approximation. A decoded asset, completed decompilation, passing
  host test or playable shortcut does not establish original-game fidelity.
- Preserve original script block types, DO ordinals, variable ownership,
  integer widths and yielding behaviour. Recover native object methods as well
  as script callbacks. Make unsupported reachable behaviour visible.
- Prioritize the earliest unresolved dependency in the master plan. Work in
  reviewable increments; do not replace the active engine with the old prototype
  wholesale or introduce puzzle shortcuts to claim completion.
