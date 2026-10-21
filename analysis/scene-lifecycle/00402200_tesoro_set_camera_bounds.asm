00402200  MOV EAX,dword ptr [ESP + 0x4]
00402204  MOV EDX,dword ptr [ESP + 0x8]
00402208  ADD ECX,0x1c
0040220b  MOV dword ptr [ECX],EAX
0040220d  MOV EAX,dword ptr [ESP + 0xc]
00402211  MOV dword ptr [ECX + 0x4],EDX
00402214  MOV EDX,dword ptr [ESP + 0x10]
00402218  MOV dword ptr [ECX + 0x8],EAX
0040221b  MOV dword ptr [ECX + 0xc],EDX
0040221e  RET 0x10
