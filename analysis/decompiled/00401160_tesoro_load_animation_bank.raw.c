/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401160
 */


uint * __thiscall tesoro_load_animation_bank(void *this,int param_1,int param_2,uint param_3)

{
  short sVar1;
  short sVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  undefined2 *puVar7;
  int iVar8;
  short *psVar9;
  undefined4 *puVar10;
  int local_428;
  int local_424 [2];
  undefined4 local_41c;
  undefined4 local_418;
  undefined2 local_414;
  short local_412;
  undefined4 local_410;
  undefined4 local_40c;
  undefined2 local_408;
  ushort uStack_406;
  undefined2 local_400 [2];
  undefined2 local_3fc;
  undefined4 local_3f8;
  undefined4 local_3f4;
  undefined4 local_3f0;
  char local_3ec;
  uint local_3e8;
  undefined4 uStack_3e2;
  undefined2 local_3de [495];
  
  piVar3 = (int *)FUN_004032e0(DAT_00442064,param_1 + 4);
  FUN_0041b4c0((char *)local_400,0x18,1,piVar3);
  *(undefined2 *)((int)this + 0x22) = local_400[0];
  *(undefined2 *)((int)this + 0x20) = local_3fc;
  *(undefined4 *)((int)this + 0x32) = local_3f8;
  *(undefined4 *)((int)this + 0x36) = local_3f4;
  *(undefined4 *)((int)this + 0x3a) = local_3f0;
  *(undefined4 *)((int)this + 0x18) = 0;
  *(uint *)((int)this + 0x18) = (uint)(local_3ec == '\x01') << 3;
  uVar4 = FUN_0040dc10(DAT_00442100,(uint)*(ushort *)((int)this + 0x20) << 2,4,param_2);
  *(uint *)((int)this + 4) = uVar4;
  uVar4 = FUN_0040dc10(DAT_00442100,(uint)*(ushort *)((int)this + 0x20) << 2,4,param_2);
  *(uint *)this = uVar4;
  uVar4 = FUN_0040dc10(DAT_00442100,(uint)*(ushort *)((int)this + 0x22) * 0x24,4,param_2);
  *(uint *)((int)this + 0x1c) = uVar4;
  local_428 = 0;
  if (*(short *)((int)this + 0x22) != 0) {
    local_424[0] = 0;
    do {
      FUN_0041b4c0((char *)&local_41c,0x18,1,piVar3);
      puVar10 = (undefined4 *)(local_424[0] + *(int *)((int)this + 0x1c));
      *puVar10 = local_41c;
      puVar10[1] = local_418;
      *(undefined2 *)(puVar10 + 2) = local_414;
      puVar10[3] = local_410;
      puVar10[4] = local_40c;
      *(undefined2 *)((int)puVar10 + 10) = local_408;
      if (local_412 < 0) {
        iVar5 = 0;
      }
      else {
                    /* WARNING: Load size is inaccurate */
        iVar5 = *this + local_412 * 4;
      }
      puVar10[6] = iVar5;
      FUN_0041b4c0((char *)&local_3e8,(uint)uStack_406,1,piVar3);
      switch(local_408) {
      case 0:
      case 1:
        *(undefined2 *)(puVar10 + 7) = 0;
        puVar10[5] = 0;
        break;
      case 2:
        puVar10[5] = 0;
        *(short *)(puVar10 + 7) = (short)local_3e8;
        uVar4 = FUN_0040dc10(DAT_00442100,(local_3e8 & 0xffff) << 2,4,param_2);
        iVar5 = 0;
        puVar10[8] = uVar4;
        if ((short)local_3e8 != 0) {
          iVar8 = 0;
          do {
            iVar5 = iVar5 + 1;
            *(undefined4 *)(iVar8 + puVar10[8]) = *(undefined4 *)((int)local_3de + iVar8 + -4);
            iVar8 = iVar8 + 4;
          } while (iVar5 < (int)(local_3e8 & 0xffff));
        }
        break;
      case 4:
        *(short *)(puVar10 + 7) = (short)local_3e8;
        uVar4 = FUN_0040dc10(DAT_00442100,(local_3e8 & 0xffff) << 2,4,param_2);
        puVar10[8] = uVar4;
        uVar4 = FUN_0040dc10(DAT_00442100,(local_3e8 & 0xffff) << 1,4,param_2);
        puVar10[5] = uVar4;
        iVar5 = 0;
        if ((short)local_3e8 != 0) {
          puVar7 = local_3de;
          do {
            *(undefined2 *)(puVar10[8] + iVar5 * 4) = puVar7[-1];
            *(undefined2 *)(puVar10[8] + 2 + iVar5 * 4) = *puVar7;
            *(undefined2 *)(puVar10[5] + iVar5 * 2) = puVar7[1];
            iVar5 = iVar5 + 1;
            puVar7 = puVar7 + 3;
          } while (iVar5 < (int)(local_3e8 & 0xffff));
        }
      }
      if (*(ushort *)(puVar10 + 7) != 0) {
        iVar5 = 0;
        iVar8 = *(ushort *)(puVar10 + 7) - 1;
        if (0 < iVar8) {
          psVar9 = (short *)puVar10[8];
          do {
            if (psVar9[2] < *psVar9) break;
            iVar5 = iVar5 + 1;
            psVar9 = psVar9 + 2;
          } while (iVar5 < iVar8);
        }
        *(short *)((int)puVar10 + 0x1e) = (short)iVar5;
      }
      local_428 = local_428 + 1;
      local_424[0] = local_424[0] + 0x24;
    } while (local_428 < (int)(uint)*(ushort *)((int)this + 0x22));
  }
  iVar5 = 0;
  *(uint *)((int)this + 0x14) = param_3;
  if (*(short *)((int)this + 0x20) != 0) {
    do {
      FUN_0041b490(piVar3,(ulonglong *)local_424);
      *(int *)(*(int *)((int)this + 4) + iVar5 * 4) = local_424[0];
      if (param_3 == 2) {
        FUN_0041b4c0((char *)&local_428,4,1,piVar3);
        FUN_0041b3f0(piVar3,local_428,1);
      }
      else {
        piVar6 = tesoro_load_image_buffer(piVar3,param_3);
                    /* WARNING: Load size is inaccurate */
        *(int **)(*this + iVar5 * 4) = piVar6;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < (int)(uint)*(ushort *)((int)this + 0x20));
  }
  iVar5 = 1;
  sVar1 = *(short *)(*(int *)((int)this + 0x1c) + 4);
  sVar2 = *(short *)(*(int *)((int)this + 0x1c) + 6);
  if (1 < *(ushort *)((int)this + 0x22)) {
    iVar8 = 0x24;
    do {
      psVar9 = (short *)(iVar8 + 4 + *(int *)((int)this + 0x1c));
      *psVar9 = *psVar9 - sVar1;
      psVar9 = (short *)(iVar8 + 6 + *(int *)((int)this + 0x1c));
      *psVar9 = *psVar9 - sVar2;
      iVar5 = iVar5 + 1;
      iVar8 = iVar8 + 0x24;
    } while (iVar5 < (int)(uint)*(ushort *)((int)this + 0x22));
  }
  *(undefined2 *)(*(int *)((int)this + 0x1c) + 4) = 0;
  *(undefined2 *)(*(int *)((int)this + 0x1c) + 6) = 0;
  return (uint *)this;
}

