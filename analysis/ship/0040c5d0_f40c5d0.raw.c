/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c5d0
 */


undefined4 __fastcall f40c5d0(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  uVar2 = tesoro_encode_name(0x42d350);
  iVar1 = tesoro_find_scene_object(DAT_00442130,(int)uVar2,(int)(uVar2 >> 0x20));
  **(byte **)(param_1 + 0x60) = (byte)(*(uint *)(iVar1 + 0x68) >> 1) & 1;
  uVar2 = tesoro_encode_name(0x42d358);
  uVar3 = tesoro_encode_name(0x42d344);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

