/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00410420
 */


void __thiscall
tesoro_initialize_scene_group
          (void *this,int param_1,int param_2,undefined4 param_3,undefined2 param_4)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  
  *(undefined4 *)((int)this + 4) = *(undefined4 *)(param_2 + 0x24);
  *(undefined2 *)((int)this + 8) = param_4;
  *(undefined2 *)((int)this + 10) = *(undefined2 *)(param_2 + 0x22);
  piVar2 = FUN_0040db40(0x1c,1);
  *(undefined4 *)this = param_3;
  *(int **)((int)this + 0xc) = piVar2;
  iVar1 = *(int *)(param_1 + 0x10);
  piVar2[4] = iVar1;
  puVar3 = FUN_0040db40(iVar1 * 8 + 0x18,1);
  *piVar2 = (int)puVar3;
  piVar2[3] = (int)(piVar2 + 1);
  piVar2[1] = 0;
  *puVar3 = 0;
  *(int *)(*piVar2 + 8) = *piVar2 + 0x10;
  return;
}

