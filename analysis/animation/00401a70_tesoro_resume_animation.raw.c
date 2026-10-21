/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401a70
 */


void __fastcall tesoro_resume_animation(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = FUN_00413a10(DAT_00442120);
  iVar1 = *(int *)(param_1 + 0x10);
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + (iVar2 - iVar1);
  *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffffd | 1;
  return;
}

