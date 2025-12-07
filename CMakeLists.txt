cmake_minimum_required(VERSION 3.15)
project(arm64disassembler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(arm64-disasm 
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
	src/decoders/decode_load_stores.cpp
	include/decoders/decode_load_stores.h
	)

target_include_directories(arm64-disasm PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/external
)