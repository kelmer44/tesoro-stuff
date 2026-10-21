/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401aa0
 */


undefined4 * __thiscall
tesoro_initialize_base_object
          (void *this,undefined4 *param_1,int param_2,int param_3,int param_4,uint param_5)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  ulonglong uVar9;
  
  *(undefined ***)this = &PTR_FUN_00428000;
  *(undefined1 *)((int)this + 0x5d) = 0;
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 2;
  *(undefined4 *)((int)this + 0x4c) = *param_1;
  *(undefined4 *)((int)this + 0x50) = param_1[1];
  sVar2 = *(short *)(param_1 + 2);
  sVar3 = *(short *)(param_2 + 8);
  *(int *)((int)this + 0x3c) = param_2;
  *(short *)((int)this + 0x5e) = sVar2 + sVar3 + 5;
  uVar9 = tesoro_encode_name((int)(param_1 + 3));
  *(ulonglong *)((int)this + 0x44) = uVar9;
  bVar1 = *(byte *)((int)param_1 + 0xb);
  *(byte *)((int)this + 0x33) = bVar1;
  iVar4 = param_1[7];
  puVar5 = FUN_0040db40((uint)bVar1 << 2,1);
  *(undefined4 **)((int)this + 0x34) = puVar5;
  iVar8 = 0;
  if (*(char *)((int)this + 0x33) != '\0') {
    do {
      iVar6 = FUN_0040f650(0x40,1);
      if (iVar6 == 0) {
        uVar7 = 0;
      }
      else {
        uVar7 = FUN_00401000(iVar6);
      }
      *(undefined4 *)(*(int *)((int)this + 0x34) + iVar8 * 4) = uVar7;
      tesoro_clone_animation_bank
                (*(void **)(*(int *)((int)this + 0x34) + iVar8 * 4),
                 *(undefined4 **)(param_3 + (uint)*(byte *)(iVar8 + iVar4 + param_4) * 4));
      iVar8 = iVar8 + 1;
      *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 0x10;
    } while (iVar8 < (int)(uint)*(byte *)((int)this + 0x33));
  }
  uVar7 = **(undefined4 **)((int)this + 0x34);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)((int)this + 0x4c);
  *(undefined4 *)((int)this + 0x58) = *(undefined4 *)((int)this + 0x50);
  *(undefined4 *)((int)this + 0x38) = uVar7;
  *(undefined1 *)((int)this + 0x32) = 0;
  *(short *)((int)this + 100) = (short)param_5;
  if ((short)param_5 == 0) {
    *(undefined4 *)((int)this + 0x60) = 0;
    return (undefined4 *)this;
  }
  puVar5 = FUN_0040db40(param_5,1);
  *(undefined4 **)((int)this + 0x60) = puVar5;
  return (undefined4 *)this;
}

