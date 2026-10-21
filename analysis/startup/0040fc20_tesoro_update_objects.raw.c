/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040fc20
 */


undefined4 __fastcall tesoro_update_objects(int *param_1)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  int iVar5;
  
  iVar5 = *param_1;
  *(undefined1 *)(iVar5 + 0x18) = 1;
  *(int *)(iVar5 + 0x10) = iVar5 + 4;
  do {
    iVar5 = *param_1;
    iVar2 = **(int **)(iVar5 + 0x10);
    if (iVar2 == 0) {
LAB_0040fc71:
      bVar4 = false;
    }
    else {
      if ((*(int **)(iVar5 + 0x10) == (int *)(iVar5 + 4)) && (*(char *)(iVar5 + 0x18) != '\0')) {
        *(undefined1 *)(iVar5 + 0x18) = 0;
      }
      else {
        *(int *)(iVar5 + 0x10) = iVar2;
      }
      iVar2 = **(int **)(iVar5 + 0x10);
      if (iVar2 == 0) goto LAB_0040fc71;
      *(undefined4 *)(iVar5 + 0x1c) = *(undefined4 *)(iVar2 + 4);
      *(undefined4 *)(iVar5 + 0x20) = *(undefined4 *)(iVar2 + 8);
      bVar4 = true;
    }
    if (!bVar4) {
      iVar5 = *param_1;
      do {
        *(int *)(iVar5 + 0x10) = iVar5 + 4;
        *(undefined1 *)(iVar5 + 0x18) = 1;
        do {
          do {
            iVar5 = *param_1;
            iVar2 = **(int **)(iVar5 + 0x10);
            if (iVar2 == 0) {
LAB_0040fcc5:
              bVar4 = false;
            }
            else {
              if ((*(int **)(iVar5 + 0x10) == (int *)(iVar5 + 4)) &&
                 (*(char *)(iVar5 + 0x18) != '\0')) {
                *(undefined1 *)(iVar5 + 0x18) = 0;
              }
              else {
                *(int *)(iVar5 + 0x10) = iVar2;
              }
              iVar2 = **(int **)(iVar5 + 0x10);
              if (iVar2 == 0) goto LAB_0040fcc5;
              *(undefined4 *)(iVar5 + 0x1c) = *(undefined4 *)(iVar2 + 4);
              *(undefined4 *)(iVar5 + 0x20) = *(undefined4 *)(iVar2 + 8);
              bVar4 = true;
            }
            if (!bVar4) {
              return 1;
            }
            uVar3 = *(uint *)(*(int *)(*param_1 + 0x1c) + 0x68);
          } while (((uVar3 >> 1 & 1) == 0) || ((uVar3 & 0x200) != 0));
          *(uint *)(*(int *)(*param_1 + 0x1c) + 0x68) = uVar3 | 0x200;
          iVar2 = *(int *)(*param_1 + 0x10);
          iVar5 = (**(code **)(**(int **)(*param_1 + 0x1c) + 0x10))();
          if (iVar5 == 2) {
            return 2;
          }
          if (iVar5 == 3) {
            return 3;
          }
          iVar5 = *param_1;
        } while (iVar2 == *(int *)(iVar5 + 0x10));
      } while( true );
    }
    puVar1 = (uint *)(*(int *)(*param_1 + 0x1c) + 0x68);
    *puVar1 = *puVar1 & 0xfffffdff;
  } while( true );
}

