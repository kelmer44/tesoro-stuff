/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401fc0
 */


void __thiscall tesoro_set_group_camera(void *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
                    /* WARNING: Load size is inaccurate */
  if (*this != 0) {
    iVar1 = *(int *)((int)this + 0x14);
    if ((*(int *)(*this + 0x4c) < iVar1 + -0x4b) && (*(int *)((int)this + 0x1c) < iVar1 + -0x140)) {
      uVar2 = *(undefined4 *)((int)this + 0x18);
      *(undefined4 *)((int)this + 0x10) = 0x2ee;
      iVar3 = FUN_00413a10(DAT_00442120);
      *(int *)((int)this + 0xc) = iVar3;
      *(int *)((int)this + 0x14) = iVar1 + -100;
      *(undefined4 *)((int)this + 4) = *(undefined4 *)((int)this + 0x2c);
      *(undefined4 *)((int)this + 8) = *(undefined4 *)((int)this + 0x30);
      *(undefined4 *)((int)this + 0x18) = uVar2;
    }
    iVar1 = *(int *)((int)this + 0x14);
                    /* WARNING: Load size is inaccurate */
    if ((iVar1 + 0x4b < *(int *)(*this + 0x4c)) && (iVar1 + 0x13f < *(int *)((int)this + 0x24))) {
      uVar2 = *(undefined4 *)((int)this + 0x18);
      *(undefined4 *)((int)this + 0x10) = 0x2ee;
      iVar3 = FUN_00413a10(DAT_00442120);
      *(int *)((int)this + 0xc) = iVar3;
      *(int *)((int)this + 0x14) = iVar1 + 100;
      *(undefined4 *)((int)this + 4) = *(undefined4 *)((int)this + 0x2c);
      *(undefined4 *)((int)this + 8) = *(undefined4 *)((int)this + 0x30);
      *(undefined4 *)((int)this + 0x18) = uVar2;
    }
  }
  if (*(int *)((int)this + 0x10) != 0) {
    iVar3 = FUN_00413a10(DAT_00442120);
    iVar1 = *(int *)((int)this + 0x10);
    iVar3 = iVar3 - *(int *)((int)this + 0xc);
    if (iVar1 < iVar3) {
      *(undefined4 *)((int)this + 0x10) = 0;
      FUN_00402120(this,*(int *)((int)this + 0x14),*(int *)((int)this + 0x18));
      *(undefined4 *)((int)this + 0x14) = *(undefined4 *)((int)this + 0x2c);
      *(undefined4 *)((int)this + 0x18) = *(undefined4 *)((int)this + 0x30);
    }
    else {
      FUN_00402120(this,((*(int *)((int)this + 0x14) - *(int *)((int)this + 4)) * iVar3) / iVar1 +
                        *(int *)((int)this + 4),
                   ((*(int *)((int)this + 0x18) - *(int *)((int)this + 8)) * iVar3) / iVar1 +
                   *(int *)((int)this + 8));
    }
  }
  *(int *)((int)this + 0x3c) = param_1;
  *(int *)((int)this + 0x34) = (*(int *)((int)this + 0x2c) * 300) / param_1;
  *(int *)((int)this + 0x38) = (*(int *)((int)this + 0x30) * 300) / param_1;
  return;
}

