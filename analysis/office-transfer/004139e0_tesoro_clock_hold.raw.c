/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004139e0
 */


void tesoro_clock_hold(uint param_1)

{
  DWORD DVar1;
  DWORD DVar2;
  uint uVar3;
  
  DVar1 = GetTickCount();
  DVar2 = GetTickCount();
  uVar3 = DVar2 - DVar1;
  while (uVar3 < param_1) {
    DVar2 = GetTickCount();
    uVar3 = DVar2 - DVar1;
  }
  return;
}

