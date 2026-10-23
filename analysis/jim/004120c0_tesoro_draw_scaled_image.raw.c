/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004120c0
 */


void __thiscall
tesoro_draw_scaled_image
          (void *this,undefined4 param_1,undefined4 param_2,undefined2 param_3,undefined4 param_4,
          undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(*(int *)((int)this + 0x664) * 0x20 + 0x24 + (int)this);
  *(int *)((int)this + 0x664) = *(int *)((int)this + 0x664) + 1;
  puVar1[1] = param_1;
  puVar1[2] = param_2;
  *(undefined2 *)(puVar1 + 3) = param_3;
  *puVar1 = param_4;
  puVar1[4] = param_5;
  puVar1[5] = param_6;
  puVar1[6] = param_7;
  puVar1[7] = param_8;
  return;
}

