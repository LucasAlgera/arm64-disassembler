#pragma once
#include "category_levels.h"
#include <string>
#include "decode_util.h"

std::string DecodeLOAD_STORE_REG_IMM_Category2(InstructionData instructionData, LOAD_STORES_Category1 cat);
std::string DecodeLOAD_STORE_REG_PAIR_Category2(InstructionData instructionData, LOAD_STORES_Category1 cat);
std::string DecodeLOAD_STORE_REG_Category2(InstructionData instructionData);
