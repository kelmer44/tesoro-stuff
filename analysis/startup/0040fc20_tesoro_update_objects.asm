0040fc20  PUSH EBX
0040fc21  PUSH EBP
0040fc22  PUSH ESI
0040fc23  PUSH EDI
0040fc24  MOV EDI,ECX
0040fc26  XOR EBX,EBX
0040fc28  MOV ESI,0xfffffdff
0040fc2d  MOV EAX,dword ptr [EDI]
0040fc2f  LEA ECX,[EAX + 0x4]
0040fc32  MOV byte ptr [EAX + 0x18],0x1
0040fc36  MOV dword ptr [EAX + 0x10],ECX
0040fc39  MOV EAX,dword ptr [EDI]
0040fc3b  MOV ECX,dword ptr [EAX + 0x10]
0040fc3e  MOV EDX,dword ptr [ECX]
0040fc40  CMP EDX,EBX
0040fc42  JZ 0x0040fc71
0040fc44  LEA EBP,[EAX + 0x4]
0040fc47  CMP ECX,EBP
0040fc49  JNZ 0x0040fc55
0040fc4b  CMP byte ptr [EAX + 0x18],BL
0040fc4e  JZ 0x0040fc55
0040fc50  MOV byte ptr [EAX + 0x18],BL
0040fc53  JMP 0x0040fc58
0040fc55  MOV dword ptr [EAX + 0x10],EDX
0040fc58  MOV EDX,dword ptr [EAX + 0x10]
0040fc5b  MOV ECX,dword ptr [EDX]
0040fc5d  CMP ECX,EBX
0040fc5f  JZ 0x0040fc71
0040fc61  MOV EDX,dword ptr [ECX + 0x4]
0040fc64  MOV dword ptr [EAX + 0x1c],EDX
0040fc67  MOV ECX,dword ptr [ECX + 0x8]
0040fc6a  MOV dword ptr [EAX + 0x20],ECX
0040fc6d  MOV AL,0x1
0040fc6f  JMP 0x0040fc73
0040fc71  XOR AL,AL
0040fc73  CMP AL,BL
0040fc75  JZ 0x0040fc81
0040fc77  MOV EDX,dword ptr [EDI]
0040fc79  MOV EAX,dword ptr [EDX + 0x1c]
0040fc7c  AND dword ptr [EAX + 0x68],ESI
0040fc7f  JMP 0x0040fc39
0040fc81  MOV EAX,dword ptr [EDI]
0040fc83  LEA ECX,[EAX + 0x4]
0040fc86  MOV dword ptr [EAX + 0x10],ECX
0040fc89  MOV byte ptr [EAX + 0x18],0x1
0040fc8d  MOV EAX,dword ptr [EDI]
0040fc8f  MOV ECX,dword ptr [EAX + 0x10]
0040fc92  MOV EDX,dword ptr [ECX]
0040fc94  CMP EDX,EBX
0040fc96  JZ 0x0040fcc5
0040fc98  LEA ESI,[EAX + 0x4]
0040fc9b  CMP ECX,ESI
0040fc9d  JNZ 0x0040fca9
0040fc9f  CMP byte ptr [EAX + 0x18],BL
0040fca2  JZ 0x0040fca9
0040fca4  MOV byte ptr [EAX + 0x18],BL
0040fca7  JMP 0x0040fcac
0040fca9  MOV dword ptr [EAX + 0x10],EDX
0040fcac  MOV EDX,dword ptr [EAX + 0x10]
0040fcaf  MOV ECX,dword ptr [EDX]
0040fcb1  CMP ECX,EBX
0040fcb3  JZ 0x0040fcc5
0040fcb5  MOV EDX,dword ptr [ECX + 0x4]
0040fcb8  MOV dword ptr [EAX + 0x1c],EDX
0040fcbb  MOV ECX,dword ptr [ECX + 0x8]
0040fcbe  MOV dword ptr [EAX + 0x20],ECX
0040fcc1  MOV AL,0x1
0040fcc3  JMP 0x0040fcc7
0040fcc5  XOR AL,AL
0040fcc7  CMP AL,BL
0040fcc9  JZ 0x0040fd0c
0040fccb  MOV EDX,dword ptr [EDI]
0040fccd  MOV ECX,dword ptr [EDX + 0x1c]
0040fcd0  MOV EAX,dword ptr [ECX + 0x68]
0040fcd3  MOV EDX,EAX
0040fcd5  SHR EDX,0x1
0040fcd7  TEST DL,0x1
0040fcda  JZ 0x0040fc8d
0040fcdc  TEST AH,0x2
0040fcdf  JNZ 0x0040fc8d
0040fce1  OR AH,0x2
0040fce4  MOV dword ptr [ECX + 0x68],EAX
0040fce7  MOV EAX,dword ptr [EDI]
0040fce9  MOV ESI,dword ptr [EAX + 0x10]
0040fcec  MOV EAX,dword ptr [EAX + 0x1c]
0040fcef  MOV ECX,EAX
0040fcf1  MOV EDX,dword ptr [EAX]
0040fcf3  CALL dword ptr [EDX + 0x10]
0040fcf6  CMP EAX,0x2
0040fcf9  JZ 0x0040fd11
0040fcfb  CMP EAX,0x3
0040fcfe  JZ 0x0040fd11
0040fd00  MOV EAX,dword ptr [EDI]
0040fd02  CMP ESI,dword ptr [EAX + 0x10]
0040fd05  JZ 0x0040fc8d
0040fd07  JMP 0x0040fc83
0040fd0c  MOV EAX,0x1
0040fd11  POP EDI
0040fd12  POP ESI
0040fd13  POP EBP
0040fd14  POP EBX
0040fd15  RET
