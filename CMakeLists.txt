cmake_minimum_required(VERSION 3.16)

project(tkom_project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

add_library(Lexer
    include/lexer/Lexer.h
    include/lexer/Position.h
    include/lexer/Token.h
    src/lexer/Lexer.cpp
    src/lexer/Position.cpp
)
target_include_directories(Lexer PUBLIC include)