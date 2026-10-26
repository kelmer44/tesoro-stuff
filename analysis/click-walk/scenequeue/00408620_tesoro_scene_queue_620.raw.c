/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408620
 */


void __thiscall tesoro_scene_queue_620(void *this,int *param_1)

{
  bool bVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  undefined4 *unaff_FS_OFFSET;
  char acStack_30 [8];
  int local_28 [3];
  int *local_1c;
  char local_14;
  int *local_10;
  undefined4 uStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_00426ff8;
  uStack_c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_c;
  piVar5 = (int *)**(undefined4 **)((int)this + 0x74);
  piVar4 = local_28;
  for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
    *piVar4 = *piVar5;
    piVar5 = piVar5 + 1;
    piVar4 = piVar4 + 1;
  }
  local_4 = 0;
  local_1c = local_28 + 1;
  local_14 = '\x01';
  piVar4 = local_1c;
  piVar5 = local_10;
  do {
    piVar3 = (int *)*piVar4;
    if (piVar3 == (int *)0x0) {
LAB_00408697:
      bVar1 = false;
    }
    else {
      if ((piVar4 == local_28 + 1) && (local_14 != '\0')) {
        local_14 = '\0';
        piVar3 = piVar4;
      }
      else {
        local_1c = piVar3;
      }
      piVar4 = piVar3;
      if (*piVar3 == 0) goto LAB_00408697;
      piVar5 = *(int **)(*piVar3 + 4);
      bVar1 = true;
      local_10 = piVar5;
    }
    if (!bVar1) {
      acStack_30[0] = '\0';
      acStack_30[1] = '\0';
      acStack_30[2] = '\0';
      acStack_30[3] = '\0';
      acStack_30[4] = '\0';
      acStack_30[5] = '\0';
      acStack_30[6] = '\0';
      acStack_30[7] = '\0';
      FUN_0041b620(acStack_30,1,8,param_1);
      *unaff_FS_OFFSET = uStack_c;
      return;
    }
    if (piVar5 != DAT_004420fc) {
      FUN_0041b620((char *)(piVar5 + 0x11),1,8,param_1);
      FUN_0041b620((char *)(piVar5 + 0x1a),1,4,param_1);
      (**(code **)(*piVar5 + 0x28))(param_1);
      piVar4 = local_1c;
      piVar5 = local_10;
    }
  } while( true );
}

