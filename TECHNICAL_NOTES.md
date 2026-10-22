# Tesoro: technical starting points

These notes accompany `FEASIBILITY_AND_PLAN.md`. Confirmed against the supplied files on 19 October 2026. No ScummVM engine implemented here.

Follow-up: a fresh Ghidra project now explicitly identifies the callback entry points, with 38 selected routines successfully decompiled. See [the annotated first batch](PSEUDOCODE_FIRST_BATCH.md) and `analysis/decompiled/`. References below to missing functions describe the earlier temporary dump, not the new project.

20 October follow-up: [the first-puzzle trace](FIRST_PUZZLE_TRACE.md) adds scene-object structure (747 records, all 50 LVLs), explicit pen-state ownership by `CAPITAN`, and the office-door call chain. There are now 47 selected Ghidra exports. The request-only tracer has six passing data-backed checks; no playable engine is claimed.

## Version identity

| File | SHA-256 |
| --- | --- |
| TESORO.EXE | `0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef` |
| TESORO.TRE | `29b116616137f1f33ce75157c9d902aed4a7021b426e7fd4e7cde5b9c2887296` |
| TESORO.DAT | `be8641cfcc02d41e4f12cbdbe9d71e01656cfdb44ad8b791864378ae297901d7` |

All executable addresses below apply to this exact executable. PE32, x86, image base `0x400000`. `.text` VA `0x401000`, file offset `0x400`, virtual size `0x260a2`. `.rdata` VA `0x428000`, file offset `0x26600`.

These SHA-256 values are investigation identities, not ready-to-use ScummVM AdvancedDetector MD5 entries. Generate detector entries using the project's appropriate hash rules and verified editions.

## Formats

All numbers below are little-endian unless stated otherwise.

- TRE has **no header**. Each 28-byte record is `u32 offset, u32 payloadSize, u8 flags, char name[19]`. File records have `flags & 4`. Both flags 4 and 6 occur on valid files. DAT offset zero is valid and contains `JIMQUIETO.anm`.
- DAT blocks are `u32 payloadSize` followed by exactly that many bytes. Read at `offset + 4`. The prefix is excluded from the indexed size. Every indexed prefix matches in this copy. Nine gaps total 3,448,956 bytes; they were not classified.
- ANM magic is `ANM\x01`, or `ANM\x00` for fonts. Header including magic is 28 bytes. Frame count is a u16 at magic+4; image count is a u16 at magic+8. Do not infer wider field widths merely from zero padding. Image-bank label for non-fonts occupies magic+12..+23. The header is followed by all frame records, then all image blocks. Frame layout is described below; recovered playback rules are in [ANIMATION_FINDINGS.md](ANIMATION_FINDINGS.md).
- Image blocks begin with a u32 buffer size. The following buffer contains `u32 pixelOffset, u32 pixelBytes, u16 width, u16 height, u16 kind, u16 unused`. Each row has `u32 spanOffset, u16 count, u16 unused`. Spans are `u32 value, u16 relativeSkip, u16 pixelCount`. Offsets are relative to the image buffer, excluding its size word. X skip is relative to the end of the preceding span. Kind 1 uses literal RGB565 pixels, kind 3 solid RGB565 colors, kind 2 shadow intensities. Grayscale shadow previews do not establish the original blending rule.
- LVL magic is `LVL\x01`, followed by three u32 fields. Bank count is at magic+8. Sequential ANM banks begin at magic+16. Object data follows banks; it has not been fully mapped for a running game. Do not scan for magic inside pixels to locate banks.
- WAV payloads are standard RIFF/WAVE. All 2,346 in this index are PCM, 22,050 Hz, mono, 8-bit. This finding does not account for external CD music.
- `.DLG` payloads are readable scripts, **not binary compiled bytecode**. `.TXT` files are text-ID/string tables. Spanish high bytes look like OEM encoding; CP850 gives readable sampled text. Verify original font/glyph mapping rather than assuming Latin-1 because the old resource viewer uses it.

### ANM frame records

`Frame` and `Image` are separate records. A frame describes an animation step and
references an image by index into the same bank's image array. The image block
above contains the encoded pixels, which the port decodes into `Image::pixels`.
Multiple frames can reference the same image; `image == -1` means no visible
image and can also delimit an animation sequence.

Each stored frame has a 24-byte fixed part followed by `extraSize` bytes:

| Byte offset | Stored type | Current engine field |
| --- | --- | --- |
| +0, +2 | i16, i16 | `x`, `y` |
| +4, +6 | i16, i16 | `dx`, `dy` |
| +8 | u16 | `duration` |
| +10 | i16 | `image` (image index, not pixel data) |
| +12, +14 | i16, i16 | `hitX`, `hitY` |
| +16, +18 | u16, u16 | `hitW`, `hitH` |
| +20 | u16 | `type` |
| +22 | u16 | `extraSize` (read by the loader, not retained in `Frame`) |
| +24 | byte[extraSize] | Type-dependent extra data; currently skipped by the port |

These names match the current engine representation, whose C++ member order
differs from the serialized order. For non-font banks, `dx`/`dy` are stored
movement positions: the original loader subtracts frame zero's position from
later frames and clears frame zero's movement. They are not per-frame velocity
increments. `duration` is used against elapsed game time in milliseconds; see
[the animation findings](ANIMATION_FINDINGS.md) for boundary and loop rules.
The original loader interprets extra data according to `type`; the field names
and fixed layout do not imply complete recovery or implementation of those
type-dependent behaviours. Evidence: [original loader export](analysis/decompiled/00401160_tesoro_load_animation_bank.raw.c).

