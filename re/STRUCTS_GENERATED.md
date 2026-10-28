# Generated class model (do not edit; produced by `re/tools/build_classes.py`)
Source: constructors/vtable stores found in the fresh decompiles, parent = vtable stored by the first constructor the class calls, fields = `this` accesses in the class's constructors and **own** virtual methods (slots not inherited unchanged).
Confidence: `c` written in a constructor · `w` written and read · `r` read only. Widths are the decompiler's access widths, a field accessed with two widths appears twice. Offsets inside the parent's range belong to the parent.
152 classes.

## Inheritance tree (class (vtable) x number of direct subclasses)

```
Obj_Kind00 (00428000)  [6 leaf classes]
  Level (00428bf8)  [3 leaf classes]
  Obj_Kind03 (0042b9c0)  [3 leaf classes]
  Obj_MidBase (0042ba68)  [7 leaf classes]
    DlgScriptEntry (00428078)  [21 leaf classes]
    Obj_Kind13Base (00428b00)  [13 leaf classes]
    Obj_Kind09Family (0042bad8)  [60 leaf classes]
      Obj_Kind06Base (0042bce8)  [10 leaf classes]
    Obj_Kind05Family (0042bb48)  [17 leaf classes]
InputDevice (0042806c)  [2 leaf classes]
```

## Obj_Kind00  (vtable 00428000)
- parent: none found · size: 0x6c · slots: 13 · constructors: inferred_Obj_Kind00_Ctor, inferred_Object_BaseCtor
- own methods: [0 OnAttach] inferred_Noop, [1] inferred_Noop, [2 AnimTick] inferred_Obj_AnimTick, [3 UpdateBounds] inferred_Obj_UpdateBounds, [4 Update] inferred_Obj_Stub_RetZero, [5 Update2] inferred_Obj_Stub_RetZero, [6 Draw] inferred_Obj_Stub_Void_1Arg, [7] inferred_Noop, [8 SetActive] inferred_Obj_SetActive, [9 LoadState] inferred_Obj_LoadState, [10 SaveState] inferred_Obj_SaveState, [11 IsHitter] inferred_Obj_Stub_RetFalse_1Arg, [12 OnTouched] inferred_Obj_Stub_RetZero_2Args

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x10 boundsLeft? | 4 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x14 boundsTop? | 4 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x18 boundsRight? | 4 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x1c boundsBottom? | 4 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x20 screenLeft? | 4 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x24 screenTop? | 4 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x28 screenRight? | 4 | w? | 0/2 | inferred_Obj_UpdateBounds |
| +0x2c screenBottom? | 4 | w? | 0/2 | inferred_Obj_UpdateBounds |
| +0x30 | 2 | w? | 0/1 | inferred_Obj_UpdateBounds |
| +0x32 bankIndex | 1 | c | 1/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_LoadState |
| +0x33 bankCount | 1 | c | 2/1 | inferred_Obj_Kind00_Ctor |
| +0x34 banks | 4 | c | 4/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_LoadState |
| +0x38 anim | 4 | c | 10/2 | inferred_Obj_Kind00_Ctor, inferred_Obj_AnimTick, inferred_Obj_UpdateBounds… |
| +0x3c room | 4 | c | 0/1 | inferred_Obj_Kind00_Ctor |
| +0x44 nameKey | 8 | c | 0/1 | inferred_Obj_Kind00_Ctor |
| +0x4c x | 4 | c | 4/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_UpdateBounds |
| +0x50 y | 4 | c | 4/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_UpdateBounds |
| +0x54 prevX | 4 | c | 2/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_UpdateBounds |
| +0x58 prevY | 4 | c | 2/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_UpdateBounds |
| +0x5c state | 1 | c | 0/1 | inferred_Object_BaseCtor |
| +0x5d | 1 | c | 0/1 | inferred_Obj_Kind00_Ctor |
| +0x5e sortValue | 2 | c | 0/2 | inferred_Obj_Kind00_Ctor, inferred_Object_BaseCtor |
| +0x60 stateBlob | 4 | c | 4/2 | inferred_Obj_Kind00_Ctor, inferred_Obj_LoadState, inferred_Obj_SaveState |
| +0x64 stateBlobSize | 2 | c | 2/1 | inferred_Obj_Kind00_Ctor, inferred_Obj_LoadState, inferred_Obj_SaveState |
| +0x68 flags | 4 | c | 7/5 | inferred_Obj_Kind00_Ctor, inferred_Object_BaseCtor, inferred_Obj_AnimTick… |

## InputDevice  (vtable 0042806c)
- parent: none found · size: unknown · slots: 1 · constructors: inferred_InputDevice_Ctor, inferred_InputDevice_Dtor

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x04 | 4 | c | 3/1 | inferred_InputDevice_Dtor |

## Keyboard  (vtable 00428070)
- parent: InputDevice (0042806c) · size: unknown · slots: 1 · constructors: inferred_Keyboard_Ctor, inferred_Keyboard_Dtor
- own methods: [0] inferred_C00428070_Slot00

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x04 | 4 | r | 6/0 | inferred_C00428070_Slot00 |
| +0x0c | 4 | c | 0/1 | inferred_Keyboard_Ctor |
| +0x10 | 4 | c | 0/1 | inferred_Keyboard_Ctor |
| +0x14 | 4 | c | 0/1 | inferred_Keyboard_Ctor |
| +0x18 | 4 | c | 2/1 | inferred_Keyboard_Ctor |
| +0x1c | 4 | c | 1/1 | inferred_Keyboard_Ctor |
| +0x20 | 4 | c | 0/1 | inferred_Keyboard_Ctor |
| +0x24 | 1 | c | 0/1 | inferred_Keyboard_Ctor |
| +0x30 | 4 | c | 1/1 | inferred_Keyboard_Ctor, inferred_C00428070_Slot00 |
| +0x34 | 4 | c | 0/1 | inferred_Keyboard_Ctor |

## Mouse  (vtable 00428074)
- parent: InputDevice (0042806c) · size: unknown · slots: 1 · constructors: inferred_MouseCtor, inferred_Mouse_Dtor
- own methods: [0] inferred_C00428074_Slot00

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x04 | 4 | r | 6/0 | inferred_C00428074_Slot00 |
| +0x0c | 4 | c | 0/1 | inferred_MouseCtor |
| +0x10 | 4 | c | 0/1 | inferred_MouseCtor |
| +0x14 | 4 | c | 0/1 | inferred_MouseCtor |
| +0x18 | 4 | c | 3/1 | inferred_MouseCtor, inferred_C00428074_Slot00 |
| +0x1c | 4 | c | 0/1 | inferred_MouseCtor |
| +0x20 | 4 | c | 3/1 | inferred_MouseCtor, inferred_C00428074_Slot00 |
| +0x24 | 4 | c | 3/1 | inferred_MouseCtor, inferred_C00428074_Slot00 |
| +0x28 | 4 | c | 3/1 | inferred_MouseCtor, inferred_C00428074_Slot00 |
| +0x2c | 4 | c | 3/1 | inferred_MouseCtor, inferred_C00428074_Slot00 |
| +0x30 | 4 | w? | 0/1 | inferred_C00428074_Slot00 |
| +0x34 | 4 | w? | 0/1 | inferred_C00428074_Slot00 |
| +0x38 | 4 | c | 2/4 | inferred_MouseCtor, inferred_C00428074_Slot00 |
| +0x3c | 4 | c | 2/4 | inferred_MouseCtor, inferred_C00428074_Slot00 |

## Obj_Kind01  (vtable 00428ac8)
- parent: Obj_Kind00 (00428000) · size: 0x84 · slots: 13 · constructors: inferred_Obj_Kind01_Ctor
- own methods: [0 OnAttach] inferred_Obj_Kind01_OnAttach, [4 Update] inferred_Obj_RetTrue

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 1/0 | inferred_Obj_Kind01_OnAttach |
| +0x40 level | 4 | r | 3/0 | inferred_Obj_Kind01_OnAttach |
| +0x4c x | 4 | r | 2/0 | inferred_Obj_Kind01_OnAttach |
| +0x50 y | 4 | r | 2/0 | inferred_Obj_Kind01_OnAttach |
| +0x6c | 1 | r | 1/0 | inferred_Obj_Kind01_OnAttach |
| +0x6c | 4 | c | 0/1 | inferred_Obj_Kind01_Ctor |
| +0x70 | 4 | c | 0/1 | inferred_Obj_Kind01_Ctor |
| +0x74 | 4 | c | 0/1 | inferred_Obj_Kind01_Ctor |
| +0x78 | 4 | w? | 0/2 | inferred_Obj_Kind01_OnAttach |
| +0x7d | 1 | c | 0/1 | inferred_Obj_Kind01_Ctor |
| +0x7e | 2 | c | 0/1 | inferred_Obj_Kind01_Ctor |
| +0x80 | 2 | c | 0/1 | inferred_Obj_Kind01_Ctor |

## Level  (vtable 00428bf8)
- parent: Obj_Kind00 (00428000) · size: 0x9c · slots: 14 · constructors: inferred_Level_Load
- own methods: [4 Update] inferred_Level_UpdateOrCollide, [5 Update2] inferred_C00428bf8_Update2, [13 RunVerb] inferred_C00428bf8_RunVerb

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x94 reloadAnimsFlag | 1 | w? | 0/1 | inferred_Level_UpdateOrCollide |
| +0x94 reloadAnimsFlag | 4 | r | 1/0 | inferred_Level_UpdateOrCollide |

