# Function checklist

Generated from `ledger.json` by `tools/make_checklist.py`. Update with `tools/mark.py` (see its docstring).
`[x]` means a C++ port exists (status ported or later). The status after each entry says how far it got and how it was checked: decompiled < understood < ported < emu-compared < original-compared.

## Overall (game code: 447 functions)

| Stage | Count | Share |
|---|---|---|
| untouched or later | 447 | 100% |
| decompiled or later | 162 | 36% |
| understood or later | 0 | 0% |
| ported or later | 0 | 0% |
| emu-compared or later | 0 | 0% |
| original-compared or later | 0 | 0% |
| replaced by a ScummVM service | 0 | |

## Game code by cluster

### Cluster 1: 00401000–00401740 (0/7 ported)

- [ ] `00401000` inferred_ReturnThis · depth 0 · 7 lines · **decompiled** — __fastcall returns ecx; used as a vtable slot of the object built by 00401aa0
- [ ] `00401580` inferred_Copy64Bytes · depth 0 · 14 lines · **decompiled** — thiscall copy of 16 dwords from param into this
- [ ] `00401010` inferred_AnimBank_ScaleBrightness · depth 1 · 73 lines · **decompiled** — builds channel scale tables from a 16-bit factor (x/256) and rewrites every 16-bit image's pixels; flag at (DAT_00442108+0x67c) picks 5-5-5 vs 5-6-5 handling
- [ ] `00401740` inferred_AnimPlayer_SetRange · depth 1 · 70 lines · **decompiled** — thiscall(first, last_or_-1, loop_flag, sum_flag): sets play range in sequence table, resets counters, stamps start time from game clock 00413a10
- [ ] `004015a0` inferred_AnimBank_LoadImage · depth 8 · 82 lines · **decompiled** — reads size, allocates from pool DAT_00442100, reads blob, relocates internal pointers by base; converts pixels 5-6-5 to 5-5-5 when flag set; image type 2 (header +0xc) entries are rewritten as (v&0x1f)<<2, meaning not yet known
- [ ] `00401160` inferred_AnimBank_Load · depth 9 · 163 lines · **decompiled** — thiscall(entry, pool, mode): opens archive entry+4, reads 0x18 header (images +0x20, sequences +0x22), 0x24-byte sequence records at +0x1c, image pointer array at +0, file offsets at +4; mode 2 defers image loading
- [ ] `00401510` inferred_AnimBank_LoadDeferredImages · depth 9 · 26 lines · **decompiled** — mode 2 only: seeks to each stored offset (+4 array) and loads the image with 004015a0

### Cluster 2: 00401bc0–00401d60 (0/12 ported)

- [ ] `00401bc0` inferred_Object_BaseCtor · depth 0 · 12 lines · **decompiled** — base game object: vtable 00428000, flags +0x68 |= 6, +0x5e = 5, byte +0x5c = 0
- [ ] `00401c20` inferred_Noop (vtable) · depth 0 · 7 lines · **decompiled** — empty function; fills vtable slots 0, 1 and 7 of the base classes (also used as the clock destructor) | empty function; fills vtable slots 0, 1 and 7 of the base classes (and was the clock destructor)
- [ ] `00401c50` inferred_Obj_UpdateBounds (vtable) · depth 0 · 30 lines · **decompiled** — base slot 3: recomputes bounds rect (+0x10..+0x1c) and screen position (+0x20..+0x2c) from the current frame record (0x24-byte entries); calls slot 2 in between
- [ ] `00401d20` inferred_Obj_Stub_Void_1Arg (vtable) · depth 0 · 7 lines · **decompiled** — base slot 6 default (draw with one argument)
- [ ] `00401d60` inferred_Obj_SetActive (vtable) · depth 0 · 8 lines · **decompiled** — base slot 8: sets flag bit 1 (+0x68) from argument
- [ ] `00401860` FUN_00401860 · depth 1 · 86 lines · **untouched**
- [ ] `00401a50` FUN_00401a50 · depth 1 · 12 lines · **untouched**
- [ ] `00401a70` FUN_00401a70 · depth 1 · 15 lines · **untouched**
- [ ] `00401bf0` inferred_Object_AttachToLevel · depth 1 · 16 lines · **decompiled** — sets owner +0x40, inserts into the room list if flag 0x100 set, then calls vtable slot 0
- [ ] `00401d30` inferred_Obj_SetPosition · depth 1 · 14 lines · **decompiled** — (x, y, flag) sets previous position (+0x54/+0x58), and current (+0x4c/+0x50) when flag set; notifies the level
- [ ] `00401aa0` inferred_Obj_Kind00_Ctor · depth 2 · 63 lines · **decompiled** — 0x6c bytes, vtable 00428000. Shared base of most kinds: position +0x4c/+0x50 from record, room +0x3c, sort value +0x5e = record short + room short + 5, 64-bit name key +0x44, anim bank pointers (+0x34 array, +0x33 count, current +0x38)
- [ ] `00401c30` inferred_Obj_AnimTick (vtable) · depth 2 · 11 lines · **decompiled** — base slot 2: if object has animation (flag bit 4) advance AnimPlayer via 00401860

### Cluster 3: 00401d80–00401fa0 (0/8 ported)

- [ ] `00401d80` FUN_00401d80 · depth 0 · 8 lines · **untouched**
- [ ] `00401da0` FUN_00401da0 · depth 0 · 9 lines · **untouched**
- [ ] `00401dc0` FUN_00401dc0 · depth 0 · 13 lines · **untouched**
- [ ] `00401f90` inferred_Obj_Stub_RetFalse_1Arg (vtable) · depth 0 · 7 lines · **decompiled** — base slot 11 default (ret 4)
- [ ] `00401fa0` inferred_Obj_Stub_RetZero_2Args (vtable) · depth 0 · 7 lines · **decompiled** — base slot 12 default (ret 8)
- [ ] `00401de0` inferred_Object_SelectAnimBank · depth 2 · 15 lines · **decompiled** — picks bank index (byte +0x32), sets +0x38, starts AnimPlayer_SetRange
- [ ] `00401e20` inferred_Obj_LoadState (vtable) · depth 8 · 22 lines · **decompiled** — base slot 9: reads from a stream: current bank index (+0x32), per-frame fields of the current sequence (+0x24, +0x26, +0x30, +0x18) and an optional blob of +0x64 bytes at +0x60. Save-game deserialisation
- [ ] `00401ee0` inferred_Obj_SaveState (vtable) · depth 8 · 18 lines · **decompiled** — base slot 10: mirror of LoadState using fwrite. Save-game serialisation

### Cluster 4: 00401fb0–00402230 (0/7 ported)

- [ ] `00401fb0` inferred_Obj_Stub_RetZero (vtable) · depth 0 · 7 lines · **decompiled** — default for update/draw-style slots: returns 0
- [ ] `00402110` inferred_Camera_Ctor · depth 0 · 9 lines · **decompiled** — 0x40-byte object at level +0x80; zeroes +0 and +0x10
- [ ] `00402120` FUN_00402120 · depth 0 · 35 lines · **untouched**
- [ ] `00402200` inferred_Camera_SetBounds · depth 0 · 12 lines · **decompiled** — stores 4 ints at +0x1c..+0x28; Level_Load passes (-320, 239, 319, -240)
- [ ] `00402230` FUN_00402230 · depth 0 · 8 lines · **untouched**
- [ ] `00401fc0` FUN_00401fc0 · depth 1 · 57 lines · **untouched**
- [ ] `004021a0` inferred_Camera_MoveTo · depth 1 · 22 lines · **decompiled** — duration 0: set position now (00402120); otherwise stores start/target and a start time from the game clock

### Cluster 5: 004023f0–004028c0 (0/7 ported)

- [ ] `004023f0` inferred_Cursor_SetText · depth 0 · 43 lines · **decompiled** — copies a C string into the cursor's hover-text buffer (+0x78), or clears it; optional ushort at +0x118
- [ ] `004025b0` FUN_004025b0 · depth 1 · 49 lines · **untouched**
- [ ] `004028c0` FUN_004028c0 · depth 1 · 51 lines · **untouched**
- [ ] `00402240` FUN_00402240 · depth 2 · 42 lines · **untouched**
- [ ] `00402440` FUN_00402440 (vtable) · depth 2 · 74 lines · **untouched**
- [ ] `004026d0` FUN_004026d0 · depth 3 · 84 lines · **untouched**
- [ ] `00402300` inferred_Obj_Kind10_Ctor · depth 4 · 46 lines · **decompiled** — 0x130 bytes, vtable 00428038, extends Kind03; creates two 30-element lists, sets the mouse object bounds (DAT_0044206c +0x20 = -320 ...), so probably the pointer/cursor object

### Cluster 6: 00402e50–00403300 (0/12 ported)

