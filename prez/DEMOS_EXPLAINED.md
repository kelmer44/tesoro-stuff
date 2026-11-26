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

The same five-line function, typed live, is used in all three, so the audience follows one thread.

---

## The example function

You type it live into `live/sum_to.c` (a backup copy is in [demo/fallback/sum_to.c](demo/fallback/sum_to.c)):

```c
int sum_to(int n)
{
    int total = 0;
    for (int i = 1; i <= n; i++)
        total += i;
    return total;
}
```

In words: add up 1 + 2 + ... + n. For `n = 10` the answer is 55.

It was chosen because it is small enough to type live, yet has what every real function has: a **loop**, a
**comparison**, an **accumulator** (`total`) and a **return value**. It is also original code written for the talk, so it contains
nothing from the game.

### How it is built

File: [demo/build.sh](demo/build.sh). Run it as `./build.sh live/sum_to.c`. It runs the compiler twice:

- `-O0` means "do not optimise". The result is a clumsy but faithful translation, easy to follow. **59 bytes, 20 instructions.**
- `-O2` means "optimise". The compiler rewrites the function to run fast. **28 bytes, 12 instructions.**

Two details worth knowing:

- It compiles for **32-bit x86**, the same CPU family as the 1990s game. The instructions you see in the demo
  are the same kind you would find in the real game, not a modern Mac or phone instruction set.
- The compiler produces an *object file* (a wrapper around the code). The helper
  [demo/extract_text.py](demo/extract_text.py) cuts out just the code (the `.text` part) into a plain file of bytes, such as
  `build/sum_to_O0.bin`. That bare file is what you open in the hex editor.

The build script and the extractor are prewritten helpers. The function itself is the part you type.

---

## Demo 1: Bytes and opcodes (by hand)

### What you do
1. Type the function.
2. Compile it for your Mac and run it to get a known answer (`55`). Then compile the same C again for x86-win32, the
   kind of code the game contains. These are different bytes, and the Mac cannot run them. `file` shows the difference.
3. Dump the `.bin` file in the terminal with `xxd -g1`, then open it in a hex editor (ImHex). The audience sees only numbers like `55 89 e5 83 ec 08`.
4. Decode a few of them with a table, then show that `objdump` does the same thing automatically.
5. Run the exact bytes in an emulator, show the `-O2` version, and finally patch a byte and watch the answer change.

### What a hex editor shows
Every file is just bytes. A hex editor shows each byte as two hex digits (`00` to `ff`). A byte is a number from 0 to 255,
so `55` in hex is 85 in everyday numbers. Nothing in the file says "this part is code". You know because you
chose the right file.

**Why hex and not binary or decimal?** One byte is 8 bits, which is exactly two hex digits (each digit is 4 bits), so every
byte is two characters and converts to bits in your head. Binary needs 8 characters per byte and is unreadable. Decimal does not line
up with bits: `7f` shows at a glance that the top bit is clear, while 127 does not. The patterns show up in hex too: `50` to `57`
are all `push`, and `55` is `push ebp` (`50` + register number 5).

### What an "opcode" is
The CPU reads bytes one after another. The first byte of each instruction (the **opcode**) tells it what to do, and
the bytes after it say what to do it *with*. For example, `8b 45 08` means "load into `eax` the number stored
at `ebp+8`": `8b` is "load", `45` says "into `eax`, from `ebp` plus a small offset", and `08` is that offset.

### A few instructions from the `-O0` function
Nine of the 20 instructions in `build/sum_to_O0.bin` (59 bytes), enough to get the idea:

| Offset | Bytes | Instruction | Plain meaning |
|---|---|---|---|
| `0x00` | `55` | `push ebp` | Save the caller's bookmark (start of every function) |
| `0x03` | `83 ec 08` | `sub esp, 8` | Reserve 8 bytes for 2 local variables |
| `0x09` | `c7 45 fc 00 00 00 00` | `mov [ebp-4], 0` | `total = 0` |
| `0x10` | `c7 45 f8 01 00 00 00` | `mov [ebp-8], 1` | `i = 1` |
| `0x1a` | `3b 45 08` | `cmp eax, [ebp+8]` | Compare `i` with `n` |
| `0x1d` | `7f 14` | `jg 0x33` | If `i > n`, jump to the end |
| `0x2b` | `83 c0 01` | `add eax, 1` | The `i++` |
| `0x31` | `eb e4` | `jmp 0x17` | Jump back to the loop test |
| `0x3a` | `c3` | `ret` | Go back to the caller |

