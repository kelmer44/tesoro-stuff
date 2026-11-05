# Likely questions and answers

Grouped by theme, with the most likely ones first. Answers use project facts as of 5 Nov 2026 and
the demo in [demo/](demo/DEMO_SCRIPT.md). Items marked **[fill in]** are personal facts only you know
(time spent, cost, models used); don't improvise those on stage.

---

## About the approach

### Why not just ask the AI to rewrite the game in one go?
It can't see the binary, so it would guess game behaviour from general knowledge. Guesses look
plausible and are often wrong in exactly the details that matter (timing, edge cases, ordering). There is also
no way to check a one-shot result. The process fixes both: the agent reads the real decompilation, and an
emulator checks its work against the original machine code. Slide 3 is the joke version of this.

### Could a future, better model do it in one shot?
A better model would make fewer mistakes, but the verification problem remains: you still need evidence
that the result matches the original. A more capable model makes the process faster, not unnecessary.

### Why function by function instead of the whole program at once?
Each function is small enough to read, contract and verify on its own, and an error stays local. Working
bottom-up (callees before callers) means every function is built on parts already checked. A whole-program pass
would produce something you can't audit.

### Why did you restart? What went wrong the first time?
Earlier notes and conclusions had drifted: plausible but not checked, so it was hard to tell what was solid.
The restart rule is that old work is reference only, never evidence. Anything new has to be re-derived and
verified. It cost time, but the second pass is something I can trust.

### How is this different from just using Ghidra?
Ghidra gives you the decompilation and the workspace. It doesn't tell you what a function means, and it
doesn't verify anything. The new parts are the agent doing the reading and naming at scale, the emulator checking
the result, and the ledger recording what is trusted and why.

---

## About AI reliability

### Doesn't the AI hallucinate? How can you trust it?
You don't. Nothing is accepted on its say-so. It proposes names, structures and behaviour; the emulator
runs the original bytes and the port on the same inputs and compares. Slide 15 shows the failure mode: a
"plausible" version that passes happy-path tests and fails on an edge case. Unknowns are written down as
unknown rather than guessed.

### Has the AI ever been wrong in a way that mattered?
Yes, and reviewing is part of the job. The honest example in the deck: reading the scene routines found a
real bug in my own port (a harbour guard switched on and off the wrong way round). It was found by comparing against
the original, not by asking the model. **[Add one example where the AI itself was wrong, if you have a good one.]**

### How do you stop the AI from "remembering" things it didn't derive?
Each session starts from the ledger, the contracts and the struct notes, not from chat memory. Contracts mark
confidence per field and say "unknown" where it is unknown. Earlier conclusions are not allowed to count as
evidence for new work.

### Which models did you use, and how much did it cost?
**[Fill in: models, rough token or subscription cost, time spent.]** Be ready to say that the cost is
dominated by the verification and iteration, not by single reads.

---

## About verification

### How can you run one function out of context? It must call other things.
You set up memory and registers the way the function expects, and stub or hook whatever it calls. For game
logic that calls other game functions, you can run those too, from the same binary. For operating-system or hardware
calls, you substitute a small stand-in. For the scene routines in the project, the check was that the port made
the same calls in the same order and left the same state as the original.

### What does "emu-compared" actually mean?
The original machine code was run in an emulator next to the port, on the same inputs, and the results
matched. It is strong evidence, not proof: only the inputs tried are covered.

### Why do the slides show 0% "seen in the running original"?
Because it is true. Matching original code in an emulator is not the same as watching the original game
do it. Right now two menu screens and a saved game have been compared against the real thing (under
DOSBox-X), but not the functions themselves in a running game. Showing that limit is deliberate.

### What if the emulator and the original differ on real hardware?
That is the gap the last ladder rung is for: comparing against the original running in a reference
environment. Emulation covers CPU behaviour; timing, rendering and input need the real thing as a check.

### What inputs do you test with?
Edge cases chosen deliberately (empty, boundary, inverted ranges) plus random inputs with a fixed seed. The demo's
test set mixes both. The lesson from slide 15 is that random or happy-path inputs alone can miss the case that
matters.

### How do you know the emulator itself is right?
You don't take it on faith either. Unicorn is a widely used CPU emulator, and the cross-checks help:
if code that behaves correctly in the real game behaves identically in the emulator, that is evidence for both. Where
something looks odd, the reference environment is the tie-breaker.

