/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401da0
 */


void __fastcall tesoro_save_object_animation_state(int param_1)

{
  *(uint *)(param_1 + 4) = (uint)*(byte *)(param_1 + 0x32);
  *(undefined4 *)(param_1 + 8) = *(undefined4 *)(*(int *)(param_1 + 0x38) + 0x18);
  return;
}

