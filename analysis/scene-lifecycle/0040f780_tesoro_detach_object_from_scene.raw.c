/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f780
 */


uint __thiscall tesoro_detach_object_from_scene(void *this,int param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  bool bVar5;
  
                    /* WARNING: Load size is inaccurate */
  iVar1 = *this;
  *(int *)(iVar1 + 0xc) = iVar1 + 4;
  *(undefined1 *)(iVar1 + 0x14) = 1;
  do {
                    /* WARNING: Load size is inaccurate */
    uVar2 = *this;
    iVar1 = **(int **)(uVar2 + 0xc);
    if (iVar1 == 0) {
LAB_0040f7c8:
      bVar5 = false;
    }
    else {
      if ((*(int **)(uVar2 + 0xc) == (int *)(uVar2 + 4)) && (*(char *)(uVar2 + 0x14) != '\0')) {
        *(undefined1 *)(uVar2 + 0x14) = 0;
      }
      else {
        *(int *)(uVar2 + 0xc) = iVar1;
      }
      if (**(int **)(uVar2 + 0xc) == 0) goto LAB_0040f7c8;
      *(undefined4 *)(uVar2 + 0x18) = *(undefined4 *)(**(int **)(uVar2 + 0xc) + 4);
      bVar5 = true;
    }
    if (!bVar5) {
      return uVar2 & 0xffffff00;
    }
                    /* WARNING: Load size is inaccurate */
    puVar3 = *this;
    if (puVar3[6] == param_1) {
      puVar4 = *(undefined4 **)puVar3[3];
      *(undefined4 *)puVar3[3] = *puVar4;
      *puVar4 = *(undefined4 *)*puVar3;
      puVar3 = (undefined4 *)*puVar3;
      *puVar3 = puVar4;
      return CONCAT31((int3)((uint)puVar3 >> 8),1);
    }
  } while( true );
}

