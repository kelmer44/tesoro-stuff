/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00409790
 */


uint __cdecl tesoro_overlap_poly_poly(ushort *param_1,ushort *param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  ushort *puVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  ushort *puVar9;
  ushort *puVar10;
  short *psVar11;
  ushort *puVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int local_20;
  short *local_18;
  
  iVar4 = *(int *)(param_1 + 2);
  puVar9 = (ushort *)(uint)param_1[1];
  puVar5 = *(ushort **)(param_2 + 2);
  iVar15 = 0;
  bVar7 = false;
  local_20 = 0;
  bVar8 = false;
  bVar6 = false;
  if (puVar9 != (ushort *)0x0) {
    psVar11 = (short *)(iVar4 + 2);
    puVar10 = puVar5;
    local_18 = psVar11;
    do {
      if ((int)(uint)param_2[1] < iVar15) break;
      uVar1 = *puVar10;
      if (psVar11[1] < (short)uVar1) {
        local_20 = local_20 + 1;
        local_18 = psVar11 + 2;
        psVar11 = local_18;
      }
      else {
        uVar2 = psVar11[-1];
        if ((short)uVar1 < (short)uVar2) {
          iVar15 = iVar15 + 1;
          puVar10 = puVar10 + 2;
        }
        else if (uVar1 == uVar2) {
          if (*psVar11 < (short)puVar10[1]) {
            if (bVar6) goto LAB_00409bf4;
            iVar15 = iVar15 + 1;
            bVar8 = true;
            puVar10 = puVar10 + 2;
            bVar7 = true;
          }
          else {
            if (bVar7) goto LAB_00409bf4;
            iVar15 = iVar15 + 1;
            bVar6 = true;
            puVar10 = puVar10 + 2;
          }
        }
        else {
          iVar16 = ((int)(short)puVar10[1] - (int)*psVar11) * ((int)psVar11[1] - (int)(short)uVar2);
          iVar14 = ((int)local_18[2] - (int)*psVar11) * ((int)(short)uVar1 - (int)(short)uVar2);
          if (iVar16 - iVar14 == 0 || iVar16 < iVar14) {
            if (bVar8) goto LAB_00409bf4;
            bVar6 = true;
          }
          else {
            if (bVar6) goto LAB_00409bf4;
            bVar8 = true;
          }
          iVar15 = iVar15 + 1;
          puVar10 = puVar10 + 2;
          psVar11 = local_18;
          bVar7 = bVar8;
        }
      }
    } while (local_20 < (int)puVar9);
  }
  iVar15 = param_2[1] + 1;
  if (-1 < local_20) {
    psVar11 = (short *)(iVar4 + 2 + local_20 * 4);
    puVar10 = puVar5 + iVar15 * 2;
    iVar16 = iVar15;
    local_18 = psVar11;
    do {
      if ((int)(uint)*param_2 <= iVar16) break;
      uVar1 = psVar11[-1];
      uVar2 = *puVar10;
      if ((short)uVar2 < (short)uVar1) {
        local_20 = local_20 + -1;
        local_18 = psVar11 + -2;
        psVar11 = local_18;
      }
      else if (psVar11[1] < (short)uVar2) {
        iVar16 = iVar16 + 1;
        puVar10 = puVar10 + 2;
      }
      else if (uVar2 == uVar1) {
        if (*psVar11 < (short)puVar10[1]) {
          if (bVar6) goto LAB_00409bf4;
          bVar7 = true;
          iVar16 = iVar16 + 1;
          puVar10 = puVar10 + 2;
          bVar8 = true;
        }
        else {
          if (bVar7) goto LAB_00409bf4;
          iVar16 = iVar16 + 1;
          bVar6 = true;
          puVar10 = puVar10 + 2;
        }
      }
      else {
        iVar14 = ((int)(short)puVar10[1] - (int)*psVar11) * ((int)psVar11[1] - (int)(short)uVar1);
        iVar17 = ((int)local_18[2] - (int)*psVar11) * ((int)(short)uVar2 - (int)(short)uVar1);
        if (iVar14 - iVar17 == 0 || iVar14 < iVar17) {
          if (bVar8) goto LAB_00409bf4;
          bVar6 = true;
          bVar7 = bVar8;
        }
        else {
          if (bVar6) goto LAB_00409bf4;
          bVar7 = true;
        }
        iVar16 = iVar16 + 1;
        puVar10 = puVar10 + 2;
        psVar11 = local_18;
        bVar8 = bVar7;
      }
    } while (-1 < local_20);
  }
  iVar16 = 0;
  uVar13 = (uint)*param_1;
  local_20 = uVar13 - 1;
  puVar10 = puVar9;
  if ((int)puVar9 < local_20) {
    puVar12 = (ushort *)(iVar4 + local_20 * 4);
    puVar10 = puVar5;
    param_1 = puVar12;
    do {
      if ((int)(uint)param_2[1] < iVar16) break;
      uVar1 = *puVar10;
      if ((short)puVar12[-2] < (short)uVar1) {
        local_20 = local_20 + -1;
        param_1 = puVar12 + -2;
        puVar12 = param_1;
      }
      else {
        uVar2 = *puVar12;
        if ((short)uVar1 < (short)uVar2) {
          iVar16 = iVar16 + 1;
          puVar10 = puVar10 + 2;
        }
        else if (uVar1 == uVar2) {
          if ((short)puVar10[1] < (short)puVar12[1]) {
            if (bVar7) goto LAB_00409bf4;
            iVar16 = iVar16 + 1;
            bVar6 = true;
            puVar10 = puVar10 + 2;
          }
          else {
            if (bVar6) goto LAB_00409bf4;
            iVar16 = iVar16 + 1;
            bVar8 = true;
            puVar10 = puVar10 + 2;
            bVar7 = true;
          }
        }
        else {
          if (((int)(short)puVar10[1] - (int)(short)puVar12[1]) *
              ((int)(short)puVar12[-2] - (int)(short)uVar2) <
              ((int)(short)param_1[-1] - (int)(short)puVar12[1]) *
              ((int)(short)uVar1 - (int)(short)uVar2)) {
            if (bVar8) goto LAB_00409bf4;
            bVar6 = true;
          }
          else {
            if (bVar6) goto LAB_00409bf4;
            bVar8 = true;
          }
          iVar16 = iVar16 + 1;
          puVar10 = puVar10 + 2;
          puVar12 = param_1;
          bVar7 = bVar8;
        }
      }
    } while ((int)puVar9 < local_20);
  }
  if (local_20 < (int)uVar13) {
    uVar1 = *param_2;
    puVar9 = (ushort *)(iVar4 + local_20 * 4);
    puVar10 = puVar5 + iVar15 * 2;
    param_2 = puVar9;
    while (iVar15 < (int)(uint)uVar1) {
      uVar2 = *puVar9;
      uVar3 = *puVar10;
      if ((short)uVar3 < (short)uVar2) {
        local_20 = local_20 + 1;
        param_2 = puVar9 + 2;
        puVar9 = param_2;
      }
      else if ((short)puVar9[2] < (short)uVar3) {
        iVar15 = iVar15 + 1;
        puVar10 = puVar10 + 2;
      }
      else if (uVar3 == uVar2) {
        if ((short)puVar10[1] < (short)puVar9[1]) {
          if (bVar7) goto LAB_00409bf4;
          iVar15 = iVar15 + 1;
          bVar6 = true;
          puVar10 = puVar10 + 2;
        }
        else {
          if (bVar6) goto LAB_00409bf4;
          bVar7 = true;
          iVar15 = iVar15 + 1;
          puVar10 = puVar10 + 2;
          bVar8 = true;
        }
      }
      else {
        if (((int)(short)puVar9[-2] - (int)(short)uVar2) *
            ((int)(short)puVar10[1] - (int)(short)puVar9[1]) <
            ((int)(short)param_2[-1] - (int)(short)puVar9[1]) *
            ((int)(short)uVar3 - (int)(short)uVar2)) {
          if (bVar8) goto LAB_00409bf4;
          bVar6 = true;
          bVar7 = bVar8;
        }
        else {
          if (bVar6) {
LAB_00409bf4:
            return CONCAT31((int3)((uint)puVar10 >> 8),1);
          }
          bVar7 = true;
        }
        iVar15 = iVar15 + 1;
        puVar10 = puVar10 + 2;
        puVar9 = param_2;
        bVar8 = bVar7;
      }
      if ((int)uVar13 <= local_20) {
        return (uint)puVar10 & 0xffffff00;
      }
    }
  }
  return (uint)puVar10 & 0xffffff00;
}

