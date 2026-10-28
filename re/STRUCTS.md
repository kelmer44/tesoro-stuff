# Recovered structures

**Generated companion:** `STRUCTS_GENERATED.md` (all 152 classes: inheritance tree, own virtual methods, field accesses with confidence) and `types/tesoro_types.h` (C structs for Ghidra's *File > Parse C Source*), both produced by `tools/build_classes.py` from the fresh exports; hand-assigned field names live in `field_names.json`. The hand-written tables below were checked against the generated ones (Obj base, Jim, Cursor agree).
Confidence per field: `?` guessed · `r` read-only evidence · `w` written+read evidence · `c` confirmed by constructor/serializer.

| Struct | Size (alloc evidence) | Vtable | Notes |
| --- | --- | --- | --- |
| Clock | 0x10 (`InitSubsystems` operator new) | none | game time source, global 00442120 |
| Pool | 0x44 (+ buffer of param*1000 bytes) | none | four bump arenas, global 00442100 |
| Obj (base object, `Obj_Kind00`) | 0x6c | 00428000 | see below and `re/vtables.md` |
| Jim | 0x114 (`004070c0` alloc) | 00428b70 | extends the Kind05-family base |
| Cursor | 0x130 | 00428038 | `Cursor_Ctor` |

## Clock (global 00442120)
| Off | Width | Field | Conf |
|---|---|---|---|
| +0x00 | int | accumulated game ms | c |
| +0x04 | int | value frozen at frame start | c |
| +0x08 | uint | last GetTickCount | c |
| +0x0c | byte | frozen flag | c |
| +0x0d | byte | suspended flag (window inactive) | c |

## Pool (global 00442100)
| Off | Width | Field | Conf |
|---|---|---|---|
| +0x00 | void* | start of the whole buffer (size = ctor arg * 1000) | c |
| +0x04 + 4*i | ptr | start of arena i (i = 0..3) | w |
| +0x14 + 4*i | ptr | bump pointer of arena i | w |
| +0x24 + 4*i | uint | size of arena i | w |
(+0x34.. derived; the allocator recomputes a remaining-size style field for every arena after each allocation, layout not pinned down)

## Obj (base object, vtable 00428000) — offsets seen in constructors and base methods
| Off | Width | Field | Conf |
|---|---|---|---|
| +0x00 | vtable | | c |
| +0x0c | byte | animation "playing" / dirty flag (cleared by SelectAnimBank) | r |
| +0x10..+0x1c | 4 ints | bounds rect (from the current frame record) | r |
| +0x20..+0x2c | 4 ints | screen-space rect | r |
| +0x32 | byte | current anim bank index | w |
| +0x33 | byte | anim bank count | c |
| +0x34 | ptr | array of AnimPlayer* (one per bank) | c |
| +0x38 | ptr | current AnimPlayer | w |
| +0x3c | ptr | room record (16 bytes) | c |
| +0x40 | ptr | owning level | w |
| +0x44 | uint64 | name key (NameHash64) | c |
| +0x4c,+0x50 | int | position x, y (world, y up) | c |
| +0x54,+0x58 | int | previous position | w |
| +0x5c | byte | state byte (Jim: 7 = special pose) | r |
| +0x5d | byte | | ? |
| +0x5e | short | sort value (draw order / contact priority) | c |
| +0x60/+0x64 | ptr/ushort | optional state blob and its size (saved) | r |
| +0x68 | uint | flags: bit0 processed this pass, bit1 active, bit2 visible, bit3 collidable, bit4 has animation, bit8 in room list, bit9 processed (0x200), bit10 hitter (0x400) | w |
(subclasses extend from +0x6c; kind-specific layouts are in `re/vtables.md` and `re/object_kinds.json`)

## AnimPlayer (0x40 bytes, built by `AnimBank_Load`; note: the file calls it "bank")
| Off | Width | Field | Conf |
|---|---|---|---|
| +0x00 | ptr | image pointer array (count +0x20) | c |
| +0x04 | ptr | file offset array for deferred loading | c |
| +0x08 | int | start time stamp (game clock) | w |
| +0x0c | int | accumulated duration | w |
| +0x14 | int | load mode (2 = deferred images) | c |
| +0x18 | uint | play flags (bit0 playing, bit2 loop flag param, bit3 from header) | r |
| +0x1c | ptr | sequence array (count +0x22, 0x24 bytes each) | c |
| +0x20 | ushort | image count | c |
| +0x22 | ushort | sequence count | c |
| +0x24..0x30 | ushort | current range / frame position fields | r |
| +0x32,+0x36,+0x3a | uint | three header words copied from the file | r |
