/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00409e50
 */


undefined4 * __thiscall
tesoro_create_scene_object(void *this,undefined4 *param_1,int param_2,byte *param_3)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)0x0;
  switch(param_1[6]) {
  case 0:
    puVar1 = FUN_0040db40(0x6c,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00401aa0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(int)param_3,0);
      return puVar1;
    }
    break;
  case 1:
    puVar1 = FUN_0040db40(0x84,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00406980(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(undefined4 *)param_3);
      *(undefined4 **)((int)this + 0x90) = puVar1;
      return puVar1;
    }
    *(undefined4 *)((int)this + 0x90) = 0;
    return (undefined4 *)0x0;
  case 2:
    puVar1 = FUN_0040db40(0x78,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00414930(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 3:
    puVar1 = FUN_0040db40(0x6c,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = tesoro_initialize_scenery
                         (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                          *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 4:
    puVar1 = FUN_0040db40(0x74,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_0040e720(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 5:
    param_1[7] = param_1[7] + -4;
    switch(param_3[5]) {
    case 0:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,4);
        *puVar1 = &PTR_FUN_0042b280;
        goto LAB_0040a430;
      }
      break;
    case 1:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,0xe);
        *puVar1 = &PTR_FUN_0042b4a0;
LAB_0040a2a0:
        FUN_00401de0(puVar1,0,0,-1,1,'\0');
        goto LAB_0040a430;
      }
      break;
    case 2:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,4);
        *puVar1 = &PTR_FUN_0042b308;
        goto LAB_0040a2a0;
      }
      break;
    case 3:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,0);
        *puVar1 = &PTR_FUN_0042b390;
        puVar1[0x1a] = puVar1[0x1a] | 4;
        goto LAB_0040a430;
      }
      break;
    case 4:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,10);
        *puVar1 = &PTR_FUN_0042b418;
        goto LAB_0040a2a0;
      }
      break;
    case 5:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,3);
        *puVar1 = &PTR_FUN_0042b6c0;
        goto LAB_0040a430;
      }
      break;
    case 6:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,6);
        *puVar1 = &PTR_FUN_0042b638;
        goto LAB_0040a2a0;
      }
      break;
    case 7:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,4);
        *puVar1 = &PTR_FUN_0042b5b0;
        goto LAB_0040a430;
      }
      break;
    case 8:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,3);
        *puVar1 = &PTR_FUN_0042b528;
        goto LAB_0040a430;
      }
      break;
    case 9:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,0);
        *puVar1 = &PTR_FUN_0042b748;
        goto LAB_0040a430;
      }
      break;
    case 10:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,1);
        *puVar1 = &PTR_FUN_0042b858;
        goto LAB_0040a430;
      }
      break;
    case 0xb:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,1);
        *puVar1 = &PTR_FUN_0042b7d0;
        goto LAB_0040a430;
      }
      break;
    default:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        puVar1 = FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                              *(int *)((int)this + 0x6c),(undefined2 *)param_3,0);
        goto LAB_0040a430;
      }
      break;
    case 0xd:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,0xf);
        *puVar1 = &PTR_FUN_0042b1f8;
        goto LAB_0040a430;
      }
      break;
    case 0xe:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,2);
        *puVar1 = &PTR_FUN_0042b170;
        goto LAB_0040a430;
      }
      break;
    case 0xf:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,0xb);
        *puVar1 = &PTR_FUN_0042b0e8;
        goto LAB_0040a430;
      }
      break;
    case 0x10:
      puVar1 = FUN_0040db40(0xd8,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_0040fec0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),(undefined2 *)param_3,7);
        *puVar1 = &PTR_FUN_0042b060;
        goto LAB_0040a430;
      }
    }
    puVar1 = (undefined4 *)0x0;
