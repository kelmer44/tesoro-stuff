/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415f90
 */


undefined4 __fastcall f415f90(uint param_1)

{
  int iVar1;
  void *this;
  undefined4 uVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  undefined4 uStack_4;
  
  if (*(char *)(DAT_004420fc + 0x110) != '\0') {
    uStack_4 = param_1 & 0xffffff;
    uVar3 = tesoro_encode_name(0x42e4d0);
    iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
    if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) != 0) {
      uStack_4 = CONCAT13(1,(undefined3)uStack_4);
    }
    uVar3 = tesoro_encode_name(0x42e4b4);
    uVar4 = tesoro_encode_name(0x42e4a4);
    pvVar5 = *(void **)(param_1 + 0x40);
    iVar1 = (int)&uStack_4 + 3;
    this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
    tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar1,pvVar5);
    return 3;
  }
  uStack_4 = param_1;
  uVar2 = tesoro_generic_door_use(param_1);
  return uVar2;
}

