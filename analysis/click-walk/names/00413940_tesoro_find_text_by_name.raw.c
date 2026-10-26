/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00413940
 */


int __cdecl tesoro_find_text_by_name(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  ulonglong uVar3;
  uint uVar4;
  CHAR local_a0 [80];
  undefined1 local_50 [80];
  
  uVar3 = tesoro_name_encode_wrapper(param_1);
  puVar2 = &DAT_00442110;
  do {
    uVar4 = (uint)(uVar3 >> 0x20);
    iVar1 = FUN_004138b0((void *)*puVar2,(uint)uVar3,uVar4);
    if (iVar1 != 0) {
      return iVar1;
    }
    puVar2 = puVar2 + 1;
  } while ((int)puVar2 < 0x442120);
  tesoro_name_to_string((uint)uVar3,uVar4,(int)local_50);
  FUN_0041c140(local_a0,s_Error__frase___s__no_encontrada_0042d6a4);
  MessageBoxA((HWND)0x0,local_a0,s_ERROR_0042d06c,0x30);
  return 0;
}

