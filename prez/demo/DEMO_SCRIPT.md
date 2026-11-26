# Demo script (everything live)

The audience watches you do every step: type a C function, compile it, find its bytes, patch them, open them
in Ghidra, and test a claim about it that you also type. Only the small helper tools are prewritten.
One running example, `sum_to`, goes through Acts 1 to 3. Nothing here uses original game code.

| Live (you do it on screen) | Prewritten tools (say so out loud) |
|---|---|
| Typing the C function (5 lines) | `build.sh`: wraps the long clang command |
| Compiling and running it | `extract_text.py`: cuts the code out of the object file |
| Reading and editing the bytes in ImHex | `emu.py` and `run.py`: run the bytes in an emulator |
| Importing and renaming in Ghidra | `verify.py`: loops over test inputs |
| Typing the hypothesis to test (one Python line) | `sum_to_O0.hexpat`: ImHex colouring |

## Before the talk

```bash
cd prez/demo
mkdir -p live
PY=../../re/.venv/bin/python
```

- Terminal font large. Editor open on an empty `live/sum_to.c`.
- Rehearse the typing once; it is 5 lines. If a typo derails you: `cp fallback/sum_to.c live/sum_to.c`.
- **Type the function exactly as below.** The patch offsets (`0x1d`, `0x2d`) assume this exact code. `patch_demo.sh` checks the byte first and says so if your code compiled differently.
- ImHex open and the pattern file `sum_to_O0.hexpat` ready to paste. Ghidra open with a new empty project, and the `TESORO.EXE` project one click away.
- Rehearse the Ghidra raw-binary import and the ImHex pattern once. Neither has been tried yet.

## Act 1: by hand (about 6 min)

**1. Type the function live** (1 min) into `live/sum_to.c`:

```c
int sum_to(int n)
{
    int total = 0;
    for (int i = 1; i <= n; i++)
        total += i;
    return total;
}
```

Say what it does: adds 1 + 2 + ... + n.

**2. Compile it for the Mac and run it** (a known answer to compare against later):

```bash
./build_native.sh live/sum_to.c           # builds build/native_demo for this Mac
./build/native_demo 10                    # native: sum_to(10) = 55
```

Then say why the next step is different: the game is 32-bit x86 Windows, so to practise on the same kind of bytes
I compile the same C again for x86-win32 (the same CPU family as the 1990s game). This Mac is arm64, so these will be
completely different bytes, and the Mac cannot run them.

```bash
./build.sh live/sum_to.c
ls -l build/sum_to_O0.bin build/sum_to_O2.bin
file build/native_demo build/sum_to_O0.bin
```

If asked why a Python script makes the `.bin`: "the compiler wraps the code in an object file with headers and names;
`extract_text.py` just cuts the code bytes out, so what we look at is as bare as a dump from a real program."
Check it: the `.text` section is 0x3b = 59 bytes, the same size as the `.bin`.

```bash
objdump -h build/sum_to_O0.obj | grep text      # .text  0000003b
```

`-O0` is 59 bytes, `-O2` is 28. The `.bin` files are only the machine code. `file` shows the difference:
`native_demo` is `Mach-O 64-bit executable arm64`, while the `.bin` is just `data`. After extraction there is no
file format left, only bytes, which is where a reverse engineer starts with a raw dump.

**3. Find the opcodes in the hex.** First look at the bytes in the terminal with `xxd`:

```bash
xxd -g1 build/sum_to_O0.bin
```

