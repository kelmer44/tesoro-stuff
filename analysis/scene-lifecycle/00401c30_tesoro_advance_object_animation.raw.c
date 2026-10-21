/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401c30
 */


void __fastcall tesoro_advance_object_animation(int param_1)

{
  if ((*(uint *)(param_1 + 0x68) >> 4 & 1) != 0) {
    FUN_00401860(*(int *)(param_1 + 0x38));
    return;
  }
  return;
}

