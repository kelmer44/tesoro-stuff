/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004101f0
 */


undefined4 __fastcall tesoro_mouse_4101f0(int *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  bool bVar4;
  
  if ((char)param_1[0x35] == '\0') {
    if (*(char *)((int)param_1 + 0xd5) == '\0') {
      uVar3 = tesoro_menu_method_10(param_1);
      return uVar3;
    }
    *(undefined1 *)((int)param_1 + 0xd5) = 0;
    return 0;
  }
  bVar4 = false;
  if (((**(byte **)(DAT_00442074 + 0x34) & 1) == 0) &&
     ((*(byte *)(*(int *)(DAT_0044206c + 0x1c) + 1) & 1) == 0)) {
    if (param_1[0x32] != 0) {
      iVar1 = FUN_004040d0(DAT_00442080,param_1[0x32]);
      bVar4 = iVar1 == 0;
    }
  }
  else {
    bVar4 = true;
    if (param_1[0x32] != 0) {
      puVar2 = (undefined4 *)FUN_004040d0(DAT_00442080,param_1[0x32]);
      if (puVar2 != (undefined4 *)0x0) {
        FUN_004043b0(puVar2);
        FUN_00404210((int)DAT_00442080);
      }
    }
  }
  if ((param_1[0x31] != 0) && (!bVar4)) {
    tesoro_menu_method_58();
    if (*(char *)(DAT_004420fc + 0x112) != '\0') {
      FUN_0040ffa0(param_1,param_1[0x31]);
    }
    return 1;
  }
  (**(code **)(*param_1 + 0x70))();
  return 3;
}

