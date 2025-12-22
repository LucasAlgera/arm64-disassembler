#include <utility>
#include "category_levels.h"
#include <string>
#include "decode_util.h"

std::string DecodeGlobalCategory0(InstructionData instructionData);
std::string DecodeDATA_PROC_IMM_Category1(InstructionData instructionData);
std::string DecodeLOAD_STORES_Category1(InstructionData instructionData);
std::string DecodeDATA_BRANCH_EXCEPTION_Category1(InstructionData instructionData);
std::string DecodeDATA_PROC_REG_Category1(InstructionData instructionData);