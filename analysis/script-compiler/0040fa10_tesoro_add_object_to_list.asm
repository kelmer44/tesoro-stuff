0040fa10  MOV EAX,dword ptr [ECX]
0040fa12  PUSH EBX
0040fa13  PUSH EBP
0040fa14  MOV EBP,dword ptr [ESP + 0x10]
0040fa18  LEA EDX,[EAX + 0x4]
0040fa1b  PUSH ESI
0040fa1c  PUSH EDI
0040fa1d  MOV dword ptr [EAX + 0x10],EDX
0040fa20  MOV byte ptr [EAX + 0x18],0x1
0040fa24  XOR EBX,EBX
0040fa26  MOV EAX,dword ptr [ECX]
0040fa28  MOV EDX,dword ptr [EAX + 0x10]
0040fa2b  MOV ESI,dword ptr [EDX]
0040fa2d  CMP ESI,EBX
0040fa2f  JZ 0x0040fa5c
0040fa31  LEA EDI,[EAX + 0x4]
0040fa34  CMP EDX,EDI
0040fa36  JNZ 0x0040fa42
0040fa38  CMP byte ptr [EAX + 0x18],BL
0040fa3b  JZ 0x0040fa42
0040fa3d  MOV byte ptr [EAX + 0x18],BL
0040fa40  JMP 0x0040fa45
0040fa42  MOV dword ptr [EAX + 0x10],ESI
0040fa45  MOV EDX,dword ptr [EAX + 0x10]
0040fa48  MOV EDX,dword ptr [EDX]
0040fa4a  CMP EDX,EBX
0040fa4c  JZ 0x0040fa9f
0040fa4e  MOV ESI,dword ptr [EDX + 0x4]
0040fa51  MOV dword ptr [EAX + 0x1c],ESI
0040fa54  MOV EDX,dword ptr [EDX + 0x8]
0040fa57  MOV dword ptr [EAX + 0x20],EDX
0040fa5a  JMP 0x0040fa62
0040fa5c  XOR AL,AL
0040fa5e  CMP AL,BL
0040fa60  JZ 0x0040fa9f
0040fa62  MOV EAX,dword ptr [ECX]
0040fa64  CMP EBP,dword ptr [EAX + 0x20]
0040fa67  JG 0x0040fa26
0040fa69  MOV EDX,EAX
0040fa6b  MOV ESI,dword ptr [ESP + 0x14]
0040fa6f  MOV ECX,dword ptr [EDX]
0040fa71  MOV EAX,dword ptr [ECX]
0040fa73  CMP EAX,EBX
0040fa75  JZ 0x0040fa7d
0040fa77  MOV EDI,dword ptr [EAX]
0040fa79  MOV dword ptr [ECX],EDI
0040fa7b  JMP 0x0040fa86
0040fa7d  MOV EAX,dword ptr [ECX + 0xc]
0040fa80  LEA EDI,[EAX + 0xc]
0040fa83  MOV dword ptr [ECX + 0xc],EDI
0040fa86  MOV ECX,dword ptr [EDX + 0x10]
0040fa89  MOV ECX,dword ptr [ECX]
0040fa8b  MOV dword ptr [EAX],ECX
0040fa8d  MOV EDX,dword ptr [EDX + 0x10]
0040fa90  MOV dword ptr [EDX],EAX
0040fa92  MOV dword ptr [EAX + 0x4],ESI
0040fa95  MOV dword ptr [EAX + 0x8],EBP
0040fa98  POP EDI
0040fa99  POP ESI
0040fa9a  POP EBP
0040fa9b  POP EBX
0040fa9c  RET 0x8
0040fa9f  MOV EAX,dword ptr [ECX]
0040faa1  MOV EDI,dword ptr [ESP + 0x14]
0040faa5  LEA ESI,[EAX + 0x4]
0040faa8  MOV byte ptr [EAX + 0x18],0x1
0040faac  MOV dword ptr [EAX + 0x10],ESI
0040faaf  MOV ECX,dword ptr [EAX + 0x10]
0040fab2  MOV EDX,dword ptr [ECX]
0040fab4  CMP EDX,EBX
0040fab6  JZ 0x0040fae0
0040fab8  CMP ECX,ESI
0040faba  JNZ 0x0040fac6
0040fabc  CMP byte ptr [EAX + 0x18],BL
0040fabf  JZ 0x0040fac6
0040fac1  MOV byte ptr [EAX + 0x18],BL
0040fac4  JMP 0x0040fac9
0040fac6  MOV dword ptr [EAX + 0x10],EDX
0040fac9  MOV ECX,dword ptr [EAX + 0x10]
0040facc  MOV ECX,dword ptr [ECX]
0040face  CMP ECX,EBX
0040fad0  JZ 0x0040fae6
0040fad2  MOV EDX,dword ptr [ECX + 0x4]
0040fad5  MOV dword ptr [EAX + 0x1c],EDX
0040fad8  MOV ECX,dword ptr [ECX + 0x8]
0040fadb  MOV dword ptr [EAX + 0x20],ECX
0040fade  JMP 0x0040faaf
0040fae0  XOR CL,CL
0040fae2  CMP CL,BL
0040fae4  JNZ 0x0040faaf
0040fae6  MOV EDX,dword ptr [EAX]
0040fae8  MOV ECX,dword ptr [EDX]
0040faea  CMP ECX,EBX
0040faec  JZ 0x0040faf4
0040faee  MOV ESI,dword ptr [ECX]
0040faf0  MOV dword ptr [EDX],ESI
0040faf2  JMP 0x0040fafd
0040faf4  MOV ECX,dword ptr [EDX + 0xc]
0040faf7  LEA ESI,[ECX + 0xc]
0040fafa  MOV dword ptr [EDX + 0xc],ESI
0040fafd  MOV EDX,dword ptr [EAX + 0x10]
0040fb00  MOV EDX,dword ptr [EDX]
0040fb02  MOV dword ptr [ECX],EDX
0040fb04  MOV EAX,dword ptr [EAX + 0x10]
0040fb07  MOV dword ptr [EAX],ECX
0040fb09  MOV dword ptr [ECX + 0x4],EDI
0040fb0c  POP EDI
0040fb0d  MOV dword ptr [ECX + 0x8],EBP
0040fb10  POP ESI
0040fb11  POP EBP
0040fb12  POP EBX
0040fb13  RET 0x8
