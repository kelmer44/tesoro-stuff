/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004198d0
 */


void __thiscall tesoro_native_type_03(void *this,int param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  int iVar3;
  
  pvVar1 = DAT_0044213c;
  if (param_1 != 0) {
    if (param_1 == 1) {
      *(void **)((int)this + 0x40) = DAT_0044213c;
      tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0x1e,0x7d);
      tesoro_update_scene_view
                (*(void **)((int)this + 0x40),*(int *)((int)DAT_004420fc + 0x4c),
                 *(int *)((int)DAT_004420fc + 0x50),0);
      tesoro_set_object_flag_bit2(DAT_004420fc,0);
      iVar3 = 0;
      if (0 < *(int *)((int)this + 0xb8)) {
        do {
          uVar2 = tesoro_find_scene_object
                            (*(void **)((int)this + 0x40),
                             *(int *)(*(int *)((int)this + 0xb4) + iVar3 * 8),
                             *(int *)(*(int *)((int)this + 0xb4) + 4 + iVar3 * 8));
          iVar3 = iVar3 + 1;
          *(undefined4 *)(*(int *)((int)this + 0xb0) + -4 + iVar3 * 4) = uVar2;
        } while (iVar3 < *(int *)((int)this + 0xb8));
      }
    }
    else if (param_1 == 2) {
      *(undefined4 *)((int)DAT_004420fc + 0x4c) = 0x1b3;
      *(undefined4 *)((int)DAT_004420fc + 0x50) = 0x2d;
      tesoro_set_object_flag_bit2(DAT_004420fc,1);
      return;
    }
  }
  return;
}

