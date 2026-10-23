/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 00403cb0
 */


uint __thiscall tesoro_play_cd_track(void *this,byte param_1,undefined1 param_2)

{
  MCIERROR MVar1;
  MCIDEVICEID mciId;
  int iVar2;
  undefined4 local_18;
  uint local_14;
  uint local_10;
  undefined1 local_c [4];
  undefined4 local_8;
  
  if (*(MCIDEVICEID *)((int)this + 0x14) != 0xffffffff) {
    mciSendCommandA(*(MCIDEVICEID *)((int)this + 0x14),0x808,0,0);
  }
  iVar2 = *(int *)((int)this + 0x1bc);
  *(undefined1 *)((int)this + 0x28) = 0;
  if ((char)param_1 <= iVar2) {
    local_8 = 10;
    MVar1 = mciSendCommandA(*(MCIDEVICEID *)((int)this + 0x14),0x80d,0x400,(DWORD_PTR)local_c);
    if (MVar1 != 0) {
      mciId = *(MCIDEVICEID *)((int)this + 0x14);
      if (mciId != 0xffffffff) {
        mciId = mciSendCommandA(mciId,0x804,0,0);
      }
      *(undefined1 *)((int)this + 0x28) = 0;
      return mciId & 0xffffff00;
    }
    local_18 = *(undefined4 *)((int)this + 0x24);
    local_14 = (uint)param_1;
    local_10 = (uint)(byte)(param_1 + 1);
    if ((int)(char)param_1 == *(int *)((int)this + 0x1bc)) {
      MVar1 = mciSendCommandA(*(MCIDEVICEID *)((int)this + 0x14),0x806,4,(DWORD_PTR)&local_18);
      if (MVar1 != 0) {
        return MVar1 & 0xffffff00;
      }
    }
    else {
      MVar1 = mciSendCommandA(*(MCIDEVICEID *)((int)this + 0x14),0x806,0xd,(DWORD_PTR)&local_18);
      if (MVar1 != 0) {
        return MVar1 & 0xffffff00;
      }
    }
    iVar2 = CONCAT31((int3)(MVar1 >> 8),param_2);
    *(byte *)((int)this + 0x29) = param_1;
    *(undefined1 *)((int)this + 0x28) = param_2;
  }
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}