How to read it:
- **Offsets** are where each instruction starts in the file. Jumps point at these offsets (`jg 0x33` means "continue at offset 0x33").
- **`[ebp+...]` is an argument, `[ebp-...]` is a local variable** (see the diagram below): `ebp+8` is `n`, `ebp-4` is `total`, `ebp-8` is `i`.
- The `jmp 0x17` at `0x31` is what turns the code into a loop: it goes back to the comparison at `0x1a`.

### Some words you will need
- **Register:** one of a few super-fast storage slots inside the CPU. `eax` is the one that usually holds the
  result. Think of them as the CPU's hands.
- **The stack:** a scratch area of memory the function uses. Arguments are passed on it, and local
  variables live on it.
- **`ebp`:** a register used as a fixed bookmark into the stack so the function can say "the argument is 8 bytes above my bookmark".

### Where the function's data lives (the one diagram worth drawing)

```
higher addresses
   ...
   [ebp+0x08]   n    (the only argument)
   [ebp+0x04]   return address (where to go back to)
   [ebp+0x00]   saved ebp
   [ebp-0x04]   total  (local variable)
   [ebp-0x08]   i      (local variable)
lower addresses
```

The calling rule that puts the argument there is called **cdecl**: the caller pushes the arguments from last to first,
then the return address. With one argument that is simply: `n`, then the return address.

Once you know this, the code reads like English. `cmp eax, [ebp+8]` is "compare `i` against `n`".

### How the jump is read
In `7f 14`, the `14` is a *distance*, not an address. The jump is measured from the end of the jump instruction.
The instruction sits at offset `0x1d` and is 2 bytes long, so it ends at `0x1f`. Add `0x14` and you get `0x33`, which is the
line that loads `total` to return it. That is the loop exit. The jump back, `eb e4`, works the same way: `e4` is -28,
and `0x33 - 28 = 0x17`, the start of the loop test.

### The whole function, line by line
All 20 instructions of `build/sum_to_O0.bin`, in the order you would narrate them, with the C line each one comes from
(the same offsets and wording as the labels in `demo/sum_to_O0.hexpat`). Reminder: `ebp+8` is `n`, `ebp-4` is `total`,
`ebp-8` is `i`, and `eax` is the CPU's working hand.

Two groups are boilerplate that nearly every function has: the **prologue** at the start and the **epilogue** at the end
(blocks 1 and 6 below). They have no C line of their own; the compiler adds them to set up and tear down the function's
workspace. Everything in between is the function's real work.

**1. Prologue: set up the function's workspace** (no C line; every function starts like this)

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x00` | `55` | `push ebp` | Save the caller's bookmark so it can be restored at the end |
| `0x01` | `89 e5` | `mov ebp, esp` | Make `ebp` point at this function's own bookmark position |
| `0x03` | `83 ec 08` | `sub esp, 8` | Reserve 8 bytes of stack for the two local variables |

The first real instruction comes right after it, and is not part of the prologue:

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x06` | `8b 45 08` | `mov eax, [ebp+8]` | Load `n` into `eax`. Nothing uses it (it is overwritten at `0x17`): an artefact of no optimisation |

**2. Setup: `int total = 0;` and `int i = 1;`**

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x09` | `c7 45 fc 00 00 00 00` | `mov [ebp-4], 0` | `total = 0` |
| `0x10` | `c7 45 f8 01 00 00 00` | `mov [ebp-8], 1` | `i = 1` |

**3. The loop test: `i <= n`** (this is where the jump back lands)

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x17` | `8b 45 f8` | `mov eax, [ebp-8]` | Load `i` |
| `0x1a` | `3b 45 08` | `cmp eax, [ebp+8]` | Compare `i` with `n` (sets flags, changes nothing else) |
| `0x1d` | `7f 14` | `jg 0x33` | If `i > n`, the loop is over: jump to the exit at `0x33` |

**4. The loop body: `total += i;`**

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x1f` | `8b 45 f8` | `mov eax, [ebp-8]` | Load `i` |
| `0x22` | `03 45 fc` | `add eax, [ebp-4]` | Add `total` to it |
| `0x25` | `89 45 fc` | `mov [ebp-4], eax` | Store the sum back into `total` |

**5. The step and the loop back: `i++`**

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x28` | `8b 45 f8` | `mov eax, [ebp-8]` | Load `i` |
| `0x2b` | `83 c0 01` | `add eax, 1` | Add 1 |
| `0x2e` | `89 45 f8` | `mov [ebp-8], eax` | Store it back into `i` |
| `0x31` | `eb e4` | `jmp 0x17` | Go back to the loop test |

