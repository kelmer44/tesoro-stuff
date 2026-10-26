/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040efa0
 */


void __thiscall tesoro_object_method_a0(void *this,int param_1)

{
  short *psVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar3 = *(int *)((int)this + 0x38);
  piVar4 = *(int **)(*(int *)(iVar3 + 0x1c) + 0x18 + (uint)*(ushort *)(iVar3 + 0x30) * 0x24);
  psVar1 = (short *)(*(int *)(iVar3 + 0x1c) + (uint)*(ushort *)(iVar3 + 0x30) * 0x24);
  if (piVar4 != (int *)0x0) {
    iVar2 = (*(int *)((int)this + 0x4c) - *(int *)(param_1 + 0x34)) + (int)*(short *)(iVar3 + 0x28)
            + 0x140 + (int)*psVar1;
    if (iVar2 < 0x281) {
      FUN_00411f80(DAT_00442108,iVar2,
                   (((*(int *)(param_1 + 0x38) - (int)psVar1[1]) - *(int *)((int)this + 0x50)) -
                   (int)*(short *)(iVar3 + 0x2a)) + 0xf0,*(ushort *)((int)this + 0x5e),*piVar4,0);
    }
  }
  return;
}

