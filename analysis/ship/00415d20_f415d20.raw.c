/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415d20
 */


undefined4 __fastcall f415d20(uint param_1)

{
  void *this;
  undefined4 uVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  void *pvVar5;
  undefined4 uStack_4;
  
  if (*(char *)(DAT_004420fc + 0x110) != '\0') {
    uStack_4 = param_1 & 0xffffff;
    uVar2 = tesoro_encode_name(0x42e4b4);
    uVar3 = tesoro_encode_name(0x42e4a4);
    pvVar5 = *(void **)(param_1 + 0x40);
    iVar4 = (int)&uStack_4 + 3;
    this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar5);
    return 3;
  }
  uStack_4 = param_1;
  uVar1 = tesoro_generic_door_use(param_1);
  return uVar1;
}

