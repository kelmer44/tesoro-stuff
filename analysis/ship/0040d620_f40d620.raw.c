/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040d620
 */


undefined4 __fastcall f40d620(int *param_1)

{
  int *piVar1;
  ulonglong uVar2;
  
  (**(code **)(*param_1 + 0x20))(0);
  uVar2 = tesoro_encode_name(0x42d53c);
  piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
  (**(code **)(*piVar1 + 0x20))();
  return 1;
}

