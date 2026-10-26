/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040d240
 */


undefined4 __fastcall f40d240(int param_1)

{
  int *piVar1;
  ulonglong uVar2;
  
  **(undefined1 **)(param_1 + 0x60) = 1;
  uVar2 = tesoro_encode_name(0x42d4bc);
  piVar1 = (int *)tesoro_find_scene_object(DAT_0044214c,(int)uVar2,(int)(uVar2 >> 0x20));
  (**(code **)(*piVar1 + 0x50))();
  return 1;
}