**6. Exit: `return total;`, then the epilogue**

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x33` | `8b 45 fc` | `mov eax, [ebp-4]` | Put `total` in `eax`, where the caller expects the return value (this is `return total;`, not epilogue) |

The epilogue undoes the prologue, in reverse order:

| Offset | Bytes | Instruction | What it does |
|---|---|---|---|
| `0x36` | `83 c4 08` | `add esp, 8` | Release the two local variables |
| `0x39` | `5d` | `pop ebp` | Restore the caller's bookmark |
| `0x3a` | `c3` | `ret` | Go back to the caller |

Why it works as a loop: the test at `0x17` is reached first by falling through from the setup, and every later time by the
`jmp` at `0x31`. The only way out is the `jg` at `0x1d`. One pass is the test (3 instructions), the body (3) and the step (4),
so 10 instructions. That is why `n = 10` takes 113 instructions in the emulator: 6 to get started (entry and setup), 10 passes of 10,
a final test of 3 that exits, and 4 at the exit.

### A small oddity you might be asked about
Right after the function start there is a `mov eax, [ebp+8]` that loads `n` and then ignores it (offset `0x06`). That is an
artefact of "no optimisation": the compiler does the literal thing, even when it is useless. It is a nice
example of why compiler output is not the same as how a person would write assembly.

### Running it live (so the demo is not just reading)

**Run the source natively**: build it with `./build_native.sh live/sum_to.c`, then run `./build/native_demo 10`. The build compiles the same C for your own Mac and the run prints
`55`. It proves the function works and gives you a known answer to compare against.

**Run the exact bytes** with `run.py`. The 32-bit Windows bytes cannot run directly on a Mac (your Mac has an arm64 CPU and runs macOS),
so the emulator from Demo 3 acts as the CPU. You can ask the audience for any `n`:

```
python run.py build/sum_to_O0.bin 1000      ->  500500   [10013 instructions]
```

The number in brackets is how many instructions the emulator **executed** during this one call (counted as it ran). It is not
the 20 instructions in the file: a loop runs the same instructions again and again. For `n = 1000` that is 6 to get started,
1000 passes of 10, a final test of 3 and an exit of 4, which makes 10,013. A bigger `n` means more passes and a bigger count.
At `-O2` it stays at 10 for any positive `n`, because the loop is gone. That is the compiler reveal below.

**Why not make an EXE?** A function alone is not a program (no `main`, no startup code), and a Mac cannot launch a 32-bit
Windows EXE anyway; that would need Wine or a Windows machine. Emulating only the CPU is simpler and is what the real project does.

### The compiler reveal (`-O2`)
Same C code, but now the file is 28 bytes, less than half. The loop is gone. The compiler recognised that adding up
1 + 2 + ... + n has a known formula and replaced the whole loop with a few arithmetic instructions: multiply, halve, add.
Run it for `n = 1000` and it needs **10 instructions instead of 10,013**, with the same answer.

**The formula.** In the talk, say the compiler replaced the loop with `n(n+1)/2` (for `n = 10`: 10 × 11 / 2 = 55).

Speaker-only caveat, in case someone reads the code: the exact arrangement the compiler uses is `(n-1)(n-2)/2 + 2n - 1`, which is
algebraically equal to `n(n+1)/2` for `n > 0`. In the code that is `lea n-1`, `lea n-2`, `mul`, `shld` (halve), `lea` (add `2n`) and `dec`.
"Multiply, halve, add" is still an accurate description.

It kept one thing: a check at the top (`test ecx, ecx` then `jle`) that returns 0 when `n` is zero or negative. A loop from
1 to a negative number never runs, so the answer must be 0, and the compiler preserved that. Remember it for Demo 3.

The takeaway for the audience: the loop you wrote no longer exists in the file. The behaviour is the same, but the
structure is gone. That is why reverse engineering is not just "read it".

### Patching a byte (the hex editor earns its place)
Open a **copy** of the `-O0` file in the hex editor (it writes changes into whatever file you open). Two edits, one at a time:

- At offset `0x1d`, change `7f` to `7d`. That is `jg` ("jump if greater") becoming `jge` ("jump if greater or equal"), so the loop test
  `i <= n` becomes `i < n`. The answer for `n = 10` drops from 55 to **45** (the last number, 10, is no longer added).
- At offset `0x2d`, change `01` to `02`. The instruction `add eax, 1` (the `i++`) becomes `add eax, 2`, so `i` goes up by 2.
  The answer drops to **25**, because only 1, 3, 5, 7 and 9 get added.

You changed what a compiled program does with one byte and no source code. This is the clearest way to show that the bytes really are the program.
If the code you typed compiles to different bytes, `patch_demo.sh` notices and tells you instead of patching the wrong place.

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
3. **Decompiles:** shows a guess of the original C, so you read `for` loops instead of jumps.

### What you do in the demo

You import the **raw bytes** (`build/sum_to_O0.bin`), not the `.obj` file. That is on purpose. An object file
carries the function's name and where it starts and ends. In a real situation you would not have any of that: a shipped
program has no label for a function you have not identified yet. The raw file is the honest starting point, where Ghidra
knows only the bytes.

1. Create a project and **import** the `.bin`. Ghidra cannot tell what kind of file it is, so you choose:
   format **Raw Binary**, language **x86, little endian, 32-bit**. Keep the base address at 0, so the offsets match the table above.
   (In a real program you would not have to choose: an EXE carries a header that says which CPU it is for, so Ghidra picks the language itself.)
2. Open it. At first you see only bytes. Ghidra has not found a function, because a raw file has no entry point or call targets to follow.
   (Again, a real EXE is different: its header gives Ghidra the entry point, and from there it follows calls and finds functions on its own.)
3. Click the first byte and press `D` (disassemble), then `F` (create function). Ghidra names it `FUN_00000000`
   and shows two panels side by side: the **listing** (instructions) and the **decompiler** (C-like code).
   In a real EXE this step mostly happens automatically; here you do it by hand to see what Ghidra is doing for you.
4. Notice what the decompiler gives you for free: the `for` loop, and a rough idea of the types.
5. Notice what it does *not* give you: meaningful names. The function is `FUN_00000000` and the variables are called things like
   `param_1` and `local_c` ("the first argument", "a local at some offset").
6. **Rename** them: the function becomes `sum_to`, `param_1` becomes `n`, and the two locals become `total` and `i`.
   In Ghidra, click a name and press `L` to rename it. The decompiler output updates everywhere at once.
7. Optionally open the `-O2` file. Expect arithmetic instead of a loop, because the loop no longer exists.
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

**What that means, with an example.** A *table of function pointers* is a list of function addresses stored as data, which the
program looks up and calls:

```c
void do_open(void) { puts("open"); }
void do_look(void) { puts("look"); }
void do_take(void) { puts("take"); }

