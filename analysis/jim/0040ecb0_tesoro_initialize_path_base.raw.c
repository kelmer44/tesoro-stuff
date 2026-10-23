/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040ecb0
 */


undefined4 * __thiscall
tesoro_initialize_path_base
          (void *this,undefined4 *param_1,int param_2,int param_3,int param_4,uint param_5)

{
  FUN_00401aa0(this,param_1,param_2,param_3,param_4,param_5);
  *(undefined ***)this = &PTR_FUN_0042ba68;
  *(uint *)((int)this + 0x68) =
       (*(int *)(*(int *)(*(int *)((int)this + 0x38) + 0x1c) + 0x18 +
                (uint)*(ushort *)(*(int *)((int)this + 0x38) + 0x30) * 0x24) != 0 | 0x142) << 2 |
       *(uint *)((int)this + 0x68) & 0xfffffffb;
  return (undefined4 *)this;
}

