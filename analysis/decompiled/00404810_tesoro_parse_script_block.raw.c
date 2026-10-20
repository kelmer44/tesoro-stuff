/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00404810
 */


undefined4 *
tesoro_parse_script_block
          (undefined1 param_1,undefined2 param_2,undefined2 param_3,int param_4,undefined1 param_5,
          int param_6,uint param_7)

{
  int *piVar1;
  undefined4 uVar2;
  byte bVar3;
  undefined4 *puVar4;
  uint uVar5;
  byte *pbVar6;
  int iVar7;
  uint uVar8;
  undefined4 *puVar9;
  undefined4 *extraout_ECX;
  int iVar10;
  char *pcVar11;
  byte *pbVar12;
  bool bVar13;
  ulonglong uVar14;
  int *in_stack_00001efc;
  
  FUN_0041b3c0();
  FUN_0040ec90(extraout_ECX);
  *extraout_ECX = &PTR_FUN_00428078;
  extraout_ECX[0x30] = 0;
  extraout_ECX[0x32] = 0;
  FUN_0041c120(in_stack_00001efc,&DAT_0042d1ec);
  uVar14 = tesoro_encode_name((int)&param_7);
  *(ulonglong *)(extraout_ECX + 0x11) = uVar14;
  iVar10 = in_stack_00001efc[1];
  in_stack_00001efc[1] = iVar10 + -1;
  if (iVar10 + -1 < 0) {
    uVar5 = FUN_0041c030(in_stack_00001efc);
  }
  else {
    uVar5 = (uint)*(byte *)*in_stack_00001efc;
    *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
  }
  if (uVar5 == 0x20) {
    FUN_0041c120(in_stack_00001efc,&DAT_0042d1e8);
  }
  else {
    extraout_ECX[0x35] = 0;
  }
  piVar1 = extraout_ECX + 0x2e;
  FUN_0041c120(in_stack_00001efc,&DAT_0042d1e8);
  puVar4 = FUN_0040db40(*piVar1 << 3,1);
  extraout_ECX[0x2d] = puVar4;
  puVar4 = FUN_0040db40(*piVar1 << 2,1);
  extraout_ECX[0x2c] = puVar4;
  iVar10 = 0;
  if (0 < *piVar1) {
    do {
      FUN_0041c120(in_stack_00001efc,&DAT_0042d1e4);
      uVar14 = tesoro_encode_name((int)&param_7);
      iVar7 = extraout_ECX[0x2d];
      *(int *)(iVar7 + iVar10 * 8) = (int)uVar14;
      iVar10 = iVar10 + 1;
      *(int *)(iVar7 + -4 + iVar10 * 8) = (int)(uVar14 >> 0x20);
    } while (iVar10 < *piVar1);
  }
  param_4 = 0;
  do {
    iVar10 = in_stack_00001efc[1];
    in_stack_00001efc[1] = iVar10 + -1;
    if (iVar10 + -1 < 0) {
      uVar5 = FUN_0041c030(in_stack_00001efc);
      uVar5 = uVar5 & 0xff;
    }
    else {
      uVar5 = (uint)*(byte *)*in_stack_00001efc;
      *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
    }
    switch(uVar5) {
    case 0x20:
      *(undefined4 *)(&stack0x000002d8 + extraout_ECX[0x30] * 0x18) = 1;
      FUN_0041c120(in_stack_00001efc,(byte *)s______s_0042d188);
      iVar10 = extraout_ECX[0x30];
      iVar7 = FUN_00413940((int)&param_7);
      *(int *)(&stack0x000002dc + iVar10 * 0x18) = iVar7;
      FUN_0041c120(in_stack_00001efc,&DAT_0042d180);
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        uVar5 = FUN_0041c030(in_stack_00001efc);
        uVar5 = uVar5 & 0xff;
      }
      else {
        uVar5 = (uint)*(byte *)*in_stack_00001efc;
        *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
      }
      iVar10 = extraout_ECX[0x30];
      if ((char)uVar5 == ',') {
        extraout_ECX[0x30] = iVar10 + 1;
        FUN_0041c120(in_stack_00001efc,&DAT_0042d190);
      }
      else {
        *(undefined4 *)(&stack0x000002e4 + iVar10 * 0x18) = 0;
        extraout_ECX[0x30] = iVar10 + 1;
        FUN_0041bfa0(uVar5,in_stack_00001efc);
      }
      break;
    default:
switchD_00404969_caseD_21:
      goto LAB_00405560;
    case 0x2e:
      iVar10 = extraout_ECX[0x30];
      *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 10;
      goto LAB_0040544f;
    case 0x44:
      iVar10 = extraout_ECX[0x30];
      iVar7 = iVar10 * 0x18;
      *(int *)(&stack0x000002dc + iVar7) = param_4;
      param_4 = param_4 + 1;
      *(undefined4 *)(&stack0x000002d8 + iVar7) = 0xc;
      extraout_ECX[0x30] = iVar10 + 1;
      break;
    case 0x5f:
      FUN_0041c120(in_stack_00001efc,&DAT_0042d1dc);
      uVar14 = tesoro_encode_name((int)&param_7);
      iVar10 = extraout_ECX[0x32];
      iVar7 = iVar10 * 0xc;
      *(int *)(&stack0x00000080 + iVar7) = (int)uVar14;
      *(int *)(&stack0x00000084 + iVar7) = (int)(uVar14 >> 0x20);
      extraout_ECX[0x32] = iVar10 + 1;
      *(undefined4 *)(&stack0x00000088 + iVar7) = extraout_ECX[0x30];
      break;
    case 0x62:
      iVar10 = extraout_ECX[0x30];
      *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 3;
      goto LAB_0040544f;
    case 0x69:
      FUN_0041c120(in_stack_00001efc,&DAT_0042d1a4);
      iVar10 = extraout_ECX[0x30] * 0x18;
      *(undefined4 *)(&stack0x000002d8 + iVar10) = 7;
      (&stack0x000002e4)[iVar10] = param_5;
      (&stack0x000002e5)[iVar10] = param_1;
      goto LAB_00405276;
    case 0x6a:
      *(undefined4 *)(&stack0x000002d8 + extraout_ECX[0x30] * 0x18) = 4;
      FUN_0041c120(in_stack_00001efc,(byte *)s_mp__s_0042d0c8);
LAB_00405276:
      uVar14 = tesoro_encode_name((int)&param_7);
      iVar10 = extraout_ECX[0x30];
      extraout_ECX[0x30] = iVar10 + 1;
      *(int *)(&stack0x000002dc + iVar10 * 0x18) = (int)uVar14;
      *(int *)(&stack0x000002e0 + iVar10 * 0x18) = (int)(uVar14 >> 0x20);
      break;
    case 0x6d:
      param_7 = param_7 & 0xffffff;
      FUN_0041c120(in_stack_00001efc,&DAT_0042d1ec);
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        FUN_0041c030(in_stack_00001efc);
      }
      else {
        *in_stack_00001efc = *in_stack_00001efc + 1;
      }
      if (param_7._3_1_ == '\0') {
        iVar10 = extraout_ECX[0x30];
        *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 5;
        goto LAB_0040544f;
      }
      FUN_0041c120(in_stack_00001efc,&DAT_0042d1ec);
      if ((char)param_7 == 'n') {
        *(undefined2 *)(&stack0x000002dc + extraout_ECX[0x30] * 0x18) = 1;
      }
      else {
        *(undefined2 *)(&stack0x000002dc + extraout_ECX[0x30] * 0x18) = 0;
      }
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        uVar8 = FUN_0041c030(in_stack_00001efc);
      }
      else {
        uVar8 = (uint)*(byte *)*in_stack_00001efc;
        *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
      }
      if (uVar8 == 10) {
        FUN_0041bfa0(10,in_stack_00001efc);
      }
      FUN_0041c120(in_stack_00001efc,(byte *)s________s_0042d0bc);
      if ((char)param_7 == '!') {
        pcVar11 = (char *)((int)&param_7 + 2);
        (&stack0x000002df)[extraout_ECX[0x30] * 0x18] = 1;
LAB_004053e9:
        FUN_0041bf50(pcVar11,&DAT_0042d190);
        iVar10 = extraout_ECX[0x30];
        iVar7 = iVar10 * 0x18;
        (&stack0x000002de)[iVar7] = param_1;
      }
      else {
        if ((char)param_7 == 'v') {
          pcVar11 = (char *)((int)&param_7 + 1);
          (&stack0x000002df)[extraout_ECX[0x30] * 0x18] = 0;
          goto LAB_004053e9;
        }
        iVar10 = extraout_ECX[0x30];
        iVar7 = iVar10 * 0x18;
        (&stack0x000002df)[iVar7] = 0;
        (&stack0x000002de)[iVar7] = 0xff;
      }
      *(undefined4 *)(&stack0x000002d8 + iVar7) = 6;
      extraout_ECX[0x30] = iVar10 + 1;
      break;
    case 0x70:
      FUN_0041c120(in_stack_00001efc,(byte *)s_lay__s_0042d19c);
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        FUN_0041c030(in_stack_00001efc);
      }
      else {
        *in_stack_00001efc = *in_stack_00001efc + 1;
      }
      if ((char)param_7 == 'a') {
        iVar10 = extraout_ECX[0x30];
        *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 0xd;
      }
      else {
        if ((char)param_7 != 'c') {
          if ((char)param_7 == 's') {
            iVar10 = in_stack_00001efc[1];
            *(undefined4 *)(&stack0x000002d8 + extraout_ECX[0x30] * 0x18) = 0xe;
            in_stack_00001efc[1] = iVar10 + -1;
            if (iVar10 + -1 < 0) {
              FUN_0041c030(in_stack_00001efc);
            }
            else {
              *in_stack_00001efc = *in_stack_00001efc + 1;
            }
            extraout_ECX[0x30] = extraout_ECX[0x30] + 1;
            FUN_0041c120(in_stack_00001efc,(byte *)s______s_0042d194);
          }
          break;
        }
        iVar10 = extraout_ECX[0x30];
        *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 0xf;
      }
      extraout_ECX[0x30] = iVar10 + 1;
      FUN_0041c120(in_stack_00001efc,&DAT_0042d190);
      break;
    case 0x72:
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        uVar5 = FUN_0041c030(in_stack_00001efc);
        bVar3 = (byte)uVar5;
        uVar5 = uVar5 & 0xff;
      }
      else {
        bVar3 = *(byte *)*in_stack_00001efc;
        uVar5 = (uint)bVar3;
        *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
      }
      if (bVar3 == 0x65) {
        iVar10 = extraout_ECX[0x30];
        *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 0;
        goto LAB_0040544f;
      }
      if (bVar3 != 0x75) goto switchD_00404969_caseD_21;
      FUN_0041c120(in_stack_00001efc,(byte *)s_n_______s__0042d1c8);
      uVar14 = tesoro_encode_name((int)&param_7);
      iVar10 = extraout_ECX[0x30];
      *(int *)(&stack0x000002dc + iVar10 * 0x18) = (int)uVar14;
      *(int *)(&stack0x000002e0 + iVar10 * 0x18) = (int)(uVar14 >> 0x20);
      FUN_0041c120(in_stack_00001efc,&DAT_0042d1bc);
      uVar14 = tesoro_encode_name((int)&param_7);
      iVar10 = extraout_ECX[0x30];
      iVar7 = iVar10 * 0x18;
      *(int *)(&stack0x000002e4 + iVar7) = (int)uVar14;
      *(int *)(&stack0x000002e8 + iVar7) = (int)(uVar14 >> 0x20);
      *(undefined4 *)(&stack0x000002d8 + iVar7) = 8;
      extraout_ECX[0x30] = iVar10 + 1;
      break;
    case 0x73:
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        uVar5 = FUN_0041c030(in_stack_00001efc);
        bVar3 = (byte)uVar5;
        uVar5 = uVar5 & 0xff;
      }
      else {
        bVar3 = *(byte *)*in_stack_00001efc;
        uVar5 = (uint)bVar3;
        *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
      }
      if (bVar3 == 0x6c) {
        iVar10 = extraout_ECX[0x30];
        *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 2;
        extraout_ECX[0x30] = iVar10 + 1;
        FUN_0041c120(in_stack_00001efc,(byte *)s_eep__i_0042d178);
      }
      else {
        if (bVar3 != 0x65) goto switchD_00404969_caseD_21;
        iVar10 = extraout_ECX[0x30] * 0x18;
        FUN_0041c120(in_stack_00001efc,(byte *)s_ndmessage_0042d16c);
        iVar7 = in_stack_00001efc[1];
        in_stack_00001efc[1] = iVar7 + -1;
        if (iVar7 + -1 < 0) {
          uVar5 = FUN_0041c030(in_stack_00001efc);
          uVar5 = uVar5 & 0xff;
        }
        else {
          uVar5 = (uint)*(byte *)*in_stack_00001efc;
          *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
        }
        if ((char)uVar5 == '\'') {
          FUN_0041c120(in_stack_00001efc,(byte *)s______s_0042d194);
          FUN_0041c120(in_stack_00001efc,&DAT_0042d168);
          uVar14 = tesoro_encode_name((int)&param_7);
          *(ulonglong *)(&stack0x000002dc + iVar10) = uVar14;
          iVar7 = in_stack_00001efc[1];
          in_stack_00001efc[1] = iVar7 + -1;
          if (iVar7 + -1 < 0) {
            uVar5 = FUN_0041c030(in_stack_00001efc);
            uVar5 = uVar5 & 0xff;
          }
          else {
            uVar5 = (uint)*(byte *)*in_stack_00001efc;
            *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
          }
          if ((char)uVar5 == 'i') {
            FUN_0041c120(in_stack_00001efc,&DAT_0042d160);
            pcVar11 = s_INVENTARIO_0042d154;
            pbVar6 = (byte *)&param_7;
            do {
              bVar3 = *pbVar6;
              bVar13 = bVar3 < (byte)*pcVar11;
              if (bVar3 != *pcVar11) {
LAB_00404e8e:
                iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                goto LAB_00404e93;
              }
              if (bVar3 == 0) break;
              bVar3 = pbVar6[1];
              bVar13 = bVar3 < ((byte *)pcVar11)[1];
              if (bVar3 != ((byte *)pcVar11)[1]) goto LAB_00404e8e;
              pbVar6 = pbVar6 + 2;
              pcVar11 = (char *)((byte *)pcVar11 + 2);
            } while (bVar3 != 0);
            iVar7 = 0;
LAB_00404e93:
            if (iVar7 == 0) {
              *(undefined4 *)(&stack0x000002e4 + iVar10) = DAT_004421ec;
            }
            else {
              FUN_0041bf50((char *)&param_7,(byte *)s_LEVEL_i_0042d14c);
              *(undefined4 *)(&stack0x000002e4 + iVar10) = (&DAT_00442130)[param_6];
            }
            goto LAB_00404ec6;
          }
          FUN_0041bfa0(uVar5,in_stack_00001efc);
          *(undefined4 *)(&stack0x000002e4 + iVar10) = 0;
        }
        else {
          iVar7 = extraout_ECX[0x2d];
          *(undefined4 *)(&stack0x000002dc + iVar10) = *(undefined4 *)(iVar7 + -0x188 + uVar5 * 8);
          uVar2 = *(undefined4 *)(iVar7 + -0x184 + uVar5 * 8);
          *(undefined4 *)(&stack0x000002e4 + iVar10) = 0;
          *(undefined4 *)(&stack0x000002e0 + iVar10) = uVar2;
LAB_00404ec6:
          iVar7 = in_stack_00001efc[1];
          in_stack_00001efc[1] = iVar7 + -1;
          if (iVar7 + -1 < 0) {
            FUN_0041c030(in_stack_00001efc);
          }
          else {
            *in_stack_00001efc = *in_stack_00001efc + 1;
          }
        }
        iVar7 = in_stack_00001efc[1];
        in_stack_00001efc[1] = iVar7 + -1;
        if (iVar7 + -1 < 0) {
          uVar8 = FUN_0041c030(in_stack_00001efc);
        }
        else {
          uVar8 = (uint)*(byte *)*in_stack_00001efc;
          *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
        }
        switch(uVar8) {
        case 0x61:
          FUN_0041c120(in_stack_00001efc,(byte *)s_ctivate_o_0042d138);
          iVar7 = in_stack_00001efc[1];
          in_stack_00001efc[1] = iVar7 + -1;
          if (iVar7 + -1 < 0) {
            uVar8 = FUN_0041c030(in_stack_00001efc);
          }
          else {
            uVar8 = (uint)*(byte *)*in_stack_00001efc;
            *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
          }
          *(undefined4 *)(&stack0x000002e8 + iVar10) = 3;
          (&stack0x000002ec)[iVar10] = uVar8 == 0x6e;
          break;
        default:
LAB_00405560:
          MessageBoxA((HWND)0x0,s_ERROR_AL_LEER_COMANDO_0042d0a4,s_ERROR_0042d06c,0);
          return extraout_ECX;
        case 0x69:
          FUN_0041c120(in_stack_00001efc,(byte *)s_ra__i__i_0042d0fc);
          *(undefined2 *)(&stack0x000002ec + iVar10) = param_2;
          *(undefined2 *)(&stack0x000002ee + iVar10) = param_3;
          *(undefined4 *)(&stack0x000002e8 + iVar10) = 1;
          break;
        case 0x6d:
          FUN_0041c120(in_stack_00001efc,(byte *)s_irar__s_0042d0f4);
          pcVar11 = s_ARRIBA_0042d0ec;
          pbVar6 = (byte *)&param_7;
          do {
            bVar3 = *pbVar6;
            bVar13 = bVar3 < (byte)*pcVar11;
            if (bVar3 != *pcVar11) {
LAB_0040515d:
              iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
              goto LAB_00405162;
            }
            if (bVar3 == 0) break;
            bVar3 = pbVar6[1];
            bVar13 = bVar3 < ((byte *)pcVar11)[1];
            if (bVar3 != ((byte *)pcVar11)[1]) goto LAB_0040515d;
            pbVar6 = pbVar6 + 2;
            pcVar11 = (char *)((byte *)pcVar11 + 2);
          } while (bVar3 != 0);
          iVar7 = 0;
LAB_00405162:
          if (iVar7 == 0) {
            (&stack0x000002ec)[iVar10] = 1;
          }
          else {
            pbVar12 = &DAT_0042d0e4;
            pbVar6 = (byte *)&param_7;
            do {
              bVar3 = *pbVar6;
              bVar13 = bVar3 < *pbVar12;
              if (bVar3 != *pbVar12) {
LAB_0040519c:
                iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                goto LAB_004051a1;
              }
              if (bVar3 == 0) break;
              bVar3 = pbVar6[1];
              bVar13 = bVar3 < pbVar12[1];
              if (bVar3 != pbVar12[1]) goto LAB_0040519c;
              pbVar6 = pbVar6 + 2;
              pbVar12 = pbVar12 + 2;
            } while (bVar3 != 0);
            iVar7 = 0;
LAB_004051a1:
            if (iVar7 == 0) {
              (&stack0x000002ec)[iVar10] = 2;
            }
            else {
              pcVar11 = s_IZQUIERDA_0042d0d8;
              pbVar6 = (byte *)&param_7;
              do {
                bVar3 = *pbVar6;
                bVar13 = bVar3 < (byte)*pcVar11;
                if (bVar3 != *pcVar11) {
LAB_004051d8:
                  iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                  goto LAB_004051dd;
                }
                if (bVar3 == 0) break;
                bVar3 = pbVar6[1];
                bVar13 = bVar3 < ((byte *)pcVar11)[1];
                if (bVar3 != ((byte *)pcVar11)[1]) goto LAB_004051d8;
                pbVar6 = pbVar6 + 2;
                pcVar11 = (char *)((byte *)pcVar11 + 2);
              } while (bVar3 != 0);
              iVar7 = 0;
LAB_004051dd:
              if (iVar7 == 0) {
                (&stack0x000002ec)[iVar10] = 3;
              }
              else {
                pcVar11 = s_DERECHA_0042d0d0;
                pbVar6 = (byte *)&param_7;
                do {
                  bVar3 = *pbVar6;
                  bVar13 = bVar3 < (byte)*pcVar11;
                  if (bVar3 != *pcVar11) {
LAB_00405214:
                    iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                    goto LAB_00405219;
                  }
                  if (bVar3 == 0) break;
                  bVar3 = pbVar6[1];
                  bVar13 = bVar3 < ((byte *)pcVar11)[1];
                  if (bVar3 != ((byte *)pcVar11)[1]) goto LAB_00405214;
                  pbVar6 = pbVar6 + 2;
                  pcVar11 = (char *)((byte *)pcVar11 + 2);
                } while (bVar3 != 0);
                iVar7 = 0;
LAB_00405219:
                if (iVar7 == 0) {
                  (&stack0x000002ec)[iVar10] = 4;
                }
              }
            }
          }
          *(undefined4 *)(&stack0x000002e8 + iVar10) = 2;
          break;
        case 0x70:
          FUN_0041c120(in_stack_00001efc,(byte *)s_lay__i__i__s_0042d128);
          (&stack0x000002ec)[iVar10] = (undefined1)param_2;
          (&stack0x000002ed)[iVar10] = (undefined1)param_3;
          pbVar12 = &DAT_0042d120;
          pbVar6 = (byte *)&param_7;
          do {
            bVar3 = *pbVar6;
            bVar13 = bVar3 < *pbVar12;
            if (bVar3 != *pbVar12) {
LAB_00405009:
              iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
              goto LAB_0040500e;
            }
            if (bVar3 == 0) break;
            bVar3 = pbVar6[1];
            bVar13 = bVar3 < pbVar12[1];
            if (bVar3 != pbVar12[1]) goto LAB_00405009;
            pbVar6 = pbVar6 + 2;
            pbVar12 = pbVar12 + 2;
          } while (bVar3 != 0);
          iVar7 = 0;
LAB_0040500e:
          if (iVar7 == 0) {
            (&stack0x000002ee)[iVar10] = 1;
            *(undefined4 *)(&stack0x000002e8 + iVar10) = 0;
          }
          else {
            pbVar12 = &DAT_0042d118;
            pbVar6 = (byte *)&param_7;
            do {
              bVar3 = *pbVar6;
              bVar13 = bVar3 < *pbVar12;
              if (bVar3 != *pbVar12) {
LAB_0040504b:
                iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                goto LAB_00405050;
              }
              if (bVar3 == 0) break;
              bVar3 = pbVar6[1];
              bVar13 = bVar3 < pbVar12[1];
              if (bVar3 != pbVar12[1]) goto LAB_0040504b;
              pbVar6 = pbVar6 + 2;
              pbVar12 = pbVar12 + 2;
            } while (bVar3 != 0);
            iVar7 = 0;
LAB_00405050:
            if (iVar7 == 0) {
              (&stack0x000002ee)[iVar10] = 0;
              *(undefined4 *)(&stack0x000002e8 + iVar10) = 0;
            }
            else {
              pbVar12 = &DAT_0042d110;
              pbVar6 = (byte *)&param_7;
              do {
                bVar3 = *pbVar6;
                bVar13 = bVar3 < *pbVar12;
                if (bVar3 != *pbVar12) {
LAB_0040508c:
                  iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                  goto LAB_00405091;
                }
                if (bVar3 == 0) break;
                bVar3 = pbVar6[1];
                bVar13 = bVar3 < pbVar12[1];
                if (bVar3 != pbVar12[1]) goto LAB_0040508c;
                pbVar6 = pbVar6 + 2;
                pbVar12 = pbVar12 + 2;
              } while (bVar3 != 0);
              iVar7 = 0;
LAB_00405091:
              if (iVar7 == 0) {
                (&stack0x000002ee)[iVar10] = 2;
                *(undefined4 *)(&stack0x000002e8 + iVar10) = 0;
              }
              else {
                pbVar12 = &DAT_0042d108;
                pbVar6 = (byte *)&param_7;
                do {
                  bVar3 = *pbVar6;
                  bVar13 = bVar3 < *pbVar12;
                  if (bVar3 != *pbVar12) {
LAB_004050ce:
                    iVar7 = (1 - (uint)bVar13) - (uint)(bVar13 != 0);
                    goto LAB_004050d3;
                  }
                  if (bVar3 == 0) break;
                  bVar3 = pbVar6[1];
                  bVar13 = bVar3 < pbVar12[1];
                  if (bVar3 != pbVar12[1]) goto LAB_004050ce;
                  pbVar6 = pbVar6 + 2;
                  pbVar12 = pbVar12 + 2;
                } while (bVar3 != 0);
                iVar7 = 0;
LAB_004050d3:
                if (iVar7 == 0) {
                  (&stack0x000002ee)[iVar10] = 3;
                }
                *(undefined4 *)(&stack0x000002e8 + iVar10) = 0;
              }
            }
          }
          break;
        case 0x73:
          FUN_0041c120(in_stack_00001efc,(byte *)s_how_o_0042d144);
          iVar7 = in_stack_00001efc[1];
          in_stack_00001efc[1] = iVar7 + -1;
          if (iVar7 + -1 < 0) {
            uVar8 = FUN_0041c030(in_stack_00001efc);
          }
          else {
            uVar8 = (uint)*(byte *)*in_stack_00001efc;
            *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
          }
          *(undefined4 *)(&stack0x000002e8 + iVar10) = 4;
          (&stack0x000002ec)[iVar10] = uVar8 == 0x6e;
        }
        iVar10 = extraout_ECX[0x30];
        extraout_ECX[0x30] = iVar10 + 1;
        *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 9;
      }
      break;
    case 0x76:
      FUN_0041c120(in_stack_00001efc,(byte *)s__i____i_0042d1d4);
      iVar10 = extraout_ECX[0x30];
      *(undefined4 *)(&stack0x000002d8 + iVar10 * 0x18) = 0xb;
