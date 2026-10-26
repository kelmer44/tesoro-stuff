/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414e00
 */


undefined4 * __thiscall
tesoro_floor_414e00(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4)

{
  FUN_0040ecb0(this,param_1,param_2,param_3,(int)param_4,0);
  *(undefined ***)this = &PTR_FUN_0042bd60;
  FUN_00401740(*(void **)((int)this + 0x38),(ushort)*param_4,(uint)*param_4,0,'\0');
  *(undefined2 *)(*(int *)(*(int *)((int)this + 0x38) + 0x1c) + 4 + (uint)*param_4 * 0x24) = 0;
  *(undefined2 *)(*(int *)(*(int *)((int)this + 0x38) + 0x1c) + 6 + (uint)*param_4 * 0x24) = 0;
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) & 0xfffffbff;
  return (undefined4 *)this;
}

