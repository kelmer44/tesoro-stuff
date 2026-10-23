0040fb20  MOV EDX,dword ptr [ECX]
0040fb22  PUSH EBX
0040fb23  PUSH EBP
0040fb24  MOV EBP,dword ptr [ESP + 0xc]
0040fb28  LEA EAX,[EDX + 0x4]
0040fb2b  PUSH ESI
0040fb2c  PUSH EDI
0040fb2d  MOV dword ptr [EDX + 0x10],EAX
0040fb30  MOV byte ptr [EDX + 0x18],0x1
0040fb34  XOR EBX,EBX
0040fb36  MOV EDX,dword ptr [ECX]
0040fb38  MOV ESI,dword ptr [EDX + 0x10]
0040fb3b  MOV EDI,dword ptr [ESI]
0040fb3d  CMP EDI,EBX
0040fb3f  JZ 0x0040fb6e
0040fb41  LEA EAX,[EDX + 0x4]
0040fb44  CMP ESI,EAX
0040fb46  JNZ 0x0040fb52
0040fb48  CMP byte ptr [EDX + 0x18],BL
0040fb4b  JZ 0x0040fb52
0040fb4d  MOV byte ptr [EDX + 0x18],BL
0040fb50  JMP 0x0040fb55
0040fb52  MOV dword ptr [EDX + 0x10],EDI
0040fb55  MOV EAX,dword ptr [EDX + 0x10]
0040fb58  MOV ESI,dword ptr [EAX]
0040fb5a  CMP ESI,EBX
0040fb5c  JZ 0x0040fb6e
0040fb5e  MOV EAX,dword ptr [ESI + 0x4]
0040fb61  MOV dword ptr [EDX + 0x1c],EAX
0040fb64  MOV EAX,dword ptr [ESI + 0x8]
0040fb67  MOV dword ptr [EDX + 0x20],EAX
0040fb6a  MOV DL,0x1
0040fb6c  JMP 0x0040fb70
0040fb6e  XOR DL,DL
0040fb70  CMP DL,BL
0040fb72  JZ 0x0040fba2
0040fb74  MOV EDX,dword ptr [ECX]
0040fb76  CMP dword ptr [EDX + 0x1c],EBP
0040fb79  JNZ 0x0040fb36
0040fb7b  MOV EDI,dword ptr [EDX + 0x10]
0040fb7e  MOV ESI,dword ptr [EDI]
0040fb80  MOV EAX,dword ptr [ESI]
0040fb82  MOV dword ptr [EDI],EAX
0040fb84  MOV EAX,dword ptr [EDX]
0040fb86  MOV EAX,dword ptr [EAX]
0040fb88  MOV dword ptr [ESI],EAX
0040fb8a  MOV EDX,dword ptr [EDX]
0040fb8c  MOV dword ptr [EDX],ESI
0040fb8e  MOV ECX,dword ptr [ECX]
0040fb90  LEA EAX,[ECX + 0x4]
0040fb93  MOV dword ptr [ECX + 0x10],EAX
0040fb96  MOV AL,0x1
0040fb98  MOV byte ptr [ECX + 0x18],AL
0040fb9b  POP EDI
0040fb9c  POP ESI
0040fb9d  POP EBP
0040fb9e  POP EBX
0040fb9f  RET 0x4
0040fba2  POP EDI
0040fba3  POP ESI
0040fba4  POP EBP
0040fba5  XOR AL,AL
0040fba7  POP EBX
0040fba8  RET 0x4
