/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f900
 */


void __thiscall tesoro_object_list_insert_priority(void *this,undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  
                    /* WARNING: Load size is inaccurate */
  iVar1 = *this;
  *(int *)(iVar1 + 0x10) = iVar1 + 4;
  *(undefined1 *)(iVar1 + 0x18) = 1;
  while( true ) {
                    /* WARNING: Load size is inaccurate */
    iVar1 = *this;
    iVar2 = **(int **)(iVar1 + 0x10);
    if (iVar2 == 0) break;
    if ((*(int **)(iVar1 + 0x10) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x18) != '\0')) {
      *(undefined1 *)(iVar1 + 0x18) = 0;
    }
    else {
      *(int *)(iVar1 + 0x10) = iVar2;
    }
    iVar2 = **(int **)(iVar1 + 0x10);
    if (iVar2 == 0) break;
    *(undefined4 *)(iVar1 + 0x1c) = *(undefined4 *)(iVar2 + 4);
    *(undefined4 *)(iVar1 + 0x20) = *(undefined4 *)(iVar2 + 8);
                    /* WARNING: Load size is inaccurate */
    piVar3 = *this;
    if (param_2 < piVar3[8]) {
      piVar4 = (int *)*piVar3;
      piVar5 = (int *)*piVar4;
      if (piVar5 == (int *)0x0) {
        piVar5 = (int *)piVar4[3];
        piVar4[3] = (int)(piVar5 + 3);
      }
      else {
        *piVar4 = *piVar5;
      }
      *piVar5 = *(undefined4 *)piVar3[4];
      *(int **)piVar3[4] = piVar5;
      piVar5[1] = param_1;
      piVar5[2] = param_2;
      return;
    }
  }
                    /* WARNING: Load size is inaccurate */
  piVar3 = *this;
  *(undefined1 *)(piVar3 + 6) = 1;
  piVar3[4] = (int)(piVar3 + 1);
  while( true ) {
    iVar1 = *(int *)piVar3[4];
    if (iVar1 == 0) break;
    if (((int *)piVar3[4] == piVar3 + 1) && ((char)piVar3[6] != '\0')) {
      *(undefined1 *)(piVar3 + 6) = 0;
    }
    else {
      piVar3[4] = iVar1;
    }
    iVar1 = *(int *)piVar3[4];
    if (iVar1 == 0) break;
    piVar3[7] = *(int *)(iVar1 + 4);
    piVar3[8] = *(int *)(iVar1 + 8);
  }
  piVar4 = (int *)*piVar3;
  piVar5 = (int *)*piVar4;
  if (piVar5 == (int *)0x0) {
    piVar5 = (int *)piVar4[3];
    piVar4[3] = (int)(piVar5 + 3);
  }
  else {
    *piVar4 = *piVar5;
  }
  *piVar5 = *(undefined4 *)piVar3[4];
  *(int **)piVar3[4] = piVar5;
  piVar5[1] = param_1;
  piVar5[2] = param_2;
  return;
}

