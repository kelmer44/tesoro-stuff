/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410060
 */


void __thiscall tesoro_character_virtual_7c(void *this,char param_1)

{
  short *psVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = *(int *)((int)this + 0x38);
  psVar1 = *(short **)(iVar2 + 0x1c);
  if (*(int **)(psVar1 + 0xc) != (int *)0x0) {
    iVar3 = *(int *)(*(int *)((int)this + 0x40) + 0x80);
    FUN_00411f80(DAT_00442108,
                 ((*(int *)((int)this + 0x4c) + (int)*(short *)(iVar2 + 0x28)) -
                 *(int *)(iVar3 + 0x34)) + 0x140 + (int)*psVar1,
                 (((*(int *)(iVar3 + 0x38) - *(int *)((int)this + 0x50)) - (int)psVar1[1]) -
                 (int)*(short *)(iVar2 + 0x2a)) + 0xf0,*(short *)((int)this + 0x5e) + 1,
                 **(int **)(psVar1 + 0xc),0);
  }
  if (param_1 == '\0') {
    FUN_00401740(*(void **)((int)this + 0x38),1,1,0,'\0');
  }
  else {
    iVar2 = FUN_00413a10(DAT_00442120);
    if (100 < (uint)(iVar2 - *(int *)((int)this + 0xa8))) {
      iVar3 = FUN_00413b00(1,*(ushort *)(*(int *)((int)this + 0x38) + 0x22) - 1);
      FUN_00401740(*(void **)((int)this + 0x38),(ushort)iVar3,iVar3,0,'\0');
      *(int *)((int)this + 0xa8) = iVar2;
      return;
    }
  }
  return;
}

