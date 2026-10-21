# Complete Tesoro implementation: governing plan

Created 21 October 2026. Owner: Gabriel and Codex. Codex leads reverse
engineering; we implement the ScummVM engine together. This is the project's
main guideline, superseding the task ordering in `FEASIBILITY_AND_PLAN.md`.
Maintain the live checkpoint in [RE_STATUS.md](RE_STATUS.md).

## Target and fidelity contract

Reimplement the complete verified Spanish edition of **El Tesoro de Isla
Alcachofa** in ScummVM, reading its original data. Completion includes the real
adventure from startup to credits, the easy/hard joke paths, optional dialogue
and interactions, backtracking, inventory combinations, special games, movies,
music, saving/loading and failure/retry paths that the original actually has.
Do not assume that finishing one puzzle or the critical route covers the game.

Match original puzzle logic, state ownership, verb/hotspot behaviour, movement,
actor placement/scaling, layering/shadows, animation sequencing, text/glyphs,
speech, audio cues, transitions, camera and timing as closely as evidence allows.
Preserve observable quirks unless an intentional correction is documented and
agreed. ScummVM supplies portable platform services; its launcher/save UI need
not imitate Windows. Do not recreate DirectDraw/COM just to reproduce outputs.

For each feature track **semantics recovered**, **implemented**, and **compared
with the running original** separately. Use unknown / hypothesis / recovered /
observed / compared descriptions with evidence links. A hypothesis may unblock
an experiment but cannot silently become the fidelity specification. Keep
temporary approximations in a ledger with location, impact and removal target.

## Starting evidence and boundaries

- Exact executable identity and addresses: [TECHNICAL_NOTES.md](TECHNICAL_NOTES.md).
  The local original is PE32 x86 at preferred image base `0x400000`; addresses
  apply only to its recorded SHA-256. Identify any different edition first.
- Verified resource structure: 2,416 indexed files; 50 LVLs; 747 object records
  across **23 observed object kinds**; 371 banks; 2,012 images; 2,346 WAVs.
  These are structural counts, not implemented features or 50 playable rooms.
- [Full catalogue](analysis/full-game-catalogue.json) lists every LVL, object-kind
  distribution, script block/label and lexical command/message form. There are
  145 blocks and 87 DO markers: 31 default no-ops, 56 dispatched through 19
  specialized handler types. Each native case still needs an explicit result.
- 59 distinct selected function exports have successful decompiler status
  (48 general + 11 startup). Inferred C signatures remain fallible. Native
  object factories/vtables contain substantial additional game logic.
- All 13 movies and 19 CD audio tracks have been located; see
  [MEDIA_STATUS.md](MEDIA_STATUS.md). Finding/decoding media does not establish
  its scene cue, loop, volume, skip or transition semantics.
- The preserved prototype implements a limited office pen path. Its straight
  walking, toolbar, scale, timing and hotspot shortcuts are not specifications.
- The active worktree is `../scummvm-tesoro`, branch `tesoro`. The inspected
  HEAD was `33316cacbf8` with local changes. Earlier startup logs describe a
  different source state: save support serializes a dummy value and the old
  test hook is absent. Local source changed during inspection, including a
  correction to the draw method. Reconcile the latest source before treating
  historical checks as current validation.
- Gabriel confirmed there is **no working original reference session yet**.
  Establishing one is a first milestone, not a requirement already satisfied.

## How research becomes implementation

For a narrow question, record a reproducible trigger and what must be learned.
Trace it from data/script into the original executable, then back to observable
state. Use the smallest useful set of functions and assets; expand when a caller,
virtual method or state lifetime requires it. Each implementation increment gets:

1. Evidence: file hash, addresses/data offsets, callers, matching assembly and
   decompilation, script file/block/line, and original capture when available.
2. A short behaviour contract: inputs, state read/written, outputs, asynchronous
   completion conditions, cancellation and persistence.
3. A focused C++ implementation using ScummVM facilities, with diagnostic names
   tied to original scene/object/script identities.
4. Appropriate build/data checks and a replay comparison; record discrepancies,
   remaining uncertainty and the next dependency in `RE_STATUS.md`.

Prefer working original data over reconstructed content. Do not replace scripts
with a hard-coded walkthrough or infer native effects from DO annotations.
The port executes reimplemented logic, never the Windows EXE.

## Reverse engineering methods and tool escalation

