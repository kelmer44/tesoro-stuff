/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004169a0
 */


void __thiscall tesoro_native_type_04(void *this,int param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 == 0) {
    tesoro_fade_to_black(DAT_00442108,4,100);
    pvVar1 = DAT_00442144;
    iVar3 = 0x87;
    *(void **)((int)this + 0x40) = DAT_00442144;
    iVar2 = 0x87;
  }
  else {
    if (param_1 < 1) {
      return;
    }
    if (2 < param_1) {
      return;
    }
    tesoro_fade_to_black(DAT_00442108,4,100);
    pvVar1 = DAT_00442140;
    iVar3 = 0x82;
    *(void **)((int)this + 0x40) = DAT_00442140;
    iVar2 = -0xfa;
  }
  tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),iVar2,iVar3);
  *(undefined1 *)((int)DAT_004420fc + 0x110) = 1;
  return;
}

