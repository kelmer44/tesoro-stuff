0040ecb0  MOV EAX,dword ptr [ESP + 0x14]
0040ecb4  MOV EDX,dword ptr [ESP + 0xc]
0040ecb8  PUSH ESI
0040ecb9  MOV ESI,ECX
0040ecbb  MOV ECX,dword ptr [ESP + 0x14]
0040ecbf  PUSH EAX
0040ecc0  MOV EAX,dword ptr [ESP + 0x10]
0040ecc4  PUSH ECX
0040ecc5  MOV ECX,dword ptr [ESP + 0x10]
0040ecc9  PUSH EDX
0040ecca  PUSH EAX
0040eccb  PUSH ECX
0040eccc  MOV ECX,ESI
0040ecce  CALL 0x00401aa0
0040ecd3  MOV ECX,dword ptr [ESI + 0x38]
0040ecd6  XOR EAX,EAX
0040ecd8  MOV dword ptr [ESI],0x42ba68
0040ecde  MOV AX,word ptr [ECX + 0x30]
0040ece2  LEA EDX,[EAX + EAX*0x8]
0040ece5  MOV EAX,dword ptr [ECX + 0x1c]
0040ece8  MOV ECX,dword ptr [EAX + EDX*0x4 + 0x18]
0040ecec  MOV EDX,dword ptr [ESI + 0x68]
0040ecef  TEST ECX,ECX
0040ecf1  SETNZ CL
0040ecf4  AND ECX,0x1
0040ecf7  AND EDX,0xfffffffb
0040ecfa  OR ECX,0x142
0040ed00  MOV EAX,ESI
0040ed02  SHL ECX,0x2
0040ed05  OR ECX,EDX
0040ed07  MOV dword ptr [ESI + 0x68],ECX
0040ed0a  POP ESI
0040ed0b  RET 0x14
