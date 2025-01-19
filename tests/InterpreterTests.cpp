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

TEST(InterpreterTests, PrintInt)
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

TEST(InterpreterTests, PrintFloat)
{
    std::string input = "int main()\n"
                        "[\n"
                            "print(1.23);\n"
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
    
    EXPECT_EQ(ss.str(), "1.23");
}

TEST(InterpreterTests, PrintString)
{
    std::string input = "int main()\n"
                        "[\n"
                            "print(\"Hello world\");\n"
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
    
    EXPECT_EQ(ss.str(), "Hello world");
}

TEST(InterpreterTests, PrintBool)
{
    std::string input = "int main()\n"
                        "[\n"
                            "print(false);\n"
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
    
    EXPECT_EQ(ss.str(), "false");
}

TEST(InterpreterTests, PrintVariable)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4;\n"
                            "print(a);\n"
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
    
    EXPECT_EQ(ss.str(), "4");
}

TEST(InterpreterTests, Assign)
{
    std::string input = "int main()\n"
                        "[\n"
                            "mut int a = 4;\n"
                            "a = 9;\n"
                            "print(a);\n"
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
    
    EXPECT_EQ(ss.str(), "9");
}