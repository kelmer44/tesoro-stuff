/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407000
 */


void __thiscall item_activate(void *this,byte param_1)

{
  uint *puVar1;
  int iVar2;
  
  if (param_1 == 0) {
    if (*(void **)((int)DAT_00442128 + 300) == this) {
      play(DAT_00442128,0,0,0,0,'\0');
      *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 4;
      *(undefined4 *)((int)DAT_00442128 + 300) = 0;
    }
  }
  else {
    if (0x280 < *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c)) {
      if (*(int *)((int)DAT_00442128 + 300) != 0) {
        puVar1 = (uint *)(*(int *)((int)DAT_00442128 + 300) + 0x68);
        *puVar1 = *puVar1 | 4;
      }
      *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) & 0xfffffffb;
    }
    *(void **)((int)DAT_00442128 + 300) = this;
    iVar2 = *(ushort *)((int)this + 0xaa) + 7;
    play(DAT_00442128,0,(ushort)iVar2,iVar2,0,'\0');
  }
  *(uint *)((int)this + 0x68) = (param_1 & 1) << 1 | *(uint *)((int)this + 0x68) & 0xfffffffd;
  return;
}

