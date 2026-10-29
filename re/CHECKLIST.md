# Function checklist

Generated from `ledger.json` by `tools/make_checklist.py`. Update with `tools/mark.py` (see its docstring).
`[x]` means a C++ port exists (status ported or later). The status after each entry says how far it got and how it was checked: decompiled < understood < ported < emu-compared < original-compared.

## Overall (game code: 447 functions)

| Stage | Count | Share |
|---|---|---|
| untouched or later | 447 | 100% |
| decompiled or later | 235 | 52% |
| understood or later | 37 | 8% |
| ported or later | 27 | 6% |
| emu-compared or later | 19 | 4% |
| original-compared or later | 0 | 0% |
| replaced by a ScummVM service | 0 | |

## Game code by cluster

### Cluster 1: 00401000–00401740 (7/7 ported)

- [x] `00401000` inferred_ReturnThis · depth 0 · 7 lines · **ported** — __fastcall returns ecx; used as a vtable slot of the object built by 00401aa0 | AnimPlayer value copy / default ctor
- [x] `00401580` inferred_Copy64Bytes · depth 0 · 14 lines · **ported** — thiscall copy of 16 dwords from param into this | AnimPlayer value copy / default ctor
- [x] `00401010` inferred_AnimBank_ScaleBrightness · depth 1 · 73 lines · **emu-compared** — builds channel scale tables from a 16-bit factor (x/256) and rewrites every 16-bit image's pixels; flag at (DAT_00442108+0x67c) picks 5-5-5 vs 5-6-5 handling | animation vectors 29 Oct
- [x] `00401740` inferred_AnimPlayer_SetRange · depth 1 · 70 lines · **emu-compared** — thiscall(first, last_or_-1, loop_flag, sum_flag): sets play range in sequence table, resets counters, stamps start time from game clock 00413a10 | animation vectors 29 Oct
- [x] `004015a0` inferred_AnimBank_LoadImage · depth 8 · 82 lines · **emu-compared** — reads size, allocates from pool DAT_00442100, reads blob, relocates internal pointers by base; converts pixels 5-6-5 to 5-5-5 when flag set; image type 2 (header +0xc) entries are rewritten as (v&0x1f)<<2, meaning not yet known | animation vectors 29 Oct
- [x] `00401160` inferred_AnimBank_Load · depth 9 · 163 lines · **emu-compared** — thiscall(entry, pool, mode): opens archive entry+4, reads 0x18 header (images +0x20, sequences +0x22), 0x24-byte sequence records at +0x1c, image pointer array at +0, file offsets at +4; mode 2 defers image loading | animation vectors 29 Oct
- [x] `00401510` inferred_AnimBank_LoadDeferredImages · depth 9 · 26 lines · **emu-compared** — mode 2 only: seeks to each stored offset (+4 array) and loads the image with 004015a0 | animation vectors 29 Oct

### Cluster 2: 00401bc0–00401d60 (3/12 ported)

- [ ] `00401bc0` inferred_Object_BaseCtor · depth 0 · 12 lines · **decompiled** — base game object: vtable 00428000, flags +0x68 |= 6, +0x5e = 5, byte +0x5c = 0
- [ ] `00401c20` inferred_Noop (vtable) · depth 0 · 7 lines · **decompiled** — empty function; fills vtable slots 0, 1 and 7 of the base classes (also used as the clock destructor) | empty function; fills vtable slots 0, 1 and 7 of the base classes (and was the clock destructor)
- [ ] `00401c50` inferred_Obj_UpdateBounds (vtable) · depth 0 · 30 lines · **decompiled** — base slot 3: recomputes bounds rect (+0x10..+0x1c) and screen position (+0x20..+0x2c) from the current frame record (0x24-byte entries); calls slot 2 in between
- [ ] `00401d20` inferred_Obj_Stub_Void_1Arg (vtable) · depth 0 · 7 lines · **decompiled** — base slot 6 default (draw with one argument)
- [ ] `00401d60` inferred_Obj_SetActive (vtable) · depth 0 · 8 lines · **decompiled** — base slot 8: sets flag bit 1 (+0x68) from argument
- [x] `00401860` inferred_AnimPlayer_Advance · depth 1 · 86 lines · **emu-compared** — advances the animation: reads the game clock, steps frames by their durations, wraps or stops at the range end (86 lines, detail not yet contracted) | name from call sites and a skim; body not yet read | player update vectors 29 Oct
- [x] `00401a50` inferred_AnimPlayer_Pause · depth 1 · 12 lines · **emu-compared** — flags: clear bit0, set bit1; +0x10 = clock now | player update vectors 29 Oct
- [x] `00401a70` inferred_AnimPlayer_Resume · depth 1 · 15 lines · **emu-compared** — adds the paused duration to +8, clears +0x10, flags: clear bit1, set bit0 | player update vectors 29 Oct
- [ ] `00401bf0` inferred_Object_AttachToLevel · depth 1 · 16 lines · **decompiled** — sets owner +0x40, inserts into the room list if flag 0x100 set, then calls vtable slot 0
- [ ] `00401d30` inferred_Obj_SetPosition · depth 1 · 14 lines · **decompiled** — (x, y, flag) sets previous position (+0x54/+0x58), and current (+0x4c/+0x50) when flag set; notifies the level
- [ ] `00401aa0` inferred_Obj_Kind00_Ctor · depth 2 · 63 lines · **decompiled** — 0x6c bytes, vtable 00428000. Shared base of most kinds: position +0x4c/+0x50 from record, room +0x3c, sort value +0x5e = record short + room short + 5, 64-bit name key +0x44, anim bank pointers (+0x34 array, +0x33 count, current +0x38)
- [ ] `00401c30` inferred_Obj_AnimTick (vtable) · depth 2 · 11 lines · **decompiled** — base slot 2: if object has animation (flag bit 4) advance AnimPlayer via 00401860

### Cluster 3: 00401d80–00401fa0 (0/8 ported)

- [ ] `00401d80` inferred_Obj_SetVisible · depth 0 · 8 lines · **decompiled** — sets flag bit 2 (+0x68) from argument
- [ ] `00401da0` inferred_Obj_StashAnimState · depth 0 · 9 lines · **decompiled** — copies bank index (+0x32) to +4 and the player's flags (+0x18) to +8; restored by Obj_ApplyPendingAnim
- [ ] `00401dc0` inferred_Obj_ApplyPendingAnim · depth 0 · 13 lines · **decompiled** — selects bank index +4 and stores +8 as the AnimPlayer start value
- [ ] `00401f90` inferred_Obj_Stub_RetFalse_1Arg (vtable) · depth 0 · 7 lines · **decompiled** — base slot 11 default (ret 4)
- [ ] `00401fa0` inferred_Obj_Stub_RetZero_2Args (vtable) · depth 0 · 7 lines · **decompiled** — base slot 12 default (ret 8)
- [ ] `00401de0` inferred_Object_SelectAnimBank · depth 2 · 15 lines · **decompiled** — picks bank index (byte +0x32), sets +0x38, starts AnimPlayer_SetRange
- [ ] `00401e20` inferred_Obj_LoadState (vtable) · depth 8 · 22 lines · **decompiled** — base slot 9: reads from a stream: current bank index (+0x32), per-frame fields of the current sequence (+0x24, +0x26, +0x30, +0x18) and an optional blob of +0x64 bytes at +0x60. Save-game deserialisation
- [ ] `00401ee0` inferred_Obj_SaveState (vtable) · depth 8 · 18 lines · **decompiled** — base slot 10: mirror of LoadState using fwrite. Save-game serialisation

### Cluster 4: 00401fb0–00402230 (0/7 ported)

- [ ] `00401fb0` inferred_Obj_Stub_RetZero (vtable) · depth 0 · 7 lines · **decompiled** — default for update/draw-style slots: returns 0
- [ ] `00402110` inferred_Camera_Ctor · depth 0 · 9 lines · **decompiled** — 0x40-byte object at level +0x80; zeroes +0 and +0x10
- [ ] `00402120` inferred_Camera_SetPositionClamped · depth 0 · 35 lines · **decompiled** — stores x,y (+0x2c,+0x30) clamped to the bounds rect (+0x1c..+0x28) with the 640x480 half extents 0x140/0xf0; returns whether it was inside
- [ ] `00402200` inferred_Camera_SetBounds · depth 0 · 12 lines · **decompiled** — stores 4 ints at +0x1c..+0x28; Level_Load passes (-320, 239, 319, -240)
- [ ] `00402230` inferred_Camera_SetValue0 · depth 0 · 8 lines · **decompiled** — stores one dword at +0
- [ ] `00401fc0` inferred_Camera_UpdateToward · depth 1 · 57 lines · **untouched** — (camera, target) moves the camera toward a target using the game clock; ends in Camera_SetPositionClamped | name from call sites and a skim; body not yet read
- [ ] `004021a0` inferred_Camera_MoveTo · depth 1 · 22 lines · **decompiled** — duration 0: set position now (00402120); otherwise stores start/target and a start time from the game clock

### Cluster 5: 004023f0–004028c0 (0/7 ported)

- [ ] `004023f0` inferred_Cursor_SetText · depth 0 · 43 lines · **decompiled** — copies a C string into the cursor's hover-text buffer (+0x78), or clears it; optional ushort at +0x118
- [ ] `004025b0` inferred_Cursor_PageInterfaceRight · depth 1 · 49 lines · **decompiled** — shifts the interface level's camera and its objects by +0x280 (one screen) when the pointer reaches the right edge
- [ ] `004028c0` inferred_Interface_PageLeft · depth 1 · 51 lines · **decompiled** — shifts the interface level camera and its objects by -0x280 (one screen)
- [ ] `00402240` inferred_Cursor_Ctor · depth 2 · 42 lines · **decompiled** — 0x130-byte object, vtable 00428038, base 0040e5a0; creates candidate lists (capacity 0x1e) at +0x6c/+0x70 and a shared list 00442060
- [ ] `00402440` inferred_C00428038_OnAttach (vtable) · depth 2 · 74 lines · **untouched** — unread; first seen in vtable 00428038 slot 0
- [ ] `004026d0` inferred_Cursor_CaptureThumbnail · depth 3 · 84 lines · **decompiled** — renders the scene without the cursor into the display buffer and copies a 213x160 sample (every third pixel of every third row) to +0x128: a save-game thumbnail (hypothesis from the sampling pattern)
- [ ] `00402300` inferred_Obj_Kind10_Ctor · depth 4 · 46 lines · **decompiled** — 0x130 bytes, vtable 00428038, extends Kind03; creates two 30-element lists, sets the mouse object bounds (DAT_0044206c +0x20 = -320 ...), so probably the pointer/cursor object

### Cluster 6: 00402e50–00403300 (4/12 ported)

