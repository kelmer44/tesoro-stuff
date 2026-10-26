/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00402e80
 */


/* WARNING: Removing unreachable block (ram,0x004030c1) */

int __fastcall tesoro_input_402e80(int param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int local_8;
  
  piVar1 = (int *)*DAT_00442060;
  piVar2 = (int *)**(undefined4 **)(param_1 + 0x6c);
  piVar1[1] = 0;
  piVar1[3] = (int)(piVar1 + 1);
  *(undefined4 *)*piVar1 = 0;
  *(int *)(*piVar1 + 8) = *piVar1 + 0x10;
  piVar2[3] = (int)(piVar2 + 1);
  *(undefined1 *)(piVar2 + 5) = 1;
  while( true ) {
    iVar3 = *(int *)piVar2[3];
    if (iVar3 == 0) break;
    if (((int *)piVar2[3] == piVar2 + 1) && ((char)piVar2[5] != '\0')) {
      *(undefined1 *)(piVar2 + 5) = 0;
    }
    else {
      piVar2[3] = iVar3;
    }
    if (*(int *)piVar2[3] == 0) break;
    piVar2[6] = *(int *)(*(int *)piVar2[3] + 4);
    piVar4 = (int *)piVar1[3];
    iVar3 = piVar2[6];
    iVar6 = *piVar4;
    if (iVar6 != 0) {
      piVar1[3] = iVar6;
    }
    piVar5 = (int *)*piVar1;
    piVar8 = (int *)*piVar5;
    if (piVar8 == (int *)0x0) {
      piVar8 = (int *)piVar5[2];
      piVar5[2] = (int)(piVar8 + 2);
    }
    else {
      *piVar5 = *piVar8;
    }
    *piVar8 = *(undefined4 *)piVar1[3];
    *(int **)piVar1[3] = piVar8;
    piVar8[1] = iVar3;
    piVar1[3] = (int)piVar4;
  }
  piVar2[3] = (int)(piVar2 + 1);
  piVar2[1] = 0;
  *(undefined4 *)*piVar2 = 0;
  *(int *)(*piVar2 + 8) = *piVar2 + 0x10;
  iVar3 = **(int **)(param_1 + 0x70);
  FUN_0040f670(DAT_00442060);
  *(undefined1 *)(iVar3 + 0x14) = 1;
  *(int *)(iVar3 + 0xc) = iVar3 + 4;
  local_8 = 0;
  while( true ) {
    iVar6 = **(int **)(iVar3 + 0xc);
    if (iVar6 == 0) break;
    if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
      *(undefined1 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(int *)(iVar3 + 0xc) = iVar6;
    }
    if ((**(int **)(iVar3 + 0xc) == 0) ||
       (*(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4), local_8 != 0))
    break;
    iVar6 = FUN_0040f680(DAT_00442060,*(int *)(*(int *)(iVar3 + 0x18) + 0x44),
                         *(int *)(*(int *)(iVar3 + 0x18) + 0x48));
    piVar2 = *(int **)(iVar3 + 0x18);
    if (iVar6 == 0) {
      iVar6 = *piVar2;
      iVar7 = (**(code **)(iVar6 + 0x40))();
      if (iVar7 != 0) {
        FUN_0040f800(*(void **)(param_1 + 0x6c),piVar2);
        local_8 = (**(code **)(iVar6 + 0x44))();
      }
    }
    else {
      if ((**(byte **)(DAT_0044206c + 0x1c) & 8) == 0) {
        if ((**(byte **)(DAT_0044206c + 0x1c) & 0x10) == 0) {
          local_8 = (**(code **)(*piVar2 + 0x44))();
        }
        else {
          local_8 = (**(code **)(*piVar2 + 0x3c))(*(undefined2 *)(*(int *)(param_1 + 0x38) + 0x30));
        }
      }
      else {
        local_8 = (**(code **)(*piVar2 + 0x38))(*(undefined2 *)(*(int *)(param_1 + 0x38) + 0x30));
      }
      if (local_8 == 3) {
        return 3;
      }
      FUN_0040f800(*(void **)(param_1 + 0x6c),piVar2);
      FUN_0040f780(DAT_00442060,(int)piVar2);
    }
  }
  *(undefined1 *)(piVar1 + 5) = 1;
  piVar1[3] = (int)(piVar1 + 1);
  while( true ) {
    iVar3 = *(int *)piVar1[3];
    if (iVar3 == 0) {
      return local_8;
    }
    if (((int *)piVar1[3] == piVar1 + 1) && ((char)piVar1[5] != '\0')) {
      *(undefined1 *)(piVar1 + 5) = 0;
    }
    else {
      piVar1[3] = iVar3;
    }
    if (*(int *)piVar1[3] == 0) break;
    piVar2 = *(int **)(*(int *)piVar1[3] + 4);
    piVar1[6] = (int)piVar2;
    (**(code **)(*piVar2 + 0x48))();
  }
  return local_8;
}

