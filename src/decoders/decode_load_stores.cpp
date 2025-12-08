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
        {0b00'0'00, "strb "},
        {0b00'0'01, "ldrb "},
        {0b00'0'10, "ldrsb "},   // 64-bit sign-extend
        {0b00'0'11, "ldrsb "},   // 32-bit sign-extend

        {0b00'1'00, "str "},     // 8-bit SIMD/FP
        {0b00'1'01, "ldr "},
        {0b00'1'10, "str "},     // 128-bit SIMD/FP
        {0b00'1'11, "ldr "},

        {0b01'0'00, "strh "},
        {0b01'0'01, "ldrh "},
        {0b01'0'10, "ldrsh "},   // 64
        {0b01'0'11, "ldrsh "},   // 32

        {0b01'1'00, "str "},     // 16-bit SIMD/FP
        {0b01'1'01, "ldr "},

        {0b10'0'00, "str "},     // 32-bit GP
        {0b10'0'01, "ldr "},
        {0b10'0'10, "ldrsw "},

        {0b10'1'00, "str "},     // 32-bit SIMD&FP
        {0b10'1'01, "ldr "},

        {0b11'0'00, "str "},     // 64-bit GP
        {0b11'0'01, "ldr "},
        {0b11'0'10, "prfm "},

        {0b11'1'00, "str "},     // 64-bit SIMD&FP
        {0b11'1'01, "ldr "},
    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI = it->second;

    uint32_t imm12 = Bits(instruction, 21, 10);
    uint32_t Rn = Bits(instruction, 9, 5);
    uint32_t Rt = Bits(instruction, 4, 0);

    // special case
    if (dI == "prfm")
        return "prfm " + std::to_string(imm12) + ", [x" + std::to_string(Rn) + "]";

    bool reg_type{};
    if (size == 0b11 || dI == "ldrsw" || (dI.substr(0, 5) == "ldrs" && opc == 0b10))
        reg_type = true;
    else
        reg_type = false;

    uint32_t offset = imm12 << size; // ARM rule: offset = imm12 << size

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_IMM_PIDX)
        return dI + GetRegName(Rt, reg_type) +
        ", [" + GetRegName(Rn, true) + "]" +
        ", #" + std::to_string(offset);

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_IMM_PRIDX)
        return dI + GetRegName(Rt, reg_type) +
        ", [" + GetRegName(Rn, true) +
        ", #" + std::to_string(offset) + "]!";

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_UIMM)
        return dI + GetRegName(Rt, reg_type) +
        ", [" + GetRegName(Rn, true) +
        ", #" + std::to_string(offset) + "]";

    return dI;
}

std::string DecodeLOAD_STORE_REG_PAIR_Category2(uint32_t instruction, LOAD_STORES_Category1 cat)
{
    uint8_t opc = Bits(instruction, 31, 30);
    uint8_t V = Bits(instruction, 26);
    uint8_t L = Bits(instruction, 22);

    uint16_t key = (opc << 2) | (V << 1) | L;

    static const std::unordered_map<uint16_t, std::string> table =
    {
        {0b00'0'0, "stp "},      //32-bit
        {0b00'0'1, "ldp "},      //32-bit
        {0b00'1'0, "stp "},      //32-bit (SIMD&FP)
        {0b00'1'1, "ldp "},      //32-bit (SIMD&FP)
        {0b01'0'0, "stgp "},
        {0b01'0'1, "ldpsw "},
        {0b01'1'0, "stp "},      //64-bit (SIMD&FP)
        {0b01'1'1, "ldp "},      //64-bit (SIMD&FP)
        {0b10'0'0, "stp "},      //64-bit
        {0b10'0'1, "ldp "},      //64-bit
        {0b10'1'0, "stp "},      //128-bit (SIMD&FP)
        {0b10'1'1, "ldp "},      //128-bit (SIMD&FP)

    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    uint8_t imm7 = Bits(instruction, 21, 15);
    uint8_t Rt2 = Bits(instruction, 14, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rt = Bits(instruction, 4, 0);

    std::string dI = it->second;

    bool reg_type{};
    reg_type = opc ? 1 : 0;

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_PIDX)
    {
        uint32_t offset = opc ? imm7 * 8: imm7 * 4;
        
        return dI + GetRegName(Rt, reg_type) + "," + GetRegName(Rt2, reg_type) +
        ", [" + GetRegName(Rn, true) + "]" +
            ", #" + "offset";
    }
    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_PRIDX)
    {
        uint32_t offset = opc ? imm7 * 8 : imm7 * 4;

        return dI + GetRegName(Rt, reg_type) + "," + GetRegName(Rt2, reg_type) +
            ", [" + GetRegName(Rn, true) +
            ", #" + "offset" + +"]!";
    }
    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_OFFS)
    {
        uint32_t offset = opc ? imm7 * 8 : imm7 * 4;

        return dI + GetRegName(Rt, reg_type) + "," + GetRegName(Rt2, reg_type) +
            ", [" + GetRegName(Rn, true) +
            ", #" + "offset" + "]";
    }
}
