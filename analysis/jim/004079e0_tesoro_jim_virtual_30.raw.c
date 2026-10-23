/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004079e0
 */


undefined4 __thiscall tesoro_jim_virtual_30(void *this,int param_1)

{
  undefined4 uVar1;
  
  if (*(char *)((int)this + 0xd5) != '\0') {
    *(undefined4 *)((int)this + 0x54) = *(undefined4 *)((int)this + 0x4c);
    *(undefined4 *)((int)this + 0x58) = *(undefined4 *)((int)this + 0x50);
    FUN_0040fb20(DAT_00442104,(int)this);
    if ((*(char *)((int)this + 0xf0) == '\0') && (*(void **)(param_1 + 0x78) != (void *)0x0)) {
      uVar1 = FUN_00414f10(*(void **)(param_1 + 0x78),*(int *)((int)this + 0x104),
                           *(int *)((int)this + 0x108),*(int *)((int)this + 0xf4));
      if ((char)uVar1 == '\0') {
                    /* WARNING: Load size is inaccurate */
        (**(code **)(*this + 0x74))(*(undefined4 *)((int)this + 0x10c));
        if (*(int **)((int)this + 0xf4) != (int *)0x0) {
          (**(code **)(**(int **)((int)this + 0xf4) + 0x34))();
          return 1;
        }
      }
    }
    else {
                    /* WARNING: Load size is inaccurate */
      *(undefined1 *)((int)this + 0xf0) = 0;
      (**(code **)(*this + 0x74))(*(undefined4 *)((int)this + 0x10c));
    }
  }
  return 1;
}

