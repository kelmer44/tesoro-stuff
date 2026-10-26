/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f280
 */


undefined4 __fastcall tesoro_object_verb_54(int param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  iVar1 = *(int *)(param_1 + 0x74);
  *(undefined1 *)(param_1 + 0x8d) = 1;
  if (iVar1 == 0) {
    uVar2 = (uint)*(ushort *)((int)DAT_004420fc + 0xd0);
    iVar1 = FUN_00413940(0x42d61c);
  }
  else {
    uVar2 = (uint)*(ushort *)((int)DAT_004420fc + 0xd0);
  }
  FUN_00410330(DAT_004420fc,iVar1,uVar2,uVar3);
  (**(code **)(*DAT_00442128 + 0x20))(0);
  *(undefined1 *)(param_1 + 0x8c) = 1;
  FUN_0040f900(DAT_00442104,param_1,5);
  return 3;
}

