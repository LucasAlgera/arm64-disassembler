#include "elf_parser.h"
#include <elfio/elfio.hpp>

using namespace ELFIO;

void LoadELF(std::string& fileName, ELFData& data)
{
	elfio reader;
	if (reader.load(fileName))
	{
		std::cout << "Loaded .elf file: " << fileName << std::endl;
	}
	if (reader.get_class() == ELFCLASS64)
	{
		data.arm64 = true;
	}
	if (reader.get_encoding() == ELFDATA2LSB)
	{
		data.littleEndian = true;
	}
}