/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401740
 */


void __thiscall
tesoro_set_animation_playback(void *this,ushort param_1,int param_2,byte param_3,char param_4)

{
  ushort uVar1;
  ushort uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  ushort *puVar6;
  uint uVar7;
  
  uVar3 = *(uint *)((int)this + 0x18);
  uVar2 = *(ushort *)((int)this + 0x22);
  uVar7 = (param_3 & 1) << 2;
  *(ushort *)((int)this + 0x24) = param_1;
  *(uint *)((int)this + 0x18) = uVar7 | uVar3 & 0xfffffff9 | 1;
  if (uVar2 == 0) {
    *(undefined2 *)((int)this + 0x26) = 0;
    *(undefined2 *)((int)this + 0x24) = 0;
    *(undefined2 *)((int)this + 0x30) = 0;
    *(undefined4 *)((int)this + 0xc) = 0;
    *(undefined4 *)((int)this + 0x18) = 0;
  }
  else {
    if (param_2 == -1) {
      *(ushort *)((int)this + 0x26) = param_1;
      if (param_1 + 1 < (uint)uVar2) {
        do {
          if (*(int *)(*(int *)((int)this + 0x1c) + 0x3c +
                      (uint)*(ushort *)((int)this + 0x26) * 0x24) == 0) {
            if ((param_3 & 1) != 0) {
              *(short *)((int)this + 0x26) = *(short *)((int)this + 0x26) + 1;
            }
            break;
          }
          uVar1 = *(ushort *)((int)this + 0x26) + 1;
          *(ushort *)((int)this + 0x26) = uVar1;
        } while (uVar1 + 1 < (uint)uVar2);
      }
    }
    else {
      *(short *)((int)this + 0x26) = (short)param_2;
    }
    *(ushort *)((int)this + 0x30) = param_1;
    *(undefined4 *)((int)this + 0xc) = 0;
    if (param_4 != '\0') {
      uVar4 = (uint)param_1;
      if (uVar4 <= *(ushort *)((int)this + 0x26)) {
        puVar6 = (ushort *)(*(int *)((int)this + 0x1c) + 8 + uVar4 * 0x24);
        iVar5 = (*(ushort *)((int)this + 0x26) - uVar4) + 1;
        do {
          uVar2 = *puVar6;
          puVar6 = puVar6 + 0x12;
          iVar5 = iVar5 + -1;
          *(uint *)((int)this + 0xc) = *(int *)((int)this + 0xc) + (uint)uVar2;
        } while (iVar5 != 0);
      }
    }
    if (param_1 == *(ushort *)((int)this + 0x26)) {
      *(uint *)((int)this + 0x18) = uVar7 | uVar3 & 0xfffffff8;
    }
  }
  iVar5 = FUN_00413a10(DAT_00442120);
  *(undefined2 *)((int)this + 0x2c) = 0;
  *(undefined2 *)((int)this + 0x2e) = 0;
  *(int *)((int)this + 8) = iVar5;
  return;
}

