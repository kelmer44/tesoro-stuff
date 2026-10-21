00401bc0  MOV EAX,ECX
00401bc2  MOV dword ptr [EAX],0x428000
00401bc8  MOV dword ptr [EAX + 0x68],0x0
00401bcf  MOV ECX,dword ptr [EAX + 0x68]
00401bd2  MOV word ptr [EAX + 0x5e],0x5
00401bd8  OR ECX,0x6
00401bdb  MOV byte ptr [EAX + 0x5c],0x0
00401bdf  MOV dword ptr [EAX + 0x68],ECX
00401be2  RET
