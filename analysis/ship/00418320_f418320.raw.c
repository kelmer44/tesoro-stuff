/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418320
 */


undefined4 __thiscall f418320(void *this,int param_1)

{
  undefined4 uVar1;
  int iVar2;
  void *this_00;
  ulonglong uVar3;
  ulonglong uVar4;
  longlong lVar5;
  
  if (param_1 != 0x26) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  uVar3 = tesoro_encode_name(0x42d4fc);
  iVar2 = tesoro_find_scene_object(DAT_00442154,(int)uVar3,(int)(uVar3 >> 0x20));
  if (*(char *)(*(int *)(iVar2 + 0x60) + 1) == '\0') {
    uVar1 = FUN_0040f480(this,0x26);
    return uVar1;
  }
  uVar3 = tesoro_encode_name(0x42d4fc);
  iVar2 = tesoro_find_scene_object(DAT_00442154,(int)uVar3,(int)(uVar3 >> 0x20));
  *(undefined1 *)(*(int *)(iVar2 + 0x60) + 0xd) = 1;
  uVar3 = tesoro_encode_name(0x42e804);
  uVar4 = tesoro_encode_name(0x42e7fc);
  lVar5 = (ulonglong)*(uint *)((int)this + 0x40) << 0x20;
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this_00,(int)uVar3,(int)(uVar3 >> 0x20),(int)lVar5,
                      (void *)((ulonglong)lVar5 >> 0x20));
  return 3;
}

