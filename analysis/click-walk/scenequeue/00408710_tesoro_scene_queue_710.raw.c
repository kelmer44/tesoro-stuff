/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00408710
 */


void __thiscall tesoro_scene_queue_710(void *this,int *param_1)

{
  int *piVar1;
  int local_8;
  int local_4;
  
  while( true ) {
    FUN_0041b4c0((char *)&local_8,8,1,param_1);
    if ((local_8 == 0) && (local_4 == 0)) break;
    piVar1 = (int *)FUN_0040f680(*(void **)((int)this + 0x74),local_8,local_4);
    if (piVar1 != (int *)0x0) {
      FUN_0041b4c0((char *)(piVar1 + 0x1a),1,4,param_1);
      (**(code **)(*piVar1 + 0x24))(param_1);
    }
  }
  return;
}

