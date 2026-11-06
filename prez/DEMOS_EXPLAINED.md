# The three demos, explained simply

This document explains what each live demo does, why it is in the talk, and what is going on under
the hood, in plain language. It assumes you can read a little code but know nothing about assembly.
The exact commands and timings are in [demo/DEMO_SCRIPT.md](demo/DEMO_SCRIPT.md); this is the "why and how it works".

## The big idea in one minute

A program you run is a long list of tiny instructions the CPU understands. Nobody writes those by hand. A
programmer writes readable code (C, for example) and a **compiler** translates it into instructions. The
compiler throws away the things that make code readable: names, comments, structure.

**Reverse engineering** is working backwards from the instructions to the meaning. The three demos
follow one small function through three levels of help:

| Demo | You use | The point |
|---|---|---|
| 1. By hand | A hex editor and a lookup table | It is possible, but painful. Information is lost. |
| 2. Ghidra | A tool that reads the instructions for you | It saves huge effort, but it can only guess at meaning. |
| 3. Emulator | A program that *runs* the original instructions | You can check a guess against reality. |

The same ten-line function is used in all three, so the audience follows one thread.

---

## The example function

File: [demo/sum_clamped.c](demo/sum_clamped.c)

```c
int sum_clamped(const int *a, int n, int lo, int hi)
{
    int total = 0;
    for (int i = 0; i < n; i++) {
        int v = a[i];
        if (v < lo) v = lo;
        else if (v > hi) v = hi;
        total += v;
    }
    return total;
}
```

In words: go through a list of `n` numbers. Force each number to stay between `lo` and `hi` (too small
becomes `lo`, too big becomes `hi`). Add them all up and return the total.

It was chosen because it is small but has the three things every real function has: a **loop**, a
**decision** (`if`), and an **accumulator** (`total`). It is also original code written for the talk, so it contains
nothing from the game.

### How it is built

File: [demo/build.sh](demo/build.sh). It runs the compiler twice:

- `-O0` means "do not optimise". The result is a clumsy but faithful translation, easy to follow. **112 bytes.**
- `-O2` means "optimise". The compiler rewrites the function to run fast. **303 bytes.**

Two details worth knowing:

- It compiles for **32-bit x86**, the same CPU family as the 1990s game. The instructions you see in the demo
  are the same kind you would find in the real game, not a modern Mac or phone instruction set.
- The compiler produces an *object file* (a wrapper around the code). The helper
  [demo/extract_text.py](demo/extract_text.py) cuts out just the code (the `.text` part) into a plain file of bytes, such as
  `build/sum_clamped_O0.bin`. That bare file is what you open in the hex editor.

---

## Demo 1: Bytes and opcodes (by hand)

### What you do
1. Show the C source.
2. Open the `.bin` file in a hex editor (or run `xxd`). The audience sees only numbers like `55 89 e5 83 ec 0c`.
3. Decode a few of them with a table, then show that `objdump` does the same thing automatically.
4. Show the `-O2` version, which is a very different (and longer) set of bytes.

### What a hex editor shows
Every file is just bytes. A hex editor shows each byte as two hex digits (`00` to `ff`). A byte is a number from 0 to 255,
so `55` in hex is 85 in everyday numbers. Nothing in the file says "this part is code". You know because you
chose the right file.

### What an "opcode" is
The CPU reads bytes one after another. The first byte of each instruction (the **opcode**) tells it what to do, and
the bytes after it say what to do it *with*. For example, `8b 45 14` means "load into `eax` the number stored
at `ebp+0x14`": `8b` is "load", `45` says "into `eax`, from `ebp` plus a small offset", and `14` is that offset.

### A few instructions from the `-O0` function
Nine of the 39 instructions in `build/sum_clamped_O0.bin` (112 bytes), enough to get the idea:

