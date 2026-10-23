/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413570
 */


int * __thiscall tesoro_load_text_table(void *this,int param_1)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  char *pcVar6;
  undefined4 uVar7;
  uint uVar8;
  uint uVar9;
  char *pcVar10;
  ulonglong uVar11;
  undefined8 local_430;
  int local_428;
  int local_424;
  int local_420;
  undefined1 local_41c [20];
  byte local_408 [32];
  char local_3e8 [1000];
  
  FUN_00403320(DAT_00442064,param_1 + -4);
  uVar9 = 0;
  piVar2 = *(int **)((int)DAT_00442064 + 4);
  *(undefined4 *)((int)this + 4) = 0;
  FUN_0041b4c0((char *)&local_420,1,4,piVar2);
  FUN_0041b490(piVar2,&local_430);
  if (((int)local_430._4_4_ < 1) &&
     ((0x7fffffff < local_430._4_4_ || ((uint)local_430 <= (uint)(local_420 + -8 + param_1))))) {
    do {
      FUN_0041c120(piVar2,(byte *)s______s_0042d69c);
      FUN_0041c120(piVar2,(byte *)s_________s_0042d690);
      uVar8 = 0xffffffff;
      pcVar6 = local_3e8;
      do {
        if (uVar8 == 0) break;
        uVar8 = uVar8 - 1;
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar1 != '\0');
      uVar9 = uVar9 + ~uVar8;
      *(int *)((int)this + 4) = *(int *)((int)this + 4) + 1;
      iVar3 = piVar2[1];
      piVar2[1] = iVar3 + -1;
      if (iVar3 + -1 < 0) {
        FUN_0041c030(piVar2);
      }
      else {
        *piVar2 = *piVar2 + 1;
      }
      iVar3 = piVar2[1];
      piVar2[1] = iVar3 + -1;
      if (iVar3 + -1 < 0) {
        FUN_0041c030(piVar2);
      }
      else {
        *piVar2 = *piVar2 + 1;
      }
      FUN_0041b490(piVar2,&local_430);
    } while ((0x7fffffff < local_430._4_4_) ||
            (((int)local_430._4_4_ < 1 && ((uint)local_430 <= (uint)(local_420 + -8 + param_1)))));
  }
  local_430._4_4_ = 0;
  local_430._0_4_ = param_1;
  FUN_0041b600(piVar2,(LONG *)&local_430);
  puVar5 = FUN_0040db40(*(int *)((int)this + 4) << 4,1);
  *(undefined4 **)this = puVar5;
  pcVar6 = (char *)FUN_0040db40(uVar9,1);
  local_424 = 0;
  if (0 < *(int *)((int)this + 4)) {
    local_428 = 0;
    do {
      iVar4 = local_428;
      FUN_0041c120(piVar2,(byte *)s______s_0042d69c);
      FUN_0041c120(piVar2,(byte *)s_________s_0042d690);
      uVar11 = tesoro_encode_name((int)local_41c);
                    /* WARNING: Load size is inaccurate */
      iVar3 = *this;
      *(int *)(iVar3 + iVar4) = (int)uVar11;
      *(int *)(iVar3 + 4 + iVar4) = (int)(uVar11 >> 0x20);
                    /* WARNING: Load size is inaccurate */
      *(char **)(*this + 0xc + iVar4) = pcVar6;
      FUN_0041c140(local_408,s__s_WAV_0042d1fc);
      uVar7 = tesoro_find_resource(DAT_00442064,local_408,'\0');
                    /* WARNING: Load size is inaccurate */
      *(undefined4 *)(*this + 8 + iVar4) = uVar7;
      uVar9 = 0xffffffff;
      pcVar10 = pcVar6;
      do {
        if (uVar9 == 0) break;
        uVar9 = uVar9 - 1;
        cVar1 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar1 != '\0');
      iVar3 = piVar2[1];
      piVar2[1] = iVar3 + -1;
      pcVar6 = pcVar6 + ~uVar9;
      if (iVar3 + -1 < 0) {
        FUN_0041c030(piVar2);
      }
      else {
        *piVar2 = *piVar2 + 1;
      }
      iVar3 = piVar2[1];
      piVar2[1] = iVar3 + -1;
      if (iVar3 + -1 < 0) {
        FUN_0041c030(piVar2);
      }
      else {
        *piVar2 = *piVar2 + 1;
      }
      local_424 = local_424 + 1;
      local_428 = local_428 + 0x10;
    } while (local_424 < *(int *)((int)this + 4));
  }
  tesoro_sort_text_table((int *)this);
  return (int *)this;
}

