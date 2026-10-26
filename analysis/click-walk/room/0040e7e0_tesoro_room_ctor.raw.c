/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040e7e0
 */


undefined4 * __thiscall
tesoro_room_ctor(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4)

{
  int iVar1;
  
  tesoro_initialize_scenery(this,param_1,param_2,param_3,param_4);
  *(undefined ***)this = &PTR_FUN_0042ba30;
  *(undefined4 *)((int)this + 0x7c) = *(undefined4 *)(param_4 + 2);
  *(undefined4 *)((int)this + 0x80) = *(undefined4 *)(param_4 + 6);
  *(undefined4 *)((int)this + 0x84) = *(undefined4 *)(param_4 + 10);
  *(byte *)((int)this + 0x8c) = param_4[0xe];
  *(byte *)((int)this + 0x8d) = param_4[0xf];
  *(uint *)((int)this + 0x78) = (uint)*(ushort *)(param_4 + 0x10);
  *(uint *)((int)this + 0x6c) = (uint)*(ushort *)(param_4 + 0x12);
  *(uint *)((int)this + 0x70) = (uint)*(ushort *)(param_4 + 0x14);
  iVar1 = FUN_00413a10(DAT_00442120);
  *(int *)((int)this + 0x74) = iVar1;
  iVar1 = FUN_00413b00(*(int *)((int)this + 0x6c),*(int *)((int)this + 0x70));
  *(int *)((int)this + 0x78) = iVar1;
  *(undefined1 *)((int)this + 0x8e) = 0;
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 4;
  return (undefined4 *)this;
}