- [ ] `00402e50` inferred_Cursor_IsFlag10 (vtable) · depth 0 · 7 lines · **decompiled** — vtable slot 11 (ret 4): IsHitter(other): returns flag bit 10 (0x400) of this, ignoring the argument. Base default returns false. Level_CollisionPass asks A.slot11(B) then B.slot11(A) to decide which object is the hitter
- [ ] `00402d80` inferred_C00428038_Draw (vtable) · depth 1 · 24 lines · **untouched** — unread; first seen in vtable 00428038 slot 6
- [ ] `00402e60` inferred_Cursor_AddCandidate (vtable) · depth 1 · 8 lines · **decompiled** — vtable slot 12 (ret 8: two stack arguments, the decompiler shows one): OnTouched(other, extra). Cursor override appends the touched object (first argument) to candidate list +0x70 and returns 1. Called by Level_CollisionPass flush
- [ ] `00402e80` inferred_Cursor_ProcessCandidates · depth 1 · 127 lines · **decompiled** — per frame: for each registered candidate, with left button (mouse buttons byte & 8) calls candidate slot 14 with the cursor's current animation index (the verb), with right button (& 0x10) slot 15, otherwise slot 17 (show hover text); candidates that stopped being under the pointer get slot 18 (clear text) and are dropped
- [ ] `004031a0` inferred_C00428038_SetActive (vtable) · depth 1 · 38 lines · **untouched** — unread; first seen in vtable 00428038 slot 8
- [x] `00403220` inferred_Archive_Ctor · depth 1 · 11 lines · **ported** → `engines/tesoro/core/archive.cpp` — allocates the index buffer (0xd7a0 bytes, from arena 0) into global 00442068
- [ ] `004029a0` inferred_Interface_ScrollOut · depth 2 · 14 lines · **decompiled** — moves the interface camera +0x285 and runs the display effect (4, 100)
- [ ] `004030e0` inferred_C00428038_Update2 (vtable) · depth 2 · 35 lines · **untouched** — unread; first seen in vtable 00428038 slot 5
- [x] `00403300` inferred_Archive_Tell · depth 4 · 10 lines · **ported** → `engines/tesoro/core/archive.cpp` — current position of the archive stream (low dword of fgetpos)
- [x] `004032e0` inferred_Archive_SeekEntry · depth 5 · 8 lines · **ported** → `engines/tesoro/core/archive.cpp` — fseek the archive handle to offset, returns the FILE*
- [x] `00403250` inferred_Archive_Open · depth 10 · 28 lines · **ported** → `engines/tesoro/core/archive.cpp` — thiscall(dat_path, tre_path): reads index from the .TRE (00403380), keeps the .DAT handle at +4
- [ ] `004029f0` inferred_Cursor_Update (vtable) · depth 12 · 156 lines · **decompiled** — vtable slot 4: reads the mouse position and buttons and the keyboard state array, handles the interface strip scrolling (shifts by 0x280 = one screen width), then positions the cursor object and its +0x74 companion at the mouse position plus the interface camera offset, and flags the held item

### Cluster 7: 00403500–00403980 (4/12 ported)

- [x] `00403500` inferred_Archive_FindEntry · depth 0 · 59 lines · **emu-compared** → `engines/tesoro/core/archiveindex.cpp::ArchiveIndex::find` — binary search by name in sorted 0x18-byte index records at 00442068; returns data offset (+0x14) or fails
- [ ] `004036c0` inferred_InputDevice_IsReady · depth 0 · 7 lines · **decompiled** — device ready check | true when the device pointer (+4) and the DirectInput object exist
- [ ] `004036e0` inferred_InputDevice_SetTimings · depth 0 · 9 lines · **decompiled** — +0xc and +0x10 (200 and 20000 are the values given at start)
- [ ] `00403700` inferred_Keyboard_UpdateKeyStates · depth 0 · 50 lines · **decompiled** — (new states, count) state machine per key (0,1,2,4,8,0x10,0x20) using GetTickCount and the two timing values (+0xc, +0x10): press, hold, release and repeat
- [ ] `004037c0` inferred_InputDevice_Dtor · depth 0 · 23 lines · **decompiled** — releases the DirectInput device and the shared DirectInput object when the count reaches zero
- [ ] `004035b0` inferred_InputDevice_Ctor · depth 1 · 42 lines · **decompiled** — base-class constructor of the DirectInput devices (stores vtable 0042806c, creates the shared DirectInput object once and the device); message "DIRECTINPUT: error al crear el objeto" on failure; reference-counted in 0044207c. Called from Keyboard_Ctor and Mouse ctor
- [ ] `004038f0` inferred_C00428070_Slot00 (vtable) · depth 1 · 40 lines · **untouched** — unread; first seen in vtable 00428070 slot 0 (non-object class)
- [x] `00403320` inferred_Archive_SeekSavePos · depth 5 · 11 lines · **ported** → `engines/tesoro/core/archive.cpp` — (offset) returns the previous position and seeks to offset
- [ ] `00403820` inferred_Keyboard_Ctor · depth 5 · 44 lines · **decompiled** — 0x38-byte keyboard device, vtable 00428070, 16-byte state buffer (+0x1c), default timings 200 and 20000
- [ ] `00403980` inferred_MouseCtor · depth 5 · 46 lines · **decompiled** — DirectInput mouse, range 640x480, starts at (320,240)
- [x] `00403360` inferred_Archive_Close · depth 6 · 10 lines · **ported** → `engines/tesoro/core/archive.cpp`
- [x] `00403380` inferred_Archive_ReadIndex · depth 8 · 109 lines · **emu-compared** → `engines/tesoro/core/archiveindex.cpp::ArchiveIndex::load` — reads 0x1c-byte index records from the .TRE keeping those with flag 4, copies the uppercased name (0x13 bytes) and offset+4 into 0x18-byte entries, then sorts them by name (selection sort); entry count in +0

### Cluster 8: 00403a70–004040d0 (0/8 ported)

- [ ] `00403a70` inferred_MouseCenter · depth 0 · 9 lines · **decompiled**
- [ ] `00403b60` inferred_CdAudioInit · depth 0 · 52 lines · **decompiled** — MCI open cdaudio; track count at +0x1bc, -1 if failed
- [ ] `00403cb0` inferred_CdAudio_PlayTrack · depth 0 · 51 lines · **decompiled** — (track, flag) MCI: stop, set time format to tracks, play from the track (to the next track or the end)
- [ ] `004040b0` inferred_Sound_IsReady · depth 0 · 7 lines · **decompiled** — returns obj[0]!=0 && obj[1]!=0 | true when the first two fields are set
- [ ] `004040d0` inferred_Sound_FindById · depth 0 · 22 lines · **decompiled** — finds the active sound slot whose id (+0x2c) matches
- [ ] `00403aa0` inferred_C00428074_Slot00 (vtable) · depth 1 · 57 lines · **untouched** — unread; first seen in vtable 00428074 slot 0 (non-object class)
- [ ] `00403dc0` inferred_CdAudio_OnNotify · depth 1 · 14 lines · **decompiled** — on MM_MCINOTIFY success restarts the saved track (+0x29)
- [ ] `00403e00` inferred_DirectSoundInit · depth 5 · 138 lines · **decompiled** — only creates DirectSound if CD track count >= 0

### Cluster 9: 004043b0–00404630 (0/7 ported)

- [ ] `004043b0` inferred_Sound_Stop · depth 0 · 11 lines · **decompiled** — stops the slot's buffer (slot 0x48/0x34 calls) and clears its id
- [ ] `00404630` inferred_DlgEntry_CopyCtorA · depth 2 · 69 lines · **decompiled** — copy constructor of a script-block entry (vtable 00428078)
- [ ] `00404260` inferred_Sound_Dtor · depth 5 · 22 lines · **untouched** —  | name from call sites and a skim; body not yet read
- [ ] `004042c0` inferred_Sound_LoadWavData · depth 8 · 54 lines · **untouched** — reads the sound data chunk from the archive into the slot | name from call sites and a skim; body not yet read
- [ ] `004043e0` inferred_Sound_StreamRefill · depth 8 · 106 lines · **untouched** — refills a streaming DirectSound buffer from the archive; returns whether the sound is still active | name from call sites and a skim; body not yet read
- [ ] `00404110` inferred_Sound_PlayWav · depth 9 · 46 lines · **decompiled** — (archive offset, ...) reads a RIFF/WAVE header from the archive, fills the next sound slot (DirectSoundBuffer), sets volume and pan, starts playing and returns a new id
- [ ] `00404210` inferred_Sound_Update · depth 9 · 28 lines · **decompiled** — per-frame: refills streaming buffers (Sound_StreamRefill) and retires finished slots

### Cluster 10: 004060b0–00406140 (0/9 ported)

- [ ] `004060b0` inferred_DialogueTable_Ctor · depth 0 · 8 lines · **decompiled** — 0x2b0-byte table; entry count at +0x2ac set to 0
- [ ] `004060c0` inferred_DialogueTable_FindByKey · depth 0 · 22 lines · **decompiled** — linear search of the table's script entries (pointer array at +0x54) for a 64-bit name key at script +0x44/+0x48
- [ ] `00405630` inferred_DlgScript_Link · depth 1 · 108 lines · **decompiled** — post-load pass over the steps resolving actor keys and jump targets (types 4-7)
- [ ] `00406110` inferred_DialogueTable_Finalize · depth 2 · 19 lines · **decompiled** — calls 00405630 for each entry
- [ ] `00404810` inferred_DlgScript_Load · depth 12 · 691 lines · **decompiled** — constructor and text parser (691 lines): reads one script block from the stream with a sscanf-based command grammar, compiles it to 24-byte steps (+0xbc, count +0xc0) plus actor key table (+0xc4, 12-byte entries) and variable byte array (+0xd0); reports "ERROR AL LEER COMANDO" on a bad line | parser read: dispatches each line on its first character (space, ., D, _, b, i, j, m, p, r, s, v); see re/SCRIPT_LANGUAGE.md
- [ ] `00405a60` inferred_DlgScript_RunSteps · depth 13 · 228 lines · **decompiled** — executes non-blocking steps from the program counter until a blocking one: say, goto, label/loop, conditional jump on a script variable, event call, actor command (animate, walk, face, activate, show), variable assignment, send message, movie, WAV, CD track
- [ ] `00405fd0` inferred_DlgScript_Start · depth 13 · 44 lines · **decompiled** — (event, key lo, key hi, variables, level): resolves actors in the level, selects the entry point matching the event key, hides the cursor, inserts the script into the main list at priority 0x1e and runs it; returns 3
- [ ] `00405850` inferred_DlgScript_Update (vtable) · depth 14 · 70 lines · **decompiled** — per-frame step of a scripted action list (24-byte steps at +0xbc, current step +0xcc): waits (type 1 until the speaker finishes plus 200 ms, type 2 timed), choice menus (types 5, 6: asks the player object to say the options), cursor show/hide (type 8), type 9 wait for an animation; advances or ends the list (returns 2 or 3) | per-frame step: handles the blocking step types (1 wait for speech +200 ms, 2 timed wait, 5/6 choice menu, 8 cursor, 9 animation wait) and, when the program counter moved, runs RunSteps; returns 2 when finished
- [ ] `00406140` inferred_DlgScript_ResumePlayer (vtable) · depth 14 · 12 lines · **decompiled** — removes a helper object from the main list, clears Jim's walking flag, sets facing and advances the program counter