- [ ] `00402e50` FUN_00402e50 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00402d80` FUN_00402d80 (vtable) · depth 1 · 24 lines · **untouched**
- [ ] `00402e60` FUN_00402e60 (vtable) · depth 1 · 8 lines · **untouched**
- [ ] `00402e80` FUN_00402e80 · depth 1 · 127 lines · **untouched**
- [ ] `004031a0` FUN_004031a0 (vtable) · depth 1 · 38 lines · **untouched**
- [ ] `00403220` FUN_00403220 · depth 1 · 11 lines · **untouched**
- [ ] `004029a0` FUN_004029a0 · depth 2 · 14 lines · **untouched**
- [ ] `004030e0` FUN_004030e0 (vtable) · depth 2 · 35 lines · **untouched**
- [ ] `00403300` FUN_00403300 · depth 4 · 10 lines · **untouched**
- [ ] `004032e0` inferred_Archive_SeekEntry · depth 5 · 8 lines · **decompiled** — fseek the archive handle to offset, returns the FILE*
- [ ] `00403250` inferred_Archive_Open · depth 10 · 28 lines · **decompiled** — thiscall(dat_path, tre_path): reads index from the .TRE (00403380), keeps the .DAT handle at +4
- [ ] `004029f0` FUN_004029f0 (vtable) · depth 12 · 156 lines · **untouched**

### Cluster 7: 00403500–00403980 (0/12 ported)

- [ ] `00403500` inferred_Archive_FindEntry · depth 0 · 59 lines · **decompiled** — binary search by name in sorted 0x18-byte index records at 00442068; returns data offset (+0x14) or fails
- [ ] `004036c0` FUN_004036c0 · depth 0 · 7 lines · **decompiled** — device ready check
- [ ] `004036e0` FUN_004036e0 · depth 0 · 9 lines · **untouched**
- [ ] `00403700` FUN_00403700 · depth 0 · 50 lines · **untouched**
- [ ] `004037c0` FUN_004037c0 · depth 0 · 23 lines · **untouched**
- [ ] `004035b0` FUN_004035b0 · depth 1 · 42 lines · **untouched**
- [ ] `004038f0` FUN_004038f0 (vtable) · depth 1 · 40 lines · **untouched**
- [ ] `00403320` FUN_00403320 · depth 5 · 11 lines · **untouched**
- [ ] `00403820` FUN_00403820 · depth 5 · 44 lines · **untouched**
- [ ] `00403980` inferred_MouseCtor · depth 5 · 46 lines · **decompiled** — DirectInput mouse, range 640x480, starts at (320,240)
- [ ] `00403360` inferred_Archive_Close · depth 6 · 10 lines · **decompiled**
- [ ] `00403380` FUN_00403380 · depth 8 · 109 lines · **untouched**

### Cluster 8: 00403a70–004040d0 (0/8 ported)

- [ ] `00403a70` inferred_MouseCenter · depth 0 · 9 lines · **decompiled**
- [ ] `00403b60` inferred_CdAudioInit · depth 0 · 52 lines · **decompiled** — MCI open cdaudio; track count at +0x1bc, -1 if failed
- [ ] `00403cb0` FUN_00403cb0 · depth 0 · 51 lines · **untouched**
- [ ] `004040b0` FUN_004040b0 · depth 0 · 7 lines · **decompiled** — returns obj[0]!=0 && obj[1]!=0
- [ ] `004040d0` FUN_004040d0 · depth 0 · 22 lines · **untouched**
- [ ] `00403aa0` FUN_00403aa0 (vtable) · depth 1 · 57 lines · **untouched**
- [ ] `00403dc0` FUN_00403dc0 · depth 1 · 14 lines · **untouched**
- [ ] `00403e00` inferred_DirectSoundInit · depth 5 · 138 lines · **decompiled** — only creates DirectSound if CD track count >= 0

### Cluster 9: 004043b0–00404630 (0/7 ported)

- [ ] `004043b0` FUN_004043b0 · depth 0 · 11 lines · **untouched**
- [ ] `00404630` FUN_00404630 · depth 2 · 69 lines · **untouched**
- [ ] `00404260` FUN_00404260 · depth 5 · 22 lines · **untouched**
- [ ] `004042c0` FUN_004042c0 · depth 8 · 54 lines · **untouched**
- [ ] `004043e0` FUN_004043e0 · depth 8 · 106 lines · **untouched**
- [ ] `00404110` FUN_00404110 · depth 9 · 46 lines · **untouched**
- [ ] `00404210` FUN_00404210 · depth 9 · 28 lines · **untouched**

### Cluster 10: 004060b0–00406140 (0/9 ported)

- [ ] `004060b0` inferred_DialogueTable_Ctor · depth 0 · 8 lines · **decompiled** — 0x2b0-byte table; entry count at +0x2ac set to 0
- [ ] `004060c0` FUN_004060c0 · depth 0 · 22 lines · **untouched**
- [ ] `00405630` FUN_00405630 · depth 1 · 108 lines · **untouched**
- [ ] `00406110` inferred_DialogueTable_Finalize · depth 2 · 19 lines · **decompiled** — calls 00405630 for each entry
- [ ] `00404810` FUN_00404810 · depth 12 · 691 lines · **untouched**
- [ ] `00405a60` FUN_00405a60 · depth 13 · 228 lines · **untouched**
- [ ] `00405fd0` FUN_00405fd0 · depth 13 · 44 lines · **untouched**
- [ ] `00405850` FUN_00405850 (vtable) · depth 14 · 70 lines · **untouched**
- [ ] `00406140` FUN_00406140 (vtable) · depth 14 · 12 lines · **untouched**

### Cluster 11: 004067a0–00407000 (0/8 ported)

- [ ] `004067a0` FUN_004067a0 · depth 0 · 67 lines · **untouched**
- [ ] `00406c50` inferred_Obj_Kind13_Update (vtable) · depth 1 · 44 lines · **decompiled** — slot 4 of kind 13: same pending-click dispatch as Interactive_Update, with an extra item-use path when no verb handler applies (calls the hovered object's slot 27)
- [ ] `004069e0` inferred_Obj_Kind01_OnAttach (vtable) · depth 2 · 35 lines · **decompiled** — vtable slot 0 of kind 1 (run by Object_AttachToLevel): resolves the walk area object by the name at +0x6c in the level's object list (+0x78), sets the camera bounds to the first frame's rectangle and moves the camera to the object's position
- [ ] `00406980` inferred_Obj_Kind01_Ctor · depth 3 · 16 lines · **decompiled** — 0x84 bytes, vtable 00428ac8, extends Kind00; Level_CreateObject stores the instance at level +0x90 (single special object per level)
- [ ] `00407000` FUN_00407000 (vtable) · depth 3 · 30 lines · **untouched**
- [ ] `00406a90` inferred_Obj_Kind13_Ctor · depth 12 · 88 lines · **decompiled** — 0xac bytes, extends MidBase; 14 per-subtype vtables selected by record byte 1; increments global instance counter 004420f8; phrase key lookup
- [ ] `00406d50` FUN_00406d50 (vtable) · depth 12 · 123 lines · **untouched**
- [ ] `00406190` inferred_DialogueTable_Load · depth 13 · 228 lines · **decompiled** — (archive offset, id): reads entries introduced by '.' bytes; each entry built by 00404810 and registered by kind

### Cluster 12: 004079d0–00407aa0 (0/11 ported)

- [ ] `004079d0` inferred_Jim_IsInPose7 (vtable) · depth 0 · 7 lines · **decompiled** — slot 11: returns (+0x5c == 7)
- [ ] `00407aa0` FUN_00407aa0 · depth 0 · 10 lines · **untouched**
- [ ] `00407730` inferred_Jim_UpdateBounds (vtable) · depth 1 · 15 lines · **decompiled** — slot 3: base UpdateBounds then multiplies the four rect values by the perspective scale (+0xfc, 0x100 = 1.0) >> 8
- [ ] `004079e0` inferred_Jim_ArriveStep (vtable) · depth 1 · 31 lines · **decompiled** — slot 12: called when a walk segment ends: updates previous position, unless the walk-area path search (WalkArea_FindPath) produces a further waypoint, calls slot 29 (face) and the target object's slot 13
- [ ] `00407600` inferred_Jim_WalkTo (vtable) · depth 2 · 55 lines · **decompiled** — slot 32: (x, y, target object, flag) starts a walk: records target +0x104/+0x108, converts position to fixed point, chooses facing 1..4 from the dominant axis, selects the matching walking animation (table +0xd8); the function always ends by calling the target object's slot 13 (RunVerb), including at walk start, purpose not yet understood
- [ ] `00407770` inferred_Obj_AnimTickResetOffset (vtable) · depth 2 · 10 lines · **decompiled** — MidBase slots 2 (and 3 in some classes): AnimTick then zero the current sequence's x/y offset (+0x28, +0x2a)
- [ ] `00407a80` inferred_Jim_SetFacing (vtable) · depth 4 · 10 lines · **decompiled** — slot 29: clears walking flag, stores direction +0x10c (1..4) and starts the matching animation
- [ ] `00407510` inferred_Jim_SaveState (vtable) · depth 8 · 12 lines · **decompiled** — slot 10: writes the same fields
- [ ] `00407580` inferred_Jim_LoadState (vtable) · depth 8 · 14 lines · **decompiled** — slot 9: stops walking, reads position (8 bytes at +0x4c), facing +0x10c, bank index +0x32, +0x110, flags +0x68
- [ ] `00407790` inferred_Jim_Update (vtable) · depth 11 · 89 lines · **decompiled** — slot 4: per-frame walking step. Position is 24.8 fixed point (+0xe0,+0xe4); the animation's own per-frame offset (+0x28,+0x2a, root motion) scaled by perspective moves Jim; ends when the squared distance reaches zero (arrival), then removes itself from the main list and either calls slot 29 (face) or the target object's slot 13 (RunVerb)
- [ ] `004070c0` inferred_Jim_Ctor · depth 12 · 282 lines · **decompiled** — 0x114-byte object, vtable 00428b70, base chain Kind05-family ctor 0040fea0 -> 0040ec90. Loads 9 animation files (JIMHABDER, JIMQUIETO, JIMARRIBA, JIMABAJO, JIMIZQUIERDA, JIMDERECHA, JIMHABAR ... .ANM) into bank array +0x34, name key from string at 0042d2a4, start position (0x15e,0x14), sort value 0x23, scale +0xfc = 0x100, speech/idle fields, calls SetFacing(3)

### Cluster 13: 00408440–00408550 (0/12 ported)

- [ ] `00408440` inferred_Level_ObjectMoved · depth 0 · 61 lines · **decompiled** — re-sorts the object in the level's sorted list at +0x7c after it moved
- [ ] `00407ac0` inferred_Jim_Draw (vtable) · depth 1 · 34 lines · **decompiled** — slot 6: updates talk state then queues the current frame scaled by +0xfc (scaled sprite, 16.16 factor) at screen position relative to the camera
- [ ] `00407d00` FUN_00407d00 · depth 1 · 71 lines · **untouched**
- [ ] `00407f30` inferred_Jim_TalkAnimate (vtable) · depth 2 · 37 lines · **decompiled** — slot 31: draws the speaking-mouth animation; while talking picks a random frame (RandomRange) at most every 100 ms of game time, else rests on frame 1
- [ ] `004080a0` inferred_Jim_UseItemForward (vtable) · depth 2 · 17 lines · **decompiled** — slot 27: forwards to a level-interface object (name key 0042d2a8) slot 27
- [ ] `00408520` inferred_Level_CameraMoveTo · depth 2 · 8 lines · **decompiled** — forwards (x, y, duration) to the camera at +0x80
- [ ] `00408550` FUN_00408550 · depth 2 · 52 lines · **untouched**
- [ ] `00408030` inferred_Jim_SetIdlePose (vtable) · depth 3 · 29 lines · **decompiled** — slot 30: direction 3,4,2,1 -> idle animation index 0,1,2,3 via 00410130, then recomputes speech anchor point +0xa4
- [ ] `00408540` inferred_Level_AttachObjects · depth 3 · 8 lines · **decompiled** — calls 0040f710 on the level's object list (+0x74), which calls 00401bf0(child, level) for every child
- [ ] `00407b80` inferred_Jim_EnterLevel · depth 11 · 50 lines · **decompiled** — (level, room, x, y) on the player: leaves the old level (removes from its lists and the main list), joins the new level, sets camera and position, inserts into the main list at priority 0x14
- [ ] `00407ce0` FUN_00407ce0 · depth 11 · 8 lines · **untouched**
- [ ] `004080e0` inferred_Level_Load · depth 15 · 142 lines · **decompiled** — level constructor and .LVL reader: base object ctor 00401bc0 (vtable 00428bf8), reads 0xc header (room count, anim-bank count), allocates lists, loads anim banks with AnimBank_Load, reads each 0x28-byte room record and its 0x20-byte object records, builds objects through Level_CreateObject; Ghidra decompile is garbled by the 0x13xx-byte frame, re-read from asm before porting

### Cluster 14: 00408780–00408890 (0/8 ported)

- [ ] `00408780` inferred_Level_FindObjectByKey · depth 1 · 8 lines · **decompiled** — forwards to the object list at +0x74 with the 64-bit key
- [ ] `00408840` FUN_00408840 (vtable) · depth 1 · 14 lines · **untouched**
- [ ] `00408890` FUN_00408890 · depth 2 · 200 lines · **untouched**
- [ ] `004087a0` FUN_004087a0 (vtable) · depth 3 · 17 lines · **untouched**
- [ ] `004087d0` FUN_004087d0 (vtable) · depth 3 · 10 lines · **untouched**
- [ ] `00408620` FUN_00408620 · depth 8 · 77 lines · **untouched**
- [ ] `00408710` FUN_00408710 · depth 8 · 20 lines · **untouched**
- [ ] `00408800` FUN_00408800 · depth 10 · 18 lines · **untouched**

### Cluster 15: 00409280–0040c040 (0/7 ported)

- [ ] `00409280` FUN_00409280 · depth 0 · 124 lines · **untouched**
- [ ] `00409560` FUN_00409560 · depth 0 · 142 lines · **untouched**
- [ ] `00409790` FUN_00409790 · depth 0 · 255 lines · **untouched**
- [ ] `00409c10` FUN_00409c10 · depth 0 · 113 lines · **untouched**
- [ ] `00408c50` FUN_00408c50 · depth 1 · 256 lines · **untouched**
- [ ] `00409e50` inferred_Level_CreateObject · depth 14 · 1145 lines · **decompiled** — factory: switch on kind (record +0x18) allocating the right object class (sizes 0x6c, 0x84, 0x78, 0x6c, 0x74, 0xd8 ... kind 5 has a sub-switch on record byte +5); adds objects of kind 1 to level +0x90
- [ ] `0040c040` FUN_0040c040 (vtable) · depth 14 · 19 lines · **untouched**

### Cluster 16: 0040c0e0–0040c4d0 (0/12 ported)

- [ ] `0040c0e0` FUN_0040c0e0 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `0040c1f0` FUN_0040c1f0 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `0040c090` FUN_0040c090 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c0f0` FUN_0040c0f0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c140` FUN_0040c140 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `0040c200` FUN_0040c200 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c250` FUN_0040c250 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `0040c2c0` FUN_0040c2c0 (vtable) · depth 14 · 31 lines · **untouched**
- [ ] `0040c370` FUN_0040c370 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c3c0` FUN_0040c3c0 (vtable) · depth 14 · 31 lines · **untouched**
- [ ] `0040c480` FUN_0040c480 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c4d0` FUN_0040c4d0 (vtable) · depth 14 · 27 lines · **untouched**

