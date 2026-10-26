/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418aa0
 */


undefined4 __thiscall f418aa0(void *this,undefined4 param_1)

{
  void *this_00;
  ulonglong uVar1;
  ulonglong uVar2;
  int iVar3;
  char *pcVar4;
  void *pvVar5;
  
  switch(param_1) {
  case 0:
    uVar1 = tesoro_encode_name(0x42e8d0);
    uVar2 = tesoro_encode_name(0x42e5ec);
    pvVar5 = *(void **)((int)this + 0x40);
    iVar3 = 0;
    break;
  default:
    goto switchD_00418ab2_caseD_1;
  case 4:
    uVar1 = tesoro_encode_name(0x42e8d0);
    pcVar4 = s_FIJARRALLENA_0042e8c0;
    goto LAB_00418b33;
  case 5:
    uVar1 = tesoro_encode_name(0x42e8d0);
    uVar2 = tesoro_encode_name(0x42e87c);
    pvVar5 = *(void **)((int)this + 0x40);
    iVar3 = 0;
    break;
  case 6:
    uVar1 = tesoro_encode_name(0x42e8d0);
    pcVar4 = s_FICUERDA_0042e8b4;
LAB_00418b33:
    uVar2 = tesoro_encode_name((int)pcVar4);
    pvVar5 = *(void **)((int)this + 0x40);
    iVar3 = *(int *)((int)this + 0x60);
  }
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar2,(int)(uVar2 >> 0x20));
  tesoro_start_script(this_00,(int)uVar1,(int)(uVar1 >> 0x20),iVar3,pvVar5);
switchD_00418ab2_caseD_1:
  return 1;
}

