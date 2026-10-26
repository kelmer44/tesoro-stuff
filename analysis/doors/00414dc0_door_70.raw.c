/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414dc0
 */


undefined4 door_70(void)

{
  if ((*(int *)(DAT_004420fc + 0x40) == DAT_00442170) && (*(char *)(DAT_004420fc + 0x113) == '\0'))
  {
    tesoro_play_external_movie(DAT_00442108);
    *(undefined1 *)(DAT_004420fc + 0x113) = 1;
  }
  return 0;
}

