/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c8a0
 */


undefined4 __fastcall office_door_v4(void *param_1)

{
  **(char **)((int)param_1 + 0x60) = **(char **)((int)param_1 + 0x60) == '\0';
  tesoro_set_object_flag_bit2(param_1,**(char **)((int)param_1 + 0x60) != '\0');
  return 1;
}

