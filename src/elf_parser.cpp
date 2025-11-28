#include "elf_parser.h"
#include <elfio/elfio.hpp>
#include <classify.h>

// Most elf loading/parsing code can be found at https://github.com/serge1/ELFIO/blob/main/examples/tutorial/tutorial.cpp

using namespace ELFIO;

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

		// Access to section's data
		if (psec->get_name() == ".text") 
		{
			const unsigned char* data = reinterpret_cast<const unsigned char*>(psec->get_data());
			size_t size = psec->get_size();

			for (size_t j = 0; j + 3 < size; j += 4) 
			{
				uint32_t instr{};
				if (elfData.littleEndian)
				{
					instr =
						uint32_t(data[j]) |
						(uint32_t(data[j + 1]) << 8) |
						(uint32_t(data[j + 2]) << 16) |
						(uint32_t(data[j + 3]) << 24);
				}
				else
				{
					instr =
						uint32_t(data[j] + 3) |
						(uint32_t(data[j + 2]) << 8) |
						(uint32_t(data[j + 1]) << 16) |
						(uint32_t(data[j]) << 24);
				}
				DecodeGlobalCategory0(instr);
			}
			printf("\n");
		}
	}
	reader.get_entry();
}