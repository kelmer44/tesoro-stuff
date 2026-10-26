/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414d50
 */


undefined4 __thiscall door_3c(void *this,int param_1)

{
  undefined4 uVar1;
  
  if ((*(int *)(DAT_004420fc[0x10] + 0x98) < 0x17) && (param_1 == 0)) {
    DAT_004420fc[0x13] = *(int *)((int)this + 0x94);
    DAT_004420fc[0x14] = *(int *)((int)this + 0x98);
    (**(code **)(*DAT_004420fc + 0x10))();
    FUN_0040fb20(DAT_00442104,(int)DAT_004420fc);
                    /* WARNING: Load size is inaccurate */
    uVar1 = (**(code **)(*this + 0x34))();
    return uVar1;
  }
  return 0;
}

