/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00419b90
 */


void __thiscall tesoro_native_type_12(void *this,int param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  int *piVar3;
  void *pvVar4;
  int iVar5;
  ulonglong uVar6;
  byte bVar7;
  ushort uVar8;
  byte bVar9;
  char cVar10;
  
  pvVar1 = DAT_00442154;
  pvVar4 = DAT_0044214c;
  if (param_1 == 0) {
    *(void **)((int)this + 0x40) = DAT_0044214c;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar4,*(int *)((int)pvVar4 + 0x70),0x28,-0x93);
    iVar5 = 0;
    if (0 < *(int *)((int)this + 0xb8)) {
      do {
        uVar2 = tesoro_find_scene_object
                          (*(void **)((int)this + 0x40),
                           *(int *)(*(int *)((int)this + 0xb4) + iVar5 * 8),
                           *(int *)(*(int *)((int)this + 0xb4) + 4 + iVar5 * 8));
        iVar5 = iVar5 + 1;
        *(undefined4 *)(*(int *)((int)this + 0xb0) + -4 + iVar5 * 4) = uVar2;
      } while (iVar5 < *(int *)((int)this + 0xb8));
    }
    uVar6 = tesoro_encode_name(0x42d4fc);
    piVar3 = (int *)tesoro_find_scene_object(DAT_0044214c,(int)uVar6,(int)(uVar6 >> 0x20));
    (**(code **)(*piVar3 + 0x20))();
    uVar6 = tesoro_encode_name(0x42e3d0);
    iVar5 = 1;
    uVar8 = 1;
  }
  else {
    if (param_1 != 1) {
      return;
    }
    *(void **)((int)this + 0x40) = DAT_00442154;
    tesoro_place_jim_in_scene(DAT_004420fc,pvVar1,*(int *)((int)pvVar1 + 0x70),0x104,-0x47);
    iVar5 = 0;
    if (0 < *(int *)((int)this + 0xb8)) {
      do {
        uVar2 = tesoro_find_scene_object
                          (*(void **)((int)this + 0x40),
                           *(int *)(*(int *)((int)this + 0xb4) + iVar5 * 8),
                           *(int *)(*(int *)((int)this + 0xb4) + 4 + iVar5 * 8));
        iVar5 = iVar5 + 1;
        *(undefined4 *)(*(int *)((int)this + 0xb0) + -4 + iVar5 * 4) = uVar2;
      } while (iVar5 < *(int *)((int)this + 0xb8));
    }
    uVar6 = tesoro_encode_name(0x42d4fc);
    piVar3 = (int *)tesoro_find_scene_object(DAT_0044214c,(int)uVar6,(int)(uVar6 >> 0x20));
    (**(code **)(*piVar3 + 0x20))();
    uVar6 = tesoro_encode_name(0x42e3d0);
    iVar5 = 0;
    uVar8 = 0;
  }
  cVar10 = '\0';
  bVar9 = 0;
  bVar7 = 0;
  pvVar4 = (void *)tesoro_find_scene_object(DAT_0044214c,(int)uVar6,(int)(uVar6 >> 0x20));
  FUN_00401de0(pvVar4,bVar7,uVar8,iVar5,bVar9,cVar10);
  return;
}