### Cluster 17: 0040c560–0040c810 (0/7 ported)

- [ ] `0040c560` FUN_0040c560 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040c5d0` FUN_0040c5d0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040c640` FUN_0040c640 (vtable) · depth 14 · 30 lines · **untouched**
- [ ] `0040c6d0` FUN_0040c6d0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c720` FUN_0040c720 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c770` FUN_0040c770 (vtable) · depth 14 · 30 lines · **untouched**
- [ ] `0040c810` FUN_0040c810 (vtable) · depth 14 · 27 lines · **untouched**

### Cluster 18: 0040c8a0–0040ca60 (0/9 ported)

- [ ] `0040c8a0` FUN_0040c8a0 (vtable) · depth 1 · 9 lines · **untouched**
- [ ] `0040c8d0` FUN_0040c8d0 (vtable) · depth 2 · 13 lines · **untouched**
- [ ] `0040c9e0` FUN_0040c9e0 (vtable) · depth 2 · 14 lines · **untouched**
- [ ] `0040c880` FUN_0040c880 (vtable) · depth 13 · 13 lines · **untouched**
- [ ] `0040c8f0` FUN_0040c8f0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c940` FUN_0040c940 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c990` FUN_0040c990 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040ca10` FUN_0040ca10 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040ca60` FUN_0040ca60 (vtable) · depth 14 · 19 lines · **untouched**

### Cluster 19: 0040cab0–0040cd60 (0/8 ported)

