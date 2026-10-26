/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415d90
 */


undefined4 __thiscall f415d90(void *this,int param_1)

{
  undefined4 uVar1;
  int iVar2;
  ulonglong uVar3;
  
  if (param_1 != 0x1e) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  tesoro_set_object_flag_bit2(DAT_004420fc,1);
  *(void **)((int)this + 0x40) = DAT_0044215c;
  uVar3 = tesoro_encode_name(0x42e3d0);
  iVar2 = tesoro_find_scene_object(DAT_0044215c,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_place_jim_in_scene
            (DAT_004420fc,*(void **)((int)this + 0x40),
             *(int *)((int)*(void **)((int)this + 0x40) + 0x70),*(int *)(iVar2 + 0x4c),
             *(int *)(iVar2 + 0x50));
  (**(code **)(*DAT_004420fc + 0x74))();
  *(undefined4 *)((int)DAT_00442128 + 300) = 0;
  FUN_00401de0(DAT_00442128,0,0,0,0,'\0');
  return 3;
}

