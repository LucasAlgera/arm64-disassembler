#include "classify.h"

#include "decoders/decode_data_proc_imm.h"
#include "decoders/decode_load_stores.h"
#include "decoders/decode_branches.h"
#include "decoders/decode_data_proc_reg.h"
#include "decode_util.h"

#include <iostream>
#include <iomanip>
#include <sstream>

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
            switch (pattern.category)
            {
            case InstructionCategory0::LOAD_STORE: DecodeLOAD_STORES_Category1(instruction); break; return InstructionCategory0::UNKNOWN;
            case InstructionCategory0::DATA_PROC_IMM: DecodeDATA_PROC_IMM_Category1(instruction); break; return InstructionCategory0::UNKNOWN;
            case InstructionCategory0::BRANCH_EXCEPTION: DecodeDATA_BRANCH_EXCEPTION_Category1(instruction); break; return InstructionCategory0::UNKNOWN;
            case InstructionCategory0::DATA_PROC_REG: DecodeDATA_PROC_REG_Category1(instruction); break; return InstructionCategory0::UNKNOWN;
            default: InstructionCategory0::UNKNOWN;

                std::stringstream ss;
                ss << "// " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << EndianSwapper(instruction);

                std::cout << std::left << std::setw(30) << "_________"
                    << std::setw(50) << ss.str();

                return InstructionCategory0::UNKNOWN; break;
            }
        }
    }
}

void DecodeDATA_PROC_IMM_Category1(uint32_t instruction)
{
    uint8_t op0 = (instruction >> 29) & 0x03; // bits[30:29]
    uint8_t op1 = (instruction >> 22) & 0x0F; // bits[25::22]

    for (const auto& pattern : DATA_PROC_IMM_patterns) {
        bool op0Match = (pattern.op0m == 0b0) || ((op0 & pattern.op0m) == pattern.op0);
        bool op1Match = (op1 & pattern.op1m) == pattern.op1;

        std::string dI;

        if (op0Match && op1Match) {
            switch (pattern.category)
            {
            case DATA_PROC_IMM_Category1::PC_REL:           dI = DecodePC_REL_Category2(instruction);           break; return;
            case DATA_PROC_IMM_Category1::ADD_SUB_IMM :     dI = DecodeADD_SUB_IMM_Category2(instruction);      break; return;
            case DATA_PROC_IMM_Category1::ADD_SUB_IMM_TAG : dI = DecodeADD_SUB_IMM_TAG_Category2(instruction);  break; return;
            case DATA_PROC_IMM_Category1::MIN_MAX_IMM :     dI = DecodeMIN_MAX_Category2(instruction);          break; return;
            case DATA_PROC_IMM_Category1::LOGICAL_IMM :     dI = DecodeLOGICAL_IMM_Category2(instruction);      break; return;
            case DATA_PROC_IMM_Category1::MOVE_WIDE_IMM :   dI = DecodeMOVE_WIDE_IMM_Category2(instruction);    break; return;
            case DATA_PROC_IMM_Category1::BITFIELD :        dI = DecodeBITFIELD_Category2(instruction);         break; return;
            case DATA_PROC_IMM_Category1::EXTRACT:          dI = DecodeEXTRACT_Category2(instruction);          break; return;
            default: DATA_PROC_IMM_Category1::UNKNOWN;      dI = "UNKNOWN DPI"; break; return;
            }

            std::stringstream ss;
            ss << "// " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << EndianSwapper(instruction);

            std::cout << std::left << std::setw(30) << dI
                << std::setw(50) << ss.str();

        }
    }

    return;
}