LAB_0040a430:
    if (puVar1 != (undefined4 *)0x0) {
      *(byte *)((int)puVar1 + 0x5d) = param_3[5];
      return puVar1;
    }
    return (undefined4 *)0x0;
  case 6:
    switch(param_3[1]) {
    case 1:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042abb0;
        goto switchD_0040b025_default;
      }
      break;
    case 2:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042ac28;
        goto switchD_0040b025_default;
      }
      break;
    case 3:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,2);
        *puVar1 = &PTR_FUN_0042aca0;
        goto switchD_0040b025_default;
      }
      break;
    case 4:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042ad18;
        goto switchD_0040b025_default;
      }
      break;
    case 5:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042ad90;
        goto switchD_0040b025_default;
      }
      break;
    case 6:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042ae08;
        goto switchD_0040b025_default;
      }
      break;
    case 7:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042ae80;
        goto switchD_0040b025_default;
      }
      break;
    case 8:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042aef8;
        goto switchD_0040b025_default;
      }
      break;
    case 9:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042af70;
        goto switchD_0040b025_default;
      }
      break;
    case 10:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_door
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042afe8;
        goto switchD_0040b025_default;
      }
      break;
    default:
      puVar1 = FUN_0040db40(0xb0,1);
      if (puVar1 != (undefined4 *)0x0) {
        puVar1 = tesoro_initialize_door
                           (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3,0);
        goto switchD_0040b025_default;
      }
    }
    goto LAB_0040b496;
  case 7:
    puVar1 = FUN_0040db40(0x7c,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00414620(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 8:
    puVar1 = FUN_0040db40(0xb4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00414e00(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 9:
    puVar1 = FUN_0040db40(0xa0,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = tesoro_initialize_interactive_object
                         (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                          *(int *)((int)this + 0x6c),param_3,0);
      goto switchD_0040b4bf_caseD_11;
    }
    goto LAB_0040bc8f;
  case 10:
    puVar1 = FUN_0040db40(0x130,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00402300(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 0xb:
    puVar1 = FUN_0040db40(0xbc,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00415250(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(int)param_3);
      return puVar1;
    }
    break;
  case 0xc:
    switch(param_3[1]) {
    case 0:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042ab40;
        goto switchD_0040b025_default;
      }
      break;
    case 1:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042aad0;
        goto switchD_0040b025_default;
      }
      break;
    case 2:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042aa60;
        goto switchD_0040b025_default;
      }
      break;
    case 3:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a9f0;
        goto switchD_0040b025_default;
      }
      break;
    case 4:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a980;
        goto switchD_0040b025_default;
      }
      break;
    case 5:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a910;
        goto switchD_0040b025_default;
      }
      break;
    case 6:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a8a0;
        goto switchD_0040b025_default;
      }
      break;
    case 7:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a830;
        goto switchD_0040b025_default;
      }
      break;
    case 8:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a750;
        goto switchD_0040b025_default;
      }
      break;
    case 9:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a7c0;
        goto switchD_0040b025_default;
      }
      break;
    case 10:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a6e0;
        goto switchD_0040b025_default;
      }
      break;
    case 0xb:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a670;
        goto switchD_0040b025_default;
      }
      break;
    default:
      goto switchD_0040b025_default;
    }
    goto LAB_0040b496;
  case 0xd:
    param_3[2] = 0x49;
    switch(param_3[1]) {
    case 9:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a520;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 10:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a590;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    default:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        puVar1 = FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                              *(int *)((int)this + 0x6c),param_3,0);
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0xf:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a4b0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x10:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,3);
        *puVar1 = &PTR_FUN_0042a600;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x13:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,2);
        *puVar1 = &PTR_FUN_0042a3d0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1a:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a0c0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1c:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a440;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1d:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a1a0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x21:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a210;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x24:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a360;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x25:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a2f0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x2a:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a130;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x2c:
      puVar1 = FUN_0040db40(0xac,1);
      if (puVar1 != (undefined4 *)0x0) {
        FUN_00406a90(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                     *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_0042a280;
        goto switchD_0040b4bf_caseD_11;
      }
    }
    goto LAB_0040bc8f;
  case 0xe:
    puVar1 = FUN_0040db40(0x74,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_004150d0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(undefined4 *)param_3);
      return puVar1;
    }
    break;
  case 0xf:
    puVar1 = FUN_0040db40(0xa0,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00415770(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(int)param_3);
      return puVar1;
    }
    break;
  case 0x10:
    puVar1 = FUN_0040db40(0xa0,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_0040e7e0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 0x11:
    switch(param_3[1]) {
    case 0:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_0042a050;
        goto switchD_0040b025_default;
      }
      break;
    case 1:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429fe0;
        goto switchD_0040b025_default;
      }
      break;
    case 2:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429f70;
        goto switchD_0040b025_default;
      }
      break;
    case 3:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429f00;
        goto switchD_0040b025_default;
      }
      break;
    case 4:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429e90;
        goto switchD_0040b025_default;
      }
      break;
    case 5:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429e20;
        goto switchD_0040b025_default;
      }
      break;
    case 6:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429db0;
        goto switchD_0040b025_default;
      }
      break;
    case 7:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429cd0;
        goto switchD_0040b025_default;
      }
      break;
    case 8:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429c60;
        goto switchD_0040b025_default;
      }
      break;
    case 9:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429bf0;
        goto switchD_0040b025_default;
      }
      break;
    case 10:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429b80;
        goto switchD_0040b025_default;
      }
      break;
    case 0xb:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429b10;
        goto switchD_0040b025_default;
      }
      break;
    case 0xc:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429aa0;
        goto switchD_0040b025_default;
      }
      break;
    case 0xd:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429d40;
        puVar1[0x1a] = puVar1[0x1a] | 4;
        goto switchD_0040b025_default;
      }
      break;
    case 0xe:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429a30;
        goto switchD_0040b025_default;
      }
      break;
    case 0xf:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_004299c0;
        goto switchD_0040b025_default;
      }
      break;
    case 0x10:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429950;
        goto switchD_0040b025_default;
      }
      break;
    default:
      goto switchD_0040b025_default;
    }
