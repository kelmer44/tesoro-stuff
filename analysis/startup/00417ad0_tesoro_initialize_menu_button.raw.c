/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00417ad0
 */


undefined4 * __thiscall
tesoro_initialize_menu_button(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4)

{
  FUN_0040ecb0(this,param_1,param_2,param_3,(int)param_4,0);
  *(undefined ***)this = &PTR_FUN_0042c000;
  *(uint *)((int)this + 0xa0) = (uint)param_4[2];
  FUN_00401de0(this,0,(ushort)*param_4,(uint)*param_4,0,'\0');
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) & 0xfffffffb | 0x508;
  return (undefined4 *)this;
}