- [ ] `0040cab0` FUN_0040cab0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040cb00` FUN_0040cb00 (vtable) · depth 14 · 22 lines · **untouched**
- [ ] `0040cb60` FUN_0040cb60 (vtable) · depth 14 · 20 lines · **untouched**
- [ ] `0040cbd0` FUN_0040cbd0 (vtable) · depth 14 · 20 lines · **untouched**
- [ ] `0040cc40` FUN_0040cc40 (vtable) · depth 14 · 25 lines · **untouched**
- [ ] `0040ccc0` FUN_0040ccc0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040cd10` FUN_0040cd10 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040cd60` FUN_0040cd60 (vtable) · depth 14 · 28 lines · **untouched**

### Cluster 20: 0040cdf0–0040d1a0 (0/10 ported)

- [ ] `0040cdf0` FUN_0040cdf0 (vtable) · depth 2 · 13 lines · **untouched**
- [ ] `0040cfd0` FUN_0040cfd0 (vtable) · depth 2 · 13 lines · **untouched**
- [ ] `0040ce20` FUN_0040ce20 (vtable) · depth 14 · 20 lines · **untouched**
- [ ] `0040ce90` FUN_0040ce90 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040cee0` FUN_0040cee0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040cf30` FUN_0040cf30 (vtable) · depth 14 · 34 lines · **untouched**
- [ ] `0040d000` FUN_0040d000 (vtable) · depth 14 · 20 lines · **untouched**
- [ ] `0040d070` FUN_0040d070 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `0040d100` FUN_0040d100 (vtable) · depth 14 · 29 lines · **untouched**
- [ ] `0040d1a0` FUN_0040d1a0 (vtable) · depth 14 · 37 lines · **untouched**

### Cluster 21: 0040d240–0040d5a0 (0/10 ported)

- [ ] `0040d240` FUN_0040d240 (vtable) · depth 2 · 14 lines · **untouched**
- [ ] `0040d330` FUN_0040d330 (vtable) · depth 2 · 16 lines · **untouched**
- [ ] `0040d270` FUN_0040d270 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d2c0` FUN_0040d2c0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040d380` FUN_0040d380 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d3d0` FUN_0040d3d0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040d450` FUN_0040d450 (vtable) · depth 14 · 27 lines · **untouched**
- [ ] `0040d4e0` FUN_0040d4e0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d530` FUN_0040d530 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040d5a0` FUN_0040d5a0 (vtable) · depth 14 · 24 lines · **untouched**

### Cluster 22: 0040d620–0040d9a0 (0/12 ported)

- [ ] `0040d620` FUN_0040d620 (vtable) · depth 2 · 14 lines · **untouched**
- [ ] `0040d820` FUN_0040d820 (vtable) · depth 2 · 16 lines · **untouched**
- [ ] `0040d910` FUN_0040d910 (vtable) · depth 2 · 14 lines · **untouched**
- [ ] `0040d650` FUN_0040d650 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d6a0` FUN_0040d6a0 (vtable) · depth 14 · 20 lines · **untouched**
- [ ] `0040d6f0` FUN_0040d6f0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d740` FUN_0040d740 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d790` FUN_0040d790 (vtable) · depth 14 · 26 lines · **untouched**
- [ ] `0040d870` FUN_0040d870 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d8c0` FUN_0040d8c0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d950` FUN_0040d950 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040d9a0` FUN_0040d9a0 (vtable) · depth 14 · 27 lines · **untouched**

### Cluster 23: 0040db40–0040dd40 (0/12 ported)

- [ ] `0040db40` inferred_Pool_AllocZero · depth 0 · 38 lines · **decompiled** — bump-allocates size bytes (4-aligned) zeroed from arena index in global pool 00442100; returns 0 when full
- [ ] `0040dc00` inferred_Pool_IsValid · depth 0 · 7 lines · **decompiled**
- [ ] `0040dc10` inferred_Pool_AllocAligned · depth 0 · 25 lines · **decompiled** — thiscall(size, align, arena) no zero fill
- [ ] `0040dc70` inferred_Pool_AllocAlignedZero · depth 0 · 36 lines · **decompiled** — thiscall(size, align, arena) zeroed
- [ ] `0040dcf0` inferred_Pool_SetArena · depth 0 · 20 lines · **decompiled** — carves arena N of given size out after the previous one (sizes 0x17a6b0, 3000000, 9600000, 650000 for arenas 0-3)
- [ ] `0040dd30` inferred_Pool_ResetArena · depth 0 · 8 lines · **decompiled** — rewinds arena bump pointer to its start
- [ ] `0040dd40` inferred_Obj_Kind23_Ctor · depth 4 · 20 lines · **decompiled** — 0xa4 bytes, vtable 0042b8e0, extends MidBase, sort value +0x5e = 2 (0 when flag byte set)
- [ ] `0040dbc0` inferred_Pool_Ctor · depth 5 · 11 lines · **decompiled** — allocates param*1000 bytes (called with 0x39d0 = about 14.8 MB)
- [ ] `0040dbf0` inferred_Pool_Dtor · depth 5 · 8 lines · **decompiled**
- [ ] `0040da30` FUN_0040da30 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040da80` FUN_0040da80 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040daf0` FUN_0040daf0 (vtable) · depth 14 · 19 lines · **untouched**

### Cluster 24: 0040e580–0040e720 (0/12 ported)

- [ ] `0040e580` FUN_0040e580 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `0040e590` FUN_0040e590 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `0040e5a0` FUN_0040e5a0 · depth 1 · 9 lines · **untouched**
- [ ] `0040dfc0` FUN_0040dfc0 (vtable) · depth 2 · 12 lines · **untouched**
- [ ] `0040e560` FUN_0040e560 (vtable) · depth 2 · 10 lines · **untouched**
- [ ] `0040e6a0` FUN_0040e6a0 (vtable) · depth 2 · 8 lines · **untouched**
- [ ] `0040e6b0` FUN_0040e6b0 (vtable) · depth 2 · 20 lines · **untouched**
- [ ] `0040e5c0` inferred_Obj_Kind03_Ctor · depth 3 · 30 lines · **decompiled** — 0x6c bytes, vtable 0042b9c0, extends Kind00; record byte 1 selects an animation start mode via AnimPlayer_SetRange (0 stay, 1 single frame, 2 loop from, 3 loop with flag)
- [ ] `0040df70` inferred_Obj_Kind22_Ctor · depth 4 · 12 lines · **decompiled** — 0xa4 bytes, vtable 0042b950, extends MidBase, sort value +0x5e = 1
- [ ] `0040e720` inferred_Obj_Kind04_Ctor · depth 4 · 10 lines · **decompiled** — 0x74 bytes, vtable 0042b9f8, extends Kind03
- [ ] `0040ddc0` FUN_0040ddc0 (vtable) · depth 14 · 73 lines · **untouched**
- [ ] `0040e000` FUN_0040e000 (vtable) · depth 15 · 142 lines · **untouched**

### Cluster 25: 0040f020–0040f020 (0/11 ported)

- [ ] `0040f020` inferred_Obj_SetVerbAndClick (vtable) · depth 0 · 10 lines · **decompiled** — MidBase slot 14: stores verb +0x9c then calls slot 19 (OnClick)
- [ ] `0040ebd0` FUN_0040ebd0 (vtable) · depth 1 · 14 lines · **untouched**
- [ ] `0040ec00` FUN_0040ec00 (vtable) · depth 1 · 24 lines · **untouched**
- [ ] `0040ec90` inferred_Obj_MidBase_DefaultCtor · depth 1 · 9 lines · **decompiled** — base object ctor with vtable 0042ba68 only
- [ ] `0040e760` FUN_0040e760 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `0040efa0` inferred_Obj_Draw (vtable) · depth 2 · 24 lines · **decompiled** — MidBase slot 6: converts world position to screen (camera at argument, +0x140/+0xf0 centre) and queues the current frame's image through Display_AddSprite with the object's sort value (+0x5e)
- [ ] `0040e890` FUN_0040e890 (vtable) · depth 3 · 103 lines · **untouched**
- [ ] `0040ecb0` inferred_Obj_MidBase_Ctor · depth 3 · 14 lines · **decompiled** — Kind00 plus vtable 0042ba68 and +0x68 bits from the current animation; base of kinds 8, 11, 13, 15, 21, 22, 23, 25
- [ ] `0040ee70` inferred_Obj_Interactive_Update (vtable) · depth 3 · 52 lines · **decompiled** — MidBase slot 4: when a click is pending (+0x8c), clears it, removes the object from the main list, runs the verb handler for the current verb (index +0x9c, slots 20-26), clears the cursor's target (+0x12c), resets the cursor animation and calls slot 27 with verb-7. Pending flag is cleared at the end
- [ ] `0040e7e0` inferred_Obj_Kind16_Ctor · depth 4 · 25 lines · **decompiled** — 0xa0 bytes, vtable 0042ba30, extends Kind03; stores a time stamp from the game clock and a random value from RandomRange(+0x6c,+0x70)
- [ ] `0040ed10` inferred_Obj_Kind09_Ctor · depth 12 · 61 lines · **decompiled** — 0xa0 bytes. Constructor also used for kinds 12, 17 and 20 with 12/17/30 per-subtype vtables selected by record byte 1, and as base of kinds 5, 6, 24. Home position +0x94/+0x98, text key looked up with PhraseLookup, sets +0x68 flag bits

### Cluster 26: 0040f040–0040f270 (0/7 ported)

- [ ] `0040f040` inferred_Obj_Slot15_ForwardToSlot14 (vtable) · depth 0 · 9 lines · **decompiled** — MidBase slot 15: calls slot 14 with its argument
- [ ] `0040f1a0` inferred_Obj_OnClick (vtable) · depth 0 · 18 lines · **decompiled** — MidBase slot 19: interrupts the player if busy, copies the clicked verb for the player object itself, sets the player's target to the object's approach point (+0x94,+0x98) via player slot 32 (walk), then player slot 4
- [ ] `0040f270` inferred_Obj_Slot27_Stub (vtable) · depth 0 · 7 lines · **decompiled** — MidBase slot 27 default (ret 4, returns 0)
- [ ] `0040f050` inferred_Obj_ShowHoverText (vtable) · depth 1 · 15 lines · **decompiled** — MidBase slot 17: puts the object's phrase text (+0x6c record, then the 64-bit name key) into the cursor's hover text via Cursor_SetText
- [ ] `0040f0b0` inferred_Obj_ClearHoverText (vtable) · depth 1 · 8 lines · **decompiled** — MidBase slot 18: Cursor_SetText(null)
- [ ] `0040f0d0` inferred_Obj_RunVerb (vtable) · depth 3 · 40 lines · **decompiled** — MidBase slot 13: tells the player object (global 004420fc) to select verb +0x90, then dispatches the verb handler like Interactive_Update
- [ ] `0040f210` inferred_Obj_Verb0_Default (vtable) · depth 11 · 16 lines · **decompiled** — MidBase slot 20: marks click pending (+0x8c, +0x8d), says phrase key +0x70 if set via GameState_Say, sets cursor busy, inserts object into the main list at priority 5

### Cluster 27: 0040f670–0040f780 (0/11 ported)

- [ ] `0040f670` FUN_0040f670 · depth 0 · 12 lines · **untouched**
- [ ] `0040f680` FUN_0040f680 · depth 0 · 55 lines · **untouched**
- [ ] `0040f780` inferred_List_Remove · depth 0 · 49 lines · **decompiled** — removes the node holding the pointer
- [ ] `0040f590` inferred_List_Ctor · depth 1 · 37 lines · **decompiled** — pooled doubly-linked list header (0x1c bytes) with node storage of capacity*8+0x18 bytes, or a copy of another list
- [ ] `0040f650` inferred_Pool_AllocAlignedZero_Global · depth 1 · 8 lines · **decompiled** — wrapper on the global pool
- [ ] `0040f710` inferred_ObjectList_ForEachAttach · depth 2 · 39 lines · **decompiled** — iterates list and calls 00401bf0(node object, arg)
- [ ] `0040f280` inferred_Obj_Verb1_Default (vtable) · depth 12 · 25 lines · **decompiled** — slot 21 of Kind09-based classes: same pattern with phrase key +0x74 and a default phrase when unset
- [ ] `0040f300` inferred_Obj_Verb3_Default (vtable) · depth 12 · 25 lines · **decompiled** — slot 22: phrase key +0x7c
- [ ] `0040f380` inferred_Obj_Verb2_Default (vtable) · depth 12 · 25 lines · **decompiled** — slot 23: phrase key +0x78
- [ ] `0040f400` inferred_Obj_Verb4_Default (vtable) · depth 12 · 25 lines · **decompiled** — slot 24: phrase key +0x80
- [ ] `0040f480` inferred_Obj_UseItem_Default (vtable) · depth 12 · 50 lines · **decompiled** — slot 27: handles using an inventory item on the object: looks the item up in the object's item table (+0xa0 pairs of name keys, +0xa4 responses) else says a default phrase (ICON01T058 or MODELO2)

### Cluster 28: 0040f800–0040fbb0 (0/7 ported)

- [ ] `0040f800` inferred_List_PushBack · depth 0 · 40 lines · **decompiled** — iterates to the last node and links a new node holding the pointer
- [ ] `0040f870` FUN_0040f870 · depth 0 · 14 lines · **untouched**
- [ ] `0040f900` inferred_ObjectList_InsertByPriority · depth 0 · 82 lines · **decompiled** — 12-byte nodes (object, priority): inserts before the first node with a higher priority; main list uses priorities 0x13 (interface level), 100 (top object)
- [ ] `0040fa10` FUN_0040fa10 · depth 0 · 82 lines · **untouched**
- [ ] `0040fb20` inferred_ObjectList_RemoveByObject · depth 0 · 58 lines · **decompiled** — removes the node holding the given object from the main list
- [ ] `0040f890` inferred_ObjectList_Ctor · depth 1 · 23 lines · **decompiled** — allocates a linked list with capacity param (100 for the main list 00442104)
- [ ] `0040fbb0` FUN_0040fbb0 · depth 2 · 38 lines · **untouched**

### Cluster 29: 0040fc20–00410330 (0/12 ported)

- [ ] `0040fc20` inferred_ObjectList_PassA · depth 0 · 83 lines · **decompiled** — walks active objects (flag +0x68 bit 1), marks 0x200, calls object vtable slot 4; returns 2 or 3 immediately if an object returns that, else 1
- [ ] `0040fd20` inferred_ObjectList_PassB · depth 0 · 83 lines · **decompiled** — same walk calling vtable slot 5
- [ ] `0040fe20` FUN_0040fe20 · depth 0 · 14 lines · **untouched**
- [ ] `0040fe40` inferred_ObjectList_EndFrame · depth 0 · 31 lines · **decompiled** — iterator reset helper; its result decides loop continuation
- [ ] `0040fea0` inferred_Obj_Kind05Base_Ctor · depth 2 · 9 lines · **decompiled** — MidBase default ctor then vtable 0042bb48
- [ ] `0040ffa0` FUN_0040ffa0 · depth 2 · 27 lines · **untouched**
- [ ] `00410060` FUN_00410060 (vtable) · depth 2 · 34 lines · **untouched**
- [ ] `00410130` inferred_Jim_SetPoseAnim (vtable) · depth 2 · 41 lines · **decompiled** — (direction index) recomputes speech anchor and selects the bank listed in +0xb0..
- [ ] `004102e0` inferred_Obj_Kind05_Draw (vtable) · depth 3 · 20 lines · **decompiled** — slot 6 of the kind 5 class family: wraps Obj_Draw with slot 31 calls depending on flags +0xd4/+0xc/+0xd5
- [ ] `004101f0` inferred_Obj_Kind05_Update (vtable) · depth 10 · 46 lines · **decompiled** — slot 4 of the kind 5 class family: runs Interactive_Update unless deferred, polls keyboard/mouse button state and the sound manager to stop a playing sound on input, calls slot 28
- [ ] `00410330` inferred_Player_Say · depth 10 · 33 lines · **decompiled** — (phrase record, id, flag) on the object in global 004420fc (walks, so probably the player): stores phrase +0xc4 / id +0xcc, busy +0xd4=1, starts the phrase sound (record +8) through the sound manager, slot 30 (0x78) with flag, inserts itself into the main list at priority 6, calls slot 4 | see ledger note
- [ ] `0040fec0` inferred_Obj_Kind05_Ctor · depth 13 · 40 lines · **decompiled** — 0xd8 bytes, extends Kind09; 17 per-subtype vtables chosen by record byte 5; scans anim banks for names starting with two 3-char tags

### Cluster 30: 004103f0–004108a0 (0/8 ported)

- [ ] `004103f0` FUN_004103f0 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `004104a0` inferred_Room_InsertSorted · depth 0 · 44 lines · **decompiled** — inserts object into the room list ordered by object field +0x4c
- [ ] `00410530` inferred_Room_RemoveObject · depth 0 · 38 lines · **decompiled**
- [ ] `004105a0` FUN_004105a0 · depth 0 · 177 lines · **untouched**
- [ ] `004108a0` FUN_004108a0 · depth 0 · 215 lines · **untouched**
- [ ] `00410400` inferred_Jim_ClearSpeech (vtable) · depth 1 · 10 lines · **decompiled** — slot 28: clears phrase +0xc4 and talking flag +0xd4, removes itself from the main list
- [ ] `00410420` inferred_Room_Ctor · depth 1 · 26 lines · **decompiled** — 16-byte room record: +0 index, +4 a field from the file record +0x24, +8/+10 shorts, +0xc object list
- [ ] `004103d0` inferred_Obj_SetFacingAnim (vtable) · depth 3 · 9 lines · **decompiled** — selects the bank from +0xac and starts animation index (param-1) with AnimPlayer

### Cluster 31: 00410cc0–00411f80 (0/12 ported)

- [ ] `00410cc0` FUN_00410cc0 · depth 0 · 173 lines · **untouched**
- [ ] `00410fb0` FUN_00410fb0 · depth 0 · 177 lines · **untouched**
- [ ] `004112d0` FUN_004112d0 · depth 0 · 159 lines · **untouched**
- [ ] `00411580` FUN_00411580 · depth 0 · 164 lines · **untouched**
- [ ] `00411be0` FUN_00411be0 · depth 0 · 10 lines · **untouched**
- [ ] `00411c00` FUN_00411c00 · depth 0 · 7 lines · **untouched**
- [ ] `00411c10` inferred_Display_SetFont · depth 0 · 8 lines · **decompiled** — stores pointer at slot (index*4+0x20)
- [ ] `00411c80` FUN_00411c80 · depth 0 · 127 lines · **untouched**
- [ ] `00411f20` inferred_Display_ResetRenderQueue · depth 0 · 30 lines · **decompiled** — clears two arrays of 0x1e0 entries (stride 500 and 200 dwords) and counters
- [ ] `00411870` FUN_00411870 · depth 1 · 127 lines · **untouched**
- [ ] `00411f80` inferred_Display_AddSprite · depth 1 · 57 lines · **decompiled** — (x, y, sort value, image, flag): clips a 640x480 sprite and queues it in the render rows (per-scanline lists) by image type (1 or 3 get extra passes)
- [ ] `00411c20` inferred_Display_LoadColourTable · depth 8 · 22 lines · **decompiled** — reads 0x8000 bytes (TABLA1.TBL) into display table +0x66c, converts to 5-5-5 when flag +0x67c set

### Cluster 32: 004120c0–00412e10 (0/7 ported)

- [ ] `004120c0` inferred_Display_AddSpriteScaled · depth 0 · 21 lines · **decompiled** — like AddSprite with x/y scale factors (16.16 fixed)
- [ ] `00412640` FUN_00412640 · depth 0 · 282 lines · **untouched**
- [ ] `00412bb0` FUN_00412bb0 · depth 0 · 34 lines · **untouched**
- [ ] `00412c50` FUN_00412c50 · depth 0 · 99 lines · **untouched**
- [ ] `00412120` FUN_00412120 · depth 1 · 101 lines · **untouched**
- [ ] `004123e0` FUN_004123e0 · depth 1 · 118 lines · **untouched**
- [ ] `00412e10` FUN_00412e10 · depth 2 · 263 lines · **untouched**

### Cluster 33: 004137c0–004139e0 (0/7 ported)

- [ ] `004137c0` FUN_004137c0 · depth 0 · 67 lines · **untouched**
- [ ] `004138b0` FUN_004138b0 · depth 0 · 47 lines · **untouched**
- [ ] `004139c0` inferred_Clock_Ctor · depth 0 · 14 lines · **decompiled** — game clock: +0 accumulated ms, +4 frozen value, +8 last tick, +0xc frozen, +0xd suspended
- [ ] `004139e0` FUN_004139e0 · depth 0 · 18 lines · **untouched**
- [ ] `004132b0` inferred_Movie_Start · depth 11 · 123 lines · **decompiled** — DirectShow setup via CoCreateInstance, builds path c:\INSTALL\BIN\DATA\%02i.BIN; last call of InitGameWorld. Only the setup was read, what it plays and when is unconfirmed
- [ ] `00413570` inferred_TextTable_Load · depth 11 · 121 lines · **decompiled** — (archive offset): reads lines of the OBJETOS.TXT-style table and stores records of 16 bytes; first pass counts lines
- [ ] `00413940` inferred_PhraseLookup · depth 11 · 27 lines · **decompiled** — (name) hashes the name with NameHash64 and searches the four text tables 00442110..0044211c; MessageBox "frase no encontrada" if missing

### Cluster 34: 00413a10–00414560 (0/12 ported)

- [ ] `00413a10` inferred_Clock_Now · depth 0 · 19 lines · **decompiled** — adds elapsed GetTickCount delta when below 500 ms (longer gaps are ignored); returns frozen value while frozen
- [ ] `00413a50` inferred_Clock_Freeze · depth 0 · 18 lines · **decompiled** — updates accumulation then freezes at the current value (called at start of each frame)
- [ ] `00413a80` inferred_Clock_Suspend · depth 0 · 8 lines · **decompiled** — window deactivated: stop accumulating
- [ ] `00413a90` inferred_Clock_Resume · depth 0 · 12 lines · **decompiled** — window activated: re-stamp last tick, resume
- [ ] `00413ab0` inferred_Clock_Unfreeze · depth 0 · 8 lines · **decompiled** — clears frozen flag (end of frame)
- [ ] `00413ac0` FUN_00413ac0 · depth 0 · 11 lines · **untouched**
- [ ] `00413b00` inferred_RandomRange · depth 0 · 13 lines · **decompiled** — THE game random generator: builds a pseudo-random fraction from bit-shuffled GetTickCount() low bits, scales by (max-min+1) >> 10, adds min. Time-based, not seeded
- [ ] `00413b90` inferred_InitSubsystems · depth 6 · 140 lines · **decompiled** — builds CD, input, sound, video objects; error dialogs
- [ ] `004144a0` inferred_Shutdown · depth 7 · 45 lines · **decompiled** — deletes display, sound, mouse, keyboard, archive, memory pools, clock in that order
- [ ] `00414420` inferred_RunFrame · depth 10 · 21 lines · **decompiled** — per-frame step: keyboard and mouse vtable[0] update, clock freeze, sound update, rewind pool 3, reset render queue, object-list pass A (and pass B unless A returned 3), clock unfreeze; returns false when loop should end
- [ ] `00414560` FUN_00414560 · depth 14 · 25 lines · **untouched**
- [ ] `00413e60` inferred_InitGameWorld · depth 16 · 252 lines · **decompiled** — opens archive Tesoro.DAT/Tesoro.TRE, loads TABLA1.TBL, FUENTES.ANM, OBJETOS.TXT/DLG tables, INTERFACE.LVL, CUBIERTA/MAPA/DESPACHO..LVL levels (0x2f slots), PRINCIPAL.LVL, NUEVO.LVL, then starts the first level; SEH frame

### Cluster 35: 00414740–00414dc0 (0/12 ported)

- [ ] `00414740` inferred_WalkArea_ScaleAt · depth 0 · 86 lines · **decompiled** — (x, y) piecewise interpolation of a perspective scale (default 0x100) from the walk-area edge table in the current frame record | (x, y) perspective scale 0x100 = 1.0: finds the polyline edges spanning x, interpolates their y and per-vertex scale, then interpolates the scale vertically between the edge above and the edge below the point; 0x100 when no edge pair brackets the point
- [ ] `00414600` inferred_Keyboard_Dtor · depth 1 · 9 lines · **decompiled** — vtable 00428070
- [ ] `00414610` inferred_Mouse_Dtor · depth 1 · 9 lines · **decompiled** — vtable 00428074
- [ ] `004146b0` FUN_004146b0 (vtable) · depth 1 · 23 lines · **untouched**
- [ ] `00414a90` FUN_00414a90 (vtable) · depth 1 · 92 lines · **untouched**
- [ ] `00414d50` FUN_00414d50 (vtable) · depth 1 · 18 lines · **untouched**
- [ ] `004145f0` FUN_004145f0 (vtable) · depth 3 · 8 lines · **untouched**
- [ ] `00414620` inferred_Obj_Kind07_Ctor · depth 3 · 19 lines · **decompiled** — 0x7c bytes, vtable 0042bc78, extends Kind00
- [ ] `00414930` inferred_Obj_Kind02_Ctor · depth 3 · 81 lines · **decompiled** — 0x78 bytes, vtable 0042bcb0, extends Kind00; builds a list of up to N children resolved by name key from the record
- [ ] `00414c40` inferred_Exit_Activate (vtable) · depth 12 · 27 lines · **decompiled** — kind 6/other exit-style handler: when Jim is idle, resets display queues, looks up the destination level in the level table 00442130 by index (+0xa0) and name key (+0xa4,+0xa8), and moves Jim there with Jim_EnterLevel
- [ ] `00414dc0` FUN_00414dc0 (vtable) · depth 12 · 12 lines · **untouched**
- [ ] `00414be0` inferred_Obj_Kind06_Ctor · depth 13 · 16 lines · **decompiled** — 0xb0 bytes, vtable 0042bce8, extends Kind09; 10 per-subtype vtables chosen by record byte 1

### Cluster 36: 00414e80–00415440 (0/12 ported)

- [ ] `00414e80` inferred_WalkArea_NextWaypoint (vtable) · depth 0 · 31 lines · **decompiled** — vtable slot 13: runs each time Jim reaches a vertex. Steps the vertex index +0xa0 one towards the destination index +0xa4 and sends Jim to that vertex (Jim slot 32, target = this); when the indices are equal first sends Jim to the real destination (+0xa8,+0xac) with the original target (+0xb0)
- [ ] `00414f10` inferred_WalkArea_FindPath · depth 0 · 73 lines · **decompiled** — (x, y, target) picks the nearest waypoint of the walk area to the destination and prepares a route; returns whether a route exists | (x, y, target) finds the polyline vertex nearest the destination (+0xa4) and the vertex nearest Jim (+0xa0), stores the destination (+0xa8,+0xac) and target (+0xb0), then sends Jim to his nearest vertex; always returns true when the polyline has vertices. No search algorithm: movement is along the vertex list
- [ ] `00415120` FUN_00415120 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00415130` FUN_00415130 (vtable) · depth 0 · 43 lines · **untouched**
- [ ] `00415200` FUN_00415200 · depth 0 · 23 lines · **untouched**
- [ ] `00415230` inferred_TopObject_Ctor · depth 1 · 9 lines · **decompiled** — base object with vtable 0042be08, inserted with priority 100 (frontmost)
- [ ] `00415290` FUN_00415290 (vtable) · depth 1 · 27 lines · **untouched**
- [ ] `004150d0` inferred_Obj_Kind14_Ctor · depth 3 · 17 lines · **decompiled** — 0x74 bytes, vtable 0042bdd0, extends Kind00; inserts itself in its room list
- [ ] `00415350` FUN_00415350 (vtable) · depth 3 · 40 lines · **untouched**
- [ ] `00415440` FUN_00415440 (vtable) · depth 3 · 17 lines · **untouched**
- [ ] `00414e00` inferred_WalkArea_Ctor · depth 4 · 13 lines · **decompiled** — 0xb4 bytes, vtable 0042bd60, extends Obj_MidBase | kind 8, 0xb4 bytes, vtable 0042bd60 (only slot 13 overridden), extends MidBase; starts the single-frame animation whose frame record holds the vertex list. Rename of inferred_Obj_Kind08_Ctor
- [ ] `00415250` inferred_Obj_Kind11_Ctor · depth 4 · 10 lines · **decompiled** — 0xbc bytes, vtable 0042be40, extends MidBase

