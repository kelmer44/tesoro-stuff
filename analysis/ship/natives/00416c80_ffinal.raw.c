/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416c80
 */


void __thiscall ffinal(void *this,undefined4 param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  int iVar3;
  
  pvVar1 = DAT_004421e0;
  switch(param_1) {
  case 0:
    *(void **)((int)this + 0x40) = DAT_004421e0;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),-0xad,-0x3f);
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
    break;
  case 1:
    fade(DAT_00442108,4,100);
    pvVar1 = DAT_004421e4;
    *(void **)((int)this + 0x40) = DAT_004421e4;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0,0);
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
    break;
  case 2:
    fade(DAT_00442108,4,100);
    pvVar1 = DAT_004421e8;
    *(void **)((int)this + 0x40) = DAT_004421e8;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0,0);
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
    break;
  case 3:
    fade(DAT_00442108,10,100);
    return;
  case 4:
    fade(DAT_00442108,4,100);
    FUN_0040fe20(DAT_00442104);
  }
  return;
}

