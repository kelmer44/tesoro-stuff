/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00404110
 */


undefined4 __thiscall
tesoro_play_wav(void *this,int param_1,undefined4 param_2,undefined4 param_3,undefined2 param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  char local_2c [12];
  char local_20 [8];
  short local_18;
  undefined4 uStack_c;
  undefined4 local_8;
  undefined4 local_4;
  
  puVar1 = *(undefined4 **)((int)this + *(int *)((int)this + 0x18) * 4 + 8);
  *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
  piVar2 = (int *)FUN_004032e0(DAT_00442064,param_1);
  puVar1[8] = piVar2;
  FUN_0041b4c0(local_2c,1,0x2c,piVar2);
  iVar3 = _strncmp(local_2c,&DAT_0042d09c,4);
  if (iVar3 == 0) {
    iVar3 = _strncmp(local_20,&DAT_0042d094,4);
    if (iVar3 == 0) {
      iVar3 = _strncmp((char *)&local_8,&DAT_0042d08c,4);
      if ((iVar3 == 0) && (local_18 == 1)) {
        puVar1[3] = local_4;
        uVar4 = FUN_00403300((int)DAT_00442064);
        puVar1[1] = uVar4;
        FUN_004042c0(puVar1);
      }
    }
  }
  *(undefined2 *)(puVar1 + 9) = param_4;
  iVar3 = *(int *)*puVar1;
  *(undefined1 *)(puVar1 + 10) = 1;
  (**(code **)(iVar3 + 0x30))((int *)*puVar1,0,0,*(undefined1 *)((int)puVar1 + 0x27));
  (**(code **)(*(int *)*puVar1 + 0x3c))((int *)*puVar1,local_8);
  (**(code **)(*(int *)*puVar1 + 0x40))((int *)*puVar1,uStack_c);
  iVar3 = *(int *)((int)this + 0x14) + 1;
  *(int *)((int)this + 0x14) = iVar3;
  puVar1[0xb] = iVar3;
  return *(undefined4 *)((int)this + 0x14);
}

