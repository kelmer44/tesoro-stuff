/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004069e0
 */


void __fastcall tesoro_resolve_room_floor(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  ulonglong uVar5;
  undefined2 uStack_6;
  undefined2 uStack_2;
  
  if (*(char *)(param_1 + 0x6c) == '\0') {
    *(undefined4 *)(param_1 + 0x78) = 0;
  }
  else {
    uVar5 = tesoro_encode_name(param_1 + 0x6c);
    iVar3 = FUN_0040f680(*(void **)(*(int *)(param_1 + 0x40) + 0x74),(int)uVar5,(int)(uVar5 >> 0x20)
                        );
    *(int *)(param_1 + 0x78) = iVar3;
  }
  iVar3 = *(int *)(*(int *)(param_1 + 0x38) + 0x1c);
  uVar1 = *(undefined4 *)(iVar3 + 0xc);
  uStack_6 = (short)((uint)uVar1 >> 0x10);
  uVar2 = *(undefined4 *)(iVar3 + 0x10);
  iVar3 = (int)(short)uVar1 + *(int *)(param_1 + 0x4c);
  iVar4 = *(int *)(param_1 + 0x50) + (int)uStack_6;
  uStack_2 = (short)((uint)uVar2 >> 0x10);
  tesoro_set_camera_bounds
            (*(void **)(*(int *)(param_1 + 0x40) + 0x80),iVar3,iVar4,(short)uVar2 + iVar3,
             iVar4 - uStack_2);
  tesoro_set_camera_position
            (*(void **)(*(int *)(param_1 + 0x40) + 0x80),*(int *)(param_1 + 0x4c),
             *(int *)(param_1 + 0x50),0);
  return;
}

