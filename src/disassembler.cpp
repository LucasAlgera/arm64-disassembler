#include "disassembler.h"
#include <iostream>
#include <map>
#include <utility>
#include <sstream>

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

DATA_PROC_IMM_Category1 DecodeDATA_PROC_IMM_Category1(uint32_t instruction)
{
    uint8_t op0 = (instruction >> 29) & 0x03; // bits[30:29]
    uint8_t op1 = (instruction >> 22) & 0x0F; // bits[25::22]

    for (const auto& pattern : DATA_PROC_IMM_patterns) {
        bool op0Match = (pattern.op0m == 0b0) || ((op0 & pattern.op0m) == pattern.op0);
        bool op1Match = (op1 & pattern.op1m) == pattern.op1;

        if (op0Match && op1Match) {
            return pattern.category;
        }
    }

    return DATA_PROC_IMM_Category1::UNKNOWN;
}

constexpr uint32_t Bits(uint32_t value, int hi, int lo = -1) 
{
    if (lo == -1) lo = hi;
    return (value >> lo) & ((1u << (hi - lo + 1)) - 1);
}

std::string DecodeADD_SUB_IMM_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t op = (instruction >> 30) & 1;       // ADD,SUB
    uint8_t S = (instruction >> 29) & 1;        // flags?
    uint8_t sh = (instruction >> 22) & 1;       // shift?
    uint16_t imm12 = Bits(instruction, 21, 10);  // immediate
    uint8_t Rn = Bits(instruction, 9, 5);       // source reg
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    if (!op)
        dI = (S ? "ADDS " : "ADD ");
    else
        dI = (S ? "SUBS " : "SUB ");

    dI += sf ? "X" : "W";
    dI += std::to_string(Rd);
    dI += ", ";

    dI += sf ? "X" : "W";
    dI += std::to_string(Rn);
    dI += ", #";

    int imm = sh ? (imm12 << 12) : imm12;
    dI += std::to_string(imm);
    
    return dI;
}

std::string DecodeADD_SUB_IMM_TAG_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t op = (instruction >> 30) & 1;       // SUMB,ADDG
    uint8_t S = (instruction >> 29) & 1;        // flags?
    uint8_t imm6 = Bits(instruction, 21, 16);   // immediate
    uint8_t imm4 = Bits(instruction, 13, 10);   // immediate
    uint8_t Rn = Bits(instruction, 9, 5);       // source reg
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    dI = (op ? "SUBG " : "ADDG ");

    dI += "X";
    dI += std::to_string(Rd);
    dI += ", ";

    dI += "X";
    dI += std::to_string(Rn);
    dI += ", #";

    dI += std::to_string(imm6);
    dI += ", #";
    dI += std::to_string(imm4);

    return dI;
}

std::string DecodeMIN_MAX_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t op = (instruction >> 30) & 1;       //
    uint8_t S = (instruction >> 29) & 1;        // flags?
    uint8_t opc = Bits(instruction, 21, 18);    // MIN/MAX
    uint8_t imm8 = Bits(instruction, 17, 10);   // immediate
    uint8_t Rn = Bits(instruction, 9, 5);       // source reg
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    switch (opc) 
    {
        case 0b0000: dI = "SMAX "; break;
        case 0b0001: dI = "UMAX "; break;
        case 0b0010: dI = "SMIN "; break;
        case 0b0011: dI = "UMIN "; break;
        default:    dI = "UNDEFINED "; break;
    }

    dI += sf ? "X" : "W";
    dI += std::to_string(Rd);
    dI += ", ";

    dI += sf ? "X" : "W";
    dI += std::to_string(Rn);
    dI += ", #";

    dI += std::to_string(imm8);

    return dI;
}

void Disassembly()
{
    uint32_t inst0 = 0x8B020020;  // ADD X0, X1, X2
    if (DecodeGlobalCategory0(inst0) == InstructionCategory0::DATA_PROC_REG) {
        std::cout << "Data Processing - Register" << std::endl;
    }

    uint32_t inst1 = 0x91001420;
    if (DecodeDATA_PROC_IMM_Category1(inst1) == DATA_PROC_IMM_Category1::ADD_SUB_IMM) {
        std::cout << "Add/subtract immediate" << std::endl;
    }
    std::cout << DecodeADD_SUB_IMM_Category2(inst1);
}




// DATA PROC IMM
// TODO: 
// PC-REL
// LOGICAL
// MOVE WIDE
// BITFIELD
// EXTRACT

// DONE: 
// ADD/SUB
// ADD/SUB (TAG)
// MIN/MAX