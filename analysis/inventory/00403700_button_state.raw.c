/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00403700
 */


void __thiscall button_state(void *this,int param_1,int param_2)

{
  undefined1 *puVar1;
  byte bVar2;
  DWORD DVar3;
  int iVar4;
  
  DVar3 = GetTickCount();
  iVar4 = 0;
  if (0 < param_2) {
    do {
      puVar1 = (undefined1 *)(iVar4 + *(int *)((int)this + 0x1c));
      bVar2 = *(byte *)(iVar4 + *(int *)((int)this + 0x1c));
      if (*(char *)(iVar4 + param_1) == '\x04') {
        if (bVar2 == 0) {
          *(DWORD *)(*(int *)((int)this + 0x14) + iVar4 * 4) = DVar3;
          *(undefined1 *)(iVar4 + *(int *)((int)this + 0x1c)) = 8;
        }
        else if ((bVar2 == 8) || (bVar2 == 0x10)) {
          *puVar1 = 4;
        }
        else if (bVar2 == 2) {
          *puVar1 = 0x10;
        }
        else if ((bVar2 == 4) &&
                (*(uint *)((int)this + 0x10) <
                 DVar3 - *(int *)(*(int *)((int)this + 0x14) + iVar4 * 4))) {
          *puVar1 = 0x20;
        }
      }
      else if (bVar2 == 1) {
        *puVar1 = 2;
      }
      else if ((bVar2 == 2) &&
              (*(uint *)((int)this + 0xc) < DVar3 - *(int *)(*(int *)((int)this + 0x14) + iVar4 * 4)
              )) {
        *puVar1 = 0;
      }
      else if (3 < bVar2) {
        *(DWORD *)(*(int *)((int)this + 0x14) + iVar4 * 4) = DVar3;
        *(undefined1 *)(iVar4 + *(int *)((int)this + 0x1c)) = 1;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < param_2);
  }
  return;
}