void (*verbs[])(void) = { do_open, do_look, do_take };   // the table: 3 addresses

void run(int choice) {
    verbs[choice]();          // call whichever one the player picked
}
```

In the compiled code, `run` has one instruction, roughly `call [verbs + choice*4]`: "call whatever address is in that slot".
No instruction says `call do_open` directly, so Ghidra never learns that `do_open` is a function.

A **virtual method** (C++) is the same idea, built in by the compiler. Each class with virtual methods gets a hidden table of
function pointers, a *vtable*, and every object starts with a hidden pointer to its class's vtable:

```cpp
struct Shape  { virtual void draw() = 0; };
struct Circle : Shape { void draw() override { /* draw a circle */ } };
struct Square : Shape { void draw() override { /* draw a square */ } };

void render(Shape *s) {
    s->draw();      // circle or square? Decided when the program runs
}
```

`s->draw()` compiles to "load the object's vtable pointer, then call whatever is in slot 0". `Circle::draw` and `Square::draw` are
only reached that way, so Ghidra misses them. A point-and-click adventure fits this pattern well: each kind of game object has
its own handlers for "look at", "use" and "talk", and the engine calls them through a table without knowing which object it has.

### Why this demo is in the talk
It shows what a good tool buys you and, just as important, what it cannot buy you. Ghidra recovers *structure*.
It cannot recover *meaning*: `param_1` is a number, but only a person (or an agent that reads the surrounding code)
can decide it is "how many numbers to add". That gap is exactly where the AI part of the talk fits.

### The scale problem (the last step)
The tiny function took minutes with a handful of lines. The game has 706 functions in the ledger (447 of them game code), many much longer.
One person naming everything by hand is the bottleneck.

### Honest limits of Ghidra's output
- It is a **guess**. It may pick the wrong type (an `int` that is really a pointer) or get signed versus unsigned wrong.
- Optimised code gives noisier output.
- So how do you know a reading is right? That question leads straight into Demo 3.

### Practical note
The raw-binary import has not been rehearsed on this machine yet, so the exact dialog, the `D` then `F` steps and what the decompiler prints are unverified.
Do it once before the talk, and take a screenshot as a backup in case something is slow on the day. If it gives trouble, the `.obj` file imports with the name
and function already found; it is quicker but less realistic, so say so if you use it.

---

## Demo 3: Check the guess with an emulator

### The problem it solves
You now have a *claim* about what the function does. Reading the `-O2` version, someone might say: "the compiler turned it into a formula,
so it is just `n*(n+1)/2`." How do you test that claim when you do not have the original source? You **run the original machine code** and see what it really does.

### What an emulator is
An emulator is a program that pretends to be a CPU. You give it the function's bytes, set up some memory,
and it executes the instructions one by one without needing the game, the operating system or any other part of the
program. The one used here is called **Unicorn**.

Think of it as a lab bench: you put one machine part on it, feed it inputs, and read the output.

### What the helper scripts do
Files: [demo/emu.py](demo/emu.py) (the runner) and [demo/verify.py](demo/verify.py) (the test loop). In plain steps:

1. **Load the bytes** into the emulator's memory at a made-up address (`0x10000000`).
2. **Create a stack** (at `0x20000000`).
3. **Set up the call exactly as a real caller would** (this is the cdecl rule from Demo 1): push `n`, then a return address. The return
   address (`0x40000000`) is a fake spot; when the function tries to go there, the emulator stops.
4. **Run.** The function executes its real instructions.
5. **Read `eax`**, which holds the answer.
6. **Compare** the answer with what your claim says, for 133 different values of `n`.

It also counts how many instructions each call used. That is why you see **10013** instructions for `n = 1000` in the `-O0` build and
**10** in the `-O2` build: same answer, very different amounts of work.

### The test inputs
- `n` from 0 to 100, plus 500 and 1000 (103 values, the "happy path").
- `n` from -1 down to -30 (30 more values, the awkward case): a negative `n`.

Total: 133. The claim itself is a Python expression **you type on the command line**, with the variable called `n`.

### The claims being tested

**Claim 1: "loop-shaped".** `sum(range(1, n+1))` is the loop written as Python.
Result: **133 out of 133 match**, for both `-O0` and `-O2`. The reading is right, and it holds even though the optimiser deleted the loop.

**Claim 2: "the formula" (the trap).** `n*(n+1)//2` looks like the obvious summary of what the `-O2` code does. Most people would say it is the same thing.
It is not, when `n` is negative.

