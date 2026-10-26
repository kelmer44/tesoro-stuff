/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00409560
 */


uint __cdecl tesoro_overlap_point_in_poly(ushort *param_1,int param_2,int param_3)

{
  short sVar1;
  short sVar2;
  short *psVar3;
  short sVar4;
  byte bVar5;
  uint uVar6;
  int iVar7;
  short *psVar8;
  int iVar9;
  short *psVar10;
  bool bVar11;
  int local_8;
  int local_4;
  
  uVar6 = (uint)*param_1;
  psVar3 = *(short **)(param_1 + 2);
  local_8 = uVar6 - 1;
  bVar5 = 0;
  if (local_8 == 0) {
    return 0;
  }
  local_4 = uVar6 - local_8;
  psVar10 = psVar3;
  do {
    sVar1 = psVar10[1];
    sVar2 = psVar10[3];
    if (sVar1 < sVar2) {
      iVar9 = (int)sVar1;
      if ((iVar9 <= param_3) && (iVar7 = (int)sVar2, param_3 <= iVar7)) {
        if (param_3 == iVar9) {
          iVar9 = (int)*psVar10;
          if (iVar9 < param_2) {
            if (local_8 == uVar6 - 1) {
              psVar8 = psVar3 + uVar6 * 2 + -4;
            }
            else {
              psVar8 = psVar10 + -2;
            }
            sVar2 = psVar8[1];
            sVar4 = sVar2 - sVar1;
            if (sVar2 == sVar1) {
              iVar9 = local_4 + -3;
              do {
                iVar7 = iVar9;
                if (iVar9 < 0) {
                  iVar7 = uVar6 - 2;
                }
                iVar9 = iVar7 + -1;
                sVar2 = psVar3[iVar7 * 2 + 1];
                sVar4 = sVar2 - sVar1;
              } while (sVar2 == sVar1);
            }
            if (SBORROW2(sVar2,sVar1) != sVar4 < 0) {
              bVar5 = bVar5 + 1;
            }
          }
          else {
joined_r0x004096fa:
            if (iVar9 == param_2) {
LAB_00409674:
              return CONCAT31((int3)((uint)iVar9 >> 8),1);
            }
          }
        }
        else if (param_3 != iVar7) {
          iVar9 = ((param_3 - iVar9) * ((int)psVar10[2] - (int)*psVar10)) / (iVar7 - iVar9) +
                  (int)*psVar10;
          if (iVar9 == param_2) goto LAB_00409674;
          if (iVar9 < param_2) {
            bVar5 = bVar5 + 1;
          }
        }
      }
    }
    else if (sVar2 < sVar1) {
      iVar9 = (int)sVar2;
      if (iVar9 <= param_3) {
        iVar7 = (int)sVar1;
        if (param_3 <= iVar7) {
          if (param_3 == iVar7) {
            iVar9 = (int)*psVar10;
            if (param_2 <= iVar9) goto joined_r0x004096fa;
            if (local_8 == uVar6 - 1) {
              psVar8 = psVar3 + uVar6 * 2 + -4;
            }
            else {
              psVar8 = psVar10 + -2;
            }
            sVar2 = psVar8[1];
            sVar4 = sVar2 - sVar1;
            bVar11 = sVar2 == sVar1;
            if (bVar11) {
              iVar9 = local_4 + -3;
              do {
                iVar7 = iVar9;
                if (iVar9 < 0) {
                  iVar7 = uVar6 - 2;
                }
                iVar9 = iVar7 + -1;
                sVar2 = psVar3[iVar7 * 2 + 1];
                sVar4 = sVar2 - sVar1;
                bVar11 = sVar2 == sVar1;
              } while (bVar11);
            }
            if (!bVar11 && SBORROW2(sVar2,sVar1) == sVar4 < 0) {
              bVar5 = bVar5 + 1;
            }
          }
          else if (param_3 != iVar9) {
            iVar9 = ((param_3 - iVar7) * ((int)psVar10[2] - (int)*psVar10)) / (iVar9 - iVar7) +
                    (int)*psVar10;
            if (iVar9 == param_2) goto LAB_00409674;
            if (iVar9 < param_2) {
              bVar5 = bVar5 + 1;
            }
          }
        }
      }
    }
    else if (sVar1 == param_3) {
      sVar1 = *psVar10;
      sVar2 = psVar10[2];
      sVar4 = sVar1;
      if (sVar1 <= sVar2) {
        sVar4 = sVar2;
        sVar2 = sVar1;
      }
      if ((sVar2 <= param_2) && (iVar9 = (int)sVar1, param_2 <= sVar4)) goto LAB_00409674;
    }
    psVar10 = psVar10 + 2;
    local_8 = local_8 + -1;
    local_4 = local_4 + 1;
    if (local_8 == 0) {
      return bVar5 & 0xffffff01;
    }
  } while( true );
}

