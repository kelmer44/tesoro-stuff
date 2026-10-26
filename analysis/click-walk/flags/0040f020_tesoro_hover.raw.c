/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f020
 */


undefined4 __thiscall tesoro_hover(void *this,int param_1)

{
                    /* WARNING: Load size is inaccurate */
  *(int *)((int)this + 0x9c) = param_1;
  (**(code **)(*this + 0x4c))();
  return 1;
}

