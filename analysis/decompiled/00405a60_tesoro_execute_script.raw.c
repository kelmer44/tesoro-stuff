/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00405a60
 */


undefined4 __fastcall tesoro_execute_script(int *param_1)

{
  char cVar1;
  short sVar2;
  short sVar3;
  int *this;
  int iVar4;
  int iVar5;
  void *pvVar6;
  ulonglong uVar7;
  undefined4 uVar8;
  int iVar9;
  undefined4 uVar10;
  undefined2 uVar11;
  byte local_10 [16];
  
switchD_00405a83_default:
  iVar4 = param_1[0x33];
  iVar5 = param_1[0x2f] + iVar4 * 0x18;
  switch(*(undefined4 *)(param_1[0x2f] + iVar4 * 0x18)) {
  case 0:
  case 10:
    goto switchD_00405a83_caseD_0;
  case 1:
    DAT_004420c8 = 0;
    iVar4 = *(int *)(iVar5 + 8);
    if (0 < iVar4) {
      iVar9 = iVar4 * 4 + -4;
      if (*(int *)(param_1[0x2c] + iVar9) == 0) {
        uVar8 = tesoro_find_scene_object
                          ((void *)param_1[0x10],*(int *)(param_1[0x2d] + -8 + iVar4 * 8),
                           *(int *)(param_1[0x2d] + -4 + iVar4 * 8));
        *(undefined4 *)(param_1[0x2c] + iVar9) = uVar8;
      }
      pvVar6 = *(void **)(param_1[0x2c] + iVar9);
      param_1[0x2b] = (int)pvVar6;
      FUN_00410330(pvVar6,*(int *)(iVar5 + 4),(uint)*(ushort *)((int)pvVar6 + 0xd0),
                   *(undefined4 *)(iVar5 + 0xc));
      return 3;
    }
    param_1[0x2b] = (int)DAT_004420fc;
    FUN_00407ce0(DAT_004420fc,*(int *)(iVar5 + 4));
    return 3;
  case 2:
    iVar5 = FUN_00413a10(DAT_00442120);
    param_1[0x2a] = iVar5;
    return 2;
  case 3:
    if (param_1[0x28] < 0) {
      FUN_0040fb20(DAT_00442104,(int)param_1);
      (**(code **)(*DAT_00442128 + 0x20))(1);
      iVar5 = 0;
      if (param_1[0x2e] < 1) {
        return 3;
      }
      do {
        iVar4 = *(int *)(param_1[0x2c] + iVar5 * 4);
        if ((iVar4 != 0) && (*(char *)(iVar4 + 0xc) != '\0')) {
          *(undefined1 *)(iVar4 + 0xc) = 0;
          FUN_00401dc0(*(int *)(param_1[0x2c] + iVar5 * 4));
          FUN_00401a70(*(int *)(*(int *)(param_1[0x2c] + iVar5 * 4) + 0x38));
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < param_1[0x2e]);
      return 3;
    }
    param_1[0x33] = param_1[0x28];
    goto switchD_00405a83_default;
  case 4:
    param_1[0x33] = *(int *)(iVar5 + 4);
    goto switchD_00405a83_default;
  case 5:
    param_1[0x28] = param_1[0x33];
    param_1[0x33] = param_1[0x33] + 1;
    DAT_00442088 = 0;
    goto LAB_00405e75;
  case 7:
    if (*(char *)((uint)*(byte *)(iVar5 + 8) + param_1[0x34]) == *(char *)(iVar5 + 9)) {
      param_1[0x33] = *(int *)(iVar5 + 4);
      goto switchD_00405a83_default;
    }
    break;
  case 8:
    if (*(int *)(iVar5 + 4) != 0) {
      tesoro_start_script(*(void **)(iVar5 + 4),*(int *)(iVar5 + 8),*(int *)(iVar5 + 0xc),
                          param_1[0x34],(void *)param_1[0x10]);
      return 3;
    }
    uVar7 = tesoro_encode_name(0x42d204);
    if ((*(int *)(iVar5 + 8) == (int)uVar7) &&
       (uVar7 = tesoro_encode_name(0x42d204), *(int *)(iVar5 + 0xc) == (int)(uVar7 >> 0x20))) {
      tesoro_native_type_20(param_1,0);
      param_1[0x33] = param_1[0x33] + 1;
    }
    else {
      tesoro_native_type_20(param_1,1);
      param_1[0x33] = param_1[0x33] + 1;
    }
    goto switchD_00405a83_default;
  case 9:
    pvVar6 = *(void **)(iVar5 + 0xc);
    if (pvVar6 == (void *)0x0) {
      iVar4 = *(int *)(iVar5 + 8);
      iVar9 = *(int *)(iVar5 + 4);
      pvVar6 = (void *)param_1[0x10];
    }
    else {
      iVar4 = *(int *)(iVar5 + 8);
      iVar9 = *(int *)(iVar5 + 4);
    }
    this = (int *)tesoro_find_scene_object(pvVar6,iVar9,iVar4);
    param_1[0x2b] = (int)this;
    switch(*(undefined4 *)(iVar5 + 0x10)) {
    case 0:
      if (*(char *)(iVar5 + 0x16) == '\0') {
        iVar4 = (int)*(char *)(iVar5 + 0x15);
      }
      else {
        iVar4 = -1;
      }
      FUN_00401de0(this,*(byte *)(iVar5 + 0x14),(short)*(char *)(iVar5 + 0x15),iVar4,
                   *(char *)(iVar5 + 0x16) == '\x02','\0');
      if (*(char *)(iVar5 + 0x16) == '\x03') {
        return 3;
      }
      param_1[0x33] = param_1[0x33] + 1;
      break;
    case 1:
      sVar2 = *(short *)(iVar5 + 0x16);
      sVar3 = *(short *)(iVar5 + 0x14);
      iVar5 = *this;
      param_1[0x2b] = (int)this;
      (**(code **)(iVar5 + 0x80))((int)sVar3,(int)sVar2,param_1,0);
      return 3;
    case 2:
      (**(code **)(*this + 0x74))((int)*(char *)(iVar5 + 0x14));
      param_1[0x33] = param_1[0x33] + 1;
      break;
    case 3:
      (**(code **)(*this + 0x20))(*(undefined1 *)(iVar5 + 0x14));
      param_1[0x33] = param_1[0x33] + 1;
      break;
    case 4:
      tesoro_set_object_flag_bit2(this,*(byte *)(iVar5 + 0x14));
      param_1[0x33] = param_1[0x33] + 1;
      break;
    default:
      goto switchD_00405b8a_default;
    }
    goto switchD_00405a83_default;
  case 0xb:
    *(undefined1 *)((uint)*(byte *)(iVar5 + 4) + param_1[0x34]) = *(undefined1 *)(iVar5 + 5);
    param_1[0x33] = param_1[0x33] + 1;
    goto switchD_00405a83_default;
  case 0xc:
    (**(code **)(*param_1 + 0x70))(*(undefined4 *)(iVar5 + 4));
switchD_00405b8a_default:
    iVar4 = param_1[0x33];
    break;
  case 0xd:
    tesoro_play_external_movie(DAT_00442108);
    param_1[0x33] = param_1[0x33] + 1;
    goto switchD_00405a83_default;
  case 0xe:
    FUN_0041c140(local_10,s__s_WAV_0042d1fc);
    uVar11 = 0;
    uVar10 = 0;
    uVar8 = 0;
    iVar5 = tesoro_find_resource(DAT_00442064,local_10,'\x01');
    FUN_00404110(DAT_00442080,iVar5,uVar8,uVar10,uVar11);
    param_1[0x33] = param_1[0x33] + 1;
    goto switchD_00405a83_default;
  case 0xf:
    goto switchD_00405a83_caseD_f;
  default:
    goto switchD_00405a83_default;
  }
  param_1[0x33] = iVar4 + 1;
  goto switchD_00405a83_default;
switchD_00405a83_caseD_f:
  FUN_00403cb0(*(void **)((int)DAT_00442080 + 0x1c),*(byte *)(iVar5 + 4),1);
  param_1[0x33] = param_1[0x33] + 1;
  goto switchD_00405a83_default;
LAB_00405e75:
  DAT_004420c0 = 0;
  iVar5 = param_1[0x2f] + param_1[0x33] * 0x18;
  cVar1 = *(char *)(iVar5 + 6);
  if (cVar1 < '\0') {
LAB_00405ebf:
    (&DAT_00442090)[DAT_00442088] = *(undefined4 *)(iVar5 + 0x1c);
    (&DAT_004420d0)[DAT_00442088] = param_1[0x33] + 2;
    DAT_00442088 = DAT_00442088 + 1;
  }
  else if (*(char *)(iVar5 + 7) == '\0') {
LAB_00405eb0:
    if (*(char *)(param_1[0x34] + (int)cVar1) == '\x01') goto LAB_00405ebf;
  }
  else {
    if (*(char *)(param_1[0x34] + (int)cVar1) == '\0') goto LAB_00405ebf;
    if (*(char *)(iVar5 + 7) == '\0') goto LAB_00405eb0;
  }
  sVar2 = *(short *)(param_1[0x2f] + 4 + param_1[0x33] * 0x18);
  if (sVar2 < 1) {
    param_1[0x2b] = (int)DAT_004420fc;
    return 3;
  }
  param_1[0x33] = (int)sVar2;
  goto LAB_00405e75;
switchD_00405a83_caseD_0:
  FUN_0040fb20(DAT_00442104,(int)param_1);
  (**(code **)(*DAT_00442128 + 0x20))(1);
  iVar5 = 0;
  if (param_1[0x2e] < 1) {
    return 3;
  }
  do {
    iVar4 = *(int *)(param_1[0x2c] + iVar5 * 4);
    if ((iVar4 != 0) && (*(char *)(iVar4 + 0xc) != '\0')) {
      *(undefined1 *)(iVar4 + 0xc) = 0;
      FUN_00401dc0(*(int *)(param_1[0x2c] + iVar5 * 4));
      FUN_00401a70(*(int *)(*(int *)(param_1[0x2c] + iVar5 * 4) + 0x38));
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < param_1[0x2e]);
  return 3;
}

