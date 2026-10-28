# Function checklist

Generated from `ledger.json` by `tools/make_checklist.py`. Update with `tools/mark.py` (see its docstring).
`[x]` means a C++ port exists (status ported or later). The status after each entry says how far it got and how it was checked: decompiled < understood < ported < emu-compared < original-compared.

## Overall (game code: 441 functions)

| Stage | Count | Share |
|---|---|---|
| untouched or later | 441 | 100% |
| decompiled or later | 9 | 2% |
| understood or later | 0 | 0% |
| ported or later | 0 | 0% |
| emu-compared or later | 0 | 0% |
| original-compared or later | 0 | 0% |
| replaced by a ScummVM service | 0 | |

## Game code by cluster

### Cluster 1: 00401000–00401740 (0/7 ported)

- [ ] `00401000` FUN_00401000 · depth 0 · 7 lines · **untouched**
- [ ] `00401580` FUN_00401580 · depth 0 · 14 lines · **untouched**
- [ ] `00401010` FUN_00401010 · depth 1 · 73 lines · **untouched**
- [ ] `00401740` FUN_00401740 · depth 1 · 70 lines · **untouched**
- [ ] `004015a0` FUN_004015a0 · depth 8 · 82 lines · **untouched**
- [ ] `00401160` FUN_00401160 · depth 9 · 163 lines · **untouched**
- [ ] `00401510` FUN_00401510 · depth 9 · 26 lines · **untouched**

### Cluster 2: 00401bc0–00401d80 (0/12 ported)

- [ ] `00401bc0` FUN_00401bc0 · depth 0 · 12 lines · **untouched**
- [ ] `00401c20` FUN_00401c20 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00401c50` FUN_00401c50 (vtable) · depth 0 · 30 lines · **untouched**
- [ ] `00401d60` FUN_00401d60 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `00401d80` FUN_00401d80 · depth 0 · 8 lines · **untouched**
- [ ] `00401860` FUN_00401860 · depth 1 · 86 lines · **untouched**
- [ ] `00401a50` FUN_00401a50 · depth 1 · 12 lines · **untouched**
- [ ] `00401a70` FUN_00401a70 · depth 1 · 15 lines · **untouched**
- [ ] `00401bf0` FUN_00401bf0 · depth 1 · 16 lines · **untouched**
- [ ] `00401d30` FUN_00401d30 · depth 1 · 14 lines · **untouched**
- [ ] `00401aa0` FUN_00401aa0 · depth 2 · 63 lines · **untouched**
- [ ] `00401c30` FUN_00401c30 (vtable) · depth 2 · 11 lines · **untouched**

### Cluster 3: 00401da0–00401fb0 (0/7 ported)

- [ ] `00401da0` FUN_00401da0 · depth 0 · 9 lines · **untouched**
- [ ] `00401dc0` FUN_00401dc0 · depth 0 · 13 lines · **untouched**
- [ ] `00401fa0` FUN_00401fa0 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00401fb0` FUN_00401fb0 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00401de0` FUN_00401de0 · depth 2 · 15 lines · **untouched**
- [ ] `00401e20` FUN_00401e20 (vtable) · depth 8 · 22 lines · **untouched**
- [ ] `00401ee0` FUN_00401ee0 (vtable) · depth 8 · 18 lines · **untouched**

### Cluster 4: 00402110–00402240 (0/7 ported)

- [ ] `00402110` FUN_00402110 · depth 0 · 9 lines · **untouched**
- [ ] `00402120` FUN_00402120 · depth 0 · 35 lines · **untouched**
- [ ] `00402200` FUN_00402200 · depth 0 · 12 lines · **untouched**
- [ ] `00402230` FUN_00402230 · depth 0 · 8 lines · **untouched**
- [ ] `00401fc0` FUN_00401fc0 · depth 1 · 57 lines · **untouched**
- [ ] `004021a0` FUN_004021a0 · depth 1 · 22 lines · **untouched**
- [ ] `00402240` FUN_00402240 · depth 2 · 42 lines · **untouched**

### Cluster 5: 004023f0–004029a0 (0/7 ported)

- [ ] `004023f0` FUN_004023f0 · depth 0 · 43 lines · **untouched**
- [ ] `004025b0` FUN_004025b0 · depth 1 · 49 lines · **untouched**
- [ ] `004028c0` FUN_004028c0 · depth 1 · 51 lines · **untouched**
- [ ] `00402440` FUN_00402440 (vtable) · depth 2 · 74 lines · **untouched**
- [ ] `004029a0` FUN_004029a0 · depth 2 · 14 lines · **untouched**
- [ ] `004026d0` FUN_004026d0 · depth 3 · 84 lines · **untouched**
- [ ] `00402300` FUN_00402300 · depth 4 · 46 lines · **untouched**

### Cluster 6: 00402e50–00403320 (0/12 ported)

- [ ] `00402e50` FUN_00402e50 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00402d80` FUN_00402d80 (vtable) · depth 1 · 24 lines · **untouched**
- [ ] `00402e60` FUN_00402e60 (vtable) · depth 1 · 8 lines · **untouched**
- [ ] `00402e80` FUN_00402e80 · depth 1 · 127 lines · **untouched**
- [ ] `004031a0` FUN_004031a0 (vtable) · depth 1 · 38 lines · **untouched**
- [ ] `00403220` FUN_00403220 · depth 1 · 11 lines · **untouched**
- [ ] `004030e0` FUN_004030e0 (vtable) · depth 2 · 35 lines · **untouched**
- [ ] `00403300` FUN_00403300 · depth 4 · 10 lines · **untouched**
- [ ] `004032e0` FUN_004032e0 · depth 5 · 8 lines · **untouched**
- [ ] `00403320` FUN_00403320 · depth 5 · 11 lines · **untouched**
- [ ] `00403250` FUN_00403250 · depth 10 · 28 lines · **untouched**
- [ ] `004029f0` FUN_004029f0 (vtable) · depth 12 · 156 lines · **untouched**

### Cluster 7: 00403500–00403a70 (0/12 ported)

- [ ] `00403500` FUN_00403500 · depth 0 · 59 lines · **untouched**
- [ ] `004036c0` FUN_004036c0 · depth 0 · 7 lines · **decompiled** — device ready check
- [ ] `004036e0` FUN_004036e0 · depth 0 · 9 lines · **untouched**
- [ ] `00403700` FUN_00403700 · depth 0 · 50 lines · **untouched**
- [ ] `004037c0` FUN_004037c0 · depth 0 · 23 lines · **untouched**
- [ ] `00403a70` inferred_MouseCenter · depth 0 · 9 lines · **decompiled**
- [ ] `004035b0` FUN_004035b0 · depth 1 · 42 lines · **untouched**
- [ ] `004038f0` FUN_004038f0 (vtable) · depth 1 · 40 lines · **untouched**
- [ ] `00403820` FUN_00403820 · depth 5 · 44 lines · **untouched**
- [ ] `00403980` inferred_MouseCtor · depth 5 · 46 lines · **decompiled** — DirectInput mouse, range 640x480, starts at (320,240)
- [ ] `00403360` FUN_00403360 · depth 6 · 10 lines · **untouched**
- [ ] `00403380` FUN_00403380 · depth 8 · 109 lines · **untouched**

