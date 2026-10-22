/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407730
 */


void __fastcall tesoro_jim_virtual_0c(int *param_1)

{
  int iVar1;
  
  tesoro_recruiter_virtual_0c(param_1);
  iVar1 = param_1[0x3f];
  param_1[4] = param_1[4] * iVar1 >> 8;
  param_1[6] = param_1[6] * iVar1 >> 8;
  param_1[5] = param_1[5] * iVar1 >> 8;
  param_1[7] = param_1[7] * iVar1 >> 8;
  return;
}