**Data and static analysis first.** Use the validated archive/scene readers and
Ghidra project. Follow constructors, factories, vtable writes and callers to
discover virtual functions that automatic analysis missed. Recover struct fields
incrementally from repeated offsets, widths, initialization and mutation; retain
raw bytes where semantics remain unknown. Fix calling conventions/types only
when supported by instructions. Check jump-table completeness and signedness.
Keep assembly/xrefs alongside raw C and human explanations.

**Use the running original to resolve behaviour.** Capture exact inputs, room,
flags, actor coordinates/facing, frame IDs, clock values, speech IDs, native
ordinals and transition boundaries where feasible. Break at script dispatch,
object update/message methods and setters rather than single-stepping the whole
engine. Translate preferred addresses if the runtime module base differs.
Use watchpoints for state ownership/lifetime; compare save files from controlled
one-action changes. Save diffs are clues until confirmed by serializer code.

**Differential experiments for ambiguous algorithms.** For walking, scaling,
depth, shadows and timing, feed matched inputs and compare coordinate/frame/state
sequences. If useful, isolate a pure original routine in an x86 harness/emulator
and compare the C++ implementation over boundaries and unusual inputs. Validate
ABI, memory inputs and dependencies first; a whole-game emulator is not required.
Record experiments that disprove a theory as well as successful ones.

**Tools and access boundaries.** The documented Ghidra 11.4.2/Java 23 workflow
has been verified. Use authorized tools for named unanswered questions. Do not
inventory installed applications, personal folders or VM configurations. Gabriel
must explicitly authorize the original reference environment and any additional
paths before that setup proceeds. Choose a debugger after the reference
environment is known; avoid a speculative installation list.

