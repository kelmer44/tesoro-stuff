# Presentation: reverse engineering with Ghidra, emulation and AI agents

Talk for engineers, 30 minutes, built around the Tesoro project.

## Contents

- [DEMOS_EXPLAINED.md](DEMOS_EXPLAINED.md): the three demos explained in simple language, with a glossary.
- [demo/](demo/): live-demo helpers (build wrapper, emulator runner, test loop, ImHex pattern `sum_to_O0.hexpat`, a backup copy of the function) and [demo/DEMO_SCRIPT.md](demo/DEMO_SCRIPT.md), the step-by-step script. The C function is typed live.

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
mkdir -p live                                    # type live/sum_to.c here (backup: fallback/sum_to.c)
./build.sh live/sum_to.c
./run_native.sh live/sum_to.c 10                 # same C, run natively
PY=../../re/.venv/bin/python
$PY run.py build/sum_to_O0.bin 10                # exact bytes, in the emulator
./patch_demo.sh jge                              # scripted byte patch (or do it in ImHex)
$PY -I verify.py build/sum_to_O2.bin --expr "n*(n+1)//2"
```

Needs clang (32-bit x86 target, no linker) and the `re/.venv` environment (Unicorn, Capstone).
The demo uses an original function written for the talk, not game code.

## Deck

- `tesoro_talk.pptx`: the 24-slide deck, with speaker notes. Slide content and notes are in [SLIDES.md](SLIDES.md).
- Rebuild with `cd deck && npm install && NODE_PATH=$PWD/node_modules node build_deck.js` (needs the pptx skill's `apply_theme.js`; set `PPTX_SKILL_DIR` if its path differs).
- Placeholders to replace by hand: slide 2 (ScummVM screenshot), optionally slide 11 (Ghidra screenshot).
