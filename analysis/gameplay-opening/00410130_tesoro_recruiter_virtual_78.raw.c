/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410130
 */


void __thiscall tesoro_recruiter_virtual_78(void *this,int param_1)

{
  short *psVar1;
  byte bVar2;
  short sVar3;
  int iVar4;
  int *piVar5;
  void *this_00;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar8 = *(int *)((int)this + 0x4c) + 0x140;
  iVar7 = 0xf0 - *(int *)((int)this + 0x50);
  *(int *)((int)this + 0xa0) = iVar8;
  *(int *)((int)this + 0xa4) = iVar7;
  uVar6 = (uint)*(ushort *)(*(int *)((int)this + 0x38) + 0x30);
  iVar4 = *(int *)(*(int *)((int)this + 0x38) + 0x1c);
  psVar1 = (short *)(iVar4 + uVar6 * 0x24);
  piVar5 = *(int **)(iVar4 + 0x18 + uVar6 * 0x24);
  if (piVar5 == (int *)0x0) {
    *(int *)((int)this + 0xa0) = (int)psVar1[8] / 2 + iVar8 + psVar1[6];
    sVar3 = psVar1[7];
  }
  else {
    *(uint *)((int)this + 0xa0) = (int)*psVar1 + iVar8 + (uint)(*(ushort *)(*piVar5 + 8) >> 1);
    sVar3 = psVar1[7];
  }
  *(int *)((int)this + 0xa4) = (iVar7 - sVar3) + -0x32;
  if (*(int *)((int)this + 0xc0) != 0) {
    bVar2 = *(byte *)((int)this + param_1 * 4 + 0xb0);
    *(byte *)((int)this + 0x32) = bVar2;
    this_00 = *(void **)(*(int *)((int)this + 0x34) + (uint)bVar2 * 4);
    *(void **)((int)this + 0x38) = this_00;
    FUN_00401740(this_00,1,1,0,'\0');
  }
  return;
}

