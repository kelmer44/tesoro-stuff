/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004026d0
 */


void __fastcall tesoro_cursor_4026d0(int param_1)

{
  uint *puVar1;
  int *piVar2;
  void *this;
  undefined2 *puVar3;
  undefined2 *puVar4;
  int iVar5;
  undefined2 *puVar6;
  int iVar7;
  undefined4 *puVar8;
  ulonglong uVar9;
  byte bVar10;
  byte bVar11;
  char cVar12;
  
  if (DAT_004420fc[0x10] != DAT_00442168) {
    if (*(int *)((int)DAT_00442128 + 300) != 0) {
      puVar1 = (uint *)(*(int *)((int)DAT_00442128 + 300) + 0x68);
      *puVar1 = *puVar1 | 4;
    }
    *(undefined4 *)((int)DAT_00442128 + 300) = 0;
    FUN_00401de0(DAT_00442128,0,0,0,0,'\0');
    iVar7 = DAT_004420fc[0x10];
    if (((*(int *)(iVar7 + 0x98) < 0x17) && (iVar7 != DAT_004421f0)) && (iVar7 != DAT_004421f4)) {
      uVar9 = tesoro_encode_name(0x42d058);
      piVar2 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar9,(int)(uVar9 >> 0x20));
      iVar7 = *piVar2;
    }
    else {
      uVar9 = tesoro_encode_name(0x42d058);
      piVar2 = (int *)tesoro_find_scene_object(DAT_004421ec,(int)uVar9,(int)(uVar9 >> 0x20));
      iVar7 = *piVar2;
    }
    (**(code **)(iVar7 + 0x20))();
    uVar9 = tesoro_encode_name(0x42d050);
    iVar5 = (int)*(char *)((int)DAT_004420fc + 0x112);
    cVar12 = '\0';
    bVar11 = 0;
    bVar10 = 0;
    iVar7 = iVar5;
    this = (void *)tesoro_find_scene_object(DAT_004421ec,(int)uVar9,(int)(uVar9 >> 0x20));
    FUN_00401de0(this,bVar10,(ushort)iVar5,iVar7,bVar11,cVar12);
    FUN_00407aa0(DAT_004420fc);
    *(undefined1 *)((int)DAT_00442128 + 0x78) = 0;
    *(int *)((int)DAT_00442128 + 0x50) = *(int *)((int)DAT_00442128 + 0x50) + -5000;
    *(int *)(*(int *)((int)DAT_004421ec + 0x80) + 0x30) =
         *(int *)(*(int *)((int)DAT_004421ec + 0x80) + 0x30) + -5000;
    tesoro_reset_render_lists((int)DAT_00442108);
    FUN_00408550(DAT_004420fc[0x10]);
    FUN_00412e10(DAT_00442108);
    iVar7 = 0xa0;
    puVar3 = *(undefined2 **)((int)DAT_00442108 + 0x678);
    puVar6 = *(undefined2 **)(param_1 + 0x128);
    do {
      iVar5 = 0xd5;
      do {
        puVar4 = puVar3;
        *puVar6 = *puVar4;
        puVar6 = puVar6 + 1;
        iVar5 = iVar5 + -1;
        puVar3 = puVar4 + 3;
      } while (iVar5 != 0);
      puVar3 = puVar4 + 0x504;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
    tesoro_reset_render_lists((int)DAT_00442108);
    *(int *)(*(int *)((int)DAT_004421ec + 0x80) + 0x30) =
         *(int *)(*(int *)((int)DAT_004421ec + 0x80) + 0x30) + 5000;
    *(int *)((int)DAT_00442128 + 0x50) = *(int *)((int)DAT_00442128 + 0x50) + 5000;
    *(int *)(*(int *)((int)DAT_004421ec + 0x80) + 0x2c) =
         *(int *)(*(int *)((int)DAT_004421ec + 0x80) + 0x2c) + -0x285;
    *(undefined2 *)(param_1 + 0x11c) = 0;
    puVar8 = *(undefined4 **)(param_1 + 0x120);
    for (iVar7 = 0x4290; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar8 = 0;
      puVar8 = puVar8 + 1;
    }
  }
  return;
}

