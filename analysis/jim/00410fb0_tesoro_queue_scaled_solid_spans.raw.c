/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410fb0
 */


void tesoro_queue_scaled_solid_spans
               (int param_1,undefined4 *param_2,short param_3,int param_4,int param_5,ushort param_6
               ,char param_7,int param_8,int param_9,int param_10)

{
  short sVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 uVar4;
  bool bVar5;
  undefined4 *puVar6;
  short sVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 *puVar11;
  undefined2 *puVar12;
  int iVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  int local_8;
  
  sVar1 = param_3;
  _param_3 = (uint)(ushort)param_3;
  if (sVar1 != 0) {
    local_8 = (_param_3 + 0xffff & 0xffff) + 1;
    puVar11 = param_2;
    do {
      iVar9 = 0;
      iVar8 = param_5 >> 0x10;
      sVar1 = *(short *)(param_1 + iVar8 * 8 + 4);
      param_5 = param_5 + param_9;
      if (sVar1 != 0) {
        puVar12 = *(undefined2 **)(param_1 + iVar8 * 8);
        puVar15 = puVar11;
        do {
          sVar1 = sVar1 + -1;
          uVar2 = *puVar12;
          iVar8 = ((int)((iVar9 + (uint)(ushort)puVar12[2]) * param_8) >> 0x10) + (param_4 >> 0x10);
          iVar9 = iVar9 + (uint)(ushort)puVar12[2] + (uint)(ushort)puVar12[3];
          param_2 = (undefined4 *)(((int)(param_8 * (uint)(ushort)puVar12[3]) >> 0x10) + -1 + iVar8)
          ;
          if (param_7 == '\0') {
LAB_00411096:
            bVar5 = false;
            puVar6 = (undefined4 *)*puVar15;
            while (puVar14 = puVar6, sVar7 = (short)iVar8, puVar14 != (undefined4 *)0x0) {
              iVar13 = (int)*(short *)((int)puVar14 + 10);
              sVar3 = *(short *)(puVar14 + 2);
              iVar10 = (int)sVar3;
              if (iVar8 <= iVar13) {
                if ((int)param_2 < iVar10) break;
                if (param_6 < *(ushort *)(puVar14 + 3)) {
                  if (iVar10 < iVar8) {
                    if ((int)param_2 < iVar13) {
                      puVar6 = (undefined4 *)puVar11[4];
                      if (puVar6 == (undefined4 *)0x0) {
                        puVar6 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
                        *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
                      }
                      else {
                        puVar11[4] = *puVar6;
                      }
                      iVar8 = puVar14[1];
                      *puVar6 = *puVar14;
                      puVar6[1] = puVar14[1];
                      puVar6[2] = puVar14[2];
                      puVar6[3] = puVar14[3];
                      puVar15 = puVar14;
                      if (iVar8 < 2) {
                        *(short *)(puVar6 + 2) = (short)param_2 + 1;
                        *(short *)((int)puVar14 + 10) = sVar7 + -1;
                        *puVar14 = puVar6;
                      }
                      else {
                        iVar8 = (int)param_2 + (1 - iVar10);
                        *(short *)(puVar6 + 2) = *(short *)(puVar6 + 2) + (short)iVar8;
                        puVar6[1] = puVar6[1] + iVar8 * 2;
                        *(short *)((int)puVar14 + 10) = sVar7 + -1;
                        *puVar14 = puVar6;
                      }
                      break;
                    }
                    *(short *)((int)puVar14 + 10) = sVar7 + -1;
                  }
                  else {
                    if ((int)param_2 < iVar13) {
                      if ((int)puVar14[1] < 2) {
                        *(short *)(puVar14 + 2) = (short)param_2 + 1;
                      }
                      else {
                        iVar8 = (int)param_2 + (1 - iVar10);
                        *(short *)(puVar14 + 2) = *(short *)(puVar14 + 2) + (short)iVar8;
                        puVar14[1] = puVar14[1] + iVar8 * 2;
                      }
                      break;
                    }
                    *puVar15 = *puVar14;
                    *puVar14 = puVar11[4];
                    puVar11[4] = puVar14;
                    puVar14 = puVar15;
                  }
                }
                else {
                  if (iVar8 < iVar10) {
                    if ((int)param_2 <= iVar13) {
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
                    *(short *)(puVar6 + 2) = sVar7;
                    *puVar6 = puVar14;
                    *(short *)((int)puVar6 + 10) = sVar3 + -1;
                    *(undefined2 *)((int)puVar6 + 0xe) = uVar2;
                    puVar6[1] = 1;
                    *(ushort *)(puVar6 + 3) = param_6;
                    *puVar15 = puVar6;
                  }
                  else if ((int)param_2 <= iVar13) {
                    bVar5 = true;
                    break;
                  }
                  iVar8 = iVar13 + 1;
                }
              }
              puVar15 = puVar14;
              puVar6 = (undefined4 *)*puVar14;
            }
            if (!bVar5) {
              puVar6 = (undefined4 *)puVar11[4];
              if (puVar6 == (undefined4 *)0x0) {
                puVar6 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
                *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
              }
              else {
                puVar11[4] = *puVar6;
              }
              uVar4 = *puVar15;
              *(short *)(puVar6 + 2) = sVar7;
              *puVar6 = uVar4;
              *(undefined2 *)((int)puVar6 + 10) = param_2._0_2_;
              puVar6[1] = 1;
              *(undefined2 *)((int)puVar6 + 0xe) = uVar2;
              *(ushort *)(puVar6 + 3) = param_6;
              *puVar15 = puVar6;
            }
          }
          else if (-1 < (int)param_2) {
            if (iVar8 < 0) {
              iVar8 = 0;
            }
            if (iVar8 < 0x280) {
              if (0x27f < (int)param_2) {
                param_2 = (undefined4 *)0x27f;
              }
              goto LAB_00411096;
            }
            break;
          }
          puVar12 = puVar12 + 4;
        } while (sVar1 != 0);
      }
      param_4 = param_4 + param_10;
      puVar11 = puVar11 + 500;
      local_8 = local_8 + -1;
    } while (local_8 != 0);
  }
  return;
}

