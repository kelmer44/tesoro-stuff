/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040ffa0
 */


void __thiscall tesoro_queue_speech_text(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = *(int *)((int)this + 0xa0);
  iVar2 = *(int *)((int)this + 0xa4);
  if (*(int *)((int)this + 0xcc) < 1) {
    tesoro_layout_text(DAT_00442108,*(byte **)(param_1 + 0xc),
                       (uint)(ushort)-*(short *)((int)this + 0xcc),0,0x140,0xf0,0x280,0,'\x01',0);
    return;
  }
  FUN_00401fc0(*(void **)(*(int *)((int)this + 0x40) + 0x80),
               *(int *)(*(int *)((int)this + 0x3c) + 4));
  iVar3 = *(int *)(*(int *)((int)this + 0x40) + 0x80);
  iVar4 = *(int *)(iVar3 + 0x34);
  tesoro_layout_text(DAT_00442108,*(byte **)(param_1 + 0xc),
                     CONCAT22((short)((uint)iVar4 >> 0x10),*(undefined2 *)((int)this + 0xcc)),0,
                     iVar1 - iVar4,iVar2 + *(int *)(iVar3 + 0x38),
                     (uint)*(ushort *)((int)this + 0xd2),0,'\x01',0);
  return;
}

