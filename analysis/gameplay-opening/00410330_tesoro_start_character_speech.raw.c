/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410330
 */


undefined4 __thiscall
tesoro_start_character_speech(void *this,int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  *(int *)((int)this + 0xcc) = param_2;
  *(int *)((int)this + 0xc4) = param_1;
  *(undefined1 *)((int)this + 0xd4) = 1;
  if (*(char *)((int)this + 0xc) == '\0') {
    *(undefined1 *)((int)this + 0xc) = 1;
    FUN_00401a50(*(int *)((int)this + 0x38));
    FUN_00401da0((int)this);
  }
  if (*(int *)(param_1 + 8) == 0) {
    *(undefined4 *)((int)this + 200) = 0;
  }
  else {
    uVar2 = FUN_00404110(DAT_00442080,*(int *)(param_1 + 8),0,0,0);
    *(undefined4 *)((int)this + 200) = uVar2;
  }
  iVar3 = FUN_00413a10(DAT_00442120);
                    /* WARNING: Load size is inaccurate */
  iVar1 = *this;
  *(int *)((int)this + 0xa8) = iVar3;
  (**(code **)(iVar1 + 0x78))(param_3);
  FUN_0040f900(DAT_00442104,this,6);
  (**(code **)(iVar1 + 0x10))();
  return 1;
}