## Obj_Kind03  (vtable 0042b9c0)
- parent: Obj_Kind00 (00428000) · size: 0x6c · slots: 13 · constructors: inferred_Obj_Kind03Base_DefaultCtor, inferred_Obj_Kind03_Ctor
- own methods: [0 OnAttach] inferred_C0042b9c0_OnAttach, [2 AnimTick] inferred_C00428038_AnimTick, [6 Draw] inferred_C0042b9c0_Draw

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 5/0 | inferred_Obj_Kind03_Ctor, inferred_C00428038_AnimTick, inferred_C0042b9c0_Draw |
| +0x3c room | 4 | r | 1/0 | inferred_C0042b9c0_OnAttach |
| +0x4c x | 4 | r | 1/0 | inferred_C0042b9c0_Draw |
| +0x50 y | 4 | r | 1/0 | inferred_C0042b9c0_Draw |
| +0x5e sortValue | 2 | r | 1/0 | inferred_C0042b9c0_Draw |
| +0x68 flags | 4 | c | 1/2 | inferred_Obj_Kind03_Ctor |

## Obj_MidBase  (vtable 0042ba68)
- parent: Obj_Kind00 (00428000) · size: unknown · slots: 28 · constructors: inferred_Obj_MidBase_DefaultCtor, inferred_Obj_MidBase_Ctor, inferred_Obj_Kind09_Ctor
- own methods: [2 AnimTick] inferred_Obj_AnimTickResetOffset, [4 Update] inferred_Obj_Interactive_Update, [6 Draw] inferred_Obj_Draw, [13 RunVerb] inferred_Obj_RunVerb, [14 SetVerbAndClick] inferred_Obj_SetVerbAndClick, [15] inferred_Obj_Slot15_ForwardToSlot14, [16 IsInteractive] inferred_Obj_RetTrue, [17 ShowHoverText] inferred_Obj_ShowHoverText, [18 ClearHoverText] inferred_Obj_ClearHoverText, [19 OnClick] inferred_Obj_OnClick, [20 Verb0] inferred_Obj_Verb0_Default, [21 Verb1] inferred_Obj_Stub_RetZero, [22 Verb3] inferred_Obj_Stub_RetZero, [23 Verb2] inferred_Obj_Stub_RetZero, [24 Verb4] inferred_Obj_Stub_RetZero, [25 Verb5] inferred_Obj_Stub_RetZero, [26 Verb6] inferred_Obj_Stub_RetZero, [27 UseItem] inferred_Obj_Slot27_Stub

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 7/0 | inferred_Obj_AnimTickResetOffset, inferred_Obj_MidBase_Ctor, inferred_Obj_Kind09_Ctor… |
| +0x44 nameKey | 4 | r | 1/0 | inferred_Obj_ShowHoverText |
| +0x48 | 4 | r | 1/0 | inferred_Obj_ShowHoverText |
| +0x4c x | 4 | r | 2/0 | inferred_Obj_Kind09_Ctor, inferred_Obj_Draw |
| +0x50 y | 4 | r | 2/0 | inferred_Obj_Kind09_Ctor, inferred_Obj_Draw |
| +0x5e sortValue | 2 | r | 1/0 | inferred_Obj_Draw |
| +0x68 flags | 4 | c | 4/2 | inferred_Obj_MidBase_Ctor, inferred_Obj_Kind09_Ctor |
| +0x6c | 4 | c | 2/2 | inferred_Obj_Kind09_Ctor, inferred_Obj_ShowHoverText |
| +0x70 | 4 | r | 2/0 | inferred_Obj_Verb0_Default |
| +0x8c | 1 | w? | 0/2 | inferred_Obj_Interactive_Update, inferred_Obj_Verb0_Default |
| +0x8c | 4 | r | 1/0 | inferred_Obj_Interactive_Update |
| +0x8d | 1 | w | 1/2 | inferred_Obj_Interactive_Update, inferred_Obj_Verb0_Default |
| +0x90 | 4 | c | 1/2 | inferred_Obj_Kind09_Ctor, inferred_Obj_RunVerb, inferred_Obj_OnClick |
| +0x94 | 4 | c | 2/2 | inferred_Obj_Kind09_Ctor, inferred_Obj_OnClick |
| +0x98 | 4 | c | 2/2 | inferred_Obj_Kind09_Ctor, inferred_Obj_OnClick |
| +0x9c | 4 | w | 4/1 | inferred_Obj_Interactive_Update, inferred_Obj_SetVerbAndClick, inferred_Obj_RunVerb |

## Obj_Kind07  (vtable 0042bc78)
- parent: Obj_Kind00 (00428000) · size: 0x7c · slots: 13 · constructors: inferred_Obj_Kind07_Ctor
- own methods: [0 OnAttach] inferred_C0042bc78_OnAttach

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x34 banks | 4 | w? | 0/1 | inferred_C0042bc78_OnAttach |
| +0x38 anim | 4 | w | 2/1 | inferred_Obj_Kind07_Ctor, inferred_C0042bc78_OnAttach |
| +0x40 level | 4 | r | 3/0 | inferred_C0042bc78_OnAttach |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind07_Ctor |
| +0x6c | 4 | c | 0/1 | inferred_Obj_Kind07_Ctor |
| +0x70 | 4 | c | 0/1 | inferred_Obj_Kind07_Ctor |
| +0x74 | 4 | c | 0/1 | inferred_Obj_Kind07_Ctor |
| +0x78 | 4 | w? | 0/1 | inferred_C0042bc78_OnAttach |

## Obj_Kind02  (vtable 0042bcb0)
- parent: Obj_Kind00 (00428000) · size: 0x78 · slots: 13 · constructors: inferred_Obj_Kind02_Ctor
- own methods: [0 OnAttach] inferred_C0042bcb0_OnAttach

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042bcb0_OnAttach |
| +0x6c | 4 | c | 5/1 | inferred_Obj_Kind02_Ctor, inferred_C0042bcb0_OnAttach |
| +0x70 | 1 | c | 0/1 | inferred_Obj_Kind02_Ctor |
| +0x72 | 2 | c | 6/1 | inferred_Obj_Kind02_Ctor, inferred_C0042bcb0_OnAttach |
| +0x74 | 4 | w | 1/1 | inferred_C0042bcb0_OnAttach |

## Obj_Kind14  (vtable 0042bdd0)
- parent: Obj_Kind00 (00428000) · size: 0x74 · slots: 13 · constructors: inferred_Obj_Kind14_Ctor
- own methods: [11 IsHitter] inferred_C0042bdd0_IsHitter, [12 OnTouched] inferred_C0042bdd0_OnTouched

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x3c room | 4 | r | 1/0 | inferred_Obj_Kind14_Ctor |
| +0x4c x | 4 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x50 y | 4 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x5e sortValue | 2 | r | 4/0 | inferred_C0042bdd0_OnTouched |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind14_Ctor |
| +0x6c | 1 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x6c | 4 | c | 0/1 | inferred_Obj_Kind14_Ctor |
| +0x6d | 1 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x6e | 1 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x6f | 1 | r | 1/0 | inferred_C0042bdd0_OnTouched |
| +0x70 | 1 | c | 1/1 | inferred_Obj_Kind14_Ctor, inferred_C0042bdd0_OnTouched |

## TopObject  (vtable 0042be08)
- parent: Obj_Kind00 (00428000) · size: unknown · slots: 13 · constructors: inferred_TopObject_Ctor
- own methods: [4 Update] inferred_Obj_RetTrue

## Obj_Kind19  (vtable 0042bf90)
- parent: Obj_Kind00 (00428000) · size: 0xa4 · slots: 28 · constructors: inferred_Obj_Kind19_Ctor
- own methods: [2 AnimTick] inferred_Obj_AnimTickResetOffset, [4 Update] inferred_Obj_Interactive_Update, [6 Draw] inferred_Obj_Draw, [13 RunVerb] inferred_Obj_RunVerb, [14 SetVerbAndClick] inferred_C0042bf90_SetVerbAndClick, [15] inferred_Obj_Slot15_ForwardToSlot14, [16 IsInteractive] inferred_Obj_RetTrue, [17 ShowHoverText] inferred_Obj_ShowHoverText, [18 ClearHoverText] inferred_Obj_ClearHoverText, [19 OnClick] inferred_Obj_OnClick, [20 Verb0] inferred_Obj_Verb0_Default, [21 Verb1] inferred_Obj_Verb1_Default, [22 Verb3] inferred_Obj_Verb3_Default, [23 Verb2] inferred_Obj_Verb2_Default, [24 Verb4] inferred_Obj_Verb4_Default, [25 Verb5] inferred_Obj_Stub_RetZero, [26 Verb6] inferred_Obj_Stub_RetZero, [27 UseItem] inferred_Obj_UseItem_Default

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x04 | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x08 | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x0c | 1 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x10 boundsLeft? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x14 boundsTop? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x18 boundsRight? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x1c boundsBottom? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x20 screenLeft? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x24 screenTop? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x28 screenRight? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x2c screenBottom? | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x30 | 2 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x32 bankIndex | 1 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x33 bankCount | 1 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x34 banks | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x38 anim | 4 | c | 8/1 | inferred_Obj_AnimTickResetOffset, inferred_Obj_Draw, inferred_Obj_Kind19_Ctor |
| +0x3c room | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x40 level | 4 | c | 1/1 | inferred_Obj_Kind19_Ctor, inferred_C0042bf90_SetVerbAndClick |
| +0x44 nameKey | 4 | c | 2/1 | inferred_Obj_ShowHoverText, inferred_Obj_UseItem_Default, inferred_Obj_Kind19_Ctor |
| +0x48 | 4 | c | 2/1 | inferred_Obj_ShowHoverText, inferred_Obj_UseItem_Default, inferred_Obj_Kind19_Ctor |
| +0x4c x | 4 | c | 1/1 | inferred_Obj_Draw, inferred_Obj_Kind19_Ctor |
| +0x50 y | 4 | c | 1/1 | inferred_Obj_Draw, inferred_Obj_Kind19_Ctor |
| +0x54 prevX | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x58 prevY | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x5c state | 1 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x5d | 1 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x5e sortValue | 2 | c | 1/1 | inferred_Obj_Draw, inferred_Obj_Kind19_Ctor |
| +0x60 stateBlob | 4 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x64 stateBlobSize | 2 | c | 0/1 | inferred_Obj_Kind19_Ctor |
| +0x68 flags | 4 | c | 1/2 | inferred_Obj_Kind19_Ctor |
| +0x6c | 4 | c | 2/1 | inferred_Obj_ShowHoverText, inferred_Obj_Kind19_Ctor |
| +0x70 | 4 | r | 2/0 | inferred_Obj_Verb0_Default |
| +0x74 | 4 | r | 1/0 | inferred_Obj_Verb1_Default |
| +0x78 | 4 | r | 1/0 | inferred_Obj_Verb2_Default |
| +0x7c | 4 | r | 1/0 | inferred_Obj_Verb3_Default |
| +0x80 | 4 | r | 1/0 | inferred_Obj_Verb4_Default |
| +0x8c | 1 | w? | 0/8 | inferred_Obj_Interactive_Update, inferred_Obj_Verb0_Default, inferred_Obj_Verb1_Default… |
| +0x8c | 4 | r | 1/0 | inferred_Obj_Interactive_Update |
| +0x8d | 1 | w | 1/7 | inferred_Obj_Interactive_Update, inferred_Obj_Verb0_Default, inferred_Obj_Verb1_Default… |
| +0x90 | 4 | w | 1/1 | inferred_Obj_RunVerb, inferred_Obj_OnClick |
| +0x94 | 4 | r | 1/0 | inferred_Obj_OnClick |
| +0x98 | 4 | r | 1/0 | inferred_Obj_OnClick |
| +0x9c | 4 | r | 4/0 | inferred_Obj_Interactive_Update, inferred_Obj_RunVerb |
| +0xa0 | 1 | c | 3/2 | inferred_Obj_Kind19_Ctor, inferred_C0042bf90_SetVerbAndClick |