`-g1` prints one byte per group (`55 89 e5 ...`); the default groups two bytes (`5589 e583`) and is harder to read.
Say: "this is the whole function, 59 numbers, and the ASCII column on the right is noise." Then move to a real
hex editor: open `build/sum_to_O0.bin` in ImHex, same numbers, but now it can annotate them.
Why hex, and not 1s and 0s or decimal: one byte is 8 bits, which is exactly two hex digits (each digit is 4 bits), so
every byte is two characters and you can convert to bits in your head. Binary is 8 characters per byte and unreadable;
decimal does not line up with bits (`55` hex is 85, and `7f` is 127, but `7f` shows "the top bit is clear" and 127 does not).
The patterns also show up in hex: `50` to `57` are all `push`, and `55` is `push ebp` (`50` + register number 5).
Open the Pattern Editor, paste `sum_to_O0.hexpat`, press Run (the pattern has one item per instruction, with its exact text as the comment, so it only fits this exact build): every instruction gets a colour and a hover comment
(blue stack frame, green loads, teal stores, orange compare, red jumps, purple arithmetic, gold return).
Decode a few by hand with the audience, using the one-byte opcode map as the reference (have it open in a browser tab):
[80386 Programmer's Reference, Appendix A](https://pdos.csail.mit.edu/6.828/2011/readings/i386/appa.htm)
(screenshot it beforehand as a fallback). How to read it: the high four bits of the opcode pick the row, the low four bits
the column, so `55` is row 5, column 5. Cells to find (checked in the page):
`55` (row 5, "PUSH general register"), `7f` (row 7, column F, `JNLE`, which is `jg`), `89` and `8b` (row 8, `MOV Ev,Gv` and `MOV Gv,Ev`),
`c7` (row C, `MOV Ev,Iv`). Not checked yet, look them up before the talk: `83` (needs the "Immediate Grp1" table), `3b`, `03`, `eb`, `c3`, `5d`.
The second byte after `89`/`8b` is a ModRM byte (2+3+3 bits: mode, register, memory operand), described in the Intel manual
(Vol. 2, Chapter 2). Current references: Intel's manual and [ref.x86asm.net](http://ref.x86asm.net/coder32.html).

| Offset | Bytes | Meaning |
|---|---|---|
| `0x00` | `55` | `push ebp` (start of the function) |
| `0x03` | `83 ec 08` | `sub esp, 8` (room for 2 local variables) |
| `0x09` | `c7 45 fc 00 00 00 00` | `mov [ebp-4], 0` (`total = 0`) |
| `0x10` | `c7 45 f8 01 00 00 00` | `mov [ebp-8], 1` (`i = 1`) |
| `0x1a` | `3b 45 08` | `cmp eax, [ebp+8]` (compare `i` with `n`) |
| `0x1d` | `7f 14` | `jg +0x14` (if `i > n`, leave the loop) |
| `0x2b` | `83 c0 01` | `add eax, 1` (the `i++`) |
| `0x31` | `eb e4` | `jmp 0x17` (back to the loop test) |
| `0x3a` | `c3` | `ret` |

Talking points:
- `8b` means "load", and the next byte says into which register and from where.
- `7f 14`: the jump is a distance, not an address. The jump ends at `0x1f`, and `0x1f + 0x14 = 0x33`, the instruction that returns `total`.
- The only argument `n` lives at `ebp+8`; locals live below `ebp` (`total` at `ebp-4`, `i` at `ebp-8`).
- Optional: View > Disassembler in ImHex (x86, 32-bit, base address 0) shows the decoded text beside the bytes.

**4. Let a tool decode it** (this is what the CPU manual lookup becomes):

```bash
objdump -d --x86-asm-syntax=intel build/sum_to_O0.obj
```

`objdump` reads the `.obj`, which has labels you would not have in real life. The same decoding from the raw bytes only
(no labels, no file format) is `$PY -I verify.py build/sum_to_O0.bin --disasm`.

**5. Run the exact bytes.** The Mac cannot run 32-bit Windows bytes itself, so a small emulator stands in for the CPU (more in Act 3):

```bash
$PY run.py build/sum_to_O0.bin 10         # f(10) = 55   [113 instructions]
$PY run.py build/sum_to_O0.bin 1000       # f(1000) = 500500   [10013 instructions]
```

Ask the audience for a value of `n`.

**6. Compiler reveal.** Same source at `-O2`:

```bash
$PY run.py build/sum_to_O2.bin 1000       # f(1000) = 500500   [10 instructions]
objdump -d --x86-asm-syntax=intel build/sum_to_O2.obj
```

Same answer, 10 instructions instead of 10,013. The loop is gone: the compiler worked out the whole sum is a
formula (multiply, halve, add) and replaced the loop with it. 59 bytes became 28.
Say it replaced the loop with `n(n+1)/2` (for `n = 10`: 55). (If asked: the exact arrangement is `(n-1)(n-2)/2 + 2n - 1`, equal for `n > 0`.)
The one thing it kept is `test ecx, ecx` / `jle` at the top, which returns 0 when `n` is zero or negative. Remember that for Act 3.
**Takeaway: all the information is there, but names, types and structure are lost.**

**7. Edit the program (patch a byte).** This is the moment the hex editor earns its place.

```bash
cp build/sum_to_O0.bin build/sum_to_patched.bin
```

Open the copy in ImHex (it writes changes into the file you open). Two patches, one at a time (revert in between):

| Patch | Where | Change | Effect |
|---|---|---|---|
| Loop test | `0x1d` | `7f` to `7d` (`jg` becomes `jge`) | `i <= n` becomes `i < n` |
| Step | `0x2d` | `01` to `02` (`add eax, 1` becomes `add eax, 2`) | `i++` becomes `i += 2` |

```bash
$PY run.py build/sum_to_patched.bin 10    # 45 after the first patch, 25 after the second (original: 55)
```

You changed what a compiled program does by editing one byte, with no source code.
Scripted fallback if the hex editor misbehaves: `./patch_demo.sh jge` or `./patch_demo.sh step`, then the same run command.

## Act 2: Ghidra (about 4 min)

Two parts. Part A uses the raw bytes of your function, so you see what Ghidra does *for* you.
Part B uses the real `TESORO.EXE`, where Ghidra finds the functions itself, so you see its real potential.

### Part A: your function as raw bytes (about 2 min)

Import the **raw bytes**, not the `.obj`. The object file carries the function's name and boundaries, which you would
never have in a real situation. The raw `.bin` is closer: Ghidra knows nothing except the bytes.

1. New project, then File > Import File, choose `build/sum_to_O0.bin`.
   Format: **Raw Binary**. Language: click `...` and pick **x86, little endian, 32-bit, default**
   (compiler spec "Visual Studio" is the natural match for Windows). Leave the base address at 0 so offsets match the table above.
   Say: "I have to tell Ghidra the CPU because a raw dump has no header. A real EXE has one, so Ghidra picks the language itself."
2. Open it in the CodeBrowser. You see bytes, not code, and no function: a raw file has no entry point or calls to follow.
   Say: "In a real program I would not need to do this either: the EXE's header gives Ghidra the entry point, and it finds functions from there."
3. Click the first byte (`00000000`) and press `D` to disassemble, then `F` to create a function there.
   Ghidra names it `FUN_00000000`. The decompiler panel now shows C-like code.
   Say it out loud: "In a real EXE, Ghidra does this step by itself. Here I did it by hand so you can see what it saves."
4. Show: the recovered `for` loop, the parameter guessed as `int`, locals named like `local_c` and `local_8`.
5. Rename live: `FUN_00000000` to `sum_to`, `param_1` to `n`, and the two locals to `total` and `i` (`L` renames a name).
   Watch the decompiler output become readable.
6. Optional: repeat with `build/sum_to_O2.bin`. Expect the decompiler to show arithmetic instead of a loop, because the loop no longer exists.

### Part B: the real TESORO.EXE (about 2 min)

Switch to the project that already has `TESORO.EXE` loaded and analysed. Now Ghidra has an entry point, imports and call
targets to follow, so it finds functions on its own. Show, in this order:

1. **The function list** (Symbol Tree > Functions, or Window > Functions): hundreds of `FUN_xxxxxxxx` entries that nobody created by hand.
2. **A short function with callers.** Pick it in advance (for example `00401a50`, 12 lines, which the ledger calls
   `inferred_AnimPlayer_Pause`). Open it in the decompiler, then right-click > References > Show References to ... to list its callers.
   The caller/callee links come for free.
3. **Window > Function Call Graph** on the same function, to show the structure at a glance.
4. **Window > Defined Strings** and the **Imports** entry in the Symbol Tree: text the game uses, and the Windows services it asks for.
5. **The honest caveat.** Ghidra finds functions by following calls. Code that is only reached through function-pointer tables
   (for example virtual methods) is invisible to it. In this project Ghidra's count went from **622 to 784** after
   161 such functions were defined with a script (`re/PLAN.md`). The tool does most of the work, not all of it.
6. Land the scale problem: 706 functions in the ledger, 447 of them game code.

Before the talk: decide whether you are comfortable showing decompiled game code on screen, and pre-select the function so you are not scrolling live.
Fallback for Part A: `build/sum_to_O0.obj` imports with the name `_sum_to` and the function already found,
which is faster but not realistic. Say so if you use it.

## Act 3: verification with Unicorn (about 4 min)

**1. Show the emulator.** Unicorn is a CPU emulator. Open `emu.py` on screen and show `Runner.call` (about 12 lines):
it puts the function's bytes in memory, builds a stack by hand (arguments right to left, then a return address), and runs.
Say plainly that this helper is prewritten; the point is what it lets you do.
(Optional aside: Capstone, a disassembler library, is its scriptable companion. It powers `verify.py --disasm`.)

**2. Type a hypothesis that mirrors the loop** and test it against both builds:

```bash
$PY -I verify.py build/sum_to_O0.bin --expr "sum(range(1, n+1))"
$PY -I verify.py build/sum_to_O2.bin --expr "sum(range(1, n+1))"
```

Both: `133/133 inputs match`. Same behaviour, very different code (n=1000 took 10013 instructions versus 10).

**3. Now type the clever hypothesis.** "I can see the compiler turned it into a formula, so surely it is just `n*(n+1)/2`."
This is exactly what a confident AI explanation looks like.

```bash
$PY -I verify.py build/sum_to_O2.bin --expr "n*(n+1)//2" --happy-path-only
$PY -I verify.py build/sum_to_O2.bin --expr "n*(n+1)//2"
```

- Happy-path inputs (`n >= 0`): `103/103 inputs match`. False confidence.
- Full inputs including negative `n`: `104/133 inputs match`, with `MISMATCH n=-2: emulator=0 hypothesis=1`.
  The loop never runs when `n` is negative, so the real answer is 0; the formula gives 1.
  The `test` / `jle` you saw at the top of the `-O2` code was exactly that guard.

**4. Fix the hypothesis.** Add the guard to the claim and run it again:

```bash
$PY -I verify.py build/sum_to_O2.bin --expr "n*(n+1)//2 if n > 0 else 0"
```

`133/133 inputs match`. Say: "The compiler did not just replace the loop with a formula. It wrote `if n <= 0 return 0`,
then the formula. The first claim was true for the interesting part and wrong at the edge, and the emulator is what showed it."

**Takeaway:** the emulator is the oracle. A hypothesis is only as good as the inputs you chose to test it with.
Say it plainly: this is evidence, not proof, and functions that call the OS or other code need stubs.

## Bridge to Act 4

"Now imagine an AI proposes the hypothesis. The emulator does not care how confident it sounds."
Then move to the Ghidra MCP loop (ledger, contract, verify, record). That part uses the real
project artifacts (`re/ledger.json`, `re/contracts/`, `re/STRUCTS.md`); record a run beforehand
in case a live agent is slow.

## Expected output cheat sheet

| Command | Output |
|---|---|
| `./build_native.sh live/sum_to.c`, then `./build/native_demo 10` | `native: sum_to(10) = 55` |
| `./build.sh live/sum_to.c` | `59 bytes` (`-O0`), `28 bytes` (`-O2`) |
| `file build/native_demo build/sum_to_O0.bin` | `Mach-O 64-bit executable arm64`, `data` |
| `run.py build/sum_to_O0.bin 10` | `f(10) = 55   [113 instructions]` |
| `run.py build/sum_to_O2.bin 10` | `f(10) = 55   [10 instructions]` |
| `run.py build/sum_to_O2.bin -5` | `f(-5) = 0   [5 instructions]` |
| `run.py build/sum_to_patched.bin 10` | `45` (jge patch) or `25` (step patch) |
| `verify.py ... --expr "sum(range(1, n+1))"` | `133/133 inputs match` (both builds) |
| `verify.py ... --expr "n*(n+1)//2" --happy-path-only` | `103/103 inputs match` |
| `verify.py ... --expr "n*(n+1)//2"` | `104/133 inputs match`, `MISMATCH n=-2: emulator=0 hypothesis=1` |
| `verify.py build/sum_to_O2.bin --expr "n*(n+1)//2 if n > 0 else 0"` | `133/133 inputs match` |

## Fallbacks

- Typo you cannot find: `cp fallback/sum_to.c live/sum_to.c`.
- Ghidra slow or unavailable: use the `objdump` output plus a screenshot taken beforehand.
- Unicorn import error: `run.py` and `verify.py` re-run themselves with `re/.venv/bin/python` automatically. If that venv is missing, create it first (it holds Unicorn and Capstone).
- `build/` missing: run `./build.sh live/sum_to.c`.
