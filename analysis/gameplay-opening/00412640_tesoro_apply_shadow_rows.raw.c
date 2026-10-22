/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00412640
 */


void __fastcall tesoro_apply_shadow_rows(int param_1)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  int *piVar4;
  bool bVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  int *piVar9;
  short sVar10;
  int iVar11;
  short sVar12;
  int iVar13;
  int iVar14;
  int *local_1c;
  int local_18;
  int *local_10;
  int *local_8;
  int local_4;
  
  local_8 = *(int **)(param_1 + 0x670);
  piVar9 = *(int **)(param_1 + 0x674);
  local_18 = *(int *)(param_1 + 0x678);
  local_4 = 0x1e0;
  do {
    iVar13 = *piVar9;
    local_10 = piVar9;
    local_1c = local_8;
    while (iVar13 != 0) {
      piVar4 = (int *)*local_10;
      sVar12 = (short)piVar4[2];
      iVar14 = (int)sVar12;
      sVar2 = *(short *)((int)piVar4 + 10);
      iVar13 = (int)sVar2;
      sVar1 = *(short *)(*local_1c + 10);
      piVar7 = (int *)*local_1c;
      while (piVar6 = piVar7, sVar1 < iVar14) {
        piVar7 = (int *)*piVar6;
        local_1c = piVar6;
        sVar1 = *(short *)(*piVar6 + 10);
      }
      sVar1 = (short)piVar6[2];
      iVar8 = (int)sVar1;
      sVar10 = *(short *)((int)piVar6 + 10);
      iVar11 = (int)sVar10;
      if (piVar4[1] == 0) {
        bVar5 = *(ushort *)(piVar6 + 3) < *(ushort *)(piVar4 + 3);
      }
      else if ((*(ushort *)(piVar6 + 3) < *(ushort *)(piVar4 + 1)) ||
              (*(ushort *)((int)piVar4 + 6) < *(ushort *)(piVar6 + 3))) {
        bVar5 = true;
      }
      else {
        bVar5 = false;
      }
      if (bVar5) {
        if (iVar11 < iVar13) {
          *(short *)(piVar4 + 2) = sVar10 + 1;
          local_1c = piVar6;
        }
        else {
          *piVar9 = *piVar4;
          *piVar4 = piVar9[4];
          piVar9[4] = (int)piVar4;
          local_10 = piVar9;
        }
      }
      else if (iVar14 == iVar8) {
        if (iVar11 < iVar13) {
          sVar10 = sVar10 + 1;
          *(short *)(piVar4 + 2) = sVar10;
          if (piVar6[1] < 2) {
            if (piVar6[1] == 0) {
              *(undefined2 *)(piVar6 + 2) = 0xffff;
              *(short *)((int)piVar6 + 10) = sVar10 - sVar1;
              piVar6[1] = local_18 + iVar8 * 2;
              *(undefined2 *)((int)piVar6 + 0xe) = *(undefined2 *)((int)piVar4 + 0xe);
              local_1c = piVar6;
            }
            else {
LAB_004127b9:
              *(undefined2 *)((int)piVar6 + 0xe) =
                   *(undefined2 *)
                    (((*(ushort *)((int)piVar6 + 0xe) & 0x71c |
                      (*(ushort *)((int)piVar6 + 0xe) & 0xe000) >> 8) >> 1) +
                     *(int *)(param_1 + 0x66c) + (uint)*(ushort *)((int)piVar4 + 0xe) * 0x100);
              local_1c = piVar6;
            }
          }
          else {
            *(short *)((int)piVar6 + 10) = sVar10 - sVar1;
            *(undefined2 *)(piVar6 + 2) = 0xffff;
            *(undefined2 *)((int)piVar6 + 0xe) = *(undefined2 *)((int)piVar4 + 0xe);
            local_1c = piVar6;
          }
        }
        else if (iVar13 == iVar11) {
          local_10 = piVar4;
          if (piVar6[1] < 2) {
            if (piVar6[1] != 0) goto LAB_004127b9;
            *(undefined2 *)(piVar6 + 2) = 0xffff;
            *(short *)((int)piVar6 + 10) = (sVar10 - sVar1) + 1;
            piVar6[1] = local_18 + iVar8 * 2;
            *(undefined2 *)((int)piVar6 + 0xe) = *(undefined2 *)((int)piVar4 + 0xe);
            local_1c = piVar6;
          }
          else {
            *(undefined2 *)(piVar6 + 2) = 0xffff;
            *(short *)((int)piVar6 + 10) = (sVar10 - sVar1) + 1;
            *(undefined2 *)((int)piVar6 + 0xe) = *(undefined2 *)((int)piVar4 + 0xe);
            local_1c = piVar6;
          }
        }
        else {
          *piVar9 = *piVar4;
          *piVar4 = (int)piVar6;
          *local_1c = (int)piVar4;
          *(short *)(piVar6 + 2) = sVar2 + 1;
          if (piVar6[1] < 2) {
            if (piVar6[1] == 0) {
              *(undefined2 *)(piVar4 + 2) = 0xffff;
              *(short *)((int)piVar4 + 10) = (sVar2 - sVar12) + 1;
              piVar4[1] = local_18 + iVar8 * 2;
              local_1c = piVar4;
              local_10 = piVar9;
            }
            else {
              uVar3 = *(undefined2 *)
                       (((*(ushort *)((int)piVar6 + 0xe) & 0x71c |
                         (*(ushort *)((int)piVar6 + 0xe) & 0xe000) >> 8) >> 1) +
                        *(int *)(param_1 + 0x66c) + (uint)*(ushort *)((int)piVar4 + 0xe) * 0x100);
              piVar4[1] = 1;
              *(undefined2 *)((int)piVar4 + 0xe) = uVar3;
              local_1c = piVar4;
              local_10 = piVar9;
            }
          }
          else {
            *(undefined2 *)(piVar4 + 2) = 0xffff;
            sVar12 = (sVar2 - sVar12) + 1;
            *(short *)((int)piVar4 + 10) = sVar12;
            piVar4[1] = piVar6[1];
            piVar6[1] = piVar6[1] + sVar12 * 2;
            local_1c = piVar4;
            local_10 = piVar9;
          }
        }
      }
      else if (iVar11 < iVar13) {
        local_1c = (int *)piVar9[4];
        if (local_1c == (int *)0x0) {
          local_1c = piVar9 + (uint)*(ushort *)(piVar9 + 3) * 4;
          *(ushort *)(piVar9 + 3) = *(ushort *)(piVar9 + 3) + 1;
        }
        else {
          piVar9[4] = *local_1c;
        }
        *(undefined2 *)((int)local_1c + 0xe) = *(undefined2 *)((int)piVar4 + 0xe);
        *(short *)(local_1c + 3) = (short)piVar4[3];
        *local_1c = *piVar6;
        *(short *)((int)piVar6 + 10) = sVar12 + -1;
        *piVar6 = (int)local_1c;
        *(short *)(piVar4 + 2) = sVar10 + 1;
        if (piVar6[1] < 2) {
          if (piVar6[1] == 0) {
            *(undefined2 *)(local_1c + 2) = 0xffff;
            *(short *)((int)local_1c + 10) = (sVar10 - sVar12) + 1;
            local_1c[1] = local_18 + iVar14 * 2;
          }
          else {
            *(short *)((int)local_1c + 10) = sVar10;
            *(short *)(local_1c + 2) = sVar12;
            local_1c[1] = 1;
            *(undefined2 *)((int)local_1c + 0xe) =
                 *(undefined2 *)
                  (((*(ushort *)((int)piVar6 + 0xe) & 0x71c |
                    (*(ushort *)((int)piVar6 + 0xe) & 0xe000) >> 8) >> 1) +
                   *(int *)(param_1 + 0x66c) + (uint)*(ushort *)((int)piVar4 + 0xe) * 0x100);
          }
        }
        else {
          *(undefined2 *)(local_1c + 2) = 0xffff;
          *(short *)((int)local_1c + 10) = (sVar10 - sVar12) + 1;
          local_1c[1] = piVar6[1] + (iVar14 - iVar8) * 2;
        }
      }
      else if (iVar13 == iVar11) {
        *piVar9 = *piVar4;
        *piVar4 = *piVar6;
        *(short *)((int)piVar6 + 10) = sVar12 + -1;
        *piVar6 = (int)piVar4;
        if (piVar6[1] < 2) {
          if (piVar6[1] == 0) {
            *(undefined2 *)(piVar4 + 2) = 0xffff;
            *(short *)((int)piVar4 + 10) = (sVar2 - sVar12) + 1;
            piVar4[1] = local_18 + iVar14 * 2;
            local_1c = piVar4;
            local_10 = piVar9;
          }
          else {
            uVar3 = *(undefined2 *)
                     (((*(ushort *)((int)piVar6 + 0xe) & 0x71c |
                       (*(ushort *)((int)piVar6 + 0xe) & 0xe000) >> 8) >> 1) +
                      *(int *)(param_1 + 0x66c) + (uint)*(ushort *)((int)piVar4 + 0xe) * 0x100);
            piVar4[1] = 1;
            *(undefined2 *)((int)piVar4 + 0xe) = uVar3;
            local_1c = piVar4;
            local_10 = piVar9;
          }
        }
        else {
          *(undefined2 *)(piVar4 + 2) = 0xffff;
          *(short *)((int)piVar4 + 10) = (sVar2 - sVar12) + 1;
          piVar4[1] = piVar6[1] + (iVar14 - iVar8) * 2;
          local_1c = piVar4;
          local_10 = piVar9;
        }
      }
      else {
        local_10 = (int *)piVar9[4];
        if (local_10 == (int *)0x0) {
          local_10 = piVar9 + (piVar9[3] & 0xffffU) * 4;
          *(short *)(piVar9 + 3) = (short)piVar9[3] + 1;
        }
        else {
          piVar9[4] = *local_10;
        }
        *(short *)(local_10 + 2) = sVar2 + 1;
        *(short *)((int)local_10 + 10) = sVar10;
        *(undefined2 *)((int)local_10 + 0xe) = *(undefined2 *)((int)piVar6 + 0xe);
        *(short *)(local_10 + 3) = (short)piVar6[3];
        *piVar9 = *piVar4;
        *local_10 = *piVar6;
        *piVar6 = (int)piVar4;
        *piVar4 = (int)local_10;
        *(short *)((int)piVar6 + 10) = sVar12 + -1;
        if (piVar6[1] < 2) {
          if (piVar6[1] == 0) {
            *(undefined2 *)(piVar4 + 2) = 0xffff;
            *(short *)((int)piVar4 + 10) = (sVar2 - sVar12) + 1;
            piVar4[1] = local_18 + iVar14 * 2;
            local_10[1] = 0;
            local_1c = piVar4;
            local_10 = piVar9;
          }
          else {
            local_10[1] = 1;
            piVar4[1] = 1;
            *(undefined2 *)((int)piVar4 + 0xe) =
                 *(undefined2 *)
                  (((*(ushort *)((int)piVar6 + 0xe) & 0x71c |
                    (*(ushort *)((int)piVar6 + 0xe) & 0xe000) >> 8) >> 1) +
                   *(int *)(param_1 + 0x66c) + (uint)*(ushort *)((int)piVar4 + 0xe) * 0x100);
            local_1c = piVar4;
            local_10 = piVar9;
          }
        }
        else {
          *(undefined2 *)(piVar4 + 2) = 0xffff;
          *(short *)((int)piVar4 + 10) = (sVar2 - sVar12) + 1;
          piVar4[1] = piVar6[1] + (iVar14 - iVar8) * 2;
          local_10[1] = piVar6[1] + 2 + (iVar13 - iVar8) * 2;
          local_1c = piVar4;
          local_10 = piVar9;
        }
      }
      iVar13 = *local_10;
    }
    local_8 = local_8 + 500;
    piVar9 = piVar9 + 200;
    local_18 = local_18 + 0x500;
    local_4 = local_4 + -1;
    if (local_4 == 0) {
      return;
    }
  } while( true );
}

