/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418cb0
 */


undefined4 __thiscall f418cb0(void *this,int param_1)

{
  void *this_00;
  ulonglong uVar1;
  ulonglong uVar2;
  int iVar3;
  char *pcVar4;
  void *pvVar5;
  
  if (param_1 == 4) {
    uVar1 = tesoro_encode_name(0x42e8e4);
    pcVar4 = s_FIJARRALLENA_0042e8c0;
  }
  else {
    if (param_1 != 5) {
      return 1;
    }
    uVar1 = tesoro_encode_name(0x42e8e4);
    pcVar4 = s_FIDOBLON_0042e87c;
  }
  uVar2 = tesoro_encode_name((int)pcVar4);
  pvVar5 = *(void **)((int)this + 0x40);
  iVar3 = *(int *)((int)this + 0x60);
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar2,(int)(uVar2 >> 0x20));
  tesoro_start_script(this_00,(int)uVar1,(int)(uVar1 >> 0x20),iVar3,pvVar5);
  return 1;
}

