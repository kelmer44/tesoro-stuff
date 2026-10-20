/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 0040ed10
 */


undefined4 * __thiscall
tesoro_initialize_interactive_object
          (void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4,uint param_5)

{
  void *this_00;
  int iVar1;
  int iVar2;
  int *piVar3;
  byte *pbVar4;
  int local_1c [7];
  
  FUN_00401aa0(this,param_1,param_2,param_3,(int)param_4,param_5);
  this_00 = *(void **)((int)this + 0x38);
  *(undefined ***)this = &PTR_FUN_0042ba68;
  iVar1 = *(int *)(*(int *)((int)this_00 + 0x1c) + 0x18 +
                  (uint)*(ushort *)((int)this_00 + 0x30) * 0x24);
  *(undefined ***)this = &PTR_FUN_0042bad8;
  *(uint *)((int)this + 0x68) = (iVar1 != 0 | 0x142) << 2 | *(uint *)((int)this + 0x68) & 0xfffffffb
  ;
  if (this_00 != (void *)0x0) {
    FUN_00401740(this_00,(ushort)*param_4,(uint)*param_4,0,'\0');
  }
  *(undefined4 *)((int)this + 0x94) = *(undefined4 *)((int)this + 0x4c);
  *(undefined4 *)((int)this + 0x98) = *(undefined4 *)((int)this + 0x50);
  *(int *)((int)this + 0x94) = *(int *)((int)this + 0x94) + (int)*(short *)(param_4 + 2);
  *(int *)((int)this + 0x98) = *(int *)((int)this + 0x98) + (int)*(short *)(param_4 + 4);
  *(uint *)((int)this + 0x90) = param_4[6] + 1;
  if (param_4[7] == 0) {
    *(undefined4 *)((int)this + 0x6c) = 0;
  }
  else {
    iVar1 = FUN_00413940((int)(param_4 + 7));
    *(int *)((int)this + 0x6c) = iVar1;
  }
  local_1c[0] = 0;
  iVar1 = 6;
  pbVar4 = param_4 + 0x13;
  local_1c[1] = 1;
  local_1c[2] = 3;
  local_1c[3] = 4;
  local_1c[4] = 5;
  local_1c[5] = 2;
  local_1c[6] = 6;
  piVar3 = local_1c;
  do {
    if (*pbVar4 == 0) {
      *(undefined4 *)((int)this + *piVar3 * 4 + 0x70) = 0;
    }
    else {
      iVar2 = FUN_00413940((int)pbVar4);
      *(int *)((int)this + *piVar3 * 4 + 0x70) = iVar2;
    }
    pbVar4 = pbVar4 + 0xc;
    piVar3 = piVar3 + 1;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 0x400;
  return (undefined4 *)this;
}