| Offset | Bytes | Instruction | Plain meaning |
|---|---|---|---|
| `0x00` | `55` | `push ebp` | Save the caller's bookmark (start of every function) |
| `0x01` | `89 e5` | `mov ebp, esp` | Set my own bookmark into the stack |
| `0x03` | `83 ec 0c` | `sub esp, 0xc` | Reserve 12 bytes for 3 local variables |
| `0x12` | `c7 45 fc 00 00 00 00` | `mov [ebp-4], 0` | `total = 0` |
| `0x23` | `3b 45 0c` | `cmp eax, [ebp+0xc]` | Compare `i` with `n` |
| `0x26` | `7d 40` | `jge 0x68` | If `i >= n`, jump to the end |
| `0x2e` | `8b 04 88` | `mov eax, [eax+ecx*4]` | Read `a[i]` (each int is 4 bytes) |
| `0x66` | `eb b8` | `jmp 0x20` | Jump back to the loop test |
| `0x6f` | `c3` | `ret` | Go back to the caller |

How to read it:
- **Offsets** are where each instruction starts in the file. Jumps point at these offsets (`jge 0x68` means "continue at offset 0x68").
- **`[ebp+...]` is an argument, `[ebp-...]` is a local variable** (see the diagram below): `ebp+8` is `a`, `+0xc` is `n`, `+0x10` is `lo`,
  `+0x14` is `hi`; `ebp-4` is `total`, `ebp-8` is `i`, `ebp-0xc` is `v`.
- The `jmp 0x20` at `0x66` is what turns the code into a loop: it goes back to the comparison at `0x23`.

### Some words you will need
- **Register:** one of a few super-fast storage slots inside the CPU. `eax` is the one that usually holds the
  result. Think of them as the CPU's hands.
- **The stack:** a scratch area of memory the function uses. Arguments are passed on it, and local
  variables live on it.
- **`ebp`:** a register used as a fixed bookmark into the stack so the function can say "the second
  argument is 12 bytes above my bookmark".

### Where the function's data lives (the one diagram worth drawing)

```
higher addresses
   ...
   [ebp+0x14]   hi   (4th argument)
   [ebp+0x10]   lo   (3rd argument)
   [ebp+0x0c]   n    (2nd argument)
   [ebp+0x08]   a    (1st argument, a pointer to the numbers)
   [ebp+0x04]   return address (where to go back to)
   [ebp+0x00]   saved ebp
   [ebp-0x04]   total  (local variable)
   [ebp-0x08]   i      (local variable)
   [ebp-0x0c]   v      (local variable)
lower addresses
```

This is why the demo says "arguments live at `ebp+8`, `+0xc`, `+0x10`, `+0x14`". The calling rule that puts them there is called
**cdecl**: the caller pushes the arguments from last to first, then the return address.

Once you know this, the code reads like English. `cmp eax, [ebp+0xc]` is "compare `i` against `n`".

### How the jump is read
In `7d 40`, the `40` is a *distance*, not an address. The jump is measured from the end of the jump instruction.
The instruction sits at offset `0x26` and is 2 bytes long, so it ends at `0x28`. Add `0x40` and you get `0x68`, which is the
line that loads `total` to return it. That is the loop exit.

### A small oddity you might be asked about
At the start of the `-O0` code there are four `mov eax, [...]` lines in a row that load the four arguments and
then ignore them. That is an artefact of "no optimisation": the compiler does the literal thing, even when it is useless. It is a nice
example of why compiler output is not the same as how a person would write assembly.

### The compiler reveal (`-O2`)
Same C code, but now the bytes are 303 long. Why bigger? The compiler noticed the loop does the same thing to every
number and used the CPU's **SIMD** (also called SSE) instructions: they work on **8 numbers at a time**. Names like `pcmpgtd`
(compare four numbers at once) and `paddd` (add four numbers at once) show this. It then adds a second, ordinary loop for any
leftover numbers.

