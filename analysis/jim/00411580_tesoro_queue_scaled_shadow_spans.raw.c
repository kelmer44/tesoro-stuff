/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00411580
 */


void tesoro_queue_scaled_shadow_spans
               (int param_1,undefined4 *param_2,short param_3,int param_4,int param_5,ushort param_6
               ,char param_7,undefined4 param_8,int param_9,int param_10,int param_11)

{
  short sVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 uVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  short sVar9;
  int iVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  undefined2 *puVar14;
  int iVar15;
  int local_c;
  
  sVar1 = param_3;
  _param_3 = (uint)(ushort)param_3;
  if (sVar1 != 0) {
    local_c = (_param_3 + 0xffff & 0xffff) + 1;
    puVar11 = param_2;
    do {
      iVar6 = param_5 >> 0x10;
      param_5 = param_5 + param_10;
      iVar10 = 0;
      puVar8 = (undefined4 *)(param_1 + iVar6 * 8);
      sVar1 = *(short *)(puVar8 + 1);
      if (sVar1 != 0) {
        puVar13 = puVar11;
        puVar14 = (undefined2 *)*puVar8;
        do {
          sVar1 = sVar1 + -1;
          uVar2 = *puVar14;
          iVar6 = ((int)((iVar10 + (uint)(ushort)puVar14[2]) * param_9) >> 0x10) + (param_4 >> 0x10)
          ;
          param_2 = (undefined4 *)(((int)(param_9 * (uint)(ushort)puVar14[3]) >> 0x10) + -1 + iVar6)
          ;
          iVar10 = iVar10 + (uint)(ushort)puVar14[2] + (uint)(ushort)puVar14[3];
          if (param_7 == '\0') {
LAB_00411672:
            bVar5 = false;
            puVar8 = (undefined4 *)*puVar13;
            while (puVar12 = puVar8, sVar9 = (short)iVar6, puVar12 != (undefined4 *)0x0) {
              iVar15 = (int)*(short *)((int)puVar12 + 10);
              sVar3 = *(short *)(puVar12 + 2);
              iVar7 = (int)sVar3;
              if (iVar6 <= iVar15) {
                if ((int)param_2 < iVar7) break;
                if (param_6 < *(ushort *)(puVar12 + 3)) {
                  if (iVar7 < iVar6) {
                    if ((int)param_2 < iVar15) {
                      puVar8 = (undefined4 *)puVar11[4];
                      if (puVar8 == (undefined4 *)0x0) {
                        puVar8 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
                        *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
                      }
                      else {
                        puVar11[4] = *puVar8;
                      }
                      *puVar8 = *puVar12;
                      puVar8[1] = puVar12[1];
                      puVar8[2] = puVar12[2];
                      puVar8[3] = puVar12[3];
                      *(short *)(puVar8 + 2) = (short)param_2 + 1;
                      *(short *)((int)puVar12 + 10) = sVar9 + -1;
                      *puVar12 = puVar8;
                      puVar13 = puVar12;
                      break;
                    }
                    *(short *)((int)puVar12 + 10) = sVar9 + -1;
                  }
                  else {
                    if ((int)param_2 < iVar15) {
                      *(short *)(puVar12 + 2) = (short)param_2 + 1;
                      break;
                    }
                    *puVar13 = *puVar12;
                    *puVar12 = puVar11[4];
                    puVar11[4] = puVar12;
                    puVar12 = puVar13;
                  }
                }
                else if (iVar6 < iVar7) {
                  if ((int)param_2 <= iVar15) {
                    param_2 = (undefined4 *)(iVar7 + -1);
                    break;
                  }
                  puVar8 = (undefined4 *)puVar11[4];
                  if (puVar8 == (undefined4 *)0x0) {
                    puVar8 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
                    *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
                  }
                  else {
                    puVar11[4] = *puVar8;
                  }
                  *(short *)(puVar8 + 2) = sVar9;
                  *(short *)((int)puVar8 + 10) = sVar3 + -1;
                  puVar8[1] = param_8;
                  *(undefined2 *)((int)puVar8 + 0xe) = uVar2;
                  *puVar8 = puVar12;
                  *(ushort *)(puVar8 + 3) = param_6;
                  *puVar13 = puVar8;
                  iVar6 = iVar15 + 1;
                }
                else {
                  if ((int)param_2 <= iVar15) {
                    bVar5 = true;
                    break;
                  }
                  iVar6 = iVar15 + 1;
                }
              }
              puVar13 = puVar12;
              puVar8 = (undefined4 *)*puVar12;
            }
            if (!bVar5) {
              puVar8 = (undefined4 *)puVar11[4];
              if (puVar8 == (undefined4 *)0x0) {
                puVar8 = puVar11 + (uint)*(ushort *)(puVar11 + 3) * 4;
                *(ushort *)(puVar11 + 3) = *(ushort *)(puVar11 + 3) + 1;
              }
              else {
                puVar11[4] = *puVar8;
              }
              uVar4 = *puVar13;
              *(short *)(puVar8 + 2) = sVar9;
              *puVar8 = uVar4;
              puVar8[1] = param_8;
              *(short *)((int)puVar8 + 10) = (short)param_2;
              *(undefined2 *)((int)puVar8 + 0xe) = uVar2;
              *(ushort *)(puVar8 + 3) = param_6;
              *puVar13 = puVar8;
            }
          }
          else if (-1 < (int)param_2) {
            if (iVar6 < 0) {
              iVar6 = 0;
            }
            if (iVar6 < 0x280) {
              if (0x27f < (int)param_2) {
                param_2 = (undefined4 *)0x27f;
              }
              goto LAB_00411672;
            }
            break;
          }
          puVar14 = puVar14 + 4;
        } while (sVar1 != 0);
      }
      param_4 = param_4 + param_11;
      puVar11 = puVar11 + 200;
      local_c = local_c + -1;
    } while (local_c != 0);
  }
  return;
}

