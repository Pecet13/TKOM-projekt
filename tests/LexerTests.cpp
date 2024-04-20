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