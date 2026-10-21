/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040fe40
 */


int __fastcall tesoro_object_list_empty(int *param_1)

{
  int iVar1;
  int iVar2;
  uint3 uVar3;
  
  iVar1 = *param_1;
  *(undefined1 *)(iVar1 + 0x18) = 1;
  *(int *)(iVar1 + 0x10) = iVar1 + 4;
  iVar1 = *param_1;
  iVar2 = **(int **)(iVar1 + 0x10);
  uVar3 = (uint3)((uint)iVar1 >> 8);
  if (iVar2 != 0) {
    if ((*(int **)(iVar1 + 0x10) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x18) != '\0')) {
      *(undefined1 *)(iVar1 + 0x18) = 0;
    }
    else {
      *(int *)(iVar1 + 0x10) = iVar2;
    }
    iVar2 = **(int **)(iVar1 + 0x10);
    if (iVar2 != 0) {
      *(undefined4 *)(iVar1 + 0x1c) = *(undefined4 *)(iVar2 + 4);
      *(undefined4 *)(iVar1 + 0x20) = *(undefined4 *)(iVar2 + 8);
      return (uint)uVar3 << 8;
    }
  }
  return CONCAT31(uVar3,1);
}

