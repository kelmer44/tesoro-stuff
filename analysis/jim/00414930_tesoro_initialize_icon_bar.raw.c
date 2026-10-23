/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00414930
 */


undefined4 * __thiscall
tesoro_initialize_icon_bar(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4)

{
  short sVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  undefined4 *puVar5;
  byte *pbVar6;
  int *piVar7;
  ulonglong uVar8;
  undefined4 local_8;
  undefined4 local_4;
  
  pbVar6 = param_4;
  FUN_00401aa0(this,param_1,param_2,param_3,(int)param_4,0);
  *(undefined ***)this = &PTR_FUN_0042bcb0;
  *(byte *)((int)this + 0x70) = *param_4 & 1;
  *(undefined2 *)((int)this + 0x72) = *(undefined2 *)(param_4 + 2);
  piVar4 = FUN_0040db40(0x24,3);
  if (piVar4 == (int *)0x0) {
    piVar4 = (int *)0x0;
  }
  else {
    sVar1 = *(short *)((int)this + 0x72);
    piVar4[5] = (int)sVar1;
    puVar5 = FUN_0040db40((sVar1 + 3) * 0xc,3);
    *piVar4 = (int)puVar5;
    piVar4[4] = (int)(piVar4 + 1);
    piVar4[1] = 0;
    *puVar5 = 0;
    *(int *)(*piVar4 + 0xc) = *piVar4 + 0x18;
  }
  *(int **)((int)this + 0x6c) = piVar4;
  piVar4[4] = (int)(piVar4 + 1);
  *(undefined1 *)(piVar4 + 6) = 1;
  param_4 = (byte *)0x0;
  if (0 < *(short *)((int)this + 0x72)) {
    pbVar6 = pbVar6 + 0x10;
    do {
      uVar8 = tesoro_encode_name((int)pbVar6);
      piVar4 = *(int **)((int)this + 0x6c);
      *(undefined1 *)(piVar4 + 6) = 1;
      piVar4[4] = (int)(piVar4 + 1);
      while( true ) {
        iVar2 = *(int *)piVar4[4];
        if (iVar2 == 0) break;
        if (((int *)piVar4[4] == piVar4 + 1) && ((char)piVar4[6] != '\0')) {
          *(undefined1 *)(piVar4 + 6) = 0;
        }
        else {
          piVar4[4] = iVar2;
        }
        iVar2 = *(int *)piVar4[4];
        if (iVar2 == 0) break;
        piVar4[7] = *(int *)(iVar2 + 4);
        piVar4[8] = *(int *)(iVar2 + 8);
      }
      piVar3 = (int *)*piVar4;
      piVar7 = (int *)*piVar3;
      if (piVar7 == (int *)0x0) {
        piVar7 = (int *)piVar3[3];
        piVar3[3] = (int)(piVar7 + 3);
      }
      else {
        *piVar3 = *piVar7;
      }
      pbVar6 = pbVar6 + 0xc;
      local_8 = (undefined4)uVar8;
      *piVar7 = *(undefined4 *)piVar4[4];
      local_4 = (undefined4)(uVar8 >> 0x20);
      *(int **)piVar4[4] = piVar7;
      piVar7[1] = local_8;
      piVar7[2] = local_4;
      param_4 = param_4 + 1;
    } while ((int)param_4 < (int)*(short *)((int)this + 0x72));
  }
  return (undefined4 *)this;
}

