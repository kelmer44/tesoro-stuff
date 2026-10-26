/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416300
 */


void __thiscall camarote(void *this,undefined4 param_1)

{
  int *piVar1;
  void *pvVar2;
  undefined4 uVar3;
  ulonglong uVar4;
  byte bVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  int iVar8;
  byte bVar9;
  char cVar10;
  
  switch(param_1) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    uVar4 = tesoro_encode_name(0x42da50);
    piVar1 = (int *)tesoro_find_scene_object(DAT_00442140,(int)uVar4,(int)(uVar4 >> 0x20));
    (**(code **)(*piVar1 + 0x50))();
    return;
  case 6:
    uVar4 = tesoro_encode_name(0x42d32c);
    iVar8 = tesoro_find_scene_object(DAT_00442140,(int)uVar4,(int)(uVar4 >> 0x20));
    *(undefined2 *)(iVar8 + 0x5e) = 10;
    return;
  case 7:
    uVar4 = tesoro_encode_name(0x42e530);
    piVar1 = (int *)tesoro_find_scene_object(DAT_00442144,(int)uVar4,(int)(uVar4 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(0);
    uVar4 = tesoro_encode_name(0x42e528);
    cVar10 = '\0';
    bVar9 = 1;
    iVar8 = -1;
    uVar6 = 0;
    uVar7 = 0;
    bVar5 = 1;
    pvVar2 = (void *)tesoro_find_scene_object(DAT_0044213c,(int)uVar4,(int)(uVar4 >> 0x20));
    FUN_00401de0(pvVar2,bVar5,CONCAT11(uVar7,uVar6),iVar8,bVar9,cVar10);
    uVar4 = tesoro_encode_name(0x42d2e8);
    piVar1 = (int *)tesoro_find_scene_object(DAT_00442148,(int)uVar4,(int)(uVar4 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(0);
    uVar4 = tesoro_encode_name(0x42e51c);
    piVar1 = (int *)tesoro_find_scene_object(DAT_00442148,(int)uVar4,(int)(uVar4 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(0);
    uVar4 = tesoro_encode_name(0x42e514);
    piVar1 = (int *)tesoro_find_scene_object(DAT_00442148,(int)uVar4,(int)(uVar4 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(0);
    fade(DAT_00442108,4,100);
    pvVar2 = DAT_0044213c;
    *(void **)((int)this + 0x40) = DAT_0044213c;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar2,*(int *)((int)pvVar2 + 0x70),0x1b3,0x2d);
    tesoro_set_object_flag_bit2(DAT_004420fc,0);
    tesoro_update_scene_view
              (*(void **)((int)this + 0x40),*(int *)((int)DAT_004420fc + 0x4c),
               *(int *)((int)DAT_004420fc + 0x50),0);
    iVar8 = 0;
    if (0 < *(int *)((int)this + 0xb8)) {
      do {
        uVar3 = tesoro_find_scene_object
                          (*(void **)((int)this + 0x40),
                           *(int *)(*(int *)((int)this + 0xb4) + iVar8 * 8),
                           *(int *)(*(int *)((int)this + 0xb4) + 4 + iVar8 * 8));
        iVar8 = iVar8 + 1;
        *(undefined4 *)(*(int *)((int)this + 0xb0) + -4 + iVar8 * 4) = uVar3;
      } while (iVar8 < *(int *)((int)this + 0xb8));
      return;
    }
    break;
  case 8:
    tesoro_set_object_flag_bit2(DAT_004420fc,1);
    return;
  case 9:
    fade(DAT_00442108,4,100);
    pvVar2 = DAT_00442140;
    *(void **)((int)this + 0x40) = DAT_00442140;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar2,*(int *)((int)pvVar2 + 0x70),-0xfa,0x82);
    *(undefined1 *)((int)DAT_004420fc + 0x110) = 1;
    *(undefined1 *)((int)DAT_004420fc + 0x111) = 2;
  }
  return;
}

