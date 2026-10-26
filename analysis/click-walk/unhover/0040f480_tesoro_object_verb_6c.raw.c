/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f480
 */


undefined4 __thiscall tesoro_object_verb_6c(void *this,int param_1)

{
  ushort uVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  uint uVar6;
  undefined4 uVar7;
  
  *(undefined1 *)((int)this + 0x8d) = 1;
  iVar3 = DAT_00442128[0x4b];
  if (iVar3 != 0) {
    iVar4 = 0;
    if (*(ushort *)(iVar3 + 0xa8) != 0) {
      piVar2 = *(int **)(iVar3 + 0xa0);
      do {
        if ((*(int *)((int)this + 0x44) == *piVar2) && (*(int *)((int)this + 0x48) == piVar2[1])) {
          FUN_00410330(DAT_004420fc,*(int *)(*(int *)(iVar3 + 0xa4) + iVar4 * 4),
                       (uint)*(ushort *)((int)DAT_004420fc + 0xd0),0);
          (**(code **)(*DAT_00442128 + 0x20))(0);
          *(undefined1 *)((int)this + 0x8c) = 1;
          goto LAB_0040f530;
        }
        iVar4 = iVar4 + 1;
        piVar2 = piVar2 + 2;
      } while (iVar4 < (int)(uint)*(ushort *)(iVar3 + 0xa8));
    }
  }
  uVar7 = 0;
  if (param_1 == 0) {
    uVar1 = *(ushort *)((int)DAT_004420fc + 0xd0);
    pcVar5 = s_ICON01T058_0042d640;
  }
  else {
    uVar1 = *(ushort *)((int)DAT_004420fc + 0xd0);
    pcVar5 = s_MODELO2_0042d208;
  }
  uVar6 = (uint)uVar1;
  iVar3 = FUN_00413940((int)pcVar5);
  FUN_00410330(DAT_004420fc,iVar3,uVar6,uVar7);
  (**(code **)(*DAT_00442128 + 0x20))(0);
  *(undefined1 *)((int)this + 0x8c) = 1;
LAB_0040f530:
  FUN_0040f900(DAT_00442104,this,5);
  return 3;
}

