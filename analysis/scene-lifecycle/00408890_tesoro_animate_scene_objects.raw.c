/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408890
 */


/* WARNING: Removing unreachable block (ram,0x00408a31) */

void __fastcall tesoro_animate_scene_objects(int param_1)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  char cVar7;
  int iVar8;
  
  iVar8 = FUN_00413a10(DAT_00442120);
  piVar2 = *(int **)(param_1 + 0x78);
  piVar2[1] = 0;
  piVar2[6] = (int)(piVar2 + 1);
  *(undefined4 *)*piVar2 = 0;
  *(int *)(*piVar2 + 0x14) = *piVar2 + 0x28;
  iVar3 = *(int *)(param_1 + 0x7c);
  *(undefined1 *)(iVar3 + 0x14) = 1;
  *(int *)(iVar3 + 0xc) = iVar3 + 4;
  while( true ) {
    iVar3 = *(int *)(param_1 + 0x7c);
    iVar4 = **(int **)(iVar3 + 0xc);
    if (iVar4 == 0) break;
    if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
      *(undefined1 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(int *)(iVar3 + 0xc) = iVar4;
    }
    if (**(int **)(iVar3 + 0xc) == 0) break;
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
    (**(code **)(**(int **)(*(int *)(param_1 + 0x7c) + 0x18) + 0xc))();
    puVar1 = (uint *)(*(int *)(*(int *)(param_1 + 0x7c) + 0x18) + 0x68);
    *puVar1 = *puVar1 | 1;
  }
  iVar3 = *(int *)(param_1 + 0x7c);
  *(undefined1 *)(iVar3 + 0x14) = 1;
  *(int *)(iVar3 + 0xc) = iVar3 + 4;
LAB_0040893b:
  iVar3 = *(int *)(param_1 + 0x7c);
  iVar4 = **(int **)(iVar3 + 0xc);
  if (iVar4 != 0) {
    if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
      *(undefined1 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(int *)(iVar3 + 0xc) = iVar4;
    }
    if (**(int **)(iVar3 + 0xc) == 0) goto LAB_004089dc;
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
    iVar3 = *(int *)(*(int *)(*(int *)(*(int *)(param_1 + 0x7c) + 0x18) + 0x3c) + 0xc);
    *(undefined1 *)(iVar3 + 0x14) = 1;
    *(int **)(iVar3 + 0xc) = (int *)(iVar3 + 4);
    while( true ) {
      iVar4 = **(int **)(iVar3 + 0xc);
      if (iVar4 == 0) break;
      if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
        *(undefined1 *)(iVar3 + 0x14) = 0;
      }
      else {
        *(int *)(iVar3 + 0xc) = iVar4;
      }
      if (**(int **)(iVar3 + 0xc) == 0) break;
      *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
      uVar5 = (*(int **)(iVar3 + 0x18))[0x1a];
      if ((((uVar5 & 8) != 0) && ((uVar5 & 2) != 0)) && ((uVar5 & 1) == 0)) {
        (**(code **)(**(int **)(iVar3 + 0x18) + 0xc))();
      }
    }
    goto LAB_0040893b;
  }
LAB_004089dc:
  iVar3 = *(int *)(param_1 + 0x7c);
  *(undefined1 *)(iVar3 + 0x14) = 1;
  *(int *)(iVar3 + 0xc) = iVar3 + 4;
  while( true ) {
    iVar3 = *(int *)(param_1 + 0x7c);
    iVar4 = **(int **)(iVar3 + 0xc);
    if (iVar4 == 0) break;
    if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
      *(undefined1 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(int *)(iVar3 + 0xc) = iVar4;
    }
    if (**(int **)(iVar3 + 0xc) == 0) break;
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
    puVar1 = (uint *)(*(int *)(*(int *)(param_1 + 0x7c) + 0x18) + 0x68);
    *puVar1 = *puVar1 & 0xfffffffe;
  }
  iVar3 = *(int *)(param_1 + 0x7c);
  *(undefined1 *)(iVar3 + 0x14) = 1;
  *(int *)(iVar3 + 0xc) = iVar3 + 4;
