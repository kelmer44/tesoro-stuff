# Presentation: reverse engineering with Ghidra, emulation and AI agents

Talk for engineers, 30 minutes, built around the Tesoro project.

## Contents

- [DEMOS_EXPLAINED.md](DEMOS_EXPLAINED.md): the three demos explained in simple language, with a glossary.
- [demo/](demo/): self-contained live-demo materials (a C function, build script,
  Capstone/Unicorn verifier) and [demo/DEMO_SCRIPT.md](demo/DEMO_SCRIPT.md), the step-by-step script.

## Outline

| Time | Section |
|---|---|
| 0:00 | Hook: the game and `TESORO.EXE`, joke slide "implement this game in one shot, no errors" (crossed out), and why it fails |
| 2:00 | What reverse engineering is and why people do it |
| 5:00 | Act 1: by hand. Compile, hex editor, opcodes (demo) |
| 12:00 | Act 2: Ghidra. Same function, then the scale problem (demo) |
| 16:00 | Act 3: verification with Unicorn (demo) |
| 20:00 | Act 4: AI agents with the Ghidra MCP in a systematic loop |
| 26:00 | Lessons, including the restart |
| 29:00 | Close, replacing the joke prompt with the real one |

## Running the demo

```bash
cd prez/demo
./build.sh
../../re/.venv/bin/python -I verify.py build/sum_clamped_O2.bin --hypothesis faithful
./run_native.sh                                       # same C, run natively
../../re/.venv/bin/python run.py build/sum_clamped_O0.bin   # exact bytes, live
./patch_demo.sh                                       # patch one byte, then run again
```

Needs clang (32-bit x86 target, no linker) and the `re/.venv` environment (Unicorn, Capstone).
The demo uses an original function written for the talk, not game code.

## Deck

- `tesoro_talk.pptx`: the 24-slide deck, with speaker notes. Slide content and notes are in [SLIDES.md](SLIDES.md).
- Rebuild with `cd deck && npm install && NODE_PATH=$PWD/node_modules node build_deck.js` (needs the pptx skill's `apply_theme.js`; set `PPTX_SKILL_DIR` if its path differs).
- Placeholders to replace by hand: slide 2 (ScummVM screenshot), optionally slide 11 (Ghidra screenshot).
