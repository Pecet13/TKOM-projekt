#include "lexer/Token.h"
#include "lexer/LexerException.h"

Token::Token(TokenType type, const Position& pos, int intVal)
    : type(type), position(pos)
{
    if (type == T_INT_VALUE)
    {
        value = intVal;
    }
    else
    {
        throw LexerException("invalid value type for token type: expected int", position);
    }
}

Token::Token(TokenType type, const Position& pos, float floatVal)
    : type(type), position(pos)
{
    if (type == T_FLOAT_VALUE)
    {
        value = floatVal;
    }
    else
    {
        throw LexerException("invalid value type for token type: expected float", position);
    }
}

Token::Token(TokenType type, const Position& pos, const std::string& stringVal)
    : type(type), position(pos)
{
    if (type == T_STRING_VALUE || type == T_ID)
    {
        value = stringVal;
    }
    else
    {
        throw LexerException("invalid value type for token type: expected string", position);
    }
}