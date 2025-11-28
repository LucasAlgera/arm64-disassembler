#include <iostream>
#include "disassembler.h"
#include <elf_parser.h>
int main()
{
    ELFData elfdata;
    LoadELF(std::string("D:/Projects/Year3/arm64-disassembler/main-aarch64.elf"), elfdata);
    Disassembly();
    return 0;
}
