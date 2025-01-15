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

TEST(InterpreterTests, PrintNumber)
{
    std::string input = "int main()\n"
                        "[\n"
                            "print(5);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::ostringstream ss;
    auto coutBuff = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    program->accept(interpreter);
    std::cout.rdbuf(coutBuff);
    
    EXPECT_EQ(ss.str(), "5");
}