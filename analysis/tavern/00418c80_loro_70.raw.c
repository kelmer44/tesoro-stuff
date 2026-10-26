/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418c80
 */


void __fastcall loro_70(int param_1)

{
  void *this;
  ulonglong uVar1;
  byte bVar2;
  
  FUN_00410400(param_1);
  uVar1 = tesoro_encode_name(0x42d2e8);
  bVar2 = 1;
  this = (void *)tesoro_find_scene_object(DAT_00442138,(int)uVar1,(int)(uVar1 >> 0x20));
  tesoro_set_object_flag_bit2(this,bVar2);
  return;
}

