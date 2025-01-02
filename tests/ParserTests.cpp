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

TEST(ParserTests, VariableDeclarationInt)
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

TEST(ParserTests, VariableDeclarationString)
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

TEST(ParserTests, VariableDeclarationMissingSemicolon)
{
    std::string input = "string s = \"hello\"";
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

TEST(ParserTests, Match)
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