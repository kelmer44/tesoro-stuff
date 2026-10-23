/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040fb20
 */


uint __thiscall tesoro_remove_object_from_list(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  bool bVar5;
  int *piVar6;
  
                    /* WARNING: Load size is inaccurate */
  iVar1 = *this;
  piVar6 = (int *)(iVar1 + 4);
  *(int **)(iVar1 + 0x10) = piVar6;
  *(undefined1 *)(iVar1 + 0x18) = 1;
  do {
                    /* WARNING: Load size is inaccurate */
    iVar1 = *this;
    iVar2 = **(int **)(iVar1 + 0x10);
    if (iVar2 == 0) {
LAB_0040fb6e:
      bVar5 = false;
    }
    else {
      if ((*(int **)(iVar1 + 0x10) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x18) != '\0')) {
        *(undefined1 *)(iVar1 + 0x18) = 0;
      }
      else {
        *(int *)(iVar1 + 0x10) = iVar2;
      }
      piVar6 = *(int **)(iVar1 + 0x10);
      iVar2 = *piVar6;
      if (iVar2 == 0) goto LAB_0040fb6e;
      *(undefined4 *)(iVar1 + 0x1c) = *(undefined4 *)(iVar2 + 4);
      piVar6 = *(int **)(iVar2 + 8);
      *(int **)(iVar1 + 0x20) = piVar6;
      bVar5 = true;
    }
    if (!bVar5) {
      return (uint)piVar6 & 0xffffff00;
    }
                    /* WARNING: Load size is inaccurate */
    puVar3 = *this;
    if (puVar3[7] == param_1) {
      puVar4 = *(undefined4 **)puVar3[4];
      *(undefined4 *)puVar3[4] = *puVar4;
      *puVar4 = *(undefined4 *)*puVar3;
      *(undefined4 **)*puVar3 = puVar4;
                    /* WARNING: Load size is inaccurate */
      iVar1 = *this;
      *(int *)(iVar1 + 0x10) = iVar1 + 4;
      *(undefined1 *)(iVar1 + 0x18) = 1;
      return CONCAT31((int3)((uint)(iVar1 + 4) >> 8),1);
    }
  } while( true );
}

