00401dc0  MOV AL,byte ptr [ECX + 0x4]
00401dc3  MOV EDX,dword ptr [ECX + 0x34]
00401dc6  MOV byte ptr [ECX + 0x32],AL
00401dc9  AND EAX,0xff
00401dce  MOV EAX,dword ptr [EDX + EAX*0x4]
00401dd1  MOV dword ptr [ECX + 0x38],EAX
00401dd4  MOV ECX,dword ptr [ECX + 0x8]
00401dd7  MOV dword ptr [EAX + 0x18],ECX
00401dda  RET
