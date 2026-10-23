/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00404210
 */


void __fastcall tesoro_audio_service(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar2 = 0;
  if (0 < *(int *)(param_1 + 0x18)) {
    puVar3 = (undefined4 *)(param_1 + 8);
    do {
      uVar1 = FUN_004043e0((undefined4 *)*puVar3);
      if ((char)uVar1 == '\0') {
        uVar1 = *puVar3;
        *puVar3 = *(undefined4 *)(param_1 + 4 + *(int *)(param_1 + 0x18) * 4);
        *(undefined4 *)(param_1 + 4 + *(int *)(param_1 + 0x18) * 4) = uVar1;
        iVar2 = iVar2 + -1;
        *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + -1;
        puVar3 = puVar3 + -1;
      }
      iVar2 = iVar2 + 1;
      puVar3 = puVar3 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x18));
  }
  return;
}

