#include <iostream>
#include "disassembler.h"
#include "elf_parser.h"
#include <filesystem>
#include <decode_util.h>
namespace fs = std::filesystem;

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isValidInstruction32(const std::string& s)
{
    if (s.size() != 8)
        return false;

    for (char c : s)
    {
        if (!std::isxdigit(static_cast<unsigned char>(c)))
            return false;
    }

    return true;
}

void entrySingleInstruction()
{
    while (true)
    {
        std::string input;
        std::cout << "Enter AArch64 instruction (8 hex chars): ";
        std::cin >> input;

        if (!isValidInstruction32(input))
        {
            std::cout << "Invalid instruction. Must be exactly 8 hex characters.\n";
            continue;
        }

        uint32_t instruction = static_cast<uint32_t>(
            std::stoul(input, nullptr, 16)
            );

        DecodeGlobalCategory0(EndianSwapper(instruction));
        return;
    }
}

void entryFileDecode()
{
    fs::path folder = "../executables";
    std::vector<fs::path> elfFiles;

    try
    {
        for (const auto& entry : fs::directory_iterator(folder))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".elf")
                elfFiles.push_back(entry.path());
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << '\n';
        return;
    }

    if (elfFiles.empty())
    {
        std::cerr << "No .elf files found.\n";
        return;
    }

    std::cout << "Files in executable dir:\n";
    for (size_t i = 0; i < elfFiles.size(); ++i)
        std::cout << i+1 << ": " << elfFiles[i].filename() << '\n';

    size_t choice;
    std::cout << "Choose file number: ";
    if (!(std::cin >> choice) || choice >= elfFiles.size()+1 || choice <= 0)
    {
        clearInput();
        std::cerr << "Invalid choice.\n";
        return;
    }

    ELFData elfdata;
    LoadELF(elfFiles[choice-1].string(), elfdata);
}

void entry()
{
    while (true)
    {
        std::cout << "\n-------- ARM64 DECODER --------\n";
        std::cout << "1: Disassemble file\n";
        std::cout << "2: Disassemble single instruction\n";
        std::cout << "3: Exit\n";
        std::cout << "Choose action number: ";

        size_t choice;
        if (!(std::cin >> choice))
        {
            clearInput();
            std::cout << "Invalid input.\n";
            continue;
        }

        switch (choice)
        {
        case 1: entryFileDecode(); break;
        case 2: entrySingleInstruction(); break;
        case 3: return;
        default:
            std::cout << "Unknown option.\n";
        }
    }
}