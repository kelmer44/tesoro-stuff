/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401dc0
 */


void __fastcall tesoro_restore_object_animation_state(int param_1)

{
  int iVar1;
  
  *(byte *)(param_1 + 0x32) = *(byte *)(param_1 + 4);
  iVar1 = *(int *)(*(int *)(param_1 + 0x34) + (uint)*(byte *)(param_1 + 4) * 4);
  *(int *)(param_1 + 0x38) = iVar1;
  *(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(param_1 + 8);
  return;
}

