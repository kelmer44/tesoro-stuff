/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00402240
 */


undefined4 * __fastcall tesoro_probe_ctor(undefined4 *param_1)

{
  void *pvVar1;
  undefined4 *puVar2;
  
  FUN_0040e5a0(param_1);
  *param_1 = &PTR_LAB_00428038;
  param_1[0x48] = 0;
  if (DAT_00442060 == (undefined4 *)0x0) {
    pvVar1 = (void *)FUN_0040f650(4,1);
    if (pvVar1 == (void *)0x0) {
      DAT_00442060 = (undefined4 *)0x0;
    }
    else {
      DAT_00442060 = FUN_0040f590(pvVar1,0x1e,1,(int *)0x0);
    }
  }
  *(undefined2 *)((int)param_1 + 0x5e) = 0;
  *(undefined1 *)(param_1 + 0x1e) = 0;
  pvVar1 = (void *)FUN_0040f650(4,1);
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_0040f590(pvVar1,0x1e,1,(int *)0x0);
  }
  param_1[0x1b] = puVar2;
  pvVar1 = (void *)FUN_0040f650(4,1);
  if (pvVar1 != (void *)0x0) {
    puVar2 = FUN_0040f590(pvVar1,0x1e,1,(int *)0x0);
    param_1[0x1c] = puVar2;
    param_1[0x1d] = 0;
    param_1[0x1a] = param_1[0x1a] | 8;
    return param_1;
  }
  param_1[0x1d] = 0;
  param_1[0x1c] = 0;
  param_1[0x1a] = param_1[0x1a] | 8;
  return param_1;
}

