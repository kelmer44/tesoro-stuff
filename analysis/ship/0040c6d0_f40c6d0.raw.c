/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c6d0
 */


undefined4 __fastcall f40c6d0(int param_1)

{
  void *this;
  ulonglong uVar1;
  ulonglong uVar2;
  int iVar3;
  void *pvVar4;
  
  uVar1 = tesoro_encode_name(0x42d2bc);
  uVar2 = tesoro_encode_name(0x42d368);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar3 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar2,(int)(uVar2 >> 0x20));
  tesoro_start_script(this,(int)uVar1,(int)(uVar1 >> 0x20),iVar3,pvVar4);
  return 1;
}

