/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00402e50
 */


uint tesoro_probe_in_front(int param_1)

{
  return *(uint *)(param_1 + 0x68) >> 10 & 1;
}

