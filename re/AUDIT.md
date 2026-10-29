# Audit: fresh port vs `tesoro-engine` (legacy engine, comparison only)

Each row compares a ported cluster with the analysis-derived implementation in `tesoro-engine` (`98415538c49`). Findings are not copied into contracts; where they disagree the fresh contract and original-code validation win, and the engine row is listed so Gabriel can decide what to port back.

## Increment 1: core primitives (29 Oct 2026)
| Area | Fresh port (`codex/fresh-core`) | `tesoro-engine` | Verdict |
|---|---|---|---|
| Game clock gap rule | step `< 500` ms accumulates, otherwise dropped; signed compare | `elapsed < 500` accumulates | agree |
| Clock runs in which states | every state: the original freezes and unfreezes the clock in every frame, menus and movies included | only `kAdventure` | **differs**: menus and movie time do not advance in tesoro-engine; matters for anything timed outside gameplay |
| Frozen time within a frame | all reads in a frame return the value captured by `freeze` | a single `_gameTime` updated once in `update()` | agree in effect |
| Suspend / resume | `pauseEngineIntern`: suspend, resume drops inactive time | `isPaused()` early return, `_lastHostTime` reset on resume | agree in effect |
| Randomness | time-driven `randomRange` (+ `randomRangeAlt`), validated against the original | seeded `Common::RandomSource` (`getRandomNumberRng`) | **differs**: original is not seeded or replayable; distribution over the range is equivalent only on average |
| Name key | `nameKey64` validated on 522 names | strings compared directly | not comparable until lookups use keys; real original lookups can collide (case-insensitive) |
Also noted in `tesoro-engine`: `testing.h` shortcuts (`TestingShortcuts::kStartInOffice` etc.) live in engine code, against the project rule for the submitted engine (not carried into `codex/fresh-core`).

## Archive (increment 2)
Compared (29 Oct 2026) with `tesoro-engine:engines/tesoro/resources.cpp` (`Archive::open/stream/has`). Read only after the fresh port and contracts existed.

Agreements:
- TRE record: u32 offset, u32 second field, flags byte, name[19]; only records with flag bit 4 are data entries; names upper-cased; payload starts at offset + 4.

Divergences (original = fresh contract):
| Topic | Original | `tesoro-engine` | Effect |
|---|---|---|---|
| Structure | array sorted descending, binary search | hash map | none for lookups of unique names |
| Duplicate names | both kept; search hits an arbitrary one (deterministic) | fatal error | only matters if the shipped TRE has duplicates |
| Lookup case | exact, case sensitive (callers pass upper case) | upper-cases the query | legacy accepts names the original would miss; check callers when ported |
| Missing name | returns 0 (optional error box) | fatal error | caller handling to be checked |
| Name length | stored truncated to 18 chars | keeps all 19 bytes | differs only for 19-char names |
| Empty name | accepted | fatal error | likely none |
| Bounds / size checks | none | checks offset+4+size against the DAT size and the DAT's u32 header against the TRE size field | legacy extra validation, not in the original |
| Entry size | not stored; the reader of each resource decides how much to read | stored from record +4, `readStream(size)` | see open question |

Open question carried into the contract: legacy treats record +4 as the payload size and the 4 bytes at the entry offset as a size header (which would explain the original's `+4`). The fresh analysis has not confirmed either: it never uses the field. Confirm by reading the real TRE/DAT (needs your OK) or from the first reader of a resource (AnimBank_Load).

## Animation bank (increment 3)
Compared (29 Oct 2026) with `tesoro-engine:engines/tesoro/{resources,animation,renderer}.cpp` (`Reader::animation`, `AnimationPlayer::play`). Read only after the fresh port and its Unicorn validation existed.

Agreements (legacy layout matches the independently recovered one):
- Entry payload: 4 bytes (size header, the `+4` in the index offset), then `ANM` + isFont byte (the second `+4` of `AnimBank_Load`), then the 0x18 header: frame count u16, image count u16 at +4, a 12 byte name at +8 (the three "header dwords" of the contract), a flags byte at +0x14 (== 1 means interpolate movement = player flag bit 3), reserved to +0x17.
- Frame record 0x18 bytes: +0/+2 position, +4/+6 movement dx/dy (made relative to frame 0), +8 duration, +0xa image index (-1 none), +0xc..+0x13 four u16 (hit rectangle), +0x14 type, +0x16 extra size. Type 2 points at extra+6 (4 bytes each), type 4 at extra+8 (6 bytes: x, y, scale). `kInterpolate == 8` equals the contract's header flag bit.
- Image blob: header +8 width, +0xa height = the contract's N span lists (one per row); item = {value, skip u16, length u16}; kind 1 literal 5-6-5 pixels (offset from blob start), 2 shadow, 3 solid colour. Frames without image end a range; a looping range includes that frame. This resolves the contract's open questions on the second `+4` skip, header +8..+0x13 and entry/item fields (taken from legacy, consistent with every read the fresh code does; not yet confirmed on the real file).

Divergences:
| Topic | Original = fresh port | `tesoro-engine` | Effect |
|---|---|---|---|
| Fonts | one path; positions made relative for every bank | `isFont` banks (tag byte 0) skip the relative step and cannot play | check whether any font bank is loaded through `AnimBank_Load`; port will need the tag byte |
| Image decode | blob kept; solid colour/shadow values converted at load ((v&0x1f)<<2 for kind 2); 5-5-5 conversion when the display needs it; images loadable later (mode 2) | decodes to rows/spans/literals eagerly, RGB565 only, shadow value raw | rendering path differs; legacy never needs 5-5-5 or deferred loading, a ScummVM renderer picks the surface format itself |
| Brightness scaling | `ScaleBrightness` (callers unknown) | none | add when its caller is found |
| Image validation | none (trusted) | many fatal checks, kinds other than 1-3 rejected | original treats every other kind as solid colour |
| Short point data | reads stale stack bytes | reads zeros | port now also reads zeros with a warning (earlier it failed the load, which would have rejected the kitchen frames legacy mentions) |
| `SetRange` vs `play` | no argument checks; sets the first frame counters (+0x2c/+0x2e) to 0 and +0x30 to first; sum only if asked | refuses first >= count, last < first, last >= count, zero-duration cases; sets `frame = first`; resets loop movement | legacy refuses calls the original performs; the original's +0x2c/+0x2e are probably its frame/time cursor (to confirm in the player update) |
| Clock | int, game clock value | uint32 | none |
| Counts | none | rejects > 10000 frames or images | none |
