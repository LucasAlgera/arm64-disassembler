#pragma once
#include "category_levels.h"
#include <string>

std::string DecodeADD_SUB_IMM_Category2(uint32_t instruction);
std::string DecodeADD_SUB_IMM_TAG_Category2(uint32_t instruction);
std::string DecodeMIN_MAX_Category2(uint32_t instruction);
std::string DecodeLOGICAL_IMM_Category2(uint32_t instruction);

void Disassembly();
