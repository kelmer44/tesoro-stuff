/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414560
 */


void tesoro_fn_414560(void)

{
  void *this;
  ulonglong uVar1;
  ulonglong uVar2;
  int iVar3;
  void *pvVar4;
  
  iVar3 = *(int *)(*(int *)((int)DAT_00442134 + 0x90) + 0x78);
  tesoro_place_jim_in_scene
            (DAT_004420fc,DAT_00442134,*(int *)((int)DAT_00442134 + 0x70),*(int *)(iVar3 + 0x4c),
             *(int *)(iVar3 + 0x50));
  (**(code **)(*DAT_004420fc + 0x20))(1);
  tesoro_set_object_flag_bit2(DAT_004420fc,1);
  uVar1 = tesoro_encode_name(0x42d4b4);
  uVar2 = tesoro_encode_name(0x42dab4);
  iVar3 = 0;
  pvVar4 = DAT_00442134;
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar2,(int)(uVar2 >> 0x20));
  tesoro_start_script(this,(int)uVar1,(int)(uVar1 >> 0x20),iVar3,pvVar4);
  return;
}

