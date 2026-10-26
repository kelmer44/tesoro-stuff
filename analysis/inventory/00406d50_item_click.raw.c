/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00406d50
 */


undefined4 __thiscall item_click(void *this,int param_1)

{
  int *piVar1;
  uint uVar2;
  int *piVar3;
  void *pvVar4;
  int iVar5;
  int iVar6;
  
  *(int *)((int)this + 0x9c) = param_1;
  if ((param_1 < 7) && (iVar5 = *(int *)((int)this + param_1 * 4 + 0x70), iVar5 != 0)) {
    iVar6 = 0;
    uVar2 = (uint)*(ushort *)((int)DAT_004420fc + 0xd0);
LAB_00406f67:
    FUN_00410330(DAT_004420fc,iVar5,uVar2,iVar6);
    (**(code **)(*DAT_00442128 + 0x20))(0);
    *(undefined1 *)((int)this + 0x8c) = 1;
    FUN_0040f900(DAT_00442104,this,5);
    return 3;
  }
  switch(param_1) {
  case 0:
                    /* WARNING: Load size is inaccurate */
    (**(code **)(*this + 0x20))(1);
    return 3;
  case 1:
                    /* WARNING: Load size is inaccurate */
    iVar5 = *this;
    iVar6 = (**(code **)(iVar5 + 0x54))();
    if (iVar6 != 0) {
      return 1;
    }
    break;
  case 2:
                    /* WARNING: Load size is inaccurate */
    iVar5 = *this;
    iVar6 = (**(code **)(iVar5 + 0x5c))();
    if (iVar6 != 0) {
      return 1;
    }
    break;
  case 3:
                    /* WARNING: Load size is inaccurate */
    iVar5 = *this;
    iVar6 = (**(code **)(iVar5 + 0x58))();
    if (iVar6 != 0) {
      return 1;
    }
    break;
  case 4:
                    /* WARNING: Load size is inaccurate */
    iVar5 = *this;
    iVar6 = (**(code **)(iVar5 + 0x60))();
    if (iVar6 != 0) {
      return 1;
    }
    break;
  case 5:
                    /* WARNING: Load size is inaccurate */
    iVar5 = *this;
    iVar6 = (**(code **)(iVar5 + 100))();
    if (iVar6 != 0) {
      return 1;
    }
    break;
  case 6:
                    /* WARNING: Load size is inaccurate */
    iVar5 = *this;
    iVar6 = (**(code **)(iVar5 + 0x68))();
    if (iVar6 != 0) {
      return 1;
    }
    break;
  default:
    iVar5 = 0;
    piVar1 = (int *)DAT_00442128[0x4b];
    if (*(short *)((int)this + 0xa8) != 0) {
      do {
        piVar3 = (int *)tesoro_find_scene_object
                                  (DAT_004421ec,*(int *)(*(int *)((int)this + 0xa0) + iVar5 * 8),
                                   *(int *)(*(int *)((int)this + 0xa0) + 4 + iVar5 * 8));
        if (piVar3 == piVar1) {
          FUN_00410330(DAT_004420fc,*(int *)(*(int *)((int)this + 0xa4) + iVar5 * 4),
                       (uint)*(ushort *)((int)DAT_004420fc + 0xd0),0);
          (**(code **)(*DAT_00442128 + 0x20))(0);
          goto LAB_00406ef4;
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < (int)(uint)*(ushort *)((int)this + 0xa8));
    }
    iVar5 = 0;
    if ((short)piVar1[0x2a] != 0) {
      do {
        pvVar4 = (void *)tesoro_find_scene_object
                                   (DAT_004421ec,*(int *)(piVar1[0x28] + iVar5 * 8),
                                    *(int *)(piVar1[0x28] + 4 + iVar5 * 8));
        if (pvVar4 == this) {
          FUN_00410330(DAT_004420fc,*(int *)(piVar1[0x29] + iVar5 * 4),
                       (uint)*(ushort *)((int)DAT_004420fc + 0xd0),0);
          (**(code **)(*DAT_00442128 + 0x20))(0);
LAB_00406ef4:
          *(undefined1 *)((int)this + 0x8c) = 1;
          FUN_0040f900(DAT_00442104,this,5);
          FUN_004028c0();
          return 3;
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < (int)(uint)*(ushort *)(piVar1 + 0x2a));
    }
                    /* WARNING: Load size is inaccurate */
    iVar5 = (**(code **)(*this + 0x6c))(param_1 + -7);
    if ((iVar5 != 0) ||
       (iVar6 = (**(code **)(*piVar1 + 0x6c))(*(undefined2 *)((int)this + 0xaa)), iVar6 != 0)) {
      return 1;
    }
    uVar2 = (uint)*(ushort *)((int)DAT_004420fc + 0xd0);
    iVar5 = FUN_00413940(0x42d208);
    goto LAB_00406f67;
  }
  (**(code **)(iVar5 + 0x20))(1);
  return 3;
}

