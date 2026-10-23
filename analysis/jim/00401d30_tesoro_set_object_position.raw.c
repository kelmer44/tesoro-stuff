/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401d30
 */


void __thiscall
tesoro_set_object_position(void *this,undefined4 param_1,undefined4 param_2,char param_3)

{
  if (param_3 != '\0') {
    *(undefined4 *)((int)this + 0x4c) = param_1;
    *(undefined4 *)((int)this + 0x50) = param_2;
  }
  *(undefined4 *)((int)this + 0x54) = param_1;
  *(undefined4 *)((int)this + 0x58) = param_2;
  FUN_00408440(*(void **)((int)this + 0x40),(int)this);
  return;
}

