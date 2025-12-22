#include "elf_parser.h"
#include "classify.h"
#include "label_inserter.h"

#include <iomanip>
#include <sstream>
#include <elfio/elfio.hpp>
#include <decode_util.h>

// Most elf loading/parsing code can be found at https://github.com/serge1/ELFIO/blob/main/examples/tutorial/tutorial.cpp

static constexpr const char* GREY = "\033[90m"; 
static constexpr const char* WHITE = "\033[37m";
static constexpr const char* BRIGHT_YELLOW = "\033[93m";
static constexpr const char* BRIGHT_BLUE = "\033[94m";
static constexpr const char* RESET = "\033[0m";

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

void LoadELF(std::string& fileName)
{
	elfio reader;
	if (reader.load(fileName))
	{
		std::cout << "Loaded .elf file: " << fileName << std::endl;
	}
	bool arm64 = reader.get_class() == ELFCLASS64;
	bool littleEndian = reader.get_encoding() == ELFDATA2LSB;

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
			std::vector<std::pair<std::string, uint32_t>> dIVec{};
			dIVec.reserve(size/4);
			for (size_t j = 0; j + 3 < size; j += 4) 
			{
				uint32_t instr{};
				uint32_t currentVA = startVA + j;

				if (littleEndian)
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

				InstructionData instructionData{};
				instructionData.instruction = instr;
				instructionData.va = currentVA;
				instructionData.startVa = startVA;
				instructionData.vaLength = startVA + size;
	
				dIVec.push_back(std::make_pair(DecodeGlobalCategory0(instructionData), EndianSwapper(instr)));
			}	

			int idxOffset = 0;

			LabelInserter li;

			// Loop over all instructions to scan for instruction entries
			for (size_t i = 0; i < dIVec.size(); i++)
			{
				li.ScanInstructions(dIVec, i, startVA);
			}

			auto pretty = [](const std::string& text)
				{
					auto pos = text.find('<'); // whenever a function is called
					if (pos == std::string::npos)
						return GREY + text + RESET;

					std::string before = text.substr(0, pos);
					std::string target = text.substr(pos);

					std::stringstream s;
					s << GREY << before
						<< BRIGHT_YELLOW << target
						<< RESET;

					return s.str();
				};

			std::cout << BRIGHT_YELLOW << "_start\n"; // Signify start of .text

			for (size_t i = 0; i < dIVec.size(); i++)
			{
				uint32_t currentVA = startVA + i * 4;

				// Insert possible "functtion start" lables
				li.InsertFunctionLabels(dIVec, i, startVA);

				// Show binary instructions (in little endian)
				std::stringstream binary;
				binary << std::hex << std::uppercase << std::setfill('0')
					<< std::setw(8) << EndianSwapper(dIVec[i].second);


				// Print it all
				std::cout
					<< WHITE << PrintVA(currentVA, 8) << "  "
					<< BRIGHT_BLUE << std::left << std::setw(15) << binary.str()
					<< pretty(dIVec[i].first)
					<< RESET << '\n';
			}
			printf("\n");
		}
	}

	reader.get_entry();
}