The takeaway for the audience: the loop you wrote no longer exists in the file. The behaviour is the same, but the
structure is gone. That is why reverse engineering is not just "read it".

### Running it live (so the demo is not just reading)

The talk would feel flat if nothing ever ran, so Demo 1 now has two live moments.

**Run the source natively** with `./run_native.sh`. This compiles the same C for your own Mac and prints
`55` (the sum of 1 to 10). It proves the function works and gives you a known answer to compare against.

**Run the exact bytes** with `run.py`. The 32-bit Windows bytes cannot run directly on a Mac (see the note on CPUs below),
so the emulator from Demo 3 acts as the CPU. You can type any inputs:

```
python run.py build/sum_clamped_O0.bin --array=-50,5,50 --lo=-10 --hi=10      ->  5
```

The `-O0` and `-O2` builds give the same answer with very different instruction counts.

**Why not make an EXE?** A function alone is not a program (no `main`, no startup code), and a Mac cannot launch a 32-bit
Windows EXE anyway; that would need Wine or a Windows machine. Emulating only the CPU is simpler and is what the real project does.

**Patch a byte live.** Open a copy of the `-O0` file in the hex editor, go to offset `0x62`, and change `83 c0 01`
to `83 c0 02`. That instruction is `add eax, 1`, the `i++` at the bottom of the loop. Now it is `i += 2`. Run it again:
the answer drops from 55 to 25, because only elements 1, 3, 5, 7 and 9 get added. You changed what a compiled program does with
one byte and no source code. This is the clearest way to show that the bytes really are the program.

### What the audience should remember
- Everything is in there, but with no names, types or structure.
- Doing this by hand is possible and slow.
- Optimisation makes it harder, not easier.

---

## Demo 2: The same function in Ghidra

### What Ghidra is
Ghidra is a free tool from the NSA (released publicly) that does three things for you:

1. **Disassembles:** turns bytes into readable instructions (like `objdump`, but interactive).
2. **Finds structure:** works out where functions start and end, and which functions call which.
3. **Decompiles:** shows a guess of the original C, so you read `for` loops and `if` statements instead of jumps.

### What you do in the demo

You import the **raw bytes** (`build/sum_clamped_O0.bin`), not the `.obj` file. That is on purpose. An object file
carries the function's name and where it starts and ends. In a real situation you would not have any of that: a shipped
program has no label for a function you have not identified yet. The raw file is the honest starting point, where Ghidra
knows only the bytes.

1. Create a project and **import** the `.bin`. Ghidra cannot tell what kind of file it is, so you choose:
   format **Raw Binary**, language **x86, little endian, 32-bit**. Keep the base address at 0, so the offsets match the table above.
2. Open it. At first you see only bytes. Ghidra has not found a function, because a raw file has no entry point or call targets to follow.
3. Click the first byte and press `D` (disassemble), then `F` (create function). Ghidra names it `FUN_00000000`
   and shows two panels side by side: the **listing** (instructions) and the **decompiler** (C-like code).
   In a real EXE this step mostly happens automatically; here you do it by hand to see what Ghidra is doing for you.
4. Notice what the decompiler gives you for free: the loop, the `if / else`, and a rough idea of the types.
5. Notice what it does *not* give you: meaningful names. The function is `FUN_00000000` and the variables are called things like
   `param_1`, `param_2` and `local_c` ("the first argument", "a local at some offset").
6. **Rename** them: the function becomes `sum_clamped`, `param_1` becomes `values`, `param_2` becomes `count`,
   `param_3` becomes `lo`, `param_4` becomes `hi`, and the accumulator becomes `total`. In Ghidra, click a name and press `L` to rename it.
   The decompiler output updates everywhere at once.
7. Optionally open the `-O2` file to show Ghidra coping with the SIMD version (messier, but still useful).
8. Then switch to the real game project (Part B below).

### Part B: the same tool on the real game

