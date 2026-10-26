/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418780
 */


undefined4 __thiscall f418780(void *this,int param_1)

{
  undefined4 uVar1;
  void *this_00;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  void *pvVar5;
  
  if (param_1 != 0x2b) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  uVar2 = tesoro_encode_name(0x42e860);
  uVar3 = tesoro_encode_name(0x42e854);
  pvVar5 = *(void **)((int)this + 0x40);
  iVar4 = 0;
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this_00,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar5);
  return 1;
}

