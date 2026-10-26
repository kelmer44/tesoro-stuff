/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040d330
 */


undefined4 f40d330(void)

{
  int *piVar1;
  ulonglong uVar2;
  
  uVar2 = tesoro_encode_name(0x42d4e8);
  piVar1 = (int *)tesoro_find_scene_object(DAT_0044214c,(int)uVar2,(int)(uVar2 >> 0x20));
  (**(code **)(*piVar1 + 0x20))(0);
  uVar2 = tesoro_encode_name(0x42d4e0);
  piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
  (**(code **)(*piVar1 + 0x20))(1);
  return 1;
}

