#include <decoders/decode_data_proc_imm.h>
#include <decode_util.h>

std::string DecodePC_REL_Category2(uint32_t instruction)
{
    uint8_t op = (instruction >> 31) & 1;       // ADR,ADRP
    uint8_t immlo = Bits(instruction, 30, 29);  // flags?
    uint32_t immhi = Bits(instruction, 23, 5);  // flags?
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    dI = (op ? "ADRP " : "ADR ");

    dI += GetRegName(Rd, true);
    dI += ", SignExtend(";
    dI += std::to_string(immhi);
    dI += ":";
    dI += std::to_string(immlo);
    dI += ", 64)";

    return dI;
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

    dI += GetRegName(Rd, sf);
    dI += ", ";

    dI += GetRegName(Rn, sf);
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

    dI += GetRegName(Rd, true);
    dI += ", ";

    dI += GetRegName(Rn, true);
    dI += ", #";

    dI += std::to_string(imm6 * 16);
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

    dI += GetRegName(Rd, sf);
    dI += ", ";

    dI += GetRegName(Rn, sf);
    dI += ", #";

    dI += std::to_string(imm8);

    return dI;
}

std::string DecodeLOGICAL_IMM_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t opc = Bits(instruction, 30, 29);    // instruction type
    uint8_t N = (instruction >> 22) & 1;        // flags?
    uint8_t immr = Bits(instruction, 21, 16);    //immediate
    uint8_t imms = Bits(instruction, 15, 10);    // immediate
    uint8_t Rn = Bits(instruction, 9, 5);       // source reg
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    switch (opc)
    {
    case 0b00: dI = "AND "; break;
    case 0b01: dI = "ORR "; break;
    case 0b10: dI = "EOR "; break;
    case 0b11: dI = "ANDS "; break;
    default:    dI = "UNDEFINED "; break;
    }

    dI += GetRegName(Rd, sf);
    dI += ", ";

    dI += GetRegName(Rn, sf);
    dI += ", #";

    dI += "decodeBitMasks(" + std::to_string(N) + ", ";
    dI += std::to_string(imms);
    dI += ", ";
    dI += std::to_string(immr);
    dI += ", TRUE, size())";

    return dI;
}


std::string DecodeMOVE_WIDE_IMM_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t opc = Bits(instruction, 30, 29);    // instruction type
    uint8_t hw = Bits(instruction, 22, 21);     // shift
    uint16_t imm16 = Bits(instruction, 20, 5);  // immediate
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    switch (opc)
    {
    case 0b00: dI = "MOVN "; break;
    case 0b10: dI = "MOVZ "; break;
    case 0b11: dI = "MOVK "; break;
    default:    dI = "UNDEFINED "; break;
    }

    dI += GetRegName(Rd, sf);

    dI += ", #";
    dI += std::to_string(imm16);
    dI += ", ";

    dI += "LSL#";
    dI += std::to_string(hw * 16); // shift by 0/16 (if 64bit also 32 or 48)
    return dI;
}
std::string DecodeBITFIELD_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t opc = Bits(instruction, 30, 29);    // instruction type
    uint8_t N = (instruction >> 22) & 1;        //
    uint16_t immr = Bits(instruction, 21, 16);  // immediate
    uint16_t imms = Bits(instruction, 15, 10);  // immediate
    uint8_t Rn = Bits(instruction, 9, 5);       // source reg
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI;

    switch (opc)
    {
    case 0b00: dI = "SBFM "; break;
    case 0b01: dI = "BFM "; break;
    case 0b10: dI = "UBFM "; break;
    default:    dI = "UNDEFINED "; break;
    }

    dI += GetRegName(Rd, sf);
    dI += ", ";

    dI += GetRegName(Rn, sf);
    dI += ", #";

    dI += std::to_string(immr);
    dI += ", #";
    dI += std::to_string(imms);

    return dI;
}

std::string DecodeEXTRACT_Category2(uint32_t instruction)
{
    uint8_t sf = (instruction >> 31) & 1;       // 64/32 bit
    uint8_t opc = Bits(instruction, 30, 29);    // instruction type
    uint8_t N = (instruction >> 22) & 1;        //
    uint16_t immr = Bits(instruction, 21, 16);  // immediate
    uint16_t imms = Bits(instruction, 15, 10);  // immediate
    uint8_t Rn = Bits(instruction, 9, 5);       // source reg
    uint8_t Rd = Bits(instruction, 4, 0);       // dest reg

    std::string dI = "EXTRACT";

    return dI;
}