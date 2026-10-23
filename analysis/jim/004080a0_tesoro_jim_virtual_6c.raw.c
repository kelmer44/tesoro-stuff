/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004080a0
 */


undefined4 tesoro_jim_virtual_6c(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  ulonglong uVar3;
  
  if (param_1 != 0x25) {
    return 0;
  }
  uVar3 = tesoro_encode_name(0x42d2a8);
  piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  uVar2 = (**(code **)(*piVar1 + 0x6c))();
  return uVar2;
}

