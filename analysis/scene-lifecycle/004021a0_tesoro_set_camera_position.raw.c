/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004021a0
 */


void __thiscall tesoro_set_camera_position(void *this,int param_1,int param_2,int param_3)

{
  int iVar1;
  
  if (param_3 != 0) {
    *(int *)((int)this + 0x10) = param_3;
    iVar1 = FUN_00413a10(DAT_00442120);
    *(int *)((int)this + 0xc) = iVar1;
    *(undefined4 *)((int)this + 4) = *(undefined4 *)((int)this + 0x2c);
    *(int *)((int)this + 0x14) = param_1;
    *(undefined4 *)((int)this + 8) = *(undefined4 *)((int)this + 0x30);
    *(int *)((int)this + 0x18) = param_2;
    return;
  }
  tesoro_clamp_camera_position(this,param_1,param_2);
  *(int *)((int)this + 0x14) = param_1;
  *(int *)((int)this + 0x18) = param_2;
  return;
}

