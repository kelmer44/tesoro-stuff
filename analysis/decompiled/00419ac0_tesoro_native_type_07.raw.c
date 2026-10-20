/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00419ac0
 */


void __thiscall tesoro_native_type_07(void *this,int param_1)

{
  int iVar1;
  ulonglong uVar2;
  
  if (param_1 == 0) {
    *(void **)((int)this + 0x40) = DAT_00442154;
    uVar2 = tesoro_encode_name(0x42d4bc);
    iVar1 = tesoro_find_scene_object(DAT_00442154,(int)uVar2,(int)(uVar2 >> 0x20));
    tesoro_place_jim_in_scene
              (DAT_004420fc,*(void **)((int)this + 0x40),
               *(int *)((int)*(void **)((int)this + 0x40) + 0x70),*(int *)(iVar1 + 0x4c),
               *(int *)(iVar1 + 0x50));
    tesoro_update_scene_view
              (*(void **)((int)this + 0x40),*(int *)((int)DAT_004420fc + 0x4c),
               *(int *)((int)DAT_004420fc + 0x50),0);
  }
  return;
}

