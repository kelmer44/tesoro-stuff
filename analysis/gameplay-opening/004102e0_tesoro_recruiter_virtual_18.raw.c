/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004102e0
 */


void __thiscall tesoro_recruiter_virtual_18(void *this,int param_1)

{
  int unaff_retaddr;
  
  if (*(char *)((int)this + 0xd4) != '\0') {
                    /* WARNING: Load size is inaccurate */
    (**(code **)(*this + 0x7c))(1);
    tesoro_draw_interactive_object(this,unaff_retaddr);
    return;
  }
  if ((*(char *)((int)this + 0xc) != '\0') && (*(char *)((int)this + 0xd5) == '\0')) {
                    /* WARNING: Load size is inaccurate */
    (**(code **)(*this + 0x7c))(0);
  }
  tesoro_draw_interactive_object(this,param_1);
  return;
}

