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
    std::string input = "string\n#comment\nint #int\nfloat bool";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_STRING);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_INT);
    EXPECT_EQ(token.position.getLine(), 3);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_FLOAT);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_BOOL);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 7);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EOF);
    EXPECT_EQ(token.position.getLine(), 4);
    EXPECT_EQ(token.position.getColumn(), 12);
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

TEST(LexerTests, NumberTooBig)
{
    std::string input = "12345678910111213141516";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, NumberStartingWithZeros)
{
    std::string input = "0001";
    std::stringstream source{input};
    Lexer lexer(source);

    EXPECT_THROW(lexer.nextToken(), LexerException);
}

TEST(LexerTests, NumberStartingWithZeros2)
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
    std::string input = "== != == = >= = <= < >";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_NOT_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 4);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 7);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 10);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_GREATER_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 12);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_ASSIGN);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 15);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_LESS_EQUAL);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 17);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_LESS);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 20);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::T_GREATER);
    EXPECT_EQ(token.position.getLine(), 1);
    EXPECT_EQ(token.position.getColumn(), 22);
}