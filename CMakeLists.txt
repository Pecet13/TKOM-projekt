cmake_minimum_required(VERSION 3.16)

project(tkom_project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)
include_directories(tests)

add_library(Lexer
    include/lexer/Lexer.h
    include/lexer/Position.h
    include/lexer/Token.h
    include/lexer/LexerException.h
    src/lexer/Lexer.cpp
    src/lexer/Position.cpp
    src/lexer/Token.cpp
)
target_include_directories(Lexer PUBLIC include)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

add_executable(LexerTests tests/LexerTests.cpp)
target_link_libraries(LexerTests gtest_main Lexer)
gtest_discover_tests(LexerTests)