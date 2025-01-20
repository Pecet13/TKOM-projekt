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

TEST(InterpreterTests, PrintIntDefault)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a;\n"
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
    
    EXPECT_EQ(ss.str(), "0");
}

TEST(InterpreterTests, PrintFloatDefault)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a;\n"
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
    
    EXPECT_EQ(ss.str(), "0");
}

TEST(InterpreterTests, PrintStringDefault)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a;\n"
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
    
    EXPECT_EQ(ss.str(), "");
}

TEST(InterpreterTests, PrintBoolDefault)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a;\n"
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
    
    EXPECT_EQ(ss.str(), "false");
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

TEST(InterpreterTests, AssignToConst)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 4.4;\n"
                            "a = 11;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntPlusInt)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 + 8;\n"
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
    
    EXPECT_EQ(ss.str(), "12");
}

TEST(InterpreterTests, IntPlusFloat)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 + 8.5;\n"
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
    
    EXPECT_EQ(ss.str(), "12");
}

TEST(InterpreterTests, IntPlusString)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 + \"6\";\n"
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
    
    EXPECT_EQ(ss.str(), "10");
}

TEST(InterpreterTests, IntPlusString2)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 + \"asd\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntPlusBool)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = -4 + false;\n"
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
    
    EXPECT_EQ(ss.str(), "-4");
}

TEST(InterpreterTests, IntOverflow)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 2147483000 + 2000000000;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FloatPlusInt)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 1.23 + 8;\n"
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
    
    EXPECT_EQ(ss.str(), "9.23");
}

TEST(InterpreterTests, FloatPlusFloat)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 4.4 + 8.5;\n"
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
    
    EXPECT_EQ(ss.str(), "12.9");
}

TEST(InterpreterTests, FloatPlusString)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 4.5 + \"6.1\";\n"
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
    
    EXPECT_EQ(ss.str(), "10.6");
}

TEST(InterpreterTests, FloatPlusString2)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 1.3 + \"asd\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FloatPlusBool)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = -2.23 + true;\n"
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
    
    EXPECT_EQ(ss.str(), "-1.23");
}

TEST(InterpreterTests, StringPlusInt)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"I have \" + 2 + \" cars\";\n"
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
    
    EXPECT_EQ(ss.str(), "I have 2 cars");
}

TEST(InterpreterTests, StringPlusFloat)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"Value is equal: \" + 8.5;\n"
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
    
    EXPECT_EQ(ss.str(), "Value is equal: 8.500000");
}

TEST(InterpreterTests, StringPlusString)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"very \" + \"good\";\n"
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
    
    EXPECT_EQ(ss.str(), "very good");
}

TEST(InterpreterTests, StringPlusBool)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"This statement is \" + false;\n"
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
    
    EXPECT_EQ(ss.str(), "This statement is false");
}

TEST(InterpreterTests, BoolPlus)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true + 1;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntMinusInt)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 - 8;\n"
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
    
    EXPECT_EQ(ss.str(), "-4");
}

TEST(InterpreterTests, IntMinusFloat)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 - 8.8;\n"
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
    
    EXPECT_EQ(ss.str(), "-4");
}

TEST(InterpreterTests, IntMinusString)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 - \"6\";\n"
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
    
    EXPECT_EQ(ss.str(), "-2");
}

TEST(InterpreterTests, IntMinusString2)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 4 - \"asd\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntMinusBool)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = -4 - !false;\n"
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
    
    EXPECT_EQ(ss.str(), "-5");
}

TEST(InterpreterTests, FloatMinusInt)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = -1.23 - 2;\n"
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
    
    EXPECT_EQ(ss.str(), "-3.23");
}

TEST(InterpreterTests, FloatMinusFloat)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 4.4 - 2.2;\n"
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
    
    EXPECT_EQ(ss.str(), "2.2");
}

TEST(InterpreterTests, FloatMinusString)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 4.5 - \"0.1\";\n"
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
    
    EXPECT_EQ(ss.str(), "4.4");
}

TEST(InterpreterTests, FloatMinusString2)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 1.3 - \"asd\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FloatMinusBool)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = -2.23 - !true;\n"
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
    
    EXPECT_EQ(ss.str(), "-2.23");
}

