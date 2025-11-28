// ---------------------------------
// All decoders for the subcategory:
// DATA PROCESSING IMMEDIATE
// ---------------------------------


#include <string>


std::string DecodePC_REL_Category2(uint32_t instruction);
std::string DecodeADD_SUB_IMM_Category2(uint32_t instruction);
std::string DecodeADD_SUB_IMM_TAG_Category2(uint32_t instruction);
std::string DecodeMIN_MAX_Category2(uint32_t instruction);
std::string DecodeLOGICAL_IMM_Category2(uint32_t instruction);
std::string DecodeMOVE_WIDE_IMM_Category2(uint32_t instruction);
std::string DecodeBITFIELD_Category2(uint32_t instruction);
std::string DecodeEXTRACT_Category2(uint32_t instruction);