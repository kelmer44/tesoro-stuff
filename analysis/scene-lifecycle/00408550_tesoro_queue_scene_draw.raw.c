/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408550
 */


void __fastcall tesoro_queue_scene_draw(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  int local_8;
  int local_4;
  
  local_8 = 0;
  if (0 < *(int *)(param_1 + 0x84)) {
    local_4 = 0;
    do {
      tesoro_set_group_camera
                (*(void **)(param_1 + 0x80),*(int *)(*(int *)(param_1 + 0x70) + 4 + local_4));
      iVar1 = *(int *)(*(int *)(param_1 + 0x70) + 0xc + local_4);
      *(undefined1 *)(iVar1 + 0x14) = 1;
      *(int *)(iVar1 + 0xc) = iVar1 + 4;
LAB_00408595:
      iVar2 = **(int **)(iVar1 + 0xc);
      if (iVar2 == 0) {
LAB_004085c8:
        bVar4 = false;
      }
      else {
        if ((*(int **)(iVar1 + 0xc) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x14) != '\0')) {
          *(undefined1 *)(iVar1 + 0x14) = 0;
        }
        else {
          *(int *)(iVar1 + 0xc) = iVar2;
        }
        if (**(int **)(iVar1 + 0xc) == 0) goto LAB_004085c8;
        bVar4 = true;
        *(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(**(int **)(iVar1 + 0xc) + 4);
      }
      if (bVar4) {
        uVar3 = (*(int **)(iVar1 + 0x18))[0x1a];
        if ((((uVar3 & 1) == 0) && ((uVar3 & 4) != 0)) && ((uVar3 & 2) != 0)) {
          iVar2 = **(int **)(iVar1 + 0x18);
          (**(code **)(iVar2 + 0xc))();
          (**(code **)(iVar2 + 0x18))(*(undefined4 *)(param_1 + 0x80));
        }
        goto LAB_00408595;
      }
      local_8 = local_8 + 1;
      local_4 = local_4 + 0x10;
    } while (local_8 < *(int *)(param_1 + 0x84));
  }
  return;
}

