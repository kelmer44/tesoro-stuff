/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004137c0
 */


void __fastcall tesoro_sort_text_table(int *param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  int iVar11;
  int local_24;
  int local_20;
  undefined4 *local_1c;
  int local_18;
  
  local_1c = (undefined4 *)*param_1;
  iVar11 = param_1[1];
  if (iVar11 != 1 && -1 < iVar11 + -1) {
    local_18 = 0;
    iVar8 = 0;
    do {
      iVar1 = iVar8 + 1;
      if (iVar1 < iVar11) {
        iVar2 = *param_1;
        iVar7 = local_18;
        local_24 = local_18;
        local_20 = iVar1;
        do {
          iVar9 = iVar7 + 0x10;
          if ((*(uint *)(local_24 + iVar2) < *(uint *)(iVar9 + iVar2)) ||
             ((*(uint *)(iVar9 + iVar2) == *(uint *)(local_24 + iVar2) &&
              (*(uint *)(local_24 + 4 + iVar2) < *(uint *)(iVar7 + 0x14 + iVar2))))) {
            iVar8 = local_20;
            local_24 = iVar9;
          }
          local_20 = local_20 + 1;
          iVar7 = iVar9;
        } while (local_20 < iVar11);
      }
      uVar3 = *local_1c;
      uVar4 = local_1c[1];
      uVar5 = local_1c[2];
      uVar6 = local_1c[3];
      puVar10 = (undefined4 *)(*param_1 + iVar8 * 0x10);
      *local_1c = *puVar10;
      local_1c[1] = puVar10[1];
      local_1c[2] = puVar10[2];
      local_1c[3] = puVar10[3];
      puVar10 = (undefined4 *)(*param_1 + iVar8 * 0x10);
      local_1c = local_1c + 4;
      *puVar10 = uVar3;
      local_18 = local_18 + 0x10;
      puVar10[1] = uVar4;
      puVar10[2] = uVar5;
      puVar10[3] = uVar6;
      iVar11 = param_1[1];
      iVar8 = iVar1;
    } while (iVar1 < iVar11 + -1);
  }
  return;
}

