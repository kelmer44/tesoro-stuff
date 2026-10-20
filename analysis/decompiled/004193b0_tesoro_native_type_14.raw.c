/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004193b0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall tesoro_native_type_14(void *this,int param_1)

{
  void *pvVar1;
  int iVar2;
  undefined4 uVar3;
  
  pvVar1 = DAT_00442154;
  if (param_1 != 0) {
    if (param_1 != 1) {
      return;
    }
    *(void **)((int)this + 0x40) = DAT_00442154;
    tesoro_place_jim_in_scene
              (DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),DAT_004420fc[0x13],
               DAT_004420fc[0x14]);
    (**(code **)(*DAT_00442128 + 0x20))(0);
    (**(code **)(*DAT_004420fc + 0x20))(1);
    return;
  }
  (**(code **)(*DAT_00442128 + 0x20))(1);
  if (DAT_00442228 == '\0') {
    if (DAT_00442230 == '\0') {
      return;
    }
    if (DAT_00442220 != 0) {
      iVar2 = FUN_00413a10(DAT_00442120);
      if ((uint)(iVar2 - DAT_00442220) < 0x5dd) {
        return;
      }
      DAT_00442220 = 0;
      return;
    }
    DAT_00442230 = '\0';
    uVar3 = FUN_004191c0(0x442278,(int)DAT_00442278);
    switch(uVar3) {
    case 1:
      DAT_00442228 = '\x02';
      break;
    case 2:
    case 3:
      DAT_00442228 = '\x01';
      break;
    default:
      goto switchD_004194cb_caseD_ffffffff;
    }
    DAT_00442230 = '\x01';
    DAT_00442220 = FUN_00413a10(DAT_00442120);
    _DAT_00442224 = FUN_00413a10(DAT_00442120);
  }
  else {
    iVar2 = FUN_00413a10(DAT_00442120);
    if (3000 < (uint)(iVar2 - DAT_00442220)) {
      *(undefined1 *)(*(int *)((int)this + 0xd0) + 2) = DAT_0044222c;
      if (DAT_00442228 == '\x02') {
        *(undefined1 *)(*(int *)((int)this + 0xd0) + 2) = 2;
      }
      (**(code **)(*DAT_00442128 + 0x20))(0);
      FUN_00418e90();
      return;
    }
  }
switchD_004194cb_caseD_ffffffff:
  FUN_00418e90();
  return;
}

