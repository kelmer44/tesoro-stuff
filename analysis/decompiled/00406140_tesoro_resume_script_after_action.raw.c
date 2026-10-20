/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00406140
 */


void __fastcall tesoro_resume_script_after_action(int *param_1)

{
  FUN_0040fb20(DAT_00442104,param_1[0x2b]);
  *(undefined1 *)((int)DAT_004420fc + 0xd5) = 0;
  (**(code **)(*DAT_004420fc + 0x74))(DAT_004420fc[0x43]);
  param_1[0x33] = param_1[0x33] + 1;
  tesoro_execute_script(param_1);
  return;
}

