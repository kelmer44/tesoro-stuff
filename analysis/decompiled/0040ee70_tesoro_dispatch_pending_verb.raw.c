/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040ee70
 */


undefined4 __fastcall tesoro_dispatch_pending_verb(int *param_1)

{
  uint *puVar1;
  undefined4 uVar2;
  
  if (((char)param_1[0x23] != '\0') && (*(char *)(DAT_004420fc + 0xd4) == '\0')) {
    (**(code **)(*DAT_00442128 + 0x20))(1);
    *(undefined1 *)(param_1 + 0x23) = 0;
    FUN_0040fb20(DAT_00442104,(int)param_1);
    if (*(char *)((int)param_1 + 0x8d) == '\0') {
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
      if (DAT_00442128[0x4b] != 0) {
        puVar1 = (uint *)(DAT_00442128[0x4b] + 0x68);
        *puVar1 = *puVar1 | 4;
      }
      DAT_00442128[0x4b] = 0;
      FUN_00401de0(DAT_00442128,0,0,0,0,'\0');
      uVar2 = (**(code **)(*param_1 + 0x6c))(param_1[0x27] + -7);
      return uVar2;
    }
    *(undefined1 *)((int)param_1 + 0x8d) = 0;
    (**(code **)(*param_1 + 0xc))();
    return 1;
  }
  (**(code **)(*param_1 + 0xc))();
  return 1;
}

