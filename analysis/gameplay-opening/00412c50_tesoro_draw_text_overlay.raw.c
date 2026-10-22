/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00412c50
 */


void __thiscall tesoro_draw_text_overlay(void *this,char *param_1)

{
  ushort *puVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  char cVar5;
  short sVar6;
  int iVar7;
  int iVar8;
  char *pcVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  undefined4 *puVar13;
  uint uVar14;
  int *piVar15;
  undefined4 *puVar16;
  undefined1 *puVar17;
  undefined4 *puVar18;
  uint local_c;
  
  iVar2 = *(int *)((int)this + 0x678) +
          (*(int *)(param_1 + 0x68) * 0x280 + *(int *)(param_1 + 100)) * 2;
  sVar6 = *(short *)(*(int *)(*(int *)((int)this + (uint)*(ushort *)(param_1 + 0x6e) * 4 + 0x20) +
                             0x1c) + 0xd3a);
  cVar5 = *param_1;
  pcVar9 = param_1;
  while (cVar5 != '\0') {
    cVar5 = *pcVar9;
    pcVar9 = pcVar9 + 1;
    local_c = (uint)(byte)(cVar5 - 0x21U);
    iVar7 = *(int *)(*(int *)((int)this + (uint)*(ushort *)(param_1 + 0x6e) * 4 + 0x20) + 0x1c);
    iVar8 = **(int **)(iVar7 + local_c * 0x24 + 0x18);
    puVar16 = (undefined4 *)(iVar8 + 0x10);
    puVar17 = (undefined1 *)
              (((int)sVar6 - (int)*(short *)(iVar7 + 2 + local_c * 0x24)) * 0x500 + iVar2);
    iVar2 = iVar2 + 4 + (uint)*(ushort *)(iVar8 + 8) * 2;
    if ((byte)(cVar5 - 0x21U) != 0x5e) {
      for (uVar10 = (uint)*(ushort *)(iVar8 + 10); uVar10 != 0; uVar10 = uVar10 - 1) {
        piVar15 = (int *)*puVar16;
        puVar4 = puVar17;
        for (uVar14 = (uint)*(ushort *)(puVar16 + 1); uVar14 != 0; uVar14 = uVar14 - 1) {
          puVar3 = (undefined4 *)(puVar4 + (uint)*(ushort *)(piVar15 + 1) * 2);
          if (*(short *)(iVar8 + 0xc) == 1) {
            uVar11 = (uint)*(ushort *)((int)piVar15 + 6) << 1;
            puVar13 = (undefined4 *)*piVar15;
            puVar18 = puVar3;
            for (uVar12 = uVar11 >> 2; uVar12 != 0; uVar12 = uVar12 - 1) {
              *puVar18 = *puVar13;
              puVar13 = puVar13 + 1;
              puVar18 = puVar18 + 1;
            }
            for (uVar11 = uVar11 & 3; uVar11 != 0; uVar11 = uVar11 - 1) {
              *(undefined1 *)puVar18 = *(undefined1 *)puVar13;
              puVar13 = (undefined4 *)((int)puVar13 + 1);
              puVar18 = (undefined4 *)((int)puVar18 + 1);
            }
          }
          else if (*(short *)(iVar8 + 0xc) == 3) {
            if (*piVar15 == 0) {
              uVar11 = (uint)*(ushort *)((int)piVar15 + 6) << 1;
              puVar13 = puVar3;
              for (uVar12 = uVar11 >> 2; uVar12 != 0; uVar12 = uVar12 - 1) {
                *puVar13 = 0;
                puVar13 = puVar13 + 1;
              }
              for (uVar11 = uVar11 & 3; uVar11 != 0; uVar11 = uVar11 - 1) {
                *(undefined1 *)puVar13 = 0;
                puVar13 = (undefined4 *)((int)puVar13 + 1);
              }
            }
            else {
              uVar12 = 0;
              puVar13 = puVar3;
              if (*(short *)((int)piVar15 + 6) != 0) {
                do {
                  uVar12 = uVar12 + 1;
                  *(undefined2 *)puVar13 = *(undefined2 *)(param_1 + 0x6c);
                  puVar13 = (undefined4 *)((int)puVar13 + 2);
                } while (uVar12 < *(ushort *)((int)piVar15 + 6));
              }
            }
          }
          puVar1 = (ushort *)((int)piVar15 + 6);
          piVar15 = piVar15 + 2;
          puVar4 = (undefined1 *)((int)puVar3 + (uint)*puVar1 * 2);
        }
        puVar16 = puVar16 + 2;
        puVar17 = puVar17 + 0x500;
      }
    }
    cVar5 = *pcVar9;
  }
  return;
}

