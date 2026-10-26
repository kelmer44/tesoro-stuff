/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00401de0
 */


void __thiscall
tesoro_play_object_animation
          (void *this,byte param_1,ushort param_2,int param_3,byte param_4,char param_5)

{
  void *this_00;
  
  *(byte *)((int)this + 0x32) = param_1;
  this_00 = *(void **)(*(int *)((int)this + 0x34) + (uint)param_1 * 4);
  *(void **)((int)this + 0x38) = this_00;
  FUN_00401740(this_00,param_2,param_3,param_4,param_5);
  *(undefined1 *)((int)this + 0xc) = 0;
  return;
}

