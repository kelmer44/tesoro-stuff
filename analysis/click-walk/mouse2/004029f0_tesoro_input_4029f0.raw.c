/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004029f0
 */


undefined4 __fastcall tesoro_input_4029f0(void *param_1)

{
  int *piVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  int iVar8;
  int iVar9;
  ulonglong uVar10;
  
  if (*(int *)((int)param_1 + 0x74) == 0) {
    return 0;
  }
  iVar3 = *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c);
  if ((-1 < iVar3) && (iVar3 < 0x280)) {
    if ((*(byte *)DAT_00442074[0xd] & 8) == 0) {
      if ((*(byte *)(*(int *)(DAT_0044206c + 0x1c) + 1) & 0x18) != 0) {
        FUN_004025b0();
      }
    }
    else {
      (**(code **)*DAT_00442074)();
      if (*(void **)((int)DAT_004420fc + 0x40) == DAT_004421f0) {
        tesoro_fade_to_black(DAT_00442108,4,100);
        FUN_0040fe20(DAT_00442104);
        return 3;
      }
      if (*(void **)((int)DAT_004420fc + 0x40) == DAT_004421f4) {
        tesoro_place_jim_in_scene(DAT_004420fc,DAT_004421f0,*(int *)((int)DAT_004421f0 + 0x70),0,0);
        tesoro_fade_to_black(DAT_00442108,4,100);
        return 1;
      }
      FUN_004026d0((int)param_1);
    }
    goto LAB_00402cf3;
  }
  if (iVar3 < 0x281) {
    if ((*(byte *)DAT_00442074[0xd] & 8) != 0) {
      (**(code **)*DAT_00442074)();
      uVar10 = tesoro_encode_name(0x42d060);
      FUN_0040f680(*(void **)(DAT_004421ec + 0x74),(int)uVar10,(int)(uVar10 >> 0x20));
      *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) =
           *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) + 0x285;
      tesoro_fade_to_black(DAT_00442108,4,100);
    }
    goto LAB_00402cf3;
  }
  if ((*(byte *)DAT_00442074[0xd] & 8) != 0) {
    (**(code **)*DAT_00442074)();
    uVar10 = tesoro_encode_name(0x42d060);
    FUN_0040f680(*(void **)(DAT_004421ec + 0x74),(int)uVar10,(int)(uVar10 >> 0x20));
    *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) =
         *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) + -0x280;
    *(int *)(DAT_004421fc + 0x4c) = *(int *)(DAT_004421fc + 0x4c) + -0x280;
    *(int *)(*(int *)(DAT_004421fc + 0xa8) + 0x4c) =
         *(int *)(*(int *)(DAT_004421fc + 0xa8) + 0x4c) + -0x280;
    iVar3 = *(int *)(DAT_004421fc + 0xa0);
    *(undefined1 *)(iVar3 + 0x14) = 1;
    *(int *)(iVar3 + 0xc) = iVar3 + 4;
    do {
      iVar3 = *(int *)(DAT_004421fc + 0xa0);
      iVar8 = **(int **)(iVar3 + 0xc);
      if (iVar8 == 0) {
LAB_00402b8c:
        bVar7 = false;
      }
      else {
        if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
          *(undefined1 *)(iVar3 + 0x14) = 0;
        }
        else {
          *(int *)(iVar3 + 0xc) = iVar8;
        }
        if (**(int **)(iVar3 + 0xc) == 0) goto LAB_00402b8c;
        *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
        bVar7 = true;
      }
      if (!bVar7) goto LAB_00402ba5;
      piVar1 = (int *)(*(int *)(*(int *)(DAT_004421fc + 0xa0) + 0x18) + 0x4c);
      *piVar1 = *piVar1 + -0x280;
    } while( true );
  }
LAB_00402bc0:
  if ((*(byte *)(*(int *)(DAT_0044206c + 0x1c) + 1) & 0x18) != 0) {
    uVar10 = tesoro_encode_name(0x42d060);
    FUN_0040f680(*(void **)(DAT_004421ec + 0x74),(int)uVar10,(int)(uVar10 >> 0x20));
    *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) =
         *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c) + -0x280;
    *(int *)(DAT_004421fc + 0x4c) = *(int *)(DAT_004421fc + 0x4c) + -0x280;
    *(int *)(*(int *)(DAT_004421fc + 0xa8) + 0x4c) =
         *(int *)(*(int *)(DAT_004421fc + 0xa8) + 0x4c) + -0x280;
    iVar3 = *(int *)(DAT_004421fc + 0xa0);
    *(undefined1 *)(iVar3 + 0x14) = 1;
    *(int *)(iVar3 + 0xc) = iVar3 + 4;
    do {
      iVar3 = *(int *)(DAT_004421fc + 0xa0);
      iVar8 = **(int **)(iVar3 + 0xc);
      if (iVar8 == 0) {
LAB_00402c75:
        bVar7 = false;
      }
      else {
        if ((*(int **)(iVar3 + 0xc) == (int *)(iVar3 + 4)) && (*(char *)(iVar3 + 0x14) != '\0')) {
          *(undefined1 *)(iVar3 + 0x14) = 0;
        }
        else {
          *(int *)(iVar3 + 0xc) = iVar8;
        }
        if (**(int **)(iVar3 + 0xc) == 0) goto LAB_00402c75;
        *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(**(int **)(iVar3 + 0xc) + 4);
        bVar7 = true;
      }
      if (!bVar7) goto LAB_00402c8e;
      piVar1 = (int *)(*(int *)(*(int *)(DAT_004421fc + 0xa0) + 0x18) + 0x4c);
      *piVar1 = *piVar1 + -0x280;
    } while( true );
  }
LAB_00402cf3:
  FUN_00401d30(param_1,*(int *)(DAT_0044206c + 0x38) +
                       *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x2c),
               *(int *)(*(int *)(DAT_004421ec + 0x80) + 0x30) - *(int *)(DAT_0044206c + 0x3c),'\x01'
              );
  iVar3 = *(int *)((int)DAT_004420fc + 0x40);
  iVar8 = *(int *)(DAT_0044206c + 0x38);
  iVar4 = *(int *)(DAT_0044206c + 0x3c);
  iVar5 = *(int *)(*(int *)(iVar3 + 0x80) + 0x2c);
  iVar9 = *(int *)(*(int *)(iVar3 + 0x80) + 0x30);
  *(int *)(*(int *)((int)param_1 + 0x74) + 0x40) = iVar3;
  iVar6 = *(int *)((int)param_1 + 0x74);
  iVar8 = iVar8 + iVar5;
  iVar9 = iVar9 - iVar4;
  *(int *)(iVar6 + 0x4c) = iVar8;
  *(int *)(iVar6 + 0x50) = iVar9;
  *(undefined4 *)(*(int *)((int)param_1 + 0x74) + 0x3c) = *(undefined4 *)(iVar3 + 0x70);
  FUN_00401d30(*(void **)((int)param_1 + 0x74),iVar8,iVar9,'\x01');
  return 1;
LAB_00402ba5:
  if (*(int *)(DAT_00442128 + 300) != 0) {
    puVar2 = (uint *)(*(int *)(DAT_00442128 + 300) + 0x68);
    *puVar2 = *puVar2 | 4;
  }
  FUN_004026d0((int)param_1);
  goto LAB_00402bc0;
LAB_00402c8e:
  if (*(int *)(DAT_00442128 + 300) != 0) {
    puVar2 = (uint *)(*(int *)(DAT_00442128 + 300) + 0x68);
    *puVar2 = *puVar2 | 4;
  }
  goto LAB_00402cf3;
}

