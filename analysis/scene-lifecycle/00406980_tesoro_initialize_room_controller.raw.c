/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00406980
 */


undefined4 * __thiscall
tesoro_initialize_room_controller
          (void *this,undefined4 *param_1,int param_2,int param_3,undefined4 *param_4)

{
  FUN_00401aa0(this,param_1,param_2,param_3,(int)param_4,0);
  *(undefined ***)this = &PTR_tesoro_resolve_room_floor_00428ac8;
  *(undefined4 *)((int)this + 0x6c) = *param_4;
  *(undefined4 *)((int)this + 0x70) = param_4[1];
  *(undefined4 *)((int)this + 0x74) = param_4[2];
  *(ushort *)((int)this + 0x80) = *(byte *)((int)param_4 + 0xd) + 1;
  *(undefined1 *)((int)this + 0x7d) = *(undefined1 *)((int)param_4 + 0xe);
  *(undefined2 *)((int)this + 0x7e) = *(undefined2 *)((int)param_4 + 0xf);
  return (undefined4 *)this;
}

