cmake_minimum_required(VERSION 3.21)
project(SP_Assignment C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES1 main.c )
add_executable(T1 ${SOURCE_FILES1})

set(SOURCE_FILES2 linenoise.c linenoise.h)
add_executable(T2 ${SOURCE_FILES2})

set(SOURCE_FILES3 tshell_func.c tshell_func.h)
add_executable(T3 ${SOURCE_FILES3})

set(SOURCE_FILES4 tokenizer.c)
add_executable(T4 ${SOURCE_FILES3})

set(SOURCE_FILES5 Parser.c)
add_executable(T5 ${SOURCE_FILES3})
