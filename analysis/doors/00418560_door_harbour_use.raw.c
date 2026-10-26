/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418560
 */


undefined4 __fastcall door_harbour_use(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  iVar1 = generic_door_use(param_1);
  if (iVar1 == 0) {
    return 0;
  }
  if (**(char **)(param_1 + 0x60) == '\0') {
    **(char **)(param_1 + 0x60) = '\x01';
    uVar2 = tesoro_encode_name(0x42d4b4);
    uVar3 = tesoro_encode_name(0x42e818);
    iVar1 = 0;
    pvVar4 = DAT_00442138;
    this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  }
  return 3;
}

