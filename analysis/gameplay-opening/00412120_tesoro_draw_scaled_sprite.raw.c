/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00412120
 */


void __thiscall
tesoro_draw_scaled_sprite
          (void *this,int param_1,uint param_2,ushort param_3,int param_4,int param_5,uint param_6,
          int param_7,undefined4 param_8)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  short sVar10;
  
  uVar6 = (uint)*(ushort *)(param_4 + 8);
  iVar1 = param_4 + 0x10;
  iVar7 = ((int)((uVar6 - 1) * param_5) >> 0x10) + param_1;
  uVar9 = ((int)((*(ushort *)(param_4 + 10) - 1) * param_6) >> 0x10) + param_2;
  iVar3 = uVar9 - param_2;
  if (iVar3 < 0) {
    iVar3 = -iVar3;
    param_6 = -(int)(0xffffffff / (ulonglong)-param_6);
    iVar2 = (param_7 << 0x10) / iVar3;
    iVar4 = iVar3 * iVar2 + param_1 * 0x10000;
    param_7 = -iVar2;
    param_2 = uVar9;
    if ((int)uVar9 < 0) {
      iVar4 = iVar4 - uVar9 * param_7;
      param_2 = 0;
    }
    iVar2 = -(((uVar9 + iVar3) - param_2) * param_6);
    uVar9 = uVar9 + iVar3;
  }
  else {
    param_6 = (uint)(0xffffffff / (ulonglong)param_6);
    iVar4 = param_1 * 0x10000;
    param_7 = (param_7 << 0x10) / iVar3;
    if ((int)param_2 < 0) {
      iVar2 = -(param_6 * param_2);
      iVar4 = iVar4 - param_2 * param_7;
      param_2 = 0;
    }
    else {
      iVar2 = 0;
    }
  }
  if (0x1df < (int)uVar9) {
    uVar9 = 0x1df;
  }
  iVar3 = (uVar9 - param_2) + 1;
  iVar8 = iVar4 >> 0x10;
  if (param_7 < 1) {
    if (param_7 < 0) {
      iVar7 = ((int)(param_5 * uVar6) >> 0x10) + iVar8;
      param_1 = iVar8 + (iVar3 * param_7 >> 0x10);
    }
  }
  else {
    iVar7 = (iVar3 * param_7 >> 0x10) + iVar8 + ((int)(param_5 * uVar6) >> 0x10);
    param_1 = iVar8;
  }
  if ((((param_1 <= iVar7) && (param_1 < 0x280)) && (-1 < iVar7)) &&
     (((int)param_2 < 0x1e0 && (-1 < (int)uVar9)))) {
    sVar10 = (short)iVar3;
    if (*(short *)(param_4 + 0xc) == 1) {
      if ((param_1 < 0) || (0x27f < iVar7)) {
        cVar5 = '\x01';
      }
      else {
        cVar5 = '\0';
      }
      FUN_004108a0(this,iVar1,(undefined4 *)(*(int *)((int)this + 0x670) + param_2 * 2000),sVar10,
                   iVar4,iVar2,param_3,cVar5,param_5,param_6,param_7,param_2);
      return;
    }
    if (*(short *)(param_4 + 0xc) == 3) {
      if ((param_1 < 0) || (0x27f < iVar7)) {
        cVar5 = '\x01';
      }
      else {
        cVar5 = '\0';
      }
      FUN_00410fb0(iVar1,(undefined4 *)(*(int *)((int)this + 0x670) + param_2 * 2000),sVar10,iVar4,
                   iVar2,param_3,cVar5,param_5,param_6,param_7);
      return;
    }
    if ((param_1 < 0) || (0x27f < iVar7)) {
      cVar5 = '\x01';
    }
    else {
      cVar5 = '\0';
    }
    FUN_00411580(iVar1,(undefined4 *)(*(int *)((int)this + 0x674) + param_2 * 800),sVar10,iVar4,
                 iVar2,param_3,cVar5,param_8,param_5,param_6,param_7);
  }
  return;
}

