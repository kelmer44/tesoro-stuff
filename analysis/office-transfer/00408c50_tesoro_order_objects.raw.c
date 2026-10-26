/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408c50
 */


void tesoro_order_objects(int param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  ushort *puVar3;
  ushort uVar4;
  short *psVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  ushort *puVar13;
  int local_1d0;
  int local_1cc;
  uint local_1c8;
  int local_1c4;
  int local_1c0;
  uint local_1bc;
  short *local_1b8;
  int local_1b4;
  int local_1b0;
  int local_1ac;
  int local_1a8;
  int local_1a4;
  int local_1a0;
  int *local_19c;
  ushort *local_198;
  int local_194;
  short local_190 [200];
  
  iVar9 = ((*(int *)(param_1 + 0x28) - *(int *)(param_1 + 0x20)) - *(int *)(param_2 + 0x28)) +
          *(int *)(param_2 + 0x20);
  iVar11 = ((*(int *)(param_1 + 0x2c) - *(int *)(param_2 + 0x2c)) - *(int *)(param_1 + 0x24)) +
           *(int *)(param_2 + 0x24);
  local_1b4 = *(int *)(param_1 + 0x20) - *(int *)(param_2 + 0x20);
  local_1bc = *(int *)(param_1 + 0x10) + local_1b4;
  local_1b4 = *(int *)(param_1 + 0x18) + local_1b4;
  local_1b0 = *(int *)(param_1 + 0x24) - *(int *)(param_2 + 0x24);
  local_1b8 = (short *)(*(int *)(param_1 + 0x14) + local_1b0);
  local_1b0 = *(int *)(param_1 + 0x1c) + local_1b0;
  local_19c = (int *)(param_2 + 0x10);
  local_1d0 = FUN_00409280(local_1bc,(int)local_1b8,local_1b4,local_1b0,*local_19c,
                           *(int *)(param_2 + 0x14),*(int *)(param_2 + 0x18),
                           *(int *)(param_2 + 0x1c),iVar9,iVar11);
  if (local_1d0 < 0) {
    return;
  }
  local_198 = (ushort *)
              (*(int *)(*(int *)(param_1 + 0x38) + 0x1c) + 0x1c +
              (uint)*(ushort *)(*(int *)(param_1 + 0x38) + 0x30) * 0x24);
  puVar13 = (ushort *)
            (*(int *)(*(int *)(param_2 + 0x38) + 0x1c) + 0x1c +
            (uint)*(ushort *)(*(int *)(param_2 + 0x38) + 0x30) * 0x24);
  uVar4 = *local_198;
  if ((uVar4 == 0) || (*puVar13 == 0)) goto LAB_004090b0;
  if ((uVar4 < 2) || (*puVar13 < 2)) {
    if (uVar4 == 1) {
      if ((iVar9 == 0) && (iVar11 == 0)) goto LAB_00409019;
      uVar8 = FUN_00409560(puVar13,*(int *)(param_1 + 0x20) - *(int *)(param_2 + 0x4c),
                           *(int *)(param_1 + 0x24) - *(int *)(param_2 + 0x50));
      if ((char)uVar8 != '\0') {
        iVar9 = *(int *)(param_1 + 0x28) - *(int *)(param_2 + 0x4c);
        iVar11 = *(int *)(param_1 + 0x2c) - *(int *)(param_2 + 0x50);
LAB_00409096:
        uVar8 = FUN_00409560(puVar13,iVar9,iVar11);
        local_1d0 = -(uint)((char)uVar8 != '\0');
        goto LAB_004090b0;
      }
    }
    else {
      if ((iVar9 == 0) && (iVar11 == 0)) {
        local_1d0 = 1;
        goto LAB_004090b0;
      }
      uVar8 = FUN_00409560(local_198,*(int *)(param_2 + 0x20) - *(int *)(param_1 + 0x4c),
                           *(int *)(param_2 + 0x24) - *(int *)(param_1 + 0x50));
      if ((char)uVar8 != '\0') {
        iVar9 = *(int *)(param_2 + 0x28) - *(int *)(param_1 + 0x4c);
        iVar11 = *(int *)(param_2 + 0x2c) - *(int *)(param_1 + 0x50);
        puVar13 = local_198;
        goto LAB_00409096;
      }
    }
    local_1d0 = 0;
  }
  else {
    local_1b8 = local_190;
    local_1a0 = -iVar11;
    local_1a4 = -iVar9;
    local_1ac = *(int *)(param_2 + 0x20) * 0x10000;
    local_1a8 = *(int *)(param_2 + 0x24) * 0x10000;
    if ((iVar9 == 0) && (iVar11 == 0)) {
      local_1c4 = 0;
      local_1c0 = 0;
      local_1cc = 0;
    }
    else {
      iVar7 = local_1a0;
      if (0 < iVar11) {
        iVar7 = iVar11;
      }
      iVar10 = local_1a4;
      if (0 < iVar9) {
        iVar10 = iVar9;
      }
      if (iVar10 < iVar7) {
        local_1cc = local_1a0;
        if (0 < iVar11) {
          local_1cc = iVar11;
        }
        local_1c0 = (iVar9 * -0x10000) / local_1cc;
        local_1c4 = ((local_1a0 < 1) - 1 & 0x20000) - 0x10000;
        local_1ac = local_1ac + 0x8000;
      }
      else {
        local_1cc = local_1a4;
        if (0 < iVar9) {
          local_1cc = iVar9;
        }
        local_1c0 = ((local_1a4 < 1) - 1 & 0x20000) - 0x10000;
        local_1c4 = (iVar11 * -0x10000) / local_1cc;
        local_1a8 = local_1a8 + 0x8000;
      }
    }
    iVar9 = local_1cc * local_1d0 >> 0x10;
    local_1ac = local_1ac + iVar9 * local_1c0;
    local_1a8 = local_1a8 + iVar9 * local_1c4;
    for (iVar9 = local_1cc - iVar9; -1 < iVar9; iVar9 = iVar9 + -1) {
      iVar10 = *(int *)(param_1 + 0x20) - (local_1ac >> 0x10);
      iVar12 = *(int *)(param_1 + 0x24) - (local_1a8 >> 0x10);
      iVar11 = *(int *)(param_1 + 0x10) + iVar10;
      iVar7 = *(int *)(param_1 + 0x18) + iVar10;
      if ((iVar11 <= *(int *)(param_2 + 0x18)) && (*local_19c <= iVar7)) {
        FUN_00409c10(puVar13,*local_19c,local_19c[1],local_19c[2],local_19c[3],
                     (undefined2 *)&local_1bc,iVar11,*(int *)(param_1 + 0x14) + iVar12,iVar7);
        puVar3 = local_198;
        psVar5 = local_190;
        for (uVar8 = local_1bc & 0xffff; uVar8 != 0; uVar8 = uVar8 - 1) {
          *psVar5 = *psVar5 - (short)iVar10;
          psVar5[1] = psVar5[1] - (short)iVar12;
          psVar5 = psVar5 + 2;
        }
        uVar8 = FUN_00409790(local_198,(ushort *)&local_1bc);
        if (((char)uVar8 != '\0') ||
           (uVar8 = FUN_00409790((ushort *)&local_1bc,puVar3), (char)uVar8 != '\0')) break;
      }
      if (iVar9 == 0) {
        return;
      }
      local_1a8 = local_1a8 + local_1c4;
      local_1ac = local_1ac + local_1c0;
    }
    if (((local_1a4 != 0) || (local_1a0 != 0)) && (local_1cc != iVar9)) {
      local_1d0 = (((local_1cc - iVar9) + -1) * 0x10000) / local_1cc;
      goto LAB_004090b0;
    }
LAB_00409019:
    local_1d0 = -1;
  }
LAB_004090b0:
  if (local_1d0 != -1) {
    iVar9 = *(int *)(local_194 + 0x78);
    *(undefined1 *)(iVar9 + 0x20) = 1;
    *(int *)(iVar9 + 0x18) = iVar9 + 4;
    uVar4 = *(ushort *)(param_1 + 0x5e);
    if (*(ushort *)(param_1 + 0x5e) <= *(ushort *)(param_2 + 0x5e)) {
      uVar4 = *(ushort *)(param_2 + 0x5e);
    }
    local_1c8 = (uint)uVar4;
    if ((param_1 == DAT_004420fc) || (param_2 == DAT_004420fc)) {
      local_1c8 = 100;
    }
    while( true ) {
      iVar9 = *(int *)(local_194 + 0x78);
      iVar11 = **(int **)(iVar9 + 0x18);
      if (iVar11 == 0) break;
      if ((*(int **)(iVar9 + 0x18) == (int *)(iVar9 + 4)) && (*(char *)(iVar9 + 0x20) != '\0')) {
        *(undefined1 *)(iVar9 + 0x20) = 0;
      }
      else {
        *(int *)(iVar9 + 0x18) = iVar11;
      }
      iVar11 = **(int **)(iVar9 + 0x18);
      if (iVar11 == 0) break;
      *(undefined4 *)(iVar9 + 0x24) = *(undefined4 *)(iVar11 + 4);
      *(undefined4 *)(iVar9 + 0x28) = *(undefined4 *)(iVar11 + 8);
      *(undefined4 *)(iVar9 + 0x2c) = *(undefined4 *)(iVar11 + 0xc);
      *(undefined4 *)(iVar9 + 0x30) = *(undefined4 *)(iVar11 + 0x10);
      iVar9 = *(int *)(*(int *)(local_194 + 0x78) + 0x2c);
      if ((local_1d0 < iVar9) ||
         ((local_1d0 == iVar9 &&
          ((int)local_1c8 < (int)*(short *)(*(int *)(local_194 + 0x78) + 0x30))))) {
        piVar1 = *(int **)(local_194 + 0x78);
        local_1b0 = CONCAT22(local_1b0._2_2_,(short)local_1c8);
        piVar2 = (int *)*piVar1;
        piVar6 = (int *)*piVar2;
        if (piVar6 == (int *)0x0) {
          piVar6 = (int *)piVar2[5];
          piVar2[5] = (int)(piVar6 + 5);
        }
        else {
          *piVar2 = *piVar6;
        }
        *piVar6 = *(undefined4 *)piVar1[6];
        *(int **)piVar1[6] = piVar6;
        piVar6[1] = param_1;
        piVar6[2] = param_2;
        piVar6[3] = local_1d0;
        piVar6[4] = local_1b0;
        return;
      }
    }
    piVar1 = *(int **)(local_194 + 0x78);
    local_1b0 = CONCAT22(local_1b0._2_2_,(short)local_1c8);
    *(undefined1 *)(piVar1 + 8) = 1;
    piVar1[6] = (int)(piVar1 + 1);
    while( true ) {
      iVar9 = *(int *)piVar1[6];
      if (iVar9 == 0) break;
      if (((int *)piVar1[6] == piVar1 + 1) && ((char)piVar1[8] != '\0')) {
        *(undefined1 *)(piVar1 + 8) = 0;
      }
      else {
        piVar1[6] = iVar9;
      }
      iVar9 = *(int *)piVar1[6];
      if (iVar9 == 0) break;
      piVar1[9] = *(int *)(iVar9 + 4);
      piVar1[10] = *(int *)(iVar9 + 8);
      piVar1[0xb] = *(int *)(iVar9 + 0xc);
      piVar1[0xc] = *(int *)(iVar9 + 0x10);
    }
    piVar2 = (int *)*piVar1;
    piVar6 = (int *)*piVar2;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)piVar2[5];
      piVar2[5] = (int)(piVar6 + 5);
    }
    else {
      *piVar2 = *piVar6;
    }
    *piVar6 = *(undefined4 *)piVar1[6];
    *(int **)piVar1[6] = piVar6;
    piVar6[1] = param_1;
    piVar6[2] = param_2;
    piVar6[3] = local_1d0;
    piVar6[4] = local_1b0;
  }
  return;
}

