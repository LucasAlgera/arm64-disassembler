#include "decoders/decode_branches.h"
#include "decode_util.h"
#include <unordered_map>

std::string DecodeCONDITIONAL_B_Category2(uint32_t instruction)
{
	uint32_t imm19 = Bits(instruction, 23, 5);
	bool o0 = Bits(instruction, 4);
	uint8_t cond = Bits(instruction, 3, 0);
	
	std::string dI{};
	dI = o0 ? "BC." : "B.";

    static const std::unordered_map<uint16_t, std::string> table =
    {
        {0b0000, "eq "},
        {0b0001, "ne "},
        {0b0010, "cs "},
        {0b0011, "cc "},
        {0b0100, "mi "},
        {0b0101, "pl "},
        {0b0110, "vs "},
        {0b0111, "vc "},
        {0b1000, "hi "},
        {0b1001, "ls "},
        {0b1010, "ge "},
        {0b1011, "lt "},
        {0b1100, "gt "},
        {0b1101, "le "},
        {0b1110, "al "},
        {0b1111, "nv "}
    };

    auto it = table.find(cond);
    if (it == table.end())
        return "UNALLOCATED";

    dI += it->second;

    dI += ToHexFormat(SignExtend(imm19 << 2, 21, 64));
	return dI;
}

std::string DecodeHINTS_Category2(uint32_t instruction)
{
    uint8_t CRm = Bits(instruction, 11, 8);
    uint8_t op2 = Bits(instruction, 7, 5);
    
    uint8_t key = (CRm << 7) | op2;

    static const std::unordered_map<uint16_t, std::string> table =
    {
        {0b0000'000, "nop "},
        {0b0000'001, "yield "},
        {0b0000'010, "wfe "},
        {0b0000'011, "wfi "},
        {0b0000'100, "sev "},
        {0b0000'101, "sevl "},
        {0b0000'110, "dgh "},
        {0b0010'000, "esb "},
        {0b0010'001, "psb "},
        {0b0010'010, "tsb "},
        {0b0010'011, "gcsb "},
        {0b0010'100, "csdb "},
        {0b0010'110, "clrbhb "},
    };

    auto it = table.find(key);
    if (it == table.end())
        return std::string("hint #" + std::to_string(key));

    std::string dI{};
    dI += it->second;

    return dI;
}

std::string DecodeSYS_INSTR_Category2(uint32_t instruction)
{
    bool L = Bits(instruction, 21);
    uint8_t op1 = Bits(instruction, 18,16);
    uint8_t CRn = Bits(instruction, 15,12);
    uint8_t CRm = Bits(instruction, 11,8);
    uint8_t op2 = Bits(instruction, 7, 5);
    uint8_t Rt = Bits(instruction,  4, 0);

    std::string dI{};
    dI = L ? "SYSL " : "SYS ";

    if (L)
        dI += GetRegName(Rt, true) + ", #" + std::to_string(op1) + ", C" + std::to_string(CRn) + ", C" + std::to_string(CRm) + ", #" + std::to_string(op2);
    else
        dI += "#" + std::to_string(op1) + ", C" + std::to_string(CRn) + ", C" + std::to_string(CRm) + ", #" + std::to_string(op2) + ", " + GetRegName(Rt, true);

    return dI;
}

std::string DecodeUNCONDITIONAL_B_R_Category2(uint32_t instruction)
{
    uint8_t opc = Bits(instruction, 24, 21);
    uint8_t op2 = Bits(instruction, 20, 16);
    uint8_t op3 = Bits(instruction, 15, 10);
    uint8_t Rn = Bits(instruction, 9, 5);
    uint8_t op4 = Bits(instruction, 4,0);

    uint32_t key{};
    static std::unordered_map<uint32_t, std::string> table{};
    
    if (Rn != 0b11111) // sp reg
    {
        key = (opc << 16) | (op2 << 11) | (op3 << 5) | op4;
        table =
        {
            {0b0000'11111'000000'00000, "br "},
            {0b0001'11111'000000'00000, "blr "},
            {0b0010'11111'000000'00000, "ret "},
        };
    }


    std::string dI{};

    auto it = table.find(key);

    if (it == table.end())
        return "UNALLOCATED";

    dI = it->second;

    if(opc == 0b10)
        dI += (Rn == 30) ? "" : GetRegName(Rn, true); // Defaults to x30, so dont include it if it's that. 
    else
        dI += GetRegName(Rn, true); 
        
    return dI; 
}

std::string DecodeUNCONDITIONAL_B_I_Category2(uint32_t instruction)
{
    bool op = Bits(instruction, 31);
    uint32_t imm26 = Bits(instruction, 25, 0);

    std::string dI; 
    dI += op ? "bl " : "b ";
    dI += ToHexFormat(SignExtend(static_cast<uint64_t>(imm26) << 2, 28, 64));

    return dI;
}
std::string DecodeCOMP_BRANCH_Category2(uint32_t instruction)
{
    bool sf = Bits(instruction, 31);
    bool op = Bits(instruction, 24);
    uint32_t imm19 = Bits(instruction, 23,5);
    uint32_t Rt = Bits(instruction, 4,0);

    std::string dI{};

    dI = op ? "cbnz " : "cbz ";
    dI += GetRegName(Rt, sf);
    dI += ", ";
    dI += ToHexFormat(SignExtend(static_cast<uint16_t>(imm19 << 2), 21, 64));

    return dI;
}
