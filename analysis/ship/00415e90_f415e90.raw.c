/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415e90
 */


undefined4 __fastcall f415e90(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  undefined4 uStack_4;
  
  uStack_4 = param_1;
  uVar2 = tesoro_encode_name(0x42e4c8);
  iVar1 = tesoro_find_scene_object(DAT_00442140,(int)uVar2,(int)(uVar2 >> 0x20));
  uStack_4 = CONCAT13('\0' < **(char **)(iVar1 + 0x60),(undefined3)uStack_4);
  uVar2 = tesoro_encode_name(0x42d2bc);
  uVar3 = tesoro_encode_name(0x42e4c0);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar1 = (int)&uStack_4 + 3;
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

