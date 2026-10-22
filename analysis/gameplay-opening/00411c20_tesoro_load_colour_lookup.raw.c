/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00411c20
 */


void __thiscall tesoro_load_colour_lookup(void *this,int param_1)

{
  int *piVar1;
  ushort *puVar2;
  int iVar3;
  
  piVar1 = (int *)FUN_004032e0(DAT_00442064,param_1);
  FUN_0041b4c0(*(char **)((int)this + 0x66c),0x8000,1,piVar1);
  if (*(char *)((int)this + 0x67c) != '\0') {
    iVar3 = 0x4000;
    puVar2 = *(ushort **)((int)this + 0x66c);
    do {
      iVar3 = iVar3 + -1;
      *puVar2 = *puVar2 >> 1 & 0x7fe0 | *puVar2 & 0x1f;
      puVar2 = puVar2 + 1;
    } while (iVar3 != 0);
  }
  return;
}