### Cluster 11: 004067a0–00407000 (0/8 ported)

- [ ] `004067a0` inferred_DlgEntry_CopyCtorB · depth 0 · 67 lines · **decompiled** — field-by-field copy of an entry (used by DialogueTable_Load for kind 1)
- [ ] `00406c50` inferred_Obj_Kind13_Update (vtable) · depth 1 · 44 lines · **decompiled** — slot 4 of kind 13: same pending-click dispatch as Interactive_Update, with an extra item-use path when no verb handler applies (calls the hovered object's slot 27)
- [ ] `004069e0` inferred_Obj_Kind01_OnAttach (vtable) · depth 2 · 35 lines · **decompiled** — vtable slot 0 of kind 1 (run by Object_AttachToLevel): resolves the walk area object by the name at +0x6c in the level's object list (+0x78), sets the camera bounds to the first frame's rectangle and moves the camera to the object's position
- [ ] `00406980` inferred_Obj_Kind01_Ctor · depth 3 · 16 lines · **decompiled** — 0x84 bytes, vtable 00428ac8, extends Kind00; Level_CreateObject stores the instance at level +0x90 (single special object per level)
- [ ] `00407000` inferred_Item_SetHeld (vtable) · depth 3 · 30 lines · **decompiled** — kind 13 method (flag): when set, makes this object the carried item (cursor +0x12c) and switches the cursor animation to item id + 7; when cleared, drops it. Verb indices 7 and above are therefore carried inventory items
- [ ] `00406a90` inferred_Obj_Kind13_Ctor · depth 12 · 88 lines · **decompiled** — 0xac bytes, extends MidBase; 14 per-subtype vtables selected by record byte 1; increments global instance counter 004420f8; phrase key lookup
- [ ] `00406d50` inferred_C00428b00_SetVerbAndClick (vtable) · depth 12 · 123 lines · **untouched** — unread; first seen in vtable 00428b00 slot 14
- [ ] `00406190` inferred_DialogueTable_Load · depth 13 · 228 lines · **decompiled** — (archive offset, id): reads entries introduced by '.' bytes; each entry built by 00404810 and registered by kind

### Cluster 12: 004079d0–00407aa0 (0/11 ported)

- [ ] `004079d0` inferred_Jim_IsInPose7 (vtable) · depth 0 · 7 lines · **decompiled** — slot 11: returns (+0x5c == 7)
- [ ] `00407aa0` inferred_Jim_ResumeFacing · depth 0 · 10 lines · **decompiled** — if walking (+0xd5) applies the stored facing again via slot 29
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
- [ ] `00407d00` inferred_Dialogue_ChoiceMenu · depth 1 · 71 lines · **decompiled** — lays out N option text lines from the phrase records, picks the one the mouse y falls in, a left click removes the entry from the main list; decompile is partly garbled
- [ ] `00407f30` inferred_Jim_TalkAnimate (vtable) · depth 2 · 37 lines · **decompiled** — slot 31: draws the speaking-mouth animation; while talking picks a random frame (RandomRange) at most every 100 ms of game time, else rests on frame 1
- [ ] `004080a0` inferred_Jim_UseItemForward (vtable) · depth 2 · 17 lines · **decompiled** — slot 27: forwards to a level-interface object (name key 0042d2a8) slot 27
- [ ] `00408520` inferred_Level_CameraMoveTo · depth 2 · 8 lines · **decompiled** — forwards (x, y, duration) to the camera at +0x80
- [ ] `00408550` inferred_Level_UpdateRoomCameras · depth 2 · 52 lines · **untouched** — for every room, moves the camera toward the room's focus point and re-sorts its objects | name from call sites and a skim; body not yet read
- [ ] `00408030` inferred_Jim_SetIdlePose (vtable) · depth 3 · 29 lines · **decompiled** — slot 30: direction 3,4,2,1 -> idle animation index 0,1,2,3 via 00410130, then recomputes speech anchor point +0xa4
- [ ] `00408540` inferred_Level_AttachObjects · depth 3 · 8 lines · **decompiled** — calls 0040f710 on the level's object list (+0x74), which calls 00401bf0(child, level) for every child
- [ ] `00407b80` inferred_Jim_EnterLevel · depth 11 · 50 lines · **decompiled** — (level, room, x, y) on the player: leaves the old level (removes from its lists and the main list), joins the new level, sets camera and position, inserts into the main list at priority 0x14
- [ ] `00407ce0` inferred_Jim_SayLine · depth 11 · 8 lines · **decompiled** — Player_Say with a negative id derived from +0x100 (spoken subtitle line)
- [ ] `004080e0` inferred_Level_Load · depth 15 · 142 lines · **decompiled** — level constructor and .LVL reader: base object ctor 00401bc0 (vtable 00428bf8), reads 0xc header (room count, anim-bank count), allocates lists, loads anim banks with AnimBank_Load, reads each 0x28-byte room record and its 0x20-byte object records, builds objects through Level_CreateObject; Ghidra decompile is garbled by the 0x13xx-byte frame, re-read from asm before porting

### Cluster 14: 00408780–00408890 (0/8 ported)

- [ ] `00408780` inferred_Level_FindObjectByKey · depth 1 · 8 lines · **decompiled** — forwards to the object list at +0x74 with the 64-bit key
- [ ] `00408840` inferred_C00428bf8_RunVerb (vtable) · depth 1 · 14 lines · **untouched** — unread; first seen in vtable 00428bf8 slot 13
- [ ] `00408890` inferred_Level_CollisionPass · depth 2 · 200 lines · **decompiled** — per-frame pass over the level's object list (+0x7c): for each pair of objects with flags 8 (collidable) and 2 (active) in the same room, asks A.slot11(B) then runs ResolveOverlap; afterwards sends queued position notifications (+0x78 list of object,x,y) through object slot 12
- [ ] `004087a0` inferred_Level_UpdateOrCollide (vtable) · depth 3 · 17 lines · **decompiled** — level virtual method: when flag +0x94 is set clears it and calls slot 13 (0x34), else runs the collision pass; returns true
- [ ] `004087d0` inferred_C00428bf8_Update2 (vtable) · depth 3 · 10 lines · **untouched** — unread; first seen in vtable 00428bf8 slot 5
- [ ] `00408620` inferred_Level_SaveState · depth 8 · 77 lines · **untouched** — writes the object keys and flags and calls each object's SaveState (slot 10) via fwrite | name from call sites and a skim; body not yet read
- [ ] `00408710` inferred_Level_LoadState · depth 8 · 20 lines · **decompiled** — reads object key + flags records until a zero key and calls LoadState (slot 9) on the matching object
- [ ] `00408800` inferred_Level_ReloadDeferredAnims · depth 10 · 18 lines · **decompiled** — rewinds pool arena 2, reloads the deferred animation images of every bank and sets +0x94

### Cluster 15: 00409280–0040c040 (0/7 ported)

- [ ] `00409280` inferred_Rect_SweepTime · depth 0 · 124 lines · **decompiled** — (rect A, rect B, dx, dy): earliest contact of two moving rectangles as a 16.16 fraction, -1 when they never touch. Name from use in ResolveOverlap, unverified
- [ ] `00409560` inferred_Polygon_ContainsPoint · depth 0 · 142 lines · **decompiled** — (polygon, x, y): even-odd edge crossing test over the shape's vertex list (count at +0, vertex array pointer at +4, vertices are short pairs); returns bool. Read partially
- [ ] `00409790` inferred_Polygon_Overlaps · depth 0 · 255 lines · **decompiled** — (polygon A, polygon B): edge/vertex overlap test between two shapes; called in both orders by ResolveOverlap. Not read in detail
- [ ] `00409c10` inferred_Polygon_ClipToRect · depth 0 · 113 lines · **decompiled** — (polygon, rect x0,y0,x1,y1, out polygon, rect of other object): produces the part of a shape inside a rectangle. Name from argument list only, unverified
- [ ] `00408c50` inferred_Level_ResolveOverlap · depth 1 · 256 lines · **decompiled** — (A, B) swept test of A's collision shape against B's using segment tests (00409280, 00409c10) and polygon tests (00409560, 00409790); chooses the contact position. Contains no virtual calls
- [ ] `00409e50` inferred_Level_CreateObject · depth 14 · 1145 lines · **decompiled** — factory: switch on kind (record +0x18) allocating the right object class (sizes 0x6c, 0x84, 0x78, 0x6c, 0x74, 0xd8 ... kind 5 has a sub-switch on record byte +5); adds objects of kind 1 to level +0x90
- [ ] `0040c040` inferred_C0042a600_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a600 slot 21 (kind 13 subtype 16)

### Cluster 16: 0040c0e0–0040c4d0 (0/12 ported)

- [ ] `0040c0e0` inferred_C0042a4b0_Verb4 (vtable) · depth 0 · 8 lines · **untouched** — unread; first seen in vtable 0042a4b0 slot 24 (kind 13 subtype 15)
- [ ] `0040c1f0` inferred_C0042a3d0_Verb4 (vtable) · depth 0 · 8 lines · **untouched** — unread; first seen in vtable 0042a3d0 slot 24 (kind 13 subtype 19)
- [ ] `0040c090` inferred_C0042a590_Verb4 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a590 slot 24 (kind 13 subtype 10)
- [ ] `0040c0f0` inferred_C0042a440_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a440 slot 21 (kind 13 subtype 28)
- [ ] `0040c140` inferred_C0042a3d0_Verb1 (vtable) · depth 14 · 35 lines · **untouched** — unread; first seen in vtable 0042a3d0 slot 21 (kind 13 subtype 19)
- [ ] `0040c200` inferred_C0042a280_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a280 slot 21 (kind 13 subtype 44)
- [ ] `0040c250` inferred_C0042a1a0_Verb4 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 0042a1a0 slot 24 (kind 13 subtype 29)
- [ ] `0040c2c0` inferred_C0042a6e0_Verb3 (vtable) · depth 14 · 31 lines · **untouched** — unread; first seen in vtable 0042a6e0 slot 22 (kind 12 subtype 10)
- [ ] `0040c370` inferred_C0042a910_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a910 slot 21 (kind 12 subtype 5)
- [ ] `0040c3c0` inferred_C0042a910_Verb3 (vtable) · depth 14 · 31 lines · **untouched** — unread; first seen in vtable 0042a910 slot 22 (kind 12 subtype 5)
- [ ] `0040c480` inferred_C0042a910_Verb4 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a910 slot 24 (kind 12 subtype 5)
- [ ] `0040c4d0` inferred_C0042aa60_Verb1 (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 0042aa60 slot 21 (kind 12 subtype 2)

### Cluster 17: 0040c560–0040c810 (0/7 ported)

- [ ] `0040c560` inferred_C0042ab40_Verb3 (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 0042ab40 slot 22 (kind 12 subtype 0)
- [ ] `0040c5d0` inferred_C0042ab40_Verb6 (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 0042ab40 slot 26 (kind 12 subtype 0)
- [ ] `0040c640` inferred_C0042a9f0_Verb4 (vtable) · depth 14 · 30 lines · **untouched** — unread; first seen in vtable 0042a9f0 slot 24 (kind 12 subtype 3)
- [ ] `0040c6d0` inferred_C0042a980_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a980 slot 21 (kind 12 subtype 4)
- [ ] `0040c720` inferred_C0042aad0_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042aad0 slot 21 (kind 12 subtype 1)
- [ ] `0040c770` inferred_C0042a8a0_Verb3 (vtable) · depth 14 · 30 lines · **untouched** — unread; first seen in vtable 0042a8a0 slot 22 (kind 12 subtype 6)
- [ ] `0040c810` inferred_C0042a670_Verb0 (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 0042a670 slot 20 (kind 12 subtype 11)

### Cluster 18: 0040c8a0–0040ca60 (0/9 ported)

- [ ] `0040c8a0` inferred_C0042aef8_Verb4 (vtable) · depth 1 · 9 lines · **untouched** — unread; first seen in vtable 0042aef8 slot 24 (kind 6 subtype 8)
- [ ] `0040c8d0` inferred_C0042aef8_Slot15 (vtable) · depth 2 · 13 lines · **untouched** — unread; first seen in vtable 0042aef8 slot 15 (kind 6 subtype 8)
- [ ] `0040c9e0` inferred_C0042a7c0_Verb3 (vtable) · depth 2 · 14 lines · **untouched** — unread; first seen in vtable 0042a7c0 slot 22 (kind 12 subtype 9)
- [ ] `0040c880` inferred_C0042aef8_Verb0 (vtable) · depth 13 · 13 lines · **untouched** — unread; first seen in vtable 0042aef8 slot 20 (kind 6 subtype 8)
- [ ] `0040c8f0` inferred_C0042b390_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042b390 slot 23 (kind 5 subtype 3)
- [ ] `0040c940` inferred_C0042a830_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a830 slot 21 (kind 12 subtype 7)
- [ ] `0040c990` inferred_C0042a830_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a830 slot 23 (kind 12 subtype 7)
- [ ] `0040ca10` inferred_C0042a750_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a750 slot 21 (kind 12 subtype 8)
- [ ] `0040ca60` inferred_C0042a750_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042a750 slot 23 (kind 12 subtype 8)

### Cluster 19: 0040cab0–0040cd60 (0/8 ported)

- [ ] `0040cab0` inferred_C00429fe0_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429fe0 slot 21 (kind 17 subtype 1)
- [ ] `0040cb00` inferred_C00429f70_Verb1 (vtable) · depth 14 · 22 lines · **untouched** — unread; first seen in vtable 00429f70 slot 21 (kind 17 subtype 2)
- [ ] `0040cb60` inferred_C00429d40_Verb3 (vtable) · depth 14 · 20 lines · **untouched** — unread; first seen in vtable 00429d40 slot 22 (kind 17 subtype 13)
- [ ] `0040cbd0` inferred_C00429f00_Verb3 (vtable) · depth 14 · 20 lines · **untouched** — unread; first seen in vtable 00429f00 slot 22 (kind 17 subtype 3)
- [ ] `0040cc40` inferred_C00429e90_Verb1 (vtable) · depth 14 · 25 lines · **untouched** — unread; first seen in vtable 00429e90 slot 21 (kind 17 subtype 4)
- [ ] `0040ccc0` inferred_C00429e20_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429e20 slot 21 (kind 17 subtype 5)
- [ ] `0040cd10` inferred_C0042b6c0_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042b6c0 slot 23 (kind 5 subtype 5)
- [ ] `0040cd60` inferred_C0042b638_Verb2 (vtable) · depth 14 · 28 lines · **untouched** — unread; first seen in vtable 0042b638 slot 23 (kind 5 subtype 6)

### Cluster 20: 0040cdf0–0040d1a0 (0/10 ported)

- [ ] `0040cdf0` inferred_C00429a30_Verb3 (vtable) · depth 2 · 13 lines · **untouched** — unread; first seen in vtable 00429a30 slot 22 (kind 17 subtype 14)
- [ ] `0040cfd0` inferred_C004299c0_Verb3 (vtable) · depth 2 · 13 lines · **untouched** — unread; first seen in vtable 004299c0 slot 22 (kind 17 subtype 15)
- [ ] `0040ce20` inferred_C00429b80_Verb3 (vtable) · depth 14 · 20 lines · **untouched** — unread; first seen in vtable 00429b80 slot 22 (kind 17 subtype 10)
- [ ] `0040ce90` inferred_C00429b10_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429b10 slot 23 (kind 17 subtype 11)
- [ ] `0040cee0` inferred_C00429aa0_Verb3 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429aa0 slot 22 (kind 17 subtype 12)
- [ ] `0040cf30` inferred_C00429cd0_Verb3 (vtable) · depth 14 · 34 lines · **untouched** — unread; first seen in vtable 00429cd0 slot 22 (kind 17 subtype 7)
- [ ] `0040d000` inferred_C00429c60_Verb1 (vtable) · depth 14 · 20 lines · **untouched** — unread; first seen in vtable 00429c60 slot 21 (kind 17 subtype 8)
- [ ] `0040d070` inferred_C00429c60_Verb3 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 00429c60 slot 22 (kind 17 subtype 8)
- [ ] `0040d100` inferred_C00429bf0_Verb3 (vtable) · depth 14 · 29 lines · **untouched** — unread; first seen in vtable 00429bf0 slot 22 (kind 17 subtype 9)
- [ ] `0040d1a0` inferred_C0042ad18_Verb0 (vtable) · depth 14 · 37 lines · **untouched** — unread; first seen in vtable 0042ad18 slot 20 (kind 6 subtype 4)

### Cluster 21: 0040d240–0040d5a0 (0/10 ported)

- [ ] `0040d240` inferred_C00428c30_Verb0 (vtable) · depth 2 · 14 lines · **untouched** — unread; first seen in vtable 00428c30 slot 20 (kind 20 subtype 30)
- [ ] `0040d330` inferred_C00428d80_Verb3 (vtable) · depth 2 · 16 lines · **untouched** — unread; first seen in vtable 00428d80 slot 22 (kind 20 subtype 27)
- [ ] `0040d270` inferred_C004298e0_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 004298e0 slot 23 (kind 20 subtype 0)
- [ ] `0040d2c0` inferred_C00429090_Verb0 (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00429090 slot 20 (kind 20 subtype 20)
- [ ] `0040d380` inferred_C0042b170_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042b170 slot 21 (kind 5 subtype 14)
- [ ] `0040d3d0` inferred_C0042b170_Verb2 (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 0042b170 slot 23 (kind 5 subtype 14)
- [ ] `0040d450` inferred_C00429720_Verb1 (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 00429720 slot 21 (kind 20 subtype 4)
- [ ] `0040d4e0` inferred_C00429790_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429790 slot 21 (kind 20 subtype 3)
- [ ] `0040d530` inferred_C00429790_Verb0 (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00429790 slot 20 (kind 20 subtype 3)
- [ ] `0040d5a0` inferred_C004296b0_Verb0 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 004296b0 slot 20 (kind 20 subtype 5)

### Cluster 22: 0040d620–0040d9a0 (0/12 ported)

- [ ] `0040d620` inferred_C00429640_Verb3 (vtable) · depth 2 · 14 lines · **untouched** — unread; first seen in vtable 00429640 slot 22 (kind 20 subtype 6)
- [ ] `0040d820` inferred_C00429480_Verb3 (vtable) · depth 2 · 16 lines · **untouched** — unread; first seen in vtable 00429480 slot 22 (kind 20 subtype 10)
- [ ] `0040d910` inferred_C00428ed0_Verb3 (vtable) · depth 2 · 14 lines · **untouched** — unread; first seen in vtable 00428ed0 slot 22 (kind 20 subtype 23)
- [ ] `0040d650` inferred_C004294f0_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 004294f0 slot 23 (kind 20 subtype 9)
- [ ] `0040d6a0` inferred_C004294f0_Verb3 (vtable) · depth 14 · 20 lines · **untouched** — unread; first seen in vtable 004294f0 slot 22 (kind 20 subtype 9)
- [ ] `0040d6f0` inferred_C00429330_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429330 slot 21 (kind 20 subtype 13)
- [ ] `0040d740` inferred_C004293a0_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 004293a0 slot 21 (kind 20 subtype 12)
- [ ] `0040d790` inferred_C004293a0_Verb3 (vtable) · depth 14 · 26 lines · **untouched** — unread; first seen in vtable 004293a0 slot 22 (kind 20 subtype 12)
- [ ] `0040d870` inferred_C00428ca0_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00428ca0 slot 21 (kind 20 subtype 29)
- [ ] `0040d8c0` inferred_C00428f40_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00428f40 slot 21 (kind 20 subtype 24)
- [ ] `0040d950` inferred_C00429170_Verb3 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00429170 slot 22 (kind 20 subtype 18)
- [ ] `0040d9a0` inferred_C00428e60_Verb1 (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 00428e60 slot 21 (kind 20 subtype 25)

### Cluster 23: 0040db40–0040dd40 (0/12 ported)

- [ ] `0040db40` inferred_Pool_AllocZero · depth 0 · 38 lines · **understood** — bump-allocates size bytes (4-aligned) zeroed from arena index in global pool 00442100; returns 0 when full
- [ ] `0040dc00` inferred_Pool_IsValid · depth 0 · 7 lines · **decompiled**
- [ ] `0040dc10` inferred_Pool_AllocAligned · depth 0 · 25 lines · **decompiled** — thiscall(size, align, arena) no zero fill
- [ ] `0040dc70` inferred_Pool_AllocAlignedZero · depth 0 · 36 lines · **decompiled** — thiscall(size, align, arena) zeroed
- [ ] `0040dcf0` inferred_Pool_SetArena · depth 0 · 20 lines · **decompiled** — carves arena N of given size out after the previous one (sizes 0x17a6b0, 3000000, 9600000, 650000 for arenas 0-3)
- [ ] `0040dd30` inferred_Pool_ResetArena · depth 0 · 8 lines · **understood** — rewinds arena bump pointer to its start
- [ ] `0040dd40` inferred_Obj_Kind23_Ctor · depth 4 · 20 lines · **decompiled** — 0xa4 bytes, vtable 0042b8e0, extends MidBase, sort value +0x5e = 2 (0 when flag byte set)
- [ ] `0040dbc0` inferred_Pool_Ctor · depth 5 · 11 lines · **decompiled** — allocates param*1000 bytes (called with 0x39d0 = about 14.8 MB)
- [ ] `0040dbf0` inferred_Pool_Dtor · depth 5 · 8 lines · **decompiled**
- [ ] `0040da30` inferred_C00428df0_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 00428df0 slot 21 (kind 20 subtype 26)
- [ ] `0040da80` inferred_C00428fb0_Verb3 (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00428fb0 slot 22 (kind 20 subtype 22)
- [ ] `0040daf0` inferred_C0042ae80_Verb0 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042ae80 slot 20 (kind 6 subtype 7)

### Cluster 24: 0040e580–0040e720 (0/12 ported)

- [ ] `0040e580` inferred_C0042b8e0_SetVerbAndClick (vtable) · depth 0 · 7 lines · **untouched** — unread; first seen in vtable 0042b8e0 slot 14
- [ ] `0040e590` inferred_C0042b8e0_IsInteractive (vtable) · depth 0 · 7 lines · **untouched** — unread; first seen in vtable 0042b8e0 slot 16
- [ ] `0040e5a0` inferred_Obj_Kind03Base_DefaultCtor · depth 1 · 9 lines · **decompiled** — base object ctor with vtable 0042b9c0
- [ ] `0040dfc0` inferred_C0042b950_IsInteractive (vtable) · depth 2 · 12 lines · **untouched** — unread; first seen in vtable 0042b950 slot 16
- [ ] `0040e560` inferred_C0042b950_ClearHoverText (vtable) · depth 2 · 10 lines · **untouched** — unread; first seen in vtable 0042b950 slot 18
- [ ] `0040e6a0` inferred_C00428038_AnimTick (vtable) · depth 2 · 8 lines · **untouched** — unread; first seen in vtable 00428038 slot 2
- [ ] `0040e6b0` inferred_C0042b9c0_Draw (vtable) · depth 2 · 20 lines · **untouched** — unread; first seen in vtable 0042b9c0 slot 6
- [ ] `0040e5c0` inferred_Obj_Kind03_Ctor · depth 3 · 30 lines · **decompiled** — 0x6c bytes, vtable 0042b9c0, extends Kind00; record byte 1 selects an animation start mode via AnimPlayer_SetRange (0 stay, 1 single frame, 2 loop from, 3 loop with flag)
- [ ] `0040df70` inferred_Obj_Kind22_Ctor · depth 4 · 12 lines · **decompiled** — 0xa4 bytes, vtable 0042b950, extends MidBase, sort value +0x5e = 1
- [ ] `0040e720` inferred_Obj_Kind04_Ctor · depth 4 · 10 lines · **decompiled** — 0x74 bytes, vtable 0042b9f8, extends Kind03
- [ ] `0040ddc0` inferred_C0042b8e0_Draw (vtable) · depth 14 · 73 lines · **untouched** — unread; first seen in vtable 0042b8e0 slot 6
- [ ] `0040e000` inferred_C0042b950_SetVerbAndClick (vtable) · depth 15 · 142 lines · **untouched** — unread; first seen in vtable 0042b950 slot 14

### Cluster 25: 0040f020–0040f020 (0/11 ported)

- [ ] `0040f020` inferred_Obj_SetVerbAndClick (vtable) · depth 0 · 10 lines · **decompiled** — MidBase slot 14: stores verb +0x9c then calls slot 19 (OnClick)
- [ ] `0040ebd0` inferred_C0042ba30_OnAttach (vtable) · depth 1 · 14 lines · **untouched** — unread; first seen in vtable 0042ba30 slot 0
- [ ] `0040ec00` inferred_C0042ba30_Draw (vtable) · depth 1 · 24 lines · **untouched** — unread; first seen in vtable 0042ba30 slot 6
- [ ] `0040ec90` inferred_Obj_MidBase_DefaultCtor · depth 1 · 9 lines · **decompiled** — base object ctor with vtable 0042ba68 only
- [ ] `0040e760` inferred_C0042b9f8_Draw (vtable) · depth 2 · 22 lines · **untouched** — unread; first seen in vtable 0042b9f8 slot 6
- [ ] `0040efa0` inferred_Obj_Draw (vtable) · depth 2 · 24 lines · **decompiled** — MidBase slot 6: converts world position to screen (camera at argument, +0x140/+0xf0 centre) and queues the current frame's image through Display_AddSprite with the object's sort value (+0x5e)
- [ ] `0040e890` inferred_C0042ba30_AnimTick (vtable) · depth 3 · 103 lines · **untouched** — unread; first seen in vtable 0042ba30 slot 2
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

- [ ] `0040f670` inferred_List_RewindIterator · depth 0 · 12 lines · **decompiled** — resets a list's iterator (current = head, first flag = 1)
- [ ] `0040f680` inferred_ObjectList_FindByKey · depth 0 · 55 lines · **decompiled** — (key lo, key hi) finds an object in the list by its 64-bit name key at object +0x44; returns 0 when absent
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
- [ ] `0040f870` inferred_List_Clear · depth 0 · 14 lines · **decompiled** — empties a 2-field-node list
- [ ] `0040f900` inferred_ObjectList_InsertByPriority · depth 0 · 82 lines · **decompiled** — 12-byte nodes (object, priority): inserts before the first node with a higher priority; main list uses priorities 0x13 (interface level), 100 (top object)
- [ ] `0040fa10` inferred_ObjectList_InsertByPriorityFront · depth 0 · 82 lines · **decompiled** — like InsertByPriority but inserts before nodes of equal priority
- [ ] `0040fb20` inferred_ObjectList_RemoveByObject · depth 0 · 58 lines · **decompiled** — removes the node holding the given object from the main list
- [ ] `0040f890` inferred_ObjectList_Ctor · depth 1 · 23 lines · **decompiled** — allocates a linked list with capacity param (100 for the main list 00442104)
- [ ] `0040fbb0` inferred_ObjectList_AttachAll · depth 2 · 38 lines · **decompiled** — walks the list calling Object_AttachToLevel on each object

### Cluster 29: 0040fc20–00410330 (0/12 ported)

- [ ] `0040fc20` inferred_ObjectList_PassA · depth 0 · 83 lines · **decompiled** — walks active objects (flag +0x68 bit 1), marks 0x200, calls object vtable slot 4; returns 2 or 3 immediately if an object returns that, else 1
- [ ] `0040fd20` inferred_ObjectList_PassB · depth 0 · 83 lines · **decompiled** — same walk calling vtable slot 5
- [ ] `0040fe20` inferred_ObjectList_Clear · depth 0 · 14 lines · **decompiled** — empties a 3-field-node list (priority lists)
- [ ] `0040fe40` inferred_ObjectList_EndFrame · depth 0 · 31 lines · **decompiled** — iterator reset helper; its result decides loop continuation
- [ ] `0040fea0` inferred_Obj_Kind05Base_Ctor · depth 2 · 9 lines · **decompiled** — MidBase default ctor then vtable 0042bb48
- [ ] `0040ffa0` inferred_Obj_DrawSubtitle · depth 2 · 27 lines · **decompiled** — draws the current phrase text twice (shadow style) at the speaker's position with Display_DrawText, keeping the camera in view
- [ ] `00410060` inferred_C0042b060_Slot31 (vtable) · depth 2 · 34 lines · **untouched** — unread; first seen in vtable 0042b060 slot 31 (kind 5 subtype 16)
- [ ] `00410130` inferred_Jim_SetPoseAnim (vtable) · depth 2 · 41 lines · **decompiled** — (direction index) recomputes speech anchor and selects the bank listed in +0xb0..
- [ ] `004102e0` inferred_Obj_Kind05_Draw (vtable) · depth 3 · 20 lines · **decompiled** — slot 6 of the kind 5 class family: wraps Obj_Draw with slot 31 calls depending on flags +0xd4/+0xc/+0xd5
- [ ] `004101f0` inferred_Obj_Kind05_Update (vtable) · depth 10 · 46 lines · **decompiled** — slot 4 of the kind 5 class family: runs Interactive_Update unless deferred, polls keyboard/mouse button state and the sound manager to stop a playing sound on input, calls slot 28
- [ ] `00410330` inferred_Player_Say · depth 10 · 33 lines · **decompiled** — (phrase record, id, flag) on the object in global 004420fc (walks, so probably the player): stores phrase +0xc4 / id +0xcc, busy +0xd4=1, starts the phrase sound (record +8) through the sound manager, slot 30 (0x78) with flag, inserts itself into the main list at priority 6, calls slot 4 | see ledger note
- [ ] `0040fec0` inferred_Obj_Kind05_Ctor · depth 13 · 40 lines · **decompiled** — 0xd8 bytes, extends Kind09; 17 per-subtype vtables chosen by record byte 5; scans anim banks for names starting with two 3-char tags

### Cluster 30: 004103f0–004108a0 (0/8 ported)

- [ ] `004103f0` inferred_C0042b060_Slot32 (vtable) · depth 0 · 8 lines · **untouched** — unread; first seen in vtable 0042b060 slot 32 (kind 5 subtype 16)
- [ ] `004104a0` inferred_Room_InsertSorted · depth 0 · 44 lines · **decompiled** — inserts object into the room list ordered by object field +0x4c
- [ ] `00410530` inferred_Room_RemoveObject · depth 0 · 38 lines · **decompiled**
- [ ] `004105a0` inferred_Display_QueueRow_TypeA · depth 0 · 177 lines · **untouched** — AddSprite helper for image type 1: builds per-scanline entries (177 lines) | name from call sites and a skim; body not yet read
- [ ] `004108a0` inferred_Display_Raster_TypeA · depth 0 · 215 lines · **untouched** — rasterises queued type-A rows (215 lines) | name from call sites and a skim; body not yet read
- [ ] `00410400` inferred_Jim_ClearSpeech (vtable) · depth 1 · 10 lines · **decompiled** — slot 28: clears phrase +0xc4 and talking flag +0xd4, removes itself from the main list
- [ ] `00410420` inferred_Room_Ctor · depth 1 · 26 lines · **decompiled** — 16-byte room record: +0 index, +4 a field from the file record +0x24, +8/+10 shorts, +0xc object list
- [ ] `004103d0` inferred_Obj_SetFacingAnim (vtable) · depth 3 · 9 lines · **decompiled** — selects the bank from +0xac and starts animation index (param-1) with AnimPlayer

### Cluster 31: 00410cc0–00411f80 (0/12 ported)

- [ ] `00410cc0` inferred_Display_QueueRow_TypeB · depth 0 · 173 lines · **untouched** — AddSprite helper for image type 3 | name from call sites and a skim; body not yet read
- [ ] `00410fb0` inferred_Display_Raster_TypeB · depth 0 · 177 lines · **untouched** — rasterises queued type-B rows | name from call sites and a skim; body not yet read
- [ ] `004112d0` inferred_Display_QueueRow_Mask · depth 0 · 159 lines · **untouched** — AddSprite helper writing the second row buffer (+0x674) | name from call sites and a skim; body not yet read
- [ ] `00411580` inferred_Display_Raster_Mask · depth 0 · 164 lines · **untouched** — rasterises the mask rows | name from call sites and a skim; body not yet read
- [ ] `00411be0` inferred_Display_Dtor · depth 0 · 10 lines · **decompiled** — releases the back surface (+0xc), primary surface (+8) and DirectDraw object (+4, slot 0x4c = RestoreDisplayMode)
- [ ] `00411c00` inferred_Display_IsReady · depth 0 · 7 lines · **decompiled** — returns whether the primary surface pointer (+8) is set (corrected: it is a check, not DirectDraw setup)
- [ ] `00411c10` inferred_Display_SetFont · depth 0 · 8 lines · **decompiled** — stores pointer at slot (index*4+0x20)
- [ ] `00411c80` inferred_Display_DrawText · depth 0 · 127 lines · **untouched** — (font, text, colour/limits, x, y, width, ...) renders a string with the game font into the frame | name from call sites and a skim; body not yet read
- [ ] `00411f20` inferred_Display_ResetRenderQueue · depth 0 · 30 lines · **decompiled** — clears two arrays of 0x1e0 entries (stride 500 and 200 dwords) and counters
- [ ] `00411870` inferred_Display_Ctor · depth 1 · 127 lines · **decompiled** — 0x13a4-byte display constructor performing the whole DirectDraw setup: DirectDrawCreate, cooperative level, display mode, primary and back surfaces; each failure shows a DIRECTDRAW message box and returns the half-built object. Flag +0x67c selects 5-5-5 pixel handling
- [ ] `00411f80` inferred_Display_AddSprite · depth 1 · 57 lines · **decompiled** — (x, y, sort value, image, flag): clips a 640x480 sprite and queues it in the render rows (per-scanline lists) by image type (1 or 3 get extra passes)
- [ ] `00411c20` inferred_Display_LoadColourTable · depth 8 · 22 lines · **decompiled** — reads 0x8000 bytes (TABLA1.TBL) into display table +0x66c, converts to 5-5-5 when flag +0x67c set

### Cluster 32: 004120c0–00412e10 (0/7 ported)

- [ ] `004120c0` inferred_Display_AddSpriteScaled · depth 0 · 21 lines · **decompiled** — like AddSprite with x/y scale factors (16.16 fixed)
- [ ] `00412640` inferred_Display_Compose_Background · depth 0 · 282 lines · **untouched** — first stage of Display_Compose (282 lines) | name from call sites and a skim; body not yet read
- [ ] `00412bb0` inferred_Display_ClearAndReset · depth 0 · 34 lines · **decompiled** — clears surfaces via DirectDraw (slot 0x80/4 and 0x1c Blt) and resets the render queue
- [ ] `00412c50` inferred_Display_Compose_Overlay · depth 0 · 99 lines · **untouched** — later stage of Display_Compose | name from call sites and a skim; body not yet read
- [ ] `00412120` inferred_Display_RasterRow · depth 1 · 101 lines · **untouched** — rasterises one scanline using the three type rasterisers | name from call sites and a skim; body not yet read
- [ ] `004123e0` inferred_Display_StartEffect · depth 1 · 118 lines · **untouched** — (effect, speed) starts a screen transition (called with (4, 100) after level changes); uses GetTickCount | name from call sites and a skim; body not yet read
- [ ] `00412e10` inferred_Display_Compose · depth 2 · 263 lines · **untouched** — per-frame composition: runs raster stages and presents (flip) the frame | name from call sites and a skim; body not yet read

### Cluster 33: 004137c0–004139e0 (1/7 ported)

- [ ] `004137c0` inferred_TextTable_Sort · depth 0 · 67 lines · **untouched** — sorts 16-byte table records by their 64-bit key | name from call sites and a skim; body not yet read
- [ ] `004138b0` inferred_TextTable_Lookup · depth 0 · 47 lines · **decompiled** — (key lo, key hi) binary search in the sorted table; returns the record or 0
- [x] `004139c0` inferred_Clock_Ctor · depth 0 · 14 lines · **emu-compared** → `core/clock.cpp::GameClock::GameClock` — game clock: +0 accumulated ms, +4 frozen value, +8 last tick, +0xc frozen, +0xd suspended
- [ ] `004139e0` inferred_BusyWait · depth 0 · 18 lines · **decompiled** — (ms) spins on GetTickCount until the time has passed
- [ ] `004132b0` inferred_Movie_Start · depth 11 · 123 lines · **decompiled** — DirectShow setup via CoCreateInstance, builds path c:\INSTALL\BIN\DATA\%02i.BIN; last call of InitGameWorld. Only the setup was read, what it plays and when is unconfirmed
- [ ] `00413570` inferred_TextTable_Load · depth 11 · 121 lines · **decompiled** — (archive offset): reads lines of the OBJETOS.TXT-style table and stores records of 16 bytes; first pass counts lines
- [ ] `00413940` inferred_PhraseLookup · depth 11 · 27 lines · **decompiled** — (name) hashes the name with NameHash64 and searches the four text tables 00442110..0044211c; MessageBox "frase no encontrada" if missing

### Cluster 34: 00413a10–00414560 (7/12 ported)

- [x] `00413a10` inferred_Clock_Now · depth 0 · 19 lines · **emu-compared** → `core/clock.cpp::GameClock::now` — adds elapsed GetTickCount delta when below 500 ms (longer gaps are ignored); returns frozen value while frozen
- [x] `00413a50` inferred_Clock_Freeze · depth 0 · 18 lines · **emu-compared** → `core/clock.cpp::GameClock::freeze` — updates accumulation then freezes at the current value (called at start of each frame)
- [x] `00413a80` inferred_Clock_Suspend · depth 0 · 8 lines · **emu-compared** → `core/clock.h::GameClock::suspend` — window deactivated: stop accumulating
- [x] `00413a90` inferred_Clock_Resume · depth 0 · 12 lines · **emu-compared** → `core/clock.cpp::GameClock::resume` — window activated: re-stamp last tick, resume
- [x] `00413ab0` inferred_Clock_Unfreeze · depth 0 · 8 lines · **emu-compared** → `core/clock.h::GameClock::unfreeze` — clears frozen flag (end of frame)
- [x] `00413ac0` inferred_RandomRangeAlt · depth 0 · 11 lines · **emu-compared** → `core/random.cpp::randomRangeAlt` — second time-driven random function: fraction = (t&0x78) | ((t>>7)&7) | ((t&7)<<7), scaled like RandomRange
- [x] `00413b00` inferred_RandomRange · depth 0 · 13 lines · **emu-compared** → `core/random.cpp::randomRange` — THE game random generator: builds a pseudo-random fraction from bit-shuffled GetTickCount() low bits, scales by (max-min+1) >> 10, adds min. Time-based, not seeded
- [ ] `00413b90` inferred_InitSubsystems · depth 6 · 140 lines · **understood** — builds CD, input, sound, video objects; error dialogs
- [ ] `004144a0` inferred_Shutdown · depth 7 · 45 lines · **understood** — deletes display, sound, mouse, keyboard, archive, memory pools, clock in that order
- [ ] `00414420` inferred_RunFrame · depth 10 · 21 lines · **understood** — per-frame step: keyboard and mouse vtable[0] update, clock freeze, sound update, rewind pool 3, reset render queue, object-list pass A (and pass B unless A returned 3), clock unfreeze; returns false when loop should end
- [ ] `00414560` inferred_Game_StartIntroSequence · depth 14 · 25 lines · **decompiled** — enters the stage level with Jim, shows him, and starts a dialogue entry by key
- [ ] `00413e60` inferred_InitGameWorld · depth 16 · 252 lines · **decompiled** — opens archive Tesoro.DAT/Tesoro.TRE, loads TABLA1.TBL, FUENTES.ANM, OBJETOS.TXT/DLG tables, INTERFACE.LVL, CUBIERTA/MAPA/DESPACHO..LVL levels (0x2f slots), PRINCIPAL.LVL, NUEVO.LVL, then starts the first level; SEH frame

### Cluster 35: 00414740–00414dc0 (0/12 ported)

- [ ] `00414740` inferred_WalkArea_ScaleAt · depth 0 · 86 lines · **decompiled** — (x, y) piecewise interpolation of a perspective scale (default 0x100) from the walk-area edge table in the current frame record | (x, y) perspective scale 0x100 = 1.0: finds the polyline edges spanning x, interpolates their y and per-vertex scale, then interpolates the scale vertically between the edge above and the edge below the point; 0x100 when no edge pair brackets the point
- [ ] `00414600` inferred_Keyboard_Dtor · depth 1 · 9 lines · **decompiled** — vtable 00428070
- [ ] `00414610` inferred_Mouse_Dtor · depth 1 · 9 lines · **decompiled** — vtable 00428074
- [ ] `004146b0` inferred_C0042bc78_OnAttach (vtable) · depth 1 · 23 lines · **untouched** — unread; first seen in vtable 0042bc78 slot 0
- [ ] `00414a90` inferred_C0042bcb0_OnAttach (vtable) · depth 1 · 92 lines · **untouched** — unread; first seen in vtable 0042bcb0 slot 0
- [ ] `00414d50` inferred_C0042abb0_Slot15 (vtable) · depth 1 · 18 lines · **untouched** — unread; first seen in vtable 0042abb0 slot 15 (kind 6 subtype 1)
- [ ] `004145f0` inferred_C0042bc40_Update2 (vtable) · depth 3 · 8 lines · **untouched** — unread; first seen in vtable 0042bc40 slot 5
- [ ] `00414620` inferred_Obj_Kind07_Ctor · depth 3 · 19 lines · **decompiled** — 0x7c bytes, vtable 0042bc78, extends Kind00
- [ ] `00414930` inferred_Obj_Kind02_Ctor · depth 3 · 81 lines · **decompiled** — 0x78 bytes, vtable 0042bcb0, extends Kind00; builds a list of up to N children resolved by name key from the record
- [ ] `00414c40` inferred_Exit_Activate (vtable) · depth 12 · 27 lines · **decompiled** — kind 6/other exit-style handler: when Jim is idle, resets display queues, looks up the destination level in the level table 00442130 by index (+0xa0) and name key (+0xa4,+0xa8), and moves Jim there with Jim_EnterLevel
- [ ] `00414dc0` inferred_C0042abb0_Slot28 (vtable) · depth 12 · 12 lines · **untouched** — unread; first seen in vtable 0042abb0 slot 28 (kind 6 subtype 1)
- [ ] `00414be0` inferred_Obj_Kind06_Ctor · depth 13 · 16 lines · **decompiled** — 0xb0 bytes, vtable 0042bce8, extends Kind09; 10 per-subtype vtables chosen by record byte 1

### Cluster 36: 00414e80–00415440 (0/12 ported)

- [ ] `00414e80` inferred_WalkArea_NextWaypoint (vtable) · depth 0 · 31 lines · **decompiled** — vtable slot 13: runs each time Jim reaches a vertex. Steps the vertex index +0xa0 one towards the destination index +0xa4 and sends Jim to that vertex (Jim slot 32, target = this); when the indices are equal first sends Jim to the real destination (+0xa8,+0xac) with the original target (+0xb0)
- [ ] `00414f10` inferred_WalkArea_FindPath · depth 0 · 73 lines · **decompiled** — (x, y, target) picks the nearest waypoint of the walk area to the destination and prepares a route; returns whether a route exists | (x, y, target) finds the polyline vertex nearest the destination (+0xa4) and the vertex nearest Jim (+0xa0), stores the destination (+0xa8,+0xac) and target (+0xb0), then sends Jim to his nearest vertex; always returns true when the polyline has vertices. No search algorithm: movement is along the vertex list
- [ ] `00415120` inferred_C0042bdd0_IsHitter (vtable) · depth 0 · 7 lines · **untouched** — unread; first seen in vtable 0042bdd0 slot 11
- [ ] `00415130` inferred_C0042bdd0_OnTouched (vtable) · depth 0 · 43 lines · **untouched** — unread; first seen in vtable 0042bdd0 slot 12
- [ ] `00415200` inferred_List_InsertAtHead · depth 0 · 23 lines · **decompiled** — allocates a node and links it at the list head
- [ ] `00415230` inferred_TopObject_Ctor · depth 1 · 9 lines · **decompiled** — base object with vtable 0042be08, inserted with priority 100 (frontmost)
- [ ] `00415290` inferred_C0042be40_OnAttach (vtable) · depth 1 · 27 lines · **untouched** — unread; first seen in vtable 0042be40 slot 0
- [ ] `004150d0` inferred_Obj_Kind14_Ctor · depth 3 · 17 lines · **decompiled** — 0x74 bytes, vtable 0042bdd0, extends Kind00; inserts itself in its room list
- [ ] `00415350` inferred_C0042be40_IsInteractive (vtable) · depth 3 · 40 lines · **untouched** — unread; first seen in vtable 0042be40 slot 16
- [ ] `00415440` inferred_C0042be40_SetVerbAndClick (vtable) · depth 3 · 17 lines · **untouched** — unread; first seen in vtable 0042be40 slot 14
- [ ] `00414e00` inferred_WalkArea_Ctor · depth 4 · 13 lines · **decompiled** — 0xb4 bytes, vtable 0042bd60, extends Obj_MidBase | kind 8, 0xb4 bytes, vtable 0042bd60 (only slot 13 overridden), extends MidBase; starts the single-frame animation whose frame record holds the vertex list. Rename of inferred_Obj_Kind08_Ctor
- [ ] `00415250` inferred_Obj_Kind11_Ctor · depth 4 · 10 lines · **decompiled** — 0xbc bytes, vtable 0042be40, extends MidBase

### Cluster 37: 004157c0–00415960 (0/8 ported)

- [ ] `004157c0` inferred_C0042beb0_SetVerbAndClick (vtable) · depth 0 · 7 lines · **untouched** — unread; first seen in vtable 0042beb0 slot 14
- [ ] `004157b0` inferred_C0042b9c0_OnAttach (vtable) · depth 1 · 8 lines · **untouched** — unread; first seen in vtable 0042b9c0 slot 0
- [ ] `004157d0` inferred_MainWndProc · depth 2 · 34 lines · **understood** — WM_ACTIVATEAPP (0x1c): clock suspend/resume and restores the two display surfaces via vtable +0x6c; MM_MCINOTIFY (0x3b9) success: CD audio notify handler 00403dc0
- [ ] `00415490` inferred_C0042be40_ShowHoverText (vtable) · depth 3 · 85 lines · **untouched** — unread; first seen in vtable 0042be40 slot 17
- [ ] `00415640` inferred_C0042be40_ClearHoverText (vtable) · depth 3 · 64 lines · **untouched** — unread; first seen in vtable 0042be40 slot 18
- [ ] `00415770` inferred_Obj_Kind15_Ctor · depth 4 · 11 lines · **decompiled** — 0xa0 bytes, vtable 0042beb0, extends MidBase, sort value +0x5e = 0x32
- [ ] `00415880` inferred_CreateMainWindow · depth 7 · 37 lines · **understood** — RegisterClass + CreateWindowEx 640x480 (style 0x90000000, ex 8), ShowWindow, ShowCursor(0), sets init flags, calls InitSubsystems
- [ ] `00415960` inferred_WinMain · depth 17 · 89 lines · **understood** — args: none=CD check+SETUP.EXE; '_$_$_ '+byte=game; 'Uninstall'

### Cluster 38: 00415c30–00416170 (1/12 ported)

- [ ] `00415c30` inferred_SetInitFlags · depth 0 · 11 lines · **understood** — sets globals 00442208 and 0044221c to 1
- [x] `00415c40` inferred_NameHash64 · depth 0 · 44 lines · **emu-compared** → `core/namekey.cpp::nameKey64` — packs up to 12 chars of a name, 5 bits each (chars below '5' get +11), into a 64-bit key; used with level object lookup
- [ ] `00415cb0` inferred_NameHash64_Decode · depth 0 · 55 lines · **decompiled** — (lo, hi, buffer) rebuilds a readable name from a 64-bit key; used for error text
- [ ] `00415b00` inferred_FindCdAndSetup · depth 10 · 71 lines · **understood** — scans drives for label TESORO, checks SETUP.EXE
- [ ] `00415d90` inferred_C00429410_UseItem (vtable) · depth 13 · 25 lines · **untouched** — unread; first seen in vtable 00429410 slot 27 (kind 20 subtype 11)
- [ ] `00415d20` inferred_C0042af70_Verb0 (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 0042af70 slot 20 (kind 6 subtype 9)
- [ ] `00415e30` inferred_C00429100_UseItem (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00429100 slot 27 (kind 20 subtype 19)
- [ ] `00415e90` inferred_C0042a050_Verb1 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 0042a050 slot 21 (kind 17 subtype 0)
- [ ] `00415f10` inferred_C0042a050_Verb3 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 0042a050 slot 22 (kind 17 subtype 0)
- [ ] `00415f90` inferred_C0042aca0_Verb0 (vtable) · depth 14 · 32 lines · **untouched** — unread; first seen in vtable 0042aca0 slot 20 (kind 6 subtype 3)
- [ ] `00416030` inferred_C0042aca0_Slot28 (vtable) · depth 14 · 42 lines · **untouched** — unread; first seen in vtable 0042aca0 slot 28 (kind 6 subtype 3)
- [ ] `00416170` inferred_C00429950_Verb4 (vtable) · depth 14 · 30 lines · **untouched** — unread; first seen in vtable 00429950 slot 24 (kind 17 subtype 16)

### Cluster 39: 00416300–004169a0 (0/12 ported)

- [ ] `00416300` inferred_C004289d8_Slot28 (vtable) · depth 12 · 86 lines · **untouched** — unread; first seen in vtable 004289d8 slot 28
- [ ] `004169a0` inferred_C004288e8_Slot28 (vtable) · depth 12 · 33 lines · **untouched** — unread; first seen in vtable 004288e8 slot 28
- [ ] `00416220` inferred_C00429950_UseItem (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00429950 slot 27 (kind 17 subtype 16)
- [ ] `00416280` inferred_C00429db0_Verb0 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 00429db0 slot 20 (kind 17 subtype 6)
- [ ] `00416540` inferred_C0042b5b0_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042b5b0 slot 23 (kind 5 subtype 7)
- [ ] `00416590` inferred_C0042b5b0_UseItem (vtable) · depth 14 · 41 lines · **untouched** — unread; first seen in vtable 0042b5b0 slot 27 (kind 5 subtype 7)
- [ ] `00416680` inferred_C0042b528_UseItem (vtable) · depth 14 · 31 lines · **untouched** — unread; first seen in vtable 0042b528 slot 27 (kind 5 subtype 8)
- [ ] `00416700` inferred_C0042b528_Verb2 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042b528 slot 23 (kind 5 subtype 8)
- [ ] `00416750` inferred_C0042b528_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042b528 slot 21 (kind 5 subtype 8)
- [ ] `004167a0` inferred_C00429b10_UseItem (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00429b10 slot 27 (kind 17 subtype 11)
- [ ] `00416800` inferred_C0042bbd0_RunVerb (vtable) · depth 14 · 43 lines · **untouched** — unread; first seen in vtable 0042bbd0 slot 13
- [ ] `00416910` inferred_C0042b6c0_UseItem (vtable) · depth 14 · 31 lines · **untouched** — unread; first seen in vtable 0042b6c0 slot 27 (kind 5 subtype 5)

### Cluster 40: 00416b50–00417250 (0/9 ported)

- [ ] `00416b50` inferred_C004280f0_Slot28 (vtable) · depth 2 · 15 lines · **untouched** — unread; first seen in vtable 004280f0 slot 28
- [ ] `00416b80` inferred_C00428258_Slot28 (vtable) · depth 12 · 45 lines · **untouched** — unread; first seen in vtable 00428258 slot 28
- [ ] `00416c80` inferred_C004281e0_Slot28 (vtable) · depth 12 · 71 lines · **untouched** — unread; first seen in vtable 004281e0 slot 28
- [ ] `00416a20` inferred_C0042b280_Verb2 (vtable) · depth 14 · 26 lines · **untouched** — unread; first seen in vtable 0042b280 slot 23 (kind 5 subtype 0)
- [ ] `00416ac0` inferred_C0042b280_UseItem (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 0042b280 slot 27 (kind 5 subtype 0)
- [ ] `00416e30` inferred_C00428d10_UseItem (vtable) · depth 14 · 35 lines · **untouched** — unread; first seen in vtable 00428d10 slot 27 (kind 20 subtype 28)
- [ ] `00416ee0` inferred_C0042a600_UseItem (vtable) · depth 14 · 106 lines · **untouched** — unread; first seen in vtable 0042a600 slot 27 (kind 13 subtype 16)
- [ ] `00417180` inferred_C0042a520_Verb4 (vtable) · depth 14 · 34 lines · **untouched** — unread; first seen in vtable 0042a520 slot 24 (kind 13 subtype 9)
- [ ] `00417250` inferred_C0042a440_UseItem (vtable) · depth 14 · 47 lines · **untouched** — unread; first seen in vtable 0042a440 slot 27 (kind 13 subtype 28)

### Cluster 41: 00417360–004178b0 (0/9 ported)

- [ ] `00417360` inferred_C0042a360_UseItem (vtable) · depth 2 · 22 lines · **untouched** — unread; first seen in vtable 0042a360 slot 27 (kind 13 subtype 36)
- [ ] `004174b0` inferred_C0042a210_UseItem (vtable) · depth 2 · 22 lines · **untouched** — unread; first seen in vtable 0042a210 slot 27 (kind 13 subtype 33)
- [ ] `00417530` inferred_C0042a130_UseItem (vtable) · depth 2 · 22 lines · **untouched** — unread; first seen in vtable 0042a130 slot 27 (kind 13 subtype 42)
- [ ] `00417720` inferred_Obj_Kind19_Ctor · depth 12 · 63 lines · **decompiled** — 0xa4 bytes, vtable 0042bf90; builds a temporary Kind00 and copies it field by field into this
- [ ] `004175b0` inferred_Obj_Kind24_Ctor · depth 13 · 10 lines · **decompiled** — 0xa4 bytes, vtable 0042bf20, extends Kind09
- [ ] `004173e0` inferred_C0042a2f0_UseItem (vtable) · depth 14 · 35 lines · **untouched** — unread; first seen in vtable 0042a2f0 slot 27 (kind 13 subtype 37)
- [ ] `004175f0` inferred_C0042bf20_Verb1 (vtable) · depth 14 · 19 lines · **untouched** — unread; first seen in vtable 0042bf20 slot 21
- [ ] `00417640` inferred_C0042afe8_Slot28 (vtable) · depth 14 · 44 lines · **untouched** — unread; first seen in vtable 0042afe8 slot 28 (kind 6 subtype 10)
- [ ] `004178b0` inferred_C0042bf90_SetVerbAndClick (vtable) · depth 14 · 37 lines · **untouched** — unread; first seen in vtable 0042bf90 slot 14

### Cluster 42: 00417b30–00417d00 (0/7 ported)

- [ ] `00417b30` inferred_Obj_RetTrue (vtable) · depth 0 · 7 lines · **decompiled** — MidBase slot 16: returns 1 (is interactive/enabled default)
- [ ] `004179b0` inferred_C0042bc08_RunVerb (vtable) · depth 2 · 41 lines · **untouched** — unread; first seen in vtable 0042bc08 slot 13
- [ ] `00417c80` inferred_C004282d0_Slot28 (vtable) · depth 2 · 10 lines · **untouched** — unread; first seen in vtable 004282d0 slot 28
- [ ] `00417ad0` inferred_Obj_Kind25_Ctor · depth 4 · 12 lines · **decompiled** — 0xa4 bytes, vtable 0042c000, extends MidBase
- [ ] `00417ca0` inferred_C004295d0_UseItem (vtable) · depth 13 · 21 lines · **untouched** — unread; first seen in vtable 004295d0 slot 27 (kind 20 subtype 7)
- [ ] `00417d00` inferred_C00429560_UseItem (vtable) · depth 13 · 31 lines · **untouched** — unread; first seen in vtable 00429560 slot 27 (kind 20 subtype 8)
- [ ] `00417b40` inferred_C0042c000_SetVerbAndClick (vtable) · depth 15 · 49 lines · **untouched** — unread; first seen in vtable 0042c000 slot 14

### Cluster 43: 00417dd0–00418560 (0/12 ported)

- [ ] `00417dd0` inferred_C0042ae08_Verb0 (vtable) · depth 13 · 10 lines · **untouched** — unread; first seen in vtable 0042ae08 slot 20 (kind 6 subtype 6)
- [ ] `004180b0` inferred_C00429870_UseItem (vtable) · depth 13 · 21 lines · **untouched** — unread; first seen in vtable 00429870 slot 27 (kind 20 subtype 1)
- [ ] `00418110` inferred_C004292c0_UseItem (vtable) · depth 13 · 31 lines · **untouched** — unread; first seen in vtable 004292c0 slot 27 (kind 20 subtype 16)
- [ ] `00417de0` inferred_C004298e0_UseItem (vtable) · depth 14 · 32 lines · **untouched** — unread; first seen in vtable 004298e0 slot 27 (kind 20 subtype 0)
- [ ] `00417e90` inferred_C00429800_UseItem (vtable) · depth 14 · 69 lines · **untouched** — unread; first seen in vtable 00429800 slot 27 (kind 20 subtype 2)
- [ ] `00418050` inferred_C0042ad90_Verb0 (vtable) · depth 14 · 24 lines · **untouched** — unread; first seen in vtable 0042ad90 slot 20 (kind 6 subtype 5)
- [ ] `004181e0` inferred_C0042b170_UseItem (vtable) · depth 14 · 35 lines · **untouched** — unread; first seen in vtable 0042b170 slot 27 (kind 5 subtype 14)
- [ ] `004182c0` inferred_C0042b1f8_UseItem (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 0042b1f8 slot 27 (kind 5 subtype 13)
- [ ] `00418320` inferred_C00429250_UseItem (vtable) · depth 14 · 33 lines · **untouched** — unread; first seen in vtable 00429250 slot 27 (kind 20 subtype 14)
- [ ] `004183e0` inferred_C0042b1f8_Verb2 (vtable) · depth 14 · 40 lines · **untouched** — unread; first seen in vtable 0042b1f8 slot 23 (kind 5 subtype 13)
- [ ] `004184e0` inferred_C0042b1f8_Verb1 (vtable) · depth 14 · 25 lines · **untouched** — unread; first seen in vtable 0042b1f8 slot 21 (kind 5 subtype 13)
- [ ] `00418560` inferred_C0042ac28_Verb0 (vtable) · depth 14 · 27 lines · **untouched** — unread; first seen in vtable 0042ac28 slot 20 (kind 6 subtype 2)

### Cluster 44: 004185c0–00418b80 (0/8 ported)

- [ ] `004185c0` inferred_C0042b4a0_Verb2 (vtable) · depth 14 · 34 lines · **untouched** — unread; first seen in vtable 0042b4a0 slot 23 (kind 5 subtype 1)
- [ ] `00418670` inferred_C0042b4a0_UseItem (vtable) · depth 14 · 54 lines · **untouched** — unread; first seen in vtable 0042b4a0 slot 27 (kind 5 subtype 1)
- [ ] `00418780` inferred_C00429020_UseItem (vtable) · depth 14 · 23 lines · **untouched** — unread; first seen in vtable 00429020 slot 27 (kind 20 subtype 21)
- [ ] `004187e0` inferred_C0042b308_Verb2 (vtable) · depth 14 · 25 lines · **untouched** — unread; first seen in vtable 0042b308 slot 23 (kind 5 subtype 2)
- [ ] `00418860` inferred_C0042b308_UseItem (vtable) · depth 14 · 50 lines · **untouched** — unread; first seen in vtable 0042b308 slot 27 (kind 5 subtype 2)
- [ ] `00418990` inferred_C0042b418_Verb2 (vtable) · depth 14 · 38 lines · **untouched** — unread; first seen in vtable 0042b418 slot 23 (kind 5 subtype 4)
- [ ] `00418aa0` inferred_C0042b418_UseItem (vtable) · depth 14 · 44 lines · **untouched** — unread; first seen in vtable 0042b418 slot 27 (kind 5 subtype 4)
- [ ] `00418b80` inferred_C0042abb0_Verb0 (vtable) · depth 14 · 34 lines · **untouched** — unread; first seen in vtable 0042abb0 slot 20 (kind 6 subtype 1)

### Cluster 45: 00418d20–00419050 (0/7 ported)

- [ ] `00418d20` inferred_Connect4_ScoreMove · depth 0 · 76 lines · **untouched** — scores a move on a 7-column board (columns hold up to 7 pieces, lines of 4): hypothesis for a board mini-game | name from call sites and a skim; body not yet read
- [ ] `00419050` inferred_Connect4_FindBestMoves · depth 1 · 74 lines · **untouched** — tries each non-full column, collects the best-scoring columns | name from call sites and a skim; body not yet read
- [ ] `00418c80` inferred_C0042b390_Slot28 (vtable) · depth 2 · 16 lines · **untouched** — unread; first seen in vtable 0042b390 slot 28 (kind 5 subtype 3)
- [ ] `00418c50` inferred_C0042b390_Slot30 (vtable) · depth 3 · 16 lines · **untouched** — unread; first seen in vtable 0042b390 slot 30 (kind 5 subtype 3)
- [ ] `00418e90` inferred_BoardGame_BlinkUpdate · depth 3 · 97 lines · **untouched** — toggles a blink flag every 200 ms of game time and updates board objects; hypothesis | name from call sites and a skim; body not yet read
- [ ] `00418c30` inferred_C0042b390_Draw (vtable) · depth 4 · 10 lines · **untouched** — unread; first seen in vtable 0042b390 slot 6 (kind 5 subtype 3)
- [ ] `00418cb0` inferred_C0042a750_UseItem (vtable) · depth 14 · 30 lines · **untouched** — unread; first seen in vtable 0042a750 slot 27 (kind 12 subtype 8)

### Cluster 46: 004191c0–00419ac0 (0/12 ported)

- [ ] `004191c0` inferred_Connect4_ChooseMove · depth 2 · 46 lines · **untouched** — picks a column (checks for immediate win/loss first, then best-scoring, using RandomRange to break ties) | name from call sites and a skim; body not yet read
- [ ] `00419340` inferred_C0042c070_SetVerbAndClick (vtable) · depth 4 · 23 lines · **untouched** — unread; first seen in vtable 0042c070 slot 14
- [ ] `004192c0` inferred_Obj_Kind21_Ctor · depth 12 · 24 lines · **decompiled** — 0xa4 bytes, vtable 0042c070, extends MidBase; builds a phrase key from a base string plus a digit from the record
- [ ] `004193b0` inferred_C00428438_Slot28 (vtable) · depth 12 · 69 lines · **untouched** — unread; first seen in vtable 00428438 slot 28
- [ ] `004197a0` inferred_C00428a50_Slot28 (vtable) · depth 12 · 51 lines · **untouched** — unread; first seen in vtable 00428a50 slot 28
- [ ] `004198d0` inferred_C00428960_Slot28 (vtable) · depth 12 · 37 lines · **untouched** — unread; first seen in vtable 00428960 slot 28
- [ ] `004199a0` inferred_C00428870_Slot28 (vtable) · depth 12 · 21 lines · **untouched** — unread; first seen in vtable 00428870 slot 28
- [ ] `00419a10` inferred_C004287f8_Slot28 (vtable) · depth 12 · 31 lines · **untouched** — unread; first seen in vtable 004287f8 slot 28
- [ ] `00419ac0` inferred_C00428780_Slot28 (vtable) · depth 12 · 20 lines · **untouched** — unread; first seen in vtable 00428780 slot 28
- [ ] `00419530` inferred_C00428708_Slot28 (vtable) · depth 14 · 43 lines · **untouched** — unread; first seen in vtable 00428708 slot 28
- [ ] `00419620` inferred_C0042b060_Verb2 (vtable) · depth 14 · 29 lines · **untouched** — unread; first seen in vtable 0042b060 slot 23 (kind 5 subtype 16)
- [ ] `004196c0` inferred_C0042b0e8_Verb2 (vtable) · depth 14 · 30 lines · **untouched** — unread; first seen in vtable 0042b0e8 slot 23 (kind 5 subtype 15)

### Cluster 47: 00419b30–00419f10 (0/6 ported)

- [ ] `00419b30` inferred_C00428690_Slot28 (vtable) · depth 12 · 17 lines · **untouched** — unread; first seen in vtable 00428690 slot 28
- [ ] `00419b90` inferred_C00428528_Slot28 (vtable) · depth 12 · 68 lines · **untouched** — unread; first seen in vtable 00428528 slot 28
- [ ] `00419cf0` inferred_C004283c0_Slot28 (vtable) · depth 12 · 16 lines · **untouched** — unread; first seen in vtable 004283c0 slot 28
- [ ] `00419d40` inferred_C00428348_Slot28 (vtable) · depth 12 · 16 lines · **untouched** — unread; first seen in vtable 00428348 slot 28
- [ ] `00419d90` inferred_C00428618_Slot28 (vtable) · depth 12 · 74 lines · **untouched** — unread; first seen in vtable 00428618 slot 28
- [ ] `00419f10` inferred_DlgScript_EnterStageLevel (vtable) · depth 12 · 57 lines · **decompiled** — (0 or 1): moves Jim into a fixed stage level (globals 00442188 / 00442154), repositions the camera and re-resolves the script's actor table there

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
- [x] `0041c3b0` inferred__execl · runtime · **replaced**
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