void DecodeLOAD_STORES_Category1(uint32_t instruction)
{
    uint8_t op0 = Bits(instruction, 31,28);   // bits[31:28]
    uint8_t op1 = Bits(instruction, 26);      // bits[26]
    uint16_t op2 = Bits(instruction, 24, 10);  // bits[24:10]

    for (const auto& pattern : LOAD_STORES_patterns) {
        bool op0Match = (op0 & pattern.op0m) == pattern.op0;
        bool op1Match = (pattern.op1m == 0b0) || ((op1 & pattern.op1m) == pattern.op1);
        bool op2Match = (pattern.op2m == 0b0) || ((op2 & pattern.op2m) == pattern.op2);

        std::string dI;
        using LS = LOAD_STORES_Category1;
        if (op0Match && op1Match && op2Match) 
        {
            switch (pattern.category) {
            case LS::LOAD_REG:                  dI = DecodeLOAD_STORE_REG_Category2(instruction);                                       break; return;
            case LS::LOAD_STORE_REG_UIMM:       dI = DecodeLOAD_STORE_REG_IMM_Category2(instruction, LS::LOAD_STORE_REG_UIMM);          break; return;
            case LS::LOAD_STORE_REG_IMM_PRIDX:  dI = DecodeLOAD_STORE_REG_IMM_Category2(instruction, LS::LOAD_STORE_REG_IMM_PRIDX);     break; return;
            case LS::LOAD_STORE_REG_IMM_PIDX:   dI = DecodeLOAD_STORE_REG_IMM_Category2(instruction, LS::LOAD_STORE_REG_IMM_PIDX);      break; return;
            case LS::LOAD_STORE_REG_PAIR_PIDX:  dI = DecodeLOAD_STORE_REG_PAIR_Category2(instruction, LS::LOAD_STORE_REG_PAIR_PIDX);    break; return;
            case LS::LOAD_STORE_REG_PAIR_OFFS:  dI = DecodeLOAD_STORE_REG_PAIR_Category2(instruction, LS::LOAD_STORE_REG_PAIR_OFFS);    break; return;
            case LS::LOAD_STORE_REG_PAIR_PRIDX: dI = DecodeLOAD_STORE_REG_PAIR_Category2(instruction, LS::LOAD_STORE_REG_PAIR_PRIDX);   break; return;
            default: LS::UNKNOWN; dI = "UNKNOWN LS"; break; return;

            }
            std::stringstream ss;
            ss << "// " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << EndianSwapper(instruction);

            std::cout << std::left << std::setw(30) << dI
                << std::setw(50) << ss.str();
            return;
        }
    }
}
void DecodeDATA_BRANCH_EXCEPTION_Category1(uint32_t instruction)
{
    uint8_t op0 = Bits(instruction, 31, 29);
    uint16_t op1 = Bits(instruction, 25,12);

    for (const auto& pattern : BRANCH_EXCEPTION_Patterns) {
        bool op0Match = (op0 & pattern.op0m) == pattern.op0;
        bool op1Match = (pattern.op1m == 0b0) || ((op1 & pattern.op1m) == pattern.op1);

        std::string dI{};
        using LS = BRANCH_EXCEPTION_Category1;
        if (op0Match && op1Match)
        {
            switch (pattern.category) {
            case LS::CONDITIONAL_B: dI = DecodeCONDITIONAL_B_Category2(instruction); break; return;
            case LS::SYS_INSTR: dI = DecodeSYS_INSTR_Category2(instruction); break; return;
            case LS::UNCONDITIONAL_B_R: dI = DecodeUNCONDITIONAL_B_R_Category2(instruction); break; return;
            case LS::UNCONDITIONAL_B_I: dI = DecodeUNCONDITIONAL_B_I_Category2(instruction); break; return;
            case LS::COMP_BRANCH: dI = DecodeCOMP_BRANCH_Category2(instruction); break; return;
            default: LS::UNKNOWN; dI = "UNKNOWN BE"; break; return; 

            }

            std::stringstream ss;
            ss << "// " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << EndianSwapper(instruction);
            
            std::cout << std::left << std::setw(30) << dI
                << std::setw(50) << ss.str();
            return;
        }
    }
    std::stringstream ss;
    ss << "// " << std::hex << std::uppercase << instruction;

    std::cout << std::left << std::setw(30) << "UNKOWN BE"
        << std::setw(50) << ss.str();
}




void DecodeDATA_PROC_REG_Category1(uint32_t instruction)
{
    uint8_t op0 = Bits(instruction, 30);
    uint8_t op1 = Bits(instruction, 28);
    uint8_t op2 = Bits(instruction, 24,21);

    for (const auto& pattern : DATA_PROC_REG_patterns) {
        bool op0Match = (pattern.op0m == 0b0) || ((op0 & pattern.op0m) == pattern.op0);
        bool op1Match = (op1 & pattern.op1m) == pattern.op1;
        bool op2Match = (pattern.op2m == 0b0) || ((op2 & pattern.op2m) == pattern.op2);

        std::string dI;

        if (op0Match && op1Match && op2Match) {
            switch (pattern.category)
            {
            case DATA_PROC_REG_Category1::DATA_PROC_2:  dI = DecodeDATA_PROC_2_Category2(instruction);             break; return;
            case DATA_PROC_REG_Category1::DATA_PROC_1:  dI = DecodeDATA_PROC_1_Category2(instruction);             break; return;
            case DATA_PROC_REG_Category1::LOG_SHIFT:    dI = DecodeLOG_SHIFT_Category2(instruction);     break; return;
            case DATA_PROC_REG_Category1::ADD_SUB_SHIFT_REG:    dI = DecodeADD_SUB_SHIFT_REG_Category2(instruction);     break; return;
            case DATA_PROC_REG_Category1::ADD_SUB_EXT_REG:    dI = DecodeADD_SUB_EXT_REG_Category2(instruction);     break; return;
            default: DATA_PROC_REG_Category1::UNKNOWN;      dI = "UNKNOWN DPR"; break; return;
            }

            std::stringstream ss;
            ss << "// " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << EndianSwapper(instruction);

            std::cout << std::left << std::setw(30) << dI
                << std::setw(50) << ss.str();

        }
    }

    return;
}