LAB_0040544f:
      extraout_ECX[0x30] = iVar10 + 1;
    }
    do {
      iVar10 = in_stack_00001efc[1];
      in_stack_00001efc[1] = iVar10 + -1;
      if (iVar10 + -1 < 0) {
        uVar8 = FUN_0041c030(in_stack_00001efc);
      }
      else {
        uVar8 = (uint)*(byte *)*in_stack_00001efc;
        *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
      }
    } while ((uVar8 != 10) && ((*(byte *)(in_stack_00001efc + 3) & 0x10) == 0));
    if ((char)uVar5 == '.') {
      do {
        iVar10 = in_stack_00001efc[1];
        in_stack_00001efc[1] = iVar10 + -1;
        if (iVar10 + -1 < 0) {
          uVar5 = FUN_0041c030(in_stack_00001efc);
        }
        else {
          uVar5 = (uint)*(byte *)*in_stack_00001efc;
          *in_stack_00001efc = (int)((byte *)*in_stack_00001efc + 1);
        }
      } while ((uVar5 != 10) && ((*(byte *)(in_stack_00001efc + 3) & 0x10) == 0));
      puVar9 = FUN_0040db40(extraout_ECX[0x30] * 0x18,1);
      extraout_ECX[0x2f] = puVar9;
      puVar4 = (undefined4 *)&stack0x000002d8;
      for (uVar5 = (uint)(extraout_ECX[0x30] * 0x18) >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *puVar9 = *puVar4;
        puVar4 = puVar4 + 1;
        puVar9 = puVar9 + 1;
      }
      for (iVar10 = 0; iVar10 != 0; iVar10 = iVar10 + -1) {
        *(undefined1 *)puVar9 = *(undefined1 *)puVar4;
        puVar4 = (undefined4 *)((int)puVar4 + 1);
        puVar9 = (undefined4 *)((int)puVar9 + 1);
      }
      puVar9 = FUN_0040db40(extraout_ECX[0x32] * 0xc,1);
      extraout_ECX[0x31] = puVar9;
      puVar4 = (undefined4 *)&stack0x00000080;
      for (uVar5 = extraout_ECX[0x32] * 3 & 0x3fffffff; uVar5 != 0; uVar5 = uVar5 - 1) {
        *puVar9 = *puVar4;
        puVar4 = puVar4 + 1;
        puVar9 = puVar9 + 1;
      }
      for (iVar10 = 0; iVar10 != 0; iVar10 = iVar10 + -1) {
        *(undefined1 *)puVar9 = *(undefined1 *)puVar4;
        puVar4 = (undefined4 *)((int)puVar4 + 1);
        puVar9 = (undefined4 *)((int)puVar9 + 1);
      }
      return extraout_ECX;
    }
  } while( true );
}

