/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f680
 */


int __thiscall tesoro_list_find(void *this,int param_1,int param_2)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int local_1c [3];
  int *local_10;
  undefined1 local_8;
  int local_4;
  
                    /* WARNING: Load size is inaccurate */
  piVar4 = local_1c + 1;
  piVar5 = *this;
  piVar6 = local_1c;
  for (iVar3 = 7; iVar3 != 0; iVar3 = iVar3 + -1) {
    *piVar6 = *piVar5;
    piVar5 = piVar5 + 1;
    piVar6 = piVar6 + 1;
  }
  bVar1 = true;
  local_10 = piVar4;
  local_8 = 1;
  do {
    local_10 = (int *)*piVar4;
    if (local_10 == (int *)0x0) {
LAB_0040f6e1:
      bVar2 = false;
    }
    else {
      if ((piVar4 == local_1c + 1) && (bVar1)) {
        bVar1 = false;
        local_8 = 0;
        local_10 = piVar4;
      }
      else {
      }
      piVar4 = local_10;
      if (*local_10 == 0) goto LAB_0040f6e1;
      local_4 = *(int *)(*local_10 + 4);
      bVar2 = true;
    }
    if (!bVar2) {
      return 0;
    }
    if ((*(int *)(local_4 + 0x44) == param_1) && (*(int *)(local_4 + 0x48) == param_2)) {
      return local_4;
    }
  } while( true );
}

