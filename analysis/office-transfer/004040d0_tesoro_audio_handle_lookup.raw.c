/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004040d0
 */


int __thiscall tesoro_audio_handle_lookup(void *this,int param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  if (*(int *)((int)this + 0x18) < 1) {
    return 0;
  }
  piVar2 = (int *)((int)this + 8);
  do {
    if (*(int *)(*piVar2 + 0x2c) == param_1) {
      return ((int *)((int)this + 8))[iVar1];
    }
    iVar1 = iVar1 + 1;
    piVar2 = piVar2 + 1;
  } while (iVar1 < *(int *)((int)this + 0x18));
  return 0;
}

