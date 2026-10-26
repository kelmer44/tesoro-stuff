/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415c40
 */


ulonglong __cdecl tesoro_name_encode_wrapper(int param_1)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  uVar2 = 0;
  uVar4 = 0;
  iVar5 = 0;
  do {
    bVar1 = *(byte *)(iVar5 + param_1);
    uVar3 = (uint)bVar1;
    if (bVar1 == 0) {
      return (ulonglong)((uVar2 << 5) << ((char)iVar5 * -5 + 0x19U & 0x1f));
    }
    if ((char)bVar1 < '5') {
      uVar3 = uVar3 + 0xb;
    }
    uVar2 = uVar2 << 5 | uVar3 & 0x1f;
    iVar5 = iVar5 + 1;
  } while (iVar5 < 6);
  if (iVar5 < 0xc) {
    while( true ) {
      bVar1 = *(byte *)(iVar5 + param_1);
      uVar3 = (uint)bVar1;
      if (bVar1 == 0) break;
      if ((char)bVar1 < '5') {
        uVar3 = uVar3 + 0xb;
      }
      uVar4 = uVar4 << 5 | uVar3 & 0x1f;
      iVar5 = iVar5 + 1;
      if (0xb < iVar5) {
        return CONCAT44(uVar4,uVar2);
      }
    }
    uVar4 = (uVar4 << 5) << ((char)iVar5 * -5 + 0x37U & 0x1f);
  }
  return CONCAT44(uVar4,uVar2);
}