### Cluster 37: 004157c0–00415960 (0/8 ported)

- [ ] `004157c0` FUN_004157c0 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `004157b0` FUN_004157b0 (vtable) · depth 1 · 8 lines · **untouched**
- [ ] `004157d0` inferred_MainWndProc · depth 2 · 34 lines · **decompiled** — WM_ACTIVATEAPP (0x1c): clock suspend/resume and restores the two display surfaces via vtable +0x6c; MM_MCINOTIFY (0x3b9) success: CD audio notify handler 00403dc0
- [ ] `00415490` FUN_00415490 (vtable) · depth 3 · 85 lines · **untouched**
- [ ] `00415640` FUN_00415640 (vtable) · depth 3 · 64 lines · **untouched**
- [ ] `00415770` inferred_Obj_Kind15_Ctor · depth 4 · 11 lines · **decompiled** — 0xa0 bytes, vtable 0042beb0, extends MidBase, sort value +0x5e = 0x32
- [ ] `00415880` inferred_CreateMainWindow · depth 7 · 37 lines · **decompiled** — RegisterClass + CreateWindowEx 640x480 (style 0x90000000, ex 8), ShowWindow, ShowCursor(0), sets init flags, calls InitSubsystems
- [ ] `00415960` inferred_WinMain · depth 17 · 89 lines · **decompiled** — args: none=CD check+SETUP.EXE; '_$_$_ '+byte=game; 'Uninstall'

