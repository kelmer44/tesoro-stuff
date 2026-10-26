/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040d1a0
 */


undefined4 __fastcall f40d1a0(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  int iVar5;
  int iVar6;
  void *pvVar7;
  undefined4 local_8;
  undefined4 local_4;
  
  iVar1 = tesoro_generic_door_use(param_1);
  if (iVar1 == 0) {
    return 0;
  }
  if (**(char **)(param_1 + 0x60) == '\0') {
    **(char **)(param_1 + 0x60) = '\x01';
    uVar2 = tesoro_encode_name(0x42d2e8);
    uVar3 = tesoro_encode_name(0x42d4b4);
    uVar4 = tesoro_encode_name(0x42d4a8);
    pvVar7 = DAT_00442148;
    iVar1 = tesoro_find_scene_object(DAT_00442148,(int)uVar2,(int)(uVar2 >> 0x20));
    iVar1 = *(int *)(iVar1 + 0x60);
    local_4 = (int)(uVar4 >> 0x20);
    local_8 = (int)uVar4;
    iVar5 = (int)uVar3;
    iVar6 = (int)(uVar3 >> 0x20);
    this = (void *)FUN_004060c0(DAT_004420b8,local_8,local_4);
    tesoro_start_script(this,iVar5,iVar6,iVar1,pvVar7);
  }
  return 1;
}

