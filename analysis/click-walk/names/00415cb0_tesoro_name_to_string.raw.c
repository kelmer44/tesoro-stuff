/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415cb0
 */


void __cdecl tesoro_name_to_string(uint param_1,uint param_2,int param_3)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;
  int iVar4;
  
  pcVar3 = (char *)(param_3 + 5);
  iVar4 = 6;
  do {
    bVar1 = (byte)param_1 & 0x1f;
    if (bVar1 < 0x1b) {
      if ((param_1 & 0x1f) != 0) {
        cVar2 = bVar1 + 0x40;
        goto LAB_00415cdd;
      }
      *pcVar3 = ' ';
    }
    else {
      cVar2 = bVar1 + 0x15;
LAB_00415cdd:
      *pcVar3 = cVar2;
    }
    param_1 = param_1 >> 5;
    pcVar3 = pcVar3 + -1;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  pcVar3 = (char *)(param_3 + 0xb);
  iVar4 = 6;
  do {
    bVar1 = (byte)param_2 & 0x1f;
    if (bVar1 < 0x1b) {
      if ((param_2 & 0x1f) != 0) {
        cVar2 = bVar1 + 0x40;
        goto LAB_00415d0d;
      }
      *pcVar3 = ' ';
    }
    else {
      cVar2 = bVar1 + 0x15;
LAB_00415d0d:
      *pcVar3 = cVar2;
    }
    param_2 = param_2 >> 5;
    pcVar3 = pcVar3 + -1;
    iVar4 = iVar4 + -1;
    if (iVar4 == 0) {
      *(undefined1 *)(param_3 + 0xc) = 0;
      return;
    }
  } while( true );
}

