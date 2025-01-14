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

add_library(Parser
    include/parser/Parser.h
    include/parser/ParserException.h
    include/parser/nodes/Node.h
    src/parser/Parser.cpp
)
file(GLOB NODE_HEADERS "include/parser/nodes/*.h")
target_sources(Parser PUBLIC ${NODE_HEADERS})
target_include_directories(Parser PUBLIC include)

target_link_libraries(Parser PUBLIC Lexer)

add_library(Interpreter
    include/interpreter/NodeVisitor.h
    include/interpreter/Interpreter.h
    include/interpreter/InterpreterException.h
    src/interpreter/Interpreter.cpp
)
target_include_directories(Interpreter PUBLIC include)

target_link_libraries(Interpreter PUBLIC Parser)

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

add_executable(ParserTests tests/ParserTests.cpp)
target_link_libraries(ParserTests gtest_main Parser)
gtest_discover_tests(ParserTests)
target_link_libraries(ParserTests gtest_main Parser Lexer)

add_executable(InterpreterTests tests/InterpreterTests.cpp)
target_link_libraries(InterpreterTests gtest_main Interpreter)
gtest_discover_tests(InterpreterTests)
target_link_libraries(InterpreterTests gtest_main Interpreter Parser Lexer)