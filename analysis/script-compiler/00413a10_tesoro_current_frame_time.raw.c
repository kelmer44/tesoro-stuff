/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413a10
 */


int __fastcall tesoro_current_frame_time(int *param_1)

{
  DWORD DVar1;
  
  if (*(char *)((int)param_1 + 0xd) == '\0') {
    DVar1 = GetTickCount();
    if ((int)(DVar1 - param_1[2]) < 500) {
      *param_1 = *param_1 + (DVar1 - param_1[2]);
    }
    param_1[2] = DVar1;
    if ((char)param_1[3] != '\0') {
      return param_1[1];
    }
  }
  return *param_1;
}

