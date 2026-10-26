/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418670
 */


undefined4 __thiscall f418670(void *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  void *this_00;
  ulonglong uVar3;
  ulonglong uVar4;
  char *pcVar5;
  void *pvVar6;
  
  if (**(char **)((int)this + 0x60) < '\x04') {
    uVar3 = tesoro_encode_name(0x42d32c);
    iVar1 = tesoro_find_scene_object(DAT_00442130,(int)uVar3,(int)(uVar3 >> 0x20));
    if (**(char **)(iVar1 + 0x60) != '\0') {
      **(undefined1 **)((int)this + 0x60) = 4;
    }
  }
  if (**(char **)((int)this + 0x60) < '\x06') {
    uVar3 = tesoro_encode_name(0x42e830);
    iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
    if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) != 0) {
      **(undefined1 **)((int)this + 0x60) = 6;
    }
  }
  if (param_1 == 0) {
    uVar3 = tesoro_encode_name(0x42e848);
    pcVar5 = s_FIBOLI_0042e5ec;
  }
  else {
    if (param_1 != 7) {
      if (param_1 != 0x2d) {
        uVar2 = FUN_0040f480(this,param_1);
        return uVar2;
      }
      uVar3 = tesoro_encode_name(0x42e848);
      uVar4 = tesoro_encode_name(0x42e83c);
      pvVar6 = *(void **)((int)this + 0x40);
      iVar1 = *(int *)((int)this + 0x60);
      goto LAB_0041875b;
    }
    uVar3 = tesoro_encode_name(0x42e848);
    pcVar5 = s_FICNUDO_0042e544;
  }
  uVar4 = tesoro_encode_name((int)pcVar5);
  pvVar6 = *(void **)((int)this + 0x40);
  iVar1 = 0;
LAB_0041875b:
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this_00,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar6);
  return 1;
}

