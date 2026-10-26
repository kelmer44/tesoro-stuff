/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040cf30
 */


undefined4 __fastcall f40cf30(int param_1)

{
  undefined1 uVar1;
  int iVar2;
  void *this;
  ulonglong uVar3;
  ulonglong uVar4;
  void *pvVar5;
  
  uVar3 = tesoro_encode_name(0x42d46c);
  iVar2 = tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
  if ((*(uint *)(iVar2 + 0x68) >> 1 & 1) == 0) {
    uVar3 = tesoro_encode_name(0x42d460);
    iVar2 = tesoro_find_scene_object(DAT_004421ec,(int)uVar3,(int)(uVar3 >> 0x20));
    if ((*(uint *)(iVar2 + 0x68) >> 1 & 1) == 0) {
      uVar1 = 0;
      goto LAB_0040cf85;
    }
  }
  uVar1 = 1;
LAB_0040cf85:
  **(undefined1 **)(param_1 + 0x60) = uVar1;
  uVar3 = tesoro_encode_name(0x42d324);
  uVar4 = tesoro_encode_name(0x42d454);
  pvVar5 = *(void **)(param_1 + 0x40);
  iVar2 = *(int *)(param_1 + 0x60);
  this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar4,(int)(uVar4 >> 0x20));
  tesoro_start_script(this,(int)uVar3,(int)(uVar3 >> 0x20),iVar2,pvVar5);
  return 1;
}

