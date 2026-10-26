/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c4d0
 */


undefined4 __fastcall carta_v1(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  undefined4 local_10;
  undefined4 local_c;
  
  uVar2 = tesoro_encode_name(0x42d32c);
  uVar3 = tesoro_encode_name(0x42d2bc);
  uVar4 = tesoro_encode_name(0x42d33c);
  pvVar5 = *(void **)(param_1 + 0x40);
  iVar1 = tesoro_find_scene_object(DAT_00442130,(int)uVar2,(int)(uVar2 >> 0x20));
  local_c = (int)(uVar4 >> 0x20);
  local_10 = (int)uVar4;
  iVar1 = *(int *)(iVar1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,local_10,local_c);
  tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar5);
  return 1;
}

