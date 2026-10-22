/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004105a0
 */


void tesoro_queue_literal_spans
               (undefined4 *param_1,undefined4 *param_2,short param_3,int param_4,ushort param_5,
               char param_6)

{
  ushort *puVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  bool bVar5;
  undefined4 *puVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  short sVar11;
  int iVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  int local_1c;
  int local_18;
  int local_c;
  
  sVar3 = param_3;
  _param_3 = (uint)(ushort)param_3;
  if (sVar3 != 0) {
    local_c = (_param_3 + 0xffff & 0xffff) + 1;
    do {
      piVar7 = (int *)*param_1;
      sVar3 = *(short *)(param_1 + 1);
      iVar2 = param_4;
      puVar14 = param_2;
joined_r0x004105e7:
      if (sVar3 != 0) {
        sVar3 = sVar3 + -1;
        puVar1 = (ushort *)((int)piVar7 + 6);
        local_18 = *piVar7;
        iVar12 = iVar2 + (uint)*(ushort *)(piVar7 + 1);
        piVar7 = piVar7 + 2;
        iVar2 = (uint)*puVar1 + iVar12;
        local_1c = iVar2 + -1;
        if (param_6 != '\0') goto code_r0x00410628;
        goto LAB_0041065b;
      }
LAB_00410868:
      param_1 = param_1 + 2;
      param_2 = param_2 + 500;
      local_c = local_c + -1;
    } while (local_c != 0);
  }
  return;
code_r0x00410628:
  if (-1 < local_1c) {
    if (iVar12 < 0) {
      local_18 = local_18 + iVar12 * -2;
      iVar12 = 0;
    }
    if (0x27f < iVar12) goto LAB_00410868;
    if (0x27f < local_1c) {
      local_1c = 0x27f;
    }
LAB_0041065b:
    bVar5 = false;
    puVar6 = (undefined4 *)*puVar14;
    iVar9 = local_18;
    while (puVar13 = puVar6, sVar11 = (short)iVar12, puVar13 != (undefined4 *)0x0) {
      iVar8 = (int)*(short *)((int)puVar13 + 10);
      sVar4 = *(short *)(puVar13 + 2);
      iVar10 = (int)sVar4;
      if (iVar12 <= iVar8) {
        if (local_1c < iVar10) break;
        if (param_5 < *(ushort *)(puVar13 + 3)) {
          if (iVar10 < iVar12) {
            if (local_1c < iVar8) {
              puVar6 = (undefined4 *)param_2[4];
              if (puVar6 == (undefined4 *)0x0) {
                puVar6 = param_2 + (uint)*(ushort *)(param_2 + 3) * 4;
                *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) + 1;
              }
              else {
                param_2[4] = *puVar6;
              }
              iVar12 = puVar13[1];
              *puVar6 = *puVar13;
              puVar6[1] = puVar13[1];
              puVar6[2] = puVar13[2];
              puVar6[3] = puVar13[3];
              puVar14 = puVar13;
              if (iVar12 < 2) {
                *(short *)(puVar6 + 2) = (short)local_1c + 1;
                *(short *)((int)puVar13 + 10) = sVar11 + -1;
                *puVar13 = puVar6;
              }
              else {
                iVar12 = (local_1c - iVar10) + 1;
                *(short *)(puVar6 + 2) = *(short *)(puVar6 + 2) + (short)iVar12;
                puVar6[1] = puVar6[1] + iVar12 * 2;
                *(short *)((int)puVar13 + 10) = sVar11 + -1;
                *puVar13 = puVar6;
              }
              break;
            }
            *(short *)((int)puVar13 + 10) = sVar11 + -1;
          }
          else {
            if (local_1c < iVar8) {
              if ((int)puVar13[1] < 2) {
                *(short *)(puVar13 + 2) = (short)local_1c + 1;
              }
              else {
                iVar12 = (local_1c - iVar10) + 1;
                *(short *)(puVar13 + 2) = *(short *)(puVar13 + 2) + (short)iVar12;
                puVar13[1] = puVar13[1] + iVar12 * 2;
              }
              break;
            }
            *puVar14 = *puVar13;
            *puVar13 = param_2[4];
            param_2[4] = puVar13;
            puVar13 = puVar14;
          }
        }
        else {
          if (iVar12 < iVar10) {
            if (local_1c <= iVar8) {
              local_1c = iVar10 + -1;
              break;
            }
            puVar6 = (undefined4 *)param_2[4];
            if (puVar6 == (undefined4 *)0x0) {
              puVar6 = param_2 + (uint)*(ushort *)(param_2 + 3) * 4;
              *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) + 1;
            }
            else {
              param_2[4] = *puVar6;
            }
            *puVar6 = puVar13;
            *(short *)((int)puVar6 + 10) = sVar4 + -1;
            *(short *)(puVar6 + 2) = sVar11;
            puVar6[1] = local_18;
            *(ushort *)(puVar6 + 3) = param_5;
            *puVar14 = puVar6;
            iVar9 = local_18;
          }
          else if (local_1c <= iVar8) {
            bVar5 = true;
            break;
          }
          iVar8 = (iVar8 - iVar12) + 1;
          iVar12 = iVar12 + iVar8;
          iVar9 = iVar9 + iVar8 * 2;
          local_18 = iVar9;
        }
      }
      puVar14 = puVar13;
      puVar6 = (undefined4 *)*puVar13;
    }
    if (!bVar5) {
      puVar6 = (undefined4 *)param_2[4];
      if (puVar6 == (undefined4 *)0x0) {
        puVar6 = param_2 + (uint)*(ushort *)(param_2 + 3) * 4;
        *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) + 1;
      }
      else {
        param_2[4] = *puVar6;
      }
      *puVar6 = *puVar14;
      *(short *)(puVar6 + 2) = sVar11;
      *(undefined2 *)((int)puVar6 + 10) = (undefined2)local_1c;
      puVar6[1] = iVar9;
      *(ushort *)(puVar6 + 3) = param_5;
      *puVar14 = puVar6;
    }
  }
  goto joined_r0x004105e7;
}