The raw-bytes part deliberately takes away what Ghidra normally provides. To show its real potential, the demo then
switches to the project with `TESORO.EXE` loaded. A real program has an **entry point**, a table of **imports** (the
Windows services it uses) and thousands of calls between functions, so Ghidra can follow them and find the functions by itself.
What you show, and what each shows the audience:

| Show | What it demonstrates |
|---|---|
| The function list | Hundreds of functions found without anyone creating them |
| A short function and its references | Who calls whom, for free |
| The call graph | The structure at a glance |
| Defined strings and imports | The text the game uses, and the Windows services it asks for |

**The honest caveat.** Ghidra finds a function by following a call to it. Code that is only reached through a table of
function pointers (virtual methods are the common case) is never called directly, so Ghidra misses it. In this project the function
count went from **622 to 784** after 161 of those were defined with a script (`re/PLAN.md`). Ghidra does most of the work, not all of it.

### Why this demo is in the talk
It shows what a good tool buys you and, just as important, what it cannot buy you. Ghidra recovers *structure*.
It cannot recover *meaning*: `param_2` is a number, but only a person (or an agent that reads the surrounding code)
can decide it is "how many items". That gap is exactly where the AI part of the talk fits.

### The scale problem (the last step)
The tiny function took minutes with 12 lines. The game has 706 functions in the ledger (447 of them game code), many much longer.
One person naming everything by hand is the bottleneck.

### Honest limits of Ghidra's output
- It is a **guess**. It may pick the wrong type (an `int` that is really a pointer) or get signed versus unsigned wrong.
- Optimised code gives noisier output.
- So how do you know a reading is right? That question leads straight into Demo 3.

### Practical note
The raw-binary import has not been rehearsed on this machine yet, so the exact dialog and the `D` then `F` steps are unverified. Do it once before the talk, and take a
screenshot as a backup in case something is slow on the day. If it gives trouble, the `.obj` file imports with the name and function already found; it is quicker but less realistic, so say so if you use it.

---

## Demo 3: Check the guess with an emulator

### The problem it solves
You now have a *claim* about what the function does: "it clamps each number then sums them". How do you test that
claim when you do not have the original source? You **run the original machine code** and see what it really does.

### What an emulator is
An emulator is a program that pretends to be a CPU. You give it the function's bytes, set up some memory,
and it executes the instructions one by one without needing the game, the operating system or any other part of the
program. The one used here is called **Unicorn**.

Think of it as a lab bench: you put one machine part on it, feed it inputs, and read the output.

### What the script does
File: [demo/verify.py](demo/verify.py). In plain steps:

1. **Load the bytes** into the emulator's memory at a made-up address (`0x10000000`).
2. **Create a stack** (at `0x20000000`) and a data area (at `0x30000000`) where the list of numbers will live.
3. **Set up the call exactly as a real caller would** (this is the cdecl rule from Demo 1): push `hi`, `lo`, `n`, then the pointer
   to the numbers, then a return address. The return address (`0x40000000`) is a fake spot; when the function tries to go
   there, the emulator stops.
4. **Run.** The function executes its real instructions.
5. **Read `eax`**, which holds the answer.
6. **Compare** the answer with what a simple Python version of the claim would produce, for 305 different test inputs.

It also counts how many instructions each call used. That is why you see **445** per call for the `-O0` build and
**148** for the `-O2` build: same answers, very different amounts of work.

### The test inputs
- 5 hand-picked cases (empty list, one item, items beyond the limits, and one where `lo` is bigger than `hi`).
- 300 random cases from a fixed seed (so every run gives the same result).

Total: 305.

### The three claims being tested

**Claim 1: "faithful".** A Python function that follows the C exactly (same order of checks, same wrap-around behaviour for
big numbers).
Result: **305 out of 305 match**, for both `-O0` and `-O2`. The reading is right, and it holds even through the
optimiser's SIMD rewrite.