### Cluster 8: 00403b60–004040d0 (0/7 ported)

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

- [ ] `004060b0` FUN_004060b0 · depth 0 · 8 lines · **untouched**
- [ ] `004060c0` FUN_004060c0 · depth 0 · 22 lines · **untouched**
- [ ] `00405630` FUN_00405630 · depth 1 · 108 lines · **untouched**
- [ ] `00406110` FUN_00406110 · depth 2 · 19 lines · **untouched**
- [ ] `00404810` FUN_00404810 · depth 12 · 691 lines · **untouched**
- [ ] `00405a60` FUN_00405a60 · depth 13 · 228 lines · **untouched**
- [ ] `00405fd0` FUN_00405fd0 · depth 13 · 44 lines · **untouched**
- [ ] `00405850` FUN_00405850 (vtable) · depth 14 · 70 lines · **untouched**
- [ ] `00406140` FUN_00406140 (vtable) · depth 14 · 12 lines · **untouched**

### Cluster 11: 004067a0–00407000 (0/8 ported)

- [ ] `004067a0` FUN_004067a0 · depth 0 · 67 lines · **untouched**
- [ ] `00406c50` FUN_00406c50 (vtable) · depth 1 · 44 lines · **untouched**
- [ ] `004069e0` FUN_004069e0 (vtable) · depth 2 · 35 lines · **untouched**
- [ ] `00406980` FUN_00406980 · depth 3 · 16 lines · **untouched**
- [ ] `00407000` FUN_00407000 (vtable) · depth 3 · 30 lines · **untouched**
- [ ] `00406a90` FUN_00406a90 · depth 12 · 88 lines · **untouched**
- [ ] `00406d50` FUN_00406d50 (vtable) · depth 12 · 123 lines · **untouched**
- [ ] `00406190` FUN_00406190 · depth 13 · 228 lines · **untouched**

### Cluster 12: 004079d0–00407aa0 (0/11 ported)

- [ ] `004079d0` FUN_004079d0 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00407aa0` FUN_00407aa0 · depth 0 · 10 lines · **untouched**
- [ ] `00407730` FUN_00407730 (vtable) · depth 1 · 15 lines · **untouched**
- [ ] `004079e0` FUN_004079e0 (vtable) · depth 1 · 31 lines · **untouched**
- [ ] `00407600` FUN_00407600 (vtable) · depth 2 · 55 lines · **untouched**
- [ ] `00407770` FUN_00407770 (vtable) · depth 2 · 10 lines · **untouched**
- [ ] `00407a80` FUN_00407a80 (vtable) · depth 4 · 10 lines · **untouched**
- [ ] `00407510` FUN_00407510 (vtable) · depth 8 · 12 lines · **untouched**
- [ ] `00407580` FUN_00407580 (vtable) · depth 8 · 14 lines · **untouched**
- [ ] `00407790` FUN_00407790 (vtable) · depth 11 · 89 lines · **untouched**
- [ ] `004070c0` FUN_004070c0 · depth 12 · 282 lines · **untouched**

### Cluster 13: 00408440–00408550 (0/12 ported)

- [ ] `00408440` FUN_00408440 · depth 0 · 61 lines · **untouched**
- [ ] `00407ac0` FUN_00407ac0 (vtable) · depth 1 · 34 lines · **untouched**
- [ ] `00407d00` FUN_00407d00 · depth 1 · 71 lines · **untouched**
- [ ] `00407f30` FUN_00407f30 (vtable) · depth 2 · 37 lines · **untouched**
- [ ] `004080a0` FUN_004080a0 (vtable) · depth 2 · 17 lines · **untouched**
- [ ] `00408520` FUN_00408520 · depth 2 · 8 lines · **untouched**
- [ ] `00408550` FUN_00408550 · depth 2 · 52 lines · **untouched**
- [ ] `00408030` FUN_00408030 (vtable) · depth 3 · 29 lines · **untouched**
- [ ] `00408540` FUN_00408540 · depth 3 · 8 lines · **untouched**
- [ ] `00407b80` FUN_00407b80 · depth 11 · 50 lines · **untouched**
- [ ] `00407ce0` FUN_00407ce0 · depth 11 · 8 lines · **untouched**
- [ ] `004080e0` FUN_004080e0 · depth 15 · 142 lines · **untouched**

### Cluster 14: 00408780–00408c50 (0/8 ported)

- [ ] `00408780` FUN_00408780 · depth 1 · 8 lines · **untouched**
- [ ] `00408840` FUN_00408840 (vtable) · depth 1 · 14 lines · **untouched**
- [ ] `00408c50` FUN_00408c50 · depth 1 · 256 lines · **untouched**
- [ ] `00408890` FUN_00408890 · depth 2 · 200 lines · **untouched**
- [ ] `004087d0` FUN_004087d0 · depth 3 · 10 lines · **untouched**
- [ ] `00408620` FUN_00408620 · depth 8 · 77 lines · **untouched**
- [ ] `00408710` FUN_00408710 · depth 8 · 20 lines · **untouched**
- [ ] `00408800` FUN_00408800 · depth 10 · 18 lines · **untouched**

### Cluster 15: 00409280–0040c090 (0/7 ported)

- [ ] `00409280` FUN_00409280 · depth 0 · 124 lines · **untouched**
- [ ] `00409560` FUN_00409560 · depth 0 · 142 lines · **untouched**
- [ ] `00409790` FUN_00409790 · depth 0 · 255 lines · **untouched**
- [ ] `00409c10` FUN_00409c10 · depth 0 · 113 lines · **untouched**
- [ ] `00409e50` FUN_00409e50 · depth 14 · 1145 lines · **untouched**
- [ ] `0040c040` FUN_0040c040 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c090` FUN_0040c090 (vtable) · depth 14 · 19 lines · **untouched**

### Cluster 16: 0040c0e0–0040c560 (0/12 ported)

- [ ] `0040c0e0` FUN_0040c0e0 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `0040c1f0` FUN_0040c1f0 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `0040c0f0` FUN_0040c0f0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c140` FUN_0040c140 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `0040c200` FUN_0040c200 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c250` FUN_0040c250 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `0040c2c0` FUN_0040c2c0 (vtable) · depth 14 · 31 lines · **untouched**
- [ ] `0040c370` FUN_0040c370 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c3c0` FUN_0040c3c0 (vtable) · depth 14 · 31 lines · **untouched**
- [ ] `0040c480` FUN_0040c480 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c4d0` FUN_0040c4d0 (vtable) · depth 14 · 27 lines · **untouched**
- [ ] `0040c560` FUN_0040c560 (vtable) · depth 14 · 23 lines · **untouched**

### Cluster 17: 0040c880–0040c880 (0/7 ported)

