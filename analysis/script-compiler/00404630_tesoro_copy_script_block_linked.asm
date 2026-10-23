00404630  PUSH EBX
00404631  PUSH ESI
00404632  PUSH EDI
00404633  MOV ESI,ECX
00404635  CALL 0x0040ec90
0040463a  MOV EAX,dword ptr [ESP + 0x10]
0040463e  MOV dword ptr [ESI],0x428078
00404644  MOV ECX,dword ptr [EAX + 0x4]
00404647  MOV dword ptr [ESI + 0x4],ECX
0040464a  MOV EDX,dword ptr [EAX + 0x8]
0040464d  MOV dword ptr [ESI + 0x8],EDX
00404650  MOV CL,byte ptr [EAX + 0xc]
00404653  LEA EDX,[EAX + 0x10]
00404656  MOV byte ptr [ESI + 0xc],CL
00404659  LEA ECX,[ESI + 0x10]
0040465c  MOV EDI,dword ptr [EDX]
0040465e  MOV dword ptr [ECX],EDI
00404660  MOV EDI,dword ptr [EDX + 0x4]
00404663  MOV dword ptr [ECX + 0x4],EDI
00404666  MOV EDI,dword ptr [EDX + 0x8]
00404669  MOV dword ptr [ECX + 0x8],EDI
0040466c  MOV EDI,0x8
00404671  MOV EDX,dword ptr [EDX + 0xc]
00404674  MOV dword ptr [ECX + 0xc],EDX
00404677  MOV ECX,dword ptr [EAX + 0x20]
0040467a  MOV dword ptr [ESI + 0x20],ECX
0040467d  MOV EDX,dword ptr [EAX + 0x24]
00404680  MOV dword ptr [ESI + 0x24],EDX
00404683  MOV ECX,dword ptr [EAX + 0x28]
00404686  MOV dword ptr [ESI + 0x28],ECX
00404689  MOV EDX,dword ptr [EAX + 0x2c]
0040468c  MOV dword ptr [ESI + 0x2c],EDX
0040468f  MOV CX,word ptr [EAX + 0x30]
00404693  MOV word ptr [ESI + 0x30],CX
00404697  MOV DL,byte ptr [EAX + 0x32]
0040469a  MOV byte ptr [ESI + 0x32],DL
0040469d  MOV CL,byte ptr [EAX + 0x33]
004046a0  MOV byte ptr [ESI + 0x33],CL
004046a3  MOV EDX,dword ptr [EAX + 0x34]
004046a6  MOV dword ptr [ESI + 0x34],EDX
004046a9  MOV ECX,dword ptr [EAX + 0x38]
004046ac  MOV dword ptr [ESI + 0x38],ECX
004046af  MOV EDX,dword ptr [EAX + 0x3c]
004046b2  MOV dword ptr [ESI + 0x3c],EDX
004046b5  MOV ECX,dword ptr [EAX + 0x40]
004046b8  MOV dword ptr [ESI + 0x40],ECX
004046bb  MOV EDX,dword ptr [EAX + 0x44]
004046be  MOV dword ptr [ESI + 0x44],EDX
004046c1  MOV ECX,dword ptr [EAX + 0x48]
004046c4  MOV dword ptr [ESI + 0x48],ECX
004046c7  MOV EDX,dword ptr [EAX + 0x4c]
004046ca  MOV dword ptr [ESI + 0x4c],EDX
004046cd  MOV ECX,dword ptr [EAX + 0x50]
004046d0  MOV dword ptr [ESI + 0x50],ECX
004046d3  MOV EDX,dword ptr [EAX + 0x54]
004046d6  MOV dword ptr [ESI + 0x54],EDX
004046d9  MOV ECX,dword ptr [EAX + 0x58]
004046dc  MOV dword ptr [ESI + 0x58],ECX
004046df  MOV DL,byte ptr [EAX + 0x5c]
004046e2  MOV byte ptr [ESI + 0x5c],DL
004046e5  MOV CL,byte ptr [EAX + 0x5d]
004046e8  MOV byte ptr [ESI + 0x5d],CL
004046eb  MOV DX,word ptr [EAX + 0x5e]
004046ef  MOV word ptr [ESI + 0x5e],DX
004046f3  MOV ECX,dword ptr [EAX + 0x60]
004046f6  MOV dword ptr [ESI + 0x60],ECX
004046f9  MOV DX,word ptr [EAX + 0x64]
004046fd  MOV word ptr [ESI + 0x64],DX
00404701  MOV ECX,dword ptr [EAX + 0x68]
00404704  MOV EDX,EAX
00404706  MOV dword ptr [ESI + 0x68],ECX
00404709  LEA ECX,[ESI + 0x6c]
0040470c  SUB EDX,ESI
0040470e  MOV EBX,dword ptr [EDX + ECX*0x1]
00404711  MOV dword ptr [ECX],EBX
00404713  ADD ECX,0x4
00404716  DEC EDI
00404717  JNZ 0x0040470e
00404719  MOV DL,byte ptr [EAX + 0x8c]
0040471f  MOV byte ptr [ESI + 0x8c],DL
00404725  MOV CL,byte ptr [EAX + 0x8d]
0040472b  LEA EDX,[EAX + 0x90]
00404731  MOV byte ptr [ESI + 0x8d],CL
00404737  LEA ECX,[ESI + 0x90]
0040473d  MOV EDI,dword ptr [EDX]
0040473f  MOV dword ptr [ECX],EDI
00404741  MOV EDI,dword ptr [EDX + 0x4]
00404744  MOV dword ptr [ECX + 0x4],EDI
00404747  POP EDI
00404748  MOV EDX,dword ptr [EDX + 0x8]
0040474b  MOV dword ptr [ECX + 0x8],EDX
0040474e  MOV ECX,dword ptr [EAX + 0x9c]
00404754  MOV dword ptr [ESI + 0x9c],ECX
0040475a  MOV EDX,dword ptr [EAX + 0xa0]
00404760  MOV dword ptr [ESI + 0xa0],EDX
00404766  MOV ECX,dword ptr [EAX + 0xa4]
0040476c  MOV dword ptr [ESI + 0xa4],ECX
00404772  MOV EDX,dword ptr [EAX + 0xa8]
00404778  MOV dword ptr [ESI + 0xa8],EDX
0040477e  MOV ECX,dword ptr [EAX + 0xac]
00404784  MOV dword ptr [ESI + 0xac],ECX
0040478a  MOV EDX,dword ptr [EAX + 0xb0]
00404790  MOV dword ptr [ESI + 0xb0],EDX
00404796  MOV ECX,dword ptr [EAX + 0xb4]
0040479c  MOV dword ptr [ESI + 0xb4],ECX
004047a2  MOV EDX,dword ptr [EAX + 0xb8]
004047a8  MOV dword ptr [ESI + 0xb8],EDX
004047ae  MOV ECX,dword ptr [EAX + 0xbc]
004047b4  MOV dword ptr [ESI + 0xbc],ECX
004047ba  MOV EDX,dword ptr [EAX + 0xc0]
004047c0  MOV dword ptr [ESI + 0xc0],EDX
004047c6  MOV ECX,dword ptr [EAX + 0xc4]
004047cc  MOV dword ptr [ESI + 0xc4],ECX
004047d2  MOV EDX,dword ptr [EAX + 0xc8]
004047d8  MOV dword ptr [ESI + 0xc8],EDX
004047de  MOV ECX,dword ptr [EAX + 0xcc]
004047e4  MOV dword ptr [ESI + 0xcc],ECX
004047ea  MOV EDX,dword ptr [EAX + 0xd0]
004047f0  MOV dword ptr [ESI + 0xd0],EDX
004047f6  MOV EAX,dword ptr [EAX + 0xd4]
004047fc  MOV dword ptr [ESI + 0xd4],EAX
00404802  MOV EAX,ESI
00404804  POP ESI
00404805  POP EBX
00404806  RET 0x4
