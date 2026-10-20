/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00403500
 */


undefined4 __thiscall tesoro_find_resource(void *this,byte *param_1,char param_2)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  byte *pbVar6;
  int iVar7;
  bool bVar8;
  
                    /* WARNING: Load size is inaccurate */
  iVar7 = -1;
  iVar4 = 0;
  iVar5 = *this + -1;
  if (-1 < iVar5) {
    do {
      iVar7 = iVar5 + iVar4 >> 1;
      pbVar6 = (byte *)(DAT_00442068 + iVar7 * 0x18);
      pbVar2 = param_1;
      do {
        bVar1 = *pbVar2;
        bVar8 = bVar1 < *pbVar6;
        if (bVar1 != *pbVar6) {
LAB_0040354d:
          iVar3 = (1 - (uint)bVar8) - (uint)(bVar8 != 0);
          goto LAB_00403552;
        }
        if (bVar1 == 0) break;
        bVar1 = pbVar2[1];
        bVar8 = bVar1 < pbVar6[1];
        if (bVar1 != pbVar6[1]) goto LAB_0040354d;
        pbVar2 = pbVar2 + 2;
        pbVar6 = pbVar6 + 2;
      } while (bVar1 != 0);
      iVar3 = 0;
LAB_00403552:
      if (iVar3 < 1) {
        if (-1 < iVar3) goto LAB_00403568;
        iVar4 = iVar7 + 1;
      }
      else {
        iVar5 = iVar7 + -1;
      }
    } while (iVar4 <= iVar5);
    iVar7 = -1;
  }
LAB_00403568:
  if (-1 < iVar7) {
    return *(undefined4 *)(DAT_00442068 + 0x14 + iVar7 * 0x18);
  }
  if (param_2 != '\0') {
    MessageBoxA((HWND)0x0,&DAT_0042dca0,s_ERROR_0042d06c,0x30);
  }
  return 0;
}

