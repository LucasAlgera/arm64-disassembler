#include "disassembler.h"
#include <iostream>
#include <map>
#include <utility>

// Last number behind the functions signify the level that category lives inside of the arm64 encoding tree.

InstructionCategory0 DecodeGlobalCategory0(uint32_t instruction)
{
    uint8_t op0 = (instruction >> 31) & 0x01;  // bit [31]
    uint8_t op1 = (instruction >> 25) & 0x0F;  // bits [28:25]

    // if ((op0 == 0b0) && (op1 == 0b0000))            // op0: 0 | op1: 0000
    // {
    //     return InstructionCategory0::RESERVED;
    // }

    for (const auto& pattern : globalPatterns) {
        bool op0Match = (pattern.op0m == 0b0) || ((op0 & pattern.op0m) == pattern.op0);
        bool op1Match = (op1 & pattern.op1m) == pattern.op1;

        if (op0Match && op1Match) {
            return pattern.category;
        }
    }

    return InstructionCategory0::UNKNOWN;
}



void Disassembly()
{
    uint32_t inst0 = 0x8B020020;  // ADD X0, X1, X2
    if (DecodeGlobalCategory0(inst0) == InstructionCategory0::DATA_PROC_REG) {
        std::cout << "Data Processing - Register" << std::endl;
    }
}