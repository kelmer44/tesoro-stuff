/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408440
 */


void __thiscall tesoro_scene_object_moved(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  
  if ((*(uint *)(param_1 + 0x68) >> 3 & 1) != 0) {
    iVar1 = *(int *)((int)this + 0x7c);
    *(undefined1 *)(iVar1 + 0x14) = 1;
    *(int *)(iVar1 + 0xc) = iVar1 + 4;
    while( true ) {
      iVar1 = *(int *)((int)this + 0x7c);
      iVar2 = **(int **)(iVar1 + 0xc);
      if (iVar2 == 0) break;
      if ((*(int **)(iVar1 + 0xc) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x14) != '\0')) {
        *(undefined1 *)(iVar1 + 0x14) = 0;
      }
      else {
        *(int *)(iVar1 + 0xc) = iVar2;
      }
      if (**(int **)(iVar1 + 0xc) == 0) break;
      *(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(**(int **)(iVar1 + 0xc) + 4);
      if (*(int *)(*(int *)((int)this + 0x7c) + 0x18) == param_1) {
        return;
      }
    }
    piVar3 = *(int **)((int)this + 0x7c);
    *(undefined1 *)(piVar3 + 5) = 1;
    piVar3[3] = (int)(piVar3 + 1);
    while( true ) {
      iVar1 = *(int *)piVar3[3];
      if (iVar1 == 0) break;
      if (((int *)piVar3[3] == piVar3 + 1) && ((char)piVar3[5] != '\0')) {
        *(undefined1 *)(piVar3 + 5) = 0;
      }
      else {
        piVar3[3] = iVar1;
      }
      if (*(int *)piVar3[3] == 0) break;
      piVar3[6] = *(int *)(*(int *)piVar3[3] + 4);
    }
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
  }
  return;
}

