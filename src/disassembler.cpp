#include "disassembler.h"
#include <iostream>
#include <map>
#include <utility>
#include <sstream>
#include "decoders/decode_data_proc_imm.h"
#include "classify.h"
#include <decode_util.h>

// Last number behind the functions signify the level that category lives inside of the arm64 encoding tree.

void PrintTest(const std::string& desc, uint32_t instruction)
{
    (DecodeGlobalCategory0(instruction) == InstructionCategory0::DATA_PROC_IMM);
}

void Disassembly()
{
    PrintTest("test", 0xD280001D);
}




// DATA PROC IMM
// TODO: 
// EXTRACT

// DONE: 
// PC-REL
// ADD/SUB
// ADD/SUB (TAG)
// MIN/MAX
// LOGICAL
// MOVE WIDE
// BITFIELD