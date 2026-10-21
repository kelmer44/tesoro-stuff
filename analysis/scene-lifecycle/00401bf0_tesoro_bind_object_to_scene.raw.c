/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401bf0
 */


void __thiscall tesoro_bind_object_to_scene(void *this,int param_1)

{
  if (param_1 != 0) {
    *(int *)((int)this + 0x40) = param_1;
  }
  if (((*(uint *)((int)this + 0x68) & 0x100) != 0) && (*(void **)((int)this + 0x3c) != (void *)0x0))
  {
    tesoro_attach_object_to_group(*(void **)((int)this + 0x3c),(int)this);
  }
                    /* WARNING: Load size is inaccurate */
  (*(code *)**this)();
  return;
}

