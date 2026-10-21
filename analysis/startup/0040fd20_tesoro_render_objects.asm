0040fd20  PUSH EBX
0040fd21  PUSH EBP
0040fd22  PUSH ESI
0040fd23  PUSH EDI
0040fd24  MOV EDI,ECX
0040fd26  XOR EBX,EBX
0040fd28  MOV ESI,0xfffffdff
0040fd2d  MOV EAX,dword ptr [EDI]
0040fd2f  LEA ECX,[EAX + 0x4]
0040fd32  MOV byte ptr [EAX + 0x18],0x1
0040fd36  MOV dword ptr [EAX + 0x10],ECX
0040fd39  MOV EAX,dword ptr [EDI]
0040fd3b  MOV ECX,dword ptr [EAX + 0x10]
0040fd3e  MOV EDX,dword ptr [ECX]
0040fd40  CMP EDX,EBX
0040fd42  JZ 0x0040fd71
0040fd44  LEA EBP,[EAX + 0x4]
0040fd47  CMP ECX,EBP
0040fd49  JNZ 0x0040fd55
0040fd4b  CMP byte ptr [EAX + 0x18],BL
0040fd4e  JZ 0x0040fd55
0040fd50  MOV byte ptr [EAX + 0x18],BL
0040fd53  JMP 0x0040fd58
0040fd55  MOV dword ptr [EAX + 0x10],EDX
0040fd58  MOV EDX,dword ptr [EAX + 0x10]
0040fd5b  MOV ECX,dword ptr [EDX]
0040fd5d  CMP ECX,EBX
0040fd5f  JZ 0x0040fd71
0040fd61  MOV EDX,dword ptr [ECX + 0x4]
0040fd64  MOV dword ptr [EAX + 0x1c],EDX
0040fd67  MOV ECX,dword ptr [ECX + 0x8]
0040fd6a  MOV dword ptr [EAX + 0x20],ECX
0040fd6d  MOV AL,0x1
0040fd6f  JMP 0x0040fd73
0040fd71  XOR AL,AL
0040fd73  CMP AL,BL
0040fd75  JZ 0x0040fd81
0040fd77  MOV EDX,dword ptr [EDI]
0040fd79  MOV EAX,dword ptr [EDX + 0x1c]
0040fd7c  AND dword ptr [EAX + 0x68],ESI
0040fd7f  JMP 0x0040fd39
0040fd81  MOV EAX,dword ptr [EDI]
0040fd83  LEA ECX,[EAX + 0x4]
0040fd86  MOV dword ptr [EAX + 0x10],ECX
0040fd89  MOV byte ptr [EAX + 0x18],0x1
0040fd8d  MOV EAX,dword ptr [EDI]
0040fd8f  MOV ECX,dword ptr [EAX + 0x10]
0040fd92  MOV EDX,dword ptr [ECX]
0040fd94  CMP EDX,EBX
0040fd96  JZ 0x0040fdc5
0040fd98  LEA ESI,[EAX + 0x4]
0040fd9b  CMP ECX,ESI
0040fd9d  JNZ 0x0040fda9
0040fd9f  CMP byte ptr [EAX + 0x18],BL
0040fda2  JZ 0x0040fda9
0040fda4  MOV byte ptr [EAX + 0x18],BL
0040fda7  JMP 0x0040fdac
0040fda9  MOV dword ptr [EAX + 0x10],EDX
0040fdac  MOV EDX,dword ptr [EAX + 0x10]
0040fdaf  MOV ECX,dword ptr [EDX]
0040fdb1  CMP ECX,EBX
0040fdb3  JZ 0x0040fdc5
0040fdb5  MOV EDX,dword ptr [ECX + 0x4]
0040fdb8  MOV dword ptr [EAX + 0x1c],EDX
0040fdbb  MOV ECX,dword ptr [ECX + 0x8]
0040fdbe  MOV dword ptr [EAX + 0x20],ECX
0040fdc1  MOV AL,0x1
0040fdc3  JMP 0x0040fdc7
0040fdc5  XOR AL,AL
0040fdc7  CMP AL,BL
0040fdc9  JZ 0x0040fe0c
0040fdcb  MOV EDX,dword ptr [EDI]
0040fdcd  MOV ECX,dword ptr [EDX + 0x1c]
0040fdd0  MOV EAX,dword ptr [ECX + 0x68]
0040fdd3  MOV EDX,EAX
0040fdd5  SHR EDX,0x1
0040fdd7  TEST DL,0x1
0040fdda  JZ 0x0040fd8d
0040fddc  TEST AH,0x2
0040fddf  JNZ 0x0040fd8d
0040fde1  OR AH,0x2
0040fde4  MOV dword ptr [ECX + 0x68],EAX
0040fde7  MOV EAX,dword ptr [EDI]
0040fde9  MOV ESI,dword ptr [EAX + 0x10]
0040fdec  MOV EAX,dword ptr [EAX + 0x1c]
0040fdef  MOV ECX,EAX
0040fdf1  MOV EDX,dword ptr [EAX]
0040fdf3  CALL dword ptr [EDX + 0x14]
0040fdf6  CMP EAX,0x2
0040fdf9  JZ 0x0040fe11
0040fdfb  CMP EAX,0x3
0040fdfe  JZ 0x0040fe11
0040fe00  MOV EAX,dword ptr [EDI]
0040fe02  CMP ESI,dword ptr [EAX + 0x10]
0040fe05  JZ 0x0040fd8d
0040fe07  JMP 0x0040fd83
0040fe0c  MOV EAX,0x1
0040fe11  POP EDI
0040fe12  POP ESI
0040fe13  POP EBP
0040fe14  POP EBX
0040fe15  RET
