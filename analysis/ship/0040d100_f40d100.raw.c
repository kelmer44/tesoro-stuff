/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040d100
 */


undefined4 __fastcall f40d100(int param_1)

{
  int iVar1;
  int *piVar2;
  void *this;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  
  uVar3 = tesoro_encode_name(0x42d49c);
  iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  **(byte **)(param_1 + 0x60) = (byte)(*(uint *)(iVar1 + 0x68) >> 1) & 1;
  if (**(char **)(param_1 + 0x60) == '\0') {
    uVar3 = tesoro_encode_name(0x42d494);
    piVar2 = (int *)tesoro_find_scene_object(DAT_0044217c,(int)uVar3,(int)(uVar3 >> 0x20));
    (**(code **)(*piVar2 + 0x20))();
  }
  uVar3 = tesoro_encode_name(0x42d324);
  uVar4 = tesoro_encode_name(0x42d48c);
  pvVar5 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar5);
  return 1;
}

