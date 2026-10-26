/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004150d0
 */


undefined4 * __thiscall
tesoro_floor_4150d0(void *this,undefined4 *param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined1 uVar1;
  
  FUN_00401aa0(this,param_1,param_2,param_3,(int)param_4,0);
  *(undefined ***)this = &PTR_FUN_0042bdd0;
  *(undefined4 *)((int)this + 0x6c) = *param_4;
  uVar1 = *(undefined1 *)(param_4 + 1);
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 8;
  *(undefined1 *)((int)this + 0x70) = uVar1;
  FUN_004104a0(*(void **)((int)this + 0x3c),(int)this);
  return (undefined4 *)this;
}

