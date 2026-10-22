/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410cc0
 */


void tesoro_queue_solid_spans
               (undefined4 *param_1,undefined4 *param_2,short param_3,int param_4,ushort param_5,
               char param_6)

{
  ushort *puVar1;
  undefined2 uVar2;
  short sVar3;
  bool bVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined2 *puVar7;
  short sVar8;
  int iVar9;
  int iVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  short sVar14;
  int iVar15;
  int local_8;
  
  sVar8 = param_3;
  _param_3 = (uint)(ushort)param_3;
  if (sVar8 != 0) {
    local_8 = (_param_3 + 0xffff & 0xffff) + 1;
    puVar11 = param_2;
    do {
      sVar8 = *(short *)(param_1 + 1);
      puVar7 = (undefined2 *)*param_1;
      iVar5 = param_4;
      puVar13 = puVar11;
joined_r0x00410d0a:
      if (sVar8 != 0) {
        sVar8 = sVar8 + -1;
        uVar2 = *puVar7;
        iVar15 = iVar5 + (uint)(ushort)puVar7[2];
        puVar1 = puVar7 + 3;
        puVar7 = puVar7 + 4;
        param_2 = (undefined4 *)((*puVar1 - 1) + iVar15);
        iVar5 = (uint)*puVar1 + iVar15;
        if (param_6 != '\0') goto code_r0x00410d49;
        goto LAB_00410d73;
      }
LAB_00410f77:
      param_1 = param_1 + 2;
      puVar11 = puVar11 + 500;
      local_8 = local_8 + -1;
    } while (local_8 != 0);
  }
  return;
code_r0x00410d49:
  if (-1 < (int)param_2) {
    if (iVar15 < 0) {
      iVar15 = 0;
    }
    if (0x27f < iVar15) goto LAB_00410f77;
    if (0x27f < (int)param_2) {
      param_2 = (undefined4 *)0x27f;
    }
LAB_00410d73:
    bVar4 = false;
    puVar6 = (undefined4 *)*puVar13;
    while (puVar12 = puVar6, sVar14 = (short)iVar15, puVar12 != (undefined4 *)0x0) {
      iVar9 = (int)*(short *)((int)puVar12 + 10);
      sVar3 = *(short *)(puVar12 + 2);
      iVar10 = (int)sVar3;
      if (iVar15 <= iVar9) {
        if ((int)param_2 < iVar10) break;
        if (param_5 < *(ushort *)(puVar12 + 3)) {
          if (iVar10 < iVar15) {
            if ((int)param_2 < iVar9) {
              puVar6 = (undefined4 *)puVar11[4];
              if (puVar6 == (undefined4 *)0x0) {
                puVar6 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
                *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
              }
              else {
                puVar11[4] = *puVar6;
              }
              iVar15 = puVar12[1];
              *puVar6 = *puVar12;
              puVar6[1] = puVar12[1];
              puVar6[2] = puVar12[2];
              puVar6[3] = puVar12[3];
              puVar13 = puVar12;
              if (iVar15 < 2) {
                *(short *)(puVar6 + 2) = (short)param_2 + 1;
                *(short *)((int)puVar12 + 10) = sVar14 + -1;
                *puVar12 = puVar6;
              }
              else {
                iVar15 = (int)param_2 + (1 - iVar10);
                *(short *)(puVar6 + 2) = *(short *)(puVar6 + 2) + (short)iVar15;
                puVar6[1] = puVar6[1] + iVar15 * 2;
                *(short *)((int)puVar12 + 10) = sVar14 + -1;
                *puVar12 = puVar6;
              }
              break;
            }
            *(short *)((int)puVar12 + 10) = sVar14 + -1;
          }
          else {
            if ((int)param_2 < iVar9) {
              if ((int)puVar12[1] < 2) {
                *(short *)(puVar12 + 2) = (short)param_2 + 1;
              }
              else {
                iVar15 = (int)param_2 + (1 - iVar10);
                *(short *)(puVar12 + 2) = *(short *)(puVar12 + 2) + (short)iVar15;
                puVar12[1] = puVar12[1] + iVar15 * 2;
              }
              break;
            }
            *puVar13 = *puVar12;
            *puVar12 = puVar11[4];
            puVar11[4] = puVar12;
            puVar12 = puVar13;
          }
        }
        else {
          if (iVar15 < iVar10) {
            if ((int)param_2 <= iVar9) {
              param_2 = (undefined4 *)(iVar10 + -1);
              break;
            }
            puVar6 = (undefined4 *)puVar11[4];
            if (puVar6 == (undefined4 *)0x0) {
              puVar6 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
              *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
            }
            else {
              puVar11[4] = *puVar6;
            }
            *(short *)(puVar6 + 2) = sVar14;
            *puVar6 = puVar12;
            *(short *)((int)puVar6 + 10) = sVar3 + -1;
            *(undefined2 *)((int)puVar6 + 0xe) = uVar2;
            puVar6[1] = 1;
            *(ushort *)(puVar6 + 3) = param_5;
            *puVar13 = puVar6;
          }
          else if ((int)param_2 <= iVar9) {
            bVar4 = true;
            break;
          }
          iVar15 = iVar9 + 1;
        }
      }
      puVar13 = puVar12;
      puVar6 = (undefined4 *)*puVar12;
    }
    if (!bVar4) {
      puVar6 = (undefined4 *)puVar11[4];
      if (puVar6 == (undefined4 *)0x0) {
        puVar6 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
        *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
      }
      else {
        puVar11[4] = *puVar6;
      }
      *puVar6 = *puVar13;
      *(undefined2 *)((int)puVar6 + 10) = param_2._0_2_;
      *(short *)(puVar6 + 2) = sVar14;
      puVar6[1] = 1;
      *(undefined2 *)((int)puVar6 + 0xe) = uVar2;
      *(ushort *)(puVar6 + 3) = param_5;
      *puVar13 = puVar6;
    }
  }
  goto joined_r0x00410d0a;
}

