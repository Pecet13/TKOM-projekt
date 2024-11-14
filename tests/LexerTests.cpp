#include "../include/lexer/Lexer.h"
#include <gtest/gtest.h>

TEST(LexerTests, SingleBracketOpen)
{
    std::string input = "[";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, MultipleBrackets)
{
    std::string input = "[]\n()";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_CLOSE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 2);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BRACKET_OPEN);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BRACKET_CLOSE);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 2);
}

TEST(LexerTests, SingleKeyword)
{
    std::string input = "string";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, ID)
{
    std::string input = "my_string";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<std::string>(token.value), "my_string");
}

TEST(LexerTests, MultipleKeywordsAndComments)
{
    std::string input = "string\n#comment\nvoid and or #int\nfloat bool true false if else match while";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_VOID);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_AND);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 6);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_OR);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 10);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BOOL);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 7);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_TRUE);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 12);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FALSE);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 17);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_IF);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 23);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ELSE);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 26);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_MATCH);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 31);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_WHILE);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 37);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EOF);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 42);
}

TEST(LexerTests, IDTooLong)
{
    std::string input = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, LongerMaxID)
{
    std::string input = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::stringstream source{input};
    Lexer lexer(source, 200);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<std::string>(token.value), input);
}

TEST(LexerTests, Zero)
{
    std::string input = "0";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<int>(token.value), 0);
}

TEST(LexerTests, IntValue)
{
    std::string input = "234";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<int>(token.value), 234);
}

TEST(LexerTests, FloatValue)
{
    std::string input = "234.567";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_NEAR(std::get<float>(token.value), 234.567, 1e-5);
}

TEST(LexerTests, FloatValueStartingWDot)
{
    std::string input = ".567";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_NEAR(std::get<float>(token.value), .567, 1e-5);
}

TEST(LexerTests, FloatValueStartingWZero)
{
    std::string input = "0.567";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_NEAR(std::get<float>(token.value), .567, 1e-5);
}

TEST(LexerTests, Dot)
{
    std::string input = ".";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_DOT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, IntOverflow)
{
    std::string input = "12345678910111213141516";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, FloatOverflow)
{
    std::string input = "12345678910111213141.516";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, FloatOverflow2)
{
    std::string input = "123.456789101112131415161718192021222324252627282930";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, IntStartingWithZeros)
{
    std::string input = "0001";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, FloatStartingWithZeros)
{
    std::string input = "00.01";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, String)
{
    std::string input = "\"Ala ma kota\"";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<std::string>(token.value), "Ala ma kota");
}

TEST(LexerTests, ArithmeticSigns)
{
    std::string input = "+ - * /";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_PLUS);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_MINUS);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 3);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASTERISK);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 5);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SLASH);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 7);
}

TEST(LexerTests, Equal)
{
    std::string input = "==";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, NotEqual)
{
    std::string input = "!=";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_NOT_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, MultipleComparisonSigns)
{
    std::string input = "==!====>==<=<>!";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_NOT_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 3);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 5);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 7);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_GREATER_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 8);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 10);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_LESS_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 11);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_LESS);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 13);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_GREATER);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 14);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_NOT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 15);
}

TEST(LexerTests, VariableDeclaration)
{
    std::string input = "int x=5;";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 5);
    EXPECT_EQ(std::get<std::string>(token.value), "x");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 6);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 7);
    EXPECT_EQ(std::get<int>(token.value), 5);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 8);
}

TEST(LexerTests, StructDeclaration)
{
    std::string input = ("struct Item\n"
                        "[\n"
                        "string name;\n"
                        "mut float value;\n"
                        "]");
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRUCT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 8);
    EXPECT_EQ(std::get<std::string>(token.value), "Item");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 8);
    EXPECT_EQ(std::get<std::string>(token.value), "name");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 12);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_MUT);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 5);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 11);
    EXPECT_EQ(std::get<std::string>(token.value), "value");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 16);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_CLOSE);
    EXPECT_EQ(token.position.getLine(), 5);
    EXPECT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, StructCreation)
{
    std::string input = ("Item my_item(\"bread\", 3.45);\n"
                        "my_item.value = 4.56;");
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<std::string>(token.value), "Item");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 6);
    EXPECT_EQ(std::get<std::string>(token.value), "my_item");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BRACKET_OPEN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 13);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 14);
    EXPECT_EQ(std::get<std::string>(token.value), "bread");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_COMMA);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 21);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT_VALUE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 23);
    EXPECT_NEAR(std::get<float>(token.value), 3.45, 1e-5);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BRACKET_CLOSE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 27);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 28);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<std::string>(token.value), "my_item");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_DOT);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 8);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 9);
    EXPECT_EQ(std::get<std::string>(token.value), "value");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 15);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT_VALUE);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 17);
    EXPECT_NEAR(std::get<float>(token.value), 4.56, 1e-5);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 21);
}

TEST(LexerTests, VariantDeclaration)
{
    std::string input = "variant[int, string] a;";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_VARIANT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 8);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 9);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_COMMA);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 12);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 14);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_CLOSE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 20);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 22);
    EXPECT_EQ(std::get<std::string>(token.value), "a");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 23);
}

TEST(LexerTests, Function)
{
    std::string input("int fun(int a)\n"
                    "[\n"
                    "a = a + 1;\n"
                    "return a;\n"
                    "]");
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 5);
    EXPECT_EQ(std::get<std::string>(token.value), "fun");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BRACKET_OPEN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 8);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 9);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 13);
    EXPECT_EQ(std::get<std::string>(token.value), "a");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BRACKET_CLOSE);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 14);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    EXPECT_EQ(token.position.getLine(), 2);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 1);
    EXPECT_EQ(std::get<std::string>(token.value), "a");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 3);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 5);
    EXPECT_EQ(std::get<std::string>(token.value), "a");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_PLUS);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 7);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT_VALUE);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 9);
    EXPECT_EQ(std::get<int>(token.value), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 10);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_RETURN);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ID);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 8);
    EXPECT_EQ(std::get<std::string>(token.value), "a");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SEMICOLON);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 9);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_SQUARE_CLOSE);
    EXPECT_EQ(token.position.getLine(), 5);
    EXPECT_EQ(token.position.getColumn(), 1);
}