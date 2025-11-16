#pragma once
#include <string>

struct ELFData
{
	bool littleEndian;
	bool arm64;
};

void LoadELF(std::string& fileName, ELFData& data);