TEST(InterpreterTests, StringMinus)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"I have \" - 2;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, BoolMinus)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true - 2;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntMultply)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 14 * \"10\";\n"
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
    
    EXPECT_EQ(ss.str(), "140");
}

TEST(InterpreterTests, FloatMultply)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 2.5 * \"1.5\";\n"
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
    
    EXPECT_EQ(ss.str(), "3.75");
}

TEST(InterpreterTests, FloatOverflow)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 123423904.1223242 * 99999999 * 99999999 * 9999999 * 723456399;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StringMultiply)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"hello\" * 2;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, BoolMultiply)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true * 2;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntDivide)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 14 / \"2\";\n"
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
    
    EXPECT_EQ(ss.str(), "7");
}

TEST(InterpreterTests, FloatDivide)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 7.5 / 5;\n"
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
    
    EXPECT_EQ(ss.str(), "1.5");
}

TEST(InterpreterTests, DivisionByZero)
{
    std::string input = "int main()\n"
                        "[\n"
                            "float a = 7.5 / 0;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StringDivide)
{
    std::string input = "int main()\n"
                        "[\n"
                            "string a = \"hello\" / 2;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, BoolDivide)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true / 2;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1 == 1;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FloatEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1.5 == \"1.5\";\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, BoolEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true == true;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, StringEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = \"hello\" == \"hello\";\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, IntNotEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1 != 0;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FloatNotEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1.5 != \"1.56\";\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, BoolNotEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true != false;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, StringNotEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = \"hello\" != \"hello world\";\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, IntGreater)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1 > -1;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FloatGreater)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1.5 > 0.5;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, BoolGreater)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true > true;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StringGreater)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = \"hello\" > \"hello\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntGreaterEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1 >= -1;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FloatGreaterEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1.5 >= 0.5;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, BoolGreaterEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true >= true;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StringGreaterEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = \"hello\" >= \"hello\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntLess)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1 < 5;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FloatLess)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1.5 < 2.5;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, BoolLess)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true < true;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StringLess)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = \"hello\" < \"hello\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, IntLessEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1 <= 4;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FloatLessEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = 1.5 <= 3.5;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, BoolLessEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = true <= true;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StringLessEqual)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = \"hello\" <= \"hello\";\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, And)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = (1.5 <= 3.5) and true;\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, Or)
{
    std::string input = "int main()\n"
                        "[\n"
                            "bool a = (1.5 <= 3.5) or (2 == 3);\n"
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
    
    EXPECT_EQ(ss.str(), "true");
}

TEST(InterpreterTests, FunctionDeclaration)
{
    std::string input = "int fun(int a)\n"
                        "[\n"
                            "return a;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "int a = fun(4);\n"
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

TEST(InterpreterTests, FunctionDeclarationIncorrectParameterNumber)
{
    std::string input = "int fun(int a)\n"
                        "[\n"
                            "return a;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "int a = fun(4, 5);\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FunctionDeclarationIncorrectParameterType)
{
    std::string input = "int fun(int a)\n"
                        "[\n"
                            "return a;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "int a = fun(true);\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FunctionDeclarationNonVoidNoReturn)
{
    std::string input = "float fun(int a)\n"
                        "[\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "int a = fun(4);\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FunctionDeclarationVoid)
{
    std::string input = "void fun(int a)\n"
                        "[\n"
                            "print(a);\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "fun(3);\n"
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

    EXPECT_EQ(ss.str(), "3");
}

TEST(InterpreterTests, MainWrongType)
{
    std::string input = "void main()\n"
                        "[\n"
                            "int a = 3;\n"
                            "print(a);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    
    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, FunctionDeclarationModifyingArgument)
{
    std::string input = "int fun(int a)\n"
                        "[\n"
                            "a = a + 1;\n"
                            "return a;"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "mut int a = 2;\n"
                            "mut int b = fun(a);\n"
                            "print(a);\n"
                            "print(\"\\n\");\n"
                            "print(b);\n"
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

    EXPECT_EQ(ss.str(), "2\n3");
}

TEST(InterpreterTests, FunctionWithRecursion)
{
    std::string input = "int silnia(int n)\n"
                        "[\n"
                            "if (n == 0)\n"
                            "[\n"
                                "return 1;\n"
                            "]\n"
                            "else\n"
                            "[\n"
                                "return n * silnia(n-1);\n"
                            "]\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "int a = silnia(5);\n"
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

    EXPECT_EQ(ss.str(), "120");
}

TEST(InterpreterTests, WhileLoop)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 10;\n"
                            "mut int b = 1;\n"
                            "while (b < a)\n"
                            "[\n"
                                "print(b);\n"
                                "print(\"\\n\");\n"
                                "b = b + 1;\n"
                            "]\n"
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

    EXPECT_EQ(ss.str(), "1\n2\n3\n4\n5\n6\n7\n8\n9\n");
}

TEST(InterpreterTests, WhileLoopMaxIterations)
{
    std::string input = "int main()\n"
                        "[\n"
                            "int a = 10;\n"
                            "mut int b = 1;\n"
                            "while (true)\n"
                            "[\n"
                                "b = b + 1;\n"
                            "]\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram(); 

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StructureCreation)
{
    std::string input = "struct Person\n"
                        "[\n"
                            "string name;\n"
                            "string surname;\n"
                            "mut int age;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "new Person person1(\"Adam\", \"Nowak\", 35);\n"
                            "print(\"Name: \" + person1.name + \"\\n\");\n"
                            "print(\"Surname: \" + person1.surname + \"\\n\");\n"
                            "print(\"Age: \" + person1.age + \"\\n\");"
                            "person1.age = 36;"
                            "print(\"New age: \" + person1.age + \"\\n\");"
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

    EXPECT_EQ(ss.str(), "Name: Adam\nSurname: Nowak\nAge: 35\nNew age: 36\n");
}

TEST(InterpreterTests, StructureChangeConstField)
{
    std::string input = "struct Person\n"
                        "[\n"
                            "string name;\n"
                            "string surname;\n"
                            "mut int age;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "new Person person1(\"Adam\", \"Nowak\", 35);\n"
                            "person1.name = \"Stefan\";"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, NestedStructure)
{
    std::string input = "struct Person\n"
                        "[\n"
                            "string name;\n"
                            "string surname;\n"
                            "mut int age;\n"
                        "]\n"
                        "struct Lecture\n"
                        "[\n"
                            "string name;\n"
                            "int roomNumber;\n"
                            "Person lecturer;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "new Person person1(\"Adam\", \"Nowak\", 35);\n"
                            "new Lecture lecture(\"Matematyka\", 123, person1);\n"
                            "lecture.lecturer.age = 36;\n"
                            "print(\"Lecture name: \" + lecture.name + \"\\n\");"
                            "print(\"Room number: \" + lecture.roomNumber + \"\\n\");"
                            "print(\"Lecturer: \" + lecture.lecturer.name + \" \" + lecture.lecturer.surname + \"\\n\");"
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

    EXPECT_EQ(ss.str(), "Lecture name: Matematyka\nRoom number: 123\nLecturer: Adam Nowak\n");
}


TEST(InterpreterTests, StructureChangeIncorrectParameterNumber)
{
    std::string input = "struct Person\n"
                        "[\n"
                            "string name;\n"
                            "string surname;\n"
                            "mut int age;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "new Person person1(\"Adam\", \"Nowak\", 35, true);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, StructureChangeIncorrectParameterType)
{
    std::string input = "struct Person\n"
                        "[\n"
                            "string name;\n"
                            "string surname;\n"
                            "mut int age;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "new Person person1(\"Adam\", 6, 35);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);
    Interpreter interpreter;

    std::unique_ptr<ProgramNode> program = parser.parseProgram();

    EXPECT_THROW(program->accept(interpreter), InterpreterException);
}

TEST(InterpreterTests, VariantWithMatch)
{
    std::string input = "variant[int, string] a;\n"
                        "void printVariant(variant[int, string] v)\n"
                        "[\n"
                            "match v\n"
                            "[\n"
                                "int i\n"
                                "[\n"
                                    "print(\"Int value: \" + i);\n"
                                "]\n"
                                "string i\n"
                                "[\n"
                                    "print(\"String value: \" + i);\n"
                                "]\n"
                                "default\n"
                                "[\n"
                                    "print(\"Different value\");\n"
                                "]\n"
                            "]\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "a = \"hello\";\n"
                            "printVariant(a);\n"
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

    EXPECT_EQ(ss.str(), "String value: hello");
}