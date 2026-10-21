/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408800
 */


void __fastcall tesoro_initialize_scene_objects(int param_1)

{
  int iVar1;
  
  FUN_0040dd30(DAT_00442100,2);
  iVar1 = 0;
  if (0 < *(int *)(param_1 + 0x88)) {
    do {
      FUN_00401510(*(int **)(*(int *)(param_1 + 0x6c) + iVar1 * 4));
      iVar1 = iVar1 + 1;
    } while (iVar1 < *(int *)(param_1 + 0x88));
  }
  *(undefined1 *)(param_1 + 0x94) = 1;
  return;
}

