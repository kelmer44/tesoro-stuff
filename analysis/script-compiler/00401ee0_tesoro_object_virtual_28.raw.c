/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401ee0
 */


void __thiscall tesoro_object_virtual_28(void *this,int *param_1)

{
  if ((*(uint *)((int)this + 0x68) >> 4 & 1) != 0) {
    FUN_0041b620((char *)((int)this + 0x32),1,1,param_1);
    FUN_0041b620((char *)(*(int *)((int)this + 0x38) + 0x24),1,2,param_1);
    FUN_0041b620((char *)(*(int *)((int)this + 0x38) + 0x26),1,2,param_1);
    FUN_0041b620((char *)(*(int *)((int)this + 0x38) + 0x30),1,1,param_1);
    FUN_0041b620((char *)(*(int *)((int)this + 0x38) + 0x18),1,1,param_1);
  }
  if (*(int *)((int)this + 0x60) != 0) {
    FUN_0041b620((char *)((int)this + 100),1,2,param_1);
    FUN_0041b620(*(char **)((int)this + 0x60),1,(uint)*(ushort *)((int)this + 100),param_1);
  }
  return;
}

