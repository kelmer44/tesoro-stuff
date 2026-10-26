/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414420
 */


bool tesoro_mouse_414420(void)

{
  int iVar1;
  
  (**(code **)*DAT_00442074)();
  (**(code **)*DAT_0044206c)();
  tesoro_begin_frame_time(DAT_00442120);
  FUN_00404210(DAT_00442080);
  FUN_0040dd30(DAT_00442100,3);
  tesoro_reset_render_lists(DAT_00442108);
  iVar1 = tesoro_update_objects(DAT_00442104);
  if (iVar1 != 3) {
    tesoro_render_objects(DAT_00442104);
  }
  tesoro_end_frame_time((int)DAT_00442120);
  iVar1 = tesoro_object_list_empty(DAT_00442104);
  return (bool)('\x01' - ((char)iVar1 != '\0'));
}