### Cluster 38: 00415c30–00416170 (0/12 ported)

- [ ] `00415c30` inferred_SetInitFlags · depth 0 · 11 lines · **decompiled** — sets globals 00442208 and 0044221c to 1
- [ ] `00415c40` inferred_NameHash64 · depth 0 · 44 lines · **decompiled** — packs up to 12 chars of a name, 5 bits each (chars below '5' get +11), into a 64-bit key; used with level object lookup
- [ ] `00415cb0` FUN_00415cb0 · depth 0 · 55 lines · **untouched**
- [ ] `00415b00` inferred_FindCdAndSetup · depth 10 · 71 lines · **decompiled** — scans drives for label TESORO, checks SETUP.EXE
- [ ] `00415d90` FUN_00415d90 (vtable) · depth 13 · 25 lines · **untouched**
- [ ] `00415d20` FUN_00415d20 (vtable) · depth 14 · 27 lines · **untouched**
- [ ] `00415e30` FUN_00415e30 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `00415e90` FUN_00415e90 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `00415f10` FUN_00415f10 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `00415f90` FUN_00415f90 (vtable) · depth 14 · 32 lines · **untouched**
- [ ] `00416030` FUN_00416030 (vtable) · depth 14 · 42 lines · **untouched**
- [ ] `00416170` FUN_00416170 (vtable) · depth 14 · 30 lines · **untouched**

### Cluster 39: 00416300–004169a0 (0/12 ported)

- [ ] `00416300` FUN_00416300 (vtable) · depth 12 · 86 lines · **untouched**
- [ ] `004169a0` FUN_004169a0 (vtable) · depth 12 · 33 lines · **untouched**
- [ ] `00416220` FUN_00416220 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `00416280` FUN_00416280 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `00416540` FUN_00416540 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `00416590` FUN_00416590 (vtable) · depth 14 · 41 lines · **untouched**
- [ ] `00416680` FUN_00416680 (vtable) · depth 14 · 31 lines · **untouched**
- [ ] `00416700` FUN_00416700 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `00416750` FUN_00416750 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `004167a0` FUN_004167a0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `00416800` FUN_00416800 (vtable) · depth 14 · 43 lines · **untouched**
- [ ] `00416910` FUN_00416910 (vtable) · depth 14 · 31 lines · **untouched**

### Cluster 40: 00416b50–00417250 (0/9 ported)

- [ ] `00416b50` FUN_00416b50 (vtable) · depth 2 · 15 lines · **untouched**
- [ ] `00416b80` FUN_00416b80 (vtable) · depth 12 · 45 lines · **untouched**
- [ ] `00416c80` FUN_00416c80 (vtable) · depth 12 · 71 lines · **untouched**
- [ ] `00416a20` FUN_00416a20 (vtable) · depth 14 · 26 lines · **untouched**
- [ ] `00416ac0` FUN_00416ac0 (vtable) · depth 14 · 27 lines · **untouched**
- [ ] `00416e30` FUN_00416e30 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `00416ee0` FUN_00416ee0 (vtable) · depth 14 · 106 lines · **untouched**
- [ ] `00417180` FUN_00417180 (vtable) · depth 14 · 34 lines · **untouched**
- [ ] `00417250` FUN_00417250 (vtable) · depth 14 · 47 lines · **untouched**

### Cluster 41: 00417360–004178b0 (0/9 ported)

- [ ] `00417360` FUN_00417360 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `004174b0` FUN_004174b0 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `00417530` FUN_00417530 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `00417720` inferred_Obj_Kind19_Ctor · depth 12 · 63 lines · **decompiled** — 0xa4 bytes, vtable 0042bf90; builds a temporary Kind00 and copies it field by field into this
- [ ] `004175b0` inferred_Obj_Kind24_Ctor · depth 13 · 10 lines · **decompiled** — 0xa4 bytes, vtable 0042bf20, extends Kind09
- [ ] `004173e0` FUN_004173e0 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `004175f0` FUN_004175f0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `00417640` FUN_00417640 (vtable) · depth 14 · 44 lines · **untouched**
- [ ] `004178b0` FUN_004178b0 (vtable) · depth 14 · 37 lines · **untouched**

### Cluster 42: 00417b30–00417d00 (0/7 ported)

- [ ] `00417b30` inferred_Obj_RetTrue (vtable) · depth 0 · 7 lines · **decompiled** — MidBase slot 16: returns 1 (is interactive/enabled default)
- [ ] `004179b0` FUN_004179b0 (vtable) · depth 2 · 41 lines · **untouched**
- [ ] `00417c80` FUN_00417c80 (vtable) · depth 2 · 10 lines · **untouched**
- [ ] `00417ad0` inferred_Obj_Kind25_Ctor · depth 4 · 12 lines · **decompiled** — 0xa4 bytes, vtable 0042c000, extends MidBase
- [ ] `00417ca0` FUN_00417ca0 (vtable) · depth 13 · 21 lines · **untouched**
- [ ] `00417d00` FUN_00417d00 (vtable) · depth 13 · 31 lines · **untouched**
- [ ] `00417b40` FUN_00417b40 (vtable) · depth 15 · 49 lines · **untouched**

### Cluster 43: 00417dd0–00418560 (0/12 ported)

- [ ] `00417dd0` thunk_FUN_00414c40 (vtable) · depth 13 · 10 lines · **untouched**
- [ ] `004180b0` FUN_004180b0 (vtable) · depth 13 · 21 lines · **untouched**
- [ ] `00418110` FUN_00418110 (vtable) · depth 13 · 31 lines · **untouched**
- [ ] `00417de0` FUN_00417de0 (vtable) · depth 14 · 32 lines · **untouched**
- [ ] `00417e90` FUN_00417e90 (vtable) · depth 14 · 69 lines · **untouched**
- [ ] `00418050` FUN_00418050 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `004181e0` FUN_004181e0 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `004182c0` FUN_004182c0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `00418320` FUN_00418320 (vtable) · depth 14 · 33 lines · **untouched**
- [ ] `004183e0` FUN_004183e0 (vtable) · depth 14 · 40 lines · **untouched**
- [ ] `004184e0` FUN_004184e0 (vtable) · depth 14 · 25 lines · **untouched**
- [ ] `00418560` FUN_00418560 (vtable) · depth 14 · 27 lines · **untouched**

### Cluster 44: 004185c0–00418b80 (0/8 ported)

- [ ] `004185c0` FUN_004185c0 (vtable) · depth 14 · 34 lines · **untouched**
- [ ] `00418670` FUN_00418670 (vtable) · depth 14 · 54 lines · **untouched**
- [ ] `00418780` FUN_00418780 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `004187e0` FUN_004187e0 (vtable) · depth 14 · 25 lines · **untouched**
- [ ] `00418860` FUN_00418860 (vtable) · depth 14 · 50 lines · **untouched**
- [ ] `00418990` FUN_00418990 (vtable) · depth 14 · 38 lines · **untouched**
- [ ] `00418aa0` FUN_00418aa0 (vtable) · depth 14 · 44 lines · **untouched**
- [ ] `00418b80` FUN_00418b80 (vtable) · depth 14 · 34 lines · **untouched**

### Cluster 45: 00418d20–00419050 (0/7 ported)

- [ ] `00418d20` FUN_00418d20 · depth 0 · 76 lines · **untouched**
- [ ] `00419050` FUN_00419050 · depth 1 · 74 lines · **untouched**
- [ ] `00418c80` FUN_00418c80 (vtable) · depth 2 · 16 lines · **untouched**
- [ ] `00418c50` FUN_00418c50 (vtable) · depth 3 · 16 lines · **untouched**
- [ ] `00418e90` FUN_00418e90 · depth 3 · 97 lines · **untouched**
- [ ] `00418c30` FUN_00418c30 (vtable) · depth 4 · 10 lines · **untouched**
- [ ] `00418cb0` FUN_00418cb0 (vtable) · depth 14 · 30 lines · **untouched**

### Cluster 46: 004191c0–00419ac0 (0/12 ported)

