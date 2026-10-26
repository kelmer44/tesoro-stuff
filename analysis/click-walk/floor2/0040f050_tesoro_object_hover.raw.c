/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f050
 */


undefined4 __fastcall tesoro_object_hover(int param_1)

{
  char local_10 [16];
  
  if (*(int *)(param_1 + 0x6c) != 0) {
    FUN_004023f0(DAT_00442128,*(char **)(*(int *)(param_1 + 0x6c) + 0xc),0xffff);
    return 1;
  }
  FUN_00415cb0(*(uint *)(param_1 + 0x44),*(uint *)(param_1 + 0x48),(int)local_10);
  FUN_004023f0(DAT_00442128,local_10,0xffff);
  return 1;
}

