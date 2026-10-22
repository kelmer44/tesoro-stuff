/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004112d0
 */


void tesoro_queue_shadow_spans
               (undefined4 *param_1,undefined4 *param_2,short param_3,int param_4,ushort param_5,
               char param_6,undefined4 param_7)

{
  ushort *puVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 uVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined2 *puVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  short sVar13;
  int iVar14;
  short sVar15;
  int iVar16;
  int local_c;
  
  sVar15 = param_3;
  _param_3 = (uint)(ushort)param_3;
  if (sVar15 != 0) {
    local_c = (_param_3 + 0xffff & 0xffff) + 1;
    puVar12 = param_2;
    do {
      sVar15 = *(short *)(param_1 + 1);
      puVar10 = (undefined2 *)*param_1;
      iVar6 = param_4;
      puVar9 = puVar12;
joined_r0x0041131a:
      if (sVar15 != 0) {
        sVar15 = sVar15 + -1;
        uVar2 = *puVar10;
        iVar14 = iVar6 + (uint)(ushort)puVar10[2];
        puVar1 = puVar10 + 3;
        puVar10 = puVar10 + 4;
        param_2 = (undefined4 *)((*puVar1 - 1) + iVar14);
        iVar6 = (uint)*puVar1 + iVar14;
        if (param_6 != '\0') goto code_r0x00411359;
        goto LAB_00411384;
      }
LAB_0041154a:
      param_1 = param_1 + 2;
      puVar12 = puVar12 + 200;
      local_c = local_c + -1;
    } while (local_c != 0);
  }
  return;
code_r0x00411359:
  if (-1 < (int)param_2) {
    if (iVar14 < 0) {
      iVar14 = 0;
    }
    if (0x27f < iVar14) goto LAB_0041154a;
    if (0x27f < (int)param_2) {
      param_2 = (undefined4 *)0x27f;
    }
LAB_00411384:
    bVar5 = false;
    puVar8 = (undefined4 *)*puVar9;
    while (puVar11 = puVar8, sVar13 = (short)iVar14, puVar11 != (undefined4 *)0x0) {
      iVar16 = (int)*(short *)((int)puVar11 + 10);
      sVar3 = *(short *)(puVar11 + 2);
      iVar7 = (int)sVar3;
      if (iVar14 <= iVar16) {
        if ((int)param_2 < iVar7) break;
        if (param_5 < *(ushort *)(puVar11 + 3)) {
          if (iVar7 < iVar14) {
            if ((int)param_2 < iVar16) {
              puVar9 = (undefined4 *)puVar12[4];
              if (puVar9 == (undefined4 *)0x0) {
                puVar9 = puVar12 + (uint)*(ushort *)(puVar12 + 3) * 4;
                *(ushort *)(puVar12 + 3) = *(ushort *)(puVar12 + 3) + 1;
              }
              else {
                puVar12[4] = *puVar9;
              }
              *puVar9 = *puVar11;
              puVar9[1] = puVar11[1];
              puVar9[2] = puVar11[2];
              puVar9[3] = puVar11[3];
              *(short *)(puVar9 + 2) = (short)param_2 + 1;
              *(short *)((int)puVar11 + 10) = sVar13 + -1;
              *puVar11 = puVar9;
              puVar9 = puVar11;
              break;
            }
            *(short *)((int)puVar11 + 10) = sVar13 + -1;
          }
          else {
            if ((int)param_2 < iVar16) {
              *(short *)(puVar11 + 2) = (short)param_2 + 1;
              break;
            }
            *puVar9 = *puVar11;
            *puVar11 = puVar12[4];
            puVar12[4] = puVar11;
            puVar11 = puVar9;
          }
        }
        else if (iVar14 < iVar7) {
          if ((int)param_2 <= iVar16) {
            param_2 = (undefined4 *)(iVar7 + -1);
            break;
          }
          puVar8 = (undefined4 *)puVar12[4];
          if (puVar8 == (undefined4 *)0x0) {
            puVar8 = puVar12 + (uint)*(ushort *)(puVar12 + 3) * 4;
            *(ushort *)(puVar12 + 3) = *(ushort *)(puVar12 + 3) + 1;
          }
          else {
            puVar12[4] = *puVar8;
          }
          *(short *)(puVar8 + 2) = sVar13;
          *(short *)((int)puVar8 + 10) = sVar3 + -1;
          puVar8[1] = param_7;
          *(undefined2 *)((int)puVar8 + 0xe) = uVar2;
          *puVar8 = puVar11;
          *(ushort *)(puVar8 + 3) = param_5;
          *puVar9 = puVar8;
          iVar14 = iVar16 + 1;
        }
        else {
          if ((int)param_2 <= iVar16) {
            bVar5 = true;
            break;
          }
          iVar14 = iVar16 + 1;
        }
      }
      puVar9 = puVar11;
      puVar8 = (undefined4 *)*puVar11;
    }
    if (!bVar5) {
      puVar8 = (undefined4 *)puVar12[4];
      if (puVar8 == (undefined4 *)0x0) {
        puVar8 = puVar12 + (uint)*(ushort *)(puVar12 + 3) * 4;
        *(ushort *)(puVar12 + 3) = *(ushort *)(puVar12 + 3) + 1;
      }
      else {
        puVar12[4] = *puVar8;
      }
      uVar4 = *puVar9;
      *(short *)(puVar8 + 2) = sVar13;
      *puVar8 = uVar4;
      puVar8[1] = param_7;
      *(undefined2 *)((int)puVar8 + 0xe) = uVar2;
      *(short *)((int)puVar8 + 10) = (short)param_2;
      *(ushort *)(puVar8 + 3) = param_5;
      *puVar9 = puVar8;
    }
  }
  goto joined_r0x0041131a;
}