- [ ] `004191c0` FUN_004191c0 · depth 2 · 46 lines · **untouched**
- [ ] `00419340` FUN_00419340 (vtable) · depth 4 · 23 lines · **untouched**
- [ ] `004192c0` inferred_Obj_Kind21_Ctor · depth 12 · 24 lines · **decompiled** — 0xa4 bytes, vtable 0042c070, extends MidBase; builds a phrase key from a base string plus a digit from the record
- [ ] `004193b0` FUN_004193b0 (vtable) · depth 12 · 69 lines · **untouched**
- [ ] `004197a0` FUN_004197a0 (vtable) · depth 12 · 51 lines · **untouched**
- [ ] `004198d0` FUN_004198d0 (vtable) · depth 12 · 37 lines · **untouched**
- [ ] `004199a0` FUN_004199a0 (vtable) · depth 12 · 21 lines · **untouched**
- [ ] `00419a10` FUN_00419a10 (vtable) · depth 12 · 31 lines · **untouched**
- [ ] `00419ac0` FUN_00419ac0 (vtable) · depth 12 · 20 lines · **untouched**
- [ ] `00419530` FUN_00419530 (vtable) · depth 14 · 43 lines · **untouched**
- [ ] `00419620` FUN_00419620 (vtable) · depth 14 · 29 lines · **untouched**
- [ ] `004196c0` FUN_004196c0 (vtable) · depth 14 · 30 lines · **untouched**

### Cluster 47: 00419b30–00419f10 (0/6 ported)

- [ ] `00419b30` FUN_00419b30 (vtable) · depth 12 · 17 lines · **untouched**
- [ ] `00419b90` FUN_00419b90 (vtable) · depth 12 · 68 lines · **untouched**
- [ ] `00419cf0` FUN_00419cf0 (vtable) · depth 12 · 16 lines · **untouched**
- [ ] `00419d40` FUN_00419d40 (vtable) · depth 12 · 16 lines · **untouched**
- [ ] `00419d90` FUN_00419d90 (vtable) · depth 12 · 74 lines · **untouched**
- [ ] `00419f10` FUN_00419f10 (vtable) · depth 12 · 57 lines · **untouched**

## Runtime, CRT and import thunks

259 functions (C runtime, SEH helpers, import stubs). They are not ported one by one; mark `replaced` when a ScummVM service covers them or `understood` when the game depends on their behaviour.

