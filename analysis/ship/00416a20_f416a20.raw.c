/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416a20
 */


undefined4 __fastcall f416a20(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  uVar2 = tesoro_encode_name(0x42e5e4);
  iVar1 = tesoro_find_scene_object(DAT_00442138,(int)uVar2,(int)(uVar2 >> 0x20));
  *(bool *)(*(int *)(param_1 + 0x60) + 1) = *(char *)(*(int *)(iVar1 + 0x60) + 4) != '\0';
  uVar2 = tesoro_encode_name(0x42e5d8);
  iVar1 = tesoro_find_scene_object(DAT_00442134,(int)uVar2,(int)(uVar2 >> 0x20));
  *(undefined1 *)(*(int *)(param_1 + 0x60) + 2) = *(undefined1 *)(*(int *)(iVar1 + 0x60) + 3);
  uVar2 = tesoro_encode_name(0x42d39c);
  uVar3 = tesoro_encode_name(0x42e5cc);
  pvVar4 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

