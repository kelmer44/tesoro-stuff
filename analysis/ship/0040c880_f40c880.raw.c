/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040c880
 */


undefined4 __fastcall f40c880(int param_1)

{
  undefined4 uVar1;
  
  if (**(char **)(param_1 + 0x60) != '\0') {
    return 1;
  }
  uVar1 = tesoro_generic_door_use(param_1);
  return uVar1;
}

