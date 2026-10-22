/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00412e10
 */


void __fastcall tesoro_compose_render_rows(void *param_1)

{
  void *this;
  int iVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  ushort *puVar5;
  uint uVar6;
  int *piVar7;
  undefined4 *puVar8;
  short sVar9;
  uint uVar10;
  uint uVar11;
  char *pcVar12;
  int *piVar13;
  int *piVar14;
  uint *puVar15;
  undefined4 *puVar16;
  uint *puVar17;
  uint *puVar18;
  uint *puVar19;
  undefined4 local_c0 [4];
  undefined4 local_b0;
  undefined4 local_9c;
  undefined4 local_44 [8];
  int local_24;
  void *local_20;
  uint *local_1c;
  int local_18;
  uint local_14;
  int *local_10;
  int local_c;
  ushort *local_8;
  
  local_20 = param_1;
  if (*(char *)((int)param_1 + 0x66a) == '\0') {
    iVar2 = (**(code **)(**(int **)((int)param_1 + 0xc) + 0x2c))(*(int **)((int)param_1 + 0xc),0,0);
    while (iVar2 == -0x7789fde4) {
      iVar2 = (**(code **)(**(int **)((int)param_1 + 0xc) + 0x2c))
                        (*(int **)((int)param_1 + 0xc),0,0);
    }
    local_c0[0] = 0x7c;
    iVar2 = (**(code **)(**(int **)((int)param_1 + 0xc) + 0x58))
                      (*(int **)((int)param_1 + 0xc),local_c0);
    if (iVar2 == 0) {
      iVar2 = (**(code **)(**(int **)((int)param_1 + 0xc) + 100))
                        (*(int **)((int)param_1 + 0xc),0,local_c0,0,0);
      while (iVar2 == -0x7789fde4) {
        iVar2 = (**(code **)(**(int **)((int)param_1 + 0xc) + 100))
                          (*(int **)((int)param_1 + 0xc),0,local_c0,0,0);
      }
      if (iVar2 == 0) {
        *(undefined1 *)((int)param_1 + 0x66a) = 1;
        *(undefined4 *)((int)param_1 + 0x1c) = local_b0;
        *(undefined4 *)((int)param_1 + 0x678) = local_9c;
      }
    }
  }
  local_10 = (int *)((int)param_1 + 0x24);
  iVar2 = *(int *)((int)param_1 + 0x664);
  if (0 < iVar2 + -1) {
    local_c = 1;
    local_8 = (ushort *)((int)param_1 + 0x44);
    do {
      piVar14 = local_10;
      piVar7 = local_10;
      if (local_c < iVar2) {
        iVar2 = iVar2 - local_c;
        puVar5 = local_8;
        do {
          if (*(ushort *)(piVar7 + 3) < *(ushort *)((int)puVar5 + 0xc)) {
            piVar7 = (int *)puVar5;
          }
          puVar5 = (ushort *)((int)puVar5 + 0x20);
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
      puVar8 = piVar7;
      puVar16 = local_44;
      for (iVar2 = 8; iVar1 = local_c, iVar2 != 0; iVar2 = iVar2 + -1) {
        *puVar16 = *puVar8;
        puVar8 = puVar8 + 1;
        puVar16 = puVar16 + 1;
      }
      piVar13 = local_10;
      for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
        *piVar7 = *piVar13;
        piVar13 = piVar13 + 1;
        piVar7 = piVar7 + 1;
      }
      local_10 = local_10 + 8;
      puVar8 = local_44;
      for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
        *piVar14 = *puVar8;
        puVar8 = puVar8 + 1;
        piVar14 = piVar14 + 1;
      }
      local_8 = (ushort *)((int)local_8 + 0x20);
      iVar2 = *(int *)((int)param_1 + 0x664);
      local_c = local_c + 1;
    } while (iVar1 < iVar2 + -1);
  }
  local_8 = (ushort *)((int)param_1 + 0x30);
  piVar14 = (int *)((int)param_1 + 0x24);
  iVar2 = 0;
  local_10 = piVar14;
  if (*(ushort *)((int)param_1 + 0x668) < *(ushort *)((int)param_1 + 0x30)) {
    do {
      if (*(int *)((int)param_1 + 0x664) <= iVar2) break;
      tesoro_draw_scaled_sprite
                (param_1,piVar14[1],piVar14[2],*(ushort *)(piVar14 + 3),*piVar14,piVar14[4],
                 piVar14[5],piVar14[6],piVar14[7]);
      piVar14 = piVar14 + 8;
      local_8 = local_8 + 0x10;
      iVar2 = iVar2 + 1;
    } while (*(ushort *)((int)param_1 + 0x668) < *local_8);
  }
  local_10 = piVar14;
  local_8 = *(ushort **)((int)param_1 + 0x670);
  local_1c = *(uint **)((int)param_1 + 0x678);
  local_14 = (uint)*(byte *)((int)param_1 + 0x67c);
  local_18 = *(int *)((int)param_1 + 0x66c);
  local_c = 0;
  local_24 = iVar2;
  tesoro_apply_shadow_rows((int)param_1);
  this = local_20;
  puVar18 = local_1c;
  do {
    puVar8 = *(undefined4 **)local_8;
    do {
      while( true ) {
        uVar3 = *(ushort *)((int)puVar8 + 10);
        puVar15 = (uint *)puVar8[1];
        if (*(short *)(puVar8 + 2) == -1) break;
        sVar9 = uVar3 - *(short *)(puVar8 + 2);
        if (puVar15 == (uint *)0x1) {
          uVar3 = *(ushort *)((int)puVar8 + 0xe);
          uVar6 = (uint)((ushort)(sVar9 + 1U) >> 1);
          puVar15 = puVar18;
          if ((sVar9 + 1U & 1) != 0) {
            puVar15 = (uint *)((int)puVar18 + 2);
            *(ushort *)puVar18 = uVar3;
          }
          puVar18 = puVar15;
          for (; uVar6 != 0; uVar6 = uVar6 - 1) {
            *puVar18 = CONCAT22(uVar3,uVar3);
            puVar18 = puVar18 + 1;
          }
          puVar8 = (undefined4 *)*puVar8;
        }
        else if (puVar15 == (uint *)0x0) {
          puVar18 = (uint *)((int)puVar18 + (uint)(ushort)(sVar9 + 1) * 2);
          puVar8 = (undefined4 *)*puVar8;
        }
        else {
          uVar3 = sVar9 + 1;
          iVar2 = 0;
          puVar19 = puVar15;
          puVar17 = puVar18;
          if ((uVar3 & 1) != 0) {
            puVar17 = (uint *)((int)puVar18 + 2);
            puVar19 = (uint *)((int)puVar15 + 2);
            *(ushort *)puVar18 = (ushort)*puVar15;
          }
          uVar4 = uVar3 >> 2;
          puVar18 = puVar17;
          puVar15 = puVar19;
          if ((uVar3 >> 1 & 1) != 0) {
            puVar18 = puVar17 + 1;
            puVar15 = puVar19 + 1;
            *puVar17 = *puVar19;
          }
          for (; uVar4 != 0; uVar4 = uVar4 - 1) {
            *(undefined4 *)((int)puVar18 + iVar2) = *(undefined4 *)((int)puVar15 + iVar2);
            *(undefined4 *)((int)puVar18 + iVar2 + 4) = *(undefined4 *)((int)puVar15 + iVar2 + 4);
            iVar2 = iVar2 + 8;
          }
          puVar18 = (uint *)((int)puVar18 + iVar2);
          puVar8 = (undefined4 *)*puVar8;
        }
joined_r0x0041315e:
        if (puVar8 == (undefined4 *)0x0) goto LAB_0041321d;
      }
      if ((int)local_14 < 1) {
        iVar2 = local_18 + (uint)*(ushort *)((int)puVar8 + 0xe) * 0x100;
        puVar19 = puVar18;
        if ((uVar3 & 1) != 0) {
          uVar6 = *puVar15;
          puVar15 = (uint *)((int)puVar15 + 2);
          puVar19 = (uint *)((int)puVar18 + 2);
          *(ushort *)puVar18 =
               *(ushort *)
                (iVar2 + ((ushort)((ushort)uVar6 >> 9) & 0x70 | ((ushort)uVar6 & 0x71c) >> 1));
        }
        uVar6 = (uint)(uVar3 >> 2);
        puVar18 = puVar19;
        if ((uVar3 >> 1 & 1) != 0) {
          puVar18 = puVar19 + 1;
          uVar10 = *puVar15 >> 9 & 0x700070 | *puVar15 >> 1 & 0x38e038e;
          puVar15 = puVar15 + 1;
          *puVar19 = CONCAT22((short)((uint)*(undefined4 *)(iVar2 + -2 + (uVar10 >> 0x10)) >> 0x10),
                              *(undefined2 *)(iVar2 + (uVar10 & 0xffff)));
        }
        for (; uVar6 != 0; uVar6 = uVar6 - 1) {
          uVar10 = puVar15[1];
          uVar11 = *puVar15 >> 9 & 0x700070 | *puVar15 >> 1 & 0x38e038e;
          puVar15 = puVar15 + 2;
          *puVar18 = CONCAT22((short)((uint)*(undefined4 *)(iVar2 + -2 + (uVar11 >> 0x10)) >> 0x10),
                              *(undefined2 *)(iVar2 + (uVar11 & 0xffff)));
          uVar10 = uVar10 >> 9 & 0x700070 | uVar10 >> 1 & 0x38e038e;
          puVar18[1] = CONCAT22((short)((uint)*(undefined4 *)(iVar2 + -2 + (uVar10 >> 0x10)) >> 0x10
                                       ),*(undefined2 *)(iVar2 + (uVar10 & 0xffff)));
          puVar18 = puVar18 + 2;
        }
        puVar8 = (undefined4 *)*puVar8;
        goto joined_r0x0041315e;
      }
      iVar2 = local_18 + (uint)*(ushort *)((int)puVar8 + 0xe) * 0x100;
      uVar6 = (uint)(uVar3 >> 1);
      if ((uVar3 & 1) != 0) {
        uVar3 = (ushort)*puVar15;
        puVar15 = (uint *)((int)puVar15 + 2);
        *(ushort *)puVar18 =
             *(ushort *)(iVar2 + (uVar3 >> 8 & 0x70 | (uVar3 & 0x1c) >> 1 | uVar3 & 0x380));
        puVar18 = (uint *)((int)puVar18 + 2);
      }
      for (; uVar6 != 0; uVar6 = uVar6 - 1) {
        uVar10 = *puVar15;
        puVar15 = puVar15 + 1;
        uVar10 = uVar10 >> 8 & 0x700070 | uVar10 >> 1 & 0xe000e | uVar10 & 0x3800380;
        *puVar18 = CONCAT22((short)((uint)*(undefined4 *)(iVar2 + -2 + (uVar10 >> 0x10)) >> 0x10),
                            *(undefined2 *)(iVar2 + (uVar10 & 0xffff)));
        puVar18 = puVar18 + 1;
      }
      puVar8 = (undefined4 *)*puVar8;
    } while (puVar8 != (undefined4 *)0x0);
LAB_0041321d:
    local_8 = (ushort *)((int)local_8 + 2000);
    local_c = local_c + 1;
    if (0x1df < local_c) {
      iVar2 = 0;
      if (0 < *(int *)((int)local_20 + 0x13a0)) {
        pcVar12 = (char *)((int)local_20 + 0x680);
        do {
          tesoro_draw_text_overlay(this,pcVar12);
          iVar2 = iVar2 + 1;
          pcVar12 = pcVar12 + 0x70;
        } while (iVar2 < *(int *)((int)this + 0x13a0));
      }
      piVar14 = local_10;
      for (iVar2 = *(int *)((int)this + 0x664) - local_24; iVar2 != 0; iVar2 = iVar2 + -1) {
        local_10 = piVar14;
        tesoro_draw_scaled_sprite
                  (this,piVar14[1],piVar14[2],*(ushort *)(piVar14 + 3),*piVar14,piVar14[4],
                   piVar14[5],piVar14[6],piVar14[7]);
        piVar14 = piVar14 + 8;
      }
      return;
    }
  } while( true );
}

