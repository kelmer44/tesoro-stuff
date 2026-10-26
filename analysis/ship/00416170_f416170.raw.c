/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416170
 */


undefined4 __fastcall f416170(int param_1)

{
  void *pvVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  void *pvVar5;
  
  if (*(char *)(DAT_004420fc + 0x111) == '\x02') {
    *(undefined1 *)(DAT_004420fc + 0x110) = 0;
    *(undefined1 *)(DAT_004420fc + 0x111) = 3;
    uVar2 = tesoro_encode_name(0x42d4b4);
    uVar3 = tesoro_encode_name(0x42e4ec);
    pvVar5 = *(void **)(param_1 + 0x40);
    iVar4 = 0;
    pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar5);
    return 1;
  }
  uVar2 = tesoro_encode_name(0x42e4e4);
  uVar3 = tesoro_encode_name(0x42e4ec);
  pvVar5 = *(void **)(param_1 + 0x40);
  iVar4 = 0;
  pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar5);
  return 0;
}