## Cursor  (vtable 00428038)
- parent: Obj_Kind03 (0042b9c0) · size: 0x130 · slots: 13 · constructors: inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor
- own methods: [0 OnAttach] inferred_C00428038_OnAttach, [4 Update] inferred_Cursor_Update, [5 Update2] inferred_C00428038_Update2, [6 Draw] inferred_C00428038_Draw, [8 SetActive] inferred_C00428038_SetActive, [11 IsHitter] inferred_Cursor_IsFlag10, [12 OnTouched] inferred_Cursor_AddCandidate

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 5/0 | inferred_C00428038_OnAttach, inferred_C00428038_Draw, inferred_C00428038_Update2 |
| +0x3c room | 4 | r | 1/0 | inferred_C00428038_OnAttach |
| +0x4c x | 4 | r | 2/0 | inferred_C00428038_Draw |
| +0x50 y | 4 | r | 2/0 | inferred_C00428038_Draw |
| +0x5e sortValue | 2 | c | 1/2 | inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor, inferred_C00428038_Draw |
| +0x68 flags | 4 | c | 3/3 | inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor |
| +0x6c | 4 | c | 1/2 | inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor, inferred_C00428038_SetActive |
| +0x70 | 4 | c | 2/3 | inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor, inferred_Cursor_AddCandidate… |
| +0x74 | 4 | c | 10/4 | inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor, inferred_C00428038_OnAttach… |
| +0x78 hoverText | 1 | c | 1/3 | inferred_Cursor_Ctor, inferred_Obj_Kind10_Ctor, inferred_C00428038_Draw… |
| +0x118 hoverTextArg | 2 | r | 1/0 | inferred_C00428038_Draw |
| +0x11a | 2 | w | 1/2 | inferred_C00428038_OnAttach |
| +0x11c | 2 | w? | 0/1 | inferred_C00428038_OnAttach |
| +0x120 | 4 | c | 0/2 | inferred_Cursor_Ctor, inferred_C00428038_OnAttach |
| +0x128 | 4 | w? | 0/1 | inferred_C00428038_OnAttach |

## DlgScriptEntry  (vtable 00428078)
- parent: Obj_MidBase (0042ba68) · size: unknown · slots: 29 · constructors: inferred_DlgEntry_CopyCtorA, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load, inferred_DialogueTable_Load
- own methods: [4 Update] inferred_DlgScript_Update, [13 RunVerb] inferred_DlgScript_ResumePlayer, [28] inferred_Obj_Stub_Void_1Arg

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x04 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x08 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x0c | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x10 boundsLeft? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x14 boundsTop? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x18 boundsRight? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x1c boundsBottom? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x20 screenLeft? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x24 screenTop? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x28 screenRight? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x2c screenBottom? | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x30 | 2 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x32 bankIndex | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x33 bankCount | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x34 banks | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x38 anim | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x3c room | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x40 level | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x44 nameKey | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x48 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x4c x | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x50 y | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x54 prevX | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x58 prevY | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x5c state | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x5d | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x5e sortValue | 2 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x60 stateBlob | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x64 stateBlobSize | 2 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x68 flags | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x8c | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x8d | 1 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x90 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x94 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x98 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0x9c | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xa0 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xa4 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xa8 | 4 | c | 1/1 | inferred_DlgEntry_CopyCtorA, inferred_DlgScript_Update |
| +0xac | 4 | c | 4/2 | inferred_DlgEntry_CopyCtorA, inferred_DlgScript_Update, inferred_DlgScript_ResumePlayer |
| +0xb0 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xb4 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xb8 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xbc | 4 | c | 2/1 | inferred_DlgEntry_CopyCtorA, inferred_DlgScript_Update |
| +0xc0 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xc4 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xc8 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xcc | 4 | c | 4/4 | inferred_DlgEntry_CopyCtorA, inferred_DlgScript_Update, inferred_DlgScript_ResumePlayer |
| +0xd0 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |
| +0xd4 | 4 | c | 0/1 | inferred_DlgEntry_CopyCtorA |

## Obj_Kind13Base  (vtable 00428b00)
- parent: Obj_MidBase (0042ba68) · size: unknown · slots: 28 · constructors: inferred_Obj_Kind13_Ctor
- own methods: [4 Update] inferred_Obj_Kind13_Update, [8 SetActive] inferred_Item_SetHeld, [14 SetVerbAndClick] inferred_C00428b00_SetVerbAndClick

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 2/0 | inferred_Obj_Kind13_Ctor |
| +0x4c x | 4 | r | 1/0 | inferred_Obj_Kind13_Ctor |
| +0x50 y | 4 | r | 1/0 | inferred_Obj_Kind13_Ctor |
| +0x5d | 1 | c | 0/1 | inferred_Obj_Kind13_Ctor |
| +0x5e sortValue | 2 | c | 1/1 | inferred_Obj_Kind13_Ctor |
| +0x68 flags | 4 | c | 4/4 | inferred_Obj_Kind13_Ctor, inferred_Item_SetHeld |
| +0x6c | 4 | c | 0/2 | inferred_Obj_Kind13_Ctor |
| +0x8c | 1 | w? | 0/3 | inferred_Obj_Kind13_Update, inferred_C00428b00_SetVerbAndClick |
| +0x8c | 4 | r | 1/0 | inferred_Obj_Kind13_Update |
| +0x94 | 4 | c | 0/1 | inferred_Obj_Kind13_Ctor |
| +0x98 | 4 | c | 0/1 | inferred_Obj_Kind13_Ctor |
| +0x9c | 4 | w | 1/1 | inferred_Obj_Kind13_Update, inferred_C00428b00_SetVerbAndClick |
| +0xa0 | 4 | c | 3/1 | inferred_Obj_Kind13_Ctor, inferred_C00428b00_SetVerbAndClick |
| +0xa4 | 4 | c | 3/1 | inferred_Obj_Kind13_Ctor, inferred_C00428b00_SetVerbAndClick |
| +0xa8 | 2 | c | 5/1 | inferred_Obj_Kind13_Ctor, inferred_C00428b00_SetVerbAndClick |
| +0xaa | 2 | c | 3/1 | inferred_Obj_Kind13_Ctor, inferred_Obj_Kind13_Update, inferred_C00428b00_SetVerbAndClick… |

## Obj_Kind23  (vtable 0042b8e0)
- parent: Obj_MidBase (0042ba68) · size: 0xa4 · slots: 28 · constructors: inferred_Obj_Kind23_Ctor
- own methods: [6 Draw] inferred_C0042b8e0_Draw, [14 SetVerbAndClick] inferred_C0042b8e0_SetVerbAndClick, [15] inferred_C0042b8e0_SetVerbAndClick, [16 IsInteractive] inferred_C0042b8e0_IsInteractive, [17 ShowHoverText] inferred_C0042b8e0_IsInteractive, [18 ClearHoverText] inferred_C0042b8e0_IsInteractive

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 1/0 | inferred_Obj_Kind23_Ctor |
| +0x5e sortValue | 2 | c | 0/2 | inferred_Obj_Kind23_Ctor |
| +0x68 flags | 4 | c | 2/2 | inferred_Obj_Kind23_Ctor |
| +0xa0 | 1 | c | 2/1 | inferred_Obj_Kind23_Ctor, inferred_C0042b8e0_Draw |

## Obj_Kind22  (vtable 0042b950)
- parent: Obj_MidBase (0042ba68) · size: 0xa4 · slots: 28 · constructors: inferred_Obj_Kind22_Ctor
- own methods: [14 SetVerbAndClick] inferred_C0042b950_SetVerbAndClick, [16 IsInteractive] inferred_C0042b950_IsInteractive, [17 ShowHoverText] inferred_Obj_RetTrue, [18 ClearHoverText] inferred_C0042b950_ClearHoverText

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 3/0 | inferred_C0042b950_IsInteractive, inferred_C0042b950_SetVerbAndClick, inferred_C0042b950_ClearHoverText |
| +0x5e sortValue | 2 | c | 0/1 | inferred_Obj_Kind22_Ctor |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind22_Ctor |
| +0xa0 | 1 | c | 2/1 | inferred_Obj_Kind22_Ctor, inferred_C0042b950_SetVerbAndClick, inferred_C0042b950_ClearHoverText |
| +0xa0 | 4 | r | 1/0 | inferred_C0042b950_IsInteractive |

