/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401d80
 */


void __thiscall tesoro_set_object_flag_bit2(void *this,byte param_1)

{
  *(uint *)((int)this + 0x68) = (param_1 & 1) << 2 | *(uint *)((int)this + 0x68) & 0xfffffffb;
  return;
}

