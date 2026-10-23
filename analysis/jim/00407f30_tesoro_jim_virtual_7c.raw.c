/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407f30
 */


void __thiscall tesoro_jim_virtual_7c(void *this,char param_1)

{
  short *psVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  FUN_00401860(*(int *)((int)this + 0x38));
  iVar3 = *(int *)((int)this + 0x38);
  psVar1 = *(short **)(iVar3 + 0x1c);
  if (*(undefined4 **)(psVar1 + 0xc) != (undefined4 *)0x0) {
    iVar4 = *(int *)((int)this + 0xfc);
    iVar2 = *(int *)(*(int *)((int)this + 0x40) + 0x80);
    FUN_004120c0(DAT_00442108,
                 (((*psVar1 * iVar4 >> 8) + *(int *)((int)this + 0x4c)) - *(int *)(iVar2 + 0x34)) +
                 0x140 + (int)*(short *)(iVar3 + 0x28),
                 (((*(int *)(iVar2 + 0x38) - (psVar1[1] * iVar4 >> 8)) - *(int *)((int)this + 0x50))
                 - (int)*(short *)(iVar3 + 0x2a)) + 0xf0,*(short *)((int)this + 0x5e) + 1,
                 **(undefined4 **)(psVar1 + 0xc),iVar4 << 8,iVar4 << 8,0,0);
  }
  if (param_1 == '\0') {
    FUN_00401740(*(void **)((int)this + 0x38),1,1,0,'\0');
  }
  else {
    iVar3 = FUN_00413a10(DAT_00442120);
    if (100 < (uint)(iVar3 - *(int *)((int)this + 0xa8))) {
      iVar4 = FUN_00413b00(1,*(ushort *)(*(int *)((int)this + 0x38) + 0x22) - 1);
      FUN_00401740(*(void **)((int)this + 0x38),(ushort)iVar4,iVar4,0,'\0');
      *(int *)((int)this + 0xa8) = iVar3;
      return;
    }
  }
  return;
}

