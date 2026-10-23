/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407600
 */


undefined4 __thiscall
tesoro_jim_start_walk
          (void *this,undefined4 param_1,undefined4 param_2,int *param_3,undefined1 param_4)

{
  void *this_00;
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  FUN_0040fb20(DAT_00442104,(int)this);
  if (*(int *)(*(int *)(*(int *)(DAT_004420fc + 0x40) + 0x90) + 0x78) != 0) {
    FUN_0040fa10(DAT_00442104,DAT_004420fc,5);
    *(undefined1 *)((int)this + 0xd5) = 1;
    *(int *)((int)this + 0xe0) = *(int *)((int)this + 0x4c) << 8;
    *(int *)((int)this + 0xe4) = *(int *)((int)this + 0x50) << 8;
    *(undefined4 *)((int)this + 0x104) = param_1;
    *(undefined4 *)((int)this + 0x108) = param_2;
    *(undefined1 *)((int)this + 0xf0) = param_4;
    *(int **)((int)this + 0xf4) = param_3;
    iVar2 = *(int *)((int)this + 0x104) - *(int *)((int)this + 0x4c);
    iVar3 = *(int *)((int)this + 0x108) - *(int *)((int)this + 0x50);
    iVar5 = iVar2;
    if (iVar2 < 0) {
      iVar5 = -iVar2;
    }
    iVar4 = iVar3;
    if (iVar3 < 0) {
      iVar4 = -iVar3;
    }
    if (iVar4 < iVar5) {
      *(uint *)((int)this + 0x10c) = (-1 < iVar2) + 3;
    }
    else {
      *(uint *)((int)this + 0x10c) = (iVar3 < 0) + 1;
    }
    this_00 = *(void **)(*(int *)((int)this + 0xd8) + *(int *)((int)this + 0x10c) * 4);
    if ((*(void **)((int)this + 0x38) != this_00) ||
       ((*(byte *)((int)*(void **)((int)this + 0x38) + 0x18) & 1) == 0)) {
      *(void **)((int)this + 0x38) = this_00;
      FUN_00401740(this_00,1,-1,1,'\0');
      *(undefined4 *)((int)this + 0xe8) = 0;
      *(undefined4 *)((int)this + 0xec) = 0;
    }
    return 1;
  }
  if (param_3 != (int *)0x0) {
    uVar1 = (**(code **)(*param_3 + 0x34))();
    return uVar1;
  }
  return 0;
}

