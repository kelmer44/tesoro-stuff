/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00412bb0
 */


void __fastcall tesoro_present_render_surface(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  (**(code **)(**(int **)(param_1 + 0xc) + 0x80))(*(int **)(param_1 + 0xc),0);
  (**(code **)(**(int **)(param_1 + 8) + 0x1c))
            (*(int **)(param_1 + 8),0,0,*(undefined4 *)(param_1 + 0xc),&stack0xffffffe8,0);
  puVar1 = *(undefined4 **)(param_1 + 0x670);
  *(undefined1 *)(param_1 + 0x66a) = 0;
  iVar2 = 0x1e0;
  do {
    *puVar1 = 0;
    *(undefined2 *)(puVar1 + 3) = 2;
    puVar1[4] = 0;
    puVar1 = puVar1 + 500;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  iVar2 = 0x1e0;
  do {
    *puVar1 = 0;
    *(undefined2 *)(puVar1 + 3) = 2;
    puVar1[4] = 0;
    puVar1 = puVar1 + 200;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *(undefined4 *)(param_1 + 0x13a0) = 0;
  *(undefined4 *)(param_1 + 0x664) = 0;
  *(undefined2 *)(param_1 + 0x668) = 0;
  return;
}

