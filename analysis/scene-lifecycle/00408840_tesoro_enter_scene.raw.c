/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408840
 */


void __fastcall tesoro_enter_scene(int *param_1)

{
  byte bVar1;
  
  bVar1 = *(byte *)(*(int *)(*(int *)(DAT_004420fc + 0x40) + 0x90) + 0x7d);
  if (bVar1 != 0) {
    FUN_00403cb0(*(void **)(DAT_00442080 + 0x1c),bVar1,1);
  }
  (**(code **)(*param_1 + 0x10))();
  return;
}

