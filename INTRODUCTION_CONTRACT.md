# Harbour introduction: recovered contract

21 October 2026. Static evidence for the verified executable edition; no
original runtime comparison is claimed. The first visible harbour prefix is now
implemented; current live-build evidence and limits are in [LIVE_OPENING.md](LIVE_OPENING.md).

## Entry and data scope

The difficulty menu's action 3 calls `0x414560`. That routine places Jim in
the harbour using the room controller's entry coordinates and starts
`FANPRESENT/main` with native type 1. The recovered harbour entry is `(213,104)`,
with camera center `(320,239)`; see [SCENE_LIFECYCLE.md](SCENE_LIFECYCLE.md).

The script declares participants `JIM` and `CAPITAN`, requests movies 2 and 3,
18 speech lines with matching audio, one `ANM1BCAP0` sound effect, and eight
explicit sleeps totalling 16,500 ms. Speech, movies, walking and waited animation
add their own durations. The sleep total is not the introduction's running time.

[The evidence report](analysis/introduction-contract.json) is generated directly
from the original executable and script corpus:

```sh
python3 tools/inspect_introduction.py /Users/gabriel/Desktop/ags/TESORO \
  --output analysis/introduction-contract.json
```

The tool requires the exact executable SHA-256 before using any fixed address.
It maps PE virtual addresses to file-backed section bytes and reads native object
name strings, the range guard, return path and jump table. It neither executes
native code nor simulates the original scheduler.

## Native dispatch that must be preserved

Handler `0x4197a0` compares the unsigned ordinal with 5. Larger ordinals go to
`0x4198b4` (`POP ESI; RET 4`). Its six-entry jump table at `0x4198b8` also sends
ordinals 1 and 2 directly to that return path.

| DO ordinal | Script line | Native target | Recovered effect |
| ---: | ---: | --- | --- |
| 0 | 72 | `0x4197b7` | Clear Jim's show bit, activate him through virtual method `+0x20`, update the current scene view. The annotation does not itself implement a fade. |
| 1 | 73 | `0x4198b4` | No effect, despite the presentation/credits annotation. |
| 2 | 79 | `0x4198b4` | No effect, despite the harbour-without-ship/people annotation. |
| 3 | 82 | `0x4197ec` | Set BARCO's show bit; call animation helper `0x401de0` with arguments `(0, 0, -1, 1, 0)`. Recover the helper's full semantics before porting this call. |
| 4 | 92 | `0x41983e` | Set Jim's show bit. |
| 5 | 102 | `0x41984f` | Activate the harbour PUERTA1 and clear its show bit; switch the script's scene context to the office; place Jim there at `(550, 100)`. |
| 6 | 104 | Range guard → `0x4198b4` | No effect, despite the captain-opening-door/Jim-entering annotation. |

Object identities are verified from executable strings: `0x42e91c = BARCO` and
`0x42da50 = PUERTA1`. Ordinal 5 refers to the **harbour door**, not CAPITAN.
The office has a separate object also named PUERTA1; scene context matters.

Supporting evidence: [native C](analysis/decompiled/004197a0_tesoro_native_type_01.raw.c),
[assembly](analysis/decompiled/004197a0_tesoro_native_type_01.asm),
[new-game entry](analysis/startup/00414560_tesoro_start_new_game.raw.c), and
[menu action dispatch](analysis/startup/00417b40_tesoro_menu_button_click.raw.c).

## Script scheduling and scene context

The sequence reveals harbour objects and starts looping gull/recruiter animation,
shows Jim, walks him to `(25, 90)`, faces him upward, plays movie 3, then switches
to the office through ordinal 5. Dialogue follows; CAPITAN's animation `play 2 0
wait` must finish before the final narration and `activate off`.

The parser maps `activate` to message kind 3, dispatched through virtual method
`+0x20`. `show` maps to kind 4, which writes bit 2 of the object flags via
`0x401d80`. Keep the two effects distinct.

Script startup `0x405fd0` initially resolves participant names in the supplied
scene. Speech execution `0x405a60`, case 1, retries a participant whose cached
pointer is zero against the **current script scene**. Ordinal 5 changes that scene
at script offset `+0x40`; a port must preserve this behaviour rather than freezing
all participant resolution to the harbour. It does not eagerly rebind every
participant on a scene change. Actual lookup and actor lifecycle details still
require recovery.

Speech, walking and waited animation yield; a completion callback increments
the script PC and resumes the executor. `DO` dispatch calls the handler and then
continues. The current request tracer and preserved tail-only VM do not establish
all of these scheduling semantics.

## Remaining implementation dependencies

1. Room-controller entry and binding/transfer paths are recovered in
   [SCENE_LIFECYCLE.md](SCENE_LIFECYCLE.md). Scenery ownership and sprite queues
   are implemented; actor transfer and other object kinds remain to be ported.
2. Playback start/end/repeat, frame advancement and scenery dispatch are now
   implemented. Script waiting/completion and character-specific behaviour,
   including CAPITAN's exit, remain. Preserve ignored DO annotations as no-ops.
3. The script subset now supplies source locations, movies/sound, offscreen
   narration, sleeps and activation/show/play messages in the main loop.
   Actor speech, general scheduling, scene context and lazy participant lookup
   remain incomplete.
4. Implement the harbour/office layers, original coordinate transforms, camera,
   scale, shadows and the walk-to-door path using recovered rules.
5. Action 3 is connected through the harbour reveals and pauses at ordinal 4.
   Continue that normal path through Jim, the office and captain. Compare the
   reference run when its environment has been explicitly authorized.

The original runtime remains an open fidelity check. Until then, label this
work as recovered static behaviour and host validation.
