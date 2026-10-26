/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004031a0
 */


void __thiscall tesoro_input_4031a0(void *this,byte param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  
  FUN_00401d60(this,param_1);
  if (param_1 == 0) {
    piVar2 = (int *)**(undefined4 **)((int)this + 0x6c);
    piVar1 = piVar2 + 1;
    *(undefined1 *)(piVar2 + 5) = 1;
    piVar2[3] = (int)piVar1;
    while( true ) {
      iVar3 = *(int *)piVar2[3];
      if (iVar3 == 0) break;
      if (((int *)piVar2[3] == piVar1) && ((char)piVar2[5] != '\0')) {
        *(undefined1 *)(piVar2 + 5) = 0;
      }
      else {
        piVar2[3] = iVar3;
      }
      if (*(int *)piVar2[3] == 0) break;
      piVar4 = *(int **)(*(int *)piVar2[3] + 4);
      piVar2[6] = (int)piVar4;
      (**(code **)(*piVar4 + 0x48))();
    }
    piVar2[3] = (int)piVar1;
    *piVar1 = 0;
    *(undefined4 *)*piVar2 = 0;
    *(int *)(*piVar2 + 8) = *piVar2 + 0x10;
    *(undefined1 *)((int)this + 0x78) = 0;
  }
  return;
}

