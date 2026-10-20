/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040e5c0
 */


undefined4 * __thiscall
tesoro_initialize_scenery(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4)

{
  void *this_00;
  uint uVar1;
  
  FUN_00401aa0(this,param_1,param_2,param_3,(int)param_4,0);
  this_00 = *(void **)((int)this + 0x38);
  *(undefined ***)this = &PTR_LAB_0042b9c0;
  uVar1 = *(uint *)((int)this + 0x68) & 0xfffffffb;
  *(uint *)((int)this + 0x68) = uVar1 | (uint)(*(short *)((int)this_00 + 0x20) != 0) << 2;
  switch(param_4[1]) {
  case 0:
    *(uint *)((int)this + 0x68) = uVar1;
  case 1:
    FUN_00401740(this_00,(ushort)*param_4,(uint)*param_4,0,'\0');
    *(undefined2 *)(*(int *)(*(int *)((int)this + 0x38) + 0x1c) + 6 + (uint)*param_4 * 0x24) = 0;
    *(undefined2 *)(*(int *)(*(int *)((int)this + 0x38) + 0x1c) + 4 + (uint)*param_4 * 0x24) = 0;
    return (undefined4 *)this;
  case 2:
    FUN_00401740(this_00,(ushort)*param_4,-1,0,'\0');
    return (undefined4 *)this;
  case 3:
    FUN_00401740(this_00,(ushort)*param_4,-1,1,'\0');
  default:
    return (undefined4 *)this;
  }
}

