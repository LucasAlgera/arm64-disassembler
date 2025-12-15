#include "elf_parser.h"
#include <elfio/elfio.hpp>
#include <classify.h>
#include <iomanip>
#include <sstream>

// Most elf loading/parsing code can be found at https://github.com/serge1/ELFIO/blob/main/examples/tutorial/tutorial.cpp

using namespace ELFIO;

static std::string PrintVA(uint32_t va, int vaLenth)
{
	std::stringstream ss;
	ss  << std::hex << std::uppercase << va;
	size_t hexLength = vaLenth - ss.str().length();

	std::string outVA = "0x";

	for (size_t i = 0; i < hexLength; i++)
	{
		outVA += "0";
	}
	outVA += ss.str() + ":";
	return outVA;
}

void LoadELF(std::string& fileName, ELFData& elfData)
{
	elfio reader;
	if (reader.load(fileName))
	{
		std::cout << "Loaded .elf file: " << fileName << std::endl;
	}
	if (reader.get_class() == ELFCLASS64)
	{
		elfData.arm64 = true;
	}
	if (reader.get_encoding() == ELFDATA2LSB)
	{
		elfData.littleEndian = true;
	}

	Elf_Half sec_num = reader.sections.size();
	std::cout << "Number of sections: " << sec_num << std::endl;

	for (int i = 0; i < sec_num; ++i) 
	{
		section* psec = reader.sections[i];
		std::cout << "  [" << i << "] " << psec->get_name() << "\t"
			<< psec->get_size() << std::endl;

		uint32_t startVA = psec->get_address();

		// Access to section's data
		if (psec->get_name() == ".text") 
		{
			const unsigned char* data = reinterpret_cast<const unsigned char*>(psec->get_data());
			size_t size = psec->get_size();

			for (size_t j = 0; j + 3 < size; j += 4) 
			{
				uint32_t currentVA = startVA + j;
				uint32_t instr{};

				if (elfData.littleEndian)
				{
					instr =
						uint8_t(data[j]) |
						(uint8_t(data[j + 1]) << 8) |
						(uint8_t(data[j + 2]) << 16) |
						(uint8_t(data[j + 3]) << 24);
				}
				else
				{
					instr =
						uint8_t(data[j] + 3) |
						(uint8_t(data[j + 2]) << 8) |
						(uint8_t(data[j + 1]) << 16) |
						(uint8_t(data[j]) << 24);
				}
				
				//std::cout << std::hex<<instr << std::endl;
				std::cout << PrintVA(currentVA, 10) << "    ";
				DecodeGlobalCategory0(instr);
				std::cout << std::endl;
			}
			printf("\n");
		}
	}
	reader.get_entry();
}