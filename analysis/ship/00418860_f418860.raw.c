/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418860
 */


undefined4 __thiscall f418860(void *this,undefined4 param_1)

{
  int iVar1;
  void *this_00;
  ulonglong uVar2;
  ulonglong uVar3;
  char *pcVar4;
  void *pvVar5;
  
  switch(param_1) {
  case 0:
    uVar2 = tesoro_encode_name(0x42e54c);
    pcVar4 = s_FIBOLI_0042e5ec;
    break;
  default:
    goto switchD_0041887a_caseD_1;
  case 3:
    uVar2 = tesoro_encode_name(0x42e874);
    iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
    if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) != 0) {
      **(undefined1 **)((int)this + 0x60) = 2;
    }
    uVar2 = tesoro_encode_name(0x42e54c);
    pcVar4 = s_FIJARRA_0042e888;
    goto LAB_004188f4;
  case 5:
    uVar2 = tesoro_encode_name(0x42e54c);
    pcVar4 = s_FIDOBLON_0042e87c;
LAB_004188f4:
    uVar3 = tesoro_encode_name((int)pcVar4);
    pvVar5 = *(void **)((int)this + 0x40);
    iVar1 = *(int *)((int)this + 0x60);
    goto LAB_0041892a;
  case 0x2d:
    uVar2 = tesoro_encode_name(0x42e54c);
    pcVar4 = s_FIHLFDBLN_0042e83c;
  }
  uVar3 = tesoro_encode_name((int)pcVar4);
  pvVar5 = *(void **)((int)this + 0x40);
  iVar1 = 0;
LAB_0041892a:
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this_00,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar5);
switchD_0041887a_caseD_1:
  return 1;
}

