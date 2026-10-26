/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00416590
 */


undefined4 __thiscall f416590(void *this,int param_1)

{
  undefined4 uVar1;
  int iVar2;
  void *pvVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  int iVar7;
  int iVar8;
  void *pvVar9;
  undefined4 local_10;
  
  if (param_1 == 7) {
    uVar4 = tesoro_encode_name(0x42e54c);
    uVar5 = tesoro_encode_name(0x42e544);
    pvVar9 = *(void **)((int)this + 0x40);
    iVar2 = *(int *)((int)this + 0x60);
    iVar7 = (int)uVar4;
    iVar8 = (int)(uVar4 >> 0x20);
    pvVar3 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar5,(int)(uVar5 >> 0x20));
    uVar1 = tesoro_start_script(pvVar3,iVar7,iVar8,iVar2,pvVar9);
    return uVar1;
  }
  if (param_1 != 0xc) {
    uVar1 = FUN_0040f480(this,param_1);
    return uVar1;
  }
  uVar4 = tesoro_encode_name(0x42e51c);
  uVar5 = tesoro_encode_name(0x42e564);
  uVar6 = tesoro_encode_name(0x42e554);
  pvVar9 = *(void **)((int)this + 0x40);
  iVar2 = tesoro_find_scene_object(DAT_00442148,(int)uVar4,(int)(uVar4 >> 0x20));
  local_10 = (int)uVar6;
  iVar2 = *(int *)(iVar2 + 0x60);
  pvVar3 = (void *)FUN_004060c0(DAT_004420b8,local_10,(int)(uVar6 >> 0x20));
  tesoro_start_script(pvVar3,(int)uVar5,(int)(uVar5 >> 0x20),iVar2,pvVar9);
  return 3;
}

