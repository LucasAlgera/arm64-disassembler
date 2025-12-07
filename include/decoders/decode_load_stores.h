#pragma once
#include "category_levels.h"
#include <string>

std::string DecodeLOAD_STORE_REG_IMM_Category2(uint32_t instruction, LOAD_STORES_Category1 cat);
std::string DecodeLOAD_STORE_REG_PAIR_Category2(uint32_t instruction, LOAD_STORES_Category1 cat);
