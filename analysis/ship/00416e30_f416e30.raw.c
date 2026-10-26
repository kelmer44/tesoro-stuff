/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416e30
 */


undefined4 __thiscall f416e30(void *this,uint param_1)

{
  void *pvVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  char *pcVar5;
  void *pvVar6;
  
  if (param_1 != 0x28) {
    if ((param_1 & 1) == 0) {
      pcVar5 = s_default2_0042e614;
    }
    else {
      pcVar5 = s_default1_0042e620;
    }
    uVar2 = tesoro_encode_name((int)pcVar5);
    uVar3 = tesoro_encode_name(0x42e604);
    pvVar6 = *(void **)((int)this + 0x40);
    iVar4 = 0;
    pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar6);
    return 1;
  }
  uVar2 = tesoro_encode_name(0x42e5fc);
  uVar3 = tesoro_encode_name(0x42e604);
  pvVar6 = *(void **)((int)this + 0x40);
  iVar4 = 0;
  pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar6);
  return 3;
}

