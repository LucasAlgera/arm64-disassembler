cmake_minimum_required(VERSION 3.15)
project(arm64disassembler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(arm64-disasm 
    src/main.cpp
    src/disassembler.cpp
    include/disassembler.h
    include/category_levels.h)