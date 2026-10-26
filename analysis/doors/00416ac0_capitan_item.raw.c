/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416ac0
 */


undefined4 __thiscall capitan_item(void *this,int param_1)

{
  undefined4 uVar1;
  int iVar2;
  void *this_00;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  
  if (param_1 != 0) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  uVar3 = tesoro_encode_name(0x42e5e4);
  iVar2 = tesoro_find_scene_object(DAT_00442138,(int)uVar3,(int)(uVar3 >> 0x20));
  *(bool *)(*(int *)((int)this + 0x60) + 1) = *(char *)(*(int *)(iVar2 + 0x60) + 4) != '\0';
  uVar3 = tesoro_encode_name(0x42e5f4);
  uVar4 = tesoro_encode_name(0x42e5ec);
  pvVar5 = *(void **)((int)this + 0x40);
  iVar2 = *(int *)((int)this + 0x60);
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this_00,(int)uVar3,(int)(uVar3 >> 0x20),iVar2,pvVar5);
  return 3;
}

