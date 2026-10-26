/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040d3d0
 */


undefined4 __fastcall f40d3d0(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  uVar2 = tesoro_encode_name(0x42d4fc);
  iVar1 = tesoro_find_scene_object(DAT_00442154,(int)uVar2,(int)(uVar2 >> 0x20));
  *(bool *)(*(int *)(param_1 + 0x60) + 1) = *(char *)(*(int *)(iVar1 + 0x60) + 1) == '\x05';
  uVar2 = tesoro_encode_name(0x42d39c);
  uVar3 = tesoro_encode_name(0x42d4f4);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

