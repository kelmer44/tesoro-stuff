/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00402300
 */


undefined4 * __thiscall
tesoro_input_402300(void *this,undefined4 *param_1,int param_2,int param_3,byte *param_4)

{
  void *pvVar1;
  undefined4 *puVar2;
  
  tesoro_initialize_scenery(this,param_1,param_2,param_3,param_4);
  *(undefined ***)this = &PTR_LAB_00428038;
  *(undefined1 *)((int)this + 0x78) = 0;
  pvVar1 = (void *)FUN_0040f650(4,1);
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_0040f590(pvVar1,0x1e,1,(int *)0x0);
  }
  *(undefined4 **)((int)this + 0x6c) = puVar2;
  pvVar1 = (void *)FUN_0040f650(4,1);
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_0040f590(pvVar1,0x1e,1,(int *)0x0);
  }
  *(undefined4 **)((int)this + 0x70) = puVar2;
  puVar2 = FUN_0040db40(0x130,1);
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_00402240(puVar2);
  }
  *(undefined4 **)((int)this + 0x74) = puVar2;
  *(undefined2 *)((int)this + 0x5e) = 0;
  pvVar1 = DAT_0044206c;
  *(undefined4 *)((int)DAT_0044206c + 0x20) = 0xfffffec0;
  *(undefined4 *)((int)pvVar1 + 0x24) = 0xffffff10;
  *(undefined4 *)((int)pvVar1 + 0x28) = 0x13f;
  *(undefined4 *)((int)pvVar1 + 0x2c) = 0xef;
  FUN_00403a70((int)DAT_0044206c);
  FUN_004036e0(DAT_0044206c,200,20000);
  *(uint *)((int)this + 0x68) = *(uint *)((int)this + 0x68) | 8;
  return (undefined4 *)this;
}