Take `n = -2`:

- The original: the loop starts at 1 and runs while `i <= -2`. That is false straight away, so the loop never runs and the answer is **0**.
  This is what the emulator reports, for both builds.
- The formula: `(-2) * (-1) / 2 = 1`. This is the `hypothesis=1` in the output.

(`n = -1` happens to give 0 with the formula too, so a single lucky test would not catch it.)

**Claim 3: "the formula", but tested only on sensible values.** If you only test `n` from 0 up, the formula
passes all 103 of them. This is the teaching moment: a test suite that avoids the awkward case gives false confidence.
And the `-O2` code had the clue all along: the `test` / `jle` at the top returns 0 for `n` of zero or less, a guard the formula does not have.

### The commands and what to say

| Command | What you see | What to say |
|---|---|---|
| `verify.py build/sum_to_O2.bin --expr "sum(range(1, n+1))"` | `133/133 inputs match` | "My loop-shaped claim holds, even for the optimised code." |
| `... --expr "n*(n+1)//2" --happy-path-only` | `103/103 inputs match` | "A reasonable-looking claim passes... if I only test the easy cases." |
| `... --expr "n*(n+1)//2"` | `104/133` plus `MISMATCH n=-2: emulator=0 hypothesis=1` | "Add the awkward case and it fails, and I can see exactly which input broke it." |
| `... --expr "n*(n+1)//2 if n > 0 else 0"` | `133/133 inputs match` | "The compiler wrote an `if n <= 0 return 0` guard and then the formula. Now the claim says what the code really does." |

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
| You typed the "claim" in Python | The agent writes a contract and a C++ port |
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
| Closed form | A formula that gives the answer directly, replacing a loop (here, `n(n+1)/2`) |
| Disassembler | A tool that turns bytes into readable instructions |
| Decompiler | A tool that turns instructions back into C-like code (a guess) |
| Ghidra | A free tool that does both, with cross-references and renaming |
| Emulator | A program that pretends to be a CPU, so you can run bytes on their own |
| Unicorn | The emulator library used here |
| Hypothesis | A claim about what a function does, to be tested |
| Oracle | The thing you trust as the judge (here, the original machine code) |
| cdecl | The rule for how arguments are passed: pushed right to left on the stack |
