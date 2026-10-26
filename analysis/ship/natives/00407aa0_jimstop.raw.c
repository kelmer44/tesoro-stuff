/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407aa0
 */


void __fastcall jimstop(int *param_1)

{
  if (*(char *)((int)param_1 + 0xd5) != '\0') {
    (**(code **)(*param_1 + 0x74))(param_1[0x43]);
  }
  return;
}

