/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413b90
 */


uint __cdecl tesoro_input_413b90(HWND param_1,undefined4 param_2)

{
  bool bVar1;
  undefined4 *puVar2;
  void *pvVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  undefined4 *unaff_FS_OFFSET;
  char *lpText;
  undefined4 local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0042705d;
  local_c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_c;
  puVar2 = (undefined4 *)FUN_0041ba10(0x10);
  local_4 = 0;
  if (puVar2 == (undefined4 *)0x0) {
    DAT_00442120 = (undefined4 *)0x0;
  }
  else {
    DAT_00442120 = FUN_004139c0(puVar2);
  }
  local_4 = 0xffffffff;
  pvVar3 = (void *)FUN_0041ba10(0x44);
  local_4 = 1;
  if (pvVar3 == (void *)0x0) {
    DAT_00442100 = (int *)0x0;
  }
  else {
    DAT_00442100 = FUN_0040dbc0(pvVar3,0x39d0);
  }
  local_4 = 0xffffffff;
  if (DAT_00442100 == (int *)0x0) {
LAB_00413e31:
    lpText = &DAT_0042e380;
  }
  else {
    bVar1 = FUN_0040dc00(DAT_00442100);
    if (!bVar1) goto LAB_00413e31;
    FUN_0040dcf0(DAT_00442100,0x17a6b0,0);
    FUN_0040dcf0(DAT_00442100,3000000,1);
    FUN_0040dcf0(DAT_00442100,9600000,2);
    FUN_0040dcf0(DAT_00442100,650000,3);
    puVar2 = FUN_0040db40(4,0);
    if (puVar2 == (undefined4 *)0x0) {
      DAT_00442104 = (undefined4 *)0x0;
    }
    else {
      DAT_00442104 = FUN_0040f890(puVar2,100,0);
    }
    iVar4 = FUN_0041ba10(8);
    local_4 = 2;
    if (iVar4 == 0) {
      uVar5 = 0;
    }
    else {
      uVar5 = FUN_00403220(iVar4);
    }
    local_4 = 0xffffffff;
    DAT_00442064 = uVar5;
    if (uVar5 == 0) goto LAB_00413e48;
    pvVar3 = (void *)FUN_0041ba10(0x38);
    local_4 = 3;
    if (pvVar3 == (void *)0x0) {
      DAT_00442074 = (undefined4 *)0x0;
    }
    else {
      DAT_00442074 = FUN_00403820(pvVar3,param_1,param_2,6);
    }
    local_4 = 0xffffffff;
    if (DAT_00442074 == (undefined4 *)0x0) {
LAB_00413e22:
      lpText = s_Error_al_inicializar_el_disposit_0042dac0;
    }
    else {
      bVar1 = FUN_004036c0((int)DAT_00442074);
      if (!bVar1) goto LAB_00413e22;
      pvVar3 = (void *)FUN_0041ba10(0x40);
      local_4 = 4;
      if (pvVar3 == (void *)0x0) {
        DAT_0044206c = (undefined4 *)0x0;
      }
      else {
        DAT_0044206c = FUN_00403980(pvVar3,param_1,param_2,5);
      }
      local_4 = 0xffffffff;
      bVar1 = FUN_004036c0((int)DAT_0044206c);
      if (bVar1) {
        pvVar3 = (void *)FUN_0041ba10(0x20);
        local_4 = 5;
        if (pvVar3 == (void *)0x0) {
          DAT_00442080 = (int *)0x0;
        }
        else {
          DAT_00442080 = FUN_00403e00(pvVar3,param_1,0);
        }
        local_4 = 0xffffffff;
        if (DAT_00442080 != (int *)0x0) {
          bVar1 = FUN_004040b0(DAT_00442080);
          if (bVar1) {
            pvVar3 = (void *)FUN_0041ba10(0x13a4);
            local_4 = 6;
            if (pvVar3 == (void *)0x0) {
              DAT_00442108 = (undefined4 *)0x0;
            }
            else {
              DAT_00442108 = FUN_00411870(pvVar3,param_1);
            }
            local_4 = 0xffffffff;
            if (DAT_00442108 != (undefined4 *)0x0) {
              uVar6 = FUN_00411c00((int)DAT_00442108);
              if ((char)uVar6 != '\0') {
                *unaff_FS_OFFSET = local_c;
                return CONCAT31((int3)((uint)uVar6 >> 8),1);
              }
            }
            lpText = s_Error_al_inicializar_el_disposit_0042dc00;
            goto LAB_00413e42;
          }
        }
        lpText = s_Error_al_inicializar_el_disposit_0042dbb0;
      }
      else {
        lpText = &DAT_0042db10;
      }
    }
  }
LAB_00413e42:
  uVar5 = MessageBoxA(param_1,lpText,s_ERROR_0042d06c,0x30);
LAB_00413e48:
  *unaff_FS_OFFSET = local_c;
  return uVar5 & 0xffffff00;
}

