/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414be0
 */


undefined4 * __thiscall
tesoro_initialize_door
          (void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4,uint param_5)

{
  ulonglong uVar1;
  
  tesoro_initialize_interactive_object(this,param_1,param_2,param_3,param_4,param_5);
  *(undefined ***)this = &PTR_FUN_0042bce8;
  *(byte *)((int)this + 0xa0) = param_4[0x67];
  uVar1 = tesoro_encode_name((int)(param_4 + 0x68));
  *(ulonglong *)((int)this + 0xa4) = uVar1;
  *(uint *)((int)this + 0xac) = param_4[0x74] + 1;
  return (undefined4 *)this;
}

