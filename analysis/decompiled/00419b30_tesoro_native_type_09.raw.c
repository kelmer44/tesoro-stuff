/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00419b30
 */


void __thiscall tesoro_native_type_09(void *this,int param_1)

{
  void *pvVar1;
  
  if (param_1 == 1) {
    *(undefined4 *)((int)DAT_00442128 + 300) = 0;
    FUN_00401de0(DAT_00442128,0,0,0,0,'\0');
    pvVar1 = DAT_00442164;
    *(void **)((int)this + 0x40) = DAT_00442164;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0,0);
    tesoro_set_object_flag_bit2(DAT_004420fc,0);
  }
  return;
}