## Obj_Kind04  (vtable 0042b9f8)
- parent: Obj_Kind03 (0042b9c0) · size: 0x74 · slots: 13 · constructors: inferred_Obj_Kind04_Ctor
- own methods: [6 Draw] inferred_C0042b9f8_Draw

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 1/0 | inferred_C0042b9f8_Draw |
| +0x4c x | 4 | r | 1/0 | inferred_C0042b9f8_Draw |
| +0x50 y | 4 | r | 1/0 | inferred_C0042b9f8_Draw |
| +0x5e sortValue | 2 | r | 1/0 | inferred_C0042b9f8_Draw |
| +0x6c | 4 | c | 0/1 | inferred_Obj_Kind04_Ctor |
| +0x70 | 4 | r | 1/0 | inferred_C0042b9f8_Draw |

## Obj_Kind16  (vtable 0042ba30)
- parent: Obj_Kind03 (0042b9c0) · size: 0xa0 · slots: 13 · constructors: inferred_Obj_Kind16_Ctor
- own methods: [0 OnAttach] inferred_C0042ba30_OnAttach, [2 AnimTick] inferred_C0042ba30_AnimTick, [6 Draw] inferred_C0042ba30_Draw

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 6/0 | inferred_C0042ba30_AnimTick, inferred_C0042ba30_Draw |
| +0x3c room | 4 | r | 1/0 | inferred_C0042ba30_OnAttach |
| +0x40 level | 4 | r | 1/0 | inferred_C0042ba30_OnAttach |
| +0x4c x | 4 | w | 4/3 | inferred_C0042ba30_AnimTick, inferred_C0042ba30_Draw |
| +0x50 y | 4 | w | 5/2 | inferred_C0042ba30_AnimTick, inferred_C0042ba30_Draw |
| +0x5e sortValue | 2 | r | 1/0 | inferred_C0042ba30_Draw |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind16_Ctor |
| +0x6c | 4 | c | 3/1 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x70 | 4 | c | 3/1 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x74 | 4 | c | 1/3 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x78 | 4 | c | 2/4 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x7c | 4 | c | 0/1 | inferred_Obj_Kind16_Ctor |
| +0x80 | 4 | c | 0/1 | inferred_Obj_Kind16_Ctor |
| +0x84 | 4 | c | 0/1 | inferred_Obj_Kind16_Ctor |
| +0x88 | 4 | w | 4/1 | inferred_C0042ba30_AnimTick, inferred_C0042ba30_OnAttach |
| +0x8c | 1 | c | 1/1 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x8d | 1 | c | 1/1 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x8e | 1 | c | 2/3 | inferred_Obj_Kind16_Ctor, inferred_C0042ba30_AnimTick |
| +0x90 | 4 | w | 1/1 | inferred_C0042ba30_AnimTick |
| +0x94 | 4 | w | 5/2 | inferred_C0042ba30_AnimTick |
| +0x98 | 4 | w | 7/2 | inferred_C0042ba30_AnimTick |
| +0x9c | 4 | w | 2/1 | inferred_C0042ba30_AnimTick, inferred_C0042ba30_Draw |

## Obj_Kind09Family  (vtable 0042bad8)
- parent: Obj_MidBase (0042ba68) · size: 0xa0 · slots: 28 · constructors: inferred_Obj_Kind09_Ctor
- own methods: [21 Verb1] inferred_Obj_Verb1_Default, [22 Verb3] inferred_Obj_Verb3_Default, [23 Verb2] inferred_Obj_Verb2_Default, [24 Verb4] inferred_Obj_Verb4_Default, [27 UseItem] inferred_Obj_UseItem_Default

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 1/0 | inferred_Obj_Kind09_Ctor |
| +0x44 nameKey | 4 | r | 1/0 | inferred_Obj_UseItem_Default |
| +0x48 | 4 | r | 1/0 | inferred_Obj_UseItem_Default |
| +0x4c x | 4 | r | 1/0 | inferred_Obj_Kind09_Ctor |
| +0x50 y | 4 | r | 1/0 | inferred_Obj_Kind09_Ctor |
| +0x68 flags | 4 | c | 2/2 | inferred_Obj_Kind09_Ctor |
| +0x6c hoverPhrase | 4 | c | 0/2 | inferred_Obj_Kind09_Ctor |
| +0x74 phraseVerb1 | 4 | r | 1/0 | inferred_Obj_Verb1_Default |
| +0x78 phraseVerb2 | 4 | r | 1/0 | inferred_Obj_Verb2_Default |
| +0x7c phraseVerb3 | 4 | r | 1/0 | inferred_Obj_Verb3_Default |
| +0x80 phraseVerb4 | 4 | r | 1/0 | inferred_Obj_Verb4_Default |
| +0x8c clickPending | 1 | w? | 0/6 | inferred_Obj_Verb1_Default, inferred_Obj_Verb3_Default, inferred_Obj_Verb2_Default… |
| +0x8d clickFlag | 1 | w? | 0/5 | inferred_Obj_Verb1_Default, inferred_Obj_Verb3_Default, inferred_Obj_Verb2_Default… |
| +0x90 | 4 | c | 0/1 | inferred_Obj_Kind09_Ctor |
| +0x94 approachX | 4 | c | 1/2 | inferred_Obj_Kind09_Ctor |
| +0x98 approachY | 4 | c | 1/2 | inferred_Obj_Kind09_Ctor |

## Obj_Kind05Family  (vtable 0042bb48)
- parent: Obj_MidBase (0042ba68) · size: unknown · slots: 33 · constructors: inferred_Obj_Kind05Base_Ctor, inferred_Obj_Kind05_Ctor
- own methods: [4 Update] inferred_Obj_Kind05_Update, [6 Draw] inferred_Obj_Kind05_Draw, [21 Verb1] inferred_Obj_Verb1_Default, [22 Verb3] inferred_Obj_Verb3_Default, [23 Verb2] inferred_Obj_Verb2_Default, [24 Verb4] inferred_Obj_Verb4_Default, [27 UseItem] inferred_Obj_UseItem_Default, [28] inferred_Jim_ClearSpeech, [29] inferred_Obj_SetFacingAnim, [30] inferred_Jim_SetPoseAnim, [31] inferred_C0042b060_Slot31, [32] inferred_C0042b060_Slot32

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x0c | 1 | r | 1/0 | inferred_Obj_Kind05_Draw |
| +0x32 bankIndex | 1 | w? | 0/1 | inferred_Jim_SetPoseAnim |
| +0x33 bankCount | 1 | r | 2/0 | inferred_Obj_Kind05_Ctor |
| +0x34 banks | 4 | r | 3/0 | inferred_Obj_Kind05_Ctor, inferred_Jim_SetPoseAnim |
| +0x38 anim | 4 | w | 6/1 | inferred_C0042b060_Slot31, inferred_Jim_SetPoseAnim |
| +0x40 level | 4 | r | 1/0 | inferred_C0042b060_Slot31 |
| +0x44 nameKey | 4 | r | 1/0 | inferred_Obj_UseItem_Default |
| +0x48 | 4 | r | 1/0 | inferred_Obj_UseItem_Default |
| +0x4c x | 4 | r | 2/0 | inferred_C0042b060_Slot31, inferred_Jim_SetPoseAnim |
| +0x50 y | 4 | r | 2/0 | inferred_C0042b060_Slot31, inferred_Jim_SetPoseAnim |
| +0x5e sortValue | 2 | r | 1/0 | inferred_C0042b060_Slot31 |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind05_Ctor |
| +0x74 | 4 | r | 1/0 | inferred_Obj_Verb1_Default |
| +0x78 | 4 | r | 1/0 | inferred_Obj_Verb2_Default |
| +0x7c | 4 | r | 1/0 | inferred_Obj_Verb3_Default |
| +0x80 | 4 | r | 1/0 | inferred_Obj_Verb4_Default |
| +0x8c | 1 | w? | 0/6 | inferred_Obj_Verb1_Default, inferred_Obj_Verb3_Default, inferred_Obj_Verb2_Default… |
| +0x8d | 1 | w? | 0/5 | inferred_Obj_Verb1_Default, inferred_Obj_Verb3_Default, inferred_Obj_Verb2_Default… |
| +0xa0 | 4 | w? | 0/3 | inferred_Jim_SetPoseAnim |
| +0xa4 | 4 | w? | 0/2 | inferred_Jim_SetPoseAnim |
| +0xa8 | 4 | w | 1/1 | inferred_C0042b060_Slot31 |
| +0xac | 4 | c | 1/2 | inferred_Obj_Kind05_Ctor, inferred_Obj_SetFacingAnim |
| +0xc0 | 4 | c | 3/2 | inferred_Obj_Kind05_Ctor, inferred_Jim_SetPoseAnim |
| +0xc4 | 4 | w | 2/1 | inferred_Obj_Kind05_Update, inferred_Jim_ClearSpeech |
| +0xc8 | 4 | r | 4/0 | inferred_Obj_Kind05_Update |
| +0xd0 | 2 | c | 0/1 | inferred_Obj_Kind05_Ctor |
| +0xd2 | 2 | c | 0/1 | inferred_Obj_Kind05_Ctor |
| +0xd4 | 1 | c | 1/2 | inferred_Obj_Kind05_Ctor, inferred_Obj_Kind05_Draw, inferred_Jim_ClearSpeech |
| +0xd4 | 4 | r | 1/0 | inferred_Obj_Kind05_Update |
| +0xd5 | 1 | c | 2/3 | inferred_Obj_Kind05_Ctor, inferred_Obj_Kind05_Update, inferred_Obj_Kind05_Draw… |

## CubiertaLevel  (vtable 0042bbd0)
- parent: Level (00428bf8) · size: 0x9c · slots: 14 · constructors: inferred_InitGameWorld
- own methods: [13 RunVerb] inferred_C0042bbd0_RunVerb

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x90 walkAreaSetup | 4 | r | 2/0 | inferred_C0042bbd0_RunVerb |

## MapaLevel  (vtable 0042bc08)
- parent: Level (00428bf8) · size: 0x9c · slots: 14 · constructors: inferred_InitGameWorld
- own methods: [13 RunVerb] inferred_C0042bc08_RunVerb

