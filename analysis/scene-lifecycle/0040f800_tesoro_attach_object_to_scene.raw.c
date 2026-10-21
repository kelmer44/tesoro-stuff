/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f800
 */


void __thiscall tesoro_attach_object_to_scene(void *this,undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  
                    /* WARNING: Load size is inaccurate */
  piVar1 = *this;
  *(undefined1 *)(piVar1 + 5) = 1;
  piVar1[3] = (int)(piVar1 + 1);
  while( true ) {
    iVar2 = *(int *)piVar1[3];
    if (iVar2 == 0) break;
    if (((int *)piVar1[3] == piVar1 + 1) && ((char)piVar1[5] != '\0')) {
      *(undefined1 *)(piVar1 + 5) = 0;
    }
    else {
      piVar1[3] = iVar2;
    }
    if (*(int *)piVar1[3] == 0) break;
    piVar1[6] = *(int *)(*(int *)piVar1[3] + 4);
  }
  piVar3 = (int *)*piVar1;
  piVar4 = (int *)*piVar3;
  if (piVar4 == (int *)0x0) {
    piVar4 = (int *)piVar3[2];
    piVar3[2] = (int)(piVar4 + 2);
  }
  else {
    *piVar3 = *piVar4;
  }
  *piVar4 = *(undefined4 *)piVar1[3];
  *(int **)piVar1[3] = piVar4;
  piVar4[1] = param_1;
  return;
}

