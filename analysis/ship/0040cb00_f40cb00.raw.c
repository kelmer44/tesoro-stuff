/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040cb00
 */


undefined4 __fastcall f40cb00(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  iVar1 = FUN_00413ac0(0,4);
  **(undefined1 **)(param_1 + 0x60) = (char)iVar1;
  uVar2 = tesoro_encode_name(0x42d2bc);
  uVar3 = tesoro_encode_name(0x42d3c8);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