- [ ] `0040c880` FUN_0040c880 (vtable) · depth 13 · 13 lines · **untouched**
- [ ] `0040c5d0` FUN_0040c5d0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `0040c640` FUN_0040c640 (vtable) · depth 14 · 30 lines · **untouched**
- [ ] `0040c6d0` FUN_0040c6d0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c720` FUN_0040c720 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `0040c770` FUN_0040c770 (vtable) · depth 14 · 30 lines · **untouched**
- [ ] `0040c810` FUN_0040c810 (vtable) · depth 14 · 27 lines · **untouched**

### Cluster 18: 0040c8a0–0040ca60 (0/8 ported)

- [ ] `0040c8a0` FUN_0040c8a0 (vtable) · depth 1 · 9 lines · **untouched**
- [ ] `0040c8d0` FUN_0040c8d0 (vtable) · depth 2 · 13 lines · **untouched**
- [ ] `0040c9e0` FUN_0040c9e0 (vtable) · depth 2 · 14 lines · **untouched**
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

- [ ] `0040db40` FUN_0040db40 · depth 0 · 38 lines · **untouched**
- [ ] `0040dc00` FUN_0040dc00 · depth 0 · 7 lines · **untouched**
- [ ] `0040dc10` FUN_0040dc10 · depth 0 · 25 lines · **untouched**
- [ ] `0040dc70` FUN_0040dc70 · depth 0 · 36 lines · **untouched**
- [ ] `0040dcf0` FUN_0040dcf0 · depth 0 · 20 lines · **untouched**
- [ ] `0040dd30` FUN_0040dd30 · depth 0 · 8 lines · **untouched**
- [ ] `0040dd40` FUN_0040dd40 · depth 4 · 20 lines · **untouched**
- [ ] `0040dbc0` FUN_0040dbc0 · depth 5 · 11 lines · **untouched**
- [ ] `0040dbf0` FUN_0040dbf0 · depth 5 · 8 lines · **untouched**
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
- [ ] `0040e5c0` FUN_0040e5c0 · depth 3 · 30 lines · **untouched**
- [ ] `0040df70` FUN_0040df70 · depth 4 · 12 lines · **untouched**
- [ ] `0040e720` FUN_0040e720 · depth 4 · 10 lines · **untouched**
- [ ] `0040ddc0` FUN_0040ddc0 (vtable) · depth 14 · 73 lines · **untouched**
- [ ] `0040e000` FUN_0040e000 (vtable) · depth 15 · 142 lines · **untouched**

### Cluster 25: 0040ebd0–0040ee70 (0/9 ported)

- [ ] `0040ebd0` FUN_0040ebd0 (vtable) · depth 1 · 14 lines · **untouched**
- [ ] `0040ec00` FUN_0040ec00 (vtable) · depth 1 · 24 lines · **untouched**
- [ ] `0040ec90` FUN_0040ec90 · depth 1 · 9 lines · **untouched**
- [ ] `0040e760` FUN_0040e760 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `0040e890` FUN_0040e890 (vtable) · depth 3 · 103 lines · **untouched**
- [ ] `0040ecb0` FUN_0040ecb0 · depth 3 · 14 lines · **untouched**
- [ ] `0040ee70` FUN_0040ee70 (vtable) · depth 3 · 52 lines · **untouched**
- [ ] `0040e7e0` FUN_0040e7e0 · depth 4 · 25 lines · **untouched**
- [ ] `0040ed10` FUN_0040ed10 · depth 12 · 61 lines · **untouched**

### Cluster 26: 0040f020–0040f1a0 (0/7 ported)

- [ ] `0040f020` FUN_0040f020 (vtable) · depth 0 · 10 lines · **untouched**
- [ ] `0040f040` FUN_0040f040 (vtable) · depth 0 · 9 lines · **untouched**
- [ ] `0040f1a0` FUN_0040f1a0 (vtable) · depth 0 · 18 lines · **untouched**
- [ ] `0040f050` FUN_0040f050 (vtable) · depth 1 · 15 lines · **untouched**
- [ ] `0040f0b0` FUN_0040f0b0 (vtable) · depth 1 · 8 lines · **untouched**
- [ ] `0040efa0` FUN_0040efa0 (vtable) · depth 2 · 24 lines · **untouched**
- [ ] `0040f0d0` FUN_0040f0d0 (vtable) · depth 3 · 40 lines · **untouched**

### Cluster 27: 0040f670–0040f710 (0/11 ported)

- [ ] `0040f670` FUN_0040f670 · depth 0 · 12 lines · **untouched**
- [ ] `0040f680` FUN_0040f680 · depth 0 · 55 lines · **untouched**
- [ ] `0040f590` FUN_0040f590 · depth 1 · 37 lines · **untouched**
- [ ] `0040f650` FUN_0040f650 · depth 1 · 8 lines · **untouched**
- [ ] `0040f710` FUN_0040f710 · depth 2 · 39 lines · **untouched**
- [ ] `0040f210` FUN_0040f210 (vtable) · depth 11 · 16 lines · **untouched**
- [ ] `0040f280` FUN_0040f280 (vtable) · depth 12 · 25 lines · **untouched**
- [ ] `0040f300` FUN_0040f300 (vtable) · depth 12 · 25 lines · **untouched**
- [ ] `0040f380` FUN_0040f380 (vtable) · depth 12 · 25 lines · **untouched**
- [ ] `0040f400` FUN_0040f400 (vtable) · depth 12 · 25 lines · **untouched**
- [ ] `0040f480` FUN_0040f480 (vtable) · depth 12 · 50 lines · **untouched**

### Cluster 28: 0040f780–0040fb20 (0/7 ported)

- [ ] `0040f780` FUN_0040f780 · depth 0 · 49 lines · **untouched**
- [ ] `0040f800` FUN_0040f800 · depth 0 · 40 lines · **untouched**
- [ ] `0040f870` FUN_0040f870 · depth 0 · 14 lines · **untouched**
- [ ] `0040f900` FUN_0040f900 · depth 0 · 82 lines · **untouched**
- [ ] `0040fa10` FUN_0040fa10 · depth 0 · 82 lines · **untouched**
- [ ] `0040fb20` FUN_0040fb20 · depth 0 · 58 lines · **untouched**
- [ ] `0040f890` FUN_0040f890 · depth 1 · 23 lines · **untouched**

### Cluster 29: 0040fc20–004102e0 (0/12 ported)

- [ ] `0040fc20` FUN_0040fc20 · depth 0 · 83 lines · **untouched**
- [ ] `0040fd20` FUN_0040fd20 · depth 0 · 83 lines · **untouched**
- [ ] `0040fe20` FUN_0040fe20 · depth 0 · 14 lines · **untouched**
- [ ] `0040fe40` FUN_0040fe40 · depth 0 · 31 lines · **untouched**
- [ ] `0040fbb0` FUN_0040fbb0 · depth 2 · 38 lines · **untouched**
- [ ] `0040fea0` FUN_0040fea0 · depth 2 · 9 lines · **untouched**
- [ ] `0040ffa0` FUN_0040ffa0 · depth 2 · 27 lines · **untouched**
- [ ] `00410060` FUN_00410060 (vtable) · depth 2 · 34 lines · **untouched**
- [ ] `00410130` FUN_00410130 (vtable) · depth 2 · 41 lines · **untouched**
- [ ] `004102e0` FUN_004102e0 (vtable) · depth 3 · 20 lines · **untouched**
- [ ] `004101f0` FUN_004101f0 (vtable) · depth 10 · 46 lines · **untouched**
- [ ] `0040fec0` FUN_0040fec0 · depth 13 · 40 lines · **untouched**

### Cluster 30: 004103f0–004108a0 (0/9 ported)

- [ ] `004103f0` FUN_004103f0 (vtable) · depth 0 · 8 lines · **untouched**
- [ ] `004104a0` FUN_004104a0 · depth 0 · 44 lines · **untouched**
- [ ] `00410530` FUN_00410530 · depth 0 · 38 lines · **untouched**
- [ ] `004105a0` FUN_004105a0 · depth 0 · 177 lines · **untouched**
- [ ] `004108a0` FUN_004108a0 · depth 0 · 215 lines · **untouched**
- [ ] `00410400` FUN_00410400 (vtable) · depth 1 · 10 lines · **untouched**
- [ ] `00410420` FUN_00410420 · depth 1 · 26 lines · **untouched**
- [ ] `004103d0` FUN_004103d0 (vtable) · depth 3 · 9 lines · **untouched**
- [ ] `00410330` FUN_00410330 · depth 10 · 33 lines · **untouched**

### Cluster 31: 00410cc0–00411f80 (0/12 ported)

- [ ] `00410cc0` FUN_00410cc0 · depth 0 · 173 lines · **untouched**
- [ ] `00410fb0` FUN_00410fb0 · depth 0 · 177 lines · **untouched**
- [ ] `004112d0` FUN_004112d0 · depth 0 · 159 lines · **untouched**
- [ ] `00411580` FUN_00411580 · depth 0 · 164 lines · **untouched**
- [ ] `00411be0` FUN_00411be0 · depth 0 · 10 lines · **untouched**
- [ ] `00411c00` FUN_00411c00 · depth 0 · 7 lines · **untouched**
- [ ] `00411c10` FUN_00411c10 · depth 0 · 8 lines · **untouched**
- [ ] `00411c80` FUN_00411c80 · depth 0 · 127 lines · **untouched**
- [ ] `00411f20` FUN_00411f20 · depth 0 · 30 lines · **untouched**
- [ ] `00411870` FUN_00411870 · depth 1 · 127 lines · **untouched**
- [ ] `00411f80` FUN_00411f80 · depth 1 · 57 lines · **untouched**
- [ ] `00411c20` FUN_00411c20 · depth 8 · 22 lines · **untouched**

### Cluster 32: 004120c0–00412e10 (0/7 ported)

- [ ] `004120c0` FUN_004120c0 · depth 0 · 21 lines · **untouched**
- [ ] `00412640` FUN_00412640 · depth 0 · 282 lines · **untouched**
- [ ] `00412bb0` FUN_00412bb0 · depth 0 · 34 lines · **untouched**
- [ ] `00412c50` FUN_00412c50 · depth 0 · 99 lines · **untouched**
- [ ] `00412120` FUN_00412120 · depth 1 · 101 lines · **untouched**
- [ ] `004123e0` FUN_004123e0 · depth 1 · 118 lines · **untouched**
- [ ] `00412e10` FUN_00412e10 · depth 2 · 263 lines · **untouched**

### Cluster 33: 004137c0–004139e0 (0/7 ported)

- [ ] `004137c0` FUN_004137c0 · depth 0 · 67 lines · **untouched**
- [ ] `004138b0` FUN_004138b0 · depth 0 · 47 lines · **untouched**
- [ ] `004139c0` FUN_004139c0 · depth 0 · 14 lines · **untouched**
- [ ] `004139e0` FUN_004139e0 · depth 0 · 18 lines · **untouched**
- [ ] `004132b0` FUN_004132b0 · depth 11 · 123 lines · **untouched**
- [ ] `00413570` FUN_00413570 · depth 11 · 121 lines · **untouched**
- [ ] `00413940` FUN_00413940 · depth 11 · 27 lines · **untouched**

### Cluster 34: 00413a10–00414560 (0/12 ported)

- [ ] `00413a10` FUN_00413a10 · depth 0 · 19 lines · **untouched**
- [ ] `00413a50` FUN_00413a50 · depth 0 · 18 lines · **untouched**
- [ ] `00413a80` FUN_00413a80 · depth 0 · 8 lines · **untouched**
- [ ] `00413a90` FUN_00413a90 · depth 0 · 12 lines · **untouched**
- [ ] `00413ab0` FUN_00413ab0 · depth 0 · 8 lines · **untouched**
- [ ] `00413ac0` FUN_00413ac0 · depth 0 · 11 lines · **untouched**
- [ ] `00413b00` FUN_00413b00 · depth 0 · 13 lines · **untouched**
- [ ] `00413b90` inferred_InitSubsystems · depth 6 · 140 lines · **decompiled** — builds CD, input, sound, video objects; error dialogs
- [ ] `004144a0` FUN_004144a0 · depth 7 · 45 lines · **untouched**
- [ ] `00414420` FUN_00414420 · depth 10 · 21 lines · **untouched**
- [ ] `00414560` FUN_00414560 · depth 14 · 25 lines · **untouched**
- [ ] `00413e60` FUN_00413e60 · depth 16 · 252 lines · **untouched**

### Cluster 35: 00414740–00414dc0 (0/12 ported)

- [ ] `00414740` FUN_00414740 · depth 0 · 86 lines · **untouched**
- [ ] `00414600` FUN_00414600 · depth 1 · 9 lines · **untouched**
- [ ] `00414610` FUN_00414610 · depth 1 · 9 lines · **untouched**
- [ ] `004146b0` FUN_004146b0 (vtable) · depth 1 · 23 lines · **untouched**
- [ ] `00414a90` FUN_00414a90 (vtable) · depth 1 · 92 lines · **untouched**
- [ ] `00414d50` FUN_00414d50 (vtable) · depth 1 · 18 lines · **untouched**
- [ ] `004145f0` FUN_004145f0 · depth 3 · 8 lines · **untouched**
- [ ] `00414620` FUN_00414620 · depth 3 · 19 lines · **untouched**
- [ ] `00414930` FUN_00414930 · depth 3 · 81 lines · **untouched**
- [ ] `00414c40` FUN_00414c40 (vtable) · depth 12 · 27 lines · **untouched**
- [ ] `00414dc0` FUN_00414dc0 (vtable) · depth 12 · 12 lines · **untouched**
- [ ] `00414be0` FUN_00414be0 · depth 13 · 16 lines · **untouched**

### Cluster 36: 00414e80–00415440 (0/12 ported)

- [ ] `00414e80` FUN_00414e80 (vtable) · depth 0 · 31 lines · **untouched**
- [ ] `00414f10` FUN_00414f10 · depth 0 · 73 lines · **untouched**
- [ ] `00415120` FUN_00415120 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `00415130` FUN_00415130 (vtable) · depth 0 · 43 lines · **untouched**
- [ ] `00415200` FUN_00415200 · depth 0 · 23 lines · **untouched**
- [ ] `00415230` FUN_00415230 · depth 1 · 9 lines · **untouched**
- [ ] `00415290` FUN_00415290 (vtable) · depth 1 · 27 lines · **untouched**
- [ ] `004150d0` FUN_004150d0 · depth 3 · 17 lines · **untouched**
- [ ] `00415350` FUN_00415350 (vtable) · depth 3 · 40 lines · **untouched**
- [ ] `00415440` FUN_00415440 (vtable) · depth 3 · 17 lines · **untouched**
- [ ] `00414e00` FUN_00414e00 · depth 4 · 13 lines · **untouched**
- [ ] `00415250` FUN_00415250 · depth 4 · 10 lines · **untouched**

### Cluster 37: 004157c0–00415960 (0/8 ported)

- [ ] `004157c0` FUN_004157c0 (vtable) · depth 0 · 7 lines · **untouched**
- [ ] `004157b0` FUN_004157b0 (vtable) · depth 1 · 8 lines · **untouched**
- [ ] `004157d0` FUN_004157d0 · depth 2 · 34 lines · **untouched**
- [ ] `00415490` FUN_00415490 (vtable) · depth 3 · 85 lines · **untouched**
- [ ] `00415640` FUN_00415640 (vtable) · depth 3 · 64 lines · **untouched**
- [ ] `00415770` FUN_00415770 · depth 4 · 11 lines · **untouched**
- [ ] `00415880` FUN_00415880 · depth 7 · 37 lines · **untouched**
- [ ] `00415960` inferred_WinMain · depth 17 · 89 lines · **decompiled** — args: none=CD check+SETUP.EXE; '_$_$_ '+byte=game; 'Uninstall'

### Cluster 38: 00415c30–00416170 (0/12 ported)

- [ ] `00415c30` FUN_00415c30 · depth 0 · 11 lines · **untouched**
- [ ] `00415c40` FUN_00415c40 · depth 0 · 44 lines · **untouched**
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

- [ ] `00416300` FUN_00416300 · depth 12 · 86 lines · **untouched**
- [ ] `004169a0` FUN_004169a0 · depth 12 · 33 lines · **untouched**
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

### Cluster 40: 00416b50–004173e0 (0/11 ported)

- [ ] `00416b50` FUN_00416b50 · depth 2 · 15 lines · **untouched**
- [ ] `00417360` FUN_00417360 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `00416b80` FUN_00416b80 · depth 12 · 45 lines · **untouched**
- [ ] `00416c80` FUN_00416c80 · depth 12 · 71 lines · **untouched**
- [ ] `00416a20` FUN_00416a20 (vtable) · depth 14 · 26 lines · **untouched**
- [ ] `00416ac0` FUN_00416ac0 (vtable) · depth 14 · 27 lines · **untouched**
- [ ] `00416e30` FUN_00416e30 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `00416ee0` FUN_00416ee0 (vtable) · depth 14 · 106 lines · **untouched**
- [ ] `00417180` FUN_00417180 (vtable) · depth 14 · 34 lines · **untouched**
- [ ] `00417250` FUN_00417250 (vtable) · depth 14 · 47 lines · **untouched**
- [ ] `004173e0` FUN_004173e0 (vtable) · depth 14 · 35 lines · **untouched**

### Cluster 41: 004174b0–004178b0 (0/7 ported)

- [ ] `004174b0` FUN_004174b0 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `00417530` FUN_00417530 (vtable) · depth 2 · 22 lines · **untouched**
- [ ] `00417720` FUN_00417720 · depth 12 · 63 lines · **untouched**
- [ ] `004175b0` FUN_004175b0 · depth 13 · 10 lines · **untouched**
- [ ] `004175f0` FUN_004175f0 (vtable) · depth 14 · 19 lines · **untouched**
- [ ] `00417640` FUN_00417640 (vtable) · depth 14 · 44 lines · **untouched**
- [ ] `004178b0` FUN_004178b0 (vtable) · depth 14 · 37 lines · **untouched**

### Cluster 42: 004179b0–00418050 (0/10 ported)

- [ ] `004179b0` FUN_004179b0 (vtable) · depth 2 · 41 lines · **untouched**
- [ ] `00417c80` FUN_00417c80 · depth 2 · 10 lines · **untouched**
- [ ] `00417ad0` FUN_00417ad0 · depth 4 · 12 lines · **untouched**
- [ ] `00417ca0` FUN_00417ca0 (vtable) · depth 13 · 21 lines · **untouched**
- [ ] `00417d00` FUN_00417d00 (vtable) · depth 13 · 31 lines · **untouched**
- [ ] `00417dd0` thunk_FUN_00414c40 (vtable) · depth 13 · 10 lines · **untouched**
- [ ] `00417de0` FUN_00417de0 (vtable) · depth 14 · 32 lines · **untouched**
- [ ] `00417e90` FUN_00417e90 (vtable) · depth 14 · 69 lines · **untouched**
- [ ] `00418050` FUN_00418050 (vtable) · depth 14 · 24 lines · **untouched**
- [ ] `00417b40` FUN_00417b40 (vtable) · depth 15 · 49 lines · **untouched**

### Cluster 43: 004180b0–004187e0 (0/12 ported)

- [ ] `004180b0` FUN_004180b0 (vtable) · depth 13 · 21 lines · **untouched**
- [ ] `00418110` FUN_00418110 (vtable) · depth 13 · 31 lines · **untouched**
- [ ] `004181e0` FUN_004181e0 (vtable) · depth 14 · 35 lines · **untouched**
- [ ] `004182c0` FUN_004182c0 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `00418320` FUN_00418320 (vtable) · depth 14 · 33 lines · **untouched**
- [ ] `004183e0` FUN_004183e0 (vtable) · depth 14 · 40 lines · **untouched**
- [ ] `004184e0` FUN_004184e0 (vtable) · depth 14 · 25 lines · **untouched**
- [ ] `00418560` FUN_00418560 (vtable) · depth 14 · 27 lines · **untouched**
- [ ] `004185c0` FUN_004185c0 (vtable) · depth 14 · 34 lines · **untouched**
- [ ] `00418670` FUN_00418670 (vtable) · depth 14 · 54 lines · **untouched**
- [ ] `00418780` FUN_00418780 (vtable) · depth 14 · 23 lines · **untouched**
- [ ] `004187e0` FUN_004187e0 (vtable) · depth 14 · 25 lines · **untouched**

### Cluster 44: 00418c80–00418cb0 (0/8 ported)

- [ ] `00418c80` FUN_00418c80 (vtable) · depth 2 · 16 lines · **untouched**
- [ ] `00418c50` FUN_00418c50 (vtable) · depth 3 · 16 lines · **untouched**
- [ ] `00418c30` FUN_00418c30 (vtable) · depth 4 · 10 lines · **untouched**
- [ ] `00418860` FUN_00418860 (vtable) · depth 14 · 50 lines · **untouched**
- [ ] `00418990` FUN_00418990 (vtable) · depth 14 · 38 lines · **untouched**
- [ ] `00418aa0` FUN_00418aa0 (vtable) · depth 14 · 44 lines · **untouched**
- [ ] `00418b80` FUN_00418b80 (vtable) · depth 14 · 34 lines · **untouched**
- [ ] `00418cb0` FUN_00418cb0 (vtable) · depth 14 · 30 lines · **untouched**

### Cluster 45: 00418d20–004199a0 (0/12 ported)

- [ ] `00418d20` FUN_00418d20 · depth 0 · 76 lines · **untouched**
- [ ] `00419050` FUN_00419050 · depth 1 · 74 lines · **untouched**
- [ ] `004191c0` FUN_004191c0 · depth 2 · 46 lines · **untouched**
- [ ] `00418e90` FUN_00418e90 · depth 3 · 97 lines · **untouched**
- [ ] `004192c0` FUN_004192c0 · depth 12 · 24 lines · **untouched**
- [ ] `004193b0` FUN_004193b0 · depth 12 · 69 lines · **untouched**
- [ ] `004197a0` FUN_004197a0 · depth 12 · 51 lines · **untouched**
- [ ] `004198d0` FUN_004198d0 · depth 12 · 37 lines · **untouched**
- [ ] `004199a0` FUN_004199a0 · depth 12 · 21 lines · **untouched**
- [ ] `00419530` FUN_00419530 · depth 14 · 43 lines · **untouched**
- [ ] `00419620` FUN_00419620 (vtable) · depth 14 · 29 lines · **untouched**
- [ ] `004196c0` FUN_004196c0 (vtable) · depth 14 · 30 lines · **untouched**

### Cluster 46: 00419a10–00419f10 (0/8 ported)

- [ ] `00419a10` FUN_00419a10 · depth 12 · 31 lines · **untouched**
- [ ] `00419ac0` FUN_00419ac0 · depth 12 · 20 lines · **untouched**
- [ ] `00419b30` FUN_00419b30 · depth 12 · 17 lines · **untouched**
- [ ] `00419b90` FUN_00419b90 · depth 12 · 68 lines · **untouched**
- [ ] `00419cf0` FUN_00419cf0 · depth 12 · 16 lines · **untouched**
- [ ] `00419d40` FUN_00419d40 · depth 12 · 16 lines · **untouched**
- [ ] `00419d90` FUN_00419d90 · depth 12 · 74 lines · **untouched**
- [ ] `00419f10` FUN_00419f10 · depth 12 · 57 lines · **untouched**

## Runtime, CRT and import thunks

259 functions (C runtime, SEH helpers, import stubs). They are not ported one by one; mark `replaced` when a ScummVM service covers them or `understood` when the game depends on their behaviour.

- [ ] `0041a070` DirectDrawCreate · import-thunk · **untouched**
- [ ] `0041a076` DirectInputCreateA · import-thunk · **untouched**
- [ ] `0041b3b0` Ordinal_1 · import-thunk · **untouched**
- [ ] `0041b3c0` FUN_0041b3c0 · runtime · **untouched**
- [ ] `0041b3f0` FUN_0041b3f0 · runtime · **untouched**
- [ ] `0041b490` FUN_0041b490 · runtime · **untouched**
- [ ] `0041b4c0` FUN_0041b4c0 · runtime · **untouched**
- [ ] `0041b600` FUN_0041b600 · runtime · **untouched**
- [ ] `0041b620` FUN_0041b620 · runtime · **untouched**
- [ ] `0041b770` FUN_0041b770 · runtime · **untouched**
- [ ] `0041b7a0` FUN_0041b7a0 · runtime · **untouched**
- [ ] `0041b7c0` FUN_0041b7c0 · runtime · **untouched**
- [ ] `0041b840` _strncpy · runtime · **untouched**
- [ ] `0041b940` FUN_0041b940 · runtime · **untouched**
- [ ] `0041ba00` FUN_0041ba00 · runtime · **untouched**
- [ ] `0041ba10` FUN_0041ba10 · runtime · **untouched**
- [ ] `0041ba20` FUN_0041ba20 · runtime · **untouched**
- [ ] `0041ba60` FUN_0041ba60 · runtime · **untouched**
- [ ] `0041ba70` FUN_0041ba70 · runtime · **untouched**
- [ ] `0041bad0` FUN_0041bad0 · runtime · **untouched**
- [ ] `0041bb10` FUN_0041bb10 · runtime · **untouched**
- [ ] `0041bb70` FUN_0041bb70 · runtime · **untouched**
- [ ] `0041bba0` FUN_0041bba0 · runtime · **untouched**
- [ ] `0041bc70` FUN_0041bc70 · runtime · **untouched**
- [ ] `0041bd00` FUN_0041bd00 · runtime · **untouched**
- [ ] `0041bd80` __global_unwind2 · runtime · **untouched**
- [ ] `0041bda0` FUN_0041bda0 · runtime · **untouched**
- [ ] `0041bdc2` __local_unwind2 · runtime · **untouched**
- [ ] `0041be2a` __abnormal_termination · runtime · **untouched**
- [ ] `0041be4d` __NLG_Notify1 · runtime · **untouched**
- [ ] `0041be56` FUN_0041be56 · runtime · **untouched**
- [ ] `0041be70` FUN_0041be70 · runtime · **untouched**
- [ ] `0041be80` _strncmp · runtime · **untouched**
- [ ] `0041bec0` __fpmath · runtime · **untouched**
- [ ] `0041bee0` FUN_0041bee0 · runtime · **untouched**
- [ ] `0041bf20` __ftol · runtime · **untouched**
- [ ] `0041bf50` FUN_0041bf50 · runtime · **untouched**
- [ ] `0041bfa0` FUN_0041bfa0 · runtime · **untouched**
- [ ] `0041c030` FUN_0041c030 · runtime · **untouched**
- [ ] `0041c120` FUN_0041c120 · runtime · **untouched**
- [ ] `0041c140` FUN_0041c140 · runtime · **untouched**
- [ ] `0041c1b0` FUN_0041c1b0 · runtime · **untouched**
- [ ] `0041c1d0` FUN_0041c1d0 · runtime · **untouched**
- [ ] `0041c300` FUN_0041c300 · runtime · **untouched**
- [ ] `0041c320` FUN_0041c320 · runtime · **untouched**
- [ ] `0041c3b0` FUN_0041c3b0 · runtime · **untouched**
- [ ] `0041c3d0` entry · runtime · **untouched**
- [ ] `0041c570` __amsg_exit · runtime · **untouched**
- [ ] `0041c5a0` FUN_0041c5a0 · runtime · **untouched**
- [ ] `0041c5d0` FUN_0041c5d0 · runtime · **untouched**
- [ ] `0041c5f0` __exit · runtime · **untouched**
- [ ] `0041c610` FUN_0041c610 · runtime · **untouched**
- [ ] `0041c6c0` FUN_0041c6c0 · runtime · **untouched**
- [ ] `0041c6e0` FUN_0041c6e0 · runtime · **untouched**
- [ ] `0041c7a0` FUN_0041c7a0 · runtime · **untouched**
- [ ] `0041c7f0` FUN_0041c7f0 · runtime · **untouched**
- [ ] `0041c860` FUN_0041c860 · runtime · **untouched**
- [ ] `0041c870` FUN_0041c870 · runtime · **untouched**
- [ ] `0041c8f0` FUN_0041c8f0 · runtime · **untouched**
- [ ] `0041caa0` FUN_0041caa0 · runtime · **untouched**
- [ ] `0041cc90` FUN_0041cc90 · runtime · **untouched**
- [ ] `0041cef0` FUN_0041cef0 · runtime · **untouched**
- [ ] `0041cfa0` FUN_0041cfa0 · runtime · **untouched**
- [ ] `0041d0d0` FUN_0041d0d0 · runtime · **untouched**
- [ ] `0041d2f0` FUN_0041d2f0 · runtime · **untouched**
- [ ] `0041d4c0` FUN_0041d4c0 · runtime · **untouched**
- [ ] `0041d550` FUN_0041d550 · runtime · **untouched**
- [ ] `0041d5a0` FUN_0041d5a0 · runtime · **untouched**
- [ ] `0041d680` FUN_0041d680 · runtime · **untouched**
- [ ] `0041d6c0` FUN_0041d6c0 · runtime · **untouched**
- [ ] `0041d6e0` FUN_0041d6e0 · runtime · **untouched**
- [ ] `0041d730` FUN_0041d730 · runtime · **untouched**
- [ ] `0041d770` FUN_0041d770 · runtime · **untouched**
- [ ] `0041d990` FUN_0041d990 · runtime · **untouched**
- [ ] `0041d9c0` FUN_0041d9c0 · runtime · **untouched**
- [ ] `0041da90` FUN_0041da90 · runtime · **untouched**
- [ ] `0041dd40` FUN_0041dd40 · runtime · **untouched**
- [ ] `0041de10` FUN_0041de10 · runtime · **untouched**
- [ ] `0041ded0` FUN_0041ded0 · runtime · **untouched**
- [ ] `0041def0` FUN_0041def0 · runtime · **untouched**
- [ ] `0041df80` FUN_0041df80 · runtime · **untouched**
- [ ] `0041e040` FUN_0041e040 · runtime · **untouched**
- [ ] `0041e061` FUN_0041e061 · runtime · **untouched**
- [ ] `0041e06a` FUN_0041e06a · runtime · **untouched**
- [ ] `0041e0d0` FUN_0041e0d0 · runtime · **untouched**
- [ ] `0041e100` FUN_0041e100 · runtime · **untouched**
- [ ] `0041e310` FUN_0041e310 · runtime · **untouched**
- [ ] `0041e390` FUN_0041e390 · runtime · **untouched**
- [ ] `0041e3c0` __CallSettingFrame@12 · runtime · **untouched**
- [ ] `0041e410` FUN_0041e410 · runtime · **untouched**
- [ ] `0041e47e` _abort · runtime · **untouched**
- [ ] `0041e4a0` FUN_0041e4a0 · runtime · **untouched**
- [ ] `0041e50e` FUN_0041e50e · runtime · **untouched**
- [ ] `0041e530` __setdefaultprecision · runtime · **untouched**
- [ ] `0041e550` FUN_0041e550 · runtime · **untouched**
- [ ] `0041e5a0` FUN_0041e5a0 · runtime · **untouched**
- [ ] `0041e5d0` FUN_0041e5d0 · runtime · **untouched**
- [ ] `0041e630` FUN_0041e630 · runtime · **untouched**
- [ ] `0041e6a0` FUN_0041e6a0 · runtime · **untouched**
- [ ] `0041e6c0` FUN_0041e6c0 · runtime · **untouched**
- [ ] `0041e720` FUN_0041e720 · runtime · **untouched**
- [ ] `0041e860` FUN_0041e860 · runtime · **untouched**
- [ ] `0041e960` FUN_0041e960 · runtime · **untouched**
- [ ] `0041ea10` FUN_0041ea10 · runtime · **untouched**
- [ ] `0041ea40` FUN_0041ea40 · runtime · **untouched**
- [ ] `0041ea70` FUN_0041ea70 · runtime · **untouched**
- [ ] `0041eae0` FUN_0041eae0 · runtime · **untouched**
- [ ] `0041eb10` FUN_0041eb10 · runtime · **untouched**
- [ ] `0041f850` FUN_0041f850 · runtime · **untouched**
- [ ] `0041f890` FUN_0041f890 · runtime · **untouched**
- [ ] `0041f8c0` FUN_0041f8c0 · runtime · **untouched**
- [ ] `0041f8e0` FUN_0041f8e0 · runtime · **untouched**
- [ ] `0041f930` FUN_0041f930 · runtime · **untouched**
- [ ] `0041f990` FUN_0041f990 · runtime · **untouched**
- [ ] `0041fb90` FUN_0041fb90 · runtime · **untouched**
- [ ] `00420520` FUN_00420520 · runtime · **untouched**
- [ ] `00420570` FUN_00420570 · runtime · **untouched**
- [ ] `004205b0` FUN_004205b0 · runtime · **untouched**
- [ ] `004205f0` FUN_004205f0 · runtime · **untouched**
- [ ] `00420610` FUN_00420610 · runtime · **untouched**
- [ ] `00420630` FUN_00420630 · runtime · **untouched**
- [ ] `00420650` FUN_00420650 · runtime · **untouched**
- [ ] `00420740` FUN_00420740 · runtime · **untouched**
- [ ] `00420780` FUN_00420780 · runtime · **untouched**
- [ ] `00420970` FUN_00420970 · runtime · **untouched**
- [ ] `00420a60` FUN_00420a60 · runtime · **untouched**
- [ ] `00420c50` FUN_00420c50 · runtime · **untouched**
- [ ] `00420cc0` FUN_00420cc0 · runtime · **untouched**
- [ ] `00420e00` FUN_00420e00 · runtime · **untouched**
- [ ] `00420e50` FUN_00420e50 · runtime · **untouched**
- [ ] `00420e70` FUN_00420e70 · runtime · **untouched**
- [ ] `00420eb0` FUN_00420eb0 · runtime · **untouched**
- [ ] `00420fa0` FUN_00420fa0 · runtime · **untouched**
- [ ] `00421040` FUN_00421040 · runtime · **untouched**
- [ ] `00421250` FUN_00421250 · runtime · **untouched**
- [ ] `004213b0` FUN_004213b0 · runtime · **untouched**
- [ ] `004215a0` FUN_004215a0 · runtime · **untouched**
- [ ] `004215f0` FUN_004215f0 · runtime · **untouched**
- [ ] `00421650` FUN_00421650 · runtime · **untouched**
- [ ] `00421680` FUN_00421680 · runtime · **untouched**
- [ ] `00421690` FUN_00421690 · runtime · **untouched**
- [ ] `004216d0` FUN_004216d0 · runtime · **untouched**
- [ ] `00421795` __seh_longjmp_unwind@4 · runtime · **untouched**
- [ ] `004217b0` FUN_004217b0 · runtime · **untouched**
- [ ] `004217f0` FUN_004217f0 · runtime · **untouched**
- [ ] `004219d0` FUN_004219d0 · runtime · **untouched**
- [ ] `00421a40` FUN_00421a40 · runtime · **untouched**
- [ ] `00421b00` FUN_00421b00 · runtime · **untouched**
- [ ] `00421bb0` FUN_00421bb0 · runtime · **untouched**
- [ ] `00421c50` FUN_00421c50 · runtime · **untouched**
- [ ] `00421ca0` FUN_00421ca0 · runtime · **untouched**
- [ ] `00421d00` FUN_00421d00 · runtime · **untouched**
- [ ] `00421dc0` FUN_00421dc0 · runtime · **untouched**
- [ ] `00421de0` FUN_00421de0 · runtime · **untouched**
- [ ] `00421ed0` FUN_00421ed0 · runtime · **untouched**
- [ ] `00421f00` FUN_00421f00 · runtime · **untouched**
- [ ] `004222c0` FUN_004222c0 · runtime · **untouched**
- [ ] `00422430` FUN_00422430 · runtime · **untouched**
- [ ] `00422490` FUN_00422490 · runtime · **untouched**
- [ ] `00422560` FUN_00422560 · runtime · **untouched**
- [ ] `004225c0` FUN_004225c0 · runtime · **untouched**
- [ ] `00422620` FUN_00422620 · runtime · **untouched**
- [ ] `00422860` FUN_00422860 · runtime · **untouched**
- [ ] `004229e0` FUN_004229e0 · runtime · **untouched**
- [ ] `00422ab0` FUN_00422ab0 · runtime · **untouched**
- [ ] `00422ad0` FUN_00422ad0 · runtime · **untouched**
- [ ] `00422b30` FUN_00422b30 · runtime · **untouched**
- [ ] `00422b50` FUN_00422b50 · runtime · **untouched**
- [ ] `00422b60` FUN_00422b60 · runtime · **untouched**
- [ ] `00422b80` FUN_00422b80 · runtime · **untouched**
- [ ] `00422ba0` FUN_00422ba0 · runtime · **untouched**
- [ ] `00422bc0` FUN_00422bc0 · runtime · **untouched**
- [ ] `00422f00` _abort · runtime · **untouched**
- [ ] `00422f20` FUN_00422f20 · runtime · **untouched**
- [ ] `00422f60` FUN_00422f60 · runtime · **untouched**
- [ ] `00422f80` FUN_00422f80 · runtime · **untouched**
- [ ] `00423020` FUN_00423020 · runtime · **untouched**
- [ ] `004230b0` FUN_004230b0 · runtime · **untouched**
- [ ] `00423150` FUN_00423150 · runtime · **untouched**
- [ ] `00423250` FUN_00423250 · runtime · **untouched**
- [ ] `004232c0` FUN_004232c0 · runtime · **untouched**
- [ ] `00423330` FUN_00423330 · runtime · **untouched**
- [ ] `004233d0` FUN_004233d0 · runtime · **untouched**
- [ ] `004233f0` FUN_004233f0 · runtime · **untouched**
- [ ] `00423400` FUN_00423400 · runtime · **untouched**
- [ ] `00423420` FUN_00423420 · runtime · **untouched**
- [ ] `004234e0` FUN_004234e0 · runtime · **untouched**
- [ ] `004236b0` FUN_004236b0 · runtime · **untouched**
- [ ] `004236d0` FUN_004236d0 · runtime · **untouched**
- [ ] `004236f0` FUN_004236f0 · runtime · **untouched**
- [ ] `00423730` FUN_00423730 · runtime · **untouched**
- [ ] `00423770` FUN_00423770 · runtime · **untouched**
- [ ] `00423810` FUN_00423810 · runtime · **untouched**
- [ ] `00423890` FUN_00423890 · runtime · **untouched**
- [ ] `00423950` FUN_00423950 · runtime · **untouched**
- [ ] `00423960` FUN_00423960 · runtime · **untouched**
- [ ] `00423a60` FUN_00423a60 · runtime · **untouched**
- [ ] `00423a90` __allmul · runtime · **untouched**
- [ ] `00423ad0` __allshl · runtime · **untouched**
- [ ] `00423af0` FUN_00423af0 · runtime · **untouched**
- [ ] `00423b70` __aulldiv · runtime · **untouched**
- [ ] `00423be0` __aullrem · runtime · **untouched**
- [ ] `00423c60` FUN_00423c60 · runtime · **untouched**
- [ ] `00423c80` FUN_00423c80 · runtime · **untouched**
- [ ] `00423f30` FUN_00423f30 · runtime · **untouched**
- [ ] `004241a0` FUN_004241a0 · runtime · **untouched**
- [ ] `00424340` FUN_00424340 · runtime · **untouched**
- [ ] `004244a0` FUN_004244a0 · runtime · **untouched**
- [ ] `004244f0` FUN_004244f0 · runtime · **untouched**
- [ ] `004245a0` FUN_004245a0 · runtime · **untouched**
- [ ] `00424620` FUN_00424620 · runtime · **untouched**
- [ ] `004248a0` FUN_004248a0 · runtime · **untouched**
- [ ] `00424b60` FUN_00424b60 · runtime · **untouched**
- [ ] `00424b80` FUN_00424b80 · runtime · **untouched**
- [ ] `00424c10` FUN_00424c10 · runtime · **untouched**
- [ ] `00424cb0` FUN_00424cb0 · runtime · **untouched**
- [ ] `00424d20` FUN_00424d20 · runtime · **untouched**
- [ ] `00424ea0` FUN_00424ea0 · runtime · **untouched**
- [ ] `00425020` FUN_00425020 · runtime · **untouched**
- [ ] `00425070` FUN_00425070 · runtime · **untouched**
- [ ] `004251a0` FUN_004251a0 · runtime · **untouched**
- [ ] `004251d0` FUN_004251d0 · runtime · **untouched**
- [ ] `00425240` FUN_00425240 · runtime · **untouched**
- [ ] `00425270` FUN_00425270 · runtime · **untouched**
- [ ] `004252a0` FUN_004252a0 · runtime · **untouched**
- [ ] `004253a0` FUN_004253a0 · runtime · **untouched**
- [ ] `00425b30` FUN_00425b30 · runtime · **untouched**
- [ ] `00425ec0` FUN_00425ec0 · runtime · **untouched**
- [ ] `00425f60` FUN_00425f60 · runtime · **untouched**
- [ ] `00426150` FUN_00426150 · runtime · **untouched**
- [ ] `00426190` FUN_00426190 · runtime · **untouched**
- [ ] `00426230` _strchr · runtime · **untouched**
- [ ] `004262f0` _strrchr · runtime · **untouched**
- [ ] `00426320` FUN_00426320 · runtime · **untouched**
- [ ] `004263a0` FUN_004263a0 · runtime · **untouched**
- [ ] `00426660` FUN_00426660 · runtime · **untouched**
- [ ] `004266f0` FUN_004266f0 · runtime · **untouched**
- [ ] `00426730` FUN_00426730 · runtime · **untouched**
- [ ] `004267b0` FUN_004267b0 · runtime · **untouched**
- [ ] `00426a80` FUN_00426a80 · runtime · **untouched**
- [ ] `00426c90` FUN_00426c90 · runtime · **untouched**
- [ ] `00426d10` FUN_00426d10 · runtime · **untouched**
- [ ] `00426d80` FUN_00426d80 · runtime · **untouched**
- [ ] `00426f20` FUN_00426f20 · runtime · **untouched**
- [ ] `00426f70` RtlUnwind · runtime · **untouched**
- [ ] `00426f80` FUN_00426f80 · runtime · **untouched**
- [ ] `00426fa0` FUN_00426fa0 · runtime · **untouched**
- [ ] `00426fc0` FUN_00426fc0 · runtime · **untouched**
- [ ] `00426fce` FUN_00426fce · runtime · **untouched**
- [ ] `00426ff0` FUN_00426ff0 · runtime · **untouched**
- [ ] `00427010` FUN_00427010 · runtime · **untouched**
- [ ] `0042701b` FUN_0042701b · runtime · **untouched**
- [ ] `00427026` FUN_00427026 · runtime · **untouched**
- [ ] `00427031` FUN_00427031 · runtime · **untouched**
- [ ] `0042703c` FUN_0042703c · runtime · **untouched**
- [ ] `00427047` FUN_00427047 · runtime · **untouched**
- [ ] `00427052` FUN_00427052 · runtime · **untouched**
- [ ] `00427070` FUN_00427070 · runtime · **untouched**
- [ ] `00427090` FUN_00427090 · runtime · **untouched**
