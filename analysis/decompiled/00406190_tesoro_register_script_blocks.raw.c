/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00406190
 */


void __thiscall tesoro_register_script_blocks(void *this,int param_1,undefined2 param_2)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 *this_00;
  int unaff_EBX;
  undefined4 unaff_EBP;
  undefined4 unaff_ESI;
  undefined4 unaff_EDI;
  int in_stack_fffffff4;
  uint local_8;
  int local_4;
  
  FUN_00403320(DAT_00442064,param_1 + -4);
  piVar1 = *(int **)((int)DAT_00442064 + 4);
  FUN_0041b4c0(&stack0xfffffff4,1,4,piVar1);
  do {
    iVar2 = piVar1[1];
    piVar1[1] = iVar2 + -1;
    if (iVar2 + -1 < 0) {
      uVar3 = FUN_0041c030(piVar1);
    }
    else {
      uVar3 = (uint)*(byte *)*piVar1;
      *piVar1 = (int)((byte *)*piVar1 + 1);
    }
    if (uVar3 != 0x2e) {
      return;
    }
    puVar4 = FUN_0040db40(0xd8,1);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = (undefined4 *)0x0;
    }
    else {
      puVar4 = tesoro_parse_script_block
                         ((char)piVar1,param_2,(short)unaff_EDI,unaff_ESI,(char)unaff_EBP,unaff_EBX,
                          in_stack_fffffff4);
    }
    switch(puVar4[0x35]) {
    case 1:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) {
LAB_004066ec:
        this_00 = (undefined4 *)0x0;
      }
      else {
        FUN_0040ec90(this_00);
        *this_00 = &PTR_FUN_00428078;
        FUN_004067a0(this_00,(int)puVar4);
        *this_00 = &PTR_FUN_00428a50;
      }
      break;
    case 2:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004289d8;
      break;
    case 3:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428960;
      break;
    case 4:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004288e8;
      break;
    case 5:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428870;
      break;
    case 6:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004287f8;
      break;
    case 7:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428780;
      break;
    case 8:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428708;
      break;
    case 9:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428690;
      break;
    case 10:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428618;
      break;
    case 0xb:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004285a0;
      break;
    case 0xc:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428528;
      break;
    case 0xd:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004284b0;
      break;
    case 0xe:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428438;
      break;
    case 0xf:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004283c0;
      break;
    case 0x10:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428348;
      break;
    case 0x11:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004282d0;
      break;
    case 0x12:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_00404630(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428258;
      break;
    case 0x13:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_00404630(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004281e0;
      break;
    case 0x14:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_00404630(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_00428168;
      break;
    case 0x15:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_00404630(this_00,(int)puVar4);
      *this_00 = &PTR_FUN_004280f0;
      break;
    default:
      this_00 = FUN_0040db40(0xd8,1);
      if (this_00 == (undefined4 *)0x0) goto LAB_004066ec;
      FUN_0040ec90(this_00);
      *this_00 = &PTR_FUN_00428078;
      FUN_004067a0(this_00,(int)puVar4);
    }
    *(undefined4 **)((int)this + *(int *)((int)this + 0x2ac) * 4 + 0x54) = this_00;
    *(int *)((int)this + 0x2ac) = *(int *)((int)this + 0x2ac) + 1;
    FUN_0041b490(piVar1,(ulonglong *)&local_8);
    if (-1 < local_4) {
      if (0 < local_4) {
        return;
      }
      if ((uint)(in_stack_fffffff4 + -8 + param_1) < local_8) {
        return;
      }
    }
  } while( true );
}

