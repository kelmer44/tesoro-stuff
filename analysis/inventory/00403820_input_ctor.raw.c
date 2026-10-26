/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00403820
 */


undefined4 * __thiscall input_ctor(void *this,HWND param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *unaff_FS_OFFSET;
  undefined4 local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_00426f88;
  local_c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_c;
  FUN_004035b0(this,param_1,param_2,param_3,&DAT_0042c0e0);
  local_4 = 0;
  *(undefined ***)this = &PTR_LAB_00428070;
  *(undefined4 *)((int)this + 0x18) = 0x10;
  uVar1 = FUN_0041ba10(0x40);
  *(undefined4 *)((int)this + 0x14) = uVar1;
  puVar2 = (undefined4 *)FUN_0041ba10(*(uint *)((int)this + 0x18));
  uVar4 = *(uint *)((int)this + 0x18);
  *(undefined4 **)((int)this + 0x1c) = puVar2;
  for (uVar3 = uVar4 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined1 *)puVar2 = 0;
    puVar2 = (undefined4 *)((int)puVar2 + 1);
  }
  *(undefined4 *)((int)this + 0x30) = 5;
  *(undefined4 *)((int)this + 0x34) = *(undefined4 *)((int)this + 0x1c);
  *(undefined4 *)((int)this + 0x20) = DAT_0042d078;
  *(undefined1 *)((int)this + 0x24) = DAT_0042d07c;
  *(undefined4 *)((int)this + 0xc) = 200;
  *(undefined4 *)((int)this + 0x10) = 20000;
  *unaff_FS_OFFSET = local_c;
  return (undefined4 *)this;
}

