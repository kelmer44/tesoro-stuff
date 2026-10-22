/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413e60
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl tesoro_initialize_game(undefined1 param_1,uint param_2)

{
  int iVar1;
  void *pvVar2;
  uint *puVar3;
  undefined4 *this;
  int *piVar4;
  undefined4 uVar5;
  undefined4 unaff_EBX;
  int unaff_EBP;
  undefined4 *puVar6;
  int unaff_ESI;
  char *pcVar7;
  int unaff_EDI;
  undefined4 *unaff_FS_OFFSET;
  ulonglong uVar8;
  undefined4 unaff_retaddr;
  undefined4 uVar9;
  int iVar10;
  undefined1 uVar11;
  byte bVar12;
  undefined1 uVar13;
  undefined2 uVar14;
  undefined4 extraout_var;
  undefined1 *puVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  
  uVar16 = 0xffffffff;
  puVar15 = &LAB_0042707b;
  uVar9 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &stack0xfffffff4;
  tesoro_open_archive(DAT_00442064,s_Tesoro_DAT_0042da9c,s_Tesoro_TRE_0042daa8);
  uVar17 = extraout_var;
  iVar1 = tesoro_find_resource(DAT_00442064,(byte *)s_TABLA1_TBL_0042da90,'\x01');
  tesoro_load_colour_lookup(DAT_00442108,iVar1);
  pvVar2 = (void *)FUN_0040f650(0x40,0);
  if (pvVar2 == (void *)0x0) {
    puVar3 = (uint *)0x0;
  }
  else {
    uVar11 = 0;
    uVar13 = 0;
    uVar14 = 0;
    iVar10 = 0;
    iVar1 = tesoro_find_resource(DAT_00442064,(byte *)s_FUENTES_ANM_0042da84,'\x01');
    puVar3 = tesoro_load_animation_bank
                       (pvVar2,iVar1,iVar10,CONCAT22(uVar14,CONCAT11(uVar13,uVar11)));
  }
  FUN_00411c10(DAT_00442108,puVar3,0);
  puVar6 = &DAT_00442110;
  pcVar7 = s_OBJETOS_TXT_0042d990;
  do {
    this = FUN_0040db40(8,1);
    if (this == (undefined4 *)0x0) {
      piVar4 = (int *)0x0;
    }
    else {
      iVar1 = tesoro_find_resource(DAT_00442064,(byte *)pcVar7,'\x01');
      piVar4 = FUN_00413570(this,iVar1);
    }
    *puVar6 = piVar4;
    pcVar7 = (char *)((byte *)pcVar7 + 0xf);
    puVar6 = puVar6 + 1;
  } while ((int)pcVar7 < 0x42d9cc);
  puVar6 = FUN_0040db40(0x9c,1);
  if (puVar6 == (undefined4 *)0x0) {
    DAT_004421ec = (undefined4 *)0x0;
  }
  else {
    uVar5 = tesoro_find_resource(DAT_00442064,(byte *)s_INTERFACE_LVL_0042da74,'\x01');
    tesoro_load_scene(uVar5,1,unaff_EDI,unaff_ESI,unaff_EBP,unaff_EBX,uVar17,CONCAT44(puVar15,uVar9)
                      ,uVar16,unaff_retaddr,param_1,param_2);
    *puVar6 = &PTR_FUN_0042bc40;
    DAT_004421ec = puVar6;
  }
  iVar1 = FUN_0041ba10(0x2b0);
  if (iVar1 == 0) {
    DAT_004420b8 = (void *)0x0;
  }
  else {
    DAT_004420b8 = (void *)FUN_004060b0(iVar1);
  }
  uVar17 = 0xffffffff;
  pcVar7 = s_OBJETOS_DLG_0042d9d0;
  puVar6 = &DAT_00442110;
  do {
    uVar11 = (undefined1)*puVar6;
    uVar13 = (undefined1)((uint)*puVar6 >> 8);
    iVar10 = tesoro_find_resource(DAT_00442064,(byte *)pcVar7,'\x01');
    tesoro_register_script_blocks(DAT_004420b8,iVar10,CONCAT11(uVar13,uVar11));
    puVar6 = puVar6 + 1;
    pcVar7 = (char *)((byte *)pcVar7 + 0xf);
  } while ((int)puVar6 < 0x442120);
  FUN_00406110((int)DAT_004420b8);
  puVar6 = FUN_0040db40(0x114,1);
  if (puVar6 == (undefined4 *)0x0) {
    DAT_004420fc = (int *)0x0;
  }
  else {
    DAT_004420fc = tesoro_initialize_jim(puVar6);
  }
  FUN_0040f900(DAT_00442104,DAT_004421ec,0x13);
  puVar6 = FUN_0040db40(0x6c,1);
  if (puVar6 == (undefined4 *)0x0) {
    _DAT_004421f8 = (undefined4 *)0x0;
  }
  else {
    _DAT_004421f8 = FUN_00415230(puVar6);
  }
  FUN_0040f900(DAT_00442104,_DAT_004421f8,100);
  FUN_0040fbb0(DAT_00442104);
  FUN_00408540((int)DAT_004421ec);
  iVar10 = 0;
  do {
    FUN_0040dd30(DAT_00442100,2);
    if (iVar10 == 3) {
      puVar6 = FUN_0040db40(0x9c,1);
      if (puVar6 == (undefined4 *)0x0) {
        puVar6 = (undefined4 *)0x0;
      }
      else {
        uVar16 = tesoro_find_resource(DAT_00442064,(byte *)s_CUBIERTA_LVL_0042d6f5,'\x01');
        tesoro_load_scene(uVar16,2,unaff_EDI,unaff_ESI,unaff_EBP,unaff_EBX,iVar1,
                          CONCAT44(puVar15,uVar9),uVar17,unaff_retaddr,param_1,param_2);
        *puVar6 = &PTR_FUN_0042bbd0;
      }
      piVar4 = (int *)&DAT_0044213c;
      DAT_0044213c = puVar6;
    }
    else if (iVar10 == 8) {
      puVar6 = FUN_0040db40(0x9c,1);
      if (puVar6 == (undefined4 *)0x0) {
        piVar4 = (int *)&DAT_00442150;
        DAT_00442150 = (undefined4 *)0x0;
      }
      else {
        uVar16 = tesoro_find_resource(DAT_00442064,(byte *)s_MAPA_LVL_0042d740,'\x01');
        tesoro_load_scene(uVar16,2,unaff_EDI,unaff_ESI,unaff_EBP,unaff_EBX,iVar1,
                          CONCAT44(puVar15,uVar9),uVar17,unaff_retaddr,param_1,param_2);
        *puVar6 = &PTR_FUN_0042bc08;
        piVar4 = (int *)&DAT_00442150;
        DAT_00442150 = puVar6;
      }
    }
    else if (s_DESPACHO_LVL_0042d6c8[iVar10 * 0xf] == '\0') {
      piVar4 = (int *)(&DAT_00442130 + iVar10);
      *piVar4 = 0;
    }
    else {
      puVar6 = FUN_0040db40(0x9c,1);
      if (puVar6 == (undefined4 *)0x0) {
        piVar4 = (int *)(&DAT_00442130 + iVar10);
        *piVar4 = 0;
      }
      else {
        uVar11 = 2;
        uVar13 = 0;
        uVar14 = 0;
        uVar16 = tesoro_find_resource
                           (DAT_00442064,(byte *)(s_DESPACHO_LVL_0042d6c8 + iVar10 * 0xf),'\x01');
        puVar6 = tesoro_load_scene(uVar16,CONCAT22(uVar14,CONCAT11(uVar13,uVar11)),unaff_EDI,
                                   unaff_ESI,unaff_EBP,unaff_EBX,iVar1,CONCAT44(puVar15,uVar9),
                                   uVar17,unaff_retaddr,param_1,param_2);
        piVar4 = (int *)(&DAT_00442130 + iVar10);
        *piVar4 = (int)puVar6;
      }
    }
    if (s_DESPACHO_LVL_0042d6c8[iVar10 * 0xf] != '\0') {
      *(int *)(*piVar4 + 0x98) = iVar10;
    }
    iVar10 = iVar10 + 1;
  } while (iVar10 < 0x2f);
  puVar6 = FUN_0040db40(0x9c,1);
  if (puVar6 == (undefined4 *)0x0) {
    DAT_004421f0 = (undefined4 *)0x0;
  }
  else {
    uVar11 = 2;
    uVar13 = 0;
    uVar14 = 0;
    uVar16 = tesoro_find_resource(DAT_00442064,(byte *)s_PRINCIPAL_LVL_0042da64,'\x01');
    DAT_004421f0 = tesoro_load_scene(uVar16,CONCAT22(uVar14,CONCAT11(uVar13,uVar11)),unaff_EDI,
                                     unaff_ESI,unaff_EBP,unaff_EBX,iVar1,CONCAT44(puVar15,uVar9),
                                     uVar17,unaff_retaddr,param_1,param_2);
  }
  puVar6 = FUN_0040db40(0x9c,1);
  if (puVar6 == (undefined4 *)0x0) {
    DAT_004421f4 = (undefined4 *)0x0;
  }
  else {
    uVar11 = 2;
    uVar13 = 0;
    uVar14 = 0;
    uVar16 = tesoro_find_resource(DAT_00442064,(byte *)s_NUEVO_LVL_0042da58,'\x01');
    DAT_004421f4 = tesoro_load_scene(uVar16,CONCAT22(uVar14,CONCAT11(uVar13,uVar11)),unaff_EDI,
                                     unaff_ESI,unaff_EBP,unaff_EBX,iVar1,CONCAT44(puVar15,uVar9),
                                     uVar17,unaff_retaddr,param_1,param_2);
  }
  FUN_00408540((int)DAT_004421f0);
  FUN_00408540((int)DAT_004421f4);
  piVar4 = (int *)&DAT_00442130;
  do {
    if (*piVar4 != 0) {
      FUN_00408540(*piVar4);
    }
    piVar4 = piVar4 + 1;
  } while ((int)piVar4 < 0x4421ec);
  uVar8 = tesoro_encode_name(0x42da50);
  bVar12 = 0;
  pvVar2 = (void *)tesoro_find_scene_object(DAT_00442130,(int)uVar8,(int)(uVar8 >> 0x20));
  tesoro_set_object_flag_bit2(pvVar2,bVar12);
  uVar8 = tesoro_encode_name(0x42da44);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442140,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42d4d8);
  piVar4 = (int *)tesoro_find_scene_object(DAT_0044214c,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42da3c);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442150,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42da34);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442150,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42d2e8);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442158,(int)uVar8,(int)(uVar8 >> 0x20));
  uVar9 = 0;
  (**(code **)(*piVar4 + 0x20))();
  uVar8 = tesoro_encode_name(0x42da28);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442174,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42da1c);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442174,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42da14);
  bVar12 = 0;
  pvVar2 = (void *)tesoro_find_scene_object(DAT_00442154,(int)uVar8,(int)(uVar8 >> 0x20));
  tesoro_set_object_flag_bit2(pvVar2,bVar12);
  uVar8 = tesoro_encode_name(0x42da14);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442154,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  uVar8 = tesoro_encode_name(0x42da0c);
  piVar4 = (int *)tesoro_find_scene_object(DAT_00442158,(int)uVar8,(int)(uVar8 >> 0x20));
  (**(code **)(*piVar4 + 0x20))(0);
  (**(code **)(*DAT_004420fc + 0x20))(0);
  tesoro_place_jim_in_scene(DAT_004420fc,DAT_004421f0,DAT_004421f0[0x1c],0,0);
  tesoro_play_external_movie((int)DAT_00442108);
  *unaff_FS_OFFSET = uVar9;
  return;
}

