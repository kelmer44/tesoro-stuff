/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401860
 */


void __fastcall tesoro_advance_animation(int param_1)

{
  short sVar1;
  short sVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  ushort uVar8;
  int iVar9;
  int local_c;
  
  local_c = FUN_00413a10(DAT_00442120);
  uVar5 = *(uint *)(param_1 + 0x18);
  if (((uVar5 & 1) != 0) && ((uVar5 & 2) == 0)) {
    uVar4 = *(uint *)(param_1 + 0xc);
    if (uVar4 != 0) {
      local_c = local_c - ((uint)(local_c - *(int *)(param_1 + 8)) / uVar4) * uVar4;
    }
    uVar8 = *(ushort *)(param_1 + 0x30);
    iVar3 = *(int *)(param_1 + 0x1c);
    iVar9 = *(int *)(param_1 + 8);
    uVar6 = (uint)uVar8;
    uVar4 = (uint)*(ushort *)(iVar3 + 8 + uVar6 * 0x24);
    if ((int)uVar4 < local_c - iVar9) {
      do {
        if (((uVar5 & 4) == 0) || (uVar7 = (uint)*(ushort *)(param_1 + 0x26), uVar6 + 1 != uVar7)) {
          if (uVar8 == *(ushort *)(param_1 + 0x26)) {
            *(uint *)(param_1 + 0x18) = uVar5 & 0xfffffffe;
            break;
          }
          *(uint *)(param_1 + 8) = uVar4 + iVar9;
          *(ushort *)(param_1 + 0x30) = uVar8 + 1;
        }
        else {
          *(uint *)(param_1 + 8) = uVar4 + iVar9;
          uVar8 = *(ushort *)(param_1 + 0x24);
          *(ushort *)(param_1 + 0x30) = uVar8;
          *(short *)(param_1 + 0x2c) =
               *(short *)(param_1 + 0x2c) +
               (*(short *)(iVar3 + 4 + uVar7 * 0x24) - *(short *)(iVar3 + 4 + (uint)uVar8 * 0x24));
          *(short *)(param_1 + 0x2e) =
               *(short *)(param_1 + 0x2e) +
               (*(short *)(iVar3 + uVar7 * 0x24 + 6) - *(short *)(iVar3 + (uint)uVar8 * 0x24 + 6));
        }
        uVar8 = *(ushort *)(param_1 + 0x30);
        iVar9 = *(int *)(param_1 + 8);
        uVar6 = (uint)uVar8;
        uVar4 = (uint)*(ushort *)(iVar3 + 8 + uVar6 * 0x24);
      } while ((int)uVar4 < local_c - iVar9);
    }
  }
  uVar8 = *(ushort *)(param_1 + 0x30);
  if ((uVar8 != *(ushort *)(param_1 + 0x26)) && ((*(uint *)(param_1 + 0x18) & 1) != 0)) {
    if ((*(uint *)(param_1 + 0x18) >> 3 & 1) == 0) {
      *(short *)(param_1 + 0x28) =
           *(short *)(param_1 + 0x2c) +
           *(short *)(*(int *)(param_1 + 0x1c) + 4 + (uint)uVar8 * 0x24);
      *(short *)(param_1 + 0x2a) =
           *(short *)(param_1 + 0x2e) +
           *(short *)(*(int *)(param_1 + 0x1c) + (uint)uVar8 * 0x24 + 6);
      return;
    }
    uVar5 = (uint)uVar8;
    local_c = local_c - *(int *)(param_1 + 8);
    iVar3 = *(int *)(param_1 + 0x1c);
    sVar1 = *(short *)(iVar3 + 0x2a + uVar5 * 0x24);
    sVar2 = *(short *)(iVar3 + 6 + uVar5 * 0x24);
    iVar3 = iVar3 + uVar5 * 0x24;
    *(short *)(param_1 + 0x28) =
         (short)((((int)*(short *)(iVar3 + 0x28) - (int)*(short *)(iVar3 + 4)) * local_c) /
                (int)(uint)*(ushort *)(iVar3 + 8)) + *(short *)(param_1 + 0x2c) +
         (short)*(undefined4 *)(iVar3 + 4);
    *(short *)(param_1 + 0x2a) =
         (short)((((int)sVar1 - (int)sVar2) * local_c) / (int)(uint)*(ushort *)(iVar3 + 8)) +
         *(short *)(param_1 + 0x2e) + *(short *)(iVar3 + 6);
    return;
  }
  *(undefined4 *)(param_1 + 0x28) =
       *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4 + (uint)uVar8 * 0x24);
  return;
}

