#include "../include/lexer/Lexer.h"
#include <gtest/gtest.h>

TEST(LexerTests, SingleBracketOpen)
{
    std::string input = "[";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    ASSERT_EQ(token.position.getLine(), 1);
    ASSERT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, MultipleBrackets)
{
    std::string input = "[]\n()";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_SQUARE_OPEN);
    ASSERT_EQ(token.position.getLine(), 1);
    ASSERT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_SQUARE_CLOSE);
    ASSERT_EQ(token.position.getLine(), 1);
    ASSERT_EQ(token.position.getColumn(), 2);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_BRACKET_OPEN);
    ASSERT_EQ(token.position.getLine(), 2);
    ASSERT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_BRACKET_CLOSE);
    ASSERT_EQ(token.position.getLine(), 2);
    ASSERT_EQ(token.position.getColumn(), 2);
}

TEST(LexerTests, SingleKeyword)
{
    std::string input = "string";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_STRING);
    ASSERT_EQ(token.position.getLine(), 1);
    ASSERT_EQ(token.position.getColumn(), 1);
}

TEST(LexerTests, ID)
{
    std::string input = "my_string";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_ID);
    ASSERT_EQ(token.position.getLine(), 1);
    ASSERT_EQ(token.position.getColumn(), 1);
    ASSERT_EQ(std::get<std::string>(token.value), "my_string");
}

TEST(LexerTests, MultipleKeywordsAndComments)
{
    std::string input = "string\n#comment\nint #int\nfloat bool";
    std::stringstream source{input};
    Lexer lexer(source);

    Token token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_STRING);
    ASSERT_EQ(token.position.getLine(), 1);
    ASSERT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_INT);
    ASSERT_EQ(token.position.getLine(), 3);
    ASSERT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_FLOAT);
    ASSERT_EQ(token.position.getLine(), 4);
    ASSERT_EQ(token.position.getColumn(), 1);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_BOOL);
    ASSERT_EQ(token.position.getLine(), 4);
    ASSERT_EQ(token.position.getColumn(), 7);

    token = lexer.nextToken();
    ASSERT_EQ(token.type, TokenType::T_EOF);
    ASSERT_EQ(token.position.getLine(), 4);
    ASSERT_EQ(token.position.getColumn(), 12);
}