/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407790
 */


undefined4 __fastcall tesoro_jim_virtual_10(int *param_1)

{
  short sVar1;
  void *pvVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  if (*(char *)((int)param_1 + 0xd5) == '\0') {
    pvVar2 = *(void **)(*(int *)(*(int *)(DAT_004420fc + 0x40) + 0x90) + 0x78);
    if (pvVar2 == (void *)0x0) {
      param_1[0x3f] = 0x100;
      uVar4 = tesoro_recruiter_virtual_10(param_1);
      return uVar4;
    }
    iVar7 = FUN_00414740(pvVar2,param_1[0x13],param_1[0x14]);
    param_1[0x3f] = iVar7;
    uVar4 = tesoro_recruiter_virtual_10(param_1);
    return uVar4;
  }
  iVar8 = param_1[0x41] - (param_1[0x38] >> 8);
  iVar3 = param_1[0x42] - (param_1[0x39] >> 8);
  iVar5 = iVar8 * iVar8 + iVar3 * iVar3;
  FUN_00401860(param_1[0xe]);
  iVar7 = param_1[0xe];
  sVar1 = *(short *)(iVar7 + 0x2a);
  iVar9 = ((int)*(short *)(iVar7 + 0x28) - param_1[0x3a]) * param_1[0x3f];
  iVar6 = param_1[0x3b];
  param_1[0x3a] = (int)*(short *)(iVar7 + 0x28);
  param_1[0x3b] = (int)*(short *)(iVar7 + 0x2a);
  iVar7 = (sVar1 - iVar6) * param_1[0x3f];
  (**(code **)(*param_1 + 8))();
  if (param_1[0x43] < 3) {
    if (iVar3 != 0) {
      iVar9 = iVar9 + (iVar7 * iVar8) / iVar3;
    }
  }
  else if (iVar8 != 0) {
    iVar7 = iVar7 + (iVar9 * iVar3) / iVar8;
  }
  param_1[0x39] = param_1[0x39] + iVar7;
  param_1[0x38] = param_1[0x38] + iVar9;
  if (iVar5 == 0) {
    param_1[0x13] = param_1[0x41];
    param_1[0x14] = param_1[0x42];
    if (param_1[0x3d] == 0) {
      FUN_0040fb20(DAT_00442104,(int)param_1);
      (**(code **)(*param_1 + 0x74))(param_1[0x43]);
    }
    else {
      FUN_0040fb20(DAT_00442104,(int)param_1);
      iVar7 = (**(code **)(*(int *)param_1[0x3d] + 0x34))();
      if (iVar7 == 2) {
        return 2;
      }
      if (iVar7 == 3) {
        return 3;
      }
    }
  }
  else {
    if (iVar5 < (iVar9 >> 8) * (iVar9 >> 8) + (iVar7 >> 8) * (iVar7 >> 8)) {
      iVar7 = param_1[0x41];
      iVar6 = param_1[0x42];
      param_1[0x38] = iVar7 << 8;
      param_1[0x39] = iVar6 << 8;
    }
    else {
      iVar7 = param_1[0x38] >> 8;
      iVar6 = param_1[0x39] >> 8;
    }
    FUN_00401d30(param_1,iVar7,iVar6,'\0');
  }
  pvVar2 = *(void **)(*(int *)(*(int *)(DAT_004420fc + 0x40) + 0x90) + 0x78);
  if (pvVar2 == (void *)0x0) {
    param_1[0x3f] = 0x100;
    return 1;
  }
  iVar7 = FUN_00414740(pvVar2,param_1[0x13],param_1[0x14]);
  param_1[0x3f] = iVar7;
  return 1;
}

