/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00402d80
 */


void __thiscall tesoro_cursor_402d80(void *this,int param_1)

{
  short *psVar1;
  int iVar2;
  
  iVar2 = *(int *)((int)this + 0x38);
  psVar1 = (short *)(*(int *)(iVar2 + 0x1c) + (uint)*(ushort *)(iVar2 + 0x30) * 0x24);
  FUN_004120c0(DAT_00442108,
               (((int)*(short *)(iVar2 + 0x28) + (int)*psVar1 + *(int *)((int)this + 0x4c)) -
               *(int *)(param_1 + 0x34)) + 0x140,
               (((*(int *)(param_1 + 0x38) - (int)psVar1[1]) - (int)*(short *)(iVar2 + 0x2a)) -
               *(int *)((int)this + 0x50)) + 0xf0,*(undefined2 *)((int)this + 0x5e),
               **(undefined4 **)(psVar1 + 0xc),0x10000,0x10000,0,0);
  if (*(char *)((int)this + 0x78) != '\0') {
    iVar2 = (*(int *)((int)this + 0x4c) - *(int *)(param_1 + 0x34)) + 0x140;
    FUN_00411c80(DAT_00442108,(byte *)((int)this + 0x78),
                 CONCAT22((short)((uint)iVar2 >> 0x10),*(undefined2 *)((int)this + 0x118)),0,iVar2,
                 (*(int *)(param_1 + 0x38) - *(int *)((int)this + 0x50)) + 0xcb,200,0,'\x01',0);
  }
  return;
}

