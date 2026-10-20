# Tesoro: first AI-assisted decompilation batch

This is the workflow we can use for the port: Ghidra produces C-like output, I check it against the instructions and game data, and I rewrite the understood behaviour as clear pseudocode. Then we implement that behaviour in ScummVM and compare it with the original game.

You do not need to interpret the assembly yourself. I can do that work. The distinction that matters is between a decompiler producing text and us establishing that the text describes the game correctly.

## What was actually run

Ghidra 11.4.2 imported the verified `TESORO.EXE` into a new, saved project and ran its x86/Windows analysis. A custom script explicitly added the virtual callback entry points missed by the older analysis, gave relevant routines descriptive names, and exported decompiled C-like output, disassembly and incoming references.

The first pass recovered all 31 selected routines. A second pass added seven helper routines used by the examples below and completed **38 out of 38** selected decompilations. See `analysis/decompiled/decompile-status.tsv` for the result of each routine, rather than assuming that a file's presence establishes successful recovery.

Ghidra MCP is installed at `/Users/gabriel/Desktop/source/GhidraMCP`, but no Ghidra MCP tools were exposed to this chat and its documented server at `127.0.0.1:8080` was unreachable. **This batch used Ghidra headless, not MCP.** Both routes use Ghidra's decompiler. A running Ghidra GUI with the plugin enabled would let us use the bridge for interactive inspection/renaming; it is not required to continue this work.

Files:

- [Decompiler status](analysis/decompiled/decompile-status.tsv)
- [Script interpreter: raw output](analysis/decompiled/00405a60_tesoro_execute_script.raw.c)
- [Script parser: raw output](analysis/decompiled/00404810_tesoro_parse_script_block.raw.c)
- [Ghidra recovery script](ghidra/RecoverTesoro.java)
- Saved Ghidra project: `analysis/ghidra-project/TesoroAnalysis.gpr`

The function names are investigation labels I supplied, not names recovered from the original developer. `.raw.c` means untouched decompiler code after naming the functions; it is not original source or compilable replacement code. The examples below are manually interpreted pseudocode.

## Example 1: how a script calls special game code

**Confirmed:** the parser at `0x404810` handles a line starting with `D` by emitting operation `12` and an incrementing counter. The counter begins at zero for each script block. In the executor at `0x405a60`, operation `12` invokes the function at offset `0x70` of the script object's virtual-function table, passing that counter.

In plain language: each script can have its own special handler. Its first `DO` line calls action zero, its second calls action one, and so on. The description after `DO` is for the person reading the script; the program dispatches using the number.

```cpp
// Simplified excerpt: other script commands omitted.
void parseScriptBlock(ScriptSource source, Script &script) {
    int nextSpecialAction = 0;

    for (Line line : source.linesInThisBlock()) {
        if (line.isDoLine()) {
            script.operations.append({
                .opcode = CallSpecialAction,  // Original internal value: 12
                .argument = nextSpecialAction++
            });
        } else {
            parseOtherInstruction(line, script);
        }
    }
}

// Simplified excerpt from the executor.
void executeSpecialAction(Script &script) {
    Operation instruction = script.operations[script.programCounter];
    script.callSpecialHandler(instruction.argument);
    script.programCounter++;
    // Original executor continues until an instruction yields or returns.
}
```

The numeric type in the script header selects the specialized handler class. There is also a default handler at `0x401d20` that simply returns. This explains why some apparently descriptive `DO` lines have no effect in this executable.

Useful script object fields inferred from the parser/executor:

| Original byte offset | Interpreted role | Status |
| --- | --- | --- |
| `+0x40` | Current scene pointer | Supported by scene lookup and transition call sites. |
| `+0xb0` | Cached pointers to participating objects/characters | Supported by speech/object lookup and callback rebinding loops. |
| `+0xb4` | Their encoded names, eight bytes each | Supported by parser and lookup calls. |
| `+0xb8` | Participant count | Confirmed by allocation and iteration. |
| `+0xbc` | Internal operation array | Confirmed by parser/executor use. |
| `+0xcc` | Current instruction index | Confirmed by branches and increments. |
| `+0xd0` | Pointer to script variable bytes | Confirmed by assignment and condition operations; ownership/lifetime still needs work. |
| `+0xd4` | Script specialization/type | Supported by registration switch. |

These are notes for recovering meaning. New ScummVM classes should use named fields rather than reproducing original memory offsets.

## Example 2: the menu fade handler

Entry: `0x417c80`, script type 17, used by `.FANMMENU 17`.

The complete decompiled handler reduces to:

