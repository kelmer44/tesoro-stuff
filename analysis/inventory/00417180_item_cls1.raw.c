/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00417180
 */


undefined4 __fastcall item_cls1(uint3 param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  undefined4 uStack_4;
  
  uStack_4 = (uint)param_1;
  if (*(char *)(DAT_004420fc + 0x111) == '\x06') {
    uVar2 = tesoro_encode_name(0x42e68c);
    iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
    if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) != 0) {
      uVar2 = tesoro_encode_name(0x42e68c);
      iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
      if (**(char **)(iVar1 + 0x60) != '\0') {
        *(undefined1 *)(DAT_004420fc + 0x110) = 0;
        *(undefined1 *)(DAT_004420fc + 0x111) = 7;
        uStack_4 = CONCAT13(1,(undefined3)uStack_4);
      }
    }
  }
  uVar2 = tesoro_encode_name(0x42d2d0);
  uVar3 = tesoro_encode_name(0x42e684);
  pvVar4 = *(void **)(DAT_004420fc + 0x40);
  iVar1 = (int)&uStack_4 + 3;
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
  tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  return 1;
}

