#include "disassembler.h"
#include <iostream>

enum class InstructionCategory {
    RESERVED,
    SME_ENCODINGS,
    SVE_ENCODINGS,
    DATA_PROC_IMM,
    BRANCH_EXCEPTION,
    DATA_PROC_REG,
    DATA_PROC_SIMD_FP,
    LOAD_STORE,
    UNKNOWN
};

InstructionCategory DecodeCategory(uint32_t instruction)
{
    uint8_t op0 = (instruction >> 31) & 0x01;  // bits [31:29]
    uint8_t op1 = (instruction >> 25) & 0x0F;  // bits [28:25]

    // Reserved
    if ((op0 == 0b0) && (op1 == 0b0000))            // op0: 0 | op1: 0000
    {          
        return InstructionCategory::RESERVED;
    }

    // SME encodings
    if ((op0 == 0b1) && (op1 == 0b0000))            // op0: 1 | op1: 0000
    {          
        return InstructionCategory::SME_ENCODINGS;
    }

    // SVE encodings
    if (op1 == 0b0010)                              // op1: 0010
    {
        return InstructionCategory::SVE_ENCODINGS;
    }

    // Data Processing - immediate
    if ((op1 & 0b1110) == 0b1000)                   // op1: 100x
    {                 
        return InstructionCategory::DATA_PROC_IMM;
    }

    // Branches, exception generating and system instrucitons
    if ((op1 & 0b1110) == 0b1010)                   // op1: 101x
    {                 
        return InstructionCategory::BRANCH_EXCEPTION;
    }

    //Data processing - register
    if ((op1 & 0b0111) == 0b0101)                   // op1: x101
    {
        return InstructionCategory::DATA_PROC_REG;
    }

    //Data processing - Scalar floating-point and advanced SIMD
    if ((op1 & 0b0111) == 0b0111)                   // op1: x111
    {                 
        return InstructionCategory::DATA_PROC_SIMD_FP;
    }

    //Data processing - Scalar floating-point and advanced SIMD
    if ((op1 & 0b0101) == 0b0100)                   // op1: x1x0
    {                 
        return InstructionCategory::LOAD_STORE;
    }

    return InstructionCategory::UNKNOWN; //No match found
}

void Disassembly()
{
    uint32_t inst8 = 0x8B020020;  // ADD X0, X1, X2
    if (DecodeCategory(inst8) == InstructionCategory::DATA_PROC_REG) {
        std::cout << "Data Processing - Register" << std::endl;
    }
}