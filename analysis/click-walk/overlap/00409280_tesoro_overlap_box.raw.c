/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00409280
 */


int __cdecl
tesoro_overlap_box(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                  int param_7,int param_8,int param_9,int param_10)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  if ((((param_4 <= param_6) && (param_8 <= param_2)) && (param_1 <= param_7)) &&
     (param_5 <= param_3)) {
    return 0;
  }
  local_10 = param_1;
  local_c = param_2;
  local_8 = param_3;
  local_4 = param_4;
  if (param_9 < 0) {
    param_1 = param_1 + param_9;
    param_3 = param_3 + param_9;
    param_2 = param_2 + param_10;
    param_4 = param_4 + param_10;
    iVar2 = -param_9;
    iVar4 = -param_10;
  }
  else {
    local_10 = param_9 + param_1;
    local_8 = param_3 + param_9;
    local_c = param_2 + param_10;
    local_4 = param_4 + param_10;
    iVar2 = param_9;
    iVar4 = param_10;
  }
  if ((param_7 < param_1) || (local_8 < param_5)) {
    return -1;
  }
  if (param_2 < local_c) {
    if (local_c < param_8) {
      return -1;
    }
    if (param_6 < param_4) {
      return -1;
    }
    if ((param_3 < param_5) &&
       (iVar1 = (param_5 - param_3) * iVar4, iVar3 = (param_6 - param_4) * iVar2,
       iVar1 - iVar3 != 0 && iVar3 <= iVar1)) {
      return -1;
    }
    if ((param_7 < local_10) && ((param_7 - param_1) * iVar4 < (param_8 - param_2) * iVar2)) {
      return -1;
    }
  }
  else {
    if (param_2 < param_8) {
      return -1;
    }
    if (param_6 < local_4) {
      return -1;
    }
    if ((param_3 < param_5) && ((param_5 - param_3) * iVar4 < (param_8 - param_2) * iVar2)) {
      return -1;
    }
    if ((param_7 < local_10) &&
       (iVar4 = (param_7 - param_1) * iVar4, iVar2 = (param_6 - param_4) * iVar2,
       iVar4 - iVar2 != 0 && iVar2 <= iVar4)) {
      return -1;
    }
  }
  if (param_9 < 0) {
    param_2 = local_c;
    param_3 = local_8;
    param_1 = local_10;
    param_4 = local_4;
  }
  if (param_3 < param_5) {
    if (param_6 < param_4) {
      iVar2 = param_6 - param_4;
      if (param_10 * (param_5 - param_3) < param_9 * iVar2) {
        return ((param_5 - param_3) * 0x10000) / param_9;
      }
LAB_004094ef:
      return (iVar2 << 0x10) / param_10;
    }
    if (param_8 <= param_2) {
      return ((param_5 - param_3) * 0x10000) / param_9;
    }
    iVar2 = param_5 - param_3;
    if (param_10 * iVar2 < param_9 * (param_8 - param_2)) {
      return ((param_8 - param_2) * 0x10000) / param_10;
    }
LAB_00409511:
    return (iVar2 << 0x10) / param_9;
  }
  if (param_1 <= param_7) {
    if (param_6 < param_4) {
      return ((param_6 - param_4) * 0x10000) / param_10;
    }
    return ((param_8 - param_2) * 0x10000) / param_10;
  }
  if (param_6 < param_4) {
    iVar4 = param_7 - param_1;
    if (param_10 * iVar4 < param_9 * (param_6 - param_4)) {
      return ((param_6 - param_4) * 0x10000) / param_10;
    }
  }
  else {
    if (param_8 <= param_2) {
      iVar2 = param_7 - param_1;
      goto LAB_00409511;
    }
    iVar4 = param_7 - param_1;
    iVar2 = param_8 - param_2;
    if (param_10 * iVar4 - param_9 * iVar2 != 0 && param_9 * iVar2 <= param_10 * iVar4)
    goto LAB_004094ef;
  }
  return (iVar4 << 0x10) / param_9;
}

