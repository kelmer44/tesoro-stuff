/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004196c0
 */


undefined4 __fastcall f4196c0(int param_1)

{
  int iVar1;
  int iVar2;
  void *this;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  
  uVar3 = tesoro_encode_name(0x42e6d4);
  uVar4 = tesoro_encode_name(0x42d4fc);
  iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  iVar2 = tesoro_find_scene_object(DAT_00442154,(int)uVar4,(int)(uVar4 >> 0x20));
  *(byte *)(*(int *)(param_1 + 0x60) + 2) =
       (*(byte *)(*(int *)(iVar2 + 0x60) + 4) | (byte)(*(uint *)(iVar1 + 0x68) >> 1)) & 1;
  uVar3 = tesoro_encode_name(0x42d564);
  iVar1 = tesoro_find_scene_object(DAT_00442154,(int)uVar3,(int)(uVar3 >> 0x20));
  *(undefined1 *)(*(int *)(param_1 + 0x60) + 3) = *(undefined1 *)(*(int *)(iVar1 + 0x60) + 1);
  uVar3 = tesoro_encode_name(0x42d39c);
  uVar4 = tesoro_encode_name(0x42e910);
  pvVar5 = *(void **)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar5);
  return 1;
}