LAB_0040b496:
    puVar1 = (undefined4 *)0x0;
switchD_0040b025_default:
    if (puVar1 != (undefined4 *)0x0) {
      *(byte *)((int)puVar1 + 0x5d) = param_3[1];
      return puVar1;
    }
    return (undefined4 *)0x0;
  case 0x13:
    puVar1 = FUN_0040db40(0xa4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00417720(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3,0);
      return puVar1;
    }
    break;
  case 0x14:
    switch(param_3[1]) {
    case 0:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_004298e0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 1:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429870;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 2:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429800;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 3:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,2);
        *puVar1 = &PTR_FUN_00429790;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 4:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429720;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 5:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_004296b0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 6:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429640;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 7:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_004295d0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 8:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429560;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 9:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_004294f0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 10:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429480;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0xb:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429410;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0xc:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_004293a0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0xd:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429330;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0xe:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429250;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0xf:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_004291e0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x10:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_004292c0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    default:
      goto switchD_0040b4bf_caseD_11;
    case 0x12:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429170;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x13:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00429100;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x14:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429090;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x15:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00429020;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x16:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00428fb0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x17:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00428ed0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x18:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00428f40;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x19:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00428e60;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1a:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00428df0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1b:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00428d80;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1c:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00428d10;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1d:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,0);
        *puVar1 = &PTR_FUN_00428ca0;
        goto switchD_0040b4bf_caseD_11;
      }
      break;
    case 0x1e:
      puVar1 = FUN_0040db40(0xa0,1);
      if (puVar1 != (undefined4 *)0x0) {
        tesoro_initialize_interactive_object
                  (puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                   *(int *)((int)this + 0x6c),param_3,1);
        *puVar1 = &PTR_FUN_00428c30;
        goto switchD_0040b4bf_caseD_11;
      }
    }
LAB_0040bc8f:
    puVar1 = (undefined4 *)0x0;
switchD_0040b4bf_caseD_11:
    if (puVar1 != (undefined4 *)0x0) {
      *(byte *)((int)puVar1 + 0x5d) = param_3[1];
      return puVar1;
    }
    return (undefined4 *)0x0;
  case 0x15:
    puVar1 = FUN_0040db40(0xa4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_004192c0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(int)param_3);
      return puVar1;
    }
    break;
  case 0x16:
    puVar1 = FUN_0040db40(0xa4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_0040df70(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),(int)param_3);
      return puVar1;
    }
    break;
  case 0x17:
    puVar1 = FUN_0040db40(0xa4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_0040dd40(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 0x18:
    puVar1 = FUN_0040db40(0xa4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_004175b0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
    break;
  case 0x19:
    puVar1 = FUN_0040db40(0xa4,1);
    if (puVar1 != (undefined4 *)0x0) {
      puVar1 = FUN_00417ad0(puVar1,param_1,param_2 * 0x10 + *(int *)((int)this + 0x70),
                            *(int *)((int)this + 0x6c),param_3);
      return puVar1;
    }
  }
  return (undefined4 *)0x0;
}

