/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413b00
 */


int tesoro_random_range(int param_1,int param_2)

{
  DWORD DVar1;
  
  DVar1 = GetTickCount();
  return ((int)(((int)((int)((int)((int)(DVar1 & 0x200 | (int)DVar1 >> 2 & 0x100U) >> 2 |
                                  DVar1 & 0x100) >> 2 | DVar1 & 0x80) >> 2 | DVar1 & 0x20) >> 1 |
                ((((DVar1 & 2 | (DVar1 & 1) << 2) << 2 | DVar1 & 4) << 2 | DVar1 & 8) << 2 |
                DVar1 & 0x10) << 1) * ((param_2 - param_1) + 1)) >> 10) + param_1;
}