## InterfaceLevel  (vtable 0042bc40)
- parent: Level (00428bf8) · size: 0x9c · slots: 14 · constructors: inferred_InitGameWorld
- own methods: [5 Update2] inferred_C0042bc40_Update2

## WalkArea  (vtable 0042bd60)
- parent: Obj_MidBase (0042ba68) · size: 0xb4 · slots: 28 · constructors: inferred_WalkArea_Ctor
- own methods: [13 RunVerb] inferred_WalkArea_NextWaypoint

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x38 anim | 4 | r | 5/0 | inferred_WalkArea_Ctor, inferred_WalkArea_NextWaypoint |
| +0x4c x | 4 | r | 1/0 | inferred_WalkArea_NextWaypoint |
| +0x50 y | 4 | r | 1/0 | inferred_WalkArea_NextWaypoint |
| +0x68 flags | 4 | c | 1/1 | inferred_WalkArea_Ctor |
| +0xa0 | 4 | w | 1/1 | inferred_WalkArea_NextWaypoint |
| +0xa4 | 4 | r | 2/0 | inferred_WalkArea_NextWaypoint |
| +0xa8 | 4 | r | 1/0 | inferred_WalkArea_NextWaypoint |
| +0xac | 4 | r | 1/0 | inferred_WalkArea_NextWaypoint |
| +0xb0 | 4 | r | 1/0 | inferred_WalkArea_NextWaypoint |

## Obj_Kind11  (vtable 0042be40)
- parent: Obj_MidBase (0042ba68) · size: 0xbc · slots: 28 · constructors: inferred_Obj_Kind11_Ctor
- own methods: [0 OnAttach] inferred_C0042be40_OnAttach, [14 SetVerbAndClick] inferred_C0042be40_SetVerbAndClick, [16 IsInteractive] inferred_C0042be40_IsInteractive, [17 ShowHoverText] inferred_C0042be40_ShowHoverText, [18 ClearHoverText] inferred_C0042be40_ClearHoverText

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x3c room | 4 | r | 1/0 | inferred_C0042be40_OnAttach |
| +0xa0 | 4 | w | 19/1 | inferred_C0042be40_OnAttach, inferred_C0042be40_IsInteractive, inferred_C0042be40_ShowHoverText… |
| +0xa4 | 4 | w | 1/1 | inferred_C0042be40_OnAttach, inferred_C0042be40_ShowHoverText |
| +0xa8 | 4 | w | 3/1 | inferred_C0042be40_OnAttach, inferred_C0042be40_IsInteractive, inferred_C0042be40_ShowHoverText… |
| +0xac | 4 | w? | 0/1 | inferred_C0042be40_OnAttach |
| +0xb0 | 4 | w? | 0/2 | inferred_C0042be40_OnAttach |
| +0xb8 | 4 | c | 3/3 | inferred_Obj_Kind11_Ctor, inferred_C0042be40_SetVerbAndClick, inferred_C0042be40_ShowHoverText |

## Obj_Kind15  (vtable 0042beb0)
- parent: Obj_MidBase (0042ba68) · size: 0xa0 · slots: 28 · constructors: inferred_Obj_Kind15_Ctor
- own methods: [0 OnAttach] inferred_C0042b9c0_OnAttach, [14 SetVerbAndClick] inferred_C0042beb0_SetVerbAndClick, [17 ShowHoverText] inferred_Obj_RetTrue

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x3c room | 4 | r | 1/0 | inferred_C0042b9c0_OnAttach |
| +0x5e sortValue | 2 | c | 0/1 | inferred_Obj_Kind15_Ctor |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind15_Ctor |

## Obj_Kind25  (vtable 0042c000)
- parent: Obj_MidBase (0042ba68) · size: 0xa4 · slots: 28 · constructors: inferred_Obj_Kind25_Ctor
- own methods: [14 SetVerbAndClick] inferred_C0042c000_SetVerbAndClick, [17 ShowHoverText] inferred_Obj_RetTrue

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042c000_SetVerbAndClick |
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind25_Ctor |
| +0xa0 | 4 | c | 1/1 | inferred_Obj_Kind25_Ctor, inferred_C0042c000_SetVerbAndClick |

## Obj_Kind21  (vtable 0042c070)
- parent: Obj_MidBase (0042ba68) · size: 0xa4 · slots: 28 · constructors: inferred_Obj_Kind21_Ctor
- own methods: [14 SetVerbAndClick] inferred_C0042c070_SetVerbAndClick

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x68 flags | 4 | c | 1/1 | inferred_Obj_Kind21_Ctor |
| +0x6c | 4 | c | 0/1 | inferred_Obj_Kind21_Ctor |
| +0xa0 | 1 | r | 2/0 | inferred_Obj_Kind21_Ctor, inferred_C0042c070_SetVerbAndClick |
| +0xa0 | 4 | c | 2/1 | inferred_Obj_Kind21_Ctor, inferred_C0042c070_SetVerbAndClick |

## Cls_004280f0  (vtable 004280f0)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004280f0_Slot28

## Cls_00428168  (vtable 00428168)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_DlgScript_EnterStageLevel

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 4/2 | inferred_DlgScript_EnterStageLevel |
| +0xb0 | 4 | r | 2/0 | inferred_DlgScript_EnterStageLevel |
| +0xb4 | 4 | r | 4/0 | inferred_DlgScript_EnterStageLevel |
| +0xb8 | 4 | r | 4/0 | inferred_DlgScript_EnterStageLevel |

## Cls_004281e0  (vtable 004281e0)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004281e0_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 3/3 | inferred_C004281e0_Slot28 |
| +0xb0 | 4 | r | 3/0 | inferred_C004281e0_Slot28 |
| +0xb4 | 4 | r | 6/0 | inferred_C004281e0_Slot28 |
| +0xb8 | 4 | r | 6/0 | inferred_C004281e0_Slot28 |

## Cls_00428258  (vtable 00428258)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428258_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/2 | inferred_C00428258_Slot28 |
| +0xb0 | 4 | r | 2/0 | inferred_C00428258_Slot28 |
| +0xb4 | 4 | r | 4/0 | inferred_C00428258_Slot28 |
| +0xb8 | 4 | r | 4/0 | inferred_C00428258_Slot28 |

## Cls_004282d0  (vtable 004282d0)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004282d0_Slot28

## Cls_00428348  (vtable 00428348)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428348_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w? | 0/1 | inferred_C00428348_Slot28 |

## Cls_004283c0  (vtable 004283c0)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004283c0_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w? | 0/1 | inferred_C004283c0_Slot28 |

## Cls_00428438  (vtable 00428438)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428438_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w? | 0/1 | inferred_C00428438_Slot28 |
| +0xd0 | 4 | r | 2/0 | inferred_C00428438_Slot28 |

## Cls_004284b0  (vtable 004284b0)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load

## Cls_00428528  (vtable 00428528)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428528_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/2 | inferred_C00428528_Slot28 |
| +0xb0 | 4 | r | 2/0 | inferred_C00428528_Slot28 |
| +0xb4 | 4 | r | 4/0 | inferred_C00428528_Slot28 |
| +0xb8 | 4 | r | 4/0 | inferred_C00428528_Slot28 |

## Cls_004285a0  (vtable 004285a0)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load

## Cls_00428618  (vtable 00428618)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428618_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/2 | inferred_C00428618_Slot28 |
| +0xb0 | 4 | r | 2/0 | inferred_C00428618_Slot28 |
| +0xb4 | 4 | r | 4/0 | inferred_C00428618_Slot28 |
| +0xb8 | 4 | r | 4/0 | inferred_C00428618_Slot28 |

## Cls_00428690  (vtable 00428690)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428690_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w? | 0/1 | inferred_C00428690_Slot28 |

## Cls_00428708  (vtable 00428708)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428708_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 1/1 | inferred_C00428708_Slot28 |
| +0xd0 | 4 | r | 3/0 | inferred_C00428708_Slot28 |

## Cls_00428780  (vtable 00428780)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428780_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 3/1 | inferred_C00428780_Slot28 |

## Cls_004287f8  (vtable 004287f8)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004287f8_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/1 | inferred_C004287f8_Slot28 |
| +0xb0 | 4 | r | 1/0 | inferred_C004287f8_Slot28 |
| +0xb4 | 4 | r | 2/0 | inferred_C004287f8_Slot28 |
| +0xb8 | 4 | r | 2/0 | inferred_C004287f8_Slot28 |

## Cls_00428870  (vtable 00428870)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428870_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 1/1 | inferred_C00428870_Slot28 |

## Cls_004288e8  (vtable 004288e8)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004288e8_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w? | 0/2 | inferred_C004288e8_Slot28 |

## Cls_00428960  (vtable 00428960)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428960_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/1 | inferred_C00428960_Slot28 |
| +0xb0 | 4 | r | 1/0 | inferred_C00428960_Slot28 |
| +0xb4 | 4 | r | 2/0 | inferred_C00428960_Slot28 |
| +0xb8 | 4 | r | 2/0 | inferred_C00428960_Slot28 |

## Cls_004289d8  (vtable 004289d8)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C004289d8_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/2 | inferred_C004289d8_Slot28 |
| +0xb0 | 4 | r | 1/0 | inferred_C004289d8_Slot28 |
| +0xb4 | 4 | r | 2/0 | inferred_C004289d8_Slot28 |
| +0xb8 | 4 | r | 2/0 | inferred_C004289d8_Slot28 |

## Cls_00428a50  (vtable 00428a50)
- parent: DlgScriptEntry (00428078) · size: unknown · slots: 29 · constructors: inferred_DialogueTable_Load
- own methods: [28] inferred_C00428a50_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w? | 0/1 | inferred_C00428a50_Slot28 |

