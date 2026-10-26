/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c770
 */


undefined4 __fastcall f40c770(int *param_1)

{
  int iVar1;
  void *this;
  int *piVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  
  uVar3 = tesoro_encode_name(0x42d32c);
  iVar1 = tesoro_find_scene_object(DAT_00442130,(int)uVar3,(int)(uVar3 >> 0x20));
  if (*(char *)(iVar1 + 0x32) != '\x02') {
    uVar3 = tesoro_encode_name(0x42d324);
    uVar4 = tesoro_encode_name(0x42d318);
    pvVar5 = (void *)param_1[0x10];
    iVar1 = 0;
    this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
    tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar5);
    return 1;
  }
  uVar3 = tesoro_encode_name(0x42d378);
  piVar2 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  (**(code **)(*piVar2 + 0x20))();
  (**(code **)(*param_1 + 0x20))(0);
  return 1;
}

