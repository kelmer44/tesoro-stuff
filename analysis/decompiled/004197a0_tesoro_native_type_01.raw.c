/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004197a0
 */


void __thiscall tesoro_native_type_01(void *this,undefined4 param_1)

{
  int *piVar1;
  void *pvVar2;
  ulonglong uVar3;
  ushort uVar4;
  int iVar5;
  byte bVar6;
  byte bVar7;
  char cVar8;
  
  switch(param_1) {
  case 0:
    tesoro_set_object_flag_bit2(DAT_004420fc,0);
    (**(code **)(*DAT_004420fc + 0x20))(1);
    tesoro_update_scene_view((void *)DAT_004420fc[0x10],DAT_004420fc[0x13],DAT_004420fc[0x14],0);
    return;
  case 3:
    uVar3 = tesoro_encode_name(0x42e91c);
    bVar7 = 1;
    pvVar2 = (void *)tesoro_find_scene_object(DAT_00442134,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_set_object_flag_bit2(pvVar2,bVar7);
    uVar3 = tesoro_encode_name(0x42e91c);
    cVar8 = '\0';
    bVar6 = 1;
    iVar5 = -1;
    uVar4 = 0;
    bVar7 = 0;
    pvVar2 = (void *)tesoro_find_scene_object(DAT_00442134,(int)uVar3,(int)(uVar3 >> 0x20));
    FUN_00401de0(pvVar2,bVar7,uVar4,iVar5,bVar6,cVar8);
    return;
  case 4:
    tesoro_set_object_flag_bit2(DAT_004420fc,1);
    return;
  case 5:
    uVar3 = tesoro_encode_name(0x42da50);
    piVar1 = (int *)tesoro_find_scene_object(DAT_00442134,(int)uVar3,(int)(uVar3 >> 0x20));
    (**(code **)(*piVar1 + 0x20))(1);
    uVar3 = tesoro_encode_name(0x42da50);
    bVar7 = 0;
    pvVar2 = (void *)tesoro_find_scene_object(DAT_00442134,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_set_object_flag_bit2(pvVar2,bVar7);
    pvVar2 = DAT_00442130;
    *(void **)((int)this + 0x40) = DAT_00442130;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar2,*(int *)((int)pvVar2 + 0x70),0x226,100);
  }
  return;
}

