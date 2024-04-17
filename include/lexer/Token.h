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
    T_RETURN
    // ...
};


struct Token
{
    TokenType type;
    std::variant<int, float, std::string, bool> value;
    Position position;
};

#endif