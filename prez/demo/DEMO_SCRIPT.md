# Demo script (live parts of the talk)

One running example, `sum_clamped.c`, carried through Acts 1 to 3 of the talk.
All commands run from `prez/demo/`. Nothing here uses original game code.

```bash
PY=../../re/.venv/bin/python
./build.sh
```

## Act 1: by hand (about 6 min)

**1. Show the source.** Ten lines: a loop, a branch, an accumulator.

```bash
cat sum_clamped.c
```

**1b. Run it live.** First the C source, natively on your Mac, then the exact 32-bit x86 bytes in the emulator.
Ask the audience for an array, a `lo` and a `hi`, and type them in.

```bash
./run_native.sh
$PY run.py build/sum_clamped_O0.bin
$PY run.py build/sum_clamped_O2.bin --array=-50,5,50 --lo=-10 --hi=10
```

Expected: `55`, `55`, then `5` (that is -10 + 5 + 10). The `-O0` and `-O2` bytes are very different but give the
same answers; the instruction counts in brackets show how different.
Note: the Mac can't run the 32-bit Windows bytes itself, so the emulator is the "CPU" (more in DEMOS_EXPLAINED.md).

**2. Compile to 32-bit x86** (same CPU family as the 1990s game). `build.sh` already did it.
The flat file is only the machine code, 112 bytes at `-O0`.

```bash
xxd -g1 build/sum_clamped_O0.bin
```

**3. Find the opcodes in the hex.** Open `build/sum_clamped_O0.bin` in a hex editor
(Hex Fiend, or just `xxd`). Point at these and decode them with the audience:

| Offset | Bytes | Meaning |
|---|---|---|
| 0x00 | `55` | `push ebp` (function prologue) |
| 0x01 | `89 e5` | `mov ebp, esp` |
| 0x03 | `83 ec 0c` | `sub esp, 0xc` (room for 3 locals) |
| 0x06 | `8b 45 14` | `mov eax, [ebp+0x14]` (an argument: 4th, `hi`) |
| 0x23 | `3b 45 0c` | `cmp eax, [ebp+0xc]` (`i < n`) |
| 0x26 | `7d 40` | `jge +0x40` (loop exit) |
| 0x6f | `c3` | `ret` |

Talking points:
- Opcode `8b` is "mov reg, r/m"; the next byte says which registers and addressing.
- `7d 40`: the jump is a relative offset, `0x28 + 0x40 = 0x68`.
- Arguments live at `ebp+8, +0xc, +0x10, +0x14` (cdecl, pushed right to left).

**4. Let a tool do the decoding** (this is what the CPU manual lookup becomes):

```bash
objdump -d --x86-asm-syntax=intel build/sum_clamped_O0.obj
```

**5. Compiler reveal.** Same source at `-O2`:

```bash
xxd build/sum_clamped_O2.bin | head
objdump -d --x86-asm-syntax=intel build/sum_clamped_O2.obj | head -60
```

112 bytes became 303. The loop was vectorised with SSE (`pcmpgtd`, `paddd`), the branches
turned into compares and masks. The structure you wrote is gone.
**Takeaway: all the information is there, but names, types and structure are lost.**

**6. Edit the program live (patch a byte).** This is the moment the hex editor earns its place.

- Copy the file: `cp build/sum_clamped_O0.bin build/sum_clamped_patched.bin`
- Open `build/sum_clamped_patched.bin` in the hex editor and go to offset `0x62`. You will see `83 c0 01`
  (`add eax, 1`, which is the `i++` of the loop). Change the `01` to `02` and save.
- Run it: `$PY run.py build/sum_clamped_patched.bin` and compare with the original `55`.

Expected: **25**, not 55. The loop now counts `i += 2`, so it only adds elements 1, 3, 5, 7 and 9.
You changed the behaviour of a compiled program by editing one byte, with no source code.
Scripted fallback if the hex editor misbehaves: `./patch_demo.sh`, then run the same command.

## Act 2: Ghidra (about 4 min)

1. New project, then File > Import File, choose `build/sum_clamped_O0.obj`
   (Ghidra reads the COFF object; language x86:LE:32 is picked automatically). Auto-analyze.
2. Open `_sum_clamped` in the Listing and the Decompiler side by side.
3. Show: recovered loop and `if/else`, parameter types guessed as `int`/`int *`, locals named
   `local_c`.
4. Rename live: `param_1` to `values`, `param_2` to `count`, `local_10` to `total`, and set the
   prototype. Watch the decompiler output become readable.
5. Repeat on `_O2.obj` to show the decompiler recovering a loop from SSE code (messier, still useful).
6. Then switch to the real `TESORO.EXE` project and show the function list. The scale problem.

## Act 3: verification with Unicorn (about 4 min)

**1. (Optional aside) Capstone** is a disassembler library: the scriptable version of step 4 above. Skip it if short on time; the slide does not mention it.

```bash
$PY -I verify.py build/sum_clamped_O0.bin --disasm 8
```

**2. Unicorn** is a CPU emulator. `verify.py` maps the function bytes at an address, builds
a stack by hand (arguments right to left, then a return address), and runs it.
Show the `Runner.call` method on screen: it is about 15 lines.

**3. Faithful hypothesis passes** (the Python version of the C, same branch order):

```bash
$PY -I verify.py build/sum_clamped_O0.bin --hypothesis faithful
$PY -I verify.py build/sum_clamped_O2.bin --hypothesis faithful
```

Both match 305/305 cases. Different instruction counts (445 vs 148 per call on average):
same behaviour, very different code.

**4. The plausible-but-wrong hypothesis.** `min(max(v, lo), hi)` looks equivalent.
This is exactly what a confident AI explanation looks like.

```bash
$PY -I verify.py build/sum_clamped_O2.bin --hypothesis plausible --happy-path-only
$PY -I verify.py build/sum_clamped_O2.bin --hypothesis plausible
```

- Happy-path tests (`lo <= hi` only): **passes**. False confidence.
- Full tests including `lo > hi`: **fails**, with the exact input that disagrees
  (`a=[1, 5, 9, 20] lo=8 hi=2: emulator=20 hypothesis=8`).

**Takeaway:** the emulator is the oracle. A hypothesis is only as good as the inputs you
chose to test it with. Say it plainly: this is evidence, not proof, and functions that
call the OS or other code need stubs.

## Bridge to Act 4

"Now imagine an AI proposes the hypothesis. The emulator does not care how confident it sounds."
Then move to the Ghidra MCP loop (ledger, contract, verify, record). That part uses the real
project artifacts (`re/ledger.json`, `re/contracts/`, `re/STRUCTS.md`); record a run beforehand
in case a live agent is slow.

## Fallbacks

- Ghidra slow or unavailable: use `objdump` output plus a screenshot taken beforehand.
- Unicorn import error: run with `re/.venv/bin/python`, never system Python.
- `build/` missing: run `./build.sh`.
