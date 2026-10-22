/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040fec0
 */


undefined4 * __thiscall
tesoro_initialize_character
          (void *this,undefined4 *param_1,int param_2,int param_3,undefined2 *param_4,uint param_5)

{
  int iVar1;
  int iVar2;
  
  tesoro_initialize_interactive_object(this,param_1,param_2,param_3,(byte *)(param_4 + 2),param_5);
  *(undefined ***)this = &PTR_FUN_0042bb48;
  *(undefined2 *)((int)this + 0xd0) = *param_4;
  *(undefined2 *)((int)this + 0xd2) = param_4[1];
  iVar2 = 0;
  *(undefined4 *)((int)this + 0xac) = 0;
  *(undefined4 *)((int)this + 0xc0) = 0;
  if (*(char *)((int)this + 0x33) != '\0') {
    do {
      iVar1 = _strncmp((char *)(*(int *)(*(int *)((int)this + 0x34) + iVar2 * 4) + 0x32),
                       &DAT_0042d650,3);
      if (iVar1 == 0) {
        *(int *)((int)this + 0xac) = iVar2;
      }
      else {
        iVar1 = _strncmp((char *)(*(int *)(*(int *)((int)this + 0x34) + iVar2 * 4) + 0x32),
                         (char *)&PTR_LAB_00424146_2_0042d64c,3);
        if (iVar1 == 0) {
          *(int *)((int)this + *(int *)((int)this + 0xc0) * 4 + 0xb0) = iVar2;
          *(int *)((int)this + 0xc0) = *(int *)((int)this + 0xc0) + 1;
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < (int)(uint)*(byte *)((int)this + 0x33));
  }
  *(undefined1 *)((int)this + 0xd4) = 0;
  *(undefined1 *)((int)this + 0xd5) = 0;
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 4;
  return (undefined4 *)this;
}