---

## About the tools

### Why Ghidra and not IDA or Binary Ninja?
Ghidra is free, scriptable, has a good decompiler, and had an MCP bridge available. IDA and Binary Ninja are
strong too; the process doesn't depend on the choice. **[Add your own reason if different.]**

### What is MCP?
Model Context Protocol: a standard way to give an AI model tools. Here it exposes Ghidra operations
(decompile, disassemble, find references, rename, set a prototype) so the agent works from the real binary
rather than from a description of it.

### What is a "contract"?
One note per function that states what it does, its inputs and outputs, and what is still unknown. It is written
before the C++ port, so the code has a spec that can be checked.

### What is the ledger for?
It is the memory across sessions: one row per function with its name, status, callers and callees. Because an
agent starts each session cold, the ledger tells it what is done, what is trusted and what comes next.

### Isn't Capstone also a disassembler? Why not use only Ghidra?
Ghidra is the interactive tool for humans. Capstone is a library you call from scripts. The talk focuses on
Unicorn because it does the verifying; Capstone is only a scriptable companion.

### Why did the demo compile for 32-bit x86?
The game is a 32-bit Windows program for that CPU family, so the demo uses the same architecture. That keeps the
bytes, calling convention and tools the same as in the real project.

---

## About scope, law and ethics

### Is this legal?
This is a preservation and interoperability project on software I own, using the original files and disc as
evidence, and no original assets or code are redistributed. Rules differ by country, and I'm not a lawyer.
**[Adjust to your own position and your employer's policy.]**

### Could this be used for piracy or malware?
Like any security tooling, the techniques are dual-use. The talk is about understanding and preserving
software you are entitled to examine. The same methods are used defensively for malware analysis.

### Will it be released? Can I play it?
The goal is a complete implementation inside ScummVM. **[Fill in current plans, branch status and what you
are willing to say about timing.]** Don't promise a date.

### How much is done?
Of 447 game functions, all have been read, 96% have a written contract, 90% are ported, and 79% have been
matched against the original machine code (figures from 5 Nov). Seen in the running original: none yet as
functions. **[Refresh these numbers before the talk.]**

### When will it be finished?
Be honest: the remaining work is dominated by verification against the running original and by the parts
not yet read. I'd rather give a milestone ("every room draws, scene scripts run") than a date.

---

## About generalising

### Would this work on any binary?
The method (read, contract, verify, record) transfers. How hard it is depends on the binary: compiler,
optimisation level, obfuscation, how much it talks to the OS. A 1990s game built with an ordinary compiler is a
friendly target.

### What about packed, obfuscated or anti-debug code?
That is a different problem. You would first unpack or defeat the protection, and a decompiler plus an
agent helps less until you have clean code. This project didn't need that.

### What about when the compiler optimises heavily?
Optimisation changes the structure (slide 9: 112 bytes became 303 with the loop vectorised), which makes
decompiler output harder to read. The behaviour is the same, which is why testing behaviour in an emulator is
more reliable than trusting a reading.

### Does this replace reverse engineers?
No. It removes a lot of repetitive reading and naming. Deciding what to trust, choosing test inputs, spotting when a
result smells wrong, and designing the process still need a person.

### How would I use this at work?
Candidates: understanding an undocumented legacy system, vendor library or file format, where you have the
binary but not the source. Start small: one function, one contract, one emulator check, then scale.

### What would you do differently?
**[Fill in your own answer.]** Likely themes: set up the ledger and verification harness first, and never let notes count as
evidence from day one.

---

## Quick facts to have ready

| Fact | Value |
|---|---|
| Target | `TESORO.EXE`, 32-bit Windows program (PE32) |
| Functions | 706 in the ledger, 447 game code (5 Nov) |
| Restart date | 28 Oct 2026 |
| Status ladder | untouched, decompiled, understood, ported, emu-compared, original-compared |
| Reference environment | DOSBox-X running a clone of the original Windows 95 image |
| Demo function | `sum_clamped`, 112 bytes at `-O0`, 303 bytes at `-O2` |
| Demo result | faithful 305/305; plausible fails on `lo > hi`, e.g. `a=[1, 5, 9, 20] lo=8 hi=2: emulator=20, hypothesis=8` |
