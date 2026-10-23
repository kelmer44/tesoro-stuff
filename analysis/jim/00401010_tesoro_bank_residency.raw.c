/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401010
 */


void tesoro_bank_residency(void)

{
  undefined4 *puVar1;
  ushort *puVar2;
  int *extraout_ECX;
  undefined2 *puVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  uint in_stack_00001044;
  
  FUN_0041b3c0();
  uVar8 = in_stack_00001044 & 0xffff;
  iVar6 = 0x20;
  iVar4 = 0;
  puVar3 = (undefined2 *)register0x00000010;
  do {
    *puVar3 = (short)((uint)iVar4 >> 8);
    puVar3 = puVar3 + 1;
    iVar6 = iVar6 + -1;
    iVar4 = iVar4 + uVar8;
  } while (iVar6 != 0);
  uVar7 = 0;
  puVar3 = (undefined2 *)&stack0x00000040;
  do {
    iVar4 = (uVar7 & 0x3f) * uVar8;
    uVar5 = iVar4 >> 8;
    if (0x3f < (ushort)((uint)iVar4 >> 8)) {
      uVar5 = 0x3f;
    }
    iVar4 = ((int)uVar7 >> 6) * uVar8;
    iVar6 = iVar4 >> 8;
    if (0x1f < (ushort)((uint)iVar4 >> 8)) {
      iVar6 = 0x1f;
    }
    *puVar3 = (short)((iVar6 << 6 | uVar5) << 5);
    uVar7 = uVar7 + 1;
    puVar3 = puVar3 + 1;
  } while ((int)uVar7 < 0x800);
  iVar4 = 0;
  if ((short)extraout_ECX[8] != 0) {
    do {
      puVar1 = *(undefined4 **)(*extraout_ECX + iVar4 * 4);
      if (*(short *)(puVar1 + 3) == 1) {
        puVar2 = (ushort *)*puVar1;
        uVar8 = (uint)puVar1[1] >> 1;
        if (*(char *)(DAT_00442108 + 0x67c) == '\0') {
          for (; uVar8 != 0; uVar8 = uVar8 - 1) {
            *puVar2 = *(ushort *)(&stack0x00000040 + (uint)(*puVar2 >> 5) * 2) |
                      *(ushort *)(&stack0x00000000 + (*puVar2 & 0x1f) * 2);
            puVar2 = puVar2 + 1;
          }
        }
        else {
          for (; uVar8 != 0; uVar8 = uVar8 - 1) {
            *puVar2 = (*(ushort *)(&stack0x00000040 + (((*puVar2 & 0x7fe0) << 1) >> 5) * 2) |
                      *(ushort *)(&stack0x00000000 + (*puVar2 & 0x1f) * 2)) & 0x1f |
                      (*(ushort *)(&stack0x00000040 + (((*puVar2 & 0x7fe0) << 1) >> 5) * 2) |
                      *(ushort *)(&stack0x00000000 + (*puVar2 & 0x1f) * 2)) >> 1 & 0x7fe0;
            puVar2 = puVar2 + 1;
          }
        }
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < (int)(uint)*(ushort *)(extraout_ECX + 8));
  }
  return;
}

