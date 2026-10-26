/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c810
 */


undefined4 __fastcall puerta2_v0(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  undefined *puVar5;
  longlong lVar4;
  
  uVar2 = tesoro_encode_name(0x42d32c);
  iVar1 = tesoro_find_scene_object(DAT_00442130,(int)uVar2,(int)(uVar2 >> 0x20));
  if (*(char *)(iVar1 + 0x32) == '\x02') {
    puVar5 = &DAT_0042d38c;
  }
  else {
    puVar5 = &DAT_0042d390;
  }
  uVar2 = tesoro_encode_name((int)puVar5);
  uVar3 = tesoro_encode_name(0x42d380);
  lVar4 = (ulonglong)*(uint *)(param_1 + 0x40) << 0x20;
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),(int)lVar4,
                      (void *)((ulonglong)lVar4 >> 0x20));
  return 1;
}

