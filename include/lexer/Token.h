#ifndef TOKEN_H
#define TOKEN_H

#include "Position.h"
#include <variant>
#include <string>

enum TokenType
{
    T_IF,
    T_ELSE,
    T_WHILE,
    T_RETURN,
    T_MATCH,
    T_DEFAULT,
    T_NEW,

    T_VOID,
    T_INT,
    T_FLOAT,
    T_STRING,
    T_BOOL,
    T_STRUCT,
    T_VARIANT,

    T_TRUE,
    T_FALSE,
    T_NOT,
    T_AND,
    T_OR,

    T_INT_VALUE,
    T_FLOAT_VALUE,
    T_STRING_VALUE,

    T_COMMA,
    T_SEMICOLON,
    T_DOT,

    T_BRACKET_OPEN,
    T_BRACKET_CLOSE,
    T_SQUARE_OPEN,
    T_SQUARE_CLOSE,

    T_PLUS,
    T_MINUS,
    T_ASTERISK,
    T_SLASH,

    T_EQUAL,
    T_NOT_EQUAL,
    T_GREATER,
    T_GREATER_EQUAL,
    T_LESS,
    T_LESS_EQUAL,

    T_MUT,
    T_ASSIGN,
    T_ID,

    T_EOF,
    T_UNKNOWN
};


struct Token
{
    TokenType type;
    Position position;
    std::variant<int, float, std::string> value;

    Token(TokenType type, const Position& pos, int intVal);
    Token(TokenType type, const Position& pos, float floatVal);
    Token(TokenType type, const Position& pos, const std::string& stringVal);
    Token(TokenType type, const Position& pos) : type(type), position(pos) {}
};

#endif