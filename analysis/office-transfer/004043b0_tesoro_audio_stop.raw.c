/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004043b0
 */


void __fastcall tesoro_audio_stop(undefined4 *param_1)

{
  (**(code **)(*(int *)*param_1 + 0x48))((int *)*param_1);
  (**(code **)(*(int *)*param_1 + 0x34))((int *)*param_1,0);
  *(undefined1 *)((int)param_1 + 0x27) = 0;
  param_1[0xc] = 0;
  return;
}

