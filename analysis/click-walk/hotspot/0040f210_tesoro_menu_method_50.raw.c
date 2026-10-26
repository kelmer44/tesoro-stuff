/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f210
 */


undefined4 __fastcall tesoro_menu_method_50(int param_1)

{
  *(undefined1 *)(param_1 + 0x8d) = 1;
  if (*(int *)(param_1 + 0x70) != 0) {
    FUN_00410330(DAT_004420fc,*(int *)(param_1 + 0x70),(uint)*(ushort *)((int)DAT_004420fc + 0xd0),0
                );
    (**(code **)(*DAT_00442128 + 0x20))(0);
    *(undefined1 *)(param_1 + 0x8c) = 1;
    FUN_0040f900(DAT_00442104,param_1,5);
    return 3;
  }
  return 0;
}

