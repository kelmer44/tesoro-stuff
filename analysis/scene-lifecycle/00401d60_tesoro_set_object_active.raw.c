/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401d60
 */


void __thiscall tesoro_set_object_active(void *this,byte param_1)

{
  *(uint *)((int)this + 0x68) = (param_1 & 1) << 1 | *(uint *)((int)this + 0x68) & 0xfffffffd;
  return;
}

