/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00407b80
 */


void __thiscall
tesoro_place_jim_in_scene(void *this,void *param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  
  if (*(int *)((int)this + 0x40) != 0) {
    piVar1 = *(int **)(*(int *)((int)this + 0x40) + 0x7c);
    piVar1[1] = 0;
    piVar1[3] = (int)(piVar1 + 1);
    *(undefined4 *)*piVar1 = 0;
    *(int *)(*piVar1 + 8) = *piVar1 + 0x10;
    FUN_0040f780(*(void **)(*(int *)((int)this + 0x40) + 0x74),(int)this);
    FUN_00410530(*(void **)((int)this + 0x3c),DAT_004420fc);
    FUN_0040fb20(DAT_00442104,*(int *)((int)this + 0x40));
  }
  *(void **)((int)this + 0x40) = param_1;
  *(int *)((int)this + 0x3c) = param_2;
  FUN_0040f800(*(void **)((int)param_1 + 0x74),this);
  piVar1 = *(int **)(*(int *)((int)this + 0x40) + 0x7c);
  piVar1[1] = 0;
  piVar1[3] = (int)(piVar1 + 1);
  *(undefined4 *)*piVar1 = 0;
  *(int *)(*piVar1 + 8) = *piVar1 + 0x10;
  FUN_004104a0(*(void **)((int)this + 0x3c),(int)this);
  tesoro_update_scene_view(param_1,*(int *)(DAT_004420fc + 0x4c),*(int *)(DAT_004420fc + 0x50),0);
  FUN_00402230(*(void **)((int)param_1 + 0x80),DAT_004420fc);
  FUN_0040f900(DAT_00442104,*(undefined4 *)((int)this + 0x40),0x14);
  FUN_00408800((int)param_1);
  iVar2 = 0;
  if (*(char *)((int)this + 0x33) != '\0') {
    do {
      FUN_00401510(*(int **)(*(int *)((int)this + 0x34) + iVar2 * 4));
      FUN_00401010();
      iVar2 = iVar2 + 1;
    } while (iVar2 < (int)(uint)*(byte *)((int)this + 0x33));
  }
  iVar2 = *(int *)((int)param_1 + 0x98);
  *(int *)((int)this + 0x4c) = param_3;
  *(uint *)((int)this + 0x68) = (uint)(iVar2 != 8) << 10 | *(uint *)((int)this + 0x68) & 0xfffffbff;
  *(int *)((int)this + 0x54) = param_3;
  *(int *)((int)this + 0x58) = param_4;
  *(int *)((int)this + 0x50) = param_4;
  tesoro_update_scene_view(param_1,param_3,param_4,0);
                    /* WARNING: Load size is inaccurate */
  (**(code **)(*this + 0x10))();
  return;
}

