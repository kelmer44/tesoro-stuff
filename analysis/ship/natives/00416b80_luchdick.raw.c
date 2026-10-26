/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416b80
 */


void __thiscall luchdick(void *this,int param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  int iVar3;
  
  if (param_1 == 0) {
    fade(DAT_00442108,4,100);
    pvVar1 = DAT_004421e0;
    *(void **)((int)this + 0x40) = DAT_004421e0;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0x8a,-0x67);
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
  else if (param_1 == 1) {
    fade(DAT_00442108,4,100);
    pvVar1 = DAT_00442184;
    *(void **)((int)this + 0x40) = DAT_00442184;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),1000,-0x5f);
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
      return;
    }
  }
  return;
}