**Claim 2: "plausible" (the trap).** A cleaner-looking version: clamp each number with `min(max(v, lo), hi)`. Most people would say it is the same thing.
It is not, when `lo` is larger than `hi`.

Take the list `[1, 5, 9, 20]` with `lo = 8` and `hi = 2` (a silly range, but legal):

- The original: 1 and 5 are below 8, so each becomes 8. 9 and 20 are not below 8, so they hit the "above `hi`" rule and become 2.
  Total: 8 + 8 + 2 + 2 = **20**. This is what the emulator reports.
- The plausible version: each number is first pushed up to at least 8, then pulled down to at most 2, so every number ends up as 2.
  Total: 2 + 2 + 2 + 2 = **8**. This is the `hypothesis=8` in the output.

The two versions give different answers (20 versus 8) because the original checks "too small" first and stops there, while the
plausible version always applies the upper limit last.

**Claim 3: "plausible", but tested only with sensible ranges.** If you only test cases where `lo` is not bigger than `hi`, the plausible version
passes all 154 of them. This is the teaching moment: a test suite that avoids the awkward case gives false confidence.

### The commands and what to say

| Command | What you see | What to say |
|---|---|---|
| `verify.py build/sum_clamped_O2.bin --hypothesis faithful` | `305/305 cases match` | "The claim holds, even for the optimised code." |
| `... --hypothesis plausible --happy-path-only` | `154/154 cases match` | "A reasonable-looking claim passes... if I only test the easy cases." |
| `... --hypothesis plausible` | `162/305` plus a `MISMATCH` line | "Add the awkward case and it fails, and I can see exactly which input broke it." |

### Why this demo is in the talk
- It makes "decompiler output is a hypothesis" concrete.
- It introduces the **oracle**: the real code is the judge, not whoever (or whatever) made the claim.
- It sets up the AI section: an AI explanation can sound confident and still be wrong. The emulator does not care
  how confident it sounds.
- It teaches the second lesson: the quality of the check depends on the inputs you choose. Edge cases matter.

### What it does not prove
- Only the inputs that were tried are covered. Passing is strong evidence, not a proof.
- Functions that talk to the operating system or hardware cannot run on their own; their calls need stand-ins.
- In the real project this is exactly how functions are checked, but against the actual game code, with more elaborate setup.

---

## How the three demos connect to the AI section

The AI part of the talk is the same process scaled up:

| In the demos | In the real project |
|---|---|
| You read the bytes and decoded by hand | The agent reads the decompiler output through Ghidra's tools |
| You renamed variables in Ghidra | The agent proposes names and records them |
| You wrote the "claim" in Python | The agent writes a contract and a C++ port |
| `verify.py` compared the claim with the original bytes | The same kind of emulator check runs against the real game code |
| You remembered what was checked | The ledger records what is trusted and why |

The message to land: **the AI proposes, the emulator checks, the ledger records.**

---

## Glossary

| Term | Plain meaning |
|---|---|
| Binary | The compiled program file; the thing you have when you don't have the source |
| Opcode | The first byte of an instruction; says what to do |
| Register | A tiny storage slot inside the CPU (`eax`, `ebp`, `esp`...) |
| Stack | Scratch memory a function uses for arguments and local variables |
| Compiler | The program that turns readable code into machine instructions |
| Optimisation (`-O2`) | The compiler rewriting code to run faster, at the cost of readability |
| SIMD / SSE | CPU instructions that work on several numbers at once |
| Disassembler | A tool that turns bytes into readable instructions |
| Decompiler | A tool that turns instructions back into C-like code (a guess) |
| Ghidra | A free tool that does both, with cross-references and renaming |
| Emulator | A program that pretends to be a CPU, so you can run bytes on their own |
| Unicorn | The emulator library used here |
| Hypothesis | A claim about what a function does, to be tested |
| Oracle | The thing you trust as the judge (here, the original machine code) |
| cdecl | The rule for how arguments are passed: pushed right to left on the stack |

