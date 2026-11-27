#pragma author prez demo
#pragma description One labelled item per instruction of build/sum_to_O0.bin (written for this exact build)
#pragma endian little

// Blue: prologue (set up the stack frame). Indigo: epilogue (tear it down). Green: loads. Teal: stores. Orange: compare. Red: jumps. Purple: arithmetic.
// One flat item per instruction, so hovering any byte shows that instruction's own comment.

u8 prologue_push[1] @ 0x00 [[color("4C9AFF"), comment("push ebp: prologue, start of the function")]];
u8 prologue_mov[2] @ 0x01 [[color("4C9AFF"), comment("mov ebp, esp: prologue, set up the stack frame")]];
u8 prologue_sub[3] @ 0x03 [[color("4C9AFF"), comment("sub esp, 8: prologue, room for 2 local variables")]];
u8 i06_load[3]   @ 0x06 [[color("5FD068"), comment("mov eax, [ebp+8]: load n (unused here, -O0 does it anyway)")]];
u8 i09_store[7]  @ 0x09 [[color("2EC4B6"), comment("mov [ebp-4], 0: total = 0")]];
u8 i10_store[7]  @ 0x10 [[color("2EC4B6"), comment("mov [ebp-8], 1: i = 1")]];
u8 i17_load[3]   @ 0x17 [[color("5FD068"), comment("mov eax, [ebp-8]: load i")]];
u8 i1a_cmp[3]    @ 0x1a [[color("FFA726"), comment("cmp eax, [ebp+8]: compare i with n")]];
u8 i1d_jg[2]     @ 0x1d [[color("EF5350"), comment("jg +0x14: if i > n, jump to 0x33 and leave the loop")]];
u8 i1f_load[3]   @ 0x1f [[color("5FD068"), comment("mov eax, [ebp-8]: load i")]];
u8 i22_add[3]    @ 0x22 [[color("BA68C8"), comment("add eax, [ebp-4]: i + total")]];
u8 i25_store[3]  @ 0x25 [[color("2EC4B6"), comment("mov [ebp-4], eax: total += i")]];
u8 i28_load[3]   @ 0x28 [[color("5FD068"), comment("mov eax, [ebp-8]: load i")]];
u8 i2b_add[3]    @ 0x2b [[color("BA68C8"), comment("add eax, 1: the i++ (the 01 at 0x2d is the amount)")]];
u8 i2e_store[3]  @ 0x2e [[color("2EC4B6"), comment("mov [ebp-8], eax: store i")]];
u8 i31_jmp[2]    @ 0x31 [[color("EF5350"), comment("jmp 0x17: back to the loop test")]];
u8 i33_load[3]   @ 0x33 [[color("5FD068"), comment("mov eax, [ebp-4]: load total, the return value")]];
u8 epilogue_add[3] @ 0x36 [[color("5C6BC0"), comment("add esp, 8: epilogue, release the locals")]];
u8 epilogue_pop[1] @ 0x39 [[color("5C6BC0"), comment("pop ebp: epilogue, restore the caller's frame")]];
u8 epilogue_ret[1] @ 0x3a [[color("5C6BC0"), comment("ret: epilogue, return to the caller")]];
