/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040e6b0
 */


void __thiscall tesoro_draw_scenery(void *this,int param_1)

{
  short *psVar1;
  int iVar2;
  
  iVar2 = *(int *)((int)this + 0x38);
  psVar1 = (short *)(*(int *)(iVar2 + 0x1c) + (uint)*(ushort *)(iVar2 + 0x30) * 0x24);
  if (*(int **)(psVar1 + 0xc) != (int *)0x0) {
    tesoro_queue_sprite(DAT_00442108,
                        (*(int *)((int)this + 0x4c) - *(int *)(param_1 + 0x34)) +
                        (int)*(short *)(iVar2 + 0x28) + 0x140 + (int)*psVar1,
                        (((*(int *)(param_1 + 0x38) - *(int *)((int)this + 0x50)) - (int)psVar1[1])
                        - (int)*(short *)(iVar2 + 0x2a)) + 0xf0,*(ushort *)((int)this + 0x5e),
                        **(int **)(psVar1 + 0xc),0);
  }
  return;
}

