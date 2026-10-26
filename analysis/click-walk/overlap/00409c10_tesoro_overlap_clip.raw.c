/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00409c10
 */


void __cdecl
tesoro_overlap_clip(ushort *param_1,int param_2,undefined4 param_3,int param_4,undefined4 param_5,
                   undefined2 *param_6,int param_7,undefined4 param_8,int param_9)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  undefined4 uVar5;
  short *psVar6;
  short *psVar7;
  int iVar8;
  undefined4 *puVar9;
  
  psVar6 = *(short **)(param_1 + 2);
  iVar8 = 0;
  puVar9 = *(undefined4 **)(param_6 + 2);
  if (param_2 < param_7) {
    sVar1 = psVar6[2];
    psVar7 = psVar6;
    while (sVar1 < param_7) {
      iVar8 = iVar8 + 1;
      sVar1 = psVar7[4];
      psVar7 = psVar7 + 2;
    }
    sVar1 = psVar6[iVar8 * 2];
    sVar2 = psVar6[iVar8 * 2 + 1];
    psVar6 = psVar6 + iVar8 * 2;
    sVar3 = psVar6[3];
    sVar4 = psVar6[2];
    *(short *)puVar9 = (short)param_7;
    *(short *)((int)puVar9 + 2) =
         (short)(((param_7 - sVar1) * ((int)sVar3 - (int)sVar2)) / ((int)sVar4 - (int)sVar1)) +
         sVar2;
    puVar9 = puVar9 + 1;
  }
  if (param_9 < param_4) {
    sVar1 = psVar6[2];
    while (sVar1 < param_9) {
      *puVar9 = *(undefined4 *)psVar6;
      puVar9 = puVar9 + 1;
      sVar1 = psVar6[4];
      psVar6 = psVar6 + 2;
    }
    *puVar9 = *(undefined4 *)psVar6;
    param_6[1] = (short)((uint)((int)puVar9 + (4 - *(int *)(param_6 + 2))) >> 2);
    sVar1 = psVar6[1];
    sVar2 = *psVar6;
    psVar7 = psVar6 + 2;
    sVar3 = psVar6[3];
    sVar4 = *psVar7;
    *(short *)(puVar9 + 1) = (short)param_9;
    *(short *)((int)puVar9 + 6) =
         (short)(((param_9 - sVar2) * ((int)sVar3 - (int)sVar1)) / ((int)sVar4 - (int)sVar2)) +
         sVar1;
    sVar1 = psVar6[4];
    while (param_9 < sVar1) {
      psVar6 = psVar7 + 4;
      psVar7 = psVar7 + 2;
      sVar1 = *psVar6;
    }
    sVar1 = *psVar7;
    sVar2 = psVar7[1];
    psVar6 = psVar7 + 2;
    sVar3 = psVar7[3];
    sVar4 = *psVar6;
    *(short *)(puVar9 + 2) = (short)param_9;
    *(short *)((int)puVar9 + 10) =
         (short)(((param_9 - sVar1) * ((int)sVar3 - (int)sVar2)) / ((int)sVar4 - (int)sVar1)) +
         sVar2;
    puVar9 = puVar9 + 3;
  }
  else {
    for (iVar8 = (uint)param_1[1] - iVar8; iVar8 != 0; iVar8 = iVar8 + -1) {
      uVar5 = *(undefined4 *)psVar6;
      psVar6 = psVar6 + 2;
      *puVar9 = uVar5;
      puVar9 = puVar9 + 1;
    }
    param_6[1] = (short)((uint)((int)puVar9 + (-4 - *(int *)(param_6 + 2))) >> 2);
  }
  if (param_7 <= param_2) {
    for (iVar8 = (uint)*param_1 - ((uint)((int)psVar6 - *(int *)(param_1 + 2)) >> 2); iVar8 != 0;
        iVar8 = iVar8 + -1) {
      uVar5 = *(undefined4 *)psVar6;
      psVar6 = psVar6 + 2;
      *puVar9 = uVar5;
      puVar9 = puVar9 + 1;
    }
    *param_6 = (short)((uint)((int)puVar9 - *(int *)(param_6 + 2)) >> 2);
    return;
  }
  sVar1 = psVar6[2];
  while (param_7 < sVar1) {
    *puVar9 = *(undefined4 *)psVar6;
    puVar9 = puVar9 + 1;
    sVar1 = psVar6[4];
    psVar6 = psVar6 + 2;
  }
  *puVar9 = *(undefined4 *)psVar6;
  sVar1 = psVar6[1];
  sVar2 = *psVar6;
  sVar3 = psVar6[3];
  sVar4 = psVar6[2];
  *(short *)(puVar9 + 1) = (short)param_7;
  *(short *)((int)puVar9 + 6) =
       (short)(((param_7 - sVar2) * ((int)sVar3 - (int)sVar1)) / ((int)sVar4 - (int)sVar2)) + sVar1;
  *param_6 = (short)((uint)((int)puVar9 + (8 - *(int *)(param_6 + 2))) >> 2);
  return;
}

