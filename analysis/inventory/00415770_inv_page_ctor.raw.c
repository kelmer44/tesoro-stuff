/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415770
 */


undefined4 * __thiscall
inv_page_ctor(void *this,undefined4 *param_1,int param_2,int param_3,int param_4)

{
  FUN_0040ecb0(this,param_1,param_2,param_3,param_4,0);
  *(undefined ***)this = &PTR_LAB_0042beb0;
  *(undefined2 *)((int)this + 0x5e) = 0x32;
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 8;
  return (undefined4 *)this;
}

