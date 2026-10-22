/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00411c80
 */


void __thiscall
tesoro_layout_text(void *this,byte *param_1,uint param_2,int param_3,int param_4,int param_5,
                  int param_6,undefined2 param_7,char param_8,int param_9)

{
  bool bVar1;
  byte bVar2;
  int iVar3;
  byte *pbVar4;
  byte *pbVar5;
  ushort *puVar7;
  int iVar8;
  int iVar9;
  byte *pbVar10;
  int iVar11;
  int local_14;
  byte *local_10;
  uint uVar6;
  
  iVar9 = *(int *)((int)this + param_3 * 4 + 0x20);
  iVar11 = *(short *)(*(int *)(iVar9 + 0x1c) + 0xd48) + 2;
  local_14 = 0;
  pbVar10 = (byte *)(*(int *)((int)this + 0x13a0) * 0x70 + 0x680 + (int)this);
  if (*(char *)((int)this + 0x67c) != '\0') {
    param_2._0_2_ = (ushort)param_2 & 0x1f | (ushort)(param_2 >> 1) & 0x7fe0;
  }
  if (param_9 == 0) {
    param_9 = (*(short *)(*(int *)(iVar9 + 0x1c) + 0xd4a) * 3) / 2;
  }
  iVar3 = (0x140 - iVar11) * 2;
  if (param_6 != iVar3 && SBORROW4(param_6,iVar3) == param_6 + (0x140 - iVar11) * -2 < 0) {
    param_4 = 0x140;
    param_6 = iVar3;
  }
  *(undefined2 *)((int)this + 0x668) = param_7;
  iVar3 = 0;
  bVar1 = true;
  bVar2 = *param_1;
  pbVar5 = pbVar10;
  local_10 = pbVar10;
  while (bVar2 != 0) {
    iVar8 = *param_1 - 0x21;
    if ((iVar8 < 0) || (pbVar10 = local_10, (int)(uint)*(ushort *)(iVar9 + 0x20) <= iVar8)) {
      *pbVar5 = 0x7f;
      iVar3 = iVar3 + iVar11;
      bVar1 = false;
    }
    else {
      *pbVar5 = *param_1;
      iVar3 = iVar3 + 2 + (int)*(short *)(*(int *)(iVar9 + 0x1c) + 0x10 + iVar8 * 0x24);
    }
    pbVar4 = pbVar5 + 1;
    _param_7 = param_1 + 1;
    if ((param_6 < iVar3) || (*_param_7 == 0)) {
      if ((bVar1) || (iVar3 <= param_6)) {
        *pbVar4 = 0;
      }
      else {
        bVar2 = *param_1;
        while (bVar2 != 0x20) {
          pbVar4 = param_1 + -1;
          param_1 = param_1 + -1;
          pbVar5 = pbVar5 + -1;
          bVar2 = *pbVar4;
        }
        _param_7 = param_1 + 1;
        *pbVar5 = 0;
      }
      iVar3 = param_6;
      if (param_8 != '\0') {
        bVar2 = *pbVar10;
        iVar3 = 0;
        if (bVar2 != 0) {
          pbVar5 = pbVar10;
          do {
            uVar6 = (uint)bVar2;
            pbVar5 = pbVar5 + 1;
            bVar2 = *pbVar5;
            iVar3 = iVar3 + 2 + (int)*(short *)(*(int *)(iVar9 + 0x1c) + -0x494 + uVar6 * 0x24);
          } while (bVar2 != 0);
        }
      }
      iVar8 = param_4 - iVar3 / 2;
      if (iVar8 < iVar11) {
        *(int *)(pbVar10 + 100) = iVar11;
      }
      else if (0x280 - iVar11 < param_4 + iVar3 / 2) {
        *(int *)(pbVar10 + 100) = (0x280 - iVar11) - iVar3;
      }
      else {
        *(int *)(pbVar10 + 100) = iVar8;
      }
      pbVar4 = pbVar10 + 0x70;
      iVar3 = 0;
      local_14 = local_14 + 1;
      bVar1 = true;
      pbVar10 = pbVar4;
      local_10 = pbVar4;
    }
    pbVar5 = pbVar4;
    param_1 = _param_7;
    bVar2 = *_param_7;
  }
  if (param_8 != '\0') {
    param_5 = param_5 - (local_14 * param_9) / 2;
  }
  iVar9 = param_9;
  if ((param_9 <= param_5) && (iVar9 = param_5, 0x1e0 - param_9 < local_14 * param_9 + param_5)) {
    iVar9 = 0x1e0 - (local_14 + 1) * param_9;
  }
  if (local_14 != 0) {
    puVar7 = (ushort *)((int)this + *(int *)((int)this + 0x13a0) * 0x70 + 0x6ec);
    iVar11 = local_14;
    do {
      *(int *)(puVar7 + -2) = iVar9;
      *puVar7 = (ushort)param_2;
      puVar7[1] = (ushort)param_3;
      puVar7 = puVar7 + 0x38;
      iVar9 = iVar9 + param_9;
      iVar11 = iVar11 + -1;
    } while (iVar11 != 0);
  }
  *(int *)((int)this + 0x13a0) = *(int *)((int)this + 0x13a0) + local_14;
  return;
}

