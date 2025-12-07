#include "decoders/decode_load_stores.h"
#include <decode_util.h>
#include <unordered_map>

std::string DecodeLOAD_STORE_REG_IMM_Category2(uint32_t instruction, LOAD_STORES_Category1 cat)
{
    uint8_t size = Bits(instruction, 31, 30);
    uint8_t V = Bits(instruction, 26);
    uint8_t opc = Bits(instruction, 23, 22);

    uint16_t key = (size << 3) | (V << 2) | opc;

    static const std::unordered_map<uint16_t, std::string> table =
    {
        {0b00'0'00, "STRB "},
        {0b00'0'01, "LDRB "},
        {0b00'0'10, "LDRSB "},   // 64-bit sign-extend
        {0b00'0'11, "LDRSB "},   // 32-bit sign-extend

        {0b00'1'00, "STR "},     // 8-bit SIMD/FP
        {0b00'1'01, "LDR "},
        {0b00'1'10, "STR "},     // 128-bit SIMD/FP
        {0b00'1'11, "LDR "},

        {0b01'0'00, "STRH "},
        {0b01'0'01, "LDRH "},
        {0b01'0'10, "LDRSH "},   // 64
        {0b01'0'11, "LDRSH "},   // 32

        {0b01'1'00, "STR "},     // 16-bit SIMD/FP
        {0b01'1'01, "LDR "},

        {0b10'0'00, "STR "},     // 32-bit GP
        {0b10'0'01, "LDR "},
        {0b10'0'10, "LDRSW "},

        {0b10'1'00, "STR "},     // 32-bit SIMD&FP
        {0b10'1'01, "LDR "},

        {0b11'0'00, "STR "},     // 64-bit GP
        {0b11'0'01, "LDR "},
        {0b11'0'10, "PRFM "},

        {0b11'1'00, "STR "},     // 64-bit SIMD&FP
        {0b11'1'01, "LDR "},
    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI = it->second;

    uint32_t imm12 = Bits(instruction, 21, 10);
    uint32_t Rn = Bits(instruction, 9, 5);
    uint32_t Rt = Bits(instruction, 4, 0);

    // special case
    if (dI == "PRFM")
        return "PRFM " + std::to_string(imm12) + ", [X" + std::to_string(Rn) + "]";

    std::string reg_type{};
    if (size == 0b11 || dI == "LDRSW" || (dI.substr(0, 5) == "LDRS" && opc == 0b10))
        reg_type = "X";
    else
        reg_type = "W";

    uint32_t offset = imm12 << size; // ARM rule: offset = imm12 << size

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_IMM_PIDX)
        return dI + reg_type + std::to_string(Rt) +
        ", [X" + std::to_string(Rn) + "]" +
        ", #" + std::to_string(offset);

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_IMM_PRIDX)
        return dI + reg_type + std::to_string(Rt) +
        ", [X" + std::to_string(Rn) +
        ", #" + std::to_string(offset) + "]!";

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_UIMM)
        return dI + reg_type + std::to_string(Rt) +
        ", [X" + std::to_string(Rn) +
        ", #" + std::to_string(offset) + "]";

    return dI;
}

std::string DecodeLOAD_STORE_REG_PAIR_PIDX_Category2(uint32_t instruction)
{
    uint8_t opc = Bits(instruction, 31, 30);
    uint8_t V = Bits(instruction, 26);
    uint8_t L = Bits(instruction, 22);

    uint16_t key = (opc << 2) | (V << 1) | L;

    static const std::unordered_map<uint16_t, std::string> table =
    {
        {0b00'0'0, "STP"},      //32-bit
        {0b00'0'1, "LDP"},      //32-bit
        {0b00'1'0, "STP"},      //32-bit (SIMD&FP)
        {0b00'1'1, "LDP"},      //32-bit (SIMD&FP)
        {0b01'0'0, "STGP"},
        {0b01'0'1, "LDPSW"},
        {0b01'1'0, "STP"},      //64-bit (SIMD&FP)
        {0b01'1'1, "LDP"},      //64-bit (SIMD&FP)
        {0b10'0'0, "STP"},      //64-bit
        {0b10'0'1, "LDP"},      //64-bit
        {0b10'1'0, "STP"},      //128-bit (SIMD&FP)
        {0b10'1'1, "LDP"},      //128-bit (SIMD&FP)

    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    uint8_t imm7 = Bits(instruction, 21, 15);
    uint8_t Rt2 = Bits(instruction, 14, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rt = Bits(instruction, 4, 0);

    std::string dI = it->second;

    dI += opc ? "X" : "W";
    std::to_string(Rt);
    dI += opc ? ",X" : ",W";
    std::to_string(Rn);


    return std::string("LOAD_STORE_REG_PAIR_PIDX!! \n");
}
