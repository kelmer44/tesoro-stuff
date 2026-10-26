/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407d00
 */


int tesoro_mouse_407d00(int param_1)

{
  int iVar1;
  int iVar2;
  void *this;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *unaff_retaddr;
  int iStack_d0;
  int local_cc [51];
  
  tesoro_cursor_text(DAT_00442128,(char *)0x0,-1);
  DAT_00442128[0x13] = *(int *)(DAT_0044206c + 0x38) + 0x140;
  DAT_00442128[0x14] = 0xf0 - *(int *)(DAT_0044206c + 0x3c);
  (**(code **)(*DAT_00442128 + 0x18))(*(undefined4 *)(DAT_004421ec + 0x80));
  iVar4 = *(int *)((int)DAT_00442108 + 0x13a0);
  iVar1 = 0x32;
  iVar2 = 0;
  if (0 < param_1) {
    iVar3 = param_1;
    piVar5 = unaff_retaddr;
    do {
      *(int *)(((int)local_cc - (int)unaff_retaddr) + (int)piVar5) = iVar1;
      FUN_00411c80(DAT_00442108,*(byte **)(*piVar5 + 0xc),0xffff,0,0x140,iVar1,0x280,5,'\0',0x12);
      piVar5 = piVar5 + 1;
      iVar1 = *(int *)(*(int *)((int)DAT_00442108 + 0x13a0) * 0x70 + 0x678 + (int)DAT_00442108) +
              0x1e;
      iVar3 = iVar3 + -1;
      iVar2 = param_1;
    } while (iVar3 != 0);
  }
  local_cc[iVar2] = 0x1e0;
  iVar1 = 0x1d1 - iVar1;
  if (iVar4 < *(int *)((int)DAT_00442108 + 0x13a0)) {
    iVar2 = iVar4 * 0x70;
    do {
      iVar4 = iVar4 + 1;
      *(int *)((int)DAT_00442108 + iVar2 + 0x6e8) =
           *(int *)((int)DAT_00442108 + iVar2 + 0x6e8) + iVar1;
      iVar2 = iVar2 + 0x70;
    } while (iVar4 < *(int *)((int)DAT_00442108 + 0x13a0));
  }
  iVar4 = 0;
  local_cc[0] = local_cc[0] + iVar1;
  if (param_1 < 1) {
    return -1;
  }
  piVar5 = local_cc;
  this = DAT_00442108;
  do {
    iVar2 = piVar5[1];
    iVar3 = *(int *)(DAT_0044206c + 0x3c);
    piVar5[1] = iVar1 + iVar2;
    if ((*piVar5 + -0xf0 <= iVar3) && (iVar3 < iVar1 + iVar2 + -0xf0)) {
      if ((**(byte **)(DAT_0044206c + 0x1c) & 8) != 0) {
        FUN_0040fb20(DAT_00442104,iStack_d0);
        return iVar4;
      }
      FUN_00411c80(this,*(byte **)(*(int *)(((int)unaff_retaddr - (int)local_cc) + (int)piVar5) +
                                  0xc),0xffe0,0,0x140,*piVar5,0x280,5,'\0',0x12);
      this = DAT_00442108;
    }
    iVar4 = iVar4 + 1;
    piVar5 = piVar5 + 1;
  } while (iVar4 < param_1);
  return -1;
}

