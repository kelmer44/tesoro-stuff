/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00403250
 */


void __thiscall tesoro_open_archive(void *this,LPCSTR param_1,LPCSTR param_2)

{
  int iVar1;
  
  if (*(int **)((int)this + 4) != (int *)0x0) {
    FUN_0041b7c0(*(int **)((int)this + 4));
  }
  if (param_2 != (LPCSTR)0x0) {
    iVar1 = FUN_0041b7a0(param_2,&DAT_0042d074);
    *(int *)((int)this + 4) = iVar1;
    if (iVar1 == 0) {
      MessageBoxA((HWND)0x0,s_ERROR_INTERNO__no_se_pudo_abrir_e_0042dc50,s_ERROR_0042d06c,0x30);
    }
    else {
      tesoro_read_index((int *)this);
      FUN_0041b7c0(*(int **)((int)this + 4));
    }
  }
  iVar1 = FUN_0041b7a0(param_1,&DAT_0042d074);
  *(int *)((int)this + 4) = iVar1;
  if (iVar1 == 0) {
    MessageBoxA((HWND)0x0,s_ERROR_INTERNO__no_se_pudo_abrir_e_0042dc50,s_ERROR_0042d06c,0x30);
  }
  return;
}

