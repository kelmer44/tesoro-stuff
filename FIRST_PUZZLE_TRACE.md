# First target: pick up the pen and leave the office

20 October 2026. This is a reverse-engineering milestone, not a playable engine.

## What now works

The read-only script tracing tool reads the original TRE/DAT files, catalogues all 145 script blocks and follows supported instructions on the pen path. It resolves the original text and WAV names and records requested actions. It does not render scenes, play sounds, walk characters or run native callbacks. The first pickup trace stops explicitly at the native door transition.

The first-pickup path, with the input state `v0=0`, produces **47 events**:

- 25 dialogue/narration requests, all with matching WAV resources.
- 14 object, animation, facing or movement requests.
- Four explicit sleep requests, totalling 6,150 ms. This is not the total scene duration: speech, walking and animation completion add time.
- One condition, one variable assignment, one transfer to another script, and one native callback.

The first branch sets `v0=1`, requests that the inventory pen be enabled, disables the pen in the room, shows and hides Jim's letter-reading graphic, changes the walking data, and requests a move to the office exit.

## A useful discovery: who owns the puzzle state?

The pickup method does **not** pass the pen's own state to its script. It looks up `CAPITAN` in the office and passes that object's state pointer. This is why assuming “each script has an independent v0” would produce an incorrect port.

Confirmed entry chain:

| Step | Original evidence |
| --- | --- |
| Room contains pen `BOLI` | `Despacho.lvl`: class 12, parameter subtype 5, coordinates (398, 133). |
| Factory chooses pen class | `0x409e50`: class 12/subtype 5 selects vtable `0x42a910`. |
| Pickup dispatch | That vtable's `+0x58` slot points to `0x40c3c0`. |
| Find state owner | `0x40c3c0` looks up `CAPITAN` in scene global `0x442130`, then reads the object's `+0x60` pointer. |
| Start script | It starts `FBOLI`, entry `coger`, with that state pointer and the clicked object's scene. |

Readable pseudocode for the core of the pickup method:

```cpp
captain = office.findObject("CAPITAN");
startScript("FBOLI", "coger", captain.variableBytes, clickedPen.scene);
```

The real method also clears a selected interface item when present. That interface state is outside the current request-only trace.

Evidence: [fresh Ghidra pickup output](analysis/decompiled/0040c3c0_tesoro_pen_pickup.raw.c). String addresses were checked against the EXE: `0x42d32c` is `CAPITAN`, `0x42d324` is `coger`, `0x42d334` is `FBOLI`.

## The exit is traced into native code

At the end of the first pickup, `FBOLI` requests `run 'FCAPITAN' salir`. The original executor passes the same variable context to the called script. The analysis tool follows this particular tail transfer because the next statement in the caller is `return`; it does not claim to reproduce the original scheduler's general call/return behaviour.

`FCAPITAN/salir` requests that Jim walk to (561, 95), then executes `DO salir`, action zero for native type 21.

The native chain is:

```text
FCAPITAN / salir
  -> type 21 callback at 0x416b50, ordinal 0
  -> find office object PUERTA1
  -> call its virtual method at +0x50
  -> 0x40c880: check the door's own variable byte zero
  -> if that byte is zero, use generic door routine 0x414c40
```

The office door is class 6/subtype 8, selected by the factory into vtable `0x42aef8`. Its `+0x50` entry is `0x40c880`. That method returns without changing scenes when its first variable byte is nonzero.

The door parameters identify scene index 1 and destination object `PUERTA1`. The startup room-name table identifies scene 1 as `PUERTO.LVL`. The harbour's destination door is at (20, 90), with zero entry-position offsets. The reciprocal harbour door points back to scene index 0, `DESPACHO.LVL`, confirming the connection. Actual transition execution still depends on the door state and other guards in the generic routine.

This gives us a concrete future test: after the pickup sequence, Jim should leave the office through this door and enter at the harbour door, when the original game's conditions allow it.

## Scene records are now structurally decoded

The extended reader accounts for **747 object records across all 50 LVL resources**, consuming every file exactly. The office has 27 records.

