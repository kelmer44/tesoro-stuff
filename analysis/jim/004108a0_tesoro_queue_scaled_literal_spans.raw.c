/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004108a0
 */


void __thiscall
tesoro_queue_scaled_literal_spans
          (void *this,int param_1,undefined4 *param_2,short param_3,int param_4,int param_5,
          ushort param_6,char param_7,int param_8,int param_9,int param_10,uint param_11)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  ushort uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  int *piVar13;
  int iVar14;
  undefined2 *puVar15;
  int iVar16;
  int local_2c;
  int local_1c;
  uint local_18;
  int local_10;
  undefined4 *local_c;
  int local_8;
  
  sVar1 = param_3;
  _param_3 = (uint)(ushort)param_3;
  iVar5 = (int)(0x1000000 / (longlong)(param_8 >> 8));
  param_4 = param_4 >> 0x10;
  local_1c = *(int *)((int)this + 0x678) + (param_11 & 0xffff) * 0x500;
  if (sVar1 != 0) {
    local_2c = (_param_3 + 0xffff & 0xffff) + 1;
    do {
      local_c = param_2;
      iVar6 = param_5 >> 0x10;
      param_5 = param_5 + param_9;
      iVar14 = 0;
      piVar13 = *(int **)(param_1 + iVar6 * 8);
      sVar1 = *(short *)(param_1 + iVar6 * 8 + 4);
      if (sVar1 != 0) {
        do {
          sVar1 = sVar1 + -1;
          local_10 = *piVar13;
          uVar11 = (iVar14 + (uint)*(ushort *)(piVar13 + 1)) * param_8;
          iVar14 = iVar14 + (uint)*(ushort *)(piVar13 + 1) + (uint)*(ushort *)((int)piVar13 + 6);
          _param_3 = ((int)uVar11 >> 0x10) + param_4;
          piVar13 = piVar13 + 2;
          local_8 = (iVar14 * param_8 + -0x7fff >> 0x10) + param_4;
          local_18 = -((uVar11 >> 8 & 0xff) * (iVar5 >> 8));
          if (local_18 != 0) {
            _param_3 = _param_3 + 1;
            local_18 = local_18 + iVar5;
          }
          if (param_7 == '\0') {
LAB_004109e4:
            if ((int)_param_3 <= local_8) {
              puVar15 = (undefined2 *)(local_1c + _param_3 * 2);
              uVar10 = (local_8 - _param_3) + 1;
              uVar11 = local_18;
              do {
                uVar12 = uVar11 >> 0x10;
                uVar11 = uVar11 + iVar5;
                *puVar15 = *(undefined2 *)(local_10 + uVar12 * 2);
                puVar15 = puVar15 + 1;
                uVar9 = (short)uVar10 - 1;
                uVar10 = (uint)uVar9;
              } while (uVar9 != 0);
              bVar4 = false;
              puVar8 = (undefined4 *)*local_c;
              while (puVar8 != (undefined4 *)0x0) {
                iVar6 = (int)*(short *)((int)puVar8 + 10);
                sVar2 = *(short *)(puVar8 + 2);
                iVar16 = (int)sVar2;
                if ((int)_param_3 <= iVar6) {
                  if (local_8 < iVar16) break;
                  if (param_6 < *(ushort *)(puVar8 + 3)) {
                    if (iVar16 < (int)_param_3) {
                      if (local_8 < iVar6) {
                        puVar7 = (undefined4 *)param_2[4];
                        if (puVar7 == (undefined4 *)0x0) {
                          puVar7 = param_2 + (uint)*(ushort *)(param_2 + 3) * 4;
                          *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) + 1;
                        }
                        else {
                          param_2[4] = *puVar7;
                        }
                        if ((int)puVar8[1] < 2) {
                          *puVar7 = *puVar8;
                          puVar7[1] = puVar8[1];
                          puVar7[2] = puVar8[2];
                          puVar7[3] = puVar8[3];
                          *(short *)(puVar7 + 2) = (short)local_8 + 1;
                        }
                        else {
                          *puVar7 = *puVar8;
                          puVar7[1] = puVar8[1];
                          puVar7[2] = puVar8[2];
                          puVar7[3] = puVar8[3];
                          iVar6 = (local_8 - iVar16) + 1;
                          *(short *)(puVar7 + 2) = *(short *)(puVar7 + 2) + (short)iVar6;
                          puVar7[1] = puVar7[1] + iVar6 * 2;
                        }
                        *puVar8 = puVar7;
                        *(short *)((int)puVar8 + 10) = param_3 + -1;
                        local_c = puVar8;
                        break;
                      }
                      *(short *)((int)puVar8 + 10) = param_3 + -1;
                    }
                    else {
                      if (local_8 < iVar6) {
                        if ((int)puVar8[1] < 2) {
                          *(short *)(puVar8 + 2) = (short)local_8 + 1;
                        }
                        else {
                          iVar6 = (local_8 - iVar16) + 1;
                          *(short *)(puVar8 + 2) = *(short *)(puVar8 + 2) + (short)iVar6;
                          puVar8[1] = puVar8[1] + iVar6 * 2;
                        }
                        break;
                      }
                      *local_c = *puVar8;
                      *puVar8 = param_2[4];
                      param_2[4] = puVar8;
                      puVar8 = local_c;
                    }
                  }
                  else {
                    if ((int)_param_3 < iVar16) {
                      if (local_8 <= iVar6) {
                        local_8 = iVar16 + -1;
                        break;
                      }
                      puVar7 = (undefined4 *)param_2[4];
                      if (puVar7 == (undefined4 *)0x0) {
                        puVar7 = param_2 + (uint)*(ushort *)(param_2 + 3) * 4;
                        *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) + 1;
                      }
                      else {
                        param_2[4] = *puVar7;
                      }
                      *(short *)(puVar7 + 2) = param_3;
                      *(ushort *)(puVar7 + 3) = param_6;
                      *puVar7 = puVar8;
                      *(short *)((int)puVar7 + 10) = sVar2 + -1;
                      puVar7[1] = 0;
                      *local_c = puVar7;
                      local_10 = local_10 + ((iVar6 + 1) - (int)*(short *)(puVar7 + 2)) * 2;
                    }
                    else {
                      if (local_8 <= iVar6) {
                        bVar4 = true;
                        break;
                      }
                      local_10 = local_10 + 2 + (iVar6 - _param_3) * 2;
                    }
                    _param_3 = iVar6 + 1;
                  }
                }
                local_c = puVar8;
                puVar8 = (undefined4 *)*local_c;
              }
              if (!bVar4) {
                puVar8 = (undefined4 *)param_2[4];
                if (puVar8 == (undefined4 *)0x0) {
                  puVar8 = param_2 + (uint)*(ushort *)(param_2 + 3) * 4;
                  *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) + 1;
                }
                else {
                  param_2[4] = *puVar8;
                }
                uVar3 = *local_c;
                *(short *)(puVar8 + 2) = param_3;
                *puVar8 = uVar3;
                *(short *)((int)puVar8 + 10) = (short)local_8;
                if (param_8 == 0x10000) {
                  local_10 = local_10 + ((int)local_18 >> 0x10) * 2;
                }
                else {
                  local_10 = 0;
                }
                puVar8[1] = local_10;
                *(ushort *)(puVar8 + 3) = param_6;
                *local_c = puVar8;
              }
            }
          }
          else if (-1 < local_8) {
            if ((int)_param_3 < 0) {
              local_18 = local_18 - _param_3 * iVar5;
              _param_3 = 0;
            }
            if ((int)_param_3 < 0x280) {
              if (0x27f < local_8) {
                local_8 = 0x27f;
              }
              goto LAB_004109e4;
            }
            break;
          }
        } while (sVar1 != 0);
      }
      param_4 = param_4 + param_10;
      local_1c = local_1c + 0x500;
      param_2 = param_2 + 500;
      local_2c = local_2c + -1;
    } while (local_2c != 0);
  }
  return;
}

