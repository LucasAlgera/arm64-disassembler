#pragma once
#include <cstdint>

// Last number behind the classes and structs signify the level that category lives inside of the arm64 encoding tree.


//--------------------------------------
//--HIGHEST_LEVEL-----------------------
//--------------------------------------

enum class InstructionCategory0
{
    RESERVED,
    SME_ENCODINGS,
    SVE_ENCODINGS,
    DATA_PROC_IMM,
    BRANCH_EXCEPTION,
    DATA_PROC_REG,
    DATA_PROC_SIMD_FP,
    LOAD_STORE,
    UNKNOWN
};

struct CategoryPattern0
{
    InstructionCategory0 category;
    uint8_t op0;
    uint8_t op0m; // mask
    uint8_t op1;
    uint8_t op1m; // mask
};

static const CategoryPattern0 globalPatterns[] =
{
                                             // op0  op0m   op1    op1m
    {InstructionCategory0::RESERVED,            0b0, 0b1, 0b0000, 0b1111},
    {InstructionCategory0::SME_ENCODINGS,       0b1, 0b1, 0b0000, 0b1111},
    {InstructionCategory0::SVE_ENCODINGS,       0b0, 0b0, 0b0010, 0b1111},
    {InstructionCategory0::DATA_PROC_IMM,       0b0, 0b0, 0b1000, 0b1110},
    {InstructionCategory0::BRANCH_EXCEPTION,    0b0, 0b0, 0b1010, 0b1110},
    {InstructionCategory0::DATA_PROC_REG,       0b0, 0b0, 0b0101, 0b0111},
    {InstructionCategory0::DATA_PROC_SIMD_FP,   0b0, 0b0, 0b0111, 0b0111},
    {InstructionCategory0::LOAD_STORE,          0b0, 0b0, 0b0100, 0b0101}
};





//--------------------------------------
//--DATA_PROCESSING_IMMEDIATE-----------
//--------------------------------------

enum class DATA_PROC_IMM_Category1
{
    PC_REL,
    ADD_SUB_IMM,
    ADD_SUB_IMM_TAG,
    MIN_MAX_IMM,
    LOGICAL_IMM,
    MOVE_WIDE_IMM,
    BITFIELD,
    EXTRACT,
    DATA_PROC,
    UNKNOWN
};

struct DATA_PROC_IMM_Pattern1
{
    DATA_PROC_IMM_Category1 category;
    uint8_t op0;
    uint8_t op0m; // mask
    uint8_t op1;
    uint8_t op1m; // mask
};

static const DATA_PROC_IMM_Pattern1 DATA_PROC_IMM_patterns[] =
{
                                             // op0   op0m    op1    op1m
    {DATA_PROC_IMM_Category1::PC_REL,           0b0,  0b0,  0b0000, 0b1100}, // op1: 00xx
    {DATA_PROC_IMM_Category1::ADD_SUB_IMM,      0b0,  0b0,  0b0100, 0b1110}, // op1: 010x
    {DATA_PROC_IMM_Category1::ADD_SUB_IMM_TAG,  0b0,  0b0,  0b0110, 0b1111}, // op1: 0110
    {DATA_PROC_IMM_Category1::MIN_MAX_IMM,      0b0,  0b0,  0b0111, 0b1111}, // op1: 0111
    {DATA_PROC_IMM_Category1::LOGICAL_IMM,      0b0,  0b0,  0b1000, 0b1110}, // op1: 100x
    {DATA_PROC_IMM_Category1::MOVE_WIDE_IMM,    0b0,  0b0,  0b1010, 0b1110}, // op1: 101x
    {DATA_PROC_IMM_Category1::BITFIELD,         0b0,  0b0,  0b1100, 0b1110}, // op1: 110x
    {DATA_PROC_IMM_Category1::DATA_PROC,        0b11, 0b11, 0b1110, 0b1110}, // op0: 11, op1: 111x
    {DATA_PROC_IMM_Category1::EXTRACT,          0b0,  0b11, 0b1110, 0b1110}  // op0: !=11, op1: 111x
};