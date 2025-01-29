#include "../include/parser/Parser.h"
#include <gtest/gtest.h>


TEST(ParserTests, VariableDeclarationFloat)
{
    std::string input = "float f;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: float, identifier: f)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationFloatMut)
{
    std::string input = "mut float f;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: float, identifier: f)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationFloatWValue)
{
    std::string input = "float f = 5.3;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: float, identifier: f)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------FloatLiteral(value: 5.300000)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationFloatMutWValue)
{
    std::string input = "mut float f = -5.3;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: float, identifier: f)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term(negation: arithmetical)\n"
                           "--------FloatLiteral(value: 5.300000)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationInt)
{
    std::string input = "int x;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: int, identifier: x)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationIntMut)
{
    std::string input = "int x;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: int, identifier: x)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationIntWValue)
{
    std::string input = "int x=5;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: int, identifier: x)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------IntLiteral(value: 5)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationBool)
{
    std::string input = "bool a;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: bool, identifier: a)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationBoolMut)
{
    std::string input = "mut bool a;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: bool, identifier: a)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationBoolWValue)
{
    std::string input = "bool a = true;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: bool, identifier: a)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------BoolLiteral(value: true)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationBoolMutWValue)
{
    std::string input = "mut bool a = false;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: bool, identifier: a)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------BoolLiteral(value: false)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationBoolMutWValue2)
{
    std::string input = "mut bool a = (2 > 3);";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: bool, identifier: a)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------Expression\n"
                           "---------AndExpression\n"
                           "----------Comparison\n"
                           "-----------AddExpression\n"
                           "------------MultExpression\n"
                           "-------------Term\n"
                           "--------------IntLiteral(value: 2)\n"
                           "-----------operator: >\n"
                           "-----------AddExpression\n"
                           "------------MultExpression\n"
                           "-------------Term\n"
                           "--------------IntLiteral(value: 3)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationBoolWNegation)
{
    std::string input = "bool a = true;\n"
                        "bool b = !a;\n";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: bool, identifier: a)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------BoolLiteral(value: true)\n"
                           "-VariableDeclaration(type: bool, identifier: b)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term(negation: logical)\n"
                           "--------FieldOrFunCall(identifier: a)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationString)
{
    std::string input = "string s;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: string, identifier: s)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationStringMut)
{
    std::string input = "mut string s;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: string, identifier: s)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationStringWValue)
{
    std::string input = "string s = \"hello\";";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(type: string, identifier: s)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------StringLiteral(value: \"hello\")\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationStringMutWValue)
{
    std::string input = "mut string s = \"hello\";";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariableDeclaration(mut, type: string, identifier: s)\n"
                           "--Expression\n"
                           "---AndExpression\n"
                           "----Comparison\n"
                           "-----AddExpression\n"
                           "------MultExpression\n"
                           "-------Term\n"
                           "--------StringLiteral(value: \"hello\")\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariableDeclarationMissingSemicolon)
{
    std::string input = "string s = \"hello\"";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, VariableDeclarationTwoTypes)
{
    std::string input = "string int s = \"hello\";";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, StructDeclaration)
{
    std::string input = "struct Item\n"
                        "[\n"
                            "string name;\n"
                            "mut float value;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-StructDeclaration(identifier: Item)\n"
                           "--StructFieldList\n"
                           "---StructField(type: string, identifier: name)\n"
                           "---StructField(mut, type: float, identifier: value)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, StructDeclarationMissingSemicolon)
{
    std::string input = "struct Item\n"
                        "[\n"
                            "string name\n"
                            "mut float value;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, StructDeclarationNoFields)
{
    std::string input = "struct Item\n"
                        "[\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, VariantDeclaration)
{
    std::string input = "variant[int, string] a;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariantDeclaration(identifier: a)\n"
                           "--Variant\n"
                           "---int\n"
                           "---string\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariantDeclarationNestedVariant)
{
    std::string input = "variant[int, string, variant[bool, float]] a;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariantDeclaration(identifier: a)\n"
                           "--Variant\n"
                           "---int\n"
                           "---string\n"
                           "---Variant\n"
                           "----bool\n"
                           "----float\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, VariantDeclarationNoType)
{
    std::string input = "variant[] a;";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, StructDeclarationWVariantField)
{
    std::string input = "struct Variable\n"
                        "[\n"
                            "string name;\n"
                            "variant[bool, int, float, string] value;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-StructDeclaration(identifier: Variable)\n"
                           "--StructFieldList\n"
                           "---StructField(type: string, identifier: name)\n"
                           "---StructField\n"
                           "----VariantDeclaration(identifier: value)\n"
                           "-----Variant\n"
                           "------bool\n"
                           "------int\n"
                           "------float\n"
                           "------string\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, FunctionDeclaration)
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
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: int, identifier: silnia)\n"
                           "--ParameterList\n"
                           "---Parameter(type: int, identifier: n)\n"
                           "--Block\n"
                           "---IfStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: n)\n"
                           "-------operator: ==\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n"
                           "----Block\n"
                           "-----ReturnStatement\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------IntLiteral(value: 1)\n"
                           "---Else\n"
                           "----Block\n"
                           "-----ReturnStatement\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------FieldOrFunCall(identifier: n)\n"
                           "-----------operator: *\n"
                           "-----------Term\n"
                           "------------FieldOrFunCall(identifier: silnia)\n"
                           "-------------ArgumentList\n"
                           "--------------Expression\n"
                           "---------------AndExpression\n"
                           "----------------Comparison\n"
                           "-----------------AddExpression\n"
                           "------------------MultExpression\n"
                           "-------------------Term\n"
                           "--------------------FieldOrFunCall(identifier: n)\n"
                           "------------------operator: -\n"
                           "------------------MultExpression\n"
                           "-------------------Term\n"
                           "--------------------IntLiteral(value: 1)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, FunctionDeclarationNoBlock)
{
    std::string input = "int silnia(int n)\n";

    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, FunctionDeclarationNoParameters)
{
    std::string input = "string hello_world()\n"
                        "[\n"
                            "return \"Hello world\";\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: string, identifier: hello_world)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------StringLiteral(value: \"Hello world\")\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, FunctionDeclarationVoid)
{
    std::string input = "void fun(int a)\n"
                        "[\n"
                            "a = a + 1;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: void, identifier: fun)\n"
                           "--ParameterList\n"
                           "---Parameter(type: int, identifier: a)\n"
                           "--Block\n"
                           "---Assignment\n"
                           "----FieldOrFunCall(identifier: a)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "--------operator: +\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 1)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, FunctionDeclarationMultipleParameters)
{
    std::string input = "void fun(int a, int b)\n"
                        "[\n"
                            "a = a + b;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: void, identifier: fun)\n"
                           "--ParameterList\n"
                           "---Parameter(type: int, identifier: a)\n"
                           "---Parameter(type: int, identifier: b)\n"
                           "--Block\n"
                           "---Assignment\n"
                           "----FieldOrFunCall(identifier: a)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "--------operator: +\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: b)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, ReturnStatementNoExpression)
{
    std::string input = "void fun()\n"
                        "[\n"
                            "return;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: void, identifier: fun)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---ReturnStatement\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, IfStatement)
{
    std::string input = "bool positive(int n)\n"
                        "[\n"
                            "if (n > 0)\n"
                            "[\n"
                                "return true;\n"
                            "]\n"
                            "else\n"
                            "[\n"
                                "return false;\n"
                            "]\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: bool, identifier: positive)\n"
                           "--ParameterList\n"
                           "---Parameter(type: int, identifier: n)\n"
                           "--Block\n"
                           "---IfStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: n)\n"
                           "-------operator: >\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n"
                           "----Block\n"
                           "-----ReturnStatement\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------BoolLiteral(value: true)\n"
                           "---Else\n"
                           "----Block\n"
                           "-----ReturnStatement\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------BoolLiteral(value: false)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, IfStatementNoElse)
{
    std::string input = "int main()\n"
                        "[\n"
                            "mut int a = 5;\n"
                            "if (a > 0)\n"
                            "[\n"
                                "a = 2 * a;\n"
                            "]\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---VariableDeclaration(mut, type: int, identifier: a)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 5)\n"
                           "---IfStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "-------operator: >\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n"
                           "----Block\n"
                           "-----Assignment\n"
                           "------FieldOrFunCall(identifier: a)\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------IntLiteral(value: 2)\n"
                           "-----------operator: *\n"
                           "-----------Term\n"
                           "------------FieldOrFunCall(identifier: a)\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, IfStatementNoExpression)
{
    std::string input = "int main()\n"
                        "[\n"
                            "mut int a = 5;\n"
                            "if ()\n"
                            "[\n"
                                "a = 2 * a;\n"
                            "]\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, IfStatementUnclosedBlock)
{
    std::string input = "int main()\n"
                        "[\n"
                            "mut int a = 5;\n"
                            "if (a > 0)\n"
                            "[\n"
                                "a = 2 * a;\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, WhileStatement)
{
    std::string input = "int main()\n"
                        "[\n"
                            "mut int a = 5;\n"
                            "while (a > 0)\n"
                            "[\n"
                                "a = a - 1;\n"
                            "]\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---VariableDeclaration(mut, type: int, identifier: a)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 5)\n"
                           "---WhileStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "-------operator: >\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n"
                           "----Block\n"
                           "-----Assignment\n"
                           "------FieldOrFunCall(identifier: a)\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------FieldOrFunCall(identifier: a)\n"
                           "----------operator: -\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------IntLiteral(value: 1)\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, WhileStatementNoExpression)
{
    std::string input = "int main()\n"
                        "[\n"
                            "mut int a = 5;\n"
                            "while ()\n"
                            "[\n"
                                "a = a - 1;\n"
                            "]\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    EXPECT_THROW(parser.parseProgram(), ParserException);
}

TEST(ParserTests, StructCreation)
{
    std::string input = "int main()\n"
                        "[\n"
                            "new Item my_item(\"bread\", 3.45);\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---StructCreation(type: Item, identifier: my_item)\n"
                           "----ArgumentList\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------StringLiteral(value: \"bread\")\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------FloatLiteral(value: 3.450000)\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, FieldAccess)
{
    std::string input = "int main()\n"
                        "[\n"
                            "new Item my_item(\"bread\", 3.45);\n"
                            "my_item.value = 4.56;\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---StructCreation(type: Item, identifier: my_item)\n"
                           "----ArgumentList\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------StringLiteral(value: \"bread\")\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------FloatLiteral(value: 3.450000)\n"
                           "---Assignment\n"
                           "----FieldOrFunCall(identifier: my_item)\n"
                           "-----(identifier: value)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FloatLiteral(value: 4.560000)\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";
    
    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, NestedStruct)
{
    std::string input = "struct Date\n"
                        "[\n"
                            "int day;\n"
                            "int month;\n"
                            "int year;\n"
                        "]\n"
                        "struct Person\n"
                        "[\n"
                            "string name;\n"
                            "string surname;\n"
                            "Date birth_date;\n"
                        "]\n"
                        "int main()\n"
                        "[\n"
                            "new Date date1(1, 1, 1970);\n"
                            "new Person person1(\"Adam\", \"Nowak\", date1);\n"
                            "int birth_year = person1.birth_date.year;\n"
                            "return 0;\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-StructDeclaration(identifier: Date)\n"
                           "--StructFieldList\n"
                           "---StructField(type: int, identifier: day)\n"
                           "---StructField(type: int, identifier: month)\n"
                           "---StructField(type: int, identifier: year)\n"
                           "-StructDeclaration(identifier: Person)\n"
                           "--StructFieldList\n"
                           "---StructField(type: string, identifier: name)\n"
                           "---StructField(type: string, identifier: surname)\n"
                           "---StructField(type: Date, identifier: birth_date)\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---StructCreation(type: Date, identifier: date1)\n"
                           "----ArgumentList\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------IntLiteral(value: 1)\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------IntLiteral(value: 1)\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------IntLiteral(value: 1970)\n"
                           "---StructCreation(type: Person, identifier: person1)\n"
                           "----ArgumentList\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------StringLiteral(value: \"Adam\")\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------StringLiteral(value: \"Nowak\")\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------FieldOrFunCall(identifier: date1)\n"
                           "---VariableDeclaration(type: int, identifier: birth_year)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: person1)\n"
                           "-----------(identifier: birth_date)\n"
                           "------------(identifier: year)\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, MatchStatement)
{
    std::string input = "variant[int, string] a;\n"
                        "int main()\n"
                        "[\n"
                            "a = \"hello\";\n"
                            "match a\n"
                            "[\n"
                                "int i\n"
                                "[\n"
                                    "print(\"Wartosc typu int: \\n\");\n"
                                    "print(i);\n"
                                "]\n"
                                "string i\n"
                                "[\n"
                                    "print(\"Wartosc typu string: \\n\");\n"
                                    "print(i);\n"
                                "]\n"
                                "default\n"
                                "[\n"
                                    "print(\"Wartosc innego typu\");\n"
                                "]\n"
                            "]\n"
                            "return 0;\n"
                        "]";

    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-VariantDeclaration(identifier: a)\n"
                           "--Variant\n"
                           "---int\n"
                           "---string\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---Assignment\n"
                           "----FieldOrFunCall(identifier: a)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------StringLiteral(value: \"hello\")\n"
                           "---MatchStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "----MatchCase(type: int, identifier: i)\n"
                           "-----Block\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: print)\n"
                           "--------ArgumentList\n"
                           "---------Expression\n"
                           "----------AndExpression\n"
                           "-----------Comparison\n"
                           "------------AddExpression\n"
                           "-------------MultExpression\n"
                           "--------------Term\n"
                           "---------------StringLiteral(value: \"Wartosc typu int: \n\")\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: print)\n"
                           "--------ArgumentList\n"
                           "---------Expression\n"
                           "----------AndExpression\n"
                           "-----------Comparison\n"
                           "------------AddExpression\n"
                           "-------------MultExpression\n"
                           "--------------Term\n"
                           "---------------FieldOrFunCall(identifier: i)\n"
                           "----MatchCase(type: string, identifier: i)\n"
                           "-----Block\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: print)\n"
                           "--------ArgumentList\n"
                           "---------Expression\n"
                           "----------AndExpression\n"
                           "-----------Comparison\n"
                           "------------AddExpression\n"
                           "-------------MultExpression\n"
                           "--------------Term\n"
                           "---------------StringLiteral(value: \"Wartosc typu string: \n\")\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: print)\n"
                           "--------ArgumentList\n"
                           "---------Expression\n"
                           "----------AndExpression\n"
                           "-----------Comparison\n"
                           "------------AddExpression\n"
                           "-------------MultExpression\n"
                           "--------------Term\n"
                           "---------------FieldOrFunCall(identifier: i)\n"
                           "----MatchCase(default)\n"
                           "-----Block\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: print)\n"
                           "--------ArgumentList\n"
                           "---------Expression\n"
                           "----------AndExpression\n"
                           "-----------Comparison\n"
                           "------------AddExpression\n"
                           "-------------MultExpression\n"
                           "--------------Term\n"
                           "---------------StringLiteral(value: \"Wartosc innego typu\")\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, MatchStatement2)
{
    std::string input = "void fun(variant[int, string] a)\n"
                        "[\n"
                            "match a\n"
                            "[\n"
                                "int i\n"
                                "[\n"
                                    "do_something();\n"
                                "]\n"
                                "string i\n"
                                "[\n"
                                    "do_something_else();\n"
                                "]\n"
                                "default\n"
                                "[\n"
                                    "print(\"Default\");\n"
                                "]\n"
                            "]\n"
                        "]";
    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: void, identifier: fun)\n"
                           "--ParameterList\n"
                           "---Parameter(type: variant, identifier: a)\n"
                           "----Variant\n"
                           "-----int\n"
                           "-----string\n"
                           "--Block\n"
                           "---MatchStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "----MatchCase(type: int, identifier: i)\n"
                           "-----Block\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: do_something)\n"
                           "--------ArgumentList\n"
                           "----MatchCase(type: string, identifier: i)\n"
                           "-----Block\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: do_something_else)\n"
                           "--------ArgumentList\n"
                           "----MatchCase(default)\n"
                           "-----Block\n"
                           "------Assignment\n"
                           "-------FieldOrFunCall(identifier: print)\n"
                           "--------ArgumentList\n"
                           "---------Expression\n"
                           "----------AndExpression\n"
                           "-----------Comparison\n"
                           "------------AddExpression\n"
                           "-------------MultExpression\n"
                           "--------------Term\n"
                           "---------------StringLiteral(value: \"Default\")\n";

    EXPECT_EQ(program->toString(), expected);
}

TEST(ParserTests, ComplexProgram)
{
    std::string input = "int main()\n"
                        "[\n"
                            "new Item my_item(\"bread\", 3.45);\n"
                            "my_item.value = 4.56;\n"
                            "int a = 10;\n"
                            "mut int b = 1;\n"
                            "b = silnia(3);\n"
                            "while (b < a)\n"
                            "[\n"
                                "print(b);\n"
                                "b = b + 1;\n"
                            "]\n"
                            "return 0;\n"
                        "]";

    std::stringstream source{input};
    Lexer lexer(source);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> program = parser.parseProgram();
    std::string expected = "Program\n"
                           "-FunctionDeclaration(type: int, identifier: main)\n"
                           "--ParameterList\n"
                           "--Block\n"
                           "---StructCreation(type: Item, identifier: my_item)\n"
                           "----ArgumentList\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------StringLiteral(value: \"bread\")\n"
                           "-----Expression\n"
                           "------AndExpression\n"
                           "-------Comparison\n"
                           "--------AddExpression\n"
                           "---------MultExpression\n"
                           "----------Term\n"
                           "-----------FloatLiteral(value: 3.450000)\n"
                           "---Assignment\n"
                           "----FieldOrFunCall(identifier: my_item)\n"
                           "-----(identifier: value)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FloatLiteral(value: 4.560000)\n"
                           "---VariableDeclaration(type: int, identifier: a)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 10)\n"
                           "---VariableDeclaration(mut, type: int, identifier: b)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 1)\n"
                           "---Assignment\n"
                           "----FieldOrFunCall(identifier: b)\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: silnia)\n"
                           "-----------ArgumentList\n"
                           "------------Expression\n"
                           "-------------AndExpression\n"
                           "--------------Comparison\n"
                           "---------------AddExpression\n"
                           "----------------MultExpression\n"
                           "-----------------Term\n"
                           "------------------IntLiteral(value: 3)\n"
                           "---WhileStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: b)\n"
                           "-------operator: <\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------FieldOrFunCall(identifier: a)\n"
                           "----Block\n"
                           "-----Assignment\n"
                           "------FieldOrFunCall(identifier: print)\n"
                           "-------ArgumentList\n"
                           "--------Expression\n"
                           "---------AndExpression\n"
                           "----------Comparison\n"
                           "-----------AddExpression\n"
                           "------------MultExpression\n"
                           "-------------Term\n"
                           "--------------FieldOrFunCall(identifier: b)\n"
                           "-----Assignment\n"
                           "------FieldOrFunCall(identifier: b)\n"
                           "------Expression\n"
                           "-------AndExpression\n"
                           "--------Comparison\n"
                           "---------AddExpression\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------FieldOrFunCall(identifier: b)\n"
                           "----------operator: +\n"
                           "----------MultExpression\n"
                           "-----------Term\n"
                           "------------IntLiteral(value: 1)\n"
                           "---ReturnStatement\n"
                           "----Expression\n"
                           "-----AndExpression\n"
                           "------Comparison\n"
                           "-------AddExpression\n"
                           "--------MultExpression\n"
                           "---------Term\n"
                           "----------IntLiteral(value: 0)\n";
                           
    EXPECT_EQ(program->toString(), expected);
}