## Jim  (vtable 00428b70)
- parent: Obj_Kind05Family (0042bb48) · size: 0x114 · slots: 33 · constructors: inferred_Jim_Ctor
- own methods: [3 UpdateBounds] inferred_Jim_UpdateBounds, [4 Update] inferred_Jim_Update, [6 Draw] inferred_Jim_Draw, [9 LoadState] inferred_Jim_LoadState, [10 SaveState] inferred_Jim_SaveState, [11 IsHitter] inferred_Jim_IsInPose7, [12 OnTouched] inferred_Jim_ArriveStep, [27 UseItem] inferred_Jim_UseItemForward, [29] inferred_Jim_SetFacing, [30] inferred_Jim_SetIdlePose, [31] inferred_Jim_TalkAnimate, [32] inferred_Jim_WalkTo

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x0c | 1 | r | 1/0 | inferred_Jim_Draw |
| +0x10 boundsLeft? | 4 | w | 1/1 | inferred_Jim_UpdateBounds |
| +0x14 boundsTop? | 4 | w | 1/1 | inferred_Jim_UpdateBounds |
| +0x18 boundsRight? | 4 | w | 1/1 | inferred_Jim_UpdateBounds |
| +0x1c boundsBottom? | 4 | w | 1/1 | inferred_Jim_UpdateBounds |
| +0x33 bankCount | 1 | c | 2/1 | inferred_Jim_Ctor |
| +0x34 banks | 4 | c | 10/1 | inferred_Jim_Ctor |
| +0x38 anim | 4 | w | 13/1 | inferred_Jim_WalkTo, inferred_Jim_Update, inferred_Jim_Draw… |
| +0x3c room | 4 | c | 0/2 | inferred_Jim_Ctor |
| +0x40 level | 4 | c | 1/2 | inferred_Jim_Ctor, inferred_Jim_TalkAnimate |
| +0x44 nameKey | 8 | c | 0/1 | inferred_Jim_Ctor |
| +0x4c x | 4 | c | 7/2 | inferred_Jim_Ctor, inferred_Jim_WalkTo, inferred_Jim_Update… |
| +0x50 y | 4 | c | 8/2 | inferred_Jim_Ctor, inferred_Jim_WalkTo, inferred_Jim_Update… |
| +0x54 prevX | 4 | w? | 0/1 | inferred_Jim_ArriveStep |
| +0x58 prevY | 4 | w? | 0/1 | inferred_Jim_ArriveStep |
| +0x5c state | 1 | c | 0/1 | inferred_Jim_Ctor |
| +0x5e sortValue | 2 | c | 2/1 | inferred_Jim_Ctor, inferred_Jim_Draw, inferred_Jim_TalkAnimate |
| +0x68 flags | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0x6c | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xa4 | 4 | r | 1/0 | inferred_Jim_SetIdlePose |
| +0xa8 | 4 | w | 1/1 | inferred_Jim_TalkAnimate |
| +0xac | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xb0 | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xb4 | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xb8 | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xbc | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xc0 | 4 | c | 0/1 | inferred_Jim_Ctor |
| +0xcc speechId | 4 | r | 1/0 | inferred_Jim_Draw |
| +0xd0 | 2 | c | 0/1 | inferred_Jim_Ctor |
| +0xd2 | 2 | c | 0/1 | inferred_Jim_Ctor |
| +0xd4 talking | 1 | r | 1/0 | inferred_Jim_Draw |
| +0xd5 walking | 1 | c | 3/4 | inferred_Jim_Ctor, inferred_Jim_LoadState, inferred_Jim_WalkTo… |
| +0xd8 walkAnimTable | 4 | c | 1/1 | inferred_Jim_Ctor, inferred_Jim_WalkTo |
| +0xe0 fixedX | 4 | w | 3/3 | inferred_Jim_WalkTo, inferred_Jim_Update |
| +0xe4 fixedY | 4 | w | 3/3 | inferred_Jim_WalkTo, inferred_Jim_Update |
| +0xe8 | 4 | w | 1/2 | inferred_Jim_WalkTo, inferred_Jim_Update |
| +0xec | 4 | w | 1/2 | inferred_Jim_WalkTo, inferred_Jim_Update |
| +0xf0 walkFlag | 1 | c | 1/3 | inferred_Jim_Ctor, inferred_Jim_WalkTo, inferred_Jim_ArriveStep |
| +0xf4 walkTarget | 4 | w | 5/1 | inferred_Jim_WalkTo, inferred_Jim_Update, inferred_Jim_ArriveStep |
| +0xfc scale | 4 | c | 6/5 | inferred_Jim_Ctor, inferred_Jim_UpdateBounds, inferred_Jim_Update… |
| +0x100 | 2 | c | 0/1 | inferred_Jim_Ctor |
| +0x102 | 1 | c | 0/1 | inferred_Jim_Ctor |
| +0x104 targetX | 4 | w | 5/1 | inferred_Jim_WalkTo, inferred_Jim_Update, inferred_Jim_ArriveStep |
| +0x108 targetY | 4 | w | 5/1 | inferred_Jim_WalkTo, inferred_Jim_Update, inferred_Jim_ArriveStep |
| +0x10c facing | 4 | c | 6/4 | inferred_Jim_Ctor, inferred_Jim_WalkTo, inferred_Jim_Update… |

## Obj_Kind20_Sub30  (vtable 00428c30)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 30
- own methods: [20 Verb0] inferred_C00428c30_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w? | 0/1 | inferred_C00428c30_Verb0 |

## Obj_Kind20_Sub29  (vtable 00428ca0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 29
- own methods: [21 Verb1] inferred_C00428ca0_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00428ca0_Verb1 |

## Obj_Kind20_Sub28  (vtable 00428d10)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 28
- own methods: [27 UseItem] inferred_C00428d10_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C00428d10_UseItem |

## Obj_Kind20_Sub27  (vtable 00428d80)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 27
- own methods: [22 Verb3] inferred_C00428d80_Verb3

## Obj_Kind20_Sub26  (vtable 00428df0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 26
- own methods: [21 Verb1] inferred_C00428df0_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00428df0_Verb1 |
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C00428df0_Verb1 |

## Obj_Kind20_Sub25  (vtable 00428e60)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 25
- own methods: [21 Verb1] inferred_C00428e60_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00428e60_Verb1 |

## Obj_Kind20_Sub23  (vtable 00428ed0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 23
- own methods: [22 Verb3] inferred_C00428ed0_Verb3

## Obj_Kind20_Sub24  (vtable 00428f40)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 24
- own methods: [21 Verb1] inferred_C00428f40_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00428f40_Verb1 |
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C00428f40_Verb1 |

## Obj_Kind20_Sub22  (vtable 00428fb0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 22
- own methods: [22 Verb3] inferred_C00428fb0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00428fb0_Verb3 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00428fb0_Verb3 |

## Obj_Kind20_Sub21  (vtable 00429020)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 21
- own methods: [27 UseItem] inferred_C00429020_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429020_UseItem |

## Obj_Kind20_Sub20  (vtable 00429090)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 20
- own methods: [20 Verb0] inferred_C00429090_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429090_Verb0 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00429090_Verb0 |

## Obj_Kind20_Sub19  (vtable 00429100)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 19
- own methods: [27 UseItem] inferred_C00429100_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429100_UseItem |

## Obj_Kind20_Sub18  (vtable 00429170)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 18
- own methods: [22 Verb3] inferred_C00429170_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429170_Verb3 |

## Obj_Kind20_Sub15  (vtable 004291e0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 15
- own methods: [20 Verb0] inferred_Obj_RetTrue

## Obj_Kind20_Sub14  (vtable 00429250)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 14
- own methods: [27 UseItem] inferred_C00429250_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429250_UseItem |

## Obj_Kind20_Sub16  (vtable 004292c0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 16
- own methods: [27 UseItem] inferred_C004292c0_UseItem

## Obj_Kind20_Sub13  (vtable 00429330)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 13
- own methods: [21 Verb1] inferred_C00429330_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429330_Verb1 |

## Obj_Kind20_Sub12  (vtable 004293a0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 12
- own methods: [21 Verb1] inferred_C004293a0_Verb1, [22 Verb3] inferred_C004293a0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C004293a0_Verb1, inferred_C004293a0_Verb3 |

## Obj_Kind20_Sub11  (vtable 00429410)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 11
- own methods: [27 UseItem] inferred_C00429410_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | w | 2/1 | inferred_C00429410_UseItem |

## Obj_Kind20_Sub10  (vtable 00429480)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 10
- own methods: [22 Verb3] inferred_C00429480_Verb3

## Obj_Kind20_Sub9  (vtable 004294f0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 9
- own methods: [22 Verb3] inferred_C004294f0_Verb3, [23 Verb2] inferred_C004294f0_Verb2

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C004294f0_Verb2, inferred_C004294f0_Verb3 |
| +0x60 stateBlob | 4 | w? | 0/1 | inferred_C004294f0_Verb3 |

## Obj_Kind20_Sub8  (vtable 00429560)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 8
- own methods: [27 UseItem] inferred_C00429560_UseItem

## Obj_Kind20_Sub7  (vtable 004295d0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 7
- own methods: [27 UseItem] inferred_C004295d0_UseItem

## Obj_Kind20_Sub6  (vtable 00429640)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 6
- own methods: [22 Verb3] inferred_C00429640_Verb3

## Obj_Kind20_Sub5  (vtable 004296b0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 5
- own methods: [20 Verb0] inferred_C004296b0_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C004296b0_Verb0 |

## Obj_Kind20_Sub4  (vtable 00429720)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 4
- own methods: [21 Verb1] inferred_C00429720_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429720_Verb1 |

## Obj_Kind20_Sub3  (vtable 00429790)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 3
- own methods: [20 Verb0] inferred_C00429790_Verb0, [21 Verb1] inferred_C00429790_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C00429790_Verb1, inferred_C00429790_Verb0 |
| +0x60 stateBlob | 4 | w | 2/1 | inferred_C00429790_Verb1, inferred_C00429790_Verb0 |

## Obj_Kind20_Sub2  (vtable 00429800)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 2
- own methods: [27 UseItem] inferred_C00429800_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C00429800_UseItem |

