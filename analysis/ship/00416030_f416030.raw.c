/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416030
 */


undefined4 __fastcall f416030(int param_1)

{
  int iVar1;
  void *this;
  ulonglong uVar2;
  ulonglong uVar3;
  void *pvVar4;
  
  if (*(char *)((int)DAT_004420fc + 0x111) == '\0') {
    uVar2 = tesoro_encode_name(0x42d420);
    iVar1 = tesoro_find_scene_object(DAT_004421ec,(int)uVar2,(int)(uVar2 >> 0x20));
    FUN_00401de0(*(void **)(*(int *)(*(int *)(param_1 + 0x40) + 0x90) + 0x78),0,0,0,0,'\0');
    FUN_00401de0(*(void **)(*(int *)(*(int *)(*(int *)(param_1 + 0x40) + 0x90) + 0x78) + 0x78),0,1,1
                 ,0,'\0');
    if (**(char **)(iVar1 + 0x60) == '\x02') {
      *(undefined1 *)((int)DAT_004420fc + 0x111) = 1;
      **(undefined1 **)(param_1 + 0x60) = 6;
    }
    if ((*(uint *)(iVar1 + 0x68) >> 1 & 1) == 0) {
      iVar1 = FUN_00413ac0(0,3);
      **(undefined1 **)(param_1 + 0x60) = (char)iVar1;
      FUN_00401de0(*(void **)(*(int *)(*(int *)(param_1 + 0x40) + 0x90) + 0x78),0,2,2,0,'\0');
      FUN_00401de0(*(void **)(*(int *)(*(int *)(*(int *)(param_1 + 0x40) + 0x90) + 0x78) + 0x78),0,3
                   ,3,0,'\0');
      (**(code **)(*DAT_004420fc + 0x10))();
    }
    else if (**(char **)(param_1 + 0x60) < '\x04') {
      **(char **)(param_1 + 0x60) = '\x04';
    }
    uVar2 = tesoro_encode_name(0x42d4b4);
    uVar3 = tesoro_encode_name(0x42e4d8);
    pvVar4 = *(void **)(param_1 + 0x40);
    iVar1 = *(int *)(param_1 + 0x60);
    this = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(this,(int)uVar2,(int)(uVar2 >> 0x20),iVar1,pvVar4);
  }
  return 1;
}