```cpp
void handleMenuSpecialAction(int actionNumber) {
    if (actionNumber >= 0 && actionNumber < 4) {
        fadeToBlack(/*steps=*/4, /*minimumMillisecondsPerStep=*/100);
    }
}
```

**Confirmed from the helper at `0x4123e0`:** it copies the current 640 × 480 image and redraws it with decreasing brightness. With the arguments above, brightness factors are `192/256`, `128/256`, `64/256`, then zero. The original waits until at least 100 ms has elapsed for each step, including rendering time. This is a minimum of roughly 400 ms for the whole fade, not 100 ms total.

Readable pseudocode for this valid call:

```cpp
void fadeToBlack(int steps, int minimumMillisecondsPerStep) {
    Image original = copyCurrentFrame();
    int brightnessStep = 256 / steps;

    for (int brightness = 256 - brightnessStep;
         brightness >= 0;
         brightness -= brightnessStep) {
        Time started = currentTime();

        // Original scales each packed color channel with integer truncation.
        Image frame = scaleOriginalPackedChannels(original, brightness, 256);
        display(frame);

        waitUntil(started + minimumMillisecondsPerStep);
    }
}
```

The source helper handles RGB565 and RGB555 display modes. For a faithful port, preserve the original integer channel rounding rather than assuming arbitrary floating-point color multiplication will match it exactly.

**ScummVM implementation choice:** reproduce this timing using the engine's event/update loop. The Windows program busy-waits; copying that loop literally would make input handling worse. This is a replacement implementation choice, not behaviour recovered from the executable.

Evidence: [handler C-like output](analysis/decompiled/00417c80_tesoro_native_type_17.raw.c), [handler instructions](analysis/decompiled/00417c80_tesoro_native_type_17.asm), [fade helper](analysis/decompiled/004123e0_tesoro_fade_to_black.raw.c).

## Example 3: a transition used by the throne sequence

Entry: `0x419f10`, type 20, `.FANMTRONO 20`. Its script has `DO ir` and `DO volver`, which become action numbers zero and one.

The two branches perform nearly the same sequence. The following merges their common behaviour without claiming to know every field's purpose:

```cpp
void handleThroneSpecialAction(Script &script, int actionNumber) {
    Scene *destination;
    bool flagBit2;

    if (actionNumber == 0) {
        destination = globalScenePointer_00442188;
        flagBit2 = false;
    } else if (actionNumber == 1) {
        destination = globalScenePointer_00442154;
        flagBit2 = true;
    } else {
        return;
    }

    fadeToBlack(4, 100);
    script.scene = destination;

    placeJimInScene(
        destination,
        destination.fieldAt_0x70,  // Exact semantic role still under investigation.
        jim.x,
        jim.y
    );

    setObjectFlagBit2(jim, flagBit2);
    updateSceneView(destination, jim.x, jim.y, 0);

    // Object pointers belong to a particular scene. Refresh them after moving.
    for (int i = 0; i < script.participantCount; i++) {
        script.participants[i] =
            findSceneObject(destination, script.participantNames[i]);
    }

    resetRenderLists();
}
```

**Confirmed:** the destinations' global addresses, preserved X/Y values, flag change, lookup loop and draw-list reset calls. The helper at `0x401d80` changes bit 2 of the object's flags at offset `+0x68`; it does not itself draw anything.

**Still to establish:** the precise meaning of that flag, the destination scene labels, the `+0x70` field, and the camera/view helper's last argument. Calling the flag “visible” immediately would be an assumption. The readable names `placeJimInScene` and `updateSceneView` summarize their apparent roles; the raw calls and instructions remain available for checking.

Evidence: [raw handler](analysis/decompiled/00419f10_tesoro_native_type_20.raw.c), [flag helper](analysis/decompiled/00401d80_tesoro_set_object_flag_bit2.raw.c).

## How this becomes C++ for ScummVM

For each small subsystem, I can deliver:

1. The original address, raw decompilation, relevant instructions and callers.
2. Clear pseudocode and a list of meanings that are confirmed versus unresolved.
3. A C++ implementation using ScummVM services, plus a focused check against known data or original behaviour.
4. A record of what the implementation has actually reproduced.

The next useful batch is to recover the complete script operation definitions and the first-room object/action path. For the parser, Ghidra currently infers a clearly unreliable function signature with stack-buffer artifacts. Its machine instructions are available; fix the calling convention and parameter types before treating that C-like signature as an API. The executor also yields during speech, movement and waits, so a replacement needs resumable script state rather than a simple loop that runs every command immediately.

For checking picture formats and script structure, we already have useful local evidence. For timing, movement and puzzle results, we will eventually need a running original. AI can do the analysis and implementation, but a successful decompilation alone is not evidence of matching gameplay.