## Obj_Kind20_Sub1  (vtable 00429870)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 1
- own methods: [27 UseItem] inferred_C00429870_UseItem

## Obj_Kind20_Sub0  (vtable 004298e0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 20 subtype 0
- own methods: [23 Verb2] inferred_C004298e0_Verb2, [27 UseItem] inferred_C004298e0_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C004298e0_Verb2, inferred_C004298e0_UseItem |
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C004298e0_Verb2 |

## Obj_Kind17_Sub16  (vtable 00429950)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 16
- own methods: [24 Verb4] inferred_C00429950_Verb4, [27 UseItem] inferred_C00429950_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C00429950_Verb4, inferred_C00429950_UseItem |

## Obj_Kind17_Sub15  (vtable 004299c0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 15
- own methods: [22 Verb3] inferred_C004299c0_Verb3

## Obj_Kind17_Sub14  (vtable 00429a30)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 14
- own methods: [22 Verb3] inferred_C00429a30_Verb3

## Obj_Kind17_Sub12  (vtable 00429aa0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 12
- own methods: [22 Verb3] inferred_C00429aa0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429aa0_Verb3 |

## Obj_Kind17_Sub11  (vtable 00429b10)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 11
- own methods: [23 Verb2] inferred_C00429b10_Verb2, [27 UseItem] inferred_C00429b10_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C00429b10_Verb2, inferred_C00429b10_UseItem |

## Obj_Kind17_Sub10  (vtable 00429b80)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 10
- own methods: [22 Verb3] inferred_C00429b80_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429b80_Verb3 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00429b80_Verb3 |

## Obj_Kind17_Sub9  (vtable 00429bf0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 9
- own methods: [22 Verb3] inferred_C00429bf0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429bf0_Verb3 |
| +0x60 stateBlob | 4 | w | 2/1 | inferred_C00429bf0_Verb3 |

## Obj_Kind17_Sub8  (vtable 00429c60)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 8
- own methods: [21 Verb1] inferred_C00429c60_Verb1, [22 Verb3] inferred_C00429c60_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C00429c60_Verb1, inferred_C00429c60_Verb3 |
| +0x60 stateBlob | 4 | w | 2/2 | inferred_C00429c60_Verb1, inferred_C00429c60_Verb3 |

## Obj_Kind17_Sub7  (vtable 00429cd0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 7
- own methods: [22 Verb3] inferred_C00429cd0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429cd0_Verb3 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00429cd0_Verb3 |

## Obj_Kind17_Sub13  (vtable 00429d40)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 13
- own methods: [22 Verb3] inferred_C00429d40_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429d40_Verb3 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00429d40_Verb3 |

## Obj_Kind17_Sub6  (vtable 00429db0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 6
- own methods: [20 Verb0] inferred_C00429db0_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429db0_Verb0 |

## Obj_Kind17_Sub5  (vtable 00429e20)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 5
- own methods: [21 Verb1] inferred_C00429e20_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429e20_Verb1 |
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C00429e20_Verb1 |

## Obj_Kind17_Sub4  (vtable 00429e90)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 4
- own methods: [21 Verb1] inferred_C00429e90_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429e90_Verb1 |
| +0x60 stateBlob | 4 | w | 2/1 | inferred_C00429e90_Verb1 |

## Obj_Kind17_Sub3  (vtable 00429f00)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 3
- own methods: [22 Verb3] inferred_C00429f00_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429f00_Verb3 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00429f00_Verb3 |

## Obj_Kind17_Sub2  (vtable 00429f70)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 2
- own methods: [21 Verb1] inferred_C00429f70_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429f70_Verb1 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C00429f70_Verb1 |

## Obj_Kind17_Sub1  (vtable 00429fe0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 1
- own methods: [21 Verb1] inferred_C00429fe0_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C00429fe0_Verb1 |

## Obj_Kind17_Sub0  (vtable 0042a050)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 17 subtype 0
- own methods: [21 Verb1] inferred_C0042a050_Verb1, [22 Verb3] inferred_C0042a050_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042a050_Verb1, inferred_C0042a050_Verb3 |

## Obj_Kind13_Sub26  (vtable 0042a0c0)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 26

## Obj_Kind13_Sub42  (vtable 0042a130)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 42
- own methods: [27 UseItem] inferred_C0042a130_UseItem

## Obj_Kind13_Sub29  (vtable 0042a1a0)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 29
- own methods: [24 Verb4] inferred_C0042a1a0_Verb4

## Obj_Kind13_Sub33  (vtable 0042a210)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 33
- own methods: [27 UseItem] inferred_C0042a210_UseItem

## Obj_Kind13_Sub44  (vtable 0042a280)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 44
- own methods: [21 Verb1] inferred_C0042a280_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C0042a280_Verb1 |

## Obj_Kind13_Sub37  (vtable 0042a2f0)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 37
- own methods: [27 UseItem] inferred_C0042a2f0_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C0042a2f0_UseItem |

## Obj_Kind13_Sub36  (vtable 0042a360)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 36
- own methods: [27 UseItem] inferred_C0042a360_UseItem

## Obj_Kind13_Sub19  (vtable 0042a3d0)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 19
- own methods: [21 Verb1] inferred_C0042a3d0_Verb1, [24 Verb4] inferred_C0042a3d0_Verb4

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w | 2/1 | inferred_C0042a3d0_Verb1 |

## Obj_Kind13_Sub28  (vtable 0042a440)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 28
- own methods: [21 Verb1] inferred_C0042a440_Verb1, [27 UseItem] inferred_C0042a440_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w | 2/1 | inferred_C0042a440_Verb1, inferred_C0042a440_UseItem |

## Obj_Kind13_Sub15  (vtable 0042a4b0)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 15
- own methods: [24 Verb4] inferred_C0042a4b0_Verb4

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w? | 0/1 | inferred_C0042a4b0_Verb4 |

## Obj_Kind13_Sub9  (vtable 0042a520)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 9
- own methods: [24 Verb4] inferred_C0042a520_Verb4

## Obj_Kind13_Sub10  (vtable 0042a590)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 10
- own methods: [24 Verb4] inferred_C0042a590_Verb4

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C0042a590_Verb4 |

## Obj_Kind13_Sub16  (vtable 0042a600)
- parent: Obj_Kind13Base (00428b00) · size: 0xac · slots: 28 · constructors: inferred_Level_CreateObject · kind 13 subtype 16
- own methods: [21 Verb1] inferred_C0042a600_Verb1, [27 UseItem] inferred_C0042a600_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | r | 10/0 | inferred_C0042a600_Verb1, inferred_C0042a600_UseItem |

## Obj_Kind12_Sub11  (vtable 0042a670)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 11
- own methods: [20 Verb0] inferred_C0042a670_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042a670_Verb0 |

## Obj_Kind12_Sub10  (vtable 0042a6e0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 10
- own methods: [22 Verb3] inferred_C0042a6e0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042a6e0_Verb3 |

## Obj_Kind12_Sub8  (vtable 0042a750)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 8
- own methods: [21 Verb1] inferred_C0042a750_Verb1, [23 Verb2] inferred_C0042a750_Verb2, [27 UseItem] inferred_C0042a750_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042a750_Verb1, inferred_C0042a750_Verb2, inferred_C0042a750_UseItem |
| +0x60 stateBlob | 4 | r | 3/0 | inferred_C0042a750_Verb1, inferred_C0042a750_Verb2, inferred_C0042a750_UseItem |

## Obj_Kind12_Sub9  (vtable 0042a7c0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 9
- own methods: [22 Verb3] inferred_C0042a7c0_Verb3

## Obj_Kind12_Sub7  (vtable 0042a830)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 7
- own methods: [21 Verb1] inferred_C0042a830_Verb1, [23 Verb2] inferred_C0042a830_Verb2

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042a830_Verb1, inferred_C0042a830_Verb2 |

## Obj_Kind12_Sub6  (vtable 0042a8a0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 6
- own methods: [22 Verb3] inferred_C0042a8a0_Verb3

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042a8a0_Verb3 |

## Obj_Kind12_Sub5  (vtable 0042a910)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 5
- own methods: [21 Verb1] inferred_C0042a910_Verb1, [22 Verb3] inferred_C0042a910_Verb3, [24 Verb4] inferred_C0042a910_Verb4

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042a910_Verb1, inferred_C0042a910_Verb3, inferred_C0042a910_Verb4 |

## Obj_Kind12_Sub4  (vtable 0042a980)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 4
- own methods: [21 Verb1] inferred_C0042a980_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042a980_Verb1 |
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C0042a980_Verb1 |

## Obj_Kind12_Sub3  (vtable 0042a9f0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 3
- own methods: [24 Verb4] inferred_C0042a9f0_Verb4

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042a9f0_Verb4 |
| +0x60 stateBlob | 4 | r | 1/0 | inferred_C0042a9f0_Verb4 |

## Obj_Kind12_Sub2  (vtable 0042aa60)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 2
- own methods: [21 Verb1] inferred_C0042aa60_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042aa60_Verb1 |

## Obj_Kind12_Sub1  (vtable 0042aad0)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 1
- own methods: [21 Verb1] inferred_C0042aad0_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042aad0_Verb1 |

## Obj_Kind12_Sub0  (vtable 0042ab40)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa0 · slots: 28 · constructors: inferred_Level_CreateObject · kind 12 subtype 0
- own methods: [22 Verb3] inferred_C0042ab40_Verb3, [26 Verb6] inferred_C0042ab40_Verb6

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042ab40_Verb3, inferred_C0042ab40_Verb6 |
| +0x60 stateBlob | 4 | w | 2/2 | inferred_C0042ab40_Verb3, inferred_C0042ab40_Verb6 |

## Obj_Kind05_Sub16  (vtable 0042b060)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 16
- own methods: [23 Verb2] inferred_C0042b060_Verb2

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042b060_Verb2 |
| +0x60 stateBlob | 4 | w | 3/1 | inferred_C0042b060_Verb2 |

