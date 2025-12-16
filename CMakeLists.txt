cmake_minimum_required(VERSION 3.15)
project(arm64disassembler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()
source_group(
    "Header Files/decoders"
    FILES
        include/decoders/decode_data_proc_imm.h
        include/decoders/decode_data_proc_reg.h
        include/decoders/decode_load_stores.h
        include/decoders/decode_branches.h
)
source_group(
    "Source Files/decoders"
    FILES
        src/decoders/decode_data_proc_imm.cpp
        src/decoders/decode_data_proc_reg.cpp
        src/decoders/decode_load_stores.cpp
        src/decoders/decode_branches.cpp
)
add_executable(arm64-disasm 
	include/main.h
	src/main.cpp
	src/disassembler.cpp
	include/disassembler.h
  	src/elf_parser.cpp
  	include/elf_parser.h
   	include/category_levels.h
   	include/decode_util.h
	src/classify.cpp
	include/classify.h
	src/decoders/decode_data_proc_imm.cpp
	include/decoders/decode_data_proc_imm.h
	src/decoders/decode_data_proc_reg.cpp
	include/decoders/decode_data_proc_reg.h
	src/decoders/decode_load_stores.cpp
	include/decoders/decode_load_stores.h
	src/decoders/decode_branches.cpp
	include/decoders/decode_branches.h
	)

target_include_directories(arm64-disasm PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/external
)