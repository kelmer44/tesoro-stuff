/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00405850
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __fastcall tesoro_script_update(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  iVar1 = param_1[0x33];
  iVar2 = param_1[0x2f] + iVar1 * 0x18;
  switch(*(undefined4 *)(param_1[0x2f] + iVar1 * 0x18)) {
  case 1:
    if (*(char *)(param_1[0x2b] + 0xd4) != '\0') goto switchD_00405874_caseD_3;
    if (DAT_004420c8 == 0) {
      DAT_004420c8 = tesoro_current_frame_time(DAT_00442120);
    }
    iVar2 = tesoro_current_frame_time(DAT_00442120);
    if ((uint)(iVar2 - DAT_004420c8) < 0xc9) goto switchD_00405874_caseD_3;
    break;
  case 2:
    iVar3 = tesoro_current_frame_time(DAT_00442120);
    if ((uint)(iVar3 - param_1[0x2a]) < *(uint *)(iVar2 + 4)) goto switchD_00405874_caseD_3;
    break;
  default:
    goto switchD_00405874_caseD_3;
  case 5:
  case 6:
    if (DAT_004420c0 == '\0') {
      _DAT_004420c4 = (uint)*(ushort *)(DAT_00442128[0xe] + 0x30);
      iVar2 = DAT_00442128[0x4b];
      if (iVar2 != 0) {
        *(uint *)(iVar2 + 0x68) = *(uint *)(iVar2 + 0x68) | 4;
      }
      DAT_00442128[0x4b] = 0;
      FUN_00401de0(DAT_00442128,0,0,0,0,'\0');
      DAT_004420bc = FUN_00407d00(0x442090);
      if (-1 < DAT_004420bc) {
        DAT_004420c0 = '\x01';
        param_1[0x2b] = (int)DAT_004420fc;
        FUN_00410330(DAT_004420fc,(&DAT_00442090)[DAT_004420bc],
                     (uint)*(ushort *)((int)DAT_004420fc + 0xd0),0);
      }
    }
    else if (*(char *)((int)DAT_004420fc + 0xd4) == '\0') {
      *(undefined2 *)(DAT_00442128[0xe] + 0x30) = DAT_004420c4;
      param_1[0x33] = (&DAT_004420d0)[DAT_004420bc];
      tesoro_execute_script(param_1);
      return 3;
    }
    goto switchD_00405874_caseD_3;
  case 8:
    (**(code **)(*DAT_00442128 + 0x20))(0);
    break;
  case 9:
    if ((*(int *)(iVar2 + 0x10) != 0) ||
       ((**(code **)(*(int *)param_1[0x2b] + 0xc))(),
       (*(byte *)(*(int *)(param_1[0x2b] + 0x38) + 0x18) & 1) != 0)) goto switchD_00405874_caseD_3;
  }
  param_1[0x33] = param_1[0x33] + 1;
switchD_00405874_caseD_3:
  if (param_1[0x33] != iVar1) {
    uVar4 = tesoro_execute_script(param_1);
    return uVar4;
  }
  return 2;
}

