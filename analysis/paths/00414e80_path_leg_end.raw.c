/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414e80
 */


void __fastcall path_leg_end(int param_1)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = *(ushort *)(*(int *)(param_1 + 0x38) + 0x30);
  iVar2 = *(int *)(*(int *)(param_1 + 0x38) + 0x1c);
  iVar3 = *(int *)(param_1 + 0xa0);
  if (iVar3 == *(int *)(param_1 + 0xa4)) {
    (**(code **)(*DAT_004420fc + 0x80))
              (*(undefined4 *)(param_1 + 0xa8),*(undefined4 *)(param_1 + 0xac),
               *(undefined4 *)(param_1 + 0xb0),1);
    return;
  }
  if (iVar3 < *(int *)(param_1 + 0xa4)) {
    iVar3 = iVar3 + 1;
  }
  else {
    iVar3 = iVar3 + -1;
  }
  *(int *)(param_1 + 0xa0) = iVar3;
  iVar2 = *(int *)(iVar2 + (uint)uVar1 * 0x24 + 0x20);
  (**(code **)(*DAT_004420fc + 0x80))
            (*(int *)(param_1 + 0x4c) + (int)*(short *)(iVar2 + iVar3 * 4),
             *(int *)(param_1 + 0x50) + (int)*(short *)(iVar2 + iVar3 * 4 + 2),param_1,1);
  return;
}

