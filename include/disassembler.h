#include <cstdint>

enum class InstructionCategory;
InstructionCategory DecodeCategory(uint32_t instruction);
void Disassembly();
