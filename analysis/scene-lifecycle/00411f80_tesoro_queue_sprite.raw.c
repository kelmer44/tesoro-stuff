/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00411f80
 */


void __thiscall
tesoro_queue_sprite(void *this,int param_1,int param_2,ushort param_3,int param_4,undefined4 param_5
                   )

{
  int iVar1;
  char cVar2;
  int iVar3;
  undefined4 *puVar4;
  short sVar5;
  
  iVar1 = (*(ushort *)(param_4 + 8) - 1) + param_1;
  iVar3 = (*(ushort *)(param_4 + 10) - 1) + param_2;
  if ((((param_1 < 0x280) && (-1 < iVar1)) && (param_2 < 0x1e0)) && (-1 < iVar3)) {
    puVar4 = (undefined4 *)(param_4 + 0x10);
    if (param_2 < 0) {
      puVar4 = puVar4 + param_2 * -2;
      param_2 = 0;
    }
    if (0x1df < iVar3) {
      iVar3 = 0x1df;
    }
    sVar5 = ((short)iVar3 - (short)param_2) + 1;
    if (*(short *)(param_4 + 0xc) == 1) {
      if ((param_1 < 0) || (0x27f < iVar1)) {
        cVar2 = '\x01';
      }
      else {
        cVar2 = '\0';
      }
      FUN_004105a0(puVar4,(undefined4 *)(*(int *)((int)this + 0x670) + param_2 * 2000),sVar5,param_1
                   ,param_3,cVar2);
      return;
    }
    if (*(short *)(param_4 + 0xc) == 3) {
      if ((param_1 < 0) || (0x27f < iVar1)) {
        cVar2 = '\x01';
      }
      else {
        cVar2 = '\0';
      }
      FUN_00410cc0(puVar4,(undefined4 *)(*(int *)((int)this + 0x670) + param_2 * 2000),sVar5,param_1
                   ,param_3,cVar2);
      return;
    }
    if ((param_1 < 0) || (0x27f < iVar1)) {
      cVar2 = '\x01';
    }
    else {
      cVar2 = '\0';
    }
    FUN_004112d0(puVar4,(undefined4 *)(*(int *)((int)this + 0x674) + param_2 * 800),sVar5,param_1,
                 param_3,cVar2,param_5);
  }
  return;
}