## Obj_Kind05_Sub15  (vtable 0042b0e8)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 15
- own methods: [23 Verb2] inferred_C0042b0e8_Verb2

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042b0e8_Verb2 |
| +0x60 stateBlob | 4 | r | 3/0 | inferred_C0042b0e8_Verb2 |

## Obj_Kind05_Sub14  (vtable 0042b170)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 14
- own methods: [21 Verb1] inferred_C0042b170_Verb1, [23 Verb2] inferred_C0042b170_Verb2, [27 UseItem] inferred_C0042b170_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 4/0 | inferred_C0042b170_Verb1, inferred_C0042b170_Verb2, inferred_C0042b170_UseItem |
| +0x60 stateBlob | 4 | r | 3/0 | inferred_C0042b170_Verb1, inferred_C0042b170_Verb2 |

## Obj_Kind05_Sub13  (vtable 0042b1f8)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 13
- own methods: [21 Verb1] inferred_C0042b1f8_Verb1, [23 Verb2] inferred_C0042b1f8_Verb2, [27 UseItem] inferred_C0042b1f8_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042b1f8_UseItem, inferred_C0042b1f8_Verb2, inferred_C0042b1f8_Verb1 |
| +0x60 stateBlob | 4 | w | 8/2 | inferred_C0042b1f8_Verb2, inferred_C0042b1f8_Verb1 |

## Obj_Kind05_Sub0  (vtable 0042b280)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 0
- own methods: [23 Verb2] inferred_C0042b280_Verb2, [27 UseItem] inferred_C0042b280_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042b280_Verb2, inferred_C0042b280_UseItem |
| +0x60 stateBlob | 4 | r | 5/0 | inferred_C0042b280_Verb2, inferred_C0042b280_UseItem |

## Obj_Kind05_Sub2  (vtable 0042b308)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 2
- own methods: [23 Verb2] inferred_C0042b308_Verb2, [27 UseItem] inferred_C0042b308_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042b308_Verb2, inferred_C0042b308_UseItem |
| +0x60 stateBlob | 4 | w | 2/2 | inferred_C0042b308_Verb2, inferred_C0042b308_UseItem |

## Obj_Kind05_Sub3  (vtable 0042b390)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 3
- own methods: [6 Draw] inferred_C0042b390_Draw, [23 Verb2] inferred_C0042b390_Verb2, [28] inferred_C0042b390_Slot28, [30] inferred_C0042b390_Slot30

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x0c | 1 | r | 1/0 | inferred_C0042b390_Draw |
| +0x40 level | 4 | r | 1/0 | inferred_C0042b390_Verb2 |
| +0xd4 | 1 | r | 1/0 | inferred_C0042b390_Draw |

## Obj_Kind05_Sub4  (vtable 0042b418)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 4
- own methods: [23 Verb2] inferred_C0042b418_Verb2, [27 UseItem] inferred_C0042b418_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 4/0 | inferred_C0042b418_Verb2, inferred_C0042b418_UseItem |
| +0x60 stateBlob | 4 | r | 6/0 | inferred_C0042b418_Verb2, inferred_C0042b418_UseItem |

## Obj_Kind05_Sub1  (vtable 0042b4a0)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 1
- own methods: [23 Verb2] inferred_C0042b4a0_Verb2, [27 UseItem] inferred_C0042b4a0_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042b4a0_Verb2, inferred_C0042b4a0_UseItem |
| +0x60 stateBlob | 4 | w | 6/4 | inferred_C0042b4a0_Verb2, inferred_C0042b4a0_UseItem |

## Obj_Kind05_Sub8  (vtable 0042b528)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 8
- own methods: [21 Verb1] inferred_C0042b528_Verb1, [23 Verb2] inferred_C0042b528_Verb2, [27 UseItem] inferred_C0042b528_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042b528_UseItem, inferred_C0042b528_Verb2, inferred_C0042b528_Verb1 |
| +0x60 stateBlob | 4 | r | 3/0 | inferred_C0042b528_UseItem, inferred_C0042b528_Verb2, inferred_C0042b528_Verb1 |

## Obj_Kind05_Sub7  (vtable 0042b5b0)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 7
- own methods: [23 Verb2] inferred_C0042b5b0_Verb2, [27 UseItem] inferred_C0042b5b0_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042b5b0_Verb2, inferred_C0042b5b0_UseItem |
| +0x60 stateBlob | 4 | r | 2/0 | inferred_C0042b5b0_Verb2, inferred_C0042b5b0_UseItem |

## Obj_Kind05_Sub6  (vtable 0042b638)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 6
- own methods: [23 Verb2] inferred_C0042b638_Verb2

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042b638_Verb2 |
| +0x60 stateBlob | 4 | r | 4/0 | inferred_C0042b638_Verb2 |

## Obj_Kind05_Sub5  (vtable 0042b6c0)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 5
- own methods: [23 Verb2] inferred_C0042b6c0_Verb2, [27 UseItem] inferred_C0042b6c0_UseItem

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 3/0 | inferred_C0042b6c0_Verb2, inferred_C0042b6c0_UseItem |
| +0x60 stateBlob | 4 | r | 2/0 | inferred_C0042b6c0_Verb2, inferred_C0042b6c0_UseItem |

## Obj_Kind05_Sub9  (vtable 0042b748)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 9

## Obj_Kind05_Sub11  (vtable 0042b7d0)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 11

## Obj_Kind05_Sub10  (vtable 0042b858)
- parent: Obj_Kind05Family (0042bb48) · size: 0xd8 · slots: 33 · constructors: inferred_Level_CreateObject · kind 5 subtype 10

## Obj_Kind06Base  (vtable 0042bce8)
- parent: Obj_Kind09Family (0042bad8) · size: unknown · slots: 29 · constructors: inferred_Obj_Kind06_Ctor
- own methods: [15] inferred_C0042abb0_Slot15, [20 Verb0] inferred_Exit_Activate, [28] inferred_C0042abb0_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x90 | 4 | r | 1/0 | inferred_Exit_Activate |
| +0x94 approachX | 4 | r | 1/0 | inferred_C0042abb0_Slot15 |
| +0x98 approachY | 4 | r | 1/0 | inferred_C0042abb0_Slot15 |
| +0x9c currentVerb | 4 | r | 1/0 | inferred_Exit_Activate |
| +0xa0 itemKeys | 1 | c | 1/1 | inferred_Obj_Kind06_Ctor, inferred_Exit_Activate |
| +0xa4 itemResponses | 4 | r | 1/0 | inferred_Exit_Activate |
| +0xa4 itemResponses | 8 | c | 0/1 | inferred_Obj_Kind06_Ctor |
| +0xa8 itemCount | 4 | r | 1/0 | inferred_Exit_Activate |
| +0xac | 4 | c | 1/1 | inferred_Obj_Kind06_Ctor, inferred_Exit_Activate |

## Obj_Kind24  (vtable 0042bf20)
- parent: Obj_Kind09Family (0042bad8) · size: 0xa4 · slots: 28 · constructors: inferred_Obj_Kind24_Ctor
- own methods: [21 Verb1] inferred_C0042bf20_Verb1, [22 Verb3] inferred_C0042bf20_Verb1

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042bf20_Verb1 |
| +0xa0 itemKeys | 4 | c | 0/1 | inferred_Obj_Kind24_Ctor |

## Obj_Kind06_Sub1  (vtable 0042abb0)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 1
- own methods: [20 Verb0] inferred_C0042abb0_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042abb0_Verb0 |
| +0x60 stateBlob | 4 | w | 1/2 | inferred_C0042abb0_Verb0 |

## Obj_Kind06_Sub2  (vtable 0042ac28)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 2
- own methods: [20 Verb0] inferred_C0042ac28_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C0042ac28_Verb0 |

## Obj_Kind06_Sub3  (vtable 0042aca0)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 3
- own methods: [20 Verb0] inferred_C0042aca0_Verb0, [28] inferred_C0042aca0_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 6/0 | inferred_C0042aca0_Verb0, inferred_C0042aca0_Slot28 |
| +0x60 stateBlob | 4 | w | 2/3 | inferred_C0042aca0_Slot28 |

## Obj_Kind06_Sub4  (vtable 0042ad18)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 4
- own methods: [20 Verb0] inferred_C0042ad18_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C0042ad18_Verb0 |

## Obj_Kind06_Sub5  (vtable 0042ad90)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 5
- own methods: [20 Verb0] inferred_C0042ad90_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042ad90_Verb0 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C0042ad90_Verb0 |

## Obj_Kind06_Sub6  (vtable 0042ae08)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 6
- own methods: [20 Verb0] inferred_C0042ae08_Verb0, [28] inferred_Obj_Stub_RetZero

## Obj_Kind06_Sub7  (vtable 0042ae80)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 7
- own methods: [20 Verb0] inferred_C0042ae80_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042ae80_Verb0 |

## Obj_Kind06_Sub8  (vtable 0042aef8)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 8
- own methods: [15] inferred_C0042aef8_Slot15, [20 Verb0] inferred_C0042aef8_Verb0, [24 Verb4] inferred_C0042aef8_Verb4

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x60 stateBlob | 4 | w | 4/1 | inferred_C0042aef8_Verb0, inferred_C0042aef8_Verb4, inferred_C0042aef8_Slot15 |

## Obj_Kind06_Sub9  (vtable 0042af70)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 9
- own methods: [20 Verb0] inferred_C0042af70_Verb0

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 1/0 | inferred_C0042af70_Verb0 |

## Obj_Kind06_Sub10  (vtable 0042afe8)
- parent: Obj_Kind06Base (0042bce8) · size: 0xb0 · slots: 29 · constructors: inferred_Level_CreateObject · kind 6 subtype 10
- own methods: [28] inferred_C0042afe8_Slot28

| Off | W | Conf | R/W | Seen in |
|---|---|---|---|---|
| +0x40 level | 4 | r | 2/0 | inferred_C0042afe8_Slot28 |
| +0x60 stateBlob | 4 | w | 1/1 | inferred_C0042afe8_Slot28 |
