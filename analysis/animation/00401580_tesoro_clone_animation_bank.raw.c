/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401580
 */


void __thiscall tesoro_clone_animation_bank(void *this,undefined4 *param_1)

{
  int iVar1;
  
  for (iVar1 = 0x10; iVar1 != 0; iVar1 = iVar1 + -1) {
    *(undefined4 *)this = *param_1;
    param_1 = param_1 + 1;
    this = (undefined4 *)((int)this + 4);
  }
  return;
}

