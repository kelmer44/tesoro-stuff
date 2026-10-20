/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004199a0
 */


void __thiscall tesoro_native_type_05(void *this,int param_1)

{
  int iVar1;
  void *pvVar2;
  
  pvVar2 = DAT_0044214c;
  if (param_1 == 0) {
    *(void **)((int)this + 0x40) = DAT_0044214c;
    iVar1 = *(int *)(*(int *)((int)pvVar2 + 0x90) + 0x78);
    tesoro_place_jim_in_scene
              (DAT_004420fc,pvVar2,*(int *)((int)pvVar2 + 0x70),*(int *)(iVar1 + 0x4c),
               *(int *)(iVar1 + 0x50));
    tesoro_update_scene_view
              (*(void **)((int)this + 0x40),*(int *)((int)DAT_004420fc + 0x4c),
               *(int *)((int)DAT_004420fc + 0x50),0);
    tesoro_set_object_flag_bit2(DAT_004420fc,1);
    *(undefined1 *)((int)DAT_004420fc + 0x110) = 0;
  }
  return;
}

