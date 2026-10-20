/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004132b0
 */


uint __fastcall tesoro_play_external_movie(int param_1)

{
  LPVOID *ppv;
  byte bVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  undefined4 uVar5;
  int *piStack_3e4;
  undefined **ppuStack_3e0;
  undefined1 **ppuStack_3dc;
  int *piStack_3d8;
  undefined4 uStack_3d4;
  undefined4 uStack_3d0;
  int *piStack_3cc;
  int **ppiStack_3c8;
  int *piStack_3c4;
  undefined *puStack_3c0;
  undefined1 *puStack_3bc;
  int *piStack_3b8;
  int *piStack_3b4;
  undefined1 local_334 [244];
  WCHAR aWStack_240 [288];
  
  FUN_0041c140(local_334,s__c__INSTALL_BIN_DATA_02i_BIN_0042d670);
  CoInitialize((LPVOID)0x0);
  ppv = (LPVOID *)(param_1 + 0x18);
  uVar2 = CoCreateInstance((IID *)&DAT_0042c218,(LPUNKNOWN)0x0,1,(IID *)&DAT_0042c228,ppv);
  if ((int)uVar2 < 0) {
    return uVar2 & 0xffffff00;
  }
                    /* WARNING: Load size is inaccurate */
  uVar2 = (**(code **)(**ppv + 0x30))();
  if ((int)uVar2 < 0) {
    return uVar2 & 0xffffff00;
  }
                    /* WARNING: Load size is inaccurate */
  uVar2 = (**(code **)(**ppv + 0x3c))();
  if ((int)uVar2 < 0) {
    return uVar2 & 0xffffff00;
  }
                    /* WARNING: Load size is inaccurate */
  uVar2 = (**(code **)(**ppv + 0x3c))();
  if ((int)uVar2 < 0) {
    return uVar2 & 0xffffff00;
  }
  piStack_3b4 = (int *)0x413399;
  MultiByteToWideChar(0,0,&stack0xfffffc94,-1,aWStack_240,0x104);
  piVar4 = (int *)*ppv;
  uVar2 = (**(code **)(*piVar4 + 0x40))();
  if ((int)uVar2 < 0) {
    return uVar2 & 0xffffff00;
  }
  *(LPVOID *)(param_1 + 0x14) = *ppv;
                    /* WARNING: Load size is inaccurate */
  uVar2 = (**(code **)(**ppv + 4))();
  if ((int *)*ppv == (int *)0x0) {
    return uVar2 & 0xffffff00;
  }
                    /* WARNING: Load size is inaccurate */
  (**(code **)(**ppv + 8))();
  piStack_3b8 = *(int **)(param_1 + 0x14);
  piStack_3b4 = (int *)&DAT_0042c208;
  puStack_3bc = (undefined1 *)0x413404;
  piStack_3c4 = piVar4;
  uVar2 = (**(code **)(*piStack_3b8 + 0x10))();
  if ((int)uVar2 < 0) {
    return uVar2 & 0xffffff00;
  }
  puStack_3bc = &stack0xfffffc58;
  puStack_3c0 = &DAT_0042c1e8;
  ppiStack_3c8 = (int **)0x413429;
  uVar2 = (**(code **)*piStack_3c4)();
  if (-1 < (int)uVar2) {
    ppiStack_3c8 = &piStack_3b8;
    piStack_3cc = (int *)0x0;
    uStack_3d0 = 0;
    uStack_3d4 = 0;
    piStack_3d8 = piStack_3b4;
    ppuStack_3dc = (undefined1 **)0x41344d;
    uVar2 = (**(code **)(*piStack_3b4 + 0x34))();
    if ((int)uVar2 < 0) {
      return uVar2 & 0xffffff00;
    }
    ppuStack_3dc = &puStack_3bc;
    ppuStack_3e0 = &puStack_3c0;
    piStack_3e4 = piStack_3cc;
    uVar2 = (**(code **)(*piStack_3cc + 0x20))();
    if (-1 < (int)uVar2) {
      uVar5 = 1;
      uVar2 = (**(code **)(**(int **)(param_1 + 0x14) + 0x1c))(*(int **)(param_1 + 0x14),1);
      if (-1 < (int)uVar2) {
        while( true ) {
          (**(code **)*DAT_00442074)();
          piVar4 = (int *)0x0;
          iVar3 = (**(code **)(*ppuStack_3e0 + 0x18))(ppuStack_3e0,0,0,0,0);
          if ((iVar3 != 0) || ((*(byte *)DAT_00442074[0xd] & 1) != 0)) break;
          (**(code **)(**(int **)(DAT_00442108 + 8) + 0x14))
                    (*(int **)(DAT_00442108 + 8),&piStack_3e4,uVar5,&piStack_3e4,0x1000000,0);
        }
        (**(code **)(**(int **)(param_1 + 0x14) + 0x1c))(*(int **)(param_1 + 0x14),0);
        (**(code **)(*piVar4 + 8))(piVar4);
        (**(code **)(*piVar4 + 8))(piVar4);
        (**(code **)(*piVar4 + 8))(piVar4);
        (**(code **)(**(int **)(param_1 + 0x14) + 8))(*(int **)(param_1 + 0x14));
        CoUninitialize();
        bVar1 = *(byte *)DAT_00442074[0xd];
        while ((bVar1 & 0x3c) != 0) {
          (**(code **)*DAT_00442074)();
          bVar1 = *(byte *)DAT_00442074[0xd];
        }
        uVar5 = FUN_004139e0(500);
        return CONCAT31((int3)((uint)uVar5 >> 8),1);
      }
      return uVar2 & 0xffffff00;
    }
    return uVar2 & 0xffffff00;
  }
  return uVar2 & 0xffffff00;
}

