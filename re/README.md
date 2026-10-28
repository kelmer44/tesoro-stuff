# Fresh function-by-function analysis (started 28 October 2026)

Goal: re-derive and port every function of `TESORO.EXE` (PE32, base 0x400000,
SHA-256 `0fcd56b3…92ef`, verified against the open Ghidra program) independently
of the earlier work. Earlier notes, `analysis/`, `tools/compare_*.py` and the engine
ports are **reference only**: read them *after* a cluster's fresh result, to compare.
A disagreement is a finding to resolve with evidence, not something to paper over.

## Files
- `ledger.json` / `ledger.csv`: one row per function. Hand-edited fields: `name`,
  `status`, `engine`, `notes`. `tools/build_ledger.py` regenerates the rest and keeps those.
- `export/` (ignored): decompiled `.c`, `.asm`, `.xrefs` per function. Refresh with
  `tools/export_ghidra.py` (needs the Ghidra bridge on :8089).
- `contracts/<addr>.md`: one note per function (template: `CONTRACT_TEMPLATE.md`).
- `STRUCTS.md`: recovered layouts, one confidence marker per field.
- `clusters.md`: suggested session units (generated view of ledger `cluster`).

## Status values (a function only advances with evidence)
`untouched` → `decompiled` (read, signature fixed) → `understood` (contract written)
→ `ported` (C++ exists) → `emu-compared` (original machine code vs port, matched inputs)
→ `original-compared` (observed in the running original; blocked until a reference
environment is authorised). Pure runtime/CRT functions: `replaced` (with the ScummVM service).

## Session protocol (every new conversation)
1. Read this file, `STRUCTS.md`, and the ledger rows + contracts of the cluster and of its callees.
2. Work bottom-up inside the cluster (ledger `depth`). Fix prototypes/names in Ghidra
   as `inferred_`/`confirmed_` prefixed only when the evidence supports it.
3. Write the contract before the C++. Unknown stays `unknown`.
4. Port on a `codex/<topic>` branch off `tesoro-engine`; never touch `tesoro-engine`.
5. Validate with an external harness (no test hooks in the engine). Record result + limits.
6. Update ledger, contracts, STRUCTS. Commit nothing in the engine without Gabriel's say-so.
