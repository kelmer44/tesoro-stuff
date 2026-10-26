/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418110
 */


void __thiscall f418110(void *this,int param_1)

{
  int *piVar1;
  ulonglong uVar2;
  
  if (param_1 == 0x20) {
    uVar2 = tesoro_encode_name(0x42e6a4);
    piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(0);
    uVar2 = tesoro_encode_name(0x42d2a8);
    piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(1);
    FUN_0040f480(this,0x20);
    return;
  }
  if (param_1 != 0x21) {
    FUN_0040f480(this,param_1);
    return;
  }
  uVar2 = tesoro_encode_name(0x42e6e0);
  piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
  (**(code **)(*piVar1 + 0x20))(0);
  uVar2 = tesoro_encode_name(0x42d2a8);
  piVar1 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
  (**(code **)(*piVar1 + 0x20))(1);
  FUN_0040f480(this,0x21);
  return;
}

