/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401c50
 */


void __fastcall tesoro_recruiter_virtual_0c(int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1[0xe];
  *(undefined2 *)(param_1 + 0xc) = *(undefined2 *)(iVar1 + 0x30);
  param_1[8] = (int)*(short *)(iVar1 + 0x28) + param_1[0x13];
  param_1[9] = (int)*(short *)(iVar1 + 0x2a) + param_1[0x14];
  (**(code **)(*param_1 + 8))();
  iVar1 = param_1[0xe];
  param_1[10] = (int)*(short *)(iVar1 + 0x28) + param_1[0x13];
  param_1[0xb] = param_1[0x14] + (int)*(short *)(iVar1 + 0x2a);
  if ((param_1[0x15] != param_1[0x13]) || (param_1[0x16] != param_1[0x14])) {
    param_1[10] = param_1[0x15] + (int)*(short *)(iVar1 + 0x28);
    param_1[0xb] = param_1[0x16] + (int)*(short *)(iVar1 + 0x2a);
  }
  iVar2 = (int)*(short *)(*(int *)(iVar1 + 0x1c) + 0xc + (uint)*(ushort *)(iVar1 + 0x30) * 0x24);
  param_1[4] = iVar2;
  param_1[6] = *(short *)(*(int *)(iVar1 + 0x1c) + 0x10 + (uint)*(ushort *)(iVar1 + 0x30) * 0x24) +
               iVar2;
  iVar2 = (int)*(short *)(*(int *)(iVar1 + 0x1c) + 0xe + (uint)*(ushort *)(iVar1 + 0x30) * 0x24);
  param_1[5] = iVar2;
  param_1[7] = iVar2 - *(short *)(*(int *)(iVar1 + 0x1c) + 0x12 +
                                 (uint)*(ushort *)(iVar1 + 0x30) * 0x24);
  return;
}

