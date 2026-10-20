/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004080e0
 */


undefined4 *
tesoro_load_scene(int *param_1,undefined4 param_2,int param_3,int param_4,int param_5,
                 undefined4 param_6,undefined4 param_7,undefined8 param_8,undefined4 param_9,
                 undefined4 param_10,undefined1 param_11,uint param_12)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  void *pvVar4;
  int iVar5;
  uint *puVar6;
  undefined4 *this;
  int iVar7;
  ushort in_stack_00000058;
  ushort in_stack_0000005a;
  int in_stack_000013ec;
  uint in_stack_000013f0;
  int iVar8;
  uint uVar9;
  
  FUN_0041b3c0();
  FUN_00401bc0(this);
  *this = &PTR_FUN_00428bf8;
  piVar1 = (int *)FUN_004032e0(DAT_00442064,in_stack_000013ec + 4);
  FUN_0041b4c0((char *)&param_2,0xc,1,piVar1);
  this[0x22] = param_3;
  this[0x21] = param_2;
  puVar2 = FUN_0040db40(0x40,1);
  if (puVar2 == (undefined4 *)0x0) {
    uVar3 = 0;
  }
  else {
    uVar3 = FUN_00402110(puVar2);
  }
  this[0x20] = uVar3;
  piVar1 = FUN_0040db40(0x1c,1);
  if (piVar1 == (int *)0x0) {
    piVar1 = (int *)0x0;
  }
  else {
    piVar1[4] = 0x14;
    puVar2 = FUN_0040db40(0xb8,1);
    *piVar1 = (int)puVar2;
    piVar1[3] = (int)(piVar1 + 1);
    piVar1[1] = 0;
    *puVar2 = 0;
    *(int *)(*piVar1 + 8) = *piVar1 + 0x10;
  }
  this[0x1f] = piVar1;
  pvVar4 = (void *)FUN_0040f650(4,1);
  if (pvVar4 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_0040f590(pvVar4,param_4 + 5,1,(int *)0x0);
  }
  this[0x1d] = puVar2;
  piVar1 = FUN_0040db40(0x34,1);
  if (piVar1 == (int *)0x0) {
    piVar1 = (int *)0x0;
  }
  else {
    piVar1[7] = 0x32;
    puVar2 = FUN_0040db40(0x424,1);
    *piVar1 = (int)puVar2;
    piVar1[6] = (int)(piVar1 + 1);
    piVar1[1] = 0;
    *puVar2 = 0;
    *(int *)(*piVar1 + 0x14) = *piVar1 + 0x28;
  }
  this[0x1e] = piVar1;
  puVar2 = FUN_0040db40(this[0x22] << 2,1);
  this[0x1b] = puVar2;
  puVar2 = FUN_0040db40(this[0x21] << 4,1);
  this[0x1c] = puVar2;
  iVar7 = 0;
  if (0 < param_3) {
    do {
      pvVar4 = (void *)FUN_0040f650(0x40,1);
      if (pvVar4 == (void *)0x0) {
        puVar6 = (uint *)0x0;
      }
      else {
        iVar8 = 1;
        uVar9 = in_stack_000013f0;
        iVar5 = FUN_00403300((int)DAT_00442064);
        puVar6 = tesoro_load_animation_bank(pvVar4,iVar5,iVar8,uVar9);
      }
      iVar7 = iVar7 + 1;
      *(uint **)(this[0x1b] + -4 + iVar7 * 4) = puVar6;
    } while (iVar7 < param_3);
  }
  param_1 = FUN_0040db40(0x1c,1);
  if (param_1 == (int *)0x0) {
    param_1 = (int *)0x0;
  }
  else {
    param_1[4] = param_4 + 5;
    puVar2 = FUN_0040db40((param_4 + 5) * 8 + 0x18,1);
    *param_1 = (int)puVar2;
    param_1[3] = (int)(param_1 + 1);
    param_1[1] = 0;
    *puVar2 = 0;
    *(int *)(*param_1 + 8) = *param_1 + 0x10;
  }
  iVar7 = 0;
  piVar1 = *(int **)((int)DAT_00442064 + 4);
  if (0 < (int)this[0x21]) {
    iVar8 = 0;
    iVar5 = 0;
    do {
      FUN_0041b4c0(&stack0x00000038,0x28,1,piVar1);
      FUN_00410420((void *)(iVar8 + this[0x1c]),(int)param_1,(int)&stack0x00000038,iVar7,
                   (short)iVar5);
      param_5 = iVar5 + (uint)in_stack_0000005a;
      iVar5 = 0;
      if (in_stack_00000058 != 0) {
        do {
          FUN_0041b4c0((char *)&param_6,0x20,1,piVar1);
          FUN_0041b4c0(&stack0x00000060,param_12,1,piVar1);
          FUN_0041b4c0(&stack0x00000060 + param_12,param_8._3_4_ & 0xff,1,piVar1);
          puVar2 = tesoro_create_scene_object(this,&param_6,iVar7,&stack0x00000060);
          if (puVar2 != (undefined4 *)0x0) {
            *(undefined1 *)(puVar2 + 0x17) = param_11;
            FUN_0040f800((void *)this[0x1d],puVar2);
          }
          iVar5 = iVar5 + 1;
        } while (iVar5 < (int)(uint)in_stack_00000058);
      }
      iVar7 = iVar7 + 1;
      iVar8 = iVar8 + 0x10;
      iVar5 = param_5;
    } while (iVar7 < (int)this[0x21]);
  }
  FUN_00402200((void *)this[0x20],0xfffffec0,0xef,0x13f,0xffffff10);
  iVar7 = FUN_00413a10(DAT_00442120);
  this[0x23] = iVar7;
  *(undefined1 *)(this + 0x25) = 0;
  return this;
}

