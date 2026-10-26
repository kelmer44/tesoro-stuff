/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004025b0
 */


void tesoro_cursor_4025b0(void)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  ulonglong uVar5;
  
  iVar3 = DAT_004420fc[0x10];
  if (((iVar3 != DAT_00442168) && (iVar3 != DAT_004421f4)) && (iVar3 != DAT_004421f0)) {
    if (*(int *)(DAT_00442128 + 300) != 0) {
      puVar1 = (uint *)(*(int *)(DAT_00442128 + 300) + 0x68);
      *puVar1 = *puVar1 & 0xfffffffb;
    }
    FUN_00407aa0(DAT_004420fc);
    uVar5 = tesoro_encode_name(0x42d044);
    FUN_0040f680(*(void **)(DAT_004421ec + 0x74),(int)uVar5,(int)(uVar5 >> 0x20));
    *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) =
         *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) + 0x280;
    *(undefined1 *)(DAT_00442128 + 0x78) = 0;
    *(int *)(*(int *)(DAT_004421fc + 0xa8) + 0x4c) =
         *(int *)(*(int *)(DAT_004421fc + 0xa8) + 0x4c) + 0x280;
    *(int *)(DAT_004421fc + 0x4c) = *(int *)(DAT_004421fc + 0x4c) + 0x280;
    iVar3 = *(int *)(DAT_004421fc + 0xa0);
    *(undefined1 *)(iVar3 + 0x14) = 1;
    *(int *)(iVar3 + 0xc) = iVar3 + 4;
    while( true ) {
      iVar3 = *(int *)(DAT_004421fc + 0xa0);
      iVar4 = **(int **)(iVar3 + 0xc);
      if (iVar4 == 0) break;
      if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
        *(undefined1 *)(iVar3 + 0x14) = 0;
      }
      else {
        *(int *)(iVar3 + 0xc) = iVar4;
      }
      if (**(int **)(iVar3 + 0xc) == 0) {
        return;
      }
      *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
      piVar2 = (int *)(*(int *)(*(int *)(DAT_004421fc + 0xa0) + 0x18) + 0x4c);
      *piVar2 = *piVar2 + 0x280;
    }
  }
  return;
}

