/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004180b0
 */


undefined4 __thiscall f4180b0(void *this,int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  ulonglong uVar3;
  
  if (param_1 != 0x23) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  uVar3 = tesoro_encode_name(0x42e784);
  piVar2 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  (**(code **)(*piVar2 + 0x20))(0);
  uVar3 = tesoro_encode_name(0x42e6b0);
  piVar2 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  (**(code **)(*piVar2 + 0x20))(1);
  return 1;
}

