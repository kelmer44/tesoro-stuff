/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00406c50
 */


undefined4 __fastcall item_update(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (((char)param_1[0x23] == '\0') || (*(char *)(DAT_004420fc + 0xd4) != '\0')) {
    return 0;
  }
  (**(code **)(*DAT_00442128 + 0x20))(1);
  *(undefined1 *)(param_1 + 0x23) = 0;
  FUN_0040fb20(DAT_00442104,(int)param_1);
  switch(param_1[0x27]) {
  case 0:
    uVar2 = (**(code **)(*param_1 + 0x50))();
    return uVar2;
  case 1:
    uVar2 = (**(code **)(*param_1 + 0x54))();
    return uVar2;
  case 2:
    uVar2 = (**(code **)(*param_1 + 0x5c))();
    return uVar2;
  case 3:
    uVar2 = (**(code **)(*param_1 + 0x58))();
    return uVar2;
  case 4:
    uVar2 = (**(code **)(*param_1 + 0x60))();
    return uVar2;
  case 5:
    uVar2 = (**(code **)(*param_1 + 100))();
    return uVar2;
  case 6:
    uVar2 = (**(code **)(*param_1 + 0x68))();
    return uVar2;
  }
  iVar1 = (**(code **)(*(int *)DAT_00442128[0x4b] + 0x6c))(*(undefined2 *)((int)param_1 + 0xaa));
  if (iVar1 != 0) {
    return 1;
  }
  uVar2 = (**(code **)(*param_1 + 0x6c))(*(undefined2 *)(DAT_00442128[0x4b] + 0xaa));
  return uVar2;
}

