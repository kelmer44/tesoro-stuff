/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414740
 */


int __thiscall tesoro_floor_scale_at(void *this,int param_1,int param_2)

{
  ushort uVar1;
  ushort uVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  short *psVar7;
  int iVar8;
  int iVar9;
  short *psVar10;
  int iVar11;
  short *psVar12;
  int iVar13;
  int iVar14;
  char local_29;
  int local_1c;
  int local_c;
  int local_8;
  
  bVar3 = false;
  iVar14 = *(int *)(*(int *)((int)this + 0x38) + 0x1c) +
           (uint)*(ushort *)(*(int *)((int)this + 0x38) + 0x30) * 0x24;
  iVar8 = param_1 - *(int *)((int)this + 0x4c);
  iVar11 = param_2 - *(int *)((int)this + 0x50);
  iVar4 = *(ushort *)(iVar14 + 0x1c) - 1;
  local_1c = 0;
  if (0 < iVar4) {
    psVar7 = *(short **)(iVar14 + 0x20);
    do {
      psVar12 = psVar7 + 2;
      if (*psVar7 < *psVar12) {
        uVar1 = *(ushort *)(*(int *)(iVar14 + 0x14) + local_1c * 2);
        uVar2 = *(ushort *)(*(int *)(iVar14 + 0x14) + 2 + local_1c * 2);
        psVar10 = psVar7;
      }
      else {
        uVar2 = *(ushort *)(*(int *)(iVar14 + 0x14) + local_1c * 2);
        uVar1 = *(ushort *)(*(int *)(iVar14 + 0x14) + 2 + local_1c * 2);
        psVar10 = psVar12;
        psVar12 = psVar7;
      }
      uVar6 = (uint)uVar1;
      iVar5 = (int)*psVar10;
      if ((iVar5 <= iVar8) && (iVar8 <= *psVar12)) {
        iVar9 = *psVar12 - iVar5;
        iVar13 = (((int)psVar12[1] - (int)psVar10[1]) * (iVar8 - iVar5)) / iVar9 + (int)psVar10[1];
        if (bVar3) {
          if (local_29 != '\0') {
            if (iVar13 <= iVar11) goto LAB_004148ad;
            if (local_29 != '\0') goto LAB_00414872;
          }
          if (iVar11 <= iVar13) {
LAB_004148ad:
            iVar14 = (int)((iVar8 - *psVar10) * (uVar2 - uVar6)) / ((int)*psVar12 - (int)*psVar10) +
                     uVar6;
            if (local_c == iVar13) {
              return iVar14;
            }
            if (local_29 == '\0') {
              return ((iVar11 - local_c) * (iVar14 - local_8)) / (iVar13 - local_c) + local_8;
            }
            return ((iVar11 - iVar13) * (local_8 - iVar14)) / (local_c - iVar13) + iVar14;
          }
        }
        else {
          bVar3 = true;
          local_8 = (int)((uVar2 - uVar6) * (iVar8 - iVar5)) / iVar9 + uVar6;
          local_29 = iVar11 < iVar13;
          local_c = iVar13;
        }
      }
LAB_00414872:
      local_1c = local_1c + 1;
      psVar7 = psVar7 + 2;
    } while (local_1c < iVar4);
  }
  if (!bVar3) {
    local_8 = 0x100;
  }
  return local_8;
}

