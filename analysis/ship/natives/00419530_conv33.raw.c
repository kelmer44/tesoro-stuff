/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00419530
 */


void __thiscall conv33(void *this,int param_1)

{
  undefined1 *puVar1;
  void *this_00;
  ulonglong uVar2;
  ulonglong uVar3;
  int iVar4;
  void *pvVar5;
  
  pvVar5 = DAT_00442168;
  if (param_1 == 0) {
    *(void **)((int)this + 0x40) = DAT_00442168;
    tesoro_place_jim_in_scene
              (DAT_004420fc,pvVar5,*(int *)((int)pvVar5 + 0x70),DAT_004420fc[0x13],
               DAT_004420fc[0x14]);
    (**(code **)(*DAT_004420fc + 0x20))(0);
    (**(code **)(*DAT_00442128 + 0x20))(1);
    puVar1 = &DAT_00442279;
    do {
      *puVar1 = 0;
      puVar1 = puVar1 + 8;
    } while ((int)puVar1 < 0x4422b1);
    FUN_00418e90();
    DAT_00442230 = 0;
    DAT_00442228 = 0;
    DAT_00442220 = 0;
    DAT_0044222c = *(undefined1 *)(*(int *)((int)this + 0xd0) + 2);
    *(undefined1 *)(*(int *)((int)this + 0xd0) + 2) = 0xff;
    uVar2 = tesoro_encode_name(0x42d4b4);
    uVar3 = tesoro_encode_name(0x42e8fc);
    pvVar5 = *(void **)((int)this + 0x40);
    iVar4 = *(int *)((int)this + 0xd0);
    this_00 = (void *)FUN_004060c0(DAT_004420b8,(int)uVar3,(int)(uVar3 >> 0x20));
    tesoro_start_script(this_00,(int)uVar2,(int)(uVar2 >> 0x20),iVar4,pvVar5);
  }
  else if (param_1 == 1) {
    (**(code **)(*DAT_00442128 + 0x20))(0);
    return;
  }
  return;
}

