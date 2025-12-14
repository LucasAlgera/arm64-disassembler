#include "decoders/decode_data_proc_reg.h"

#include "decode_util.h"
#include <unordered_map>

std::string DecodeDATA_PROC_2_Category2(uint32_t instruction)
{
    bool sf = Bits(instruction, 31);
    bool S = Bits(instruction, 29);
    uint8_t opcode= Bits(instruction, 15,10);


    uint8_t key = (sf << 8) | (sf << 7) | opcode;

    static const std::unordered_map<uint8_t, std::string> table =
    {
        // 32 bit
        {0b0'0'000010, "udiv "},
        {0b0'0'000011, "sdiv "},
        {0b0'0'001000, "lslv "},
        {0b0'0'001001, "lsrv "},
        {0b0'0'001010, "asrv "},
        {0b0'0'001001, "rorv "},
        {0b0'0'010000, "crc32b "},
        {0b0'0'010001, "crc32h "},
        {0b0'0'010010, "crc32w "},
        {0b0'0'010100, "crc32cb "},
        {0b0'0'010101, "crc32ch "},
        {0b0'0'010110, "crc32cw "},
        {0b0'0'011000, "smax "},
        {0b0'0'011001, "umax "},
        {0b0'0'011010, "smin "},
        {0b0'0'011011, "umin "},

        //64 bit
        {0b1'0'000000, "subp "},
        {0b1'0'000010, "udiv "},
        {0b1'0'000011, "sdiv "},
        {0b1'0'000100, "irg "},
        {0b1'0'000101, "gmi "},
        {0b1'0'001000, "lslv "},
        {0b1'0'001001, "lsrv "},
        {0b1'0'001010, "asrv "},
        {0b1'0'001011, "rorv "},
        {0b1'0'001100, "pacga "},
        {0b1'0'010011, "crc32x "},
        {0b1'0'010111, "crc32cx "},
        {0b0'0'011000, "smax "},
        {0b0'0'011001, "umax "},
        {0b0'0'011010, "smin "},
        {0b0'0'011011, "umin "},
        {0b0'0'000000, "subps "},
    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI{};
    dI = it->second;

    uint8_t Rm = Bits(instruction, 20, 16);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rd = Bits(instruction, 4, 0);

    dI += GetRegName(Rd, sf);
    dI += ", ";
    dI += GetRegName(Rn, sf);
    dI += ", ";
    dI += GetRegName(Rm, sf);

	return dI;
}

std::string DecodeDATA_PROC_1_Category2(uint32_t instruction)
{
    bool sf = Bits(instruction, 31);
    bool S = Bits(instruction, 29);
    uint8_t opcode2 = Bits(instruction, 20, 16);
    uint8_t opcode = Bits(instruction, 15, 10);


    uint16_t key = (sf << 14) | (S << 13) | (opcode2 << 12) | opcode;

    static const std::unordered_map<uint8_t, std::string> table =
    {
        // 32 bit
        {0b0'0'000010'000000 , "rbit "},
        {0b0'0'000010'000001 , "rev16 "},
        {0b0'0'000010'000010 , "rev "},
        {0b0'0'000010'000100 , "clz "},
        {0b0'0'000010'000101 , "cls "},
        {0b0'0'000010'000110 , "ctz "},
        {0b0'0'000010'000111 , "cnt "},
        {0b0'0'000010'001000 , "abs "},

        //64 bit
        {0b1'0'000010'000000 , "rbit "},
        {0b1'0'000010'000001 , "rev16 "},
        {0b1'0'000010'000010 , "rev32 "},
        {0b1'0'000010'000100 , "clz "},
        {0b1'0'000010'000101 , "cls "},
        {0b1'0'000010'000110 , "ctz "},
        {0b1'0'000010'000111 , "cnt "},
        {0b1'0'000010'001000 , "abs "},
    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI{};
    dI = it->second;

    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rd = Bits(instruction, 4, 0);

    dI += GetRegName(Rd, sf);
    dI += ", ";
    dI += GetRegName(Rn, sf);

    return dI;
}



std::string DecodeLOG_SHIFT_Category2(uint32_t instruction)
{
	bool sf = Bits(instruction, 31);
	uint8_t opc = Bits(instruction, 30,29);
	bool N = Bits(instruction, 21);

	uint8_t key = (sf << 3) | (sf << 1) | N;

    static const std::unordered_map<uint8_t, std::string> table =
    {
        {0b0'00'0, "and "},
        {0b0'00'1, "bic "},
        {0b0'01'0, "orr "},
        {0b0'01'1, "orn "},
        {0b0'10'0, "eor "},
        {0b0'10'1, "eon "},
        {0b0'11'0, "ands "},
        {0b0'11'1, "bics "},
        {0b1'00'0, "and "},
        {0b1'00'1, "bic "},
        {0b1'01'0, "orr "},
        {0b1'01'1, "orn "},
        {0b1'10'0, "eor "},
        {0b1'10'1, "eon "},
        {0b1'11'0, "ands "},
        {0b1'11'1, "bics "},
    };

    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";


    uint8_t shift = Bits(instruction, 23, 22);
    uint8_t Rm = Bits(instruction, 20, 16);
    uint8_t imm6 = Bits(instruction, 15, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rd = Bits(instruction, 4, 0);

    std::string dI{};
    dI = it->second;

    dI += GetRegName(Rd, sf);
    dI += ", ";
    dI += GetRegName(Rn, sf);
    dI += ", ";
    dI += GetRegName(Rm, sf);


    if (imm6)
    {
        dI += ", ";
        if (shift == 0b00) dI += "lsl #";
        if (shift == 0b01) dI += "lsr #";
        if (shift == 0b10) dI += "asr #";
        if (shift == 0b11) dI += "ror #";
        dI += ToHexFormat(imm6);
    }

    if (it->second.compare("orr") && !shift && !imm6 && Rn == 31) // alias for orr
        return std::string("mov " + GetRegName(Rd, sf) + ", " + GetRegName(Rm, sf));
    return dI;
}

std::string DecodeADD_SUB_SHIFT_REG_Category2(uint32_t instruction)
{
    bool sf = Bits(instruction, 31);
    bool op = Bits(instruction, 30);
    bool N = Bits(instruction, 29);


    uint8_t key = (op << 1) | N;

    static const std::unordered_map<uint8_t, std::string> table =
    {
        {0b0'0, "add "},
        {0b0'1, "adds "},
        {0b1'0, "sub "},
        {0b1'1, "subs "},
    };
    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI{};
    dI = it->second;

    uint8_t shift = Bits(instruction, 23, 22);
    uint8_t Rm = Bits(instruction, 20, 16);
    uint8_t imm6 = Bits(instruction, 15, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rd = Bits(instruction, 4, 0);

    dI += GetRegName(Rd, sf);
    dI += ", ";
    dI += GetRegName(Rn, sf);
    dI += ", ";
    dI += GetRegName(Rm, sf);

    if (imm6)
    {
        dI += ", ";
        if (shift == 0b00) dI += "lsl #";
        if (shift == 0b01) dI += "lsr #";
        if (shift == 0b10) dI += "asr #";
        if (shift == 0b11) dI += "ror #";
        dI += ToHexFormat(imm6);
    }

    return dI;
}

std::string DecodeADD_SUB_EXT_REG_Category2(uint32_t instruction)
{
    bool sf = Bits(instruction, 31);
    bool op = Bits(instruction, 30);
    bool N = Bits(instruction, 29);


    uint8_t key = (op << 1) | N;

    static const std::unordered_map<uint8_t, std::string> table =
    {
        {0b0'0, "add "},
        {0b0'1, "adds "},
        {0b1'0, "sub "},
        {0b1'1, "subs "},
    };
    auto it = table.find(key);
    if (it == table.end())
        return "UNALLOCATED";

    std::string dI{};
    dI = it->second;

    uint8_t Rm = Bits(instruction, 20, 16);
    bool optionw = Bits(instruction, 15);
    uint8_t option = Bits(instruction, 14, 13);
    uint8_t imm3 = Bits(instruction, 12, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t Rd = Bits(instruction, 4, 0);

    dI += GetRegName(Rd, sf);
    dI += ", ";
    dI += GetRegName(Rn, sf);
    dI += ", ";
    dI += GetRegName(Rm, sf);

    if (imm3)
    {
        dI += ", ";
        dI += optionw ? "s" : "u";
        if (option == 0b00) dI += "xtb #";
        if (option == 0b01) dI += "xth #";
        if (option == 0b10) dI += "xtw #";
        if (option == 0b11) dI += "xtx #";
        dI += ToHexFormat(imm3);
    }

    return dI;
}

std::string DecodeDATA_PROC_3_Category2(uint32_t instruction)
{
    return std::string("DATA PROC 3 instruction");
}
