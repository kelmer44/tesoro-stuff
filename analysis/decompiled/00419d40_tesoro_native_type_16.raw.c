/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00419d40
 */


void __thiscall tesoro_native_type_16(void *this,int param_1)

{
  void *pvVar1;
  
  if (param_1 == 0) {
    tesoro_fade_to_black(DAT_00442108,4,100);
    FUN_004139e0(300);
    pvVar1 = DAT_00442178;
    *(void **)((int)this + 0x40) = DAT_00442178;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0x82,0x82);
  }
  return;
}

