/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408030
 */


void __fastcall tesoro_jim_virtual_78(void *param_1)

{
  int iVar1;
  
  iVar1 = *(int *)((int)param_1 + 0x10c);
  if (iVar1 == 3) {
    iVar1 = 0;
  }
  else if (iVar1 == 4) {
    iVar1 = 1;
  }
  else if (iVar1 == 2) {
    iVar1 = 2;
  }
  else {
    if (iVar1 != 1) goto LAB_00408061;
    iVar1 = 3;
  }
  tesoro_recruiter_virtual_78(param_1,iVar1);
LAB_00408061:
  *(int *)((int)param_1 + 0xa4) =
       (0xbe - ((int)*(short *)(*(int *)(*(int *)((int)param_1 + 0x38) + 0x1c) + 0xe +
                               (uint)*(ushort *)(*(int *)((int)param_1 + 0x38) + 0x30) * 0x24) *
                *(int *)((int)param_1 + 0xfc) >> 8)) - *(int *)((int)param_1 + 0x50);
  return;
}

