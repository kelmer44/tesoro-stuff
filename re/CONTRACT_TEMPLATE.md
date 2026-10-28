# FUN_xxxxxxxx — <name or "unnamed">
Status: <ledger status> · Cluster: N · Depth: N · Convention: <cdecl/stdcall/thiscall/fastcall + evidence>

**Purpose (one line, `inferred` or `confirmed`):**

**Inputs:** args with widths/signedness; `this` fields read (offset, width, struct).
**Outputs / state written:** return value; fields, globals, objects written.
**Calls:** callee contracts relied on (addr → name). Virtual calls: slot, table.
**Yields / blocks / timing:** none | describes (clock reads, waits, script resume).
**Edge cases:** jump tables, signed/unsigned compares, overflow, null handling, FP/`__ftol`.
**Evidence:** asm ranges, strings, xrefs, data tables, callers' usage.
**Port:** engine file::function (branch/commit) · Deviations from original (if any, with reason).
**Validation:** method (emulator / trace / original) · inputs covered · result · what it does *not* cover.
**Open questions:**
