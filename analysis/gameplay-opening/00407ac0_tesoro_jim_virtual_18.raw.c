/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407ac0
 */


void __thiscall tesoro_jim_virtual_18(void *this,int param_1)

{
  short *psVar1;
  int iVar2;
  undefined4 *puVar3;
  uint uVar4;
  
  if (*(char *)((int)this + 0xd4) == '\0') {
    if ((*(char *)((int)this + 0xc) != '\0') && (*(char *)((int)this + 0xd5) == '\0')) {
                    /* WARNING: Load size is inaccurate */
      (**(code **)(*this + 0x7c))(0);
    }
  }
  else {
                    /* WARNING: Load size is inaccurate */
    (**(code **)(*this + 0x7c))(0 < *(int *)((int)this + 0xcc));
  }
  uVar4 = (uint)*(ushort *)(*(int *)((int)this + 0x38) + 0x30);
  iVar2 = *(int *)(*(int *)((int)this + 0x38) + 0x1c);
  puVar3 = *(undefined4 **)(iVar2 + 0x18 + uVar4 * 0x24);
  psVar1 = (short *)(iVar2 + uVar4 * 0x24);
  if (puVar3 != (undefined4 *)0x0) {
    iVar2 = *(int *)((int)this + 0xfc);
    FUN_004120c0(DAT_00442108,
                 ((*psVar1 * iVar2 >> 8) - *(int *)(param_1 + 0x34)) + 0x140 +
                 *(int *)((int)this + 0x4c),
                 ((*(int *)(param_1 + 0x38) - (psVar1[1] * iVar2 >> 8)) - *(int *)((int)this + 0x50)
                 ) + 0xf0,*(undefined2 *)((int)this + 0x5e),*puVar3,iVar2 << 8,iVar2 << 8,0,0);
  }
  return;
}