After the sequential animation banks, each group has a 40-byte header. Its object count is a u16 at group+32. Each object has:

| Offset | Field |
| --- | --- |
| `+0` | Signed x coordinate, 32 bits. |
| `+4` | Signed y coordinate, 32 bits. |
| `+8` | 16-bit field; exact layering/priority meaning still under investigation. |
| `+10` | One-byte field; meaning not established. |
| `+11` | Number of trailing extra bytes. |
| `+12` | Object name, 12 bytes. |
| `+24` | Class/type, 32 bits. |
| `+28` | Parameter payload length, 32 bits. |
| `+32` | Parameter bytes, followed by the stated number of extra bytes. |

Do not interpret the extra bytes as puzzle variables. Their values and use in animation-bank binding indicate resource references; their full semantics still need checking. The decoder keeps them raw. Likewise, unused padding in parameters contains old-looking strings and should not automatically be treated as meaningful game text.

Office records: [office-objects.json](analysis/office-objects.json).

## Checks and how to reproduce them

Six checks pass against the actual supplied game files:

1. First pickup takes the letter-reading branch and reaches the expected native exit request.
2. Later pickup takes the other branch and requests the rope inventory item.
3. Returning the pen without knowing about the rope takes its expected dialogue branch.
4. Returning the pen with that knowledge takes the other branch and re-enables the room pen.
5. A conversation menu that the tracer does not implement raises an explicit unsupported-operation error.
6. All 50 level files' object records are structurally accounted for, including the office's pen and door subtypes.

The input variables are supplied explicitly. These tests establish script/data consistency, not that we have reproduced all original initialization or runtime behaviour.

```sh
python3 tests/test_pen_trace.py --game /Users/gabriel/Desktop/ags/TESORO
python3 tools/trace_script.py /Users/gabriel/Desktop/ags/TESORO --vars v0=0 --output analysis/traces/pen-first-pickup.json
```

Four saved traces live in `analysis/traces/`. Their `object_requests` describe requested switches, not a complete simulation of object state after native code, animation or movement effects.

The saved Ghidra project now exports **47 selected routines**, including the pickup handler, scene-object factory, script start/resume paths, and office/generic door methods. Their instruction and C-like output files remain available under `analysis/decompiled/`.

## Next implementation milestone

Build the minimal C++ runtime needed for this path: archive access, explicit variable contexts, resumable script execution, object requests and host completion events. Then put it behind a small ScummVM scene prototype, using the known artwork and object records. The first visual target is the office and pen, followed by Jim's walking and this exact script path. At each step, keep unsupported behaviour visible instead of silently skipping it.

A running original and complete media will be needed for exact timing, scene composition and end-to-end comparison. Their absence does not block continuing the data and code analysis.

## Working C++ prototype — 20 October 2026

The implementation milestone above is now available in
[prototype/README.md](prototype/README.md).
The original pen script runs inside ScummVM with real graphics, voices,
subtitles, inventory switches, waits and the native exit to the harbour. Its
controlled-host checks and full real-time autoplay pass. See
[validation](prototype/VALIDATION.md).

Additional findings used by the renderer:

- `JIMQUIETO` frames are up, down, left, right, confirmed by inspecting their
  artwork; the initial guess was corrected before delivery.
- Scenery constructor `0x40e5c0` reads parameter byte 0 as its initial frame and
  byte 1 as hidden/hold/once/loop (0/1/2/3). This explains the letter/boat overlay
  visibility, looping sea/bird assets and MASKS2's initial frame.
- `FANPRESENT` explicitly disables the harbour's `FONDONEGRO` overlay before
  normal play. The prototype applies this post-introduction state when starting.
- The captain's last arrival frame must be retained while speech temporarily
  uses his talking bank; resetting arrival to frame zero makes him vanish.
- The office background spans 682 pixels, so its 640-pixel viewport scrolls by
  up to 42 pixels. A clamped camera centres the letter overlay in this slice.

The selected Ghidra export now contains 48 routines, including the scenery
constructor. These findings do not establish the full original initialization,
pathfinding, scaling, camera or speech-animation algorithms. The prototype
explicitly documents its temporary substitutes for those systems.
