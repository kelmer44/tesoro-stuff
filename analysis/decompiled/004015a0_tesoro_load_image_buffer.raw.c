/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004015a0
 */


int * tesoro_load_image_buffer(int *param_1,int param_2)

{
  ushort *puVar1;
  uint *puVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  uint uVar6;
  int iVar7;
  
  piVar5 = param_1;
  FUN_0041b4c0((char *)&param_1,4,1,param_1);
  piVar4 = (int *)FUN_0040dc10(DAT_00442100,(int)param_1,4,param_2);
  if (piVar4 == (int *)0x0) {
    return (int *)0x0;
  }
  FUN_0041b4c0((char *)piVar4,(uint)param_1,1,piVar5);
  if (*(char *)(DAT_00442108 + 0x67c) != '\0') {
    puVar1 = (ushort *)((int)piVar4 + *piVar4);
    for (iVar7 = (int)param_1 - *piVar4; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar1 = *puVar1 >> 1 & 0x7fe0 | *puVar1 & 0x1f;
      puVar1 = puVar1 + 1;
    }
  }
  *piVar4 = *piVar4 + (int)piVar4;
  iVar7 = 0;
  if (*(short *)((int)piVar4 + 10) != 0) {
    piVar5 = piVar4 + 4;
    do {
      iVar7 = iVar7 + 1;
      *piVar5 = *piVar5 + (int)piVar4;
      piVar5 = piVar5 + 2;
    } while (iVar7 < (int)(uint)*(ushort *)((int)piVar4 + 10));
  }
  piVar5 = piVar4 + 4;
  if ((short)piVar4[3] == 1) {
    iVar7 = 0;
    if (*(short *)((int)piVar4 + 10) != 0) {
      do {
        piVar3 = (int *)*piVar5;
        for (uVar6 = (uint)*(ushort *)(piVar5 + 1); uVar6 != 0; uVar6 = uVar6 - 1) {
          *piVar3 = *piVar3 + (int)piVar4;
          piVar3 = piVar3 + 2;
        }
        piVar5 = piVar5 + 2;
        iVar7 = iVar7 + 1;
      } while (iVar7 < (int)(uint)*(ushort *)((int)piVar4 + 10));
      return piVar4;
    }
  }
  else if ((short)piVar4[3] == 2) {
    iVar7 = 0;
    if (*(short *)((int)piVar4 + 10) != 0) {
      do {
        puVar2 = (uint *)*piVar5;
        for (uVar6 = (uint)*(ushort *)(piVar5 + 1); uVar6 != 0; uVar6 = uVar6 - 1) {
          *puVar2 = (*puVar2 & 0x1f) << 2;
          puVar2 = puVar2 + 2;
        }
        piVar5 = piVar5 + 2;
        iVar7 = iVar7 + 1;
      } while (iVar7 < (int)(uint)*(ushort *)((int)piVar4 + 10));
      return piVar4;
    }
  }
  else if ((*(char *)(DAT_00442108 + 0x67c) != '\0') &&
          (iVar7 = 0, *(short *)((int)piVar4 + 10) != 0)) {
    do {
      puVar2 = (uint *)*piVar5;
      for (uVar6 = (uint)*(ushort *)(piVar5 + 1); uVar6 != 0; uVar6 = uVar6 - 1) {
        *puVar2 = (ushort)((ushort)*puVar2 >> 1) & 0x7fe0 | (ushort)*puVar2 & 0x1f;
        puVar2 = puVar2 + 2;
      }
      piVar5 = piVar5 + 2;
      iVar7 = iVar7 + 1;
    } while (iVar7 < (int)(uint)*(ushort *)((int)piVar4 + 10));
  }
  return piVar4;
}

