#include "../include/parser/Parser.h"
#include "../include/interpreter/Interpreter.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(InterpreterTests, NoMainProgram)
{
    std::string input = "float f;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}