See the current resource browser implementation and `engine-sources.md` for validated image parsing. Do not use the superseded historical layouts in `TESORO_RE_NOTES.md`.

## Executable anchors

| Address | What inspection indicates | Confidence/next check |
| --- | --- | --- |
| `0x403250` | Open DAT/TRE and initialize resource index | Confirmed by strings, call sites and earlier decompilation. |
| `0x403380` | Read 28-byte TRE records and retain flag-bit-4 files | Matches independent parser and existing reader. |
| `0x403500` | Resource lookup | Earlier decompilation shows binary lookup by name. Verify case normalization at call sites. |
| `0x401160` | ANM/font/player bank loading | Existing decoder validates the relevant layout. Remaining frame semantics require work. |
| `0x4015a0` | Load/convert image buffer | Do not duplicate original DirectDraw-dependent conversion; preserve source RGB565 semantics. |
| `0x404810` | Parse one script block into internal operations | Important anchor for labels, variables, messages and `DO` ordinals. |
| `0x405a60` | Execute script operations | Includes waits, object messages, native callbacks and media operations. |
| `0x406190` | Load/register script blocks, specialize by numeric type | Selects 21 special vtables plus the default. |
| `0x4070c0` | Initialize Jim and named player animation banks | Earlier decompilation; useful anchor for character state. |
| `0x4080e0` | Scene loading | Connect banks and object records to class factories/state. |
| `0x4132b0` | External movie playback via Windows COM APIs | Format string references `X:\INSTALL\BIN\DATA%02i.BIN`; actual media/codec unavailable. |
| `0x413e80` | Initialization call site opening TESORO.DAT/TRE | Good anchor for startup order, interface and level loading. |
| `0x401d20` | Default native callback | Verified machine instructions: `ret 4` (no operation). |

Existing temporary Ghidra dump: `/tmp/tesoro/decomp.c`. Reanalyze the executable with virtual-function targets explicitly added: several addresses in the table below are absent from that dump. Decompiler function signatures around stream parsing and COM calls are demonstrably unreliable. Treat them as navigation aids.

## Native script callbacks

The `.BLOCK N` header selects a specialized script class when N is recognized. Blocks without a type use the default class. In parser `0x404810`, a counter starts at zero for each block. A line starting with `D` creates internal operation `0x0c` with that counter and increments it. Executor `0x405a60`, case `0x0c`, calls the current script class's virtual function at vtable offset `0x70`, passing the counter. Words following `DO` are not the function name.

The table comes from class selection in `0x406190` and pointers read directly from this executable. Handler locations are established; most handler behaviours have not been reconstructed.

| Script type | Vtable | DO handler |
| ---: | --- | --- |
| 0 / default | `0x428078` | `0x401d20` (no-op) |
| 1 | `0x428a50` | `0x4197a0` |
| 2 | `0x4289d8` | `0x416300` |
| 3 | `0x428960` | `0x4198d0` |
| 4 | `0x4288e8` | `0x4169a0` |
| 5 | `0x428870` | `0x4199a0` |
| 6 | `0x4287f8` | `0x419a10` |
| 7 | `0x428780` | `0x419ac0` |
| 8 | `0x428708` | `0x419530` |
| 9 | `0x428690` | `0x419b30` |
| 10 | `0x428618` | `0x419d90` |
| 11 | `0x4285a0` | `0x401d20` (no-op) |
| 12 | `0x428528` | `0x419b90` |
| 13 | `0x4284b0` | `0x401d20` (no-op) |
| 14 | `0x428438` | `0x4193b0` |
| 15 | `0x4283c0` | `0x419cf0` |
| 16 | `0x428348` | `0x419d40` |
| 17 | `0x4282d0` | `0x417c80` |
| 18 | `0x428258` | `0x416b80` |
| 19 | `0x4281e0` | `0x416c80` |
| 20 | `0x428168` | `0x419f10` |
| 21 | `0x4280f0` | `0x416b50` |

`analysis/native-callbacks.csv` records all 87 source markers with block name, original file line, type, ordinal and target. Thirty-one are in default-type blocks; the other 56 use 19 nondefault types. These are not 56 distinct functions or proof that every marker has an effect. For example, type 17's routine only fades for ordinals 0 through 3; a callback can ignore other values.

Useful check: `.FANMTRONO 20` has `DO ir` and `DO volver`, ordinals 0 and 1, dispatched to `0x419f10`. The surviving decompilation contains that handler and shows two room/state paths. Type 1's handler has a jump table and is not fully present in the earlier analysis; add its targets before trusting a regenerated decompilation.

Do not stop at DO handlers. Object class methods, the interface, scene initialization, special sequences and save logic also contain rules absent from text scripts.

## Checks completed

1. Independently audited 2,416 indexed files: all offsets, lengths and prefix values valid; no duplicate names.
2. Independently parsed 371 banks and counted 2,012 images; checked RIFF PCM metadata/data lengths for all 2,346 WAVs.
3. Ran existing `adventure-explorer` Lua regression/full-resource sweep: passed all images and sounds, 112 sequence previews.
4. Independently decoded four PNG samples; visually inspected harbour and Jim. Correct recognizable assets, not complete rendered scenes.
5. Inspected script parser/executor, registration, movie path and native-handler pointers. Checked default no-op and sample handlers against x86 instructions.
6. Checked current upstream detection/compatibility and compared local Alcachofa formats. No Tesoro entry found in the checked upstream table/list. This does not establish whether an unpublished developer branch exists.

Not checked: original runtime behaviour, full playthrough, media codecs, all LVL properties, original frame timing, original save layout, all native callback bodies, or multiple editions. No changes made to original game files, `adventure-explorer`, or the ScummVM checkout.
