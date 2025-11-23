#pragma once
#include "category_levels.h"
#include <string>

InstructionCategory0 DecodeGlobalCategory0(uint32_t instruction);
DATA_PROC_IMM_Category1 DecodeDATA_PROC_IMM_Category1(uint32_t instruction);
std::string DecodeADD_SUB_IMM_Category2(uint32_t instruction);
std::string DecodeADD_SUB_IMM_TAG_Category2(uint32_t instruction);
std::string DecodeMIN_MAX_Category2(uint32_t instruction);
std::string DecodeLOGICAL_IMM_Category2(uint32_t instruction);

void Disassembly();
