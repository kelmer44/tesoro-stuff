004120c0  MOV EDX,dword ptr [ECX + 0x664]
004120c6  MOV EAX,EDX
004120c8  SHL EAX,0x5
004120cb  INC EDX
004120cc  LEA EAX,[EAX + ECX*0x1 + 0x24]
004120d0  MOV dword ptr [ECX + 0x664],EDX
004120d6  MOV ECX,dword ptr [ESP + 0x4]
004120da  MOV EDX,dword ptr [ESP + 0x8]
004120de  MOV dword ptr [EAX + 0x4],ECX
004120e1  MOV CX,word ptr [ESP + 0xc]
004120e6  MOV dword ptr [EAX + 0x8],EDX
004120e9  MOV EDX,dword ptr [ESP + 0x10]
004120ed  MOV word ptr [EAX + 0xc],CX
004120f1  MOV ECX,dword ptr [ESP + 0x14]
004120f5  MOV dword ptr [EAX],EDX
004120f7  MOV EDX,dword ptr [ESP + 0x18]
004120fb  MOV dword ptr [EAX + 0x10],ECX
004120fe  MOV ECX,dword ptr [ESP + 0x1c]
00412102  MOV dword ptr [EAX + 0x14],EDX
00412105  MOV EDX,dword ptr [ESP + 0x20]
00412109  MOV dword ptr [EAX + 0x18],ECX
0041210c  MOV dword ptr [EAX + 0x1c],EDX
0041210f  RET 0x20
