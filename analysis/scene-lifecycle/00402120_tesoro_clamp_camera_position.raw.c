/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00402120
 */


bool __thiscall tesoro_clamp_camera_position(void *this,int param_1,int param_2)

{
  int iVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  
  iVar1 = *(int *)((int)this + 0x1c);
  *(int *)((int)this + 0x2c) = param_1;
  *(int *)((int)this + 0x30) = param_2;
  bVar2 = iVar1 <= param_1 + -0x140;
  if (!bVar2) {
    *(int *)((int)this + 0x2c) = iVar1 + 0x140;
  }
  iVar1 = *(int *)((int)this + 0x28);
  bVar3 = iVar1 <= param_2 + -0xf0;
  if (!bVar3) {
    *(int *)((int)this + 0x30) = iVar1 + 0xf0;
  }
  iVar1 = *(int *)((int)this + 0x24);
  bVar4 = *(int *)((int)this + 0x2c) + 0x140 <= iVar1;
  if (!bVar4) {
    *(int *)((int)this + 0x2c) = iVar1 + -0x140;
  }
  iVar1 = *(int *)((int)this + 0x20);
  bVar5 = *(int *)((int)this + 0x30) + 0xf0 <= iVar1;
  if (!bVar5) {
    *(int *)((int)this + 0x30) = iVar1 + -0xf0;
  }
  return bVar5 && (bVar4 && (bVar3 && bVar2));
}

