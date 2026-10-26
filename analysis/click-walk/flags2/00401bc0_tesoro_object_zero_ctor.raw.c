/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401bc0
 */


void __fastcall tesoro_object_zero_ctor(undefined4 *param_1)

{
  *param_1 = &PTR_FUN_00428000;
  param_1[0x1a] = 0;
  *(undefined2 *)((int)param_1 + 0x5e) = 5;
  *(undefined1 *)(param_1 + 0x17) = 0;
  param_1[0x1a] = param_1[0x1a] | 6;
  return;
}

