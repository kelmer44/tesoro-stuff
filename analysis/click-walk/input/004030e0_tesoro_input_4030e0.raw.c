/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004030e0
 */


int __fastcall tesoro_input_4030e0(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = FUN_00402e80(param_1);
  FUN_0040f870(*(undefined4 **)(param_1 + 0x70));
  if (iVar5 != 0) {
    FUN_0040f870(*(undefined4 **)(*(int *)(param_1 + 0x74) + 0x70));
    return iVar5;
  }
  iVar5 = FUN_00402e80(*(int *)(param_1 + 0x74));
  FUN_0040f870(*(undefined4 **)(*(int *)(param_1 + 0x74) + 0x70));
  if ((iVar5 != 2) && (iVar5 != 3)) {
    iVar1 = *(int *)(DAT_0044206c + 0x38);
    iVar2 = *(int *)(*(int *)(DAT_004420fc[0x10] + 0x80) + 0x2c);
    iVar3 = *(int *)(*(int *)(DAT_004420fc[0x10] + 0x80) + 0x30);
    iVar4 = *(int *)(DAT_0044206c + 0x3c);
    if ((((**(byte **)(DAT_0044206c + 0x1c) & 8) != 0) && (iVar5 == 0)) &&
       (*(short *)(*(int *)(param_1 + 0x38) + 0x30) == 0)) {
      if ((char)DAT_004420fc[0x35] != '\0') {
        (**(code **)(*DAT_004420fc + 0x70))();
      }
      (**(code **)(*DAT_004420fc + 0x80))(iVar1 + iVar2,iVar3 - iVar4,0,0);
    }
    return 1;
  }
  return iVar5;
}

