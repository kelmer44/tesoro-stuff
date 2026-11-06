#pragma author prez demo
#pragma description Colours the x86-32 instructions of build/sum_clamped_O0.bin by kind
#pragma endian little

import std.mem;

// Blue: stack frame (push/pop ebp, mov ebp esp, sub/add esp).
struct Frame1 { u8 opcode; } [[color("4C9AFF"), comment("push or pop ebp")]];
struct FrameMov { u8 opcode; u8 modrm; } [[color("4C9AFF"), comment("mov ebp, esp")]];
struct FrameAdj { u8 opcode; u8 modrm; s8 amount; } [[color("4C9AFF"), comment("sub/add esp: reserve or release local variables")]];

// Green: load from memory into a register.
struct Load { u8 opcode; u8 modrm; s8 disp; } [[color("5FD068"), comment("load register from [ebp+disp]")]];
struct LoadIndexed { u8 opcode; u8 modrm; u8 sib; } [[color("5FD068"), comment("load a[i]: [eax + 4*ecx]")]];

// Teal: store a register or constant into a local variable.
struct Store { u8 opcode; u8 modrm; s8 disp; } [[color("2EC4B6"), comment("store register into [ebp+disp]")]];
struct StoreImm { u8 opcode; u8 modrm; s8 disp; s32 value; } [[color("2EC4B6"), comment("store a constant into [ebp+disp]")]];

// Orange: compare. Red: jumps. Purple: arithmetic. Gold: return.
struct Compare { u8 opcode; u8 modrm; s8 disp; } [[color("FFA726"), comment("cmp register with [ebp+disp]")]];
struct Jump { u8 opcode; s8 offset; } [[color("EF5350"), comment("jump by offset from the end of this instruction")]];
struct ArithMem { u8 opcode; u8 modrm; s8 disp; } [[color("BA68C8"), comment("add register, [ebp+disp]")]];
struct ArithImm { u8 opcode; u8 modrm; s8 value; } [[color("BA68C8"), comment("add register, constant")]];
struct Ret { u8 opcode; } [[color("FFD54F"), comment("return to the caller")]];
struct Unknown { u8 opcode; } [[color("9E9E9E"), comment("not handled by this pattern")]];

// Decodes only the opcodes that the -O0 build uses.
struct Insn {
    u8 b0 = std::mem::read_unsigned($, 1);
    u8 b1 = 0;
    if ($ + 1 < std::mem::size())
        b1 = std::mem::read_unsigned($ + 1, 1);

    if (b0 == 0xC3)
        Ret ret;
    else if (b0 == 0x55 || b0 == 0x5D)
        Frame1 frame1;
    else if (b0 == 0x89 && b1 == 0xE5)
        FrameMov frameMov;
    else if (b0 == 0x83 && (b1 == 0xEC || b1 == 0xC4))
        FrameAdj frameAdj;
    else if (b0 == 0x83 && b1 == 0xC0)
        ArithImm arithImm;
    else if (b0 == 0x03)
        ArithMem arithMem;
    else if (b0 == 0x3B)
        Compare compare;
    else if (b0 == 0x7D || b0 == 0x7E || b0 == 0xEB)
        Jump jump;
    else if (b0 == 0xC7)
        StoreImm storeImm;
    else if (b0 == 0x8B && b1 == 0x04)
        LoadIndexed loadIndexed;
    else if (b0 == 0x8B)
        Load load;
    else if (b0 == 0x89)
        Store store;
    else
        Unknown unknown;
};

Insn instructions[while(!std::mem::eof())] @ 0x00;
