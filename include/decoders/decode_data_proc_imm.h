// ---------------------------------
// All decoders for the subcategory:
// DATA PROCESSING IMMEDIATE
// ---------------------------------


#include <string>
#include "decode_util.h"


std::string DecodePC_REL_Category2(InstructionData instructionData);
std::string DecodeADD_SUB_IMM_Category2(InstructionData instructionData);
std::string DecodeADD_SUB_IMM_TAG_Category2(InstructionData instructionData);
std::string DecodeMIN_MAX_Category2(InstructionData instructionData);
std::string DecodeLOGICAL_IMM_Category2(InstructionData instructionData);
std::string DecodeMOVE_WIDE_IMM_Category2(InstructionData instructionData);
std::string DecodeBITFIELD_Category2(InstructionData instructionData);
std::string DecodeEXTRACT_Category2(InstructionData instructionData);