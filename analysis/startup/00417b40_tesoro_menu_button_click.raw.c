/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00417b40
 */


undefined4 __fastcall tesoro_menu_button_click(int param_1)

{
  void *pvVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  char *pcVar5;
  void *pvVar6;
  
  switch(*(undefined4 *)(param_1 + 0xa0)) {
  case 0:
    tesoro_fade_to_black(DAT_00442108,4,100);
    tesoro_place_jim_in_scene(DAT_004420fc,DAT_004421f4,*(int *)((int)DAT_004421f4 + 0x70),0,0);
    uVar2 = tesoro_encode_name(0x42e774);
    uVar3 = tesoro_encode_name(0x42e768);
    pvVar6 = *(void **)((int)DAT_004420fc + 0x40);
    iVar4 = 0;
    pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar6);
    return 3;
  case 1:
    tesoro_fade_to_black(DAT_00442108,4,100);
    FUN_004026d0(DAT_00442128);
    return 1;
  case 2:
    pcVar5 = s_facil_0042e760;
    break;
  case 3:
    tesoro_fade_to_black(DAT_00442108,4,100);
    tesoro_start_new_game();
    return 3;
  case 4:
    pcVar5 = s_dificil_0042e758;
    break;
  default:
    goto switchD_00417b54_default;
  }
  uVar2 = tesoro_encode_name((int)pcVar5);
  uVar3 = tesoro_encode_name(0x42e768);
  pvVar6 = *(void **)(param_1 + 0x40);
  iVar4 = 0;
  pvVar1 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(pvVar1,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar6);
switchD_00417b54_default:
  return 1;
}

