/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414c40
 */


undefined4 __fastcall tesoro_generic_door_use(int param_1)

{
  void *this;
  int *piVar1;
  
  if ((*(int *)(param_1 + 0x9c) == 0) && ((char)DAT_004420fc[0x44] == '\0')) {
    this = (void *)(&DAT_00442130)[*(char *)(param_1 + 0xa0)];
    piVar1 = (int *)tesoro_find_scene_object(this,*(int *)(param_1 + 0xa4),*(int *)(param_1 + 0xa8))
    ;
    (**(code **)(*DAT_004420fc + 0x74))(*(undefined4 *)(param_1 + 0x90));
    tesoro_reset_render_lists((int)DAT_00442108);
    FUN_00408550(DAT_004420fc[0x10]);
    FUN_00412e10(DAT_00442108);
    FUN_00412bb0((int)DAT_00442108);
    tesoro_fade_to_black(DAT_00442108,4,100);
    FUN_004139e0(300);
    (**(code **)(*DAT_004420fc + 0x74))(*(undefined4 *)(param_1 + 0xac));
    tesoro_place_jim_in_scene
              (DAT_004420fc,this,*(int *)((int)this + 0x70),piVar1[0x25],piVar1[0x26]);
    (**(code **)(*DAT_004420fc + 0x10))();
    FUN_004023f0(DAT_00442128,(char *)0x0,-1);
    (**(code **)(*piVar1 + 0x70))();
    return 3;
  }
  return 0;
}

