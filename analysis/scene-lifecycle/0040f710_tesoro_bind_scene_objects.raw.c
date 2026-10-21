/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040f710
 */


void __thiscall tesoro_bind_scene_objects(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  
                    /* WARNING: Load size is inaccurate */
  iVar1 = *this;
  *(undefined1 *)(iVar1 + 0x14) = 1;
  *(int *)(iVar1 + 0xc) = iVar1 + 4;
  do {
                    /* WARNING: Load size is inaccurate */
    iVar1 = *this;
    iVar2 = **(int **)(iVar1 + 0xc);
    if (iVar2 == 0) {
LAB_0040f75a:
      bVar3 = false;
    }
    else {
      if ((*(int **)(iVar1 + 0xc) == (int *)(iVar1 + 4)) && (*(char *)(iVar1 + 0x14) != '\0')) {
        *(undefined1 *)(iVar1 + 0x14) = 0;
      }
      else {
        *(int *)(iVar1 + 0xc) = iVar2;
      }
      if (**(int **)(iVar1 + 0xc) == 0) goto LAB_0040f75a;
      *(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(**(int **)(iVar1 + 0xc) + 4);
      bVar3 = true;
    }
    if (!bVar3) {
      return;
    }
                    /* WARNING: Load size is inaccurate */
    tesoro_bind_object_to_scene(*(void **)(*this + 0x18),param_1);
  } while( true );
}

