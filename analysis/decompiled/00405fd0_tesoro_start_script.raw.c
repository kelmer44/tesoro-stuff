/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00405fd0
 */


undefined4 __thiscall
tesoro_start_script(void *this,int param_1,int param_2,int param_3,void *param_4)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  
  iVar3 = 0;
  if (0 < *(int *)((int)this + 0xb8)) {
    do {
      uVar1 = tesoro_find_scene_object
                        (param_4,*(int *)(*(int *)((int)this + 0xb4) + iVar3 * 8),
                         *(int *)(*(int *)((int)this + 0xb4) + 4 + iVar3 * 8));
      iVar3 = iVar3 + 1;
      *(undefined4 *)(*(int *)((int)this + 0xb0) + -4 + iVar3 * 4) = uVar1;
    } while (iVar3 < *(int *)((int)this + 0xb8));
  }
  iVar3 = 0;
  *(undefined4 *)((int)this + 0xa4) = 0xffffffff;
  *(undefined4 *)((int)this + 0xa0) = 0xffffffff;
  if (0 < *(int *)((int)this + 200)) {
    piVar2 = *(int **)((int)this + 0xc4);
    do {
      if ((*piVar2 == param_1) && (piVar2[1] == param_2)) {
        uVar1 = *(undefined4 *)(*(int *)((int)this + 0xc4) + 8 + iVar3 * 0xc);
        goto LAB_0040604d;
      }
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 3;
    } while (iVar3 < *(int *)((int)this + 200));
  }
  uVar1 = 0;
LAB_0040604d:
  *(undefined4 *)((int)this + 0xcc) = uVar1;
  *(void **)((int)this + 0x40) = param_4;
  *(int *)((int)this + 0xd0) = param_3;
  *(undefined1 *)((int)this + 0x8c) = 0;
  FUN_0040fa10(DAT_00442104,this,0x1e);
  (**(code **)(*DAT_00442128 + 0x20))(0);
  tesoro_execute_script((int *)this);
  return 3;
}

