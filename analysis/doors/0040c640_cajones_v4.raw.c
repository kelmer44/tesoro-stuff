/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c640
 */


undefined4 __fastcall cajones_v4(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  uVar2 = tesoro_encode_name(0x42d32c);
  iVar1 = tesoro_find_scene_object(DAT_00442130,(int)uVar2,(int)(uVar2 >> 0x20));
  if (*(char *)(iVar1 + 0x32) == '\x02') {
    uVar3 = tesoro_encode_name(0x42d2d0);
    uVar2 = tesoro_encode_name(0x42d360);
    pvVar4 = *(void **)(param_1 + 0x40);
    iVar1 = *(int *)(param_1 + 0x60);
  }
  else {
    uVar3 = tesoro_encode_name(0x42d324);
    uVar2 = tesoro_encode_name(0x42d318);
    pvVar4 = *(void **)(param_1 + 0x40);
    iVar1 = 0;
  }
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar2,(int)(uVar2 >> 0x20));
  tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar4);
  return 1;
}

