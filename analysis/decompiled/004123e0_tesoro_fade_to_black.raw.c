/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004123e0
 */


void __thiscall tesoro_fade_to_black(void *this,int param_1,int param_2)

{
  char cVar1;
  ushort uVar2;
  ushort uVar3;
  code *pcVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  DWORD DVar8;
  int iVar9;
  ushort *puVar10;
  ushort uVar11;
  undefined4 *puVar12;
  int iVar13;
  ushort *puVar14;
  undefined2 uVar15;
  int iStack_214;
  ushort *local_210;
  undefined4 uStack_20c;
  undefined4 auStack_208 [3];
  undefined1 auStack_1fc [16];
  undefined4 uStack_1ec;
  undefined4 uStack_1d8;
  ushort auStack_180 [64];
  ushort auStack_100 [64];
  ushort auStack_80 [64];
  
  local_210 = (ushort *)FUN_0040db40(0x96000,3);
  iVar5 = (int)(0x100 / (longlong)param_1);
  puVar12 = *(undefined4 **)((int)this + 0x678);
  puVar10 = local_210;
  for (iVar9 = 0x25800; iVar9 != 0; iVar9 = iVar9 + -1) {
    *(undefined4 *)puVar10 = *puVar12;
    puVar12 = puVar12 + 1;
    puVar10 = puVar10 + 2;
  }
  pcVar4 = GetTickCount_exref;
  for (iVar9 = 0x100 - iVar5; -1 < iVar9; iVar9 = iVar9 - iVar5) {
    if (*(char *)((int)this + 0x66a) == '\0') {
      iVar6 = (**(code **)(**(int **)((int)this + 0xc) + 0x2c))(*(int **)((int)this + 0xc),0,0);
      while (iVar6 == -0x7789fde4) {
        iVar6 = (**(code **)(**(int **)((int)this + 0xc) + 0x2c))(*(int **)((int)this + 0xc),0,0);
      }
      auStack_208[0] = 0x7c;
      iVar6 = (**(code **)(**(int **)((int)this + 0xc) + 0x58))
                        (*(int **)((int)this + 0xc),auStack_208);
      if (iVar6 == 0) {
        iVar6 = (**(code **)(**(int **)((int)this + 0xc) + 100))
                          (*(int **)((int)this + 0xc),0,auStack_1fc,0,0);
        while (iVar6 == -0x7789fde4) {
          iVar6 = (**(code **)(**(int **)((int)this + 0xc) + 100))
                            (*(int **)((int)this + 0xc),0,auStack_1fc,0,0);
        }
        if (iVar6 == 0) {
          *(undefined1 *)((int)this + 0x66a) = 1;
          *(undefined4 *)((int)this + 0x1c) = uStack_1ec;
          *(undefined4 *)((int)this + 0x678) = uStack_1d8;
        }
      }
    }
    cVar1 = *(char *)((int)this + 0x67c);
    iVar13 = 0;
    iVar6 = 0;
    do {
      iVar7 = iVar13 >> 8;
      if (cVar1 == '\0') {
        uVar15 = (undefined2)(iVar7 << 0xb);
      }
      else {
        uVar15 = (undefined2)(iVar7 << 10);
      }
      *(undefined2 *)((int)auStack_100 + iVar6) = uVar15;
      *(short *)((int)auStack_80 + iVar6) = (short)((uint)iVar13 >> 8);
      *(short *)((int)auStack_180 + iVar6) = (short)(iVar7 << 5);
      iVar6 = iVar6 + 2;
      iVar13 = iVar13 + iVar9;
    } while (iVar6 < 0x80);
    iVar6 = (*pcVar4)();
    iStack_214 = 0x4b000;
    puVar10 = *(ushort **)((int)this + 0x678);
    puVar14 = local_210;
    do {
      uVar2 = *puVar14;
      puVar14 = puVar14 + 1;
      if (*(char *)((int)this + 0x67c) == '\0') {
        uVar11 = auStack_180[uVar2 >> 5 & 0x3f];
        uVar3 = uVar2 >> 0xb;
      }
      else {
        uVar11 = auStack_180[uVar2 >> 5 & 0x1f];
        uVar3 = uVar2 >> 10;
      }
      *puVar10 = auStack_100[uVar3 & 0x1f] | auStack_80[uVar2 & 0x1f] | uVar11;
      iStack_214 = iStack_214 + -1;
      puVar10 = puVar10 + 1;
    } while (iStack_214 != 0);
    (**(code **)(**(int **)((int)this + 0xc) + 0x80))(*(int **)((int)this + 0xc),0);
    iStack_214 = 0;
    local_210 = (ushort *)0x0;
    uStack_20c = 0x280;
    auStack_208[0] = 0x1e0;
    (**(code **)(**(int **)((int)this + 8) + 0x1c))
              (*(int **)((int)this + 8),0,0,*(undefined4 *)((int)this + 0xc),&iStack_214,0);
    pcVar4 = GetTickCount_exref;
    *(undefined1 *)((int)this + 0x66a) = 0;
    DVar8 = GetTickCount();
    iVar13 = DVar8 - iVar6;
    while (iVar13 < param_2) {
      DVar8 = GetTickCount();
      iVar13 = DVar8 - iVar6;
    }
  }
  return;
}

