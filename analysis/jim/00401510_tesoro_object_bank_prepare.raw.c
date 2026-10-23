/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401510
 */


void __fastcall tesoro_object_bank_prepare(int *param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  LONG local_8 [2];
  
  if (param_1[5] == 2) {
    iVar3 = 0;
    piVar1 = *(int **)(DAT_00442064 + 4);
    if ((short)param_1[8] != 0) {
      do {
        local_8[0] = *(LONG *)(param_1[1] + iVar3 * 4);
        local_8[1] = 0;
        FUN_0041b600(piVar1,local_8);
        piVar2 = tesoro_load_image_buffer(piVar1,param_1[5]);
        iVar3 = iVar3 + 1;
        *(int **)(*param_1 + -4 + iVar3 * 4) = piVar2;
      } while (iVar3 < (int)(uint)*(ushort *)(param_1 + 8));
    }
  }
  return;
}

