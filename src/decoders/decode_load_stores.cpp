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

    int8_t imm7 = Bits(instruction, 21, 15);
    uint8_t Rt2 = Bits(instruction, 14, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rt = Bits(instruction, 4, 0);

    std::string dI = it->second;

    bool reg_type{};
    reg_type = opc ? 1 : 0;

    std::string t1{};
    std::string t2{};
    int32_t offset{};

    if (!V)
    {
        offset = SignExtend(imm7, 7, 64);
        offset <<= (opc ? 3 : 2);
        t1 = GetRegName(Rt, reg_type);
        t2 = GetRegName(Rt2, reg_type);
    }
    else
    {
        offset = SignExtend(imm7, 7, 64);
        opc ? offset <<= 3 : offset <<= 2;
        if (opc == 0b00)
            t1 = "S" + std::to_string(Rt); t2 = "s" + std::to_string(Rt2);
        if (opc == 0b01)
            t1 = "d" + std::to_string(Rt); t2 = "d" + std::to_string(Rt2);
        if (opc == 0b10)
            t1 = "q" + std::to_string(Rt); t2 = "q" + std::to_string(Rt2);

    }

    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_PIDX)
    {    
        return dI + t1 + "," + t2 +
        ", [" + GetRegName(Rn, true) + "]" +
            ", #" + ToHexFormat(offset);
    }
    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_PRIDX)
    {
        return dI + t1 + "," + t2 +
            ", [" + GetRegName(Rn, true) +
            ", #" + ToHexFormat(offset) + +"]!";
    }
    if (cat == LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_OFFS)
    {
        return dI + t1 + "," + t2 +
            ", [" + GetRegName(Rn, true) +
            ", #" + ToHexFormat(offset) + "]";
    }
}
std::string DecodeLOAD_STORE_REG_Category2(uint32_t instruction)
{
    uint8_t opc = Bits(instruction, 31, 30);
    uint8_t VR = Bits(instruction, 26);

    uint16_t key = (opc << 2) | VR;

    static const std::unordered_map<uint16_t, std::string> table =
    {
        {0b00'0, "ldr "},      //32-bit
        {0b00'1, "ldr "},      //32-bit (SIMD&FP)
        {0b01'0, "ldr "},      //64-bit 
        {0b01'1, "ldr "},      //64-bit (SIMD&FP)
        {0b10'0, "ldrsw "},
        {0b10'1, "ldr "},      //(SIMD&FP)
        {0b11'0, "prfm "},     //128-bit (SIMD&FP)
        {0b11'1, "- "},     
    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI = it->second;

    uint32_t imm19 = Bits(instruction, 23, 5);
    uint8_t Rt = Bits(instruction, 4, 0);

    if (!VR)
        dI += GetRegName(Rt, opc);
    else
    {
        if (opc == 0b00)
            dI += "S" + std::to_string(Rt);
        if(opc == 0b01)
            dI += "D" + std::to_string(Rt);
        if(opc == 0b10)
            dI += "Q" + std::to_string(Rt);
    }

    dI += ", ";
    dI += ToHexFormat(SignExtend(imm19 << 2, 21, 64));

    return dI;
}

