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



//--------------------------------------
//--LOAD_STORES-------------------------
//--------------------------------------

// Only the 9 most important categories 
enum class LOAD_STORES_Category1
{
    //COMPARE_SWAP,
    //AVD_SIMD_MSTRUCT,
    //AVD_SIMD_MSTRUCT_PIDX,
    //AVD_SIMD_SSTRUCT,
    //AVD_SIMD_SSTRUCT_PIDX,
    //RWC_CMP_SWAP,
    //RWC_CMP_SWAP_PAIR,
    //B128_ATM_MEM,
    //GCS_LOAD_STORE,
    //LOAD_STORE_MEM_TAG, 
    //LOAD_STORE_EXC_PAIR, 
    //LOAD_STORE_EXC_REG, 
    //LOAD_STORE_ORDER, 
    //COMP_SWAP, 
    //LDIAPP_STILP, 
    //LDIAPR_STILP,
    LOAD_REG,                            
    //MEMCP_MEMSET,                        
    //LOAD_STORE_NOALLOC_PAIR,             
    LOAD_STORE_REG_PAIR_PIDX,
    LOAD_STORE_REG_PAIR_OFFS,
    LOAD_STORE_REG_PAIR_PRIDX,
    LOAD_STORE_REG_UNSC_IMM,                // 2
    LOAD_STORE_REG_IMM_PIDX,
    //LOAD_STORE_REG_UNPR,                 
    LOAD_STORE_REG_IMM_PRIDX,
    //ATM_MEM_OPERATION,                   
    LOAD_STORE_REG_OFFS,                        // 3
    //LOAD_STORE_REG_PAC,                  
    LOAD_STORE_REG_UIMM, 
    UNKNOWN
};

struct LOAD_STORES_Pattern1
{
    LOAD_STORES_Category1 category;
    uint8_t op0;
    uint8_t op0m;  // mask
    uint8_t op1;   
    uint8_t op1m;  // mask
    uint16_t op2;  
    uint16_t op2m; // maks
};

static const LOAD_STORES_Pattern1 LOAD_STORES_patterns[] =
{
                                                        // op0     op0m    op1  op1m        op2                 op2m
    {LOAD_STORES_Category1::LOAD_REG,                    0b0001,  0b0011,  0b0, 0b0,   0b000000000000000, 0b10000000000000},
    {LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_PIDX,    0b0010,  0b0011,  0b0, 0b0,   0b010000000000000, 0b110000000000000},
    {LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_OFFS,    0b0010,  0b0011,  0b0, 0b0,   0b100000000000000, 0b110000000000000},
    {LOAD_STORES_Category1::LOAD_STORE_REG_PAIR_PRIDX,   0b0010,  0b0011,  0b0, 0b0,   0b110000000000000, 0b110000000000000},
    {LOAD_STORES_Category1::LOAD_STORE_REG_UIMM,         0b0011,  0b0011,  0b0, 0b0,   0b100000000000000, 0b100000000000000},
    {LOAD_STORES_Category1::LOAD_STORE_REG_IMM_PRIDX,    0b0011,  0b0011,  0b0, 0b0,   0b000000000000011, 0b100100000000011},
    {LOAD_STORES_Category1::LOAD_STORE_REG_IMM_PIDX,     0b0011,  0b0011,  0b0, 0b0,   0b000000000000001, 0b100100000000011},
};



//--------------------------------------
//--BRANCH_EXCEPTION--------------------
//--------------------------------------

enum class BRANCH_EXCEPTION_Category1
{
    CONDITIONAL_B,
    SYS_INSTR, 
    HINTS,
    UNCONDITIONAL_B_R, 
    UNCONDITIONAL_B_I,
    COMP_BRANCH,
    UNKNOWN
};

struct BRANCH_EXCEPTION_Pattern1
{
    BRANCH_EXCEPTION_Category1 category;
    uint8_t op0;
    uint8_t op0m;  // mask
    uint16_t op1;
    uint16_t op1m;  // mask
};

static const BRANCH_EXCEPTION_Pattern1 BRANCH_EXCEPTION_Patterns[] =
{
    { BRANCH_EXCEPTION_Category1::CONDITIONAL_B,     0b010, 0b111, 0b00000000000000, 0b11000000000000 },
    { BRANCH_EXCEPTION_Category1::HINTS,             0b110, 0b111, 0b01000000110010, 0b11111111111111 },
    { BRANCH_EXCEPTION_Category1::SYS_INSTR,         0b110, 0b111, 0b01000010000000, 0b11111110000000 },
    { BRANCH_EXCEPTION_Category1::UNCONDITIONAL_B_R, 0b110, 0b111, 0b10000000000000, 0b10000000000000 },
    { BRANCH_EXCEPTION_Category1::UNCONDITIONAL_B_I, 0b000, 0b011, 0b00000000000000, 0b00000000000000 },
    { BRANCH_EXCEPTION_Category1::COMP_BRANCH,       0b001, 0b011, 0b00000000000000, 0b00000000000000 }
};


//--------------------------------------
//--DATA_PROCESSING_REGISTER------------
//--------------------------------------

enum class DATA_PROC_REG_Category1
{
    DATA_PROC_1, 
    DATA_PROC_2, 
    LOG_SHIFT,
    ADD_SUB_SHIFT_REG, 
    ADD_SUB_EXT_REG,
    DATA_PROC_3,
    UNKNOWN
};

struct DATA_PROC_REG_Pattern1
{
    DATA_PROC_REG_Category1 category;
    uint8_t op0;
    uint8_t op0m; // mask
    uint8_t op1;
    uint8_t op1m; // mask
    uint8_t op2;
    uint8_t op2m; // mask
};

static const DATA_PROC_REG_Pattern1 DATA_PROC_REG_patterns[] =
{
                                              // op0   op0m  op1   op1m   op2     op2m
{ DATA_PROC_REG_Category1::DATA_PROC_2,          0b0,  0b1,  0b1,  0b1,  0b0110, 0b1111},
{ DATA_PROC_REG_Category1::DATA_PROC_1,          0b0,  0b1,  0b1,  0b1,  0b0110, 0b1111}, 
{ DATA_PROC_REG_Category1::LOG_SHIFT,            0b0,  0b0,  0b0,  0b1,  0b0000, 0b1000},
{ DATA_PROC_REG_Category1::ADD_SUB_SHIFT_REG,    0b0,  0b0,  0b0,  0b1,  0b1000, 0b1001},
{ DATA_PROC_REG_Category1::ADD_SUB_EXT_REG,      0b0,  0b0,  0b0,  0b1,  0b1001, 0b1001},
{ DATA_PROC_REG_Category1::DATA_PROC_3,          0b0,  0b0,  0b1,  0b1,  0b1000, 0b1000}
};
