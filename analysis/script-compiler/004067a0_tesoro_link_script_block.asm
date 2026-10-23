004067a0  MOV EAX,ECX
004067a2  PUSH EBX
004067a3  MOV ECX,dword ptr [ESP + 0x8]
004067a7  PUSH ESI
004067a8  PUSH EDI
004067a9  LEA ESI,[EAX + 0x10]
004067ac  MOV EDX,dword ptr [ECX + 0x4]
004067af  MOV dword ptr [EAX + 0x4],EDX
004067b2  MOV EDX,dword ptr [ECX + 0x8]
004067b5  MOV dword ptr [EAX + 0x8],EDX
004067b8  MOV DL,byte ptr [ECX + 0xc]
004067bb  MOV byte ptr [EAX + 0xc],DL
004067be  LEA EDX,[ECX + 0x10]
004067c1  MOV EDI,dword ptr [ECX + 0x10]
004067c4  MOV dword ptr [ESI],EDI
004067c6  MOV EDI,dword ptr [EDX + 0x4]
004067c9  MOV dword ptr [ESI + 0x4],EDI
004067cc  MOV EDI,dword ptr [EDX + 0x8]
004067cf  MOV dword ptr [ESI + 0x8],EDI
004067d2  MOV EDI,0x8
004067d7  MOV EDX,dword ptr [EDX + 0xc]
004067da  MOV dword ptr [ESI + 0xc],EDX
004067dd  MOV EDX,dword ptr [ECX + 0x20]
004067e0  MOV dword ptr [EAX + 0x20],EDX
004067e3  MOV EDX,dword ptr [ECX + 0x24]
004067e6  MOV dword ptr [EAX + 0x24],EDX
004067e9  MOV EDX,dword ptr [ECX + 0x28]
004067ec  MOV dword ptr [EAX + 0x28],EDX
004067ef  MOV EDX,dword ptr [ECX + 0x2c]
004067f2  MOV dword ptr [EAX + 0x2c],EDX
004067f5  MOV DX,word ptr [ECX + 0x30]
004067f9  MOV word ptr [EAX + 0x30],DX
004067fd  MOV DL,byte ptr [ECX + 0x32]
00406800  MOV byte ptr [EAX + 0x32],DL
00406803  MOV DL,byte ptr [ECX + 0x33]
00406806  MOV byte ptr [EAX + 0x33],DL
00406809  MOV EDX,dword ptr [ECX + 0x34]
0040680c  MOV dword ptr [EAX + 0x34],EDX
0040680f  MOV EDX,dword ptr [ECX + 0x38]
00406812  MOV dword ptr [EAX + 0x38],EDX
00406815  MOV EDX,dword ptr [ECX + 0x3c]
00406818  MOV dword ptr [EAX + 0x3c],EDX
0040681b  MOV EDX,dword ptr [ECX + 0x40]
0040681e  MOV dword ptr [EAX + 0x40],EDX
00406821  MOV EDX,dword ptr [ECX + 0x44]
00406824  MOV dword ptr [EAX + 0x44],EDX
00406827  MOV EDX,dword ptr [ECX + 0x48]
0040682a  MOV dword ptr [EAX + 0x48],EDX
0040682d  MOV EDX,dword ptr [ECX + 0x4c]
00406830  MOV dword ptr [EAX + 0x4c],EDX
00406833  MOV EDX,dword ptr [ECX + 0x50]
00406836  MOV dword ptr [EAX + 0x50],EDX
00406839  MOV EDX,dword ptr [ECX + 0x54]
0040683c  MOV dword ptr [EAX + 0x54],EDX
0040683f  MOV EDX,dword ptr [ECX + 0x58]
00406842  MOV dword ptr [EAX + 0x58],EDX
00406845  MOV DL,byte ptr [ECX + 0x5c]
00406848  MOV byte ptr [EAX + 0x5c],DL
0040684b  MOV DL,byte ptr [ECX + 0x5d]
0040684e  MOV byte ptr [EAX + 0x5d],DL
00406851  MOV DX,word ptr [ECX + 0x5e]
00406855  MOV word ptr [EAX + 0x5e],DX
00406859  MOV EDX,dword ptr [ECX + 0x60]
0040685c  MOV dword ptr [EAX + 0x60],EDX
0040685f  MOV DX,word ptr [ECX + 0x64]
00406863  MOV word ptr [EAX + 0x64],DX
00406867  MOV EDX,dword ptr [ECX + 0x68]
0040686a  MOV ESI,ECX
0040686c  MOV dword ptr [EAX + 0x68],EDX
0040686f  LEA EDX,[EAX + 0x6c]
00406872  SUB ESI,EAX
00406874  MOV EBX,dword ptr [ESI + EDX*0x1]
00406877  MOV dword ptr [EDX],EBX
00406879  ADD EDX,0x4
0040687c  DEC EDI
0040687d  JNZ 0x00406874
0040687f  MOV DL,byte ptr [ECX + 0x8c]
00406885  LEA ESI,[EAX + 0x90]
0040688b  MOV byte ptr [EAX + 0x8c],DL
00406891  MOV DL,byte ptr [ECX + 0x8d]
00406897  MOV byte ptr [EAX + 0x8d],DL
0040689d  LEA EDX,[ECX + 0x90]
004068a3  MOV EDI,dword ptr [ECX + 0x90]
004068a9  MOV dword ptr [ESI],EDI
004068ab  MOV EDI,dword ptr [EDX + 0x4]
004068ae  MOV dword ptr [ESI + 0x4],EDI
004068b1  POP EDI
004068b2  MOV EDX,dword ptr [EDX + 0x8]
004068b5  MOV dword ptr [ESI + 0x8],EDX
004068b8  MOV EDX,dword ptr [ECX + 0x9c]
004068be  MOV dword ptr [EAX + 0x9c],EDX
004068c4  MOV EDX,dword ptr [ECX + 0xa0]
004068ca  MOV dword ptr [EAX + 0xa0],EDX
004068d0  MOV EDX,dword ptr [ECX + 0xa4]
004068d6  MOV dword ptr [EAX + 0xa4],EDX
004068dc  MOV EDX,dword ptr [ECX + 0xa8]
004068e2  MOV dword ptr [EAX + 0xa8],EDX
004068e8  MOV EDX,dword ptr [ECX + 0xac]
004068ee  MOV dword ptr [EAX + 0xac],EDX
004068f4  MOV EDX,dword ptr [ECX + 0xb0]
004068fa  MOV dword ptr [EAX + 0xb0],EDX
00406900  MOV EDX,dword ptr [ECX + 0xb4]
00406906  MOV dword ptr [EAX + 0xb4],EDX
0040690c  MOV EDX,dword ptr [ECX + 0xb8]
00406912  MOV dword ptr [EAX + 0xb8],EDX
00406918  MOV EDX,dword ptr [ECX + 0xbc]
0040691e  MOV dword ptr [EAX + 0xbc],EDX
00406924  MOV EDX,dword ptr [ECX + 0xc0]
0040692a  MOV dword ptr [EAX + 0xc0],EDX
00406930  MOV EDX,dword ptr [ECX + 0xc4]
00406936  MOV dword ptr [EAX + 0xc4],EDX
0040693c  MOV EDX,dword ptr [ECX + 0xc8]
00406942  MOV dword ptr [EAX + 0xc8],EDX
00406948  MOV EDX,dword ptr [ECX + 0xcc]
0040694e  MOV dword ptr [EAX + 0xcc],EDX
00406954  MOV EDX,dword ptr [ECX + 0xd0]
0040695a  MOV dword ptr [EAX + 0xd0],EDX
00406960  MOV ECX,dword ptr [ECX + 0xd4]
00406966  POP ESI
00406967  MOV dword ptr [EAX + 0xd4],ECX
0040696d  POP EBX
0040696e  RET 0x4
