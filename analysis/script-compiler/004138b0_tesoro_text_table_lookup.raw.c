/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004138b0
 */


int __thiscall tesoro_text_table_lookup(void *this,int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  bool bVar7;
  int local_8;
  
  iVar1 = 0;
  iVar4 = *(int *)((int)this + 4) + -1;
  local_8 = -1;
  iVar2 = local_8;
  if (-1 < iVar4) {
                    /* WARNING: Load size is inaccurate */
    do {
      iVar2 = iVar4 + iVar1 >> 1;
      piVar6 = (int *)(*this + iVar2 * 0x10);
      iVar3 = *piVar6;
      iVar5 = param_1;
      if (param_1 == iVar3) {
        iVar3 = piVar6[1];
        iVar5 = param_2;
      }
      bVar7 = iVar5 - iVar3 < 0;
      if (iVar5 == iVar3 || bVar7) {
        if (!bVar7) break;
        iVar1 = iVar2 + 1;
      }
      else {
        iVar4 = iVar2 + -1;
      }
      iVar2 = local_8;
    } while (iVar1 <= iVar4);
  }
  local_8 = iVar2;
  if (local_8 < 0) {
    return 0;
  }
                    /* WARNING: Load size is inaccurate */
  return local_8 * 0x10 + *this;
}

