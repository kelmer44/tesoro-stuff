/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f0d0
 */


void __fastcall tesoro_object_method_d0(int *param_1)

{
  uint *puVar1;
  
  (**(code **)(*DAT_004420fc + 0x74))(param_1[0x24]);
  switch(param_1[0x27]) {
  case 0:
    (**(code **)(*param_1 + 0x50))();
    return;
  case 1:
    (**(code **)(*param_1 + 0x54))();
    return;
  case 2:
    (**(code **)(*param_1 + 0x5c))();
    return;
  case 3:
    (**(code **)(*param_1 + 0x58))();
    return;
  case 4:
    (**(code **)(*param_1 + 0x60))();
    return;
  case 5:
    (**(code **)(*param_1 + 100))();
    return;
  case 6:
    (**(code **)(*param_1 + 0x68))();
    return;
  }
  if (*(int *)((int)DAT_00442128 + 300) != 0) {
    puVar1 = (uint *)(*(int *)((int)DAT_00442128 + 300) + 0x68);
    *puVar1 = *puVar1 | 4;
  }
  *(undefined4 *)((int)DAT_00442128 + 300) = 0;
  FUN_00401de0(DAT_00442128,0,0,0,0,'\0');
  (**(code **)(*param_1 + 0x6c))(param_1[0x27] + -7);
  return;
}

