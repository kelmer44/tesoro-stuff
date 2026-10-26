/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00417640
 */


undefined4 __fastcall f417640(int param_1)

{
  void *pvVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  char *pcVar5;
  void *pvVar6;
  
  iVar4 = *(int *)(DAT_004420fc + 0x40);
  if (iVar4 == DAT_0044218c) {
    if (**(char **)(param_1 + 0x60) != '\0') {
      uVar2 = tesoro_encode_name(0x42e73c);
      uVar3 = tesoro_encode_name(0x42e730);
      pvVar6 = *(void **)(param_1 + 0x40);
      iVar4 = 0;
      pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
      tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar6);
    }
    **(undefined1 **)(param_1 + 0x60) = 1;
    return 1;
  }
  if (iVar4 == DAT_004421b8) {
    uVar2 = tesoro_encode_name(0x42e724);
    pcVar5 = s_FPUERTALAB_0042e730;
  }
  else {
    if (iVar4 != DAT_004421dc) {
      return 1;
    }
    uVar2 = tesoro_encode_name(0x42d4b4);
    pcVar5 = s_FANMLGTES_0042e718;
  }
  uVar3 = tesoro_encode_name((int)pcVar5);
  pvVar6 = *(void **)(param_1 + 0x40);
  iVar4 = 0;
  pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar6);
  return 1;
}