- [ ] `0041a070` DirectDrawCreate · import-thunk · **untouched**
- [ ] `0041a076` DirectInputCreateA · import-thunk · **untouched**
- [ ] `0041b3b0` Ordinal_1 · import-thunk · **untouched**
- [x] `0041b3c0` __chkstk · runtime · **replaced**
- [x] `0041b3f0` inferred_fseek · runtime · **replaced**
- [x] `0041b490` inferred_fgetpos · runtime · **replaced**
- [x] `0041b4c0` inferred_fread · runtime · **replaced**
- [x] `0041b600` inferred_fsetpos · runtime · **replaced**
- [x] `0041b620` inferred_fwrite · runtime · **replaced**
- [x] `0041b770` inferred__fsopen · runtime · **replaced**
- [x] `0041b7a0` inferred_fopen · runtime · **replaced**
- [x] `0041b7c0` inferred_fclose · runtime · **replaced**
- [x] `0041b840` _strncpy · runtime · **replaced**
- [x] `0041b940` inferred__strupr · runtime · **replaced**
- [x] `0041ba00` FUN_0041ba00 · runtime · **replaced**
- [x] `0041ba10` inferred_operator_new · runtime · **replaced**
- [x] `0041ba20` FUN_0041ba20 · runtime · **replaced**
- [x] `0041ba60` FUN_0041ba60 · runtime · **replaced**
- [x] `0041ba70` FUN_0041ba70 · runtime · **replaced**
- [x] `0041bad0` FUN_0041bad0 · runtime · **replaced**
- [x] `0041bb10` FUN_0041bb10 · runtime · **replaced**
- [x] `0041bb70` FUN_0041bb70 · runtime · **replaced**
- [x] `0041bba0` FUN_0041bba0 · runtime · **replaced**
- [x] `0041bc70` FUN_0041bc70 · runtime · **replaced**
- [x] `0041bd00` FUN_0041bd00 · runtime · **replaced**
- [x] `0041bd80` __global_unwind2 · runtime · **replaced**
- [x] `0041bda0` FUN_0041bda0 · runtime · **replaced**
- [x] `0041bdc2` __local_unwind2 · runtime · **replaced**
- [x] `0041be2a` __abnormal_termination · runtime · **replaced**
- [x] `0041be4d` __NLG_Notify1 · runtime · **replaced**
- [x] `0041be56` FUN_0041be56 · runtime · **replaced**
- [x] `0041be70` inferred_operator_delete · runtime · **replaced**
- [x] `0041be80` _strncmp · runtime · **replaced**
- [x] `0041bec0` __fpmath · runtime · **replaced**
- [x] `0041bee0` FUN_0041bee0 · runtime · **replaced**
- [x] `0041bf20` __ftol · runtime · **replaced**
- [x] `0041bf50` inferred_sscanf · runtime · **replaced**
- [x] `0041bfa0` inferred_ungetc · runtime · **replaced**
- [x] `0041c030` inferred__filbuf · runtime · **replaced**
- [x] `0041c120` inferred_fscanf · runtime · **replaced**
- [x] `0041c140` inferred_sprintf · runtime · **replaced**
- [x] `0041c1b0` inferred_ctime · runtime · **replaced**
- [x] `0041c1d0` inferred__findfirst · runtime · **replaced**
- [x] `0041c300` inferred__findclose · runtime · **replaced**
- [x] `0041c320` inferred___timet_from_ft · runtime · **replaced**
- [x] `0041c3b0` inferred__stat · runtime · **replaced**
- [x] `0041c3d0` inferred_WinMainCRTStartup · runtime · **replaced**
- [x] `0041c570` __amsg_exit · runtime · **replaced**
- [x] `0041c5a0` FUN_0041c5a0 · runtime · **replaced**
- [x] `0041c5d0` FUN_0041c5d0 · runtime · **replaced**
- [x] `0041c5f0` __exit · runtime · **replaced**
- [x] `0041c610` FUN_0041c610 · runtime · **replaced**
- [x] `0041c6c0` FUN_0041c6c0 · runtime · **replaced**
- [x] `0041c6e0` inferred__lseek · runtime · **replaced**
- [x] `0041c7a0` FUN_0041c7a0 · runtime · **replaced**
- [x] `0041c7f0` inferred__flush · runtime · **replaced**
- [x] `0041c860` FUN_0041c860 · runtime · **replaced**
- [x] `0041c870` FUN_0041c870 · runtime · **replaced**
- [x] `0041c8f0` inferred__ftell · runtime · **replaced**
- [x] `0041caa0` inferred__lseeki64_tell · runtime · **replaced**
- [x] `0041cc90` inferred__read · runtime · **replaced**
- [x] `0041cef0` FUN_0041cef0 · runtime · **replaced**
- [x] `0041cfa0` inferred__flsbuf · runtime · **replaced**
- [x] `0041d0d0` inferred__write · runtime · **replaced**
- [x] `0041d2f0` inferred__openfile · runtime · **replaced**
- [x] `0041d4c0` inferred__getstream · runtime · **replaced**
- [x] `0041d550` inferred_free · runtime · **replaced**
- [x] `0041d5a0` inferred__close · runtime · **replaced**
- [x] `0041d680` inferred__freebuf · runtime · **replaced**
- [x] `0041d6c0` inferred_malloc · runtime · **replaced**
- [x] `0041d6e0` inferred__nh_malloc · runtime · **replaced**
- [x] `0041d730` inferred__heap_alloc · runtime · **replaced**
- [x] `0041d770` FUN_0041d770 · runtime · **replaced**
- [x] `0041d990` FUN_0041d990 · runtime · **replaced**
- [x] `0041d9c0` FUN_0041d9c0 · runtime · **replaced**
- [x] `0041da90` FUN_0041da90 · runtime · **replaced**
- [x] `0041dd40` FUN_0041dd40 · runtime · **replaced**
- [x] `0041de10` FUN_0041de10 · runtime · **replaced**
- [x] `0041ded0` FUN_0041ded0 · runtime · **replaced**
- [x] `0041def0` FUN_0041def0 · runtime · **replaced**
- [x] `0041df80` FUN_0041df80 · runtime · **replaced**
- [x] `0041e040` FUN_0041e040 · runtime · **replaced**
- [x] `0041e061` FUN_0041e061 · runtime · **replaced**
- [x] `0041e06a` FUN_0041e06a · runtime · **replaced**
- [x] `0041e0d0` FUN_0041e0d0 · runtime · **replaced**
- [x] `0041e100` FUN_0041e100 · runtime · **replaced**
- [x] `0041e310` FUN_0041e310 · runtime · **replaced**
- [x] `0041e390` FUN_0041e390 · runtime · **replaced**
- [x] `0041e3c0` __CallSettingFrame@12 · runtime · **replaced**
- [x] `0041e410` FUN_0041e410 · runtime · **replaced**
- [x] `0041e47e` _abort · runtime · **replaced**
- [x] `0041e4a0` FUN_0041e4a0 · runtime · **replaced**
- [x] `0041e50e` FUN_0041e50e · runtime · **replaced**
- [x] `0041e530` __setdefaultprecision · runtime · **replaced**
- [x] `0041e550` FUN_0041e550 · runtime · **replaced**
- [x] `0041e5a0` FUN_0041e5a0 · runtime · **replaced**
- [x] `0041e5d0` FUN_0041e5d0 · runtime · **replaced**
- [x] `0041e630` FUN_0041e630 · runtime · **replaced**
- [x] `0041e6a0` FUN_0041e6a0 · runtime · **replaced**
- [x] `0041e6c0` FUN_0041e6c0 · runtime · **replaced**
- [x] `0041e720` FUN_0041e720 · runtime · **replaced**
- [x] `0041e860` FUN_0041e860 · runtime · **replaced**
- [x] `0041e960` FUN_0041e960 · runtime · **replaced**
- [x] `0041ea10` FUN_0041ea10 · runtime · **replaced**
- [x] `0041ea40` FUN_0041ea40 · runtime · **replaced**
- [x] `0041ea70` FUN_0041ea70 · runtime · **replaced**
- [x] `0041eae0` FUN_0041eae0 · runtime · **replaced**
- [x] `0041eb10` inferred__input · runtime · **replaced**
- [x] `0041f850` FUN_0041f850 · runtime · **replaced**
- [x] `0041f890` FUN_0041f890 · runtime · **replaced**
- [x] `0041f8c0` FUN_0041f8c0 · runtime · **replaced**
- [x] `0041f8e0` FUN_0041f8e0 · runtime · **replaced**
- [x] `0041f930` inferred__getbuf · runtime · **replaced**
- [x] `0041f990` inferred__ioinit · runtime · **replaced**
- [x] `0041fb90` inferred__output · runtime · **replaced**
- [x] `00420520` FUN_00420520 · runtime · **replaced**
- [x] `00420570` FUN_00420570 · runtime · **replaced**
- [x] `004205b0` FUN_004205b0 · runtime · **replaced**
- [x] `004205f0` FUN_004205f0 · runtime · **replaced**
- [x] `00420610` FUN_00420610 · runtime · **replaced**
- [x] `00420630` FUN_00420630 · runtime · **replaced**
- [x] `00420650` FUN_00420650 · runtime · **replaced**
- [x] `00420740` FUN_00420740 · runtime · **replaced**
- [x] `00420780` FUN_00420780 · runtime · **replaced**
- [x] `00420970` FUN_00420970 · runtime · **replaced**
- [x] `00420a60` FUN_00420a60 · runtime · **replaced**
- [x] `00420c50` FUN_00420c50 · runtime · **replaced**
- [x] `00420cc0` FUN_00420cc0 · runtime · **replaced**
- [x] `00420e00` FUN_00420e00 · runtime · **replaced**
- [x] `00420e50` FUN_00420e50 · runtime · **replaced**
- [x] `00420e70` FUN_00420e70 · runtime · **replaced**
- [x] `00420eb0` FUN_00420eb0 · runtime · **replaced**
- [x] `00420fa0` FUN_00420fa0 · runtime · **replaced**
- [x] `00421040` FUN_00421040 · runtime · **replaced**
- [x] `00421250` FUN_00421250 · runtime · **replaced**
- [x] `004213b0` FUN_004213b0 · runtime · **replaced**
- [x] `004215a0` FUN_004215a0 · runtime · **replaced**
- [x] `004215f0` FUN_004215f0 · runtime · **replaced**
- [x] `00421650` FUN_00421650 · runtime · **replaced**
- [x] `00421680` FUN_00421680 · runtime · **replaced**
- [x] `00421690` FUN_00421690 · runtime · **replaced**
- [x] `004216d0` FUN_004216d0 · runtime · **replaced**
- [x] `00421795` __seh_longjmp_unwind@4 · runtime · **replaced**
- [x] `004217b0` FUN_004217b0 · runtime · **replaced**
- [x] `004217f0` FUN_004217f0 · runtime · **replaced**
- [x] `004219d0` FUN_004219d0 · runtime · **replaced**
- [x] `00421a40` FUN_00421a40 · runtime · **replaced**
- [x] `00421b00` FUN_00421b00 · runtime · **replaced**
- [x] `00421bb0` FUN_00421bb0 · runtime · **replaced**
- [x] `00421c50` FUN_00421c50 · runtime · **replaced**
- [x] `00421ca0` FUN_00421ca0 · runtime · **replaced**
- [x] `00421d00` FUN_00421d00 · runtime · **replaced**
- [x] `00421dc0` FUN_00421dc0 · runtime · **replaced**
- [x] `00421de0` FUN_00421de0 · runtime · **replaced**
- [x] `00421ed0` FUN_00421ed0 · runtime · **replaced**
- [x] `00421f00` FUN_00421f00 · runtime · **replaced**
- [x] `004222c0` FUN_004222c0 · runtime · **replaced**
- [x] `00422430` FUN_00422430 · runtime · **replaced**
- [x] `00422490` FUN_00422490 · runtime · **replaced**
- [x] `00422560` FUN_00422560 · runtime · **replaced**
- [x] `004225c0` FUN_004225c0 · runtime · **replaced**
- [x] `00422620` FUN_00422620 · runtime · **replaced**
- [x] `00422860` FUN_00422860 · runtime · **replaced**
- [x] `004229e0` FUN_004229e0 · runtime · **replaced**
- [x] `00422ab0` FUN_00422ab0 · runtime · **replaced**
- [x] `00422ad0` FUN_00422ad0 · runtime · **replaced**
- [x] `00422b30` FUN_00422b30 · runtime · **replaced**
- [x] `00422b50` FUN_00422b50 · runtime · **replaced**
- [x] `00422b60` FUN_00422b60 · runtime · **replaced**
- [x] `00422b80` FUN_00422b80 · runtime · **replaced**
- [x] `00422ba0` FUN_00422ba0 · runtime · **replaced**
- [x] `00422bc0` FUN_00422bc0 · runtime · **replaced**
- [x] `00422f00` _abort · runtime · **replaced**
- [x] `00422f20` FUN_00422f20 · runtime · **replaced**
- [x] `00422f60` FUN_00422f60 · runtime · **replaced**
- [x] `00422f80` FUN_00422f80 · runtime · **replaced**
- [x] `00423020` FUN_00423020 · runtime · **replaced**
- [x] `004230b0` FUN_004230b0 · runtime · **replaced**
- [x] `00423150` FUN_00423150 · runtime · **replaced**
- [x] `00423250` FUN_00423250 · runtime · **replaced**
- [x] `004232c0` FUN_004232c0 · runtime · **replaced**
- [x] `00423330` FUN_00423330 · runtime · **replaced**
- [x] `004233d0` FUN_004233d0 · runtime · **replaced**
- [x] `004233f0` FUN_004233f0 · runtime · **replaced**
- [x] `00423400` FUN_00423400 · runtime · **replaced**
- [x] `00423420` FUN_00423420 · runtime · **replaced**
- [x] `004234e0` FUN_004234e0 · runtime · **replaced**
- [x] `004236b0` FUN_004236b0 · runtime · **replaced**
- [x] `004236d0` FUN_004236d0 · runtime · **replaced**
- [x] `004236f0` FUN_004236f0 · runtime · **replaced**
- [x] `00423730` FUN_00423730 · runtime · **replaced**
- [x] `00423770` FUN_00423770 · runtime · **replaced**
- [x] `00423810` FUN_00423810 · runtime · **replaced**
- [x] `00423890` FUN_00423890 · runtime · **replaced**
- [x] `00423950` FUN_00423950 · runtime · **replaced**
- [x] `00423960` FUN_00423960 · runtime · **replaced**
- [x] `00423a60` FUN_00423a60 · runtime · **replaced**
- [x] `00423a90` __allmul · runtime · **replaced**
- [x] `00423ad0` __allshl · runtime · **replaced**
- [x] `00423af0` FUN_00423af0 · runtime · **replaced**
- [x] `00423b70` __aulldiv · runtime · **replaced**
- [x] `00423be0` __aullrem · runtime · **replaced**
- [x] `00423c60` FUN_00423c60 · runtime · **replaced**
- [x] `00423c80` FUN_00423c80 · runtime · **replaced**
- [x] `00423f30` FUN_00423f30 · runtime · **replaced**
- [x] `004241a0` FUN_004241a0 · runtime · **replaced**
- [x] `00424340` FUN_00424340 · runtime · **replaced**
- [x] `004244a0` FUN_004244a0 · runtime · **replaced**
- [x] `004244f0` FUN_004244f0 · runtime · **replaced**
- [x] `004245a0` FUN_004245a0 · runtime · **replaced**
- [x] `00424620` FUN_00424620 · runtime · **replaced**
- [x] `004248a0` FUN_004248a0 · runtime · **replaced**
- [x] `00424b60` FUN_00424b60 · runtime · **replaced**
- [x] `00424b80` FUN_00424b80 · runtime · **replaced**
- [x] `00424c10` FUN_00424c10 · runtime · **replaced**
- [x] `00424cb0` FUN_00424cb0 · runtime · **replaced**
- [x] `00424d20` FUN_00424d20 · runtime · **replaced**
- [x] `00424ea0` FUN_00424ea0 · runtime · **replaced**
- [x] `00425020` FUN_00425020 · runtime · **replaced**
- [x] `00425070` FUN_00425070 · runtime · **replaced**
- [x] `004251a0` FUN_004251a0 · runtime · **replaced**
- [x] `004251d0` FUN_004251d0 · runtime · **replaced**
- [x] `00425240` FUN_00425240 · runtime · **replaced**
- [x] `00425270` FUN_00425270 · runtime · **replaced**
- [x] `004252a0` FUN_004252a0 · runtime · **replaced**
- [x] `004253a0` FUN_004253a0 · runtime · **replaced**
- [x] `00425b30` FUN_00425b30 · runtime · **replaced**
- [x] `00425ec0` FUN_00425ec0 · runtime · **replaced**
- [x] `00425f60` FUN_00425f60 · runtime · **replaced**
- [x] `00426150` FUN_00426150 · runtime · **replaced**
- [x] `00426190` FUN_00426190 · runtime · **replaced**
- [x] `00426230` _strchr · runtime · **replaced**
- [x] `004262f0` _strrchr · runtime · **replaced**
- [x] `00426320` FUN_00426320 · runtime · **replaced**
- [x] `004263a0` FUN_004263a0 · runtime · **replaced**
- [x] `00426660` FUN_00426660 · runtime · **replaced**
- [x] `004266f0` FUN_004266f0 · runtime · **replaced**
- [x] `00426730` FUN_00426730 · runtime · **replaced**
- [x] `004267b0` FUN_004267b0 · runtime · **replaced**
- [x] `00426a80` FUN_00426a80 · runtime · **replaced**
- [x] `00426c90` FUN_00426c90 · runtime · **replaced**
- [x] `00426d10` FUN_00426d10 · runtime · **replaced**
- [x] `00426d80` FUN_00426d80 · runtime · **replaced**
- [x] `00426f20` FUN_00426f20 · runtime · **replaced**
- [x] `00426f70` RtlUnwind · runtime · **replaced**
- [x] `00426f80` inferred_ehfunclet_00426f80 · runtime · **replaced**
- [x] `00426fa0` inferred_ehfunclet_00426fa0 · runtime · **replaced**
- [x] `00426fc0` inferred_ehfunclet_00426fc0 · runtime · **replaced**
- [x] `00426fce` inferred_ehfunclet_00426fce · runtime · **replaced**
- [x] `00426ff0` inferred_ehfunclet_00426ff0 · runtime · **replaced**
- [x] `00427010` inferred_ehfunclet_00427010 · runtime · **replaced**
- [x] `0042701b` inferred_ehfunclet_0042701b · runtime · **replaced**
- [x] `00427026` inferred_ehfunclet_00427026 · runtime · **replaced**
- [x] `00427031` inferred_ehfunclet_00427031 · runtime · **replaced**
- [x] `0042703c` inferred_ehfunclet_0042703c · runtime · **replaced**
- [x] `00427047` inferred_ehfunclet_00427047 · runtime · **replaced**
- [x] `00427052` inferred_ehfunclet_00427052 · runtime · **replaced**
- [x] `00427070` inferred_ehfunclet_00427070 · runtime · **replaced**
- [x] `00427090` inferred_ehfunclet_00427090 · runtime · **replaced**
