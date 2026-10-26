/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00417e90
 */


undefined4 __thiscall f417e90(void *this,uint param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  void *this_00;
  ulonglong uVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  uint *puVar12;
  char *pcVar13;
  void *pvVar14;
  int local_18;
  int local_10;
  int local_8;
  
  if (param_1 == 0x17) {
    uVar7 = tesoro_encode_name(0x42e7bc);
    pcVar13 = s_FICOCO_0042e7ac;
  }
  else {
    if (param_1 != 0x1a) {
      if (param_1 != 0x1b) {
        uVar1 = FUN_0040f480(this,param_1);
        return uVar1;
      }
      uVar7 = tesoro_encode_name(0x42e784);
      uVar8 = tesoro_encode_name(0x42e6b0);
      uVar9 = tesoro_encode_name(0x42e6a4);
      uVar10 = tesoro_encode_name(0x42d2a8);
      uVar11 = tesoro_encode_name(0x42e6e0);
      iVar2 = tesoro_find_scene_object(DAT_004421ec,(int)uVar7,(int)(uVar7 >> 0x20));
      iVar3 = tesoro_find_scene_object(DAT_004421ec,(int)uVar8,(int)(uVar8 >> 0x20));
      local_18 = (int)uVar9;
      iVar4 = tesoro_find_scene_object(DAT_004421ec,local_18,(int)(uVar9 >> 0x20));
      local_10 = (int)uVar10;
      iVar5 = tesoro_find_scene_object(DAT_004421ec,local_10,(int)(uVar10 >> 0x20));
      local_8 = (int)uVar11;
      iVar6 = tesoro_find_scene_object(DAT_004421ec,local_8,(int)(uVar11 >> 0x20));
      param_1 = CONCAT31(param_1._1_3_,
                         (byte)(*(uint *)(iVar6 + 0x68) >> 1) | (byte)(*(uint *)(iVar2 + 0x68) >> 1)
                         | (byte)(*(uint *)(iVar3 + 0x68) >> 1) |
                         (byte)(*(uint *)(iVar4 + 0x68) >> 1) | (byte)(*(uint *)(iVar5 + 0x68) >> 1)
                        ) & 0xffffff01;
      uVar7 = tesoro_encode_name(0x42e7bc);
      uVar8 = tesoro_encode_name(0x42e78c);
      pvVar14 = *(void **)((int)this + 0x40);
      puVar12 = &param_1;
      goto LAB_00418020;
    }
    uVar7 = tesoro_encode_name(0x42e7bc);
    pcVar13 = s_FICANTO_0042e7b4;
  }
  uVar8 = tesoro_encode_name((int)pcVar13);
  pvVar14 = *(void **)((int)this + 0x40);
  puVar12 = (uint *)0x0;
LAB_00418020:
  this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar8,(int)(uVar8 >> 0x20));
  tesoro_start_script(this_00,(int)uVar7,(int)(uVar7 >> 0x20),(int)puVar12,pvVar14);
  return 1;
}

