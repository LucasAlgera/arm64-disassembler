#include "classify.h"
#include "decoders/decode_data_proc_imm.h"
#include "decoders/decode_load_stores.h"

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
            default: InstructionCategory0::UNKNOWN; std::cout << "-----" << std::endl; break;
            }
        }
    }

    return InstructionCategory0::UNKNOWN;
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
            case DATA_PROC_IMM_Category1::PC_REL: dI = DecodePC_REL_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::ADD_SUB_IMM : dI = DecodeADD_SUB_IMM_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::ADD_SUB_IMM_TAG : dI = DecodeADD_SUB_IMM_TAG_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::MIN_MAX_IMM : dI = DecodeMIN_MAX_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::LOGICAL_IMM : dI = DecodeLOGICAL_IMM_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::MOVE_WIDE_IMM : dI = DecodeMOVE_WIDE_IMM_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::BITFIELD : dI = DecodeBITFIELD_Category2(instruction); break; return;
            case DATA_PROC_IMM_Category1::EXTRACT: dI = DecodeEXTRACT_Category2(instruction); break; return;
            default: DATA_PROC_IMM_Category1::UNKNOWN; dI = "UNKNOWN"; break; return;
            }

            std::stringstream ss;
            ss << "0x" << std::hex << std::uppercase << instruction;

            std::cout << std::left << std::setw(15) << ss.str()
                << std::setw(8) << ">>>"
                << dI << std::endl;

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
            case LS::LOAD_STORE_REG_UIMM:       dI = DecodeLOAD_STORE_REG_IMM_Category2(instruction, LS::LOAD_STORE_REG_UIMM);          break; return;
            case LS::LOAD_STORE_REG_IMM_PRIDX:  dI = DecodeLOAD_STORE_REG_IMM_Category2(instruction, LS::LOAD_STORE_REG_IMM_PRIDX);     break; return;
            case LS::LOAD_STORE_REG_IMM_PIDX:   dI = DecodeLOAD_STORE_REG_IMM_Category2(instruction, LS::LOAD_STORE_REG_IMM_PIDX);      break; return;
            case LS::LOAD_STORE_REG_PAIR_PIDX:  dI = DecodeLOAD_STORE_REG_PAIR_PIDX_Category2(instruction);                             break; return;
            default: LS::UNKNOWN; dI = "UNKNOWN"; break;

            }
            std::stringstream ss;
            ss << "0x" << std::hex << std::uppercase << instruction;

            std::cout << std::left << std::setw(15) << ss.str()
                << std::setw(8) << ">>>"
                << dI << std::endl;
        }
    }
    return;
}