Proven reusable procedures belong in [the research skill](skills/tesoro-re/SKILL.md).
Keep untested methods here as candidate experiments. Ghidra's official
[headless guide](https://github.com/NationalSecurityAgency/ghidra/blob/master/Ghidra/RuntimeScripts/support/analyzeHeadlessREADME.md)
documents processing existing projects, script paths and read-only runs; the
installed version's documentation governs the exact local invocation.

## Milestone 0 — reproducible source and original reference

**Source baseline:** inspect the active worktree and local edits; jointly retain
the intended incremental implementation. Resolve compile errors, clock/pacing,
movie-stream ownership and unfinished save feature declarations. Record source
revision plus local diff and matching build. Audit which historical tests still
apply; port the useful startup harness to current architecture before running it.
Never overwrite the preserved prototype or claim an old binary tests new code.

**Original reference:** first obtain Gabriel's explicit choice of environment and
paths; this part of M0 is currently on hold. Then try a separate Wine prefix with a writable installed copy
and the complete disc available. Record startup errors and missing dependencies;
do not patch the evidence files. If graphics, codecs, CD audio or input remain
unreliable, use an isolated Windows guest in an explicitly authorized emulator/VM.
Gabriel identifies the guest/Windows media and supplies any missing OS
installation/licensing material. Prefer a working historically appropriate x86
environment over spending indefinitely on compatibility workarounds.

Record OS/runtime/renderer, resolution, CD drive/media paths and game hashes.
Verify movie 0, title menu, movie 1/card, real adventure, speech and music. Save a
clean snapshot before the game starts, then named checkpoints with repeatable
input recipes. Check compatibility-induced defects against static code or a
second environment before reproducing them as original behaviour.

**Exit evidence:** current source builds; startup smoke check matches that source;
original reaches office; capture and action recipe cover opening and pen path;
missing capabilities are listed. Static recovery can continue while VM setup is
blocked, but original-comparison status stays open.

## Milestone 1 — world model, formats and full behaviour inventory

Extend the structural catalogue into linked matrices: every LVL → object record
→ kind/subtype → constructor/vtable → overridden methods; every script → commands,
messages, state owner, child scripts, native ordinal and referenced media. Use
`0x409e50` as the scene factory anchor and `0x406190` for script classes.
Determine actual subtype fields by class; do not generalize one byte layout to
all 23 kinds. Distinguish present resources from executable-reachable content.

Recover ANM frame duration, origin, offsets, movement, events, repeat/stop/wait
semantics, extra-data and signed invisible-image indices. Map scene/group fields,
floor geometry, scale/depth curves, camera and object-reference bytes. Trace the
lookup table's actual role and original character encoding/glyph indexing.
Use targeted exports named for the question and attach confidence per field.

**Exit evidence:** all records accounted for, all observed kinds assigned research
entries, unknown fields retained explicitly, parser corpus catalogued and every
native marker tracked. Implementation/reachability columns may remain open.

## Milestone 2 — resumable runtime and original new-game sequence

Introduce modules as needed for resources, world/objects, graphics/animation,
actor/walking, scripts, UI, audio/media and serialization. Keep research Python
outside the runtime. Reuse proven prototype readers and speech/script pieces
selectively; extend them against original semantics before generalizing.

Recover the scheduler's update/render order, mutation rules, timers and pause
semantics. The original object loop is cooperative; scripts wait for actors,
speech, animations, timers and child actions while other objects still update.
Investigate `run`, `break`, return/resumption and participant/variable context;
the prototype's tail-only transfer is insufficient. Do not impose a 60 Hz game
simulation because one host loop was paced at 60 Hz. Preserve original clock
rules (including the observed handling of deltas >=500 ms) where they affect play.

Port action 3 → `FANPRESENT/main` → harbour → office. Trace type-1 native ordinals,
reveals, camera, Jim placement, movie 2/3 handoffs and captain entrance/exit. Retain
no-effect ordinals when the executable ignores them. Add the easy/hard joke
scripts and remaining startup fades/cursor when their machinery is ready.

**Exit evidence:** new game reaches the authentic office state without a debug
teleport; input, movies, speech, waits and scene changes coexist in one runtime;
the same action recipe matches the original at named checkpoints.

## Milestone 3 — faithful interaction and movement foundation

Recover floor/path connectivity, destination projection, unreachable targets,
approach points, direction changes, speed, frame-driven displacement, depth and
scale. Compare straight walks, obstacle detours, near/far positions, exits and
interrupted walks. Implement original camera limits, scroll response and picking
coordinate transforms. Avoid guessed straight-line movement and harbour scale.

Recover composition: frame anchors, clipping, draw order/ties, behind-scenery
occlusion, shadow blend/lookup arithmetic, palette/fade steps and text placement.
Compare exact RGB565 source results where possible; record conversion tolerance
for captured displays/video instead of assuming screenshot pixel identity.

Recover the original interface from `INTERFACE.LVL` and native UI methods: verb
selection, cursor changes, hover text, hotspot priority/hit test, inventory layout,
selection/combination/give/use dispatch, walk-before-action and disabled states.
Determine how object methods select script/label and supply variables. Keep
`activate` and `show` distinct and preserve original default responses.

Complete the pen puzzle's first/later pickup, both pen-return branches and native
office/harbour door paths using this general system. The pen is a regression
scenario for the engine, not a special runtime path.

**Exit evidence:** input-driven first puzzle and backtracking compare with the
original, including movement, UI, speech/state and negative interactions; no
temporary toolbar, scale or hotspot shortcuts remain in this slice.

## Milestone 4 — complete script semantics, dialogue and saves

Cover the real corpus: conditions, assignments/widths, labels/jumps, all speech
forms, menu/menuitem forms, `break`, `run`, messages, media, sleeps and DO dispatch.
Recover parser behaviour for the observed `menutiem0` spelling before deciding
whether it is accepted, ignored or erroneous. Preserve source file/block/line in
diagnostics. Unknown commands fail visibly in development rather than pretending
success; label lookups and unsupported native/object cases get the same treatment.

Implement conversation choice layout/hover, choice enablement, consumption,
re-entry and state. Verify glyph mapping, wrapping, actor text anchors/colors,
speech duration/mouth animation, skip and subtitle settings against actual paths.

Build real versioned ScummVM saves alongside world-state recovery. Serialize
stable object/scene IDs, all relevant per-object variables, inventory, actor and
camera state, animation state, active script contexts/PCs, waits, clock-relative
deadlines, dialogue and random state where required. Rebuild derived pointers and
audio/render handles. Define evidence-based safe save boundaries; do not advertise
save-anywhere or successful saves while state is dummy/incomplete. Test reload
after room changes and during every supported asynchronous state. Original save
import is a separate fidelity benefit after native save format research; ScummVM
saves must work regardless.

**Exit evidence:** corpus forms have tested semantics or explicit unreachable
classification; conversation re-entry works; representative reloads produce the
same future behaviour and state as uninterrupted play.

## Milestone 5 — finish the adventure by verified progression

Build the room/puzzle dependency graph from scripts, object handlers and reference
play. Names below organize investigation; they do not assert the final route.
For each section implement all newly reached object classes/native cases, optional
dialogue, inventory states, returns and media before declaring that section done.

| Section | Resource starting points | Required evidence |
| --- | --- | --- |
| Bristol/opening | Puerto, Despacho, Taberna, Entrada, Armario | Full recruitment/departure chain and optional returns; office flags persist. |
| Ship and mutiny | Cubierta/Cubierta2, Camarote/Camarote2, Cocina/Cocina2, Almacen/Almacen2 | Pre/post-event room states, inventory and native transitions; no sequence shortcuts. |
| Island progression | Playa, Tierra, Poblado, Campamento, Pantano, Mapa, Trono, Volcan | Verified puzzle order, travel/return conditions, conversations and cutscenes. |
| Special systems | Tablero, Lucha, numbered LVLs 1–20 and their callers | Actual roles/reachability, rules, AI/randomness, input, win/loss/retry and difficulty where present. |
| Finale | Risco, RiscoFinal, Final, Creditos and final scripts | Fight/transitions, ending, credits and return/quit behaviour. |

For all 87 DO markers, record confirmed no-op or original case logic, state,
call chain, implementation and replay evidence. Complete every reachable object
override too: recovering all DO handlers alone does not finish native behaviour.
Trace executable-only movie/music references, unused resources and content
variants. Do not force unused assets into gameplay for a higher coverage count.

**Exit evidence:** continuous new-game-to-credits completion without state injection;
all reachable puzzle branches, special-system outcomes and optional content have
a coverage entry and a reproduced result. Unreachable/unused classifications cite
evidence rather than masking an unimplemented branch.

## Milestone 6 — media, fidelity closure and ScummVM readiness

Develop these throughout the earlier milestones, then close them systematically:

- Verify all reachable movies, positioning/scaling, audio sync, skip/completion
  handoffs, pause/quit and missing/invalid media handling. Check all 13 files for
  decoding, separately from whether every one is used.
- Recover CD cue numbering (disc tracks 2–20 versus ScummVM file numbering),
  loops, scene changes, fades and mixer volume/ducking. Verify tracks against the
  existing PCM evidence and detect audible differences in a real output session.
- Replay a second full route with different optional actions/order, repeated
  save/load, conversation exits/re-entry, item misuse and transitions. Exercise
  every reachable special-system outcome, including losing/retrying if present.
- Compare timing on different host rates, pause/resume and slow frames; check
  deterministic state where appropriate and distributions/rules for randomness.
- Build/test at least macOS plus another supported ScummVM desktop platform;
  investigate endian/alignment, integer widths, memory ownership, malformed data,
  audio/video cleanup and low-performance behaviour. Add sanitizers where useful.
- Audit detection/file requirements, launcher features, debugger diagnostics,
  subtitles/keymaps, save metadata/thumbnails and documentation against actual
  supported behaviour. Follow the checked-out ScummVM contribution conventions.
- Prepare reviewable commits and testing instructions. Ask for maintainer input
  on engine placement early when Gabriel wants contact; upstream acceptance is a
  separate review outcome. No external contact is assumed by this plan.

**Full completion gate:** all reachable gameplay implemented; reproducible
completion plus branch/special-system coverage; original comparison evidence for
graphics, logic, movement, timing and media; real save/load checks; no unexplained
reachable unknowns, placeholders or untracked approximations. Any residual known
differences are documented and explicitly accepted before claiming full fidelity.

## Working rhythm and next actions

Use evidence and milestone exits instead of a speculative completion percentage
or calendar promise. At each checkpoint report what we learned, what changed,
what was compared and what remains uncertain. Reorder tasks when new evidence
reveals a dependency; record the reason without discarding prior evidence.

The source/build part of M0 is complete; see [M0_BASELINE.md](M0_BASELINE.md).
The original reference is pending explicit environment/path authorization. Recover
world/animation/scheduler fields needed by M2 using the
[introduction contract](INTRODUCTION_CONTRACT.md). The animation core is now
implemented and compared against isolated original routines; scene integration
is still pending (see [ANIMATION_FINDINGS.md](ANIMATION_FINDINGS.md)). Next recover
harbour entry and scene/object lifecycle. The first visible gameplay increment after the baseline is
the real adventure button starting the authentic introduction. Full-game scope
remains the destination throughout those small increments.
