/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00406a90
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __thiscall
item_ctor(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4,uint param_5)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  int *piVar6;
  byte *pbVar7;
  ulonglong uVar8;
  int local_1c [7];
  
  FUN_0040ecb0(this,param_1,param_2,param_3,(int)param_4,param_5);
  *(undefined ***)this = &PTR_FUN_00428b00;
  *(ushort *)((int)this + 0xaa) = (ushort)param_4[1];
  _DAT_004420f8 = _DAT_004420f8 + 1;
  if (*(void **)((int)this + 0x38) != (void *)0x0) {
    FUN_00401740(*(void **)((int)this + 0x38),(ushort)*param_4,(uint)*param_4,0,'\0');
  }
  pbVar7 = param_4 + 2;
  *(undefined4 *)((int)this + 0x94) = *(undefined4 *)((int)this + 0x4c);
  *(undefined4 *)((int)this + 0x98) = *(undefined4 *)((int)this + 0x50);
  if (*pbVar7 == 0) {
    *(undefined4 *)((int)this + 0x6c) = 0;
  }
  else {
    iVar3 = FUN_00413940((int)pbVar7);
    *(int *)((int)this + 0x6c) = iVar3;
    pbVar7 = param_4 + 0xe;
  }
  local_1c[0] = 0;
  iVar3 = 6;
  local_1c[1] = 1;
  local_1c[2] = 3;
  local_1c[3] = 4;
  local_1c[4] = 5;
  local_1c[5] = 2;
  local_1c[6] = 6;
  piVar6 = local_1c;
  do {
    if (*pbVar7 == 0) {
      *(undefined4 *)((int)this + *piVar6 * 4 + 0x70) = 0;
    }
    else {
      iVar4 = FUN_00413940((int)pbVar7);
      *(int *)((int)this + *piVar6 * 4 + 0x70) = iVar4;
    }
    pbVar7 = pbVar7 + 0xc;
    piVar6 = piVar6 + 1;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  uVar2 = *(ushort *)(param_4 + 0x62);
  pbVar7 = param_4 + 0x7c;
  *(ushort *)((int)this + 0xa8) = uVar2;
  puVar5 = FUN_0040db40((uint)uVar2 << 3,1);
  *(undefined4 **)((int)this + 0xa0) = puVar5;
  puVar5 = FUN_0040db40((uint)*(ushort *)((int)this + 0xa8) << 2,1);
  iVar3 = 0;
  *(undefined4 **)((int)this + 0xa4) = puVar5;
  if (*(short *)((int)this + 0xa8) != 0) {
    do {
      uVar8 = tesoro_encode_name((int)pbVar7);
      iVar4 = *(int *)((int)this + 0xa0);
      *(int *)(iVar4 + iVar3 * 8) = (int)uVar8;
      *(int *)(iVar4 + 4 + iVar3 * 8) = (int)(uVar8 >> 0x20);
      if (pbVar7[0xc] == 0) {
        *(undefined4 *)(*(int *)((int)this + 0xa4) + iVar3 * 4) = 0;
      }
      else {
        iVar4 = FUN_00413940((int)(pbVar7 + 0xc));
        *(int *)(*(int *)((int)this + 0xa4) + iVar3 * 4) = iVar4;
      }
      pbVar7 = pbVar7 + 0x18;
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)(uint)*(ushort *)((int)this + 0xa8));
  }
  bVar1 = param_4[1];
  *(short *)((int)this + 0x5e) = *(short *)((int)this + 0x5e) + 2;
  *(byte *)((int)this + 0x5d) = bVar1;
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) & 0xfffffffd;
  return (undefined4 *)this;
}

