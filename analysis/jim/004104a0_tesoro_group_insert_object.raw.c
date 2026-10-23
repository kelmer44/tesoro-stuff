/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004104a0
 */


void __thiscall tesoro_group_insert_object(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  
  iVar1 = *(int *)((int)this + 0xc);
  *(int *)(iVar1 + 0xc) = iVar1 + 4;
  *(undefined1 *)(iVar1 + 0x14) = 1;
  while( true ) {
    iVar1 = *(int *)((int)this + 0xc);
    iVar2 = **(int **)(iVar1 + 0xc);
    if (iVar2 == 0) break;
    if ((*(int **)(iVar1 + 0xc) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x14) != '\0')) {
      *(undefined1 *)(iVar1 + 0x14) = 0;
    }
    else {
      *(int *)(iVar1 + 0xc) = iVar2;
    }
    if ((**(int **)(iVar1 + 0xc) == 0) ||
       (*(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(**(int **)(iVar1 + 0xc) + 4),
       *(int *)(*(int *)(*(int *)((int)this + 0xc) + 0x18) + 0x4c) < *(int *)(param_1 + 0x4c)))
    break;
  }
  piVar3 = *(int **)((int)this + 0xc);
  piVar4 = (int *)*piVar3;
  piVar5 = (int *)*piVar4;
  if (piVar5 == (int *)0x0) {
    piVar5 = (int *)piVar4[2];
    piVar4[2] = (int)(piVar5 + 2);
  }
  else {
    *piVar4 = *piVar5;
  }
  *piVar5 = *(undefined4 *)piVar3[3];
  *(int **)piVar3[3] = piVar5;
  piVar5[1] = param_1;
  return;
}

