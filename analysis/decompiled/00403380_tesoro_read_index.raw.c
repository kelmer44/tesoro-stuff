/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00403380
 */


void __fastcall tesoro_read_index(int *param_1)

{
  byte bVar1;
  uint uVar2;
  char *_Source;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  byte *pbVar10;
  int *piVar11;
  int iVar12;
  int *piVar13;
  bool bVar14;
  size_t _Count;
  int local_30;
  int local_2c;
  int local_1c [2];
  byte local_14;
  char local_13 [19];
  
  piVar11 = DAT_00442068;
  piVar9 = (int *)param_1[1];
  *param_1 = 0;
  bVar1 = *(byte *)(piVar9 + 3);
  while (((bVar1 & 0x10) == 0 && (uVar2 = FUN_0041b4c0((char *)local_1c,0x1c,1,piVar9), uVar2 != 0))
        ) {
    if ((local_14 & 4) != 0) {
      _Count = 0x13;
      _Source = FUN_0041b940(local_13);
      _strncpy((char *)piVar11,_Source,_Count);
      *(char *)((int)piVar11 + 0x12) = '\0';
      piVar11[5] = local_1c[0] + 4;
      piVar11 = piVar11 + 6;
      *param_1 = *param_1 + 1;
    }
    piVar9 = (int *)param_1[1];
    bVar1 = *(byte *)(piVar9 + 3);
  }
  iVar8 = *param_1;
  if (iVar8 != 1 && -1 < iVar8 + -1) {
    local_30 = 0;
    iVar3 = 0;
    piVar9 = DAT_00442068;
    do {
      iVar4 = iVar3 + 1;
      iVar12 = local_30;
      local_2c = local_30;
      for (iVar7 = iVar4; iVar7 < iVar8; iVar7 = iVar7 + 1) {
        iVar12 = iVar12 + 0x18;
        pbVar10 = (byte *)((int)DAT_00442068 + local_2c);
        pbVar5 = (byte *)((int)DAT_00442068 + iVar12);
        do {
          bVar1 = *pbVar5;
          bVar14 = bVar1 < *pbVar10;
          if (bVar1 != *pbVar10) {
LAB_00403470:
            iVar6 = (1 - (uint)bVar14) - (uint)(bVar14 != 0);
            goto LAB_00403475;
          }
          if (bVar1 == 0) break;
          bVar1 = pbVar5[1];
          bVar14 = bVar1 < pbVar10[1];
          if (bVar1 != pbVar10[1]) goto LAB_00403470;
          pbVar5 = pbVar5 + 2;
          pbVar10 = pbVar10 + 2;
        } while (bVar1 != 0);
        iVar6 = 0;
LAB_00403475:
        if (0 < iVar6) {
          iVar3 = iVar7;
          local_2c = iVar12;
        }
      }
      piVar11 = piVar9;
      piVar13 = local_1c;
      for (iVar8 = 6; iVar8 != 0; iVar8 = iVar8 + -1) {
        *piVar13 = *piVar11;
        piVar11 = piVar11 + 1;
        piVar13 = piVar13 + 1;
      }
      piVar11 = DAT_00442068 + iVar3 * 6;
      piVar13 = piVar9;
      for (iVar8 = 6; iVar8 != 0; iVar8 = iVar8 + -1) {
        *piVar13 = *piVar11;
        piVar11 = piVar11 + 1;
        piVar13 = piVar13 + 1;
      }
      piVar11 = local_1c;
      piVar13 = DAT_00442068 + iVar3 * 6;
      for (iVar8 = 6; iVar8 != 0; iVar8 = iVar8 + -1) {
        *piVar13 = *piVar11;
        piVar11 = piVar11 + 1;
        piVar13 = piVar13 + 1;
      }
      local_30 = local_30 + 0x18;
      iVar8 = *param_1;
      iVar3 = iVar4;
      piVar9 = piVar9 + 6;
    } while (iVar4 < iVar8 + -1);
  }
  return;
}

