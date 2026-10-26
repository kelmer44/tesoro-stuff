00407aa0  MOV AL,byte ptr [ECX + 0xd5]
00407aa6  TEST AL,AL
00407aa8  JZ 0x00407ab6
00407aaa  MOV EDX,dword ptr [ECX + 0x10c]
00407ab0  MOV EAX,dword ptr [ECX]
00407ab2  PUSH EDX
00407ab3  CALL dword ptr [EAX + 0x74]
00407ab6  RET