LAB_00408a49:
  iVar3 = *(int *)(param_1 + 0x7c);
  iVar4 = **(int **)(iVar3 + 0xc);
  if (iVar4 != 0) {
    if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
      *(undefined1 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(int *)(iVar3 + 0xc) = iVar4;
    }
    if (**(int **)(iVar3 + 0xc) == 0) goto LAB_00408b2f;
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
    piVar2 = *(int **)(*(int *)(param_1 + 0x7c) + 0x18);
    if ((*(byte *)(piVar2 + 0x1a) & 8) != 0) {
      iVar3 = *(int *)(piVar2[0xf] + 0xc);
      *(undefined1 *)(iVar3 + 0x14) = 1;
      *(int **)(iVar3 + 0xc) = (int *)(iVar3 + 4);
      while( true ) {
        iVar4 = **(int **)(iVar3 + 0xc);
        if (iVar4 == 0) break;
        if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
          *(undefined1 *)(iVar3 + 0x14) = 0;
        }
        else {
          *(int *)(iVar3 + 0xc) = iVar4;
        }
        if (**(int **)(iVar3 + 0xc) == 0) break;
        *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
        piVar6 = *(int **)(iVar3 + 0x18);
        if (((piVar2 != piVar6) && ((piVar6[0x1a] & 8U) != 0)) && ((piVar6[0x1a] & 2U) != 0)) {
          cVar7 = (**(code **)(*piVar2 + 0x2c))(piVar6);
          if (cVar7 == '\0') {
            cVar7 = (**(code **)(*piVar6 + 0x2c))(piVar2);
            if (cVar7 != '\0') {
              FUN_00408c50((int)piVar6,(int)piVar2);
            }
          }
          else {
            FUN_00408c50((int)piVar2,(int)piVar6);
          }
        }
      }
    }
    goto LAB_00408a49;
  }
LAB_00408b2f:
  iVar3 = *(int *)(param_1 + 0x78);
  *(undefined1 *)(iVar3 + 0x20) = 1;
  *(int *)(iVar3 + 0x18) = iVar3 + 4;
  while( true ) {
    iVar3 = *(int *)(param_1 + 0x78);
    iVar4 = **(int **)(iVar3 + 0x18);
    if (iVar4 == 0) break;
    if ((*(int **)(iVar3 + 0x18) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x20) != '\0')) {
      *(undefined1 *)(iVar3 + 0x20) = 0;
    }
    else {
      *(int *)(iVar3 + 0x18) = iVar4;
    }
    iVar4 = **(int **)(iVar3 + 0x18);
    if (iVar4 == 0) break;
    *(undefined4 *)(iVar3 + 0x24) = *(undefined4 *)(iVar4 + 4);
    *(undefined4 *)(iVar3 + 0x28) = *(undefined4 *)(iVar4 + 8);
    *(undefined4 *)(iVar3 + 0x2c) = *(undefined4 *)(iVar4 + 0xc);
    *(undefined4 *)(iVar3 + 0x30) = *(undefined4 *)(iVar4 + 0x10);
    iVar3 = *(int *)(param_1 + 0x78);
    (**(code **)(**(int **)(iVar3 + 0x24) + 0x30))
              (*(undefined4 *)(iVar3 + 0x28),*(undefined4 *)(iVar3 + 0x2c));
  }
  piVar2 = *(int **)(param_1 + 0x78);
  piVar2[1] = 0;
  piVar2[6] = (int)(piVar2 + 1);
  *(undefined4 *)*piVar2 = 0;
  *(int *)(*piVar2 + 0x14) = *piVar2 + 0x28;
  iVar3 = *(int *)(param_1 + 0x7c);
  *(undefined1 *)(iVar3 + 0x14) = 1;
  *(int *)(iVar3 + 0xc) = iVar3 + 4;
  while( true ) {
    iVar3 = *(int *)(param_1 + 0x7c);
    iVar4 = **(int **)(iVar3 + 0xc);
    if (iVar4 == 0) break;
    if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
      *(undefined1 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(int *)(iVar3 + 0xc) = iVar4;
    }
    if (**(int **)(iVar3 + 0xc) == 0) break;
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
    iVar3 = *(int *)(*(int *)(param_1 + 0x7c) + 0x18);
    *(undefined4 *)(iVar3 + 0x4c) = *(undefined4 *)(iVar3 + 0x54);
    *(undefined4 *)(iVar3 + 0x50) = *(undefined4 *)(iVar3 + 0x58);
  }
  piVar2 = *(int **)(param_1 + 0x7c);
  piVar2[1] = 0;
  piVar2[3] = (int)(piVar2 + 1);
  *(undefined4 *)*piVar2 = 0;
  *(int *)(*piVar2 + 8) = *piVar2 + 0x10;
  *(int *)(param_1 + 0x8c) = iVar8;
  return;
}

