/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00417de0
 */


undefined4 __thiscall f417de0(void *this,uint param_1)

{
  undefined4 uVar1;
  int iVar2;
  void *this_00;
  ulonglong uVar3;
  ulonglong uVar4;
  uint *puVar5;
  void *pvVar6;
  
  if (*(void **)((int)this + 0x40) != DAT_0044214c) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  if (param_1 != 0x1b) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  uVar3 = tesoro_encode_name(0x42e7a4);
  iVar2 = tesoro_find_scene_object(DAT_0044214c,(int)uVar3,(int)(uVar3 >> 0x20));
  param_1 = CONCAT31(param_1._1_3_,(char)(*(uint *)(iVar2 + 0x68) >> 1)) & 0xffffff01;
  uVar3 = tesoro_encode_name(0x42e798);
  uVar4 = tesoro_encode_name(0x42e78c);
  pvVar6 = *(void **)((int)this + 0x40);
  puVar5 = &param_1;
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this_00,(int)uVar3,(int)(uVar3 >> 0x20),(int)puVar5,pvVar6);
  return 1;
}

