/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00405630
 */


void __fastcall tesoro_link_script_ops(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  ulonglong uVar7;
  int local_1c;
  int local_14;
  
  local_1c = -1;
  local_14 = 0;
  if (0 < *(int *)(param_1 + 0xc0)) {
    iVar6 = 0;
    do {
      iVar2 = *(int *)(param_1 + 0xbc);
      switch(*(undefined4 *)(iVar6 + iVar2)) {
      case 4:
        puVar1 = (undefined4 *)(iVar6 + 4 + iVar2);
        iVar3 = 0;
        if (*(int *)(param_1 + 200) < 1) {
          *puVar1 = 0;
        }
        else {
          piVar5 = *(int **)(param_1 + 0xc4);
          do {
            if ((*piVar5 == *(int *)(iVar6 + 4 + iVar2)) && (piVar5[1] == puVar1[1])) {
              *puVar1 = *(undefined4 *)(*(int *)(param_1 + 0xc4) + 8 + iVar3 * 0xc);
              goto switchD_0040567a_default;
            }
            iVar3 = iVar3 + 1;
            piVar5 = piVar5 + 3;
          } while (iVar3 < *(int *)(param_1 + 200));
          *puVar1 = 0;
        }
        break;
      case 5:
        local_1c = -1;
        break;
      case 6:
        if (0 < local_1c) {
          *(short *)(iVar2 + 4 + local_1c * 0x18) = (short)local_14;
        }
        local_1c = local_14;
        if (*(short *)(iVar6 + 4 + *(int *)(param_1 + 0xbc)) == 0) {
          local_1c = -1;
        }
        *(undefined2 *)(iVar6 + 4 + *(int *)(param_1 + 0xbc)) = 0xffff;
        break;
      case 7:
        iVar3 = 0;
        if (0 < *(int *)(param_1 + 200)) {
          piVar5 = *(int **)(param_1 + 0xc4);
          do {
            if ((*piVar5 == *(int *)(iVar6 + 4 + iVar2)) &&
               (piVar5[1] == *(int *)(iVar6 + 8 + iVar2))) {
              uVar4 = *(undefined4 *)(*(int *)(param_1 + 0xc4) + 8 + iVar3 * 0xc);
              goto LAB_004056e9;
            }
            iVar3 = iVar3 + 1;
            piVar5 = piVar5 + 3;
          } while (iVar3 < *(int *)(param_1 + 200));
        }
        uVar4 = 0;
LAB_004056e9:
        *(undefined4 *)(iVar6 + 4 + iVar2) = uVar4;
        *(undefined1 *)(iVar6 + 9 + iVar2) = *(undefined1 *)(iVar6 + 0xd + iVar2);
        *(undefined1 *)(iVar6 + 8 + iVar2) = *(undefined1 *)(iVar6 + 0xc + iVar2);
        break;
      case 8:
        uVar7 = tesoro_encode_name(0x42d1f0);
        if ((*(int *)(iVar6 + 4 + iVar2) == (int)uVar7) &&
           (uVar7 = tesoro_encode_name(0x42d1f0),
           *(int *)(iVar6 + 8 + iVar2) == (int)(uVar7 >> 0x20))) {
          *(undefined4 *)(iVar6 + 4 + iVar2) = 0;
        }
        else {
          iVar3 = 0;
          if (0 < *(int *)(DAT_004420b8 + 0x2ac)) {
            piVar5 = (int *)(DAT_004420b8 + 0x54);
            do {
              if ((*(int *)(*piVar5 + 0x44) == *(int *)(iVar6 + 4 + iVar2)) &&
                 (*(int *)(*piVar5 + 0x48) == *(int *)(iVar6 + 8 + iVar2))) {
                uVar4 = *(undefined4 *)(DAT_004420b8 + 0x54 + iVar3 * 4);
                goto LAB_004057f3;
              }
              iVar3 = iVar3 + 1;
              piVar5 = piVar5 + 1;
            } while (iVar3 < *(int *)(DAT_004420b8 + 0x2ac));
          }
          uVar4 = 0;
LAB_004057f3:
          *(undefined4 *)(iVar6 + 4 + iVar2) = uVar4;
        }
        *(undefined4 *)(iVar6 + 8 + iVar2) = *(undefined4 *)(iVar6 + 0xc + iVar2);
        *(undefined4 *)(iVar6 + 0xc + iVar2) = *(undefined4 *)(iVar6 + 0x10 + iVar2);
      }
switchD_0040567a_default:
      local_14 = local_14 + 1;
      iVar6 = iVar6 + 0x18;
    } while (local_14 < *(int *)(param_1 + 0xc0));
  }
  return;
}

