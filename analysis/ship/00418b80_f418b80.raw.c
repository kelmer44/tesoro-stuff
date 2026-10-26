/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00418b80
 */


undefined4 __fastcall f418b80(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  uVar2 = tesoro_encode_name(0x42e8a0);
  iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
  if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) == 0) {
    uVar2 = tesoro_encode_name(0x42d3ac);
    iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
    if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) == 0) {
      tesoro_generic_door_use(param_1);
      return 1;
    }
    **(undefined1 **)(param_1 + 0x60) = 0;
  }
  else {
    **(undefined1 **)(param_1 + 0x60) = 1;
  }
  uVar2 = tesoro_encode_name(0x42d4b4);
  uVar3 = tesoro_encode_name(0x42e8d8);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

