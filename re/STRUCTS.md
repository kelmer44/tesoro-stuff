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
| +0x04 | uint | stashed bank index (Obj_StashAnimState) | c |
| +0x08 | uint | stashed play flags | c |
| +0x0c | byte | animation stashed/paused (set by the Jim code, cleared by SelectAnimBank and the restore code) | c |
| +0x10,+0x14,+0x18,+0x1c | 4 ints | hit rectangle of the shown sequence: x, y, x + width, y - height | c |
| +0x20,+0x24 | 2 ints | position + movement offset before the tick | c |
| +0x28,+0x2c | 2 ints | position + movement offset after the tick (previous position if the object moved) | c |
| +0x30 | ushort | sequence shown before the tick | c |
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
| +0x18 | uint | play flags (bit0 playing, bit1 paused, bit2 loop, bit3 interpolate movement, from the header) | c |
| +0x1c | ptr | sequence array (count +0x22, 0x24 bytes each) | c |
| +0x20 | ushort | image count | c |
| +0x22 | ushort | sequence count | c |
| +0x10 | int | time of the last Pause (uninitialised before) | c |
| +0x24 | ushort | first sequence of the range | c |
| +0x26 | ushort | last sequence of the range | c |
| +0x28, +0x2a | short | movement offset computed by Advance (x, y) | c |
| +0x2c, +0x2e | short | movement gathered by completed loops (x, y) | c |
| +0x30 | ushort | sequence being played | c |
| +0x32,+0x36,+0x3a | uint | three header words copied from the file | r |

## CursorList: `List` and `ObjectList` (pooled singly linked lists with a built-in cursor)
`List` object = one pointer L to a header; `ObjectList` is the same design with priorities.
| | List | ObjectList |
|---|---|---|
| header size | 0x1c | 0x24 |
| L+0 | pointer to node block P | same |
| L+4 | head link (first node, 0 = empty) | same |
| cursor (node before the current element) | L+0xc | L+0x10 |
| capacity | L+0x10 | L+0x14 |
| at-start flag (byte) | L+0x14 | L+0x18 |
| value slot (last element read) | L+0x18 | L+0x1c |
| priority slot | none | L+0x20 |
| node | 8 bytes: next, value | 12 bytes: next, value, priority |
| block P | P+0 free-list head, P+8 bump pointer (P+0x10 first), capacity + 1 nodes | P+0 free-list head, P+0xc bump pointer (P+0x18 first), capacity + 3 nodes |
All confirmed (asm of the ten functions). Nodes are taken from the free list first, then the bump pointer; there is no capacity check.


## Global objects (provisional names; creation read in InitGameWorld 00413e60, roles inferred)
| Address | What | Conf |
|---|---|---|
| 00442064 | archive (Tesoro.dat / Tesoro.TRE) | c |
| 00442100 | memory pool | c |
| 00442104 | main object list (levels and the top object live in it, with priorities 0x13 interface, 0x14 current level, 100 top object) | c |
| 00442108 | display | c |
| 00442110 + 4*i | text tables OBJETOS, ANIMS, CONVERS, ICONS | c |
| 004420b8 | dialogue table (OBJETOS/ANIMS/CONVERS/ICONS .DLG) | c |
| 00442120 | game clock | c |
| 00442128 | main cursor | c |
| 00442130 + 4*i (i = 0..46) | the 47 levels, in the order of the table at 0042d6c8 (index also stored at level +0x98) | c |
| 004420fc | Jim | c |
| 004421ec | interface level (INTERFACE.LVL) | c |
| 004421f0 / 004421f4 | PRINCIPAL.LVL (main menu) / NUEVO.LVL (new-game menu) | c |
| 004421f8 / 004421fc | top object / interface strip object (+0xa0 child list, +0xa8 child) | w |
Level +0x98 is the level's index in the table (8 = MAPA, 14 = TABLERO); Jim_EnterLevel sets Jim's hitter flag (0x400) unless the level index is 8.

## Level (0x98+ bytes, vtable 00428bf8, extends Object; built by 004080e0)
+0x6c bank array (AnimPlayer*, one per bank) · +0x70 room array (Room, 0x10 each) · +0x74 handle of the object list (file order) · +0x78 0x34-byte list (collision pairs, capacity 0x32) · +0x7c list of collidable objects that moved (capacity 0x14) · +0x80 camera (0x40 bytes) · +0x84 room count · +0x88 bank count · +0x8c load time · +0x90 the kind 1 object · +0x94 byte "images were reloaded" flag · +0x98 index in the level table (set by InitGameWorld). Vtable: slot 4 Level_UpdateOrCollide, slot 5 Level_Update2, slot 13 RunVerb; the rest are the Object defaults.
Level file: header {rooms, banks, objects}; banks; per room a 0x28 record {+0x20 u16 objects, +0x22 u16 extent, +0x24 u32 camera divisor} then per object a 0x20 record {x, y, i16 sort, +0xb bank count, +0xc name[12], +0x18 kind, +0x1c data size} + data + bank indices.
