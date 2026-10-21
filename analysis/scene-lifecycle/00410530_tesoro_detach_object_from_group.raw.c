/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410530
 */


void __thiscall tesoro_detach_object_from_group(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  iVar1 = *(int *)((int)this + 0xc);
  *(int *)(iVar1 + 0xc) = iVar1 + 4;
  *(undefined1 *)(iVar1 + 0x14) = 1;
  while( true ) {
    iVar1 = *(int *)((int)this + 0xc);
    iVar2 = **(int **)(iVar1 + 0xc);
    if (iVar2 == 0) {
      return;
    }
    if ((*(int **)(iVar1 + 0xc) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x14) != '\0')) {
      *(undefined1 *)(iVar1 + 0x14) = 0;
    }
    else {
      *(int *)(iVar1 + 0xc) = iVar2;
    }
    if (**(int **)(iVar1 + 0xc) == 0) break;
    *(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(**(int **)(iVar1 + 0xc) + 4);
    puVar3 = *(undefined4 **)((int)this + 0xc);
    if (puVar3[6] == param_1) {
      puVar4 = *(undefined4 **)puVar3[3];
      *(undefined4 *)puVar3[3] = *puVar4;
      *puVar4 = *(undefined4 *)*puVar3;
      *(undefined4 **)*puVar3 = puVar4;
      return;
    }
  }
  return;
}

