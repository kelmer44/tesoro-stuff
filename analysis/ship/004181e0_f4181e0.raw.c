/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004181e0
 */


undefined4 __thiscall f4181e0(void *this,int param_1)

{
  undefined4 uVar1;
  void *this_00;
  ulonglong uVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  int iVar5;
  void *pvVar6;
  
  if (param_1 == 0x1a) {
    uVar4 = tesoro_encode_name(0x42e7d0);
    uVar2 = tesoro_encode_name(0x42e7dc);
    uVar3 = tesoro_encode_name(0x42e7b4);
    pvVar6 = *(void **)((int)this + 0x40);
    iVar5 = tesoro_find_scene_object(DAT_004421ec,(int)uVar4,(int)(uVar4 >> 0x20));
    iVar5 = *(int *)(iVar5 + 0x60);
  }
  else {
    if (param_1 != 0x1b) {
      uVar1 = FUN_0040f480(this,param_1);
      return uVar1;
    }
    uVar2 = tesoro_encode_name(0x42e7dc);
    uVar3 = tesoro_encode_name(0x42e78c);
    pvVar6 = *(void **)((int)this + 0x40);
    iVar5 = 0;
  }
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this_00,(int)uVar2,(int)(uVar2 >> 0x20),iVar5,pvVar6);
  return 3;
}

