/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00415960
 */


undefined4 tesoro_win_main(HINSTANCE param_1,undefined4 param_2,byte *param_3,int param_4)

{
  byte bVar1;
  bool bVar2;
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  BOOL BVar6;
  uint unaff_ESI;
  char *pcVar7;
  undefined1 unaff_DI;
  tagMSG local_1c;
  
  if (*param_3 == 0) {
    uVar4 = FUN_00415b00('\0');
    if ((char)uVar4 != '\0') {
      return 0;
    }
    while (iVar5 = MessageBoxA((HWND)0x0,s_Por_favor__inserte_el_CD_de_El_T_0042e430,
                               s_CD_no_encontrado_0042e468,0x21), iVar5 != 2) {
      uVar4 = FUN_00415b00('\0');
      if ((char)uVar4 != '\0') {
        return 0;
      }
    }
  }
  else {
    iVar5 = _strncmp((char *)param_3,s_______0042e428,6);
    if (iVar5 == 0) {
      DAT_00442204 = param_3[6];
      bVar2 = FUN_00415880(param_1,param_4);
      if (!bVar2) {
        return 0;
      }
      tesoro_initialize_game(unaff_DI,unaff_ESI);
      do {
        BVar6 = PeekMessageA(&local_1c,(HWND)0x0,0,0,0);
        if (BVar6 != 0) {
          BVar6 = GetMessageA(&local_1c,(HWND)0x0,0,0);
          if (BVar6 == 0) {
            return local_1c.wParam;
          }
          TranslateMessage(&local_1c);
          DispatchMessageA(&local_1c);
        }
        bVar2 = tesoro_main_loop_step();
      } while (bVar2);
      FUN_004144a0();
      return 1;
    }
    pcVar7 = s_Uninstall_0042e41c;
    do {
      bVar1 = *param_3;
      bVar2 = bVar1 < (byte)*pcVar7;
      if (bVar1 != *pcVar7) {
LAB_00415aa6:
        iVar5 = (1 - (uint)bVar2) - (uint)(bVar2 != 0);
        goto LAB_00415aab;
      }
      if (bVar1 == 0) break;
      bVar1 = param_3[1];
      bVar2 = bVar1 < ((byte *)pcVar7)[1];
      if (bVar1 != ((byte *)pcVar7)[1]) goto LAB_00415aa6;
      param_3 = param_3 + 2;
      pcVar7 = (char *)((byte *)pcVar7 + 2);
    } while (bVar1 != 0);
    iVar5 = 0;
LAB_00415aab:
    if (iVar5 == 0) {
      uVar4 = FUN_00415b00('\x01');
      cVar3 = (char)uVar4;
      while( true ) {
        if (cVar3 != '\0') {
          return 0;
        }
        iVar5 = MessageBoxA((HWND)0x0,s_Por_favor__inserte_el_CD_de_El_T_0042e430,
                            s_CD_no_encontrado_0042e468,0x21);
        if (iVar5 == 2) break;
        uVar4 = FUN_00415b00('\x01');
        cVar3 = (char)uVar4;
      }
      return 0xffffffff;
    }
  }
  return 0xffffffff;
}

