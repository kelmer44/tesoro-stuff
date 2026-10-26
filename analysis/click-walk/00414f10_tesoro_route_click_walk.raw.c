/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414f10
 */


uint __thiscall tesoro_route_click_walk(void *this,int param_1,int param_2,int param_3)

{
  ushort *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  short *psVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint local_18;
  
  iVar10 = param_1;
  local_18 = 0xffffffff;
  param_1 = 0;
  iVar7 = iVar10 - DAT_004420fc[0x13];
  iVar8 = iVar7 * iVar7 + (param_2 - DAT_004420fc[0x14]) * (param_2 - DAT_004420fc[0x14]);
  puVar1 = (ushort *)
           (*(int *)(*(int *)((int)this + 0x38) + 0x1c) + 0x1c +
           (uint)*(ushort *)(*(int *)((int)this + 0x38) + 0x30) * 0x24);
  iVar7 = *(int *)((int)this + 0x50);
  iVar2 = *(int *)((int)this + 0x4c);
  iVar10 = iVar10 - iVar2;
  if (*puVar1 != 0) {
    psVar5 = *(short **)(puVar1 + 2);
    do {
      iVar3 = iVar10 - *psVar5;
      iVar6 = (param_2 - iVar7) - (int)psVar5[1];
      iVar3 = iVar3 * iVar3 + iVar6 * iVar6;
      if (iVar3 < iVar8) {
        local_18 = param_1;
        iVar8 = iVar3;
      }
      param_1 = param_1 + 1;
      psVar5 = psVar5 + 2;
    } while (param_1 < (int)(uint)*puVar1);
  }
  if (-1 < (int)local_18) {
    *(int *)((int)this + 0xa8) = iVar10 + iVar2;
    *(int *)((int)this + 0xb0) = param_3;
    *(int *)((int)this + 0xac) = (param_2 - iVar7) + iVar7;
    *(uint *)((int)this + 0xa4) = local_18;
    param_3 = 0xfffffff;
    param_1 = 0;
    local_18 = -1;
    uVar9 = -1;
    if (*puVar1 != 0) {
      psVar5 = *(short **)(puVar1 + 2);
      do {
        iVar10 = (DAT_004420fc[0x13] - iVar2) - (int)*psVar5;
        iVar8 = (DAT_004420fc[0x14] - iVar7) - (int)psVar5[1];
        iVar10 = iVar10 * iVar10 + iVar8 * iVar8;
        if (iVar10 < param_3) {
          local_18 = param_1;
          param_3 = iVar10;
        }
        param_1 = param_1 + 1;
        psVar5 = psVar5 + 2;
        uVar9 = local_18;
      } while (param_1 < (int)(uint)*puVar1);
    }
    *(uint *)((int)this + 0xa0) = uVar9;
    psVar5 = (short *)(*(int *)(puVar1 + 2) + uVar9 * 4);
    uVar4 = (**(code **)(*DAT_004420fc + 0x80))(*psVar5 + iVar2,psVar5[1] + iVar7,this,1);
    return CONCAT31((int3)((uint)uVar4 >> 8),1);
  }
  return local_18 & 0xffffff00;
}

