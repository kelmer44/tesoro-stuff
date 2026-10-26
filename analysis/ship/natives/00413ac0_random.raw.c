/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413ac0
 */


int random(int param_1,int param_2)

{
  DWORD DVar1;
  
  DVar1 = GetTickCount();
  return ((int)((DVar1 & 0x78 | (int)DVar1 >> 7 & 7U | (DVar1 & 7) << 7) * ((param_2 - param_1) + 1)
               ) >> 10) + param_1